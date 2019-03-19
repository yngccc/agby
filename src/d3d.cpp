/****************************************************************************************************/
/*			Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.			*/
/****************************************************************************************************/

#ifndef __D3D_CPP__
#define __D3D_CPP__

#include "common.cpp"
#include "math.cpp"

#include <d3d11_1.h>
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>
#include <dxcapi.h>
	
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

#define m_d3d_set_name(d3d_resource, d3d_resource_name, ...) \
{ \
	wchar_t name[128] = {}; \
	_snwprintf(name, sizeof(name), L##d3d_resource_name, __VA_ARGS__); \
	(d3d_resource)->SetName(name); \
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

const uint32 d3d12_swap_chain_buffer_count = 2;

struct d3d12 {
	ID3D12CommandQueue *command_queue;
	ID3D12CommandAllocator *command_allocator;
	ID3D12GraphicsCommandList4 *command_list;
	ID3D12Fence *command_list_fence;
	uint64 command_list_fence_value;
	HANDLE command_list_fence_event;

	ID3D12Resource *swap_chain_render_targets[d3d12_swap_chain_buffer_count];
	uint32 swap_chain_buffer_index;
	ID3D12DescriptorHeap *swap_chain_render_targets_descriptor_heap;

	ID3D12RootSignature *blit_to_swap_chain_root_signature;
	ID3D12PipelineState *blit_to_swap_chain_pipeline_state;
	ID3D12RootSignature *pbr_mesh_root_signature;
	ID3D12PipelineState *pbr_mesh_pipeline_state;
	ID3D12RootSignature *gbuffer_root_signature;
	ID3D12PipelineState *gbuffer_pipeline_state;
	ID3D12RootSignature *gbuffer_direct_lit_root_signature;
	ID3D12PipelineState *gbuffer_direct_lit_pipeline_state;

	ID3D12Resource *copy_buffer;
	ID3D12Resource *dither_texture;

	ID3D12Device5 *device;
	IDXGISwapChain4 *swap_chain;
	IDXGIAdapter1 *adapter;
	IDXGIFactory5 *factory;
	IDXGIDebug *dxgi_debug;
	IDXGIInfoQueue *dxgi_info_queue;
	bool dxr_support;
};

struct hlsl_bytecode_file {
	file_mapping file_mapping;
	D3D12_SHADER_BYTECODE bytecode;
	hlsl_bytecode_file(const char *file) {
		m_assert(file_mapping_open(file, &file_mapping, true));
		bytecode.pShaderBytecode = file_mapping.ptr;
		bytecode.BytecodeLength = file_mapping.size;
	}
	~hlsl_bytecode_file() {
		file_mapping_close(file_mapping);
	}
};

void d3d12_init(d3d12 *d3d12, window *window) {
	*d3d12 = {};

	uint32 dxgi_factory_flags = 0;

	ID3D12Debug1 *debug_controller;
	m_d3d_assert(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)));
	debug_controller->EnableDebugLayer();
	debug_controller->SetEnableGPUBasedValidation(true);
	debug_controller->SetEnableSynchronizedCommandQueueValidation(true);
	dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
	debug_controller->Release();

	m_d3d_assert(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&d3d12->dxgi_debug)));
	m_d3d_assert(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&d3d12->dxgi_info_queue)));
	d3d12->dxgi_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
	d3d12->dxgi_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

	m_d3d_assert(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&d3d12->factory)));

	for (uint32 i = 0; d3d12->factory->EnumAdapters1(i, &d3d12->adapter) != DXGI_ERROR_NOT_FOUND; i += 1) {
		DXGI_ADAPTER_DESC1 adapter_desc;
		d3d12->adapter->GetDesc1(&adapter_desc);
		if (!(adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) {
			if (SUCCEEDED(D3D12CreateDevice(d3d12->adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr))) {
				break;
			}
		}
	}
	m_d3d_assert(D3D12CreateDevice(d3d12->adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&d3d12->device)));

	D3D12_FEATURE_DATA_D3D12_OPTIONS5 features;
	m_d3d_assert(d3d12->device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &features, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)));
	d3d12->dxr_support = features.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;

	D3D12_COMMAND_QUEUE_DESC queue_desc = {};
	queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	m_d3d_assert(d3d12->device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&d3d12->command_queue)));
	m_d3d_assert(d3d12->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&d3d12->command_allocator)));
	m_d3d_assert(d3d12->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3d12->command_allocator, nullptr, IID_PPV_ARGS(&d3d12->command_list)));
	d3d12->command_list->Close();

	m_d3d_assert(d3d12->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12->command_list_fence)));
	d3d12->command_list_fence_value = 0;
	d3d12->command_list_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	m_assert(d3d12->command_list_fence_event);

	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
	swap_chain_desc.Width = window->width;
	swap_chain_desc.Height = window->height;
	swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = d3d12_swap_chain_buffer_count;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	m_d3d_assert(d3d12->factory->CreateSwapChainForHwnd(d3d12->command_queue, window->handle, &swap_chain_desc, nullptr, nullptr, (IDXGISwapChain1 **)&d3d12->swap_chain));
	d3d12->swap_chain_buffer_index = d3d12->swap_chain->GetCurrentBackBufferIndex();

	D3D12_DESCRIPTOR_HEAP_DESC rtv_descriptor_heap_desc = {};
	rtv_descriptor_heap_desc.NumDescriptors = d3d12_swap_chain_buffer_count;
	rtv_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	m_d3d_assert(d3d12->device->CreateDescriptorHeap(&rtv_descriptor_heap_desc, IID_PPV_ARGS(&d3d12->swap_chain_render_targets_descriptor_heap)));
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = d3d12->swap_chain_render_targets_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
	for (uint32 i = 0; i < d3d12_swap_chain_buffer_count; i += 1) {
		m_d3d_assert(d3d12->swap_chain->GetBuffer(i, IID_PPV_ARGS(&d3d12->swap_chain_render_targets[i])));
		d3d12->device->CreateRenderTargetView(d3d12->swap_chain_render_targets[i], nullptr, rtv_handle);
		rtv_handle.ptr += d3d12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_d3d_set_name(d3d12->swap_chain_render_targets[i], "swap chain backbuffer #%u", i);
	}

	void d3d12_init_common_resources(struct d3d12 *);
	void d3d12_init_common_pipelines(struct d3d12 *);
	d3d12_init_common_resources(d3d12);
	d3d12_init_common_pipelines(d3d12);
}

void d3d12_init_common_resources(d3d12 *d3d12) {
	uint32 copy_buffer_size = m_megabytes(32);
	D3D12_HEAP_PROPERTIES heap_props = { D3D12_HEAP_TYPE_UPLOAD };
	D3D12_RESOURCE_DESC copy_buffer_desc = { D3D12_RESOURCE_DIMENSION_BUFFER, 0, copy_buffer_size, 1, 1, 1, DXGI_FORMAT_UNKNOWN, {1, 0}, D3D12_TEXTURE_LAYOUT_ROW_MAJOR };
	m_d3d_assert(d3d12->device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &copy_buffer_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&d3d12->copy_buffer)));

	uint8 dither_texture[] = {
		0, 32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
		48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
		12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
		60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
		3, 35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
		51, 19, 59, 27, 49, 17, 57, 25,
		15, 47,  7, 39, 13, 45,  5, 37,
		63, 31, 55, 23, 61, 29, 53, 21
	};
	ID3D12Resource *d3d12_create_texture_2d(struct d3d12 *, uint32, uint32, uint32, DXGI_FORMAT, D3D12_HEAP_TYPE, D3D12_RESOURCE_FLAGS, D3D12_RESOURCE_STATES);
	void d3d12_copy_texture_2d(struct d3d12 *, ID3D12Resource *, uint8 *, D3D12_RESOURCE_STATES);
	d3d12->dither_texture = d3d12_create_texture_2d(d3d12, 8, 8, 1, DXGI_FORMAT_R8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	d3d12_copy_texture_2d(d3d12, d3d12->dither_texture, dither_texture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void d3d12_init_common_pipelines(d3d12 *d3d12) {
	{
		D3D12_DESCRIPTOR_RANGE desc_ranges[1] = {};
		desc_ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		desc_ranges[0].NumDescriptors = 2;
		desc_ranges[0].BaseShaderRegister = 0;

		D3D12_ROOT_PARAMETER params[1] = {};
		params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[0].DescriptorTable.NumDescriptorRanges = m_countof(desc_ranges);
		params[0].DescriptorTable.pDescriptorRanges = desc_ranges;
		params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC samplers[2] = {};
		samplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].ShaderRegister = 0;
		samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplers[1].ShaderRegister = 1;
		samplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC sig_desc = {};
		sig_desc.NumParameters = m_countof(params);
		sig_desc.pParameters = params;
		sig_desc.NumStaticSamplers = m_countof(samplers);
		sig_desc.pStaticSamplers = samplers;
		sig_desc.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		ID3DBlob *sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		d3d12->device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&d3d12->blit_to_swap_chain_root_signature));
		sig_blob->Release();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = d3d12->blit_to_swap_chain_root_signature;
		hlsl_bytecode_file vs_file("hlsl/blit_to_swap_chain.vps.vs.bytecode");
		hlsl_bytecode_file ps_file("hlsl/blit_to_swap_chain.vps.ps.bytecode");
		pso_desc.VS = vs_file.bytecode;
		pso_desc.PS = ps_file.bytecode;
		pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		pso_desc.InputLayout = {};
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso_desc.SampleDesc.Count = 1;
		m_d3d_assert(d3d12->device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&d3d12->blit_to_swap_chain_pipeline_state)));
	}
	{
		D3D12_ROOT_PARAMETER params[5] = {};
		for (uint32 i = 0; i < 4; i += 1) {
			params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			params[i].Descriptor.ShaderRegister = i;
			params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		}
		params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		D3D12_DESCRIPTOR_RANGE range = {};
		range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range.NumDescriptors = 4;
		range.BaseShaderRegister = 0;
		range.OffsetInDescriptorsFromTableStart = 0;
		params[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[4].DescriptorTable.NumDescriptorRanges = 1;
		params[4].DescriptorTable.pDescriptorRanges = &range;
		params[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC sig_desc = {};
		sig_desc.NumParameters = m_countof(params);
		sig_desc.pParameters = params;
		sig_desc.NumStaticSamplers = 1;
		sig_desc.pStaticSamplers = &sampler;
		sig_desc.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		ID3DBlob *sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		d3d12->device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&d3d12->pbr_mesh_root_signature));
		sig_blob->Release();

		D3D12_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = d3d12->pbr_mesh_root_signature;
		hlsl_bytecode_file vs_file("hlsl/pbr_mesh.vps.vs.bytecode");
		hlsl_bytecode_file ps_file("hlsl/pbr_mesh.vps.ps.bytecode");
		pso_desc.VS = vs_file.bytecode;
		pso_desc.PS = ps_file.bytecode;
		pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		pso_desc.RasterizerState.FrontCounterClockwise = true;
		pso_desc.DepthStencilState.DepthEnable = true;
		pso_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pso_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		pso_desc.InputLayout = { input_element_descs, m_countof(input_element_descs) };
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		pso_desc.SampleDesc.Count = 1;
		m_d3d_assert(d3d12->device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&d3d12->pbr_mesh_pipeline_state)));
	}
	{
		D3D12_ROOT_PARAMETER params[5] = {};
		for (uint32 i = 0; i < 4; i += 1) {
			params[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			params[i].Descriptor.ShaderRegister = i;
			params[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		}
		params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		D3D12_DESCRIPTOR_RANGE range = {};
		range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range.NumDescriptors = 4;
		range.BaseShaderRegister = 0;
		range.OffsetInDescriptorsFromTableStart = 0;
		params[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[4].DescriptorTable.NumDescriptorRanges = 1;
		params[4].DescriptorTable.pDescriptorRanges = &range;
		params[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC sig_desc = {};
		sig_desc.NumParameters = m_countof(params);
		sig_desc.pParameters = params;
		sig_desc.NumStaticSamplers = 1;
		sig_desc.pStaticSamplers = &sampler;
		sig_desc.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		ID3DBlob *sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		d3d12->device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&d3d12->gbuffer_root_signature));
		sig_blob->Release();

		D3D12_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = d3d12->gbuffer_root_signature;
		hlsl_bytecode_file vs_file("hlsl/gbuffer.vps.vs.bytecode");
		hlsl_bytecode_file ps_file("hlsl/gbuffer.vps.ps.bytecode");
		pso_desc.VS = vs_file.bytecode;
		pso_desc.PS = ps_file.bytecode;
		pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		pso_desc.RasterizerState.FrontCounterClockwise = true;
		pso_desc.DepthStencilState.DepthEnable = true;
		pso_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pso_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		pso_desc.InputLayout = { input_element_descs, m_countof(input_element_descs) };
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 4;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso_desc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		pso_desc.RTVFormats[2] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pso_desc.RTVFormats[3] = DXGI_FORMAT_R8G8_UNORM;
		pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		pso_desc.SampleDesc.Count = 1;
		m_d3d_assert(d3d12->device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&d3d12->gbuffer_pipeline_state)));
	}
	{
		D3D12_DESCRIPTOR_RANGE desc_ranges[1] = {};
		desc_ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		desc_ranges[0].NumDescriptors = 4;
		desc_ranges[0].BaseShaderRegister = 0;

		D3D12_ROOT_PARAMETER params[2] = {};
		params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[1].DescriptorTable.NumDescriptorRanges = m_countof(desc_ranges);
		params[1].DescriptorTable.pDescriptorRanges = desc_ranges;
		params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC samplers[1] = {};
		samplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].ShaderRegister = 0;
		samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC sig_desc = {};
		sig_desc.NumParameters = m_countof(params);
		sig_desc.pParameters = params;
		sig_desc.NumStaticSamplers = m_countof(samplers);
		sig_desc.pStaticSamplers = samplers;
		sig_desc.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		ID3DBlob *sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		d3d12->device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&d3d12->gbuffer_direct_lit_root_signature));
		sig_blob->Release();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = d3d12->gbuffer_direct_lit_root_signature;
		hlsl_bytecode_file vs_file("hlsl/gbuffer_direct_lit.vps.vs.bytecode");
		hlsl_bytecode_file ps_file("hlsl/gbuffer_direct_lit.vps.ps.bytecode");
		pso_desc.VS = vs_file.bytecode;
		pso_desc.PS = ps_file.bytecode;
		pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		pso_desc.InputLayout = {};
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pso_desc.SampleDesc.Count = 1;
		m_d3d_assert(d3d12->device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&d3d12->gbuffer_direct_lit_pipeline_state)));
	}
}

void d3d12_wait_command_list(d3d12 *d3d12) {
	d3d12->command_list_fence_value += 1;
	d3d12->command_queue->Signal(d3d12->command_list_fence, d3d12->command_list_fence_value);
	d3d12->command_list_fence->SetEventOnCompletion(d3d12->command_list_fence_value, d3d12->command_list_fence_event);
	WaitForSingleObject(d3d12->command_list_fence_event, INFINITE);
}

void d3d12_resize_swap_chain(d3d12 *d3d12, uint32 width, uint32 height) {
	d3d12_wait_command_list(d3d12);
	for (uint32 i = 0; i < d3d12_swap_chain_buffer_count; i += 1) {
		d3d12->swap_chain_render_targets[i]->Release();
	}
	d3d12->swap_chain->ResizeBuffers(d3d12_swap_chain_buffer_count, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = d3d12->swap_chain_render_targets_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
	for (uint32 i = 0; i < d3d12_swap_chain_buffer_count; i += 1) {
		m_d3d_assert(d3d12->swap_chain->GetBuffer(i, IID_PPV_ARGS(&d3d12->swap_chain_render_targets[i])));
		d3d12->device->CreateRenderTargetView(d3d12->swap_chain_render_targets[i], nullptr, rtv_handle);
		rtv_handle.ptr += d3d12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_d3d_set_name(d3d12->swap_chain_render_targets[i], "swap chain backbuffer #%u", i);
	}
	d3d12->swap_chain_buffer_index = d3d12->swap_chain->GetCurrentBackBufferIndex();
}

ID3D12Resource *d3d12_create_buffer(d3d12 *d3d12, uint64 size, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_FLAGS resource_flags, D3D12_RESOURCE_STATES resource_state) {
	D3D12_HEAP_PROPERTIES heap_prop = {};
	heap_prop.Type = heap_type;

	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource_desc.Width = size;
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource_desc.Flags = resource_flags;

	ID3D12Resource *buffer = nullptr;
	m_d3d_assert(d3d12->device->CreateCommittedResource(&heap_prop, D3D12_HEAP_FLAG_NONE, &resource_desc, resource_state, nullptr, IID_PPV_ARGS(&buffer)));
	return buffer;
}

void d3d12_copy_buffer(d3d12 *d3d12, ID3D12Resource *dst_buffer, uint8 *src_buffer, D3D12_RESOURCE_STATES resource_state) {
	D3D12_RESOURCE_DESC dst_buffer_desc = dst_buffer->GetDesc();
	D3D12_RESOURCE_DESC copy_buffer_desc = d3d12->copy_buffer->GetDesc();

	if (dst_buffer_desc.Width > copy_buffer_desc.Width) {
		d3d12->copy_buffer->Release();
		copy_buffer_desc.Width = dst_buffer_desc.Width * 2;
		D3D12_HEAP_PROPERTIES heap_props = { D3D12_HEAP_TYPE_UPLOAD };
		m_d3d_assert(d3d12->device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &copy_buffer_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&d3d12->copy_buffer)));
	}

	uint8 *copy_buffer_ptr = nullptr;
	D3D12_RANGE copy_range = { 0, dst_buffer_desc.Width };
	m_d3d_assert(d3d12->copy_buffer->Map(0, &copy_range, (void **)&copy_buffer_ptr));
	memcpy(copy_buffer_ptr, src_buffer, dst_buffer_desc.Width);
	d3d12->copy_buffer->Unmap(0, &copy_range);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dst_buffer;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = resource_state;

	m_d3d_assert(d3d12->command_allocator->Reset());
	m_d3d_assert(d3d12->command_list->Reset(d3d12->command_allocator, nullptr));
	d3d12->command_list->CopyBufferRegion(dst_buffer, 0, d3d12->copy_buffer, 0, dst_buffer_desc.Width);
	d3d12->command_list->ResourceBarrier(1, &barrier);
	m_d3d_assert(d3d12->command_list->Close());
	d3d12->command_queue->ExecuteCommandLists(1, (ID3D12CommandList**)&d3d12->command_list);

	d3d12_wait_command_list(d3d12);
}

ID3D12Resource *d3d12_create_texture_2d(d3d12 *d3d12, uint32 width, uint32 height, uint32 mips, DXGI_FORMAT format, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_FLAGS resource_flags, D3D12_RESOURCE_STATES resource_state) {
	D3D12_HEAP_PROPERTIES heap_props = {};
	heap_props.Type = heap_type;

	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resource_desc.Alignment = 0;
	resource_desc.Width = width;
	resource_desc.Height = height;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = mips;
	resource_desc.Format = format;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resource_desc.Flags = resource_flags;

	ID3D12Resource *texture = nullptr;
	if (resource_flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || resource_flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) {
		D3D12_CLEAR_VALUE clear_value = {};
		clear_value.Format = format;
		m_d3d_assert(d3d12->device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc, resource_state, &clear_value, IID_PPV_ARGS(&texture)));
	}
	else {
		m_d3d_assert(d3d12->device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc, resource_state, nullptr, IID_PPV_ARGS(&texture)));
	}
	return texture;
}

void d3d12_copy_texture_2d(d3d12 *d3d12, ID3D12Resource *dst_texture, uint8 *src_texture, D3D12_RESOURCE_STATES resource_state) {
	D3D12_RESOURCE_DESC texture_desc = dst_texture->GetDesc();

	const uint32 max_mip_count = 16;
	m_assert(texture_desc.MipLevels <= max_mip_count);
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprints[max_mip_count] = {};
	uint32 row_counts[max_mip_count] = {};
	uint64 row_sizes[max_mip_count] = {};
	uint64 total_size = 0;
	d3d12->device->GetCopyableFootprints(&texture_desc, 0, texture_desc.MipLevels, 0, footprints, row_counts, row_sizes, &total_size);

	D3D12_RESOURCE_DESC copy_buffer_desc = d3d12->copy_buffer->GetDesc();
	if (copy_buffer_desc.Width < total_size) {
		d3d12->copy_buffer->Release();
		copy_buffer_desc.Width = (uint64)(total_size * 1.5);
		D3D12_HEAP_PROPERTIES heap_props = { D3D12_HEAP_TYPE_UPLOAD };
		m_d3d_assert(d3d12->device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &copy_buffer_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&d3d12->copy_buffer)));
	}

	uint8 *copy_buffer_ptr = nullptr;
	D3D12_RANGE copy_range = { 0, total_size };
	m_d3d_assert(d3d12->copy_buffer->Map(0, &copy_range, (void **)&copy_buffer_ptr));
	uint64 dst_texture_offset = 0;
	uint64 src_texture_offset = 0;
	for (uint32 i = 0; i < texture_desc.MipLevels; i += 1) {
		for (uint32 j = 0; j < row_counts[i]; j += 1) {
			memcpy(copy_buffer_ptr + dst_texture_offset, src_texture + src_texture_offset, row_sizes[i]);
			dst_texture_offset += footprints[i].Footprint.RowPitch;
			src_texture_offset += row_sizes[i];
		}
	}
	d3d12->copy_buffer->Unmap(0, &copy_range);

	m_d3d_assert(d3d12->command_allocator->Reset());
	m_d3d_assert(d3d12->command_list->Reset(d3d12->command_allocator, nullptr));

	for (uint32 i = 0; i < texture_desc.MipLevels; i += 1) {
		D3D12_TEXTURE_COPY_LOCATION src_location = {};
		src_location.pResource = d3d12->copy_buffer;
		src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src_location.PlacedFootprint = footprints[i];

		D3D12_TEXTURE_COPY_LOCATION dst_location = {};
		dst_location.pResource = dst_texture;
		dst_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst_location.SubresourceIndex = i;

		d3d12->command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);
	}

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dst_texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = resource_state;
	d3d12->command_list->ResourceBarrier(1, &barrier);

	m_d3d_assert(d3d12->command_list->Close());
	d3d12->command_queue->ExecuteCommandLists(1, (ID3D12CommandList**)&d3d12->command_list);

	d3d12_wait_command_list(d3d12);
}

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
	D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };

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
			m_assert(file_mapping_open(vs_file, &vs_file_mapping, true));
			m_d3d_assert(d3d->device->CreateVertexShader(vs_file_mapping.ptr, vs_file_mapping.size, nullptr, vs));
			m_assert(file_mapping_open(ps_file, &ps_file_mapping, true));
			m_d3d_assert(d3d->device->CreatePixelShader(ps_file_mapping.ptr, ps_file_mapping.size, nullptr, ps));
		}
		~shader_file_mappings() {
			file_mapping_close(vs_file_mapping);
			file_mapping_close(ps_file_mapping);
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
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MipLODBias = 0;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_d3d_assert(d3d->device->CreateSamplerState(&sampler_desc, &d3d->repeat_sampler_state));

	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	m_d3d_assert(d3d->device->CreateSamplerState(&sampler_desc, &d3d->mirror_repeat_sampler_state));

	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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

#endif // __D3D_CPP__
