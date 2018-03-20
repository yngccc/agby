#version 450

#include "../shader_type.cpp"

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
layout(location = 5) out vec3 tbn_directional_light_out;
layout(location = 6) out vec3 tbn_point_light_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform level_info {
  shader_level_info level;
};

layout(set = 0, binding = 1) uniform model_info {
  shader_model_info model;
};

layout(set = 0, binding = 2) uniform mesh_info {
  shader_mesh_info mesh;
};

layout(set = 0, binding = 3) uniform primitive_info {
  shader_primitive_info primitive;
};

layout(push_constant) uniform push_constant {
  uint level_info_offset;
  uint model_info_offset;
  uint mesh_info_offset;
  uint primitive_info_offset;
  uint diffuse_map_index;
  uint metallic_map_index;
  uint roughness_map_index;
  uint normal_map_index;
  uint height_map_index;
  uint shadow_map_index;
} pc;

void main() {
  mat4 joint_mat = mesh.joint_mats[joints_in[0]] * weights_in[0] + 
                   mesh.joint_mats[joints_in[1]] * weights_in[1] + 
                   mesh.joint_mats[joints_in[2]] * weights_in[2] + 
                   mesh.joint_mats[joints_in[3]] * weights_in[3];

  mat4 model_mat = model.model_mat * joint_mat;
  mat3 normal_mat = mat3(model_mat);

  vec4 position = model_mat * vec4(position_in, 1);
  vec3 normal = normalize(normal_mat * normal_in);
  vec3 tangent = normalize(normal_mat * tangent_in);
  vec3 bitangent = normalize(cross(normal, tangent));
  mat3 inverse_tbn_mat = transpose(mat3(tangent, bitangent, normal));

  gl_Position = level.view_proj_mat * position;
  position_out = position.xyz;
  shadow_map_coord_out = level.shadow_map_proj_mat * position;
  uv_out = uv_in;
  tbn_position_out = inverse_tbn_mat * position.xyz;
  tbn_camera_position_out = inverse_tbn_mat * level.camera_position.xyz;
  tbn_directional_light_out = inverse_tbn_mat * level.directional_light_dir.xyz;
  tbn_point_light_out = inverse_tbn_mat * level.point_light_position.xyz;
} 
