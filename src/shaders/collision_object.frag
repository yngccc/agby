#version 450

layout(location = 0) out vec4 color_out;

layout(push_constant) uniform push_constants {
  mat4 transform_mat;
  vec4 color;
} push_consts;

void main() {
  color_out = push_consts.color;
}
