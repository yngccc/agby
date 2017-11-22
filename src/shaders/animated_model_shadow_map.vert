#version 450

layout(location = 0) in vec3 position_in;
layout(location = 1) in uvec4 joint_indices;
layout(location = 2) in vec4 joint_weights;

layout(location = 0) out vec4 position_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform uniform_buffer {
  mat4 model_mat;
  vec2 diffuse_image_scale;
  vec2 normal_map_image_scale;
  mat4 joints[128];
};

layout(push_constant) uniform push_constant {
  mat4 shadow_map_mat;
} push_const;

void main() {
  mat4 joint_mat = mat4(0);
  for (uint i = 0; i < 4; i += 1) {
    joint_mat += joints[joint_indices[i]] * joint_weights[i];
  }
  position_out = push_const.shadow_map_mat * model_mat * joint_mat * vec4(position_in, 1);
  gl_Position = position_out;
}
