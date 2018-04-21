#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 uv_in;

layout(location = 0) out vec4 color_out;

m_declare_cube_texture_array
m_declare_skybox_push_constant

void main() {
  color_out = texture(m_cube_map, uv_in);
}
