#include "common.h"

cbuffer terrain_constants : register(b1) {
	float max_height;
  float sample_per_meter;
};

Texture2D<float> height_texture : register(t0);
Texture2D diffuse_texture : register(t1);
Texture2D<float2> shadow_texture : register(t2);

sampler texture_sampler : register(s0);
sampler shadow_texture_sampler : register(s1);

struct vs_input {
  float2 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct vs_output {
  float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 shadow_position : POSITIONT;
};

vs_output vertex_shader(vs_input vs_input) {
  float height = height_texture.SampleLevel(texture_sampler, vs_input.texcoord, 0) * max_height;
	float3 position = float3(vs_input.position.x, height, vs_input.position.y);

	float left = height_texture.SampleLevel(texture_sampler, vs_input.texcoord, 0, int2(-1, 0)).x;
	float right = height_texture.SampleLevel(texture_sampler, vs_input.texcoord, 0, int2(1, 0)).x;
	float top = height_texture.SampleLevel(texture_sampler, vs_input.texcoord, 0, int2(0, -1)).x;
	float bottom = height_texture.SampleLevel(texture_sampler, vs_input.texcoord, 0, int2(0, 1)).x;

  vs_output output;
	output.position = mul(camera_view_proj_mat, float4(position, 1));
	output.texcoord = vs_input.texcoord;
	output.normal = normalize(float3((left - right) * max_height, 2 / sample_per_meter, (top - bottom) * max_height));
	output.shadow_position = mul(shadow_proj_mat, float4(position, 1));
	output.shadow_position /= output.shadow_position.w;
  return output;
}

struct ps_output {
  float4 color : SV_TARGET;
};

ps_output pixel_shader(vs_output vs_output) {
  float2 texcoord = vs_output.shadow_position.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
  float2 moments = shadow_texture.Sample(shadow_texture_sampler, texcoord);
	float shadow = 1;
	if (vs_output.shadow_position.z >= 0 && vs_output.shadow_position.z <= 1) {
    float depth = clamp(vs_output.shadow_position.z, 0, 1);
    shadow = chebyshev_upper_bound(moments, depth, 0.2f);
	}

  ps_output output;
	output.color = diffuse_texture.Sample(texture_sampler, vs_output.texcoord);
	output.color.xyz *= max(dot(vs_output.normal, sun_light_dir), 0);
	output.color.xyz *= shadow;
	return output;
}