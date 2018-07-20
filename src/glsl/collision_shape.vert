#version 450

#include "common.h"

layout(location = 0) in vec3 position_in;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(push_constant) uniform push_constant {
  mat4 transform_mat;
  vec4 color;                         
} pc;

void main() {
  vec4 position = pc.transform_mat * vec4(position_in, 1);
  gl_Position = view_proj_mat * position;
}