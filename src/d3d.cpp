/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#include "common.cpp"

#include <d3d11_1.h>
// #include <d3d12.h>
#include <directxmath.h>
#include <directxcolors.h>

using namespace DirectX;

#define m_d3d_assert(d3d_call) { HRESULT hr = d3d_call; if (FAILED(hr)) { _com_error err(hr); const char *err_msg = err.ErrorMessage(); fatal("D3D Error:\n\nCode: %s\nError: %s\nFile: %s\nLine: %d", #d3d_call, err_msg, __FILE__, __LINE__); } }

XMMATRIX xmmatrix_transform(transform *transform) {
	return XMMatrixTransformation(XMVectorSet(0, 0, 0, 0), XMQuaternionIdentity(), XMVectorSet(m_unpack3(transform->scale), 0), XMVectorSet(0, 0, 0, 0), XMVectorSet(m_unpack4(transform->rotate)), XMVectorSet(m_unpack3(transform->translate), 0));
}

struct d3d_swap_chain {
	ID3D11Device1 *device;
	ID3D11DeviceContext1 *context;

	IDXGISwapChain1 *swap_chain;
	ID3D11Texture2D *swap_chain_back_buffer;
	ID3D11RenderTargetView *swap_chain_render_target_view;
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc;
};

struct d3d {
	ID3D11Device1 *device;
	ID3D11DeviceContext1 *context;

	IDXGISwapChain1 *swap_chain;
	ID3D11Texture2D *swap_chain_back_buffer;
	ID3D11RenderTargetView *swap_chain_render_target_view;
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc;

	ID3D11Texture2D *final_framebuffer_texture;
	ID3D11ShaderResourceView *final_framebuffer_texture_view;
	ID3D11RenderTargetView *final_framebuffer_render_target_view;
	ID3D11Texture2D *final_framebuffer_depth_texture;
	ID3D11DepthStencilView *final_framebuffer_depth_view;
	uint32 final_framebuffer_width;
	uint32 final_framebuffer_height;

	ID3D11Texture2D *shadow_framebuffer_textures[2];
	ID3D11ShaderResourceView *shadow_framebuffer_texture_views[2];
	ID3D11RenderTargetView *shadow_framebuffer_render_target_views[2];
	ID3D11Texture2D *shadow_framebuffer_depth_texture;
	ID3D11DepthStencilView *shadow_framebuffer_depth_view;
	uint32 shadow_framebuffer_width;
	uint32 shadow_framebuffer_height;

	ID3D11SamplerState *repeat_sampler_state;
	ID3D11SamplerState *mirror_repeat_sampler_state;
	ID3D11SamplerState *clamp_edge_sampler_state;
	ID3D11SamplerState *shadow_framebuffer_sampler_state;
	
	ID3D11VertexShader *mesh_vs;
	ID3D11PixelShader *mesh_ps;
	ID3D11InputLayout *mesh_input_layout;
	ID3D11RasterizerState *mesh_rasterizer_state;
	ID3D11DepthStencilState *mesh_depth_stencil_state;

	ID3D11VertexShader *mesh_shadow_vs;
	ID3D11PixelShader *mesh_shadow_ps;

	ID3D11VertexShader *terrain_vs;
	ID3D11PixelShader *terrain_ps;
	ID3D11InputLayout *terrain_input_layout;
	ID3D11DepthStencilState *terrain_depth_stencil_state;

	ID3D11VertexShader *terrain_shadow_vs;
	ID3D11PixelShader *terrain_shadow_ps;

	ID3D11VertexShader *terrain_brush_vs;
	ID3D11PixelShader *terrain_brush_ps;
	ID3D11InputLayout *terrain_brush_input_layout;
	ID3D11DepthStencilState *terrain_brush_depth_stencil_state;

	ID3D11VertexShader *gaussian_blur_x_vs;
	ID3D11PixelShader *gaussian_blur_x_ps;
	ID3D11VertexShader *gaussian_blur_y_vs;
	ID3D11PixelShader *gaussian_blur_y_ps;
			
	ID3D11VertexShader *skybox_vs;
	ID3D11PixelShader *skybox_ps;
	ID3D11RasterizerState *skybox_rasterizer_state;
	ID3D11DepthStencilState *skybox_depth_stencil_state;

	ID3D11VertexShader *collision_shape_vs;
	ID3D11PixelShader *collision_shape_ps;
	ID3D11InputLayout *collision_shape_input_layout;
	ID3D11DepthStencilState *collision_shape_depth_stencil_state;
	ID3D11BlendState *collision_shape_blend_state;

	ID3D11VertexShader *reference_grid_vs;
	ID3D11PixelShader *reference_grid_ps;
	ID3D11InputLayout *reference_grid_input_layout;
	ID3D11DepthStencilState *reference_grid_depth_stencil_state;
	
	ID3D11VertexShader *final_framebuffer_to_screen_vs;
	ID3D11PixelShader *final_framebuffer_to_screen_ps;

	ID3D11VertexShader *imgui_vs;
	ID3D11PixelShader *imgui_ps;
	ID3D11InputLayout *imgui_input_layout;
	ID3D11RasterizerState *imgui_rasterizer_state;
	ID3D11BlendState *imgui_blend_state;
};

void initialize_d3d_device_swap_chain(d3d *d3d, window *window) {
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
}

void initialize_d3d_framebuffers(d3d *d3d) {
	{
		d3d->final_framebuffer_width = d3d->swap_chain_desc.Width;
		d3d->final_framebuffer_height = d3d->swap_chain_desc.Height;

		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = d3d->swap_chain_desc.Width;
		texture_desc.Height = d3d->swap_chain_desc.Height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, nullptr, &d3d->final_framebuffer_texture));
		m_d3d_assert(d3d->device->CreateRenderTargetView(d3d->final_framebuffer_texture, nullptr, &d3d->final_framebuffer_render_target_view));
		m_d3d_assert(d3d->device->CreateShaderResourceView(d3d->final_framebuffer_texture, nullptr, &d3d->final_framebuffer_texture_view));

		texture_desc.Format = DXGI_FORMAT_D32_FLOAT;
		texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, nullptr, &d3d->final_framebuffer_depth_texture));
		m_d3d_assert(d3d->device->CreateDepthStencilView(d3d->final_framebuffer_depth_texture, nullptr, &d3d->final_framebuffer_depth_view));
	}
	{
		d3d->shadow_framebuffer_width = 1024;
		d3d->shadow_framebuffer_height = 1024;

		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = d3d->shadow_framebuffer_width;
		texture_desc.Height = d3d->shadow_framebuffer_height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;
		for (uint32 i = 0; i < 2; i += 1) {
			m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, nullptr, &d3d->shadow_framebuffer_textures[i]));
			m_d3d_assert(d3d->device->CreateRenderTargetView(d3d->shadow_framebuffer_textures[i], nullptr, &d3d->shadow_framebuffer_render_target_views[i]));
			m_d3d_assert(d3d->device->CreateShaderResourceView(d3d->shadow_framebuffer_textures[i], nullptr, &d3d->shadow_framebuffer_texture_views[i]));
		}

		texture_desc.Format = DXGI_FORMAT_D32_FLOAT;
		texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, nullptr, &d3d->shadow_framebuffer_depth_texture));
		m_d3d_assert(d3d->device->CreateDepthStencilView(d3d->shadow_framebuffer_depth_texture, nullptr, &d3d->shadow_framebuffer_depth_view));
	}
}

void initialize_d3d_shaders(d3d *d3d) {
	struct shader_file_mappings {
		file_mapping vs_file_mapping;
		file_mapping ps_file_mapping;
		shader_file_mappings(struct d3d *d3d, const char *vs_file, const char *ps_file, ID3D11VertexShader **vs, ID3D11PixelShader **ps) {
			m_assert(open_file_mapping(vs_file, &vs_file_mapping, true), "");
			m_d3d_assert(d3d->device->CreateVertexShader(vs_file_mapping.ptr, vs_file_mapping.size, nullptr, vs));
			m_assert(open_file_mapping(ps_file, &ps_file_mapping, true), "");
			m_d3d_assert(d3d->device->CreatePixelShader(ps_file_mapping.ptr, ps_file_mapping.size, nullptr, ps));
		}
		~shader_file_mappings() {
			close_file_mapping(vs_file_mapping);
			close_file_mapping(ps_file_mapping);
		}
	};
	{ // mesh
		shader_file_mappings shaders(d3d, "hlsl/mesh.vs.fxc", "hlsl/mesh.ps.fxc", &d3d->mesh_vs, &d3d->mesh_ps);

		D3D11_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT", 0, DXGI_FORMAT_R16G16B16A16_UNORM, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_d3d_assert(d3d->device->CreateInputLayout(input_element_descs, m_countof(input_element_descs), shaders.vs_file_mapping.ptr, shaders.vs_file_mapping.size, &d3d->mesh_input_layout));
		
		D3D11_RASTERIZER_DESC rasterizer_state_desc = {};
		rasterizer_state_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_state_desc.CullMode = D3D11_CULL_FRONT;
		m_d3d_assert(d3d->device->CreateRasterizerState(&rasterizer_state_desc, &d3d->mesh_rasterizer_state));

		D3D11_DEPTH_STENCIL_DESC depth_stencil_state = {};
		depth_stencil_state.DepthEnable = true;
		depth_stencil_state.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_state.DepthFunc = D3D11_COMPARISON_GREATER;
		m_d3d_assert(d3d->device->CreateDepthStencilState(&depth_stencil_state, &d3d->mesh_depth_stencil_state));
	}
	{ // mesh shadow
		shader_file_mappings shaders(d3d, "hlsl/mesh_shadow.vs.fxc", "hlsl/mesh_shadow.ps.fxc", &d3d->mesh_shadow_vs, &d3d->mesh_shadow_ps);
	}
	{ // terrain
		shader_file_mappings shaders(d3d, "hlsl/terrain.vs.fxc", "hlsl/terrain.ps.fxc", &d3d->terrain_vs, &d3d->terrain_ps);

		D3D11_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_d3d_assert(d3d->device->CreateInputLayout(input_element_descs, m_countof(input_element_descs), shaders.vs_file_mapping.ptr, shaders.vs_file_mapping.size, &d3d->terrain_input_layout));
		
		D3D11_DEPTH_STENCIL_DESC depth_stencil_state = {};
		depth_stencil_state.DepthEnable = true;
		depth_stencil_state.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_state.DepthFunc = D3D11_COMPARISON_GREATER;
		m_d3d_assert(d3d->device->CreateDepthStencilState(&depth_stencil_state, &d3d->terrain_depth_stencil_state));
	}
	{ // terrain shadow
		shader_file_mappings shaders(d3d, "hlsl/terrain_shadow.vs.fxc", "hlsl/terrain_shadow.ps.fxc", &d3d->terrain_shadow_vs, &d3d->terrain_shadow_ps);
	}
	{ // terrain brush
		shader_file_mappings shaders(d3d, "hlsl/terrain_brush.vs.fxc", "hlsl/terrain_brush.ps.fxc", &d3d->terrain_brush_vs, &d3d->terrain_brush_ps);

		D3D11_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_d3d_assert(d3d->device->CreateInputLayout(input_element_descs, m_countof(input_element_descs), shaders.vs_file_mapping.ptr, shaders.vs_file_mapping.size, &d3d->terrain_brush_input_layout));

		D3D11_DEPTH_STENCIL_DESC depth_stencil_state = {};
		depth_stencil_state.DepthEnable = false;
		depth_stencil_state.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		m_d3d_assert(d3d->device->CreateDepthStencilState(&depth_stencil_state, &d3d->terrain_brush_depth_stencil_state));
	}
	{ // gaussian blur x
		shader_file_mappings shaders(d3d, "hlsl/gaussian_blur_x.vs.fxc", "hlsl/gaussian_blur_x.ps.fxc", &d3d->gaussian_blur_x_vs, &d3d->gaussian_blur_x_ps);
	}
	{ // gaussian blur y
		shader_file_mappings shaders(d3d, "hlsl/gaussian_blur_y.vs.fxc", "hlsl/gaussian_blur_y.ps.fxc", &d3d->gaussian_blur_y_vs, &d3d->gaussian_blur_y_ps);
	}
	{ // skybox
		shader_file_mappings shaders(d3d, "hlsl/skybox.vs.fxc", "hlsl/skybox.ps.fxc", &d3d->skybox_vs, &d3d->skybox_ps);

		D3D11_RASTERIZER_DESC rasterizer_state_desc = {};
		rasterizer_state_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_state_desc.CullMode = D3D11_CULL_NONE;
		m_d3d_assert(d3d->device->CreateRasterizerState(&rasterizer_state_desc, &d3d->skybox_rasterizer_state));

		D3D11_DEPTH_STENCIL_DESC depth_stencil_state = {};
		depth_stencil_state.DepthEnable = true;
		depth_stencil_state.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_state.DepthFunc = D3D11_COMPARISON_EQUAL;
		m_d3d_assert(d3d->device->CreateDepthStencilState(&depth_stencil_state, &d3d->skybox_depth_stencil_state));
	}
	{ // collision shape
		shader_file_mappings shaders(d3d, "hlsl/collision_shape.vs.fxc", "hlsl/collision_shape.ps.fxc", &d3d->collision_shape_vs, &d3d->collision_shape_ps);

		D3D11_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		m_d3d_assert(d3d->device->CreateInputLayout(input_element_descs, m_countof(input_element_descs), shaders.vs_file_mapping.ptr, shaders.vs_file_mapping.size, &d3d->collision_shape_input_layout));

		D3D11_DEPTH_STENCIL_DESC depth_stencil_state = {};
		depth_stencil_state.DepthEnable = true;
		depth_stencil_state.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_state.DepthFunc = D3D11_COMPARISON_GREATER;
		m_d3d_assert(d3d->device->CreateDepthStencilState(&depth_stencil_state, &d3d->collision_shape_depth_stencil_state));

		D3D11_BLEND_DESC blend_state_desc = {};
		blend_state_desc.RenderTarget[0].BlendEnable = true;
		blend_state_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_state_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_state_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_state_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_state_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_state_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_state_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_d3d_assert(d3d->device->CreateBlendState(&blend_state_desc, &d3d->collision_shape_blend_state));
	}
	{ // reference grid
		shader_file_mappings shaders(d3d, "hlsl/reference_grid.vs.fxc", "hlsl/reference_grid.ps.fxc", &d3d->reference_grid_vs, &d3d->reference_grid_ps);

		D3D11_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_d3d_assert(d3d->device->CreateInputLayout(input_element_descs, m_countof(input_element_descs), shaders.vs_file_mapping.ptr, shaders.vs_file_mapping.size, &d3d->reference_grid_input_layout));

		D3D11_DEPTH_STENCIL_DESC depth_stencil_state = {};
		depth_stencil_state.DepthEnable = true;
		depth_stencil_state.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_state.DepthFunc = D3D11_COMPARISON_GREATER;
		m_d3d_assert(d3d->device->CreateDepthStencilState(&depth_stencil_state, &d3d->reference_grid_depth_stencil_state));
	}
	{ // final framebuffer to screen
		shader_file_mappings shaders(d3d, "hlsl/final_framebuffer_to_screen.vs.fxc", "hlsl/final_framebuffer_to_screen.ps.fxc", &d3d->final_framebuffer_to_screen_vs, &d3d->final_framebuffer_to_screen_ps);
	}
	{ // imgui
		shader_file_mappings shaders(d3d, "hlsl/imgui.vs.fxc", "hlsl/imgui.ps.fxc", &d3d->imgui_vs, &d3d->imgui_ps);

		D3D11_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		m_d3d_assert(d3d->device->CreateInputLayout(input_element_descs, m_countof(input_element_descs), shaders.vs_file_mapping.ptr, shaders.vs_file_mapping.size, &d3d->imgui_input_layout));
		
		D3D11_RASTERIZER_DESC rasterizer_state_desc = {};
		rasterizer_state_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_state_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_state_desc.ScissorEnable = true;
		m_d3d_assert(d3d->device->CreateRasterizerState(&rasterizer_state_desc, &d3d->imgui_rasterizer_state));

		D3D11_BLEND_DESC blend_state_desc = {};
		blend_state_desc.RenderTarget[0].BlendEnable = true;
		blend_state_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_state_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_state_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_state_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_state_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_state_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_state_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_d3d_assert(d3d->device->CreateBlendState(&blend_state_desc, &d3d->imgui_blend_state));
	}
}

void initialize_d3d_samplers(d3d *d3d) {
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV	= D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MinLOD	= 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MipLODBias =	0;
	sampler_desc.MaxAnisotropy	= 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_d3d_assert(d3d->device->CreateSamplerState(&sampler_desc, &d3d->repeat_sampler_state));

	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampler_desc.AddressV	= D3D11_TEXTURE_ADDRESS_MIRROR;
	sampler_desc.AddressW	= D3D11_TEXTURE_ADDRESS_MIRROR;
	m_d3d_assert(d3d->device->CreateSamplerState(&sampler_desc, &d3d->mirror_repeat_sampler_state));

	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV	= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW	= D3D11_TEXTURE_ADDRESS_CLAMP;
	m_d3d_assert(d3d->device->CreateSamplerState(&sampler_desc, &d3d->clamp_edge_sampler_state));

	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	m_d3d_assert(d3d->device->CreateSamplerState(&sampler_desc, &d3d->shadow_framebuffer_sampler_state));
}

void initialize_d3d(d3d *d3d, window *window) {
	initialize_d3d_device_swap_chain(d3d, window);
	initialize_d3d_framebuffers(d3d);
	initialize_d3d_shaders(d3d);
	initialize_d3d_samplers(d3d);
}

void resize_d3d_swap_chain(d3d *d3d, uint32 width, uint32 height) {
	if (d3d->swap_chain_desc.Width != width || d3d->swap_chain_desc.Height != height) {
		d3d->swap_chain_render_target_view->Release();
		d3d->swap_chain_back_buffer->Release();

		m_d3d_assert(d3d->swap_chain->ResizeBuffers(d3d->swap_chain_desc.BufferCount, width, height, d3d->swap_chain_desc.Format, 0));
		m_d3d_assert(d3d->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&d3d->swap_chain_back_buffer)));
		m_d3d_assert(d3d->device->CreateRenderTargetView(d3d->swap_chain_back_buffer, nullptr, &d3d->swap_chain_render_target_view));

		d3d->swap_chain_desc.Width = width;
		d3d->swap_chain_desc.Height = height;
	
		d3d->final_framebuffer_texture_view->Release();
		d3d->final_framebuffer_render_target_view->Release();
		d3d->final_framebuffer_texture->Release();
		d3d->final_framebuffer_depth_view->Release();
		d3d->final_framebuffer_depth_texture->Release();

		initialize_d3d_framebuffers(d3d);
	}
}


