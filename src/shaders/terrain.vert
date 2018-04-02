#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;

layout(location = 0) out vec2 uv_out;

out gl_PerVertex {
  vec4 gl_Position;
};

m_declare_uniform_buffer
m_declare_2d_texture_array
m_declare_terrain_push_constant

void main() {
	float height = texture(m_height_map, uv_in).x * m_terrain_max_height;
	vec3 position = position_in + vec3(0, height, 0);
	gl_Position = m_level_view_proj_mat * vec4(position, 1);
	uv_out = uv_in;
}
