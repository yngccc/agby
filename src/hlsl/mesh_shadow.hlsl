#include "common.h"

cbuffer model_constants : register(b1) {
  float4x4 model_mat;
};

cbuffer mesh_constants : register(b2) {
  float4x4 joint_mats[256];
};

struct vs_input {
  float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	uint4 joint_indices : BLENDINDICES;
	float4 joint_weights : BLENDWEIGHT;
};

struct vs_output {
  float4 position : SV_POSITION;
	float depth : DEPTH;
};

vs_output vertex_shader(vs_input vs_input) {
  float4x4 joint_mat =
	  joint_mats[vs_input.joint_indices[0]] * vs_input.joint_weights[0] +
	  joint_mats[vs_input.joint_indices[1]] * vs_input.joint_weights[1] +
	  joint_mats[vs_input.joint_indices[2]] * vs_input.joint_weights[2] +
	  joint_mats[vs_input.joint_indices[3]] * vs_input.joint_weights[3];

  vs_output output;
	output.position = mul(shadow_proj_mat, mul(model_mat, mul(joint_mat, float4(vs_input.position, 1))));
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