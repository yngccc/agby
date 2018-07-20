#include "common.h"

cbuffer terrain_constants : register(b1) {
	float max_height;
  float sample_per_meter;
};

Texture2D<float> height_texture : register(t0);

sampler texture_sampler : register(s0);

struct vs_input {
  float2 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct vs_output {
  float4 position : SV_POSITION;
	float depth : DEPTH;
};

vs_output vertex_shader(vs_input vs_input) {
  float height = height_texture.SampleLevel(texture_sampler, vs_input.texcoord, 0) * max_height;
	float3 position = float3(vs_input.position.x, height, vs_input.position.y);

  vs_output output;
	output.position = mul(shadow_proj_mat, float4(position, 1));
  output.depth = output.position.z / output.position.w;
	return output;
}

struct ps_output {
  float4 color : SV_TARGET;
};

ps_output pixel_shader(vs_output vs_output) {
	float moment1 = vs_output.depth;
	float moment2 = moment1 * moment1;
	float dx = ddx(moment1);
	float dy = ddy(moment1);
	moment2 += 0.25f * (dx * dx + dy * dy);

  ps_output ps_output;
	ps_output.color = float4(moment1, moment2, 0, 0);
	return ps_output;
}