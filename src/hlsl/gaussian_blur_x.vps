#include "common.h"

Texture2D src_texture;

sampler texture_sampler : register(s0);

struct vs_output {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

vs_output vertex_shader(uint vertex_id: SV_VertexID) {
	vs_output output;
  output.texcoord = float2((vertex_id << 1) & 2, vertex_id & 2);
	output.position = float4(output.texcoord * float2(2, -2) + float2(-1, 1), 0, 1);
	return output;
}

struct ps_output {
  float4 color : SV_Target;			 
};

ps_output pixel_shader(vs_output vs_output) {
  uint width, height;
	src_texture.GetDimensions(width, height);
  float du = 1.0 / width;
	float4 color = float4(0, 0, 0, 0);
	color += src_texture.Sample(texture_sampler, vs_output.texcoord + float2(du * 0.53805, 0)) * 0.44908;
	color += src_texture.Sample(texture_sampler, vs_output.texcoord - float2(du * 0.53805, 0)) * 0.44908;
	color += src_texture.Sample(texture_sampler, vs_output.texcoord + float2(du * 2.06278, 0)) * 0.05092;
	color += src_texture.Sample(texture_sampler, vs_output.texcoord - float2(du * 2.06278, 0)) * 0.05092;

  ps_output output;
	output.color = color;
	return output;
}