
#define M_PI 3.14159265359

layout(set = 0, binding = 0) uniform per_frame_uniform_buffer {
	mat4 view_proj_mat;
	mat4 shadow_map_proj_mat;
	vec4 camera_position;
	vec4 ambient_light_color;
	vec4 direct_light_color;
	vec4 direct_light_dir; 
	vec4 point_light_color;
	vec4 point_light_position;
};

layout(set = 1, binding = 0) uniform sampler2D texture_2ds[1024];

layout(set = 2, binding = 0) uniform samplerCube texture_cubes[128];
