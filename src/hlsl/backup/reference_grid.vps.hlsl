#include "common.h"

struct vs_input {
  float3 position : POSITION;
};

struct vs_output {
  float4 position : SV_POSITION;
};

vs_output vertex_shader(vs_input vs_input) {
  vs_output output;
	output.position = mul(camera_view_proj_mat, float4(vs_input.position, 1));
	return output;
}

struct ps_output {
  float4 color : SV_TARGET;
};

ps_output pixel_shader(vs_output vs_output) {
  ps_output output;
	output.color = float4(1, 1, 1, 1);
	return output;
}