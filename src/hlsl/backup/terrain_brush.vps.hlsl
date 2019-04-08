#include "common.h"

cbuffer brush_constants : register(b1) {
  float4x4 transform_mat;
  uint width;
  uint height;
	float max_height;
};

Texture2D<float> height_texture : register(t0);

sampler texture_sampler : register(s0);

struct vs_input {
  float3 position : POSITION;
};

struct vs_output {
  float4 position : SV_POSITION;
};

vs_output vertex_shader(vs_input vs_input) {
  float4 position = mul(transform_mat, float4(vs_input.position, 1));
  float x = clamp(position.x + width * 0.5f, 0, width);
	float z = clamp(position.z + height * 0.5f, 0, height);
	float2 uv = float2(x / width, z / height);
	float height = height_texture.SampleLevel(texture_sampler, uv, 0).x;
	position.y += height * max_height;

  vs_output output;
	output.position = mul(camera_view_proj_mat, position);
	return output;
}

struct ps_output {
  float4 color : SV_TARGET;
};

ps_output pixel_shader(vs_output vs_output) {
  ps_output output;
	output.color = float4(0.8f, 0, 0, 1);
	return output;
}