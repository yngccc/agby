#include "common.h"

cbuffer model_constants : register(b1) {
  float4x4 model_mat;
};

cbuffer mesh_constants : register(b2) {
  float4x4 joint_mats[256];
};

cbuffer primitive_constants : register(b3) {
  float3 diffuse_factor;												
  float metallic_factor;											
  float roughness_factor;											
};

Texture2D<float3> diffuse_texture : register(t0);
Texture2D<float> roughness_texture : register(t1);
Texture2D<float> metallic_texture : register(t2);
Texture2D<float2> normal_texture : register(t3);

sampler texture_sampler : register(s0);

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
	float2 texcoord : TEXCOORD;
	float3 tbn_position : OUTPUT0;
	float3 tbn_camera_position : OUTPUT1;
	float3 tbn_sun_light_dir : OUTPUT2;
};

vs_output vertex_shader(vs_input vs_input) {
  float4x4 joint_mat = joint_mats[vs_input.joint_indices[0]] * vs_input.joint_weights[0] +
	  			 					 	 joint_mats[vs_input.joint_indices[1]] * vs_input.joint_weights[1] +
											 joint_mats[vs_input.joint_indices[2]] * vs_input.joint_weights[2] +
											 joint_mats[vs_input.joint_indices[3]] * vs_input.joint_weights[3];
  float4x4 transform_mat = mul(model_mat, joint_mat);
  float4 position = mul(transform_mat, float4(vs_input.position, 1));

  float3x3 normal_mat = float3x3(transform_mat[0].xyz, transform_mat[1].xyz, transform_mat[2].xyz);
  float3 normal = normalize(mul(normal_mat, vs_input.normal));
  float3 tangent = normalize(mul(normal_mat, vs_input.tangent));
  float3 bitangent = normalize(cross(normal, tangent));
  float3x3 inverse_tbn_mat = float3x3(tangent, bitangent, normal);

  vs_output vs_output;
	vs_output.position = mul(camera_view_proj_mat, position);
	vs_output.texcoord = vs_input.texcoord;
	vs_output.tbn_position = mul(inverse_tbn_mat, position.xyz);
	vs_output.tbn_camera_position = mul(inverse_tbn_mat, camera_position);
  vs_output.tbn_sun_light_dir = mul(inverse_tbn_mat, sun_light_dir);
	return vs_output;
}

struct ps_output {
  float3 color : SV_TARGET;
};

ps_output pixel_shader(vs_output vs_output) {
  float3 diffuse = diffuse_texture.Sample(texture_sampler, vs_output.texcoord) * diffuse_factor;
	float roughness = roughness_texture.Sample(texture_sampler, vs_output.texcoord) * roughness_factor;
	float metallic = metallic_texture.Sample(texture_sampler, vs_output.texcoord) * metallic_factor;
	float2 normal = normal_texture.Sample(texture_sampler, vs_output.texcoord);

  float3 tbn_normal;
	tbn_normal.xy = normal * 2 - 1;
	tbn_normal.z = sqrt(1 - tbn_normal.x * tbn_normal.x - tbn_normal.y * tbn_normal.y);

  float3 wi = vs_output.tbn_sun_light_dir;
  float3 wo = normalize(vs_output.tbn_camera_position - vs_output.tbn_position);

  ps_output ps_output;
	ps_output.color = cook_torrance_brdf(wi, wo, tbn_normal, diffuse, roughness, metallic);
	return ps_output;
}