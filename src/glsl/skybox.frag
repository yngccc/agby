#version 450

#include "common.h"

layout(location = 0) in vec3 uv_in;

layout(location = 0) out vec4 color_out;

layout(push_constant) uniform push_constant {
  mat4 view_proj_mat; 
  uint cube_map_index;
} pc;

void main() {
  color_out = texture(texture_cubes[pc.cube_map_index], uv_in);
}
