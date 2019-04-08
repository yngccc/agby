cbuffer world_constants : register(b0) {
	float4x4 camera_view_proj_mat;
	float3 camera_position;
	float3 camera_view;
	float3 sun_light_dir;
	float3 sun_light_color;
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

static const float PI = 3.14159265358979323846;

float3 cook_torrance_brdf(float3 wi, float3 wo, float3 normal, float3 diffuse, float roughness, float metallic) {
	float3 half_way = normalize(wi + wo);
	float n_dot_h = max(dot(normal, half_way), 0);
	float n_dot_wi = max(dot(normal, wi), 0);
	float n_dot_wo = max(dot(normal, wo), 0);

	float ggx_distribution;
	{
		float a = roughness * roughness;
		float a2 = a * a;
		float n_dot_h2 = n_dot_h * n_dot_h;
		float denom = n_dot_h2 * (a2 - 1) + 1;
		ggx_distribution = a2 / (PI * denom * denom);
	}
	float ggx_geometry;
	{
		float k = (roughness + 1) * (roughness + 1) / 8;
		float ggx1 = n_dot_wi / (n_dot_wi * (1 - k) + k);
		float ggx2 = n_dot_wo / (n_dot_wo * (1 - k) + k);
		ggx_geometry = ggx1 * ggx2;
	}
	float3 fresnel;
	{
		float3 f0 = lerp(float3(0.04, 0.04, 0.04), diffuse, float3(metallic, metallic, metallic));
		float cos_theta = max(dot(half_way, wo), 0);
		fresnel = f0 + (float3(1, 1, 1) - f0) * pow(1 - cos_theta, 5);
	}

	float3 kd = (float3(1, 1, 1) - fresnel) * (1 - metallic);
	float3 fd = diffuse / PI;
	float3 fs = ggx_distribution * ggx_geometry * fresnel / max(4 * n_dot_wo * n_dot_wi, 0.01);

	return (kd * fd + fs) * n_dot_wi;
}

struct ps_output {
	float4 color : SV_TARGET;
};

ps_output pixel_shader(vs_output vs_output) {
	float3 diffuse = diffuse_texture.Sample(texture_sampler, vs_output.texcoord) * diffuse_factor;
	float2 tnormal = normal_texture.Sample(texture_sampler, vs_output.texcoord) * 2 - 1;
	float roughness = roughness_texture.Sample(texture_sampler, vs_output.texcoord) * roughness_factor;
	float metallic = metallic_texture.Sample(texture_sampler, vs_output.texcoord) * metallic_factor;

	// float3 normal = mul(vs_output.tbn_mat, float3(tnormal, sqrt(1 - tnormal.x * tnormal.x - tnormal.y * tnormal.y)));

	float3 wi = sun_light_dir;
	float3 wo = normalize(camera_position - vs_output.position);
	float3 normal = normalize(vs_output.normal);

	ps_output ps_output;
	ps_output.color = float4(cook_torrance_brdf(wi, wo, normal, diffuse, roughness, metallic), 1);
	return ps_output;
}
