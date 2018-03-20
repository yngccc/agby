#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec3 position_in;

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
	gl_Position = level.view_proj_mat * model.model_mat * vec4(position_in, 1);
}