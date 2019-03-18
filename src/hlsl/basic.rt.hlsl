RaytracingAccelerationStructure scene : register(t0);

cbuffer constants : register(b0) {
	float4x4 camera_to_world_mat;
	float camera_fovy;
};

RWTexture2D<float4> output : register(u0);

struct ray_pay_load {
	float3 color;
};

static const float PI = 3.14159265358979323846;

[shader("raygeneration")]
void ray_gen() {
	uint3 dimensions = DispatchRaysDimensions();
	uint3 ray_index = DispatchRaysIndex();

	float aspect_ratio = dimensions.x / (float)dimensions.y;
	float px = (2 * ((ray_index.x + 0.5) / dimensions.x) - 1) * tan(camera_fovy * 0.5) * aspect_ratio;
	float py = (2 * ((ray_index.y + 0.5) / dimensions.y) - 1) * tan(camera_fovy * 0.5);

	RayDesc ray;
	ray.Origin = mul(camera_to_world_mat, float4(0, 0, 0, 1)).xyz;
	ray.Direction = normalize(mul(camera_to_world_mat, float4(px, py, -1, 1)).xyz - ray.Origin);
	ray.TMin = 0;
	ray.TMax = 100000;

	ray_pay_load payload;
	TraceRay(scene, 0, 0xFF, 0, 0, 0, ray, payload);
	
	output[ray_index.xy] = float4(payload.color, 1);
}

[shader("miss")]
void miss(inout ray_pay_load payload) {
	payload.color = float3(1, 0, 0);
}

[shader("closesthit")]
void closest_hit(inout ray_pay_load payload, in BuiltInTriangleIntersectionAttributes attribs) {
	float distance = RayTCurrent();
	payload.color = float3(distance, distance, distance);
	//payload.color = WorldRayDirection();
	//float3 barycentrics = float3(1.0 - attribs.barycentrics.x - attribs.barycentrics.y, attribs.barycentrics.x, attribs.barycentrics.y);

	//const float3 A = float3(1, 0, 0);
	//const float3 B = float3(0, 1, 0);
	//const float3 C = float3(0, 0, 1);

	//payload.color = A * barycentrics.x + B * barycentrics.y + C * barycentrics.z;
}