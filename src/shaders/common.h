const uint max_directional_light_count = 1;
const uint max_point_light_count = 4;

struct directional_light {
  vec4 color;
  vec4 direction;
};

struct point_light {
  vec4 color;
  vec4 position;
};

#define m_common_uniform \
  mat4 camera_view_proj_mat; \
  vec4 camera_position; \
  mat4 shadow_map_proj_mat; \
  vec4 ambient_light_color; \
  directional_light directional_lights[max_directional_light_count]; \
  point_light point_lights[max_point_light_count]; \
  uint directional_light_count; \
  uint point_light_count

#define m_mesh_uniform \
  mat4 model_mat; \
  mat4 normal_mat; \
  vec2 uv_scale; \
  float roughness; \
  float metallic; \
  float height_map_scale


