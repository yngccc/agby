#include "common.h"

RaytracingAccelerationStructure scene : register(t0);
Texture2D<float3> position_texture : register(t1);
Texture2D<float3> normal_texture : register(t2);
Texture2D<float3> diffuse_textures[] : register(t3, space0);

struct vertex {
	float3 position;
	float3 normal;
	float2 uv;
};
StructuredBuffer<vertex> vertices : register(t3, space1);

struct material {
	float3 emissive;
	uint diffuse_texture_index;
};
StructuredBuffer<material> materials : register(t3, space2);

struct instance {
	uint vertex_offset;
	uint material_index;
};
StructuredBuffer<instance> instances : register(t3, space3);

RWTexture2D<float> output_texture : register(u0);

struct ray_pay_load {
	float3 diffuse;
	uint depth;
};

float3 offset_normal(float3 n, float3 n2) {
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

static const uint sample_count = 8;
static const uint depth_count = 2;

[shader("raygeneration")]
void ray_gen() {
	uint3 dimensions = DispatchRaysDimensions();
	uint3 ray_index = DispatchRaysIndex();

	float3 position = position_texture[ray_index.xy];
	float3 normal = normal_texture[ray_index.xy];

	uint rng_state = ray_index.y * dimensions.x + ray_index.x;
	rng_state = wang_hash(rng_state);

	float3 diffuse = { 0, 0, 0 };
	for (uint i = 0; i < sample_count; i += 1) {
		float2 xi = { rand_xorshift(rng_state), rand_xorshift(rng_state) };
		float3 offset = { sqrt(xi[0]) * cos(2 * PI * xi[1]), sqrt(xi[0]) * sin(2 * PI * xi[1]), sqrt(1 - xi[0]) };
		float3 dir = offset_normal(normal, offset);

		RayDesc ray;
		ray.Origin = position;
		ray.Direction = dir;
		ray.TMin = 0.0001;
		ray.TMax = 100;

		ray_pay_load payload;
		payload.diffuse = float3(0, 0, 0);
		payload.depth = 0;
		uint ray_flags = 0;
		TraceRay(scene, ray_flags, 0xFF, 0, 0, 0, ray, payload);
		
		diffuse += payload.diffuse;
	}
	diffuse = diffuse / sample_count;

	output_texture[ray_index.xy] = 0;
}

[shader("miss")]
void miss(inout ray_pay_load payload) {
	payload.diffuse = float3(0, 0, 0);
}

[shader("closesthit")]
void closest_hit(inout ray_pay_load payload, in BuiltInTriangleIntersectionAttributes attribs) {
	uint instance_index = InstanceIndex();
	uint primitive_index = PrimitiveIndex();

	instance inst = instances[instance_index];
	material mat = materials[inst.material_index];

	vertex vert1 = vertices[inst.vertex_offset + primitive_index * 3];
	vertex vert2 = vertices[inst.vertex_offset + primitive_index * 3 + 1];
	vertex vert3 = vertices[inst.vertex_offset + primitive_index * 3 + 2];

	float3 bary = float3(1.0 - attribs.barycentrics.x - attribs.barycentrics.y, attribs.barycentrics.x, attribs.barycentrics.y);
	float3 position = vert1.position * bary.x + vert2.position * bary.y + vert3.position * bary.z;
	float3 normal = vert1.normal * bary.x + vert2.normal * bary.y + vert3.normal * bary.z;
	float2 uv = vert1.uv * bary.x + vert2.uv * bary.y + vert3.uv * bary.z;
	
	Texture2D<float3> texture = diffuse_textures[mat.diffuse_texture_index];
	uint width, height;
	texture.GetDimensions(width, height);
	float3 diffuse = texture.Load(int3(width * uv.x, height * uv.y, 0));

	if (payload.depth < depth_count) {
		payload.depth += 1;
	}
	else {

	}
}