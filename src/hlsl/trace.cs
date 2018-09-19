float rng(inout uint state) {
  uint x = state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 15;
  state = x;
  return (float)x / 4294967295.0;
}

StructuredBuffer<uint2> block_positions : register(t0);

struct Sphere {
  float3 center;
	float radius;
};

StructuredBuffer<Sphere> spheres : register(t1);

cbuffer info : register(b0) {
  uint block_index;
};

RWTexture2D<float4> image : register(u0);

[numthreads(16, 16, 1)]
void compute_shader(uint3 group_id : SV_GroupID, uint3 group_thread_id : SV_GroupThreadID) {
  uint2 position = block_positions[block_index];
	image[position + group_thread_id.xy] = float4(1, 0, 0, 0);
}