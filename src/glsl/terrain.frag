#version 450

#include "common.h"

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec4 shadow_map_coord_in;

layout(location = 0) out vec4 color_out;

layout(push_constant) uniform push_constant {
  float sample_per_meter;
  float max_height;											
  uint height_map_index;
  uint diffuse_map_index;
  float diffuse_map_uv_repeat;
  uint shadow_map_index;								
} pc;

float shadow_mapping() {
  vec3 shadow_map_coord = shadow_map_coord_in.xyz / shadow_map_coord_in.w;
  shadow_map_coord.xy = shadow_map_coord.xy * 0.5 + 0.5;
  vec2 moments = texture(texture_2ds[pc.shadow_map_index], shadow_map_coord.xy).xy;
  float p = step(shadow_map_coord_in.z, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.00002);
  float d = shadow_map_coord.z - moments.x;
  float p_max = clamp((variance / (variance + d * d) - 0.2) / 0.8, 0.0, 1.0);
  return min(max(p, p_max), 1);
}

void main() {
	vec3 diffuse = texture(texture_2ds[pc.diffuse_map_index], uv_in * pc.diffuse_map_uv_repeat).xyz;
  color_out = vec4(0, 0, 0, 1);
	color_out.xyz += diffuse * ambient_light_color.xyz;
	color_out.xyz += diffuse * max(dot(normal_in, direct_light_dir.xyz), 0);
	// shadow_mapping();
}