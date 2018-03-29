#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec3 normal_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform level_info {
  shader_level_info level;
};

layout(set = 1, binding = 0) uniform sampler2D height_map;

const float max_height = 1024;

void main() {
	float height_percentage = texture(height_map, uv_in).x;
	float height = max_height * height_percentage;
	vec3 position = position_in + vec3(0, height, 0);
	gl_Position = level.view_proj_mat * vec4(position, 1);
	uv_out = uv_in;
	normal_out = vec3(0, 1, 0);
}
