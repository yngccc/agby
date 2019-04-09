#include "common.h"

RWTexture2DArray<float> output_textures : register(u0);

RaytracingAccelerationStructure scene : register(t0);
Texture2D<float3> position_texture : register(t1);
Texture2D<float3> normal_texture : register(t2);

cbuffer constants : register(b0) {
	light lights[4];
	uint light_count;
};

struct ray_pay_load {
	float visibility;
};

float trace_direct_light(light l, float3 position, float3 normal) {
	if (dot(normal, l.dir.xyz) > 0) {
		RayDesc ray;
		ray.Origin = position;
		ray.Direction = l.dir.xyz;
		ray.TMin = 0.001;
		ray.TMax = 1000;

		ray_pay_load payload;
		payload.visibility = 0;
		uint ray_flags = RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH | RAY_FLAG_SKIP_CLOSEST_HIT_SHADER;
		TraceRay(scene, ray_flags, 0xFF, 0, 0, 0, ray, payload);

		return payload.visibility;
	}
	else {
		return 0;
	}
}

float trace_sphere_light(light l, float3 position) {
	RayDesc ray;
	ray.Origin = position;
	ray.Direction = normalize(l.position.xyz - position);
	ray.TMin = 0.001;
	ray.TMax = length(l.position.xyz - position);

	ray_pay_load payload;
	payload.visibility = 0;
	uint ray_flags = RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH | RAY_FLAG_SKIP_CLOSEST_HIT_SHADER;
	TraceRay(scene, ray_flags, 0xFF, 0, 0, 0, ray, payload);

	return payload.visibility;
}

[shader("raygeneration")]
void ray_gen() {
	uint3 dimensions = DispatchRaysDimensions();
	uint3 ray_index = DispatchRaysIndex();

	float3 position = position_texture[ray_index.xy];
	float3 normal = normal_texture[ray_index.xy];

	for (uint i = 0; i < 4; i += 1) {
		float visibility = 1;
		if (i < light_count) {
			light l = lights[i];
			if (l.type == direct_light) {
				visibility = trace_direct_light(l, position, normal);
			}
			else if (l.type == sphere_light) {
				visibility = trace_sphere_light(l, position);
			}
		}
		output_textures[float3(ray_index.xy, i)] = visibility;
	}
}

[shader("miss")]
void miss(inout ray_pay_load payload) {
	payload.visibility = 1;
}

[shader("anyhit")]
void any_hit(inout ray_pay_load payload, in BuiltInTriangleIntersectionAttributes attribs) {
}