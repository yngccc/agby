#include "common.h"

cbuffer shape_constants : register(b1) {
  float4x4 transform_mat;
};

struct vs_input {
  float3 position : POSITION;
};

struct vs_output {
  float4 position : SV_POSITION;  
};

vs_output vertex_shader(vs_input vs_input) {
	vs_output output;
	output.position = mul(camera_view_proj_mat, mul(transform_mat, float4(vs_input.position, 1)));
	return output;
}

struct ps_output {
  float4 color : SV_Target;			 
};

ps_output pixel_shader(vs_output vs_output) {
  ps_output output;
	output.color = float4(0, 0.8f, 0, 0.75f);
	return output;
}
