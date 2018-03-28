#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec4 shadow_map_coord_in;
layout(location = 2) in vec2 uv_in;
layout(location = 3) in vec3 tbn_position_in;
layout(location = 4) in vec3 tbn_camera_position_in;
layout(location = 5) in vec3 tbn_directional_light_in;
layout(location = 6) in vec3 tbn_point_light_in;

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform level_info {
  shader_level_info level;
};

layout(set = 0, binding = 3) uniform primitive_info {
  shader_primitive_info primitive;
};

layout(set = 1, binding = 0) uniform sampler2D diffuse_map;
layout(set = 1, binding = 1) uniform sampler2D metallic_map;
layout(set = 1, binding = 2) uniform sampler2D roughness_map;
layout(set = 1, binding = 3) uniform sampler2D normal_map;
layout(set = 1, binding = 4) uniform sampler2D height_map;

layout(set = 2, binding = 0) uniform sampler2D shadow_map;

const float PI = 3.14159265359;

float shadow_mapping() {
  vec3 shadow_map_coord = shadow_map_coord_in.xyz / shadow_map_coord_in.w;
  shadow_map_coord.xy = shadow_map_coord.xy * 0.5 + 0.5;
  vec2 moments = texture(shadow_map, shadow_map_coord.xy).xy;
  float p = step(shadow_map_coord_in.z, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.00002);
  float d = shadow_map_coord.z - moments.x;
  float p_max = clamp((variance / (variance + d * d) - 0.2) / 0.8, 0.0, 1.0);
  return min(max(p, p_max), 1);
}

vec2 parallax_mapping(vec2 uv, vec3 view) {
  float layer_count = 16;
  float layer_height = 1 / layer_count;
  float current_layer_height = 0;
  float height_map_scale = 1;
  vec2 P = view.xy * height_map_scale;
  P.y = -P.y;
  vec2 delta_uv = P / layer_count;
  vec2 current_uv = uv;
  float current_height_map_value = texture(height_map, current_uv).r;
  while(current_layer_height < current_height_map_value) {
    current_uv -= delta_uv;
    current_height_map_value = texture(height_map, current_uv).r;  
    current_layer_height += layer_height;  
  }
  vec2 prev_uv = current_uv + delta_uv;
  float after_height = current_height_map_value - current_layer_height;
  float before_height = texture(height_map, prev_uv).r - current_layer_height + layer_height;
  float weight = after_height / (after_height - before_height);
  vec2 final_uv = prev_uv * weight + current_uv * (1 - weight);
  return final_uv;
} 

vec3 get_light_irradiance(vec3 normal, vec3 view, vec3 light_dir, vec3 light_color, vec3 diffuse, float metallic, float roughness) {
  vec3 half_way = normalize(light_dir + view);
  float distribution_ggx;
  {
    float a = roughness * roughness;
    float a2 = a * a;
    float n_dot_h = max(dot(normal, half_way), 0);
    float n_dot_h2 = n_dot_h * n_dot_h;
    float num = a2;
    float denom = n_dot_h2 * (a2 - 1) + 1;
    denom = PI * denom * denom;
    distribution_ggx = num / denom;
  }
  float geometry_smith;
  {
    float n_dot_l = max(dot(normal, light_dir), 0);
    float n_dot_v = max(dot(normal, view), 0);
    float r = roughness + 1;
    float k = r * r / 8;
    float ggx1  = n_dot_l / (n_dot_l * (1 - k) + k);
    float ggx2  = n_dot_v / (n_dot_v * (1 - k) + k);
    geometry_smith = ggx1 * ggx2;
  }
  vec3 fresnel_schlick;
  {
    vec3 F0 = mix(vec3(0.04), diffuse, metallic);
    float cos_theta = max(dot(half_way, view), 0);
    fresnel_schlick = F0 + (1 - F0) * pow(1 - cos_theta, 5);
  }
  vec3 kd = (vec3(1, 1, 1) - fresnel_schlick) * (1 - metallic);
  vec3 num = distribution_ggx * geometry_smith * fresnel_schlick;
  float denom = max(4 * max(dot(normal, view), 0) * max(dot(normal, light_dir), 0), 0.001);
  vec3 specular = num / denom;
  return (kd * diffuse / PI + specular) * light_color * max(dot(normal, light_dir), 0);
}

void main() {
  vec3 view = normalize(tbn_camera_position_in - tbn_position_in);
  vec2 uv = uv_in; // parallax_mapping(uv_in,  view);
  vec2 normal_xy = texture(normal_map, uv).xy * 2 - 1;
  float normal_z = sqrt(1 - normal_xy.x * normal_xy.x - normal_xy.y * normal_xy.y);
  vec3 normal = vec3(normal_xy, normal_z);

  vec3 diffuse = texture(diffuse_map, uv).xyz * primitive.diffuse_factor.xyz;
  float metallic = texture(metallic_map, uv).x * primitive.metallic_factor;  
  float roughness = texture(roughness_map, uv).x * primitive.roughness_factor;

  vec3 irradiance = diffuse * level.ambient_light_color.xyz;
  irradiance += get_light_irradiance(normal, view, tbn_directional_light_in, level.directional_light_color.xyz, diffuse, metallic, roughness) * shadow_mapping();

  // float attenuation = level.point_light_position.w;
  // vec3 direction = tbn_point_light_in - tbn_position_in;
  // vec3 light_color = level.point_light_color.xyz / pow(length(direction), attenuation);
  // brdf += cook_torrance_brdf(normal, view, normalize(direction), light_color, diffuse, metallic, roughness);

  color_out = vec4(irradiance, 1);
}
