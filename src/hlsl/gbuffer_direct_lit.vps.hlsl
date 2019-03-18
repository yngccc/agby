cbuffer world_constants : register(b0) {
	float3 camera_position;
	float3 sun_light_dir;
	float3 sun_light_color;
}

Texture2D<float3> diffuse_texture : register(t0);
Texture2D<float3> position_texture : register(t1);
Texture2D<float3> normal_texture : register(t2);
Texture2D<float> roughness_metallic_texture : register(t3);
 
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

static const float PI = 3.14159265358979323846;

float3 cook_torrance_brdf(float3 wi, float3 wo, float3 normal, float3 diffuse, float roughness, float metallic) {
	float3 half_way = normalize(wi + wo);
	float n_dot_h = max(dot(normal, half_way), 0);
	float n_dot_wi = max(dot(normal, wi), 0.01);
	float n_dot_wo = max(dot(normal, wo), 0.01);

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
	float4 color : SV_TARGET0;
};

ps_output pixel_shader(vs_output vs_output) {
	float3 diffuse = diffuse_texture.Sample(texture_sampler, vs_output.texcoord);
	float3 position = position_texture.Sample(texture_sampler, vs_output.texcoord);
	float3 normal = normal_texture.Sample(texture_sampler, vs_output.texcoord);
	float2 roughness_metallic = roughness_metallic_texture.Sample(texture_sampler, vs_output.texcoord);

	float3 wi = sun_light_dir;
	float3 wo = normalize(camera_position - position);
	float3 lo = cook_torrance_brdf(wi, wo, normal, diffuse, roughness_metallic.x, roughness_metallic.y) * sun_light_color;

	ps_output ps_output;
	ps_output.color = float4(lo, 0);
	return ps_output;
}
