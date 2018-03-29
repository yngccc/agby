#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec3 normal_in;

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform level_info {
  shader_level_info level;
};

layout(set = 1, binding = 0) uniform sampler2D height_map;
layout(set = 1, binding = 1) uniform sampler2D diffuse_map;

void main() {
  vec3 diffuse = texture(diffuse_map, uv_in).xyz;
  color_out = vec4(diffuse, 1);
}