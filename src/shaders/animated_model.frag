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

layout(set = 0, binding = 1) uniform uniform_buffer_2 {
  mat4 model_mat;
  vec2 diffuse_map_image_scales;
  vec2 normal_map_image_scales;
};

layout(set = 1, binding = 0) uniform sampler2D diffuse_map;

void main() {
  color_out = texture(diffuse_map, uv_in * diffuse_map_image_scales);
  vec3 normal = normal_in;
  //    vec3 directional_light_reflect = normalize(reflect(directional_light_direction.xyz, normal));
  //    vec3 camera_direction = normalize(camera_position.xyz - position_in);
  //    vec3 specular_color = directional_light_color.rgb * directional_light_color.a * 0.5 * max(0, pow(dot(directional_light_reflect, camera_direction), 32));
  vec3 light_color = ambient_light_color.xyz + directional_light_color.xyz * max(0, dot(normal, -directional_light_direction.xyz));
  color_out.xyz = color_out.xyz * light_color;
}
