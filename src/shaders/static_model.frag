#version 450

#include "common.glsl"

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec4 shadow_map_coord_in;
layout(location = 2) in vec2 uv_in;
layout(location = 3) in vec3 tbn_position_in;
layout(location = 4) in vec3 tbn_camera_position_in;
layout(location = 5) in vec3 tbn_directional_lights_in[max_directional_light_count];
layout(location = 6) in vec3 tbn_point_lights_in[max_point_light_count];

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform common_uniform {
  mat4 camera_view_proj_mat;
  vec4 camera_position;
  mat4 shadow_map_proj_mat;
  ambient_light_t ambient_light;
  directional_light_t directional_lights[max_directional_light_count];
  point_light_t point_lights[max_point_light_count];
  spot_light_t spot_lights[max_spot_light_count];
  uint directional_light_count;
  uint point_light_count;
  uint spot_light_count;
};

layout(set = 0, binding = 1) uniform mesh_uniform {
  mat4 model_mat;
  mat4 normal_mat;
  vec2 uv_scale;
  float roughness;
  float metallic;
  float height_map_scale;
};

layout(set = 1, binding = 0) uniform sampler2D albedo_map;
layout(set = 1, binding = 1) uniform sampler2D metallic_map;
layout(set = 1, binding = 2) uniform sampler2D roughness_map;
layout(set = 1, binding = 3) uniform sampler2D normal_map;
layout(set = 1, binding = 4) uniform sampler2D height_map;

layout(set = 2, binding = 0) uniform sampler2D shadow_map;

float shadow_mapping() {
  vec3 shadow_map_coord = shadow_map_coord_in.xyz / shadow_map_coord_in.w;
  shadow_map_coord.xy = shadow_map_coord.xy * 0.5 + 0.5;
  vec2 moments = texture(shadow_map, shadow_map_coord.xy).rg;
  float p = step(shadow_map_coord_in.z, moments.r);
  float variance = moments.g - moments.r * moments.r;
  variance = max(variance, 0.00002);
  float d = shadow_map_coord.z - moments.r;
  float p_max = variance / (variance + d * d);
  return min(max(p, p_max), 1);
}

vec2 parallax_mapping(vec2 uv, vec3 view) {
  float layer_count = 16;
  float layer_height = 1 / layer_count;
  float current_layer_height = 0;
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

vec3 cook_torrance_brdf(vec3 normal, vec3 view, vec3 light_dir, vec3 light_color, vec3 albedo, float metallic, float roughness) {
  const float PI = 3.14159265359;
  vec3 half_way = normalize(view + light_dir);
  float distribution_ggx = 0;
  {
    float a = roughness * roughness;
    float a2 = a * a;
    float n_dot_h = max(dot(normal, half_way), 0);
    float n_dot_h2 = n_dot_h * n_dot_h;
    float nom = a2;
    float denom = n_dot_h2 * (a2 - 1) + 1;
    denom = PI * denom * denom;
    distribution_ggx = nom / denom;
  }
  float geometry_smith = 0;
  {
    float r = roughness + 1;
    float k = r * r / 8;
    float n_dot_l = max(dot(normal, light_dir), 0);
    float n_dot_v = max(dot(normal, view), 0);
    float ggx1  = n_dot_l / (n_dot_l * (1 - k) + k);
    float ggx2  = n_dot_v / (n_dot_v * (1 - k) + k);
    geometry_smith = ggx1 * ggx2;
  }
  vec3 fresnel_schlick = vec3(0, 0, 0); 
  {
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float cos_theta = max(dot(half_way, view), 0);
    fresnel_schlick = F0 + (1 - F0) * pow(1 - cos_theta, 5);
  }
  vec3 ks = fresnel_schlick;
  vec3 kd = (vec3(1) - ks) * (1 - metallic);
  vec3 nom = distribution_ggx * geometry_smith * fresnel_schlick;
  float denom = 4 * max(dot(normal, view), 0) * max(dot(normal, light_dir), 0) + 0.001;
  vec3 specular = nom / denom;
  return (kd * albedo / PI + specular) * light_color * max(dot(normal, light_dir), 0);
}

void main() {
  vec3 view = normalize(tbn_camera_position_in - tbn_position_in);
  vec2 uv = parallax_mapping(uv_in * uv_scale,  view);
  vec2 normal_xy = texture(normal_map, uv).xy * 2 - 1;
  float normal_z = sqrt(1 - normal_xy.x * normal_xy.x - normal_xy.y * normal_xy.y);
  vec3 normal = vec3(normal_xy, normal_z);
  vec3 albedo = texture(albedo_map, uv).rgb;
  float metallic = texture(metallic_map, uv).r;
  float roughness = texture(roughness_map, uv).r;

  color_out = vec4(ambient_light.color.rgb * albedo, 1);
  for (uint i = 0; i < directional_light_count; i += 1) {
    vec3 brdf = cook_torrance_brdf(normal, view, tbn_directional_lights_in[i], directional_lights[i].color.rgb, albedo, metallic, roughness);
    brdf = brdf * shadow_mapping();
    color_out.rgb += brdf;
  }
  for (uint i = 0; i < point_light_count; i += 1) {
    float attenuation = point_lights[i].position.w;
    vec3 direction = tbn_point_lights_in[i] - tbn_position_in;
    vec3 light_color = point_lights[i].color.rgb / pow(length(direction), attenuation);
    vec3 brdf = cook_torrance_brdf(normal, view, normalize(direction), light_color, albedo, metallic, roughness);
    color_out.rgb += brdf;
  }
}
