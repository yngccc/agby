#include "common.h"

RWTexture2DArray<float> output_textures : register(u0);

RaytracingAccelerationStructure scene : register(t0);
Texture2D<float3> position_texture : register(t1);
Texture2D<float3> normal_texture : register(t2);

struct ray_pay_load {
	float visibility;
};

[shader("raygeneration")]
void ray_gen() {
}

[shader("miss")]
void miss(inout ray_pay_load payload) {
}

[shader("anyhit")]
void any_hit(inout ray_pay_load payload, in BuiltInTriangleIntersectionAttributes attribs) {
}