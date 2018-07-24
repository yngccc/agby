/***************************************************************************************************/
/*					Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.					 */
/***************************************************************************************************/

#include "common.cpp"
#include "math.cpp"

#include <tinyxml/tinyxml2.cpp>

#include <atomic>

const uint32 image_width = 1600;
const uint32 image_height = 900;
const uint32 sample_count = 32;
const uint32 depth_count = 4;
vec3 *image = new vec3[image_width * image_height]();

struct rng {
	uint32 rng_state;
	float gen() {
		uint32 x = rng_state;
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 15;
		rng_state = x;
		return (float)((double)x / (double)UINT32_MAX);
	}
};

enum material_type {
	material_emissive,
	material_diffuse,
	material_metal,
	material_dielectric,
};

struct material {
	material_type type;
	vec3 color;
	float refractive_index;
};

struct scene_floor {
	plane plane;
	material material;
};

struct scene_sphere {
	sphere sphere;
	material material;
};

struct scene {
	scene_floor floor;
	scene_sphere *spheres;
	uint32 sphere_count;
	camera camera;
};

void initialize_scene(scene *scene) {
	scene->floor.plane = plane{{0, 1, 0}, 0};
	scene->floor.material.type = material_diffuse;
	scene->floor.material.color = {0.5f, 0.5f, 0.5f};

	const uint32 sphere_count = 4;
	vec3 sphere_positions[sphere_count] = {{0, 8, 0}, {-5, 2, 0}, {5, 2, 0}, {3, 2, 7}};
	material sphere_materials[sphere_count] = {{material_emissive, {8.0f, 8.0f, 8.0f}},
																						 {material_metal, {0, 0.6f, 0}},
																						 {material_diffuse, {0.6f, 0.6f, 0.6f}},
																						 {material_dielectric, {0.8f, 0.8f, 0.8f}, 1.3f}};
	scene->spheres = new scene_sphere[sphere_count];
	scene->sphere_count = sphere_count;
	for (uint32 i = 0; i < sphere_count; i += 1) {
		scene->spheres[i].sphere = {sphere_positions[i], 2};
		scene->spheres[i].material = sphere_materials[i];
	}

	scene->camera.position = {0, 10, 30};
	scene->camera.view = vec3_normalize(-scene->camera.position);
	scene->camera.aspect = (float)image_width / (float)image_height;
	scene->camera.fovy = hfov_to_vfov(degree_to_radian(80), scene->camera.aspect);
	scene->camera.znear = 0.1f;
	scene->camera.zfar = 100;
}

struct ray_hit {
	float t;
	vec3 point;
	vec3 normal;
	material *material;
};

bool ray_first_hit(scene *scene, ray ray, ray_hit *hit) {
	ray_hit ray_hits[8];
	uint32 ray_hit_count = 0;
	{
		float t;
		vec3 point;
		if (ray_hit_plane(ray, scene->floor.plane, &t, &point)) {
			if (fabsf(vec3_len(point - ray.origin)) > 0.0001f) {
				ray_hits[ray_hit_count++] = {t, point, {0, 1, 0}, &scene->floor.material};
			}
		}
	}
	for (uint32 i = 0; i < scene->sphere_count; i += 1) {
		float t;
		vec3 point;
		if (ray_hit_sphere(ray, scene->spheres[i].sphere, &t, &point)) {
			if (fabsf(vec3_len(point - ray.origin)) > 0.0001f) {
				ray_hits[ray_hit_count++] = {t, point, vec3_normalize(point - scene->spheres[i].sphere.center), &scene->spheres[i].material};
			}
		}
	}
	if (ray_hit_count == 0) {
		return false;
	}
	ray_hit *closest_hit = &ray_hits[0];
	for (uint32 i = 1; i < ray_hit_count; i += 1) {
		if (ray_hits[i].t < closest_hit->t) {
			closest_hit = &ray_hits[i];
		}
	}
	*hit = *closest_hit;
	return true;
}

vec3 reflect(vec3 view, vec3 normal) {
	return view -  normal * 2 * vec3_dot(view, normal);
}

bool refract(vec3 view, vec3 normal, float ni_over_nt, vec3 *refracted) {
	float dt = vec3_dot(view, normal);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
	if (discriminant > 0) {
		*refracted = (view - normal * dt) * ni_over_nt - normal * sqrtf(discriminant);
		return true;
	}
	else {
		return false;
	}
}

vec3 trace(scene *scene, rng *rng, ray ray, uint32 depth) {
	ray_hit hit;
	if (depth <= depth_count && ray_first_hit(scene, ray, &hit)) {
		if (hit.material->type == material_emissive) {
			return hit.material->color;
		}
		else if (hit.material->type == material_diffuse) {
			vec3 color_accumulate = {0, 0, 0};
			struct ray random_rays[sample_count];
			for (uint32 i = 0; i < m_countof(random_rays); i += 1) {
				random_rays[i].origin = hit.point;
				random_rays[i].dir = cosine_sample_hemisphere(rng->gen(), rng->gen());
				random_rays[i].dir = quat_from_between({0, 1, 0}, hit.normal) * random_rays[i].dir;
				random_rays[i].len = scene->camera.zfar;
				color_accumulate += trace(scene, rng, random_rays[i], depth + 1) * hit.material->color * vec3_dot(hit.normal, random_rays[i].dir) / (float)M_PI;
			}
			return color_accumulate / m_countof(random_rays);
		}
		else if (hit.material->type == material_metal) {
			struct ray reflect_ray;
			reflect_ray.origin = hit.point;
			reflect_ray.dir = reflect(ray.dir, hit.normal);
			reflect_ray.len = scene->camera.zfar;
			return trace(scene, rng, reflect_ray, depth + 1) * hit.material->color * vec3_dot(hit.normal, reflect_ray.dir);
		}
		else if (hit.material->type == material_dielectric) {
			float r_dot_n = vec3_dot(ray.dir, hit.normal);
			vec3 refracted;
			vec3 outward_normal;
			float ni_over_nt;
			float cosine;
			float reflect_prob;
			if (r_dot_n > 0) {
				outward_normal = -hit.normal;
				ni_over_nt = hit.material->refractive_index;
				cosine = hit.material->refractive_index * r_dot_n;
			}
			else {
				outward_normal = hit.normal;
				ni_over_nt = 1.0f / hit.material->refractive_index;
				cosine = -r_dot_n;
			}
			if (refract(ray.dir, outward_normal, ni_over_nt, &refracted)) {
				float r0 = (1.0f - hit.material->refractive_index) / (1.0f + hit.material->refractive_index);
				r0 = r0 * r0;
				reflect_prob = r0 + (1.0f - r0) * powf(1.0f - cosine, 5.0f);
			}
			else {
				reflect_prob = 1.0f;
			}
			if (rng->gen() < reflect_prob) {
				struct ray reflect_ray;
				reflect_ray.origin = hit.point;
				reflect_ray.dir = reflect(ray.dir, hit.normal);
				reflect_ray.len = scene->camera.zfar;
				return trace(scene, rng, reflect_ray, depth + 1) * hit.material->color * vec3_dot(hit.normal, reflect_ray.dir);
			}
			else {
				struct ray reflect_ray;
				reflect_ray.origin = hit.point;
				reflect_ray.dir = refracted;
				reflect_ray.len = scene->camera.zfar;
				return trace(scene, rng, reflect_ray, depth + 1) * hit.material->color;
			}
		}
		else {
			m_assert(false, "unimplemented material type found");
			return vec3{0, 0, 0};
		}
	}
	else {
		return vec3{0, 0, 0};
	}
}

struct thread_param {
	scene *scene;
	std::atomic<uint32> counter;
};

DWORD thread_func(void *param) {
	rng rng;
	rng.rng_state = 1;
	
	scene *scene = ((thread_param *)param)->scene;
	std::atomic<uint32> *counter = &((thread_param *)param)->counter;

	mat4 view_mat = camera_view_mat4(scene->camera);
	mat4 proj_mat = camera_project_mat4(scene->camera);
	vec4 view_port = {0, 0, (float)image_width, (float)image_height};
	uint32 image_pixel_count = image_width * image_height;

	while (true) {
		uint32 image_pixel_index = std::atomic_fetch_add(counter, 1);
		if (image_pixel_index >= image_pixel_count) {
			break;
		}
		uint32 x = image_pixel_index % image_width; 
		uint32 y = image_pixel_index / image_width;
		vec3 window_coord = {(float)x, (float)y, 0.5f};
		vec3 unproj = mat4_unproject(window_coord, view_mat, proj_mat, view_port);
		ray ray = {scene->camera.position, vec3_normalize(unproj - scene->camera.position), scene->camera.zfar};
		image[image_width * y + x] = trace(scene, &rng, ray, 1);
		uint32 progress_pixel_count = image_pixel_count / 20;
		if (image_pixel_index % progress_pixel_count == 0) {
			printf("%d..", image_pixel_index / progress_pixel_count * 5);
		}
	}

	return 0;
}

int main() {
	set_current_dir_to_exe_dir();
	
	scene *scene = new struct scene;
	initialize_scene(scene);

	thread_param thread_param;
	thread_param.scene = scene;
	thread_param.counter = 0;

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	HANDLE *thread_handles = new HANDLE[sysinfo.dwNumberOfProcessors];
	for (uint32 i = 0; i < sysinfo.dwNumberOfProcessors; i += 1) {
		thread_handles[i] = CreateThread(nullptr, 0, thread_func, &thread_param, 0, nullptr);
		m_assert(thread_handles[i], "");
	}
	for (uint32 i = 0; i < sysinfo.dwNumberOfProcessors; i += 1) {
		WaitForSingleObject(thread_handles[i], INFINITE);
	}

	for (uint32 i = 0; i < image_height; i += 1) {
		for (uint32 j = 0; j < image_width; j += 1) {
			vec3 *pixel = &image[image_width * i + j];
			for (uint32 i = 0; i < 3; i += 1) {
				(*pixel)[i] = clamp((*pixel)[i], 0.0f, 1.0f);
				(*pixel)[i] = clamp(1.055f * powf((*pixel)[i], 0.416666667f) - 0.055f, 0.0f, 1.0f);
			}
		}
	}

	u8vec4 *bmp_image = new u8vec4[image_width * image_height];
	for (uint32 i = 0; i < image_height; i += 1) {
		for (uint32 j = 0; j < image_width; j += 1) {
			vec3 *pixel = &image[image_width * i + j];
			u8vec4 *bmp_pixel = &bmp_image[image_width * i + j];
			bmp_pixel->x = uint8(255.99 * pixel->x);
			bmp_pixel->y = uint8(255.99 * pixel->y);
			bmp_pixel->z = uint8(255.99 * pixel->z);
			bmp_pixel->w = 255;
		}
	}
	rgba_image_to_bmp_file(bmp_image, image_width, image_height, "result.bmp");
}
