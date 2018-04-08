#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;

out gl_PerVertex {
  vec4 gl_Position;
};

m_declare_uniform_buffer
m_declare_color_vertex_push_constant
m_declare_2d_texture_array

void main() {
	vec4 position = pc.transform_mat * vec4(position_in, 1);
	// special case for rendering terrain brush
	float x = clamp(position.x, -32, 32);
	float z = clamp(position.z, -32, 32);
	float u = (x + 32) / 64;
	float v = (z + 32) / 64;
	float height = texture(m_height_map, vec2(u, v)).x;
	position.y += height * m_terrain_scale;

  gl_Position = m_level_view_proj_mat * position;
}