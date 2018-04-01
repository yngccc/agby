#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;

out gl_PerVertex {
  vec4 gl_Position;
};

m_declare_uniform_buffer
m_declare_basic_color_vertex_push_constant

void main() {
  gl_Position = m_level_view_proj_mat * pc.transform_mat * vec4(position_in, 1);
}