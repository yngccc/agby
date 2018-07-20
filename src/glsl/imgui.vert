#version 450

#include "common.h"

layout(location = 0) in vec2 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec4 color_in;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec4 color_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(push_constant) uniform push_constant {
  uint framebuffer_width;
  uint framebuffer_height;
  uint texture_index;                   
} pc;

void main() {
  uv_out = uv_in;
  color_out = color_in;
	color_out.xyz = pow(color_out.xyz, vec3(2.2, 2.2, 2.2));

	float px = position_in.x / pc.framebuffer_width;
	float py = position_in.y / pc.framebuffer_height;
	gl_Position = vec4(px * 2 - 1, py * 2 - 1, 0, 1);
} 