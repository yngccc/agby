#version 450

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 normal_in;
layout(location = 3) in vec3 tangent_in;
layout(location = 4) in uvec4 joint_indices;
layout(location = 5) in vec4 joint_weights;

layout(location = 0) out vec3 position_out;
layout(location = 1) out vec2 uv_out;
layout(location = 2) out vec3 normal_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform uniform_buffer {
  mat4 camera_view_proj_mat;
};

layout(set = 0, binding = 1) uniform uniform_buffer_2 {
  mat4 model_mat;
  vec2 diffuse_map_image_scales;
  vec2 normal_map_image_scales;
  mat4 joints[128];
};

void main() {
  mat4 joint_mat = mat4(0);
  for (uint i = 0; i < 4; i += 1) {
    joint_mat += joints[joint_indices[i]] * joint_weights[i];
  }
  mat4 model_mat = model_mat * joint_mat;
  vec4 position = model_mat * vec4(position_in, 1);
  position_out = position.xyz;
  gl_Position = camera_view_proj_mat * position;
  uv_out = uv_in;
  normal_out = normalize((transpose(inverse(model_mat)) * vec4(normal_in, 1)).xyz);
} 
