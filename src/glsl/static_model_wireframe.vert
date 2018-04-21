#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;
layout(location = 1) in uvec4 joints_in;
layout(location = 2) in vec4 weights_in;

out gl_PerVertex {
  vec4 gl_Position;
};

m_declare_uniform_buffer
m_declare_model_push_constant

void main() {
  mat4 joint_mat = m_mesh_joint_mat(joints_in[0]) * weights_in[0] +
                   m_mesh_joint_mat(joints_in[1]) * weights_in[1] +
                   m_mesh_joint_mat(joints_in[2]) * weights_in[2] +
                   m_mesh_joint_mat(joints_in[3]) * weights_in[3];
  gl_Position = m_level_view_proj_mat * m_model_model_mat * joint_mat * vec4(position_in, 1);
}