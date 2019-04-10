Texture2D<float4> src_texture : register(t0);
Texture2D<float> dither_texture : register(t1);

sampler texture_sampler : register(s0);
sampler dither_sampler : register(s1);

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

float3 linear_to_srgb(float3 rgb) {
	rgb = clamp(rgb, float3(0, 0, 0), float3(1, 1, 1));
	return max(1.055 * pow(rgb, 0.416666667) - 0.055, 0);
}

float3 aces_film_tone_map(float3 x) {
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return saturate((x* (a * x + b)) / (x * (c * x + d) + e));
}

ps_output pixel_shader(vs_output vs_output) {
	ps_output output;
	output.color = src_texture.Sample(texture_sampler, vs_output.texcoord);
	output.color.rgb = aces_film_tone_map(output.color.rgb);
	float dither = dither_texture.Sample(dither_sampler, vs_output.position.xy / 8.0) / 32.0 - (1.0 / 128.0);
	output.color.rgb += float3(dither, dither, dither);
	output.color.rgb = linear_to_srgb(output.color.rgb);
	return output;
}