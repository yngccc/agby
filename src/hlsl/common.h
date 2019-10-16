static const float PI = 3.14159265358979323846f;

static const uint direct_light = 0;
static const uint sphere_light = 1;

struct light {
	uint type;
	float radius;
	float2 padding;
	float4 color;
	float4 position;
	float4 dir;
};

float chebyshev_upper_bound(float2 moments, float t, float min_p_max) {
	float p = t >= moments.x;
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, 0.00002f);
	float d = t - moments.x;
	float p_max = variance / (variance + d * d);
	p_max = clamp((p_max - min_p_max) / (1 - min_p_max), 0, 1);
	return max(p, p_max);
}

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

float rand_xorshift(inout uint rng_state) {
	rng_state ^= (rng_state << 13);
	rng_state ^= (rng_state >> 17);
	rng_state ^= (rng_state << 5);
	return float(rng_state) * (1.0 / 4294967296.0);
}

uint wang_hash(uint seed) {
	seed = (seed ^ 61) ^ (seed >> 16);
	seed *= 9;
	seed = seed ^ (seed >> 4);
	seed *= 0x27d4eb2d;
	seed = seed ^ (seed >> 15);
	return seed;
}
