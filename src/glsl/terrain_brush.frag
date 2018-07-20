#version 450

#include "common.h"

layout(location = 0) out vec4 color_out;

layout(push_constant) uniform push_constant {
  mat4 transform_mat;	
  vec4 color;													
  uint height_map_index;
  uint width;											
  uint height;
  float max_height;
} pc;

void main() {
  color_out = pc.color;
}
