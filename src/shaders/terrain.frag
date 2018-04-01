#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec2 uv_in;

layout(location = 0) out vec4 color_out;

m_declare_uniform_buffer
m_declare_2d_texture_array
m_declare_terrain_push_constant

const float max_height = 16;

void main() {
	float left = textureOffset(m_height_map, uv_in, ivec2(-1, 0)).x;
	float right = textureOffset(m_height_map, uv_in, ivec2(1, 0)).x;
	float top = textureOffset(m_height_map, uv_in, ivec2(0, -1)).x;
	float bottom = textureOffset(m_height_map, uv_in, ivec2(0, 1)).x;
	vec3 va = normalize(vec3(1, (right - left) * max_height, 0));
	vec3 vb = normalize(vec3(0, (top - bottom) * max_height, 1));
	vec3 normal = cross(vb, va);

  vec3 diffuse = texture(m_diffuse_map, uv_in).xyz;
	diffuse *= max(dot(normal, m_level_directional_light_dir.xyz), 0);
  
  color_out = vec4(diffuse, 1);
}