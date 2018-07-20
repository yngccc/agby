#version 450

#include "common.h"

layout(location = 0) in vec3 position_in;
layout(location = 1) in uvec4 joints_in;
layout(location = 2) in vec4 weights_in;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(std140, set = 0, binding = 1) uniform per_model_uniform_buffer {
  mat4 model_mat;
};

layout(std140, set = 0, binding = 2) uniform per_mesh_uniform_buffer {
  mat4 joint_mats[256];
};

void main() {
  mat4 joint_mat = joint_mats[joints_in[0]] * weights_in[0] +
                   joint_mats[joints_in[1]] * weights_in[1] +
                   joint_mats[joints_in[2]] * weights_in[2] +
                   joint_mats[joints_in[3]] * weights_in[3];
  gl_Position = view_proj_mat * model_mat * joint_mat * vec4(position_in, 1);
}