#version 450

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 normal_in;

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform uniform_buffer {
  mat4 camera_view_proj_mat;
  vec4 camera_position;
  vec4 ambient_light_color; 
  vec4 directional_light_color;
  vec4 directional_light_direction;
};

layout(set = 0, binding = 1) uniform uniform_buffer2 {
  mat4 model_mat;
  mat4 normal_mat;
  mat4 shadow_proj_mat;
  vec2 diffuse_map_image_scales;
};

layout(set = 1, binding = 0) uniform sampler2D diffuse_map;
layout(set = 1, binding = 1) uniform sampler2DShadow shadow_map;

void main() {
  vec3 normal = normalize(normal_in);
  vec4 shadow_map_coord = shadow_proj_mat * vec4(position_in, 1);
  shadow_map_coord.xyz = shadow_map_coord.xyz / shadow_map_coord.w;
  shadow_map_coord.x = shadow_map_coord.x * 0.5 + 0.5;
  shadow_map_coord.y = shadow_map_coord.y * 0.5 + 0.5;
  float shadow = 1;
  if (shadow_map_coord.z < 1.0) {
    shadow = texture(shadow_map, shadow_map_coord.xyz);
  }
  vec3 light_color = ambient_light_color.xyz + directional_light_color.xyz * shadow * max(0, dot(normal, -directional_light_direction.xyz));
  vec3 diffuse = texture(diffuse_map, uv_in * diffuse_map_image_scales).rgb;
  color_out = vec4(diffuse * light_color, 1.0);
}