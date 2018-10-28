float rng(inout uint state) {
  uint x = state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 15;
  state = x;
  return (float)x / 4294967295.0;
}

struct Material {
  uint type;
	float3 color;
	float refractive_index;
};

struct Sphere {
  float3 center;
	float radius;
	Material material;
};

struct Plane {
  float3 normal;
	float distance;
	Material material;
};

struct Camera {
	float3 position;
	float3 view;
	float aspect;
	float fovy;
	float znear;
	float zfar;
};

struct Ray {
	float3 origin;
	float3 dir;
	float len;
};

struct RayHit {
	float t;
	float3 position;
	float3 normal;
	Material material;
};

RWTexture2D<float4> image : register(u0);

StructuredBuffer<uint2> block_positions : register(t0);
StructuredBuffer<Sphere> spheres : register(t1);
StructuredBuffer<Plane> planes : register(t2);
StructuredBuffer<Camera> camera : register(t3);

cbuffer info : register(b0) {
  uint block_index;
};

[numthreads(16, 16, 1)]
void compute_shader(uint3 group_id : SV_GroupID, uint3 group_thread_id : SV_GroupThreadID) {
  uint2 position = block_positions[block_index];
	image[position + group_thread_id.xy] = float4(1, 0, 0, 0);
}