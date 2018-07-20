#version 450

#include "common.h"

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec4 color_in;

layout(location = 0) out vec4 color_out;

layout(push_constant) uniform push_constant {
  uint framebuffer_width;
  uint framebuffer_height;
  uint texture_index;                   
} pc;

void main() {
  color_out = color_in * texture(texture_2ds[pc.texture_index], uv_in);
}


