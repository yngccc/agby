#include "common.h"

RaytracingAccelerationStructure scene : register(t0);
Texture2D<float3> position_texture : register(t1);
Texture2D<float3> normal_texture : register(t2);

RWTexture2D<float> output_texture : register(u0);

struct ray_pay_load {
	float visibility;
};

float3 transform_normal(float3 n, float3 n2) {
	float3 t = cross(float3(0, 1, 0), n);
	if (length(t) == 0) {
		t = float3(-1, 0, 0);
	}
	else {
		t = normalize(t);
	}
	float3 b = cross(n, t);
	float3x3 tbn_mat = { t.x, b.x, n.x,
						 t.y, b.y, n.y,
						 t.z, b.z, n.z };
	return mul(tbn_mat, n2);
}

[shader("raygeneration")]
void ray_gen() {
	uint3 dimensions = DispatchRaysDimensions();
	uint3 ray_index = DispatchRaysIndex();

	float3 position = position_texture[ray_index.xy];
	float3 normal = normal_texture[ray_index.xy];

	uint rng_state = ray_index.y * dimensions.x + ray_index.x;
	rng_state = wang_hash(rng_state);

	float visibility = 0;
	const int sample_count = 8;
	for (int i = 0; i < sample_count; i += 1) {
		float2 xi = { rand_xorshift(rng_state), rand_xorshift(rng_state) };
		float3 offset = { sqrt(xi[0]) * cos(2 * PI * xi[1]), sqrt(xi[0]) * sin(2 * PI * xi[1]), sqrt(1 - xi[0]) };
		float3 dir = transform_normal(normal, offset);

		RayDesc ray;
		ray.Origin = position;
		ray.Direction = dir;
		ray.TMin = 0.0001;
		ray.TMax = 0.02;

		ray_pay_load payload;
		payload.visibility = 0;
		uint ray_flags = RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH | RAY_FLAG_SKIP_CLOSEST_HIT_SHADER;
		TraceRay(scene, ray_flags, 0xFF, 0, 0, 0, ray, payload);
		
		visibility += payload.visibility;
	}
	visibility = visibility / sample_count;

	output_texture[ray_index.xy] = visibility;
}

[shader("miss")]
void miss(inout ray_pay_load payload) {
	payload.visibility = 1;
}

[shader("anyhit")]
void any_hit(inout ray_pay_load payload, in BuiltInTriangleIntersectionAttributes attribs) {
}