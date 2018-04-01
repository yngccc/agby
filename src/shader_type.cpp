/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#ifdef __cplusplus

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
};

struct shader_model_push_constant {
  uint32 model_offset;
  uint32 mesh_offset;
  uint32 primitive_offset;
  uint32 diffuse_map_index;
  uint32 metallic_map_index;
  uint32 roughness_map_index;
  uint32 normal_map_index;
  uint32 height_map_index;
  uint32 shadow_map_index;
};

struct shader_gaussian_blur_push_constant {
  uint32 index;
  float x_dir;
  float y_dir;
};

struct shader_terrain_push_constant {
  uint32 height_map_index;
  uint32 diffuse_map_index;
};

struct shader_skybox_push_constant {
  mat4 view_proj_mat;
  uint32 cube_map_index;
};

struct shader_lines_push_constant {

};

struct shader_basic_color_vertex_push_constant {
  mat4 transform_mat;
  vec4 color;
};

struct shader_swap_chain_push_constant {
  uint32 texture_index;
};

struct shader_imgui_push_constant {
  vec2 viewport;
  uint32 texture_index;
  uint32 texture_is_grayscale;
};

#else

#define m_declare_uniform_buffer \
  layout(std140, set = 0, binding = 0) uniform uniform_buffer { mat4 ubuf[65536 / 64]; };

#define m_declare_2d_texture_array \
  layout(set = 1, binding = 0) uniform sampler2D textures[1024];

#define m_declare_cube_texture_array \
  layout(set = 1, binding = 1) uniform samplerCube cube_textures[128];

#define m_declare_model_push_constant            \
  layout(push_constant) uniform push_constant {  \
    uint model_offset;                           \
    uint mesh_offset;                            \
    uint primitive_offset;                       \
    uint diffuse_map_index;                      \
    uint metallic_map_index;                     \
    uint roughness_map_index;                    \
    uint normal_map_index;                       \
    uint height_map_index;                       \
    uint shadow_map_index;                       \
  } pc;

#define m_declare_gaussian_blur_push_constant    \
  layout(push_constant) uniform push_constant {  \
    uint index;                                  \
    float x_dir;                                 \
    float y_dir;                                 \
  } pc;

#define m_declare_terrain_push_constant          \
  layout(push_constant) uniform push_constant {  \
    uint height_map_index;                       \
    uint diffuse_map_index;                      \
  } pc;

#define m_declare_skybox_push_constant           \
  layout(push_constant) uniform push_constant {  \
    mat4 view_proj_mat;                          \
    uint cube_map_index;                         \
  } pc;

#define m_declare_basic_color_vertex_push_constant \
  layout(push_constant) uniform push_constant {    \
    mat4 transform_mat;                            \
    vec4 color;                                    \
  } pc;

#define m_declare_swap_chain_push_constant      \
  layout(push_constant) uniform push_constant { \
    uint texture_index;                         \
  } pc;

#define m_declare_imgui_push_constant           \
  layout(push_constant) uniform push_constant { \
    vec2 viewport;                              \
    uint texture_index;                         \
    uint texture_is_grayscale;                  \
  } pc;

#define m_level_view_proj_mat           (ubuf[0])
#define m_level_shadow_map_proj_mat     (ubuf[1])
#define m_level_camera_position         (ubuf[2][0])
#define m_level_ambient_light_color     (ubuf[2][1])
#define m_level_directional_light_color (ubuf[2][2])
#define m_level_directional_light_dir   (ubuf[2][3])
#define m_level_point_light_color       (ubuf[3][0])
#define m_level_point_light_position    (ubuf[3][1])

#define m_model_model_mat (ubuf[pc.model_offset])

#define m_mesh_joint_mat(i) (ubuf[pc.mesh_offset + i])

#define m_primitive_diffuse_factor   (ubuf[pc.primitive_offset][0])
#define m_primitive_metallic_factor  (ubuf[pc.primitive_offset][1][0])
#define m_primitive_roughness_factor (ubuf[pc.primitive_offset][1][1])

#define m_diffuse_map (textures[pc.diffuse_map_index])
#define m_metallic_map (textures[pc.metallic_map_index])
#define m_roughness_map (textures[pc.roughness_map_index])
#define m_normal_map (textures[pc.normal_map_index])
#define m_shadow_map (textures[pc.shadow_map_index])
#define m_height_map (textures[pc.height_map_index])
#define m_cube_map (cube_textures[pc.cube_map_index])

#endif
