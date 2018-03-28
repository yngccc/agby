#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;
layout(location = 1) in uvec4 joints_in;
layout(location = 2) in vec4 weights_in;

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

void main() {
  mat4 joint_mat = mesh.joint_mats[joints_in[0]] * weights_in[0] + 
                   mesh.joint_mats[joints_in[1]] * weights_in[1] + 
                   mesh.joint_mats[joints_in[2]] * weights_in[2] + 
                   mesh.joint_mats[joints_in[3]] * weights_in[3];
  mat4 model_mat = model.model_mat * joint_mat;

	gl_Position = level.shadow_map_proj_mat * model_mat * vec4(position_in, 1);
}
