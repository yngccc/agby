#version 450

#include "common.h"

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 normal_in;
layout(location = 3) in vec3 tangent_in;
layout(location = 4) in uvec4 joints_in;
layout(location = 5) in vec4 weights_in;

layout(location = 0) out vec3 position_out;
layout(location = 1) out vec4 shadow_map_coord_out;
layout(location = 2) out vec2 uv_out;
layout(location = 3) out vec3 tbn_position_out;
layout(location = 4) out vec3 tbn_camera_position_out;
layout(location = 5) out vec3 tbn_direct_light_out;
layout(location = 6) out vec3 tbn_point_light_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(std140, set = 0, binding = 1) uniform per_model_uniform_buffer {
  mat4 model_mat;
};

layout(std140, set = 0, binding = 2) uniform per_mesh_uniform_buffer {
  mat4 joint_mats[256];
};

layout(push_constant) uniform push_constant { 
  uint diffuse_map_index;                     
  uint metallic_map_index;                    
  uint roughness_map_index;                   
  uint normal_map_index;                      
  uint height_map_index;                      
  uint shadow_map_index;                      
  float metallic_factor;											
  float roughness_factor;											
  vec4 diffuse_factor;												
} pc;

void main() {
  mat4 joint_mat = joint_mats[joints_in[0]] * weights_in[0] +
                   joint_mats[joints_in[1]] * weights_in[1] +
                   joint_mats[joints_in[2]] * weights_in[2] +
                   joint_mats[joints_in[3]] * weights_in[3];

  mat4 model_mat = model_mat * joint_mat;
  mat3 normal_mat = mat3(model_mat);

  vec4 position = model_mat * vec4(position_in, 1);
  position_out = position.xyz;
  gl_Position = view_proj_mat * position;

  uv_out = uv_in;
  shadow_map_coord_out = shadow_map_proj_mat * position;

  vec3 normal = normalize(normal_mat * normal_in);
  vec3 tangent = normalize(normal_mat * tangent_in);
  vec3 bitangent = normalize(cross(normal, tangent));
  mat3 inverse_tbn_mat = transpose(mat3(tangent, bitangent, normal));

  tbn_position_out = inverse_tbn_mat * position.xyz;
  tbn_camera_position_out = inverse_tbn_mat * camera_position.xyz;
  tbn_direct_light_out = inverse_tbn_mat * direct_light_dir.xyz;
  tbn_point_light_out = inverse_tbn_mat * point_light_position.xyz;
} 
