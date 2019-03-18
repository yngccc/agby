#include "common.h"

TextureCube cube_texture;

sampler texture_sampler : register(s0);

static const float3 face_vertices[6][6] = {
  {float3(-1, 1, 1), float3(-1, -1, 1), float3(1, 1, 1), float3(1, 1, 1), float3(-1, -1, 1), float3(1, -1, 1)},
  {float3(-1, 1, -1), float3(-1, -1, -1), float3(1, 1, -1), float3(1, 1, -1), float3(-1, -1, -1), float3(1, -1, -1)},
  {float3(-1, -1, -1), float3(-1, 1, -1), float3(-1, -1, 1), float3(-1, -1, 1), float3(-1, 1, -1), float3(-1, 1, 1)},
  {float3(1, -1, -1), float3(1, 1, -1), float3(1, -1, 1), float3(1, -1, 1), float3(1, 1, -1), float3(1, 1, 1)},
  {float3(-1, 1, 1), float3(-1, 1, -1), float3(1, 1, 1), float3(1, 1, 1), float3(-1, 1, -1), float3(1, 1, -1)},
  {float3(-1, -1, 1), float3(-1, -1, -1), float3(1, -1, 1), float3(1, -1, 1), float3(-1, -1, -1), float3(1, -1, -1)}
};

struct vs_output {
	float4 position : SV_POSITION;
	float3 texcoord : TEXCOORD;
};

vs_output vertex_shader(uint vertex_id: SV_VertexID) {
  float3 position = face_vertices[vertex_id / 6][vertex_id % 6];
  float4x4 translate_mat = {1, 0, 0, camera_position.x,
					 							 	  0, 1, 0, camera_position.y,
														0, 0, 1, camera_position.z,
														0, 0, 0, 1};
	vs_output output;
	output.position = mul(camera_view_proj_mat, mul(translate_mat, float4(position, 1)));
	output.position.z = 0;
	output.texcoord = position;
	return output;
}

struct ps_output {
  float4 color : SV_Target;			 
};

ps_output pixel_shader(vs_output vs_output) {
  ps_output output;
	output.color = cube_texture.Sample(texture_sampler, vs_output.texcoord);
	return output;
}