cbuffer world_constants : register(b0) {
	float4x4 camera_view_proj_mat;
}

cbuffer model_constants : register(b1) {
	float4x4 model_mat;
};

cbuffer mesh_constants : register(b2) {
	float4x4 mesh_mat;
};

cbuffer primitive_constants : register(b3) {
	float3 diffuse_factor;
	float metallic_factor;
	float roughness_factor;
};

Texture2D<float3> diffuse_texture : register(t0);
Texture2D<float2> normal_texture : register(t1);
Texture2D<float> roughness_texture : register(t2);
Texture2D<float> metallic_texture : register(t3);

sampler texture_sampler : register(s0);

struct vs_input {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct vs_output {
	float4 sv_position : SV_POSITION;
	float3 position : POSITIONT;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3x3 tbn_mat : OUTPUT0;
};

vs_output vertex_shader(vs_input vs_input) {
	float4x4 transform_mat = mul(model_mat, mesh_mat);
	float3x3 normal_mat = float3x3(transform_mat[0].xyz, transform_mat[1].xyz, transform_mat[2].xyz);

	vs_output vs_output;
	vs_output.position = mul(transform_mat, float4(vs_input.position, 1)).xyz;
	vs_output.sv_position = mul(camera_view_proj_mat, float4(vs_output.position, 1));
	vs_output.texcoord = vs_input.texcoord;
	vs_output.normal = normalize(mul(normal_mat, vs_input.normal));
	float3 tangent = normalize(mul(normal_mat, vs_input.tangent));
	float3 bitangent = normalize(cross(vs_output.normal, tangent));
	vs_output.tbn_mat = float3x3(tangent, bitangent, vs_output.normal);

	return vs_output;
}

struct ps_output {
	float4 diffuse : SV_TARGET0;
	float4 position : SV_TARGET1;
	float4 normal : SV_TARGET2;
	float2 roughness_metallic : SV_TARGET3;
};

ps_output pixel_shader(vs_output vs_output) {
	float3 diffuse = diffuse_texture.Sample(texture_sampler, vs_output.texcoord) * diffuse_factor;
	float2 tnormal = normal_texture.Sample(texture_sampler, vs_output.texcoord) * 2 - 1;
	float roughness = roughness_texture.Sample(texture_sampler, vs_output.texcoord) * roughness_factor;
	float metallic = metallic_texture.Sample(texture_sampler, vs_output.texcoord) * metallic_factor;

	// float3 normal = mul(vs_output.tbn_mat, float3(tnormal, sqrt(1 - tnormal.x * tnormal.x - tnormal.y * tnormal.y)));
	float3 normal = normalize(vs_output.normal);

	ps_output ps_output;
	ps_output.diffuse = float4(diffuse, 0);
	ps_output.position = float4(vs_output.position, 0);
	ps_output.normal = float4(normal, 0);
	ps_output.roughness_metallic = float2(roughness, metallic);
	return ps_output;
}
