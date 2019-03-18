/***************************************************************************************************/
/*			Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.		   */
/***************************************************************************************************/

#include "common.cpp"
#include "math.cpp"

#include <atomic>

#include <d3d11_1.h>
#include <directxcolors.h>

#include <tinyxml/tinyxml2.cpp>

#define USE_GPU 0

const uint32 image_width = 1280;
const uint32 image_height = 720;
const uint32 sample_count = 16;
const uint32 bounce_count = 2;
vec4 *image = new vec4[image_width * image_height]();

struct block_position {
	uint32 x, y;
};
const uint32 block_width = 16;
const uint32 block_height = 16;
const uint32 block_pixel_count = block_width * block_height;
const uint32 block_count = (image_width / block_width) * (image_height / block_height);
const block_position *block_positions = [] {
	block_position *positions = new block_position[block_count];
	block_position current_position = {image_width / block_width / 2 * block_width, image_height / block_height / 2 * block_height};
	positions[0] = current_position;
	uint32 increment = 1;
	uint32 direction = 0;
	uint32 index = 1;
						
	auto update_current_position = [&] {
		if (index >= block_count) {
			return;
		}
		switch (direction) {
		case 0: { // right
			current_position.x += block_width;
		} break;
		case 1: { // down
			current_position.y += block_height;
		} break;
		case 2: { // left
			current_position.x -= block_width;
		} break;
		case 3: { // up
			current_position.y -= block_height;
		} break;
		}
		bool x_out_of_bound = current_position.x < 0 || current_position.x >= image_width;
		bool y_out_of_bound = current_position.y < 0 || current_position.y >= image_height;
		if (!x_out_of_bound && !y_out_of_bound) {
			positions[index++] = current_position;
		}
	};
							
	while (index < block_count) {
		for (uint32 i = 0; i < increment; i += 1) {
			update_current_position();
		}
		direction = (direction + 1) % 4;
		for (uint32 i = 0; i < increment; i += 1) {
			update_current_position();
		}
		direction = (direction + 1) % 4;
		increment += 1;
	}
	return positions;
}();

uint32 block_index = 0;
std::atomic<uint32> block_pixel_index(0);
HANDLE block_pixel_semaphore = CreateSemaphoreA(nullptr, block_pixel_count, block_pixel_count, nullptr);

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

struct scene_plane {
	plane plane;
	material material;
};

struct scene_sphere {
	sphere sphere;
	material material;
};

struct scene {
	scene_plane planes[6];
	scene_sphere spheres[5];
	camera camera;
};

#define m_d3d_assert(d3d_call) \
{ \
	HRESULT hr = d3d_call; \
	if (FAILED(hr)) { \
		const char *err_msg = _com_error(hr).ErrorMessage(); \
		if (IsDebuggerPresent()) { \
			__debugbreak(); \
		} \
		else { \
			char msg[2048]; \
			snprintf(msg, sizeof(msg), "D3D Error:\n\nCode: %s\nError: %s", #d3d_call, err_msg); \
			DWORD response; \
			char title[] = "Fatal Error"; \
			WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, title, sizeof(title) - 1, msg, (DWORD)strlen(msg), MB_OK, 0, &response, FALSE); \
		} \
		ExitProcess(1); \
	} \
}

struct d3d {
	ID3D11Device *device = nullptr;
	ID3D11DeviceContext *context = nullptr;

	IDXGISwapChain1 *swap_chain;
	ID3D11Texture2D *swap_chain_back_buffer;
	ID3D11RenderTargetView *swap_chain_render_target_view;
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc;
	
	ID3D11Texture2D *image;
	ID3D11ShaderResourceView *image_shader_resource_view;

#if USE_GPU
	ID3D11UnorderedAccessView *image_unordered_access_view;
	ID3D11Buffer *info_buffer;
	ID3D11Buffer *block_positions_buffer;
	ID3D11ShaderResourceView *block_positions_buffer_resource_view;
	ID3D11ComputeShader *trace_cs;
#endif

	ID3D11VertexShader *blit_framebuffer_vs;
	ID3D11PixelShader *blit_framebuffer_ps;

	ID3D11SamplerState *clamp_edge_sampler_state;
};

void init_d3d(d3d *d3d, window *window) {
	*d3d = {};

	ID3D11Device *device = nullptr;
	ID3D11DeviceContext *device_context = nullptr;
	IDXGIDevice *dxgi_device = nullptr;
	IDXGIAdapter *adapter = nullptr;
	IDXGIFactory1 *dxgi_factory = nullptr;
	IDXGIFactory2 *dxgi_factory_2 = nullptr;

	m_scope_exit(if (dxgi_device) dxgi_device->Release();
							 if (adapter) adapter->Release();
							 if (dxgi_factory) dxgi_factory->Release();
							 if (dxgi_factory_2) dxgi_factory_2->Release());

	uint32 create_device_flags = D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL feature_levels[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};

	m_d3d_assert(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_levels, m_countof(feature_levels), D3D11_SDK_VERSION, &device, nullptr, &device_context));
	m_d3d_assert(device->QueryInterface(__uuidof(ID3D11Device1), (void**)(&d3d->device)));
	m_d3d_assert(device_context->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&d3d->context));
	m_d3d_assert(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device));
	m_d3d_assert(dxgi_device->GetAdapter(&adapter));
	m_d3d_assert(adapter->GetParent(__uuidof(IDXGIFactory1), (void**)&dxgi_factory));
	m_d3d_assert(dxgi_factory->QueryInterface(__uuidof(IDXGIFactory2), (void**)&dxgi_factory_2));

	ID3D11Debug *debug = nullptr;
	ID3D11InfoQueue *info_queue = nullptr;
 
	m_d3d_assert(device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug));
	m_d3d_assert(device->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&info_queue));
	m_d3d_assert(info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true));
	m_d3d_assert(info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true));
	m_d3d_assert(info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true));
 	
	RECT window_rect;
	GetClientRect(window->handle, &window_rect);
	uint32 window_width = window_rect.right - window_rect.left;
	uint32 window_height = window_rect.bottom - window_rect.top;	

	d3d->swap_chain_desc = {};
	d3d->swap_chain_desc.Width = window_width;
	d3d->swap_chain_desc.Height = window_height;
	d3d->swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3d->swap_chain_desc.SampleDesc.Count = 1;
	d3d->swap_chain_desc.SampleDesc.Quality = 0;
	d3d->swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d3d->swap_chain_desc.BufferCount = 2;
	d3d->swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	m_d3d_assert(dxgi_factory_2->CreateSwapChainForHwnd(device, window->handle, &d3d->swap_chain_desc, nullptr, nullptr, &d3d->swap_chain));

	dxgi_factory->MakeWindowAssociation(window->handle, DXGI_MWA_NO_ALT_ENTER);

	m_d3d_assert(d3d->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&d3d->swap_chain_back_buffer)));
	m_d3d_assert(d3d->device->CreateRenderTargetView(d3d->swap_chain_back_buffer, nullptr, &d3d->swap_chain_render_target_view));

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = image_width;
	texture_desc.Height = image_height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture_desc.SampleDesc.Count = 1;
#if USE_GPU
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
#else
	texture_desc.Usage = D3D11_USAGE_DYNAMIC;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#endif

	D3D11_SUBRESOURCE_DATA texture_subresource_data = {};
	texture_subresource_data.pSysMem = image;
	texture_subresource_data.SysMemPitch = image_width * sizeof(vec4);

	m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, &d3d->image));
	m_d3d_assert(d3d->device->CreateShaderResourceView(d3d->image, nullptr, &d3d->image_shader_resource_view));
	
#if USE_GPU
	D3D11_UNORDERED_ACCESS_VIEW_DESC unordered_access_view_desc = {};
	unordered_access_view_desc.Format = texture_desc.Format;
	unordered_access_view_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	unordered_access_view_desc.Texture2D.MipSlice = 0;
	m_d3d_assert(d3d->device->CreateUnorderedAccessView(d3d->image, &unordered_access_view_desc, &d3d->image_unordered_access_view));

	D3D11_BUFFER_DESC buffer_desc = {};
	D3D11_SUBRESOURCE_DATA buffer_subresource_data = {};

	buffer_desc.ByteWidth = 256;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, nullptr, &d3d->info_buffer));

	buffer_desc.ByteWidth = sizeof(struct block_position) * block_count;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buffer_desc.StructureByteStride = sizeof(struct block_position);
	buffer_subresource_data.pSysMem = block_positions;

	m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, &buffer_subresource_data, &d3d->block_positions_buffer));
	m_d3d_assert(d3d->device->CreateShaderResourceView(d3d->block_positions_buffer, nullptr, &d3d->block_positions_buffer_resource_view));
#endif
	
	file_mapping shader_file_mapping;

	m_assert(file_mapping_open("hlsl/blit_framebuffer.vs.fxc", &shader_file_mapping, true));
	m_d3d_assert(d3d->device->CreateVertexShader(shader_file_mapping.ptr, shader_file_mapping.size, nullptr, &d3d->blit_framebuffer_vs));
	file_mapping_close(shader_file_mapping);

	m_assert(file_mapping_open("hlsl/blit_framebuffer.ps.fxc", &shader_file_mapping, true));
	m_d3d_assert(d3d->device->CreatePixelShader(shader_file_mapping.ptr, shader_file_mapping.size, nullptr, &d3d->blit_framebuffer_ps));
	file_mapping_close(shader_file_mapping);

#if USE_GPU
	m_assert(open_file_mapping("hlsl/trace.cs.fxc", &shader_file_mapping, true));
	m_d3d_assert(d3d->device->CreateComputeShader(shader_file_mapping.ptr, shader_file_mapping.size, nullptr, &d3d->trace_cs));
	close_file_mapping(shader_file_mapping);
#endif	

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV	= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW	= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MinLOD	= 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MipLODBias =	0;
	sampler_desc.MaxAnisotropy	= 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_d3d_assert(d3d->device->CreateSamplerState(&sampler_desc, &d3d->clamp_edge_sampler_state));
}

void initialize_scene(scene *scene) {
	scene->planes[0] = {plane{{0, 1, 0}, 0}, material{material_diffuse, {0.7f, 0.7f, 0.7f}}};
	scene->planes[1] = {plane{{0, -1, 0}, -15}, material{material_diffuse, {0.7f, 0.7f, 0.7f}}};
	scene->planes[2] = {plane{{0, 0, 1}, -5}, material{material_diffuse, {0.7f, 0.7f, 0.7f}}};
	scene->planes[3] = {plane{{0, 0, -1}, -25}, material{material_diffuse, {0.7f, 0.7f, 0.7f}}};
	scene->planes[4] = {plane{{1, 0, 0}, -9}, material{material_diffuse, {0.7f, 0, 0}}};
	scene->planes[5] = {plane{{-1, 0, 0}, -9}, material{material_diffuse, {0, 0.7f, 0}}};

	scene->spheres[0] = {sphere{{0, 11, 0}, 2}, material{material_emissive, {10.0f, 10.0f, 10.0f}}};
	scene->spheres[1] = {sphere{{-5, 2, 0}, 2}, material{material_metal, {0.75f, 0.75f, 0.75f}}};
	scene->spheres[2] = {sphere{{-3, 2, 9}, 2}, material{material_diffuse, {0.9f, 0.9f, 0.05f}}};
	scene->spheres[3] = {sphere{{5, 2, 0}, 2}, material{material_diffuse, {0.121f, 0.533f, 1.0f}}};
	scene->spheres[4] = {sphere{{2, 2, 7}, 2}, material{material_metal, {0.75f, 0.75f, 0.75f}, 1.3f}};
	
	scene->camera.position = {0, 10, 22};
	scene->camera.view = -vec3_normalize(scene->camera.position);
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
	for (uint32 i = 0; i < m_countof(scene->planes); i += 1) {
		float t;
		if (ray_hit_plane(ray, scene->planes[i].plane, &t)) {
			if (t > 0.0001f) {
				vec3 p = ray.origin + ray.dir * t;
				ray_hits[ray_hit_count++] = {t, p, scene->planes[i].plane.normal, &scene->planes[i].material};
			}
		}
	}
	for (uint32 i = 0; i < m_countof(scene->spheres); i += 1) {
		float t;
		if (ray_hit_sphere(ray, scene->spheres[i].sphere, &t)) {
			if (t > 0.0001f) {
				vec3 p = ray.origin + ray.dir * t;
				ray_hits[ray_hit_count++] = {t, p, vec3_normalize(p - scene->spheres[i].sphere.center), &scene->spheres[i].material};
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

vec3 trace(scene *scene, rng *rng, ray ray, uint32 bounce) {
	ray_hit hit;
	if (bounce <= bounce_count && ray_first_hit(scene, ray, &hit)) {
		if (hit.material->type == material_emissive) {
			return hit.material->color;
		}
		else if (hit.material->type == material_diffuse) {
			vec3 diffuse_color = {0, 0, 0};
			struct ray random_rays[sample_count];
			for (uint32 i = 0; i < m_countof(random_rays); i += 1) {
				vec3 dir = {};
				float pdf = 0;
				cosine_weighted_sample_hemisphere(rng->gen(), rng->gen(), &dir, &pdf);
				random_rays[i].origin = hit.point;
				random_rays[i].dir = quat_from_between(vec3{0, 1, 0}, hit.normal) * dir;
				random_rays[i].len = scene->camera.zfar;
				diffuse_color += trace(scene, rng, random_rays[i], bounce + 1) * hit.material->color * vec3_dot(hit.normal, random_rays[i].dir) / (float)M_PI / pdf;
			}
			diffuse_color /= m_countof(random_rays);
			return diffuse_color;
		}
		else if (hit.material->type == material_metal) {
			struct ray reflect_ray;
			reflect_ray.origin = hit.point;
			reflect_ray.dir = reflect(ray.dir, hit.normal);
			reflect_ray.len = scene->camera.zfar;
			float dot = vec3_dot(hit.normal, reflect_ray.dir);
			if (dot > 0) {
				return trace(scene, rng, reflect_ray, bounce + 1) * dot * hit.material->color;
			}
			else {
				return {0, 0, 0};
			}
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
				return trace(scene, rng, reflect_ray, bounce + 1) * vec3_dot(hit.normal, reflect_ray.dir);
			}
			else {
				struct ray refract_ray;
				refract_ray.origin = hit.point;
				refract_ray.dir = refracted;
				refract_ray.len = scene->camera.zfar;
				return trace(scene, rng, refract_ray, bounce + 1);
			}
		}
		else {
			m_assert(false);
			return vec3{0, 0, 0};
		}
	}
	else {
		return vec3{0, 0, 0};
	}
}

struct thread_param {
	scene *scene;
};

DWORD thread_func(void *param) {
	rng rng;
	rng.rng_state = 1;
	
	scene *scene = ((thread_param *)param)->scene;

	mat4 view_mat = camera_view_mat4(scene->camera);
	mat4 proj_mat = camera_project_mat4(scene->camera);
	vec4 view_port = {0, 0, (float)image_width, (float)image_height};

	while (true) {
		uint32 pixel_index = block_pixel_index.load();
		do {
			if (pixel_index >= block_pixel_count) {
				break;
			}
		} while (!block_pixel_index.compare_exchange_strong(pixel_index, pixel_index + 1));

		if (pixel_index < block_pixel_count) {
			uint32 x = block_positions[block_index].x + pixel_index % block_width;
			uint32 y = block_positions[block_index].y + pixel_index / block_width;
			vec3 window_coord = {(float)x, (float)(image_height - y), 0.5f};
			vec3 unproj = mat4_unproject(window_coord, view_mat, proj_mat, view_port);
			ray ray = {scene->camera.position, vec3_normalize(unproj - scene->camera.position), scene->camera.zfar};
			vec3 color = trace(scene, &rng, ray, 0);
			for (uint32 i = 0; i < 3; i += 1) {
				color[i] = clamp(color[i], 0.0f, 1.0f);
			}
			image[image_width * y + x] = vec4{color.x, color.y, color.z, 0};
			WaitForSingleObject(block_pixel_semaphore, INFINITE);
		}
	}

	return 0;
}

bool current_block_done() {
	if (block_index >= block_count) {
		return false;
	}
	LONG previous_count = 0;
	return ReleaseSemaphore(block_pixel_semaphore, block_pixel_count, &previous_count);
}

void next_block() {
	block_index += 1;
	if (block_index < block_count) {
		block_pixel_index.store(0);
	}
}

LRESULT window_message_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		default: {
			return DefWindowProcA(hwnd, msg, wparam, lparam);
		} break;
		case WM_CLOSE:
		case WM_QUIT: {
			ExitProcess(0);
			return 0;
		} break;
	}
}

int main() {
	set_current_dir_to_exe_dir();

	window *window = new struct window;
	window_init(window, window_message_callback);
	window_show(window);
	
	scene *scene = new struct scene;
	initialize_scene(scene);

	d3d *d3d = new struct d3d;
	init_d3d(d3d, window);

#if USE_GPU
	while (true) {
		handle_window_messages(window);

		d3d->context->CSSetShader(d3d->trace_cs, nullptr, 0);
		d3d->context->CSSetUnorderedAccessViews(0, 1, &d3d->image_unordered_access_view, nullptr);
		d3d->context->CSSetShaderResources(0, 1, &d3d->block_positions_buffer_resource_view);
		D3D11_MAPPED_SUBRESOURCE mapped_subresource = {};
		m_d3d_assert(d3d->context->Map(d3d->info_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource));
		*(uint32 *)mapped_subresource.pData = block_index;
		block_index = min(block_index + 1, block_count - 1);
		d3d->context->Unmap(d3d->info_buffer, 0);
		d3d->context->CSSetConstantBuffers(0, 1, &d3d->info_buffer);
		d3d->context->Dispatch(1, 1, 1);
		ID3D11UnorderedAccessView *uav = nullptr;
		d3d->context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)d3d->swap_chain_desc.Width;
		viewport.Height = (float)d3d->swap_chain_desc.Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		d3d->context->RSSetViewports(1, &viewport);

		d3d->context->ClearRenderTargetView(d3d->swap_chain_render_target_view, DirectX::Colors::Black);
		d3d->context->OMSetRenderTargets(1, &d3d->swap_chain_render_target_view, nullptr);

		d3d->context->VSSetShader(d3d->blit_framebuffer_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->blit_framebuffer_ps, nullptr, 0);
		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->context->RSSetState(nullptr);
		d3d->context->PSSetShaderResources(0, 1, &d3d->image_shader_resource_view);
		d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
		d3d->context->Draw(3, 0);
		ID3D11ShaderResourceView *srv = nullptr;
		d3d->context->PSSetShaderResources(0, 1, &srv);

		m_d3d_assert(d3d->swap_chain->Present(0, 0));
	}
#else
	thread_param thread_param;
	thread_param.scene = scene;

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	uint32 thread_count = sysinfo.dwNumberOfProcessors - 1;
	HANDLE *thread_handles = new HANDLE[thread_count];
	for (uint32 i = 0; i < thread_count; i += 1) {
		thread_handles[i] = CreateThread(nullptr, 0, thread_func, &thread_param, 0, nullptr);
		m_assert(thread_handles[i]);
	}

	while (true) {
		window_handle_messages(window);
		if (current_block_done()) {
			D3D11_MAPPED_SUBRESOURCE mapped_subresource = {};
			m_d3d_assert(d3d->context->Map(d3d->image, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource));
			memcpy(mapped_subresource.pData, image, image_width * image_height * sizeof(vec4));
			d3d->context->Unmap(d3d->image, 0);

			next_block();
			
			D3D11_VIEWPORT viewport = {};
			viewport.Width = (float)d3d->swap_chain_desc.Width;
			viewport.Height = (float)d3d->swap_chain_desc.Height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			d3d->context->RSSetViewports(1, &viewport);

			d3d->context->ClearRenderTargetView(d3d->swap_chain_render_target_view, DirectX::Colors::Black);
			d3d->context->OMSetRenderTargets(1, &d3d->swap_chain_render_target_view, nullptr);

			d3d->context->VSSetShader(d3d->blit_framebuffer_vs, nullptr, 0);
			d3d->context->PSSetShader(d3d->blit_framebuffer_ps, nullptr, 0);
			d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			d3d->context->RSSetState(nullptr);
			d3d->context->PSSetShaderResources(0, 1, &d3d->image_shader_resource_view);
			d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
			d3d->context->Draw(3, 0);

			m_d3d_assert(d3d->swap_chain->Present(0, 0));
		}
	}
#endif
}
