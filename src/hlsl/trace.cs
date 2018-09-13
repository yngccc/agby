RWTexture2D<float4> dst_image;

float rng(inout uint state) {
  uint x = state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 15;
  state = x;
  return (float)x / 4294967295.0;
}

struct Sphere {
  float3 center;
	float radius;
};

StructuredBuffer<Sphere> spheres : register(t0);

[numthreads(16, 16, 1)]
void compute_shader(uint3 group_id : SV_GroupID, uint3 group_thread_id : SV_GroupThreadID) {
    		 
}