#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec3 normal_in;

layout(location = 0) out vec4 color_out;

m_declare_uniform_buffer
m_declare_2d_texture_array
m_declare_terrain_push_constant

void main() {
  vec3 diffuse = texture(m_diffuse_map, uv_in).xyz;
  color_out = vec4(diffuse, 1);
}