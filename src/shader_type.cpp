// types shared between c++ and glsl shader
// must be in valid glsl syntax

struct shader_level_info {
  mat4 view_proj_mat;
  mat4 shadow_map_proj_mat;

  vec4 camera_position;
  vec4 ambient_light_color;
  vec4 directional_light_color;
  vec4 directional_light_dir;
  vec4 point_light_color;
  vec4 point_light_position;
};

struct shader_model_info {
  mat4 model_mat;
};

struct shader_mesh_info {
  mat4 joint_mats[256];
};

struct shader_primitive_info {
  vec4 diffuse_factor;
  float metallic_factor;
  float roughness_factor;
  float height_map_factor;
  float padding;
};

