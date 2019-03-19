RWTexture2D<float> output : register(u0);

RaytracingAccelerationStructure scene : register(t0);
Texture2D<float3> position_texture : register(t1);
Texture2D<float3> normal_texture : register(t2);

cbuffer constants : register(b0) {
	float3 sun_light_dir;
};

struct ray_pay_load {
	float visibility;
};

static const float PI = 3.14159265358979323846;

[shader("raygeneration")]
void ray_gen() {
	uint3 dimensions = DispatchRaysDimensions();
	uint3 ray_index = DispatchRaysIndex();

	float3 position = position_texture[ray_index.xy];
	float3 normal = normal_texture[ray_index.xy];

	if (dot(normal, sun_light_dir) > 0) {
		RayDesc ray;
		ray.Origin = position;
		ray.Direction = sun_light_dir;
		ray.TMin = 0.0001;
		ray.TMax = 10000;

		ray_pay_load payload;
		payload.visibility = 0;
		uint ray_flags = RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH | RAY_FLAG_SKIP_CLOSEST_HIT_SHADER;
		TraceRay(scene, ray_flags, 0xFF, 0, 0, 0, ray, payload);

		output[ray_index.xy] = payload.visibility;
	}
	else {
		output[ray_index.xy] = 0;
	}
}

[shader("miss")]
void miss(inout ray_pay_load payload) {
	payload.visibility = 1;
}

[shader("anyhit")]
void any_hit(inout ray_pay_load payload, in BuiltInTriangleIntersectionAttributes attribs) {
}