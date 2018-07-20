#version 450

#include "common.h"

layout(location = 0) in vec3 position_in;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(push_constant) uniform push_constant {
  mat4 transform_mat;	
  vec4 color;													
  uint height_map_index;
  uint width;											
  uint height;
  float max_height;
} pc;

void main() {
  vec4 position = pc.transform_mat * vec4(position_in, 1);
  float x = clamp(position.x + pc.width * 0.5f, 0, pc.width);
	float z = clamp(position.z + pc.height * 0.5f, 0, pc.height);
	vec2 uv = vec2(x / pc.width, z / pc.height);
	float height = texture(texture_2ds[pc.height_map_index], uv).x;
	position.y += height * pc.max_height;
  gl_Position = view_proj_mat * position;
}