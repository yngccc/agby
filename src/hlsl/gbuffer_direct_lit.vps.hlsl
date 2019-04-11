#include "common.h"

cbuffer world_constants : register(b0) {
	float4 camera_position;
	light lights[4];
	uint light_count;
};

Texture2D<float3> diffuse_texture : register(t0);
Texture2D<float3> position_texture : register(t1);
Texture2D<float3> normal_texture : register(t2);
Texture2D<float> roughness_metallic_texture : register(t3);
Texture2D<float3> emissive_texture : register(t4);
Texture2DArray<float> light_visibility_textures : register(t5);

sampler texture_sampler : register(s0);

struct vs_input {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

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
	float4 color : SV_TARGET0;
};

ps_output pixel_shader(vs_output vs_output) {
	float3 diffuse = diffuse_texture.Sample(texture_sampler, vs_output.texcoord);
	float3 position = position_texture.Sample(texture_sampler, vs_output.texcoord);
	float3 normal = normal_texture.Sample(texture_sampler, vs_output.texcoord);
	float2 roughness_metallic = roughness_metallic_texture.Sample(texture_sampler, vs_output.texcoord);
	float3 emissive = emissive_texture.Sample(texture_sampler, vs_output.texcoord);

	float3 lo = emissive;
	for (uint i = 0; i < 4; i += 1) {
		if (i < light_count) {
			light l = lights[i];
			float3 wo = normalize(camera_position.xyz - position);
			float3 wi = { 0, 1, 0 };
			if (l.type == direct_light) {
				wi = l.dir.xyz;
			}
			else if (l.type == sphere_light) {
				wi = normalize(l.position.xyz - position);
			}
			float visibility = light_visibility_textures.Sample(texture_sampler, float3(vs_output.texcoord, i));
#if 1
			lo += cook_torrance_brdf(wi, wo, normal, diffuse, roughness_metallic.x, roughness_metallic.y) * l.color.rgb * visibility;
#else
			lo += cook_torrance_brdf(wi, wo, normal, float3(0.8, 0.8, 0.8), 1, 0) * l.color.rgb * visibility; 
#endif
		}
	}

	ps_output output;
	output.color = float4(lo, 0);
	return output;
}
