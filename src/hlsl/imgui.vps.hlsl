cbuffer constants : register(b0) {
	uint window_width;
	uint window_height;
};

Texture2D imgui_texture : register(t0);

sampler texture_sampler : register(s0);

struct vs_input {
	float2 position : POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR; 
};

struct vs_output {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

vs_output vertex_shader(vs_input vs_input) {
	vs_output output;
	output.position.x = (vs_input.position.x / window_width) * 2 - 1;
	output.position.y = -((vs_input.position.y / window_height) * 2 - 1);
	output.position.zw = float2(0, 1);
	output.texcoord = vs_input.texcoord;
	output.color = vs_input.color;
	// output.color.rgb = pow(abs(output.color.rgb), float3(2.2f, 2.2f, 2.2f));
	return output;
}

struct ps_output {
	float4 color : SV_Target;			 
};

ps_output pixel_shader(vs_output vs_output) {
	ps_output output;
	output.color = vs_output.color;
	output.color *= imgui_texture.Sample(texture_sampler, vs_output.texcoord);
	return output;
}