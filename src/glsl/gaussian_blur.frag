#version 450

#include "common.h"

layout(location = 0) in vec2 uv_in;
layout(location = 0) out vec4 color_out;

layout(push_constant) uniform push_constant {
  uint index;
  float x_dir;
  float y_dir;
} pc;

void main() {
	vec2 ds_dt = vec2(1, 1) / textureSize(texture_2ds[pc.index], 0);
	float dx = ds_dt.x * pc.x_dir;
	float dy = ds_dt.y * pc.y_dir;
	vec4 color = vec4(0, 0, 0, 0);
  color += texture(texture_2ds[pc.index], vec2(uv_in.x - 3 * dx, uv_in.y - 3 * dy)) * 0.00598;
  color += texture(texture_2ds[pc.index], vec2(uv_in.x - 2 * dx, uv_in.y - 2 * dy)) * 0.060626;
  color += texture(texture_2ds[pc.index], vec2(uv_in.x - 1 * dx, uv_in.y - 1 * dy)) * 0.241843;
  color += texture(texture_2ds[pc.index], vec2(uv_in.x, uv_in.y)) * 0.383103;
  color += texture(texture_2ds[pc.index], vec2(uv_in.x + 1 * dx, uv_in.y + 1 * dy)) * 0.241843;
  color += texture(texture_2ds[pc.index], vec2(uv_in.x + 2 * dx, uv_in.y + 2 * dy)) * 0.060626;
  color += texture(texture_2ds[pc.index], vec2(uv_in.x + 3 * dx, uv_in.y + 3 * dy)) * 0.00598;
  color_out = color;
}

