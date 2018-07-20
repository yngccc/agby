#version 450

#include "common.h"

layout(location = 0) in vec2 position_in;
layout(location = 1) in vec2 uv_in;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec3 normal_out;
layout(location = 2) out vec4 shadow_map_coord_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(push_constant) uniform push_constant {
  float sample_per_meter;
  float max_height;											
  uint height_map_index;
  uint diffuse_map_index;
  float diffuse_map_uv_repeat;
  uint shadow_map_index;								
} pc;

void main() {
  vec3 position = vec3(position_in.x, 0, position_in.y);
  float height = texture(texture_2ds[pc.height_map_index], uv_in).x * pc.max_height;
	position.y += height;
	gl_Position = view_proj_mat * vec4(position, 1);

  uv_out = uv_in;

	float left = textureOffset(texture_2ds[pc.height_map_index], uv_in, ivec2(-1, 0)).x;
	float right = textureOffset(texture_2ds[pc.height_map_index], uv_in, ivec2(1, 0)).x;
	float top = textureOffset(texture_2ds[pc.height_map_index], uv_in, ivec2(0, -1)).x;
	float bottom = textureOffset(texture_2ds[pc.height_map_index], uv_in, ivec2(0, 1)).x;
	normal_out = normalize(vec3((left - right) * pc.max_height, 2 / pc.sample_per_meter, (top - bottom) * pc.max_height));
  // vec3 va = normalize(vec3(1, (right - left) * pc.max_height, 0));
	// vec3 vb = normalize(vec3(0, (top - bottom) * pc.max_height, 1));
	// normal_out = cross(vb, va);

  shadow_map_coord_out = shadow_map_proj_mat * vec4(position, 1);
}
