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
	if (hr != S_OK) { \
		const char *err_msg = _com_error(hr).ErrorMessage(); \
		if (IsDebuggerPresent()) { \
			__debugbreak(); \
		} \
		else { \
			char msg[2048] = {}; \
			snprintf(msg, sizeof(msg), "D3D Error:\n\nCode: %s\nError: %s", #d3d_call, err_msg); \
			DWORD response = 0; \
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

const uint64 d3d12_swap_chain_buffer_count = 2;

struct d3d12 {
	ID3D12Device5* device;
	IDXGIAdapter1* adapter;
	IDXGIFactory5* factory;
	IDXGIDebug* dxgi_debug;
	IDXGIInfoQueue* dxgi_info_queue;

	ID3D12CommandQueue* command_queue;
	ID3D12CommandAllocator* command_allocator;
	ID3D12GraphicsCommandList4* command_list;
	ID3D12Fence* command_list_fence;
	uint64 command_list_fence_value;
	HANDLE command_list_fence_event;

	ID3D12DescriptorHeap* rtv_descriptor_heap;
	uint32 rtv_descriptor_count;
	uint32 rtv_descriptor_capacity;
	uint32 rtv_descriptor_size;

	ID3D12DescriptorHeap* dsv_descriptor_heap;
	uint32 dsv_descriptor_count;
	uint32 dsv_descriptor_capacity;
	uint32 dsv_descriptor_size;

	ID3D12DescriptorHeap* cbv_srv_uav_descriptor_heap;
	uint32 cbv_srv_uav_descriptor_count;
	uint32 cbv_srv_uav_descriptor_capacity;
	uint32 cbv_srv_uav_descriptor_size;

	IDXGISwapChain4* swap_chain;
	ID3D12Resource* swap_chain_textures[d3d12_swap_chain_buffer_count];
	D3D12_CPU_DESCRIPTOR_HANDLE swap_chain_texture_descriptors[d3d12_swap_chain_buffer_count];
	uint32 swap_chain_buffer_index;

	ID3D12Resource* frame_constants_buffer;
	uint32 frame_constants_buffer_size;
	uint32 frame_constants_buffer_capacity;

	ID3D12Resource* copy_temp_buffer;

	ID3D12Resource* gbuffer_textures[5];
	D3D12_CPU_DESCRIPTOR_HANDLE gbuffer_texture_rtv_descriptors[m_countof(gbuffer_textures)];
	D3D12_GPU_DESCRIPTOR_HANDLE gbuffer_texture_srv_descriptors[m_countof(gbuffer_textures)];

	ID3D12Resource* render_texture;
	D3D12_CPU_DESCRIPTOR_HANDLE render_texture_rtv_descriptor;
	D3D12_GPU_DESCRIPTOR_HANDLE render_texture_srv_descriptor;

	ID3D12Resource* depth_texture;
	D3D12_CPU_DESCRIPTOR_HANDLE depth_texture_dsv_descriptor;

	ID3D12Resource* dither_texture;
	D3D12_GPU_DESCRIPTOR_HANDLE dither_texture_srv_descriptor;

	ID3D12RootSignature* blit_to_swap_chain_root_signature;
	ID3D12PipelineState* blit_to_swap_chain_pipeline_state;
	ID3D12RootSignature* gbuffer_root_signature;
	ID3D12PipelineState* gbuffer_pipeline_state;
	ID3D12RootSignature* gbuffer_direct_lit_root_signature;
	ID3D12PipelineState* gbuffer_direct_lit_pipeline_state;

	bool dxr_enabled;

	array<ID3D12Resource*> dxr_bottom_acceleration_buffers;
	ID3D12Resource* dxr_top_acceleration_buffer;

	ID3D12RootSignature* dxr_shadow_global_root_sig;
	ID3D12StateObject* dxr_shadow_pipeline_state;
	ID3D12Resource* dxr_shadow_shader_table;
	uint32 dxr_shadow_shader_table_entry_size;
	ID3D12Resource* dxr_shadow_output_texture_array;

	ID3D12RootSignature* dxr_ao_global_root_sig;
	ID3D12StateObject* dxr_ao_pipeline_state;
	ID3D12Resource* dxr_ao_shader_table;
	uint32 dxr_ao_shader_table_entry_size;
	ID3D12Resource* dxr_ao_output_texture;

	// top acceleration buffer | gbuffer position texture | gbuffer normal texture | shadow output textures | ao output texture
	ID3D12DescriptorHeap* dxr_descriptor_heap;

	void wait_command_list_completion();
	ID3D12Resource* create_buffer(uint64 size, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_FLAGS resource_flags, D3D12_RESOURCE_STATES resource_state);
	void copy_buffer(ID3D12Resource* dst_buffer, const void* src_buffer, D3D12_RESOURCE_STATES resource_state);
	ID3D12Resource* create_texture_2d(uint32 width, uint32 height, uint32 array_size, uint32 mips, DXGI_FORMAT format, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_FLAGS resource_flags, D3D12_RESOURCE_STATES resource_state);
	void copy_texture_2d(ID3D12Resource* dst_texture, uint8* src_texture, D3D12_RESOURCE_STATES resource_state);
	void append_rtv_descriptor(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE* cpu_descriptor_ptr, D3D12_GPU_DESCRIPTOR_HANDLE* gpu_descriptor_ptr);
	void append_dsv_descriptor(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE* cpu_descriptor_ptr, D3D12_GPU_DESCRIPTOR_HANDLE* gpu_descriptor_ptr);
	void append_srv_descriptor(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE* cpu_descriptor_ptr, D3D12_GPU_DESCRIPTOR_HANDLE* gpu_descriptor_ptr);
	void resize_swap_chain(uint32 width, uint32 height);
	void init_pipelines();
	void init(window* window);
};

struct hlsl_bytecode_file {
	file_mapping file_mapping;
	D3D12_SHADER_BYTECODE bytecode;
	hlsl_bytecode_file(const char* file) {
		m_assert(file_mapping_open(file, &file_mapping, true));
		bytecode.pShaderBytecode = file_mapping.ptr;
		bytecode.BytecodeLength = file_mapping.size;
	}
	~hlsl_bytecode_file() {
		file_mapping_close(file_mapping);
	}
};

void d3d12::wait_command_list_completion() {
	command_list_fence_value += 1;
	m_d3d_assert(command_queue->Signal(command_list_fence, command_list_fence_value));
	m_d3d_assert(command_list_fence->SetEventOnCompletion(command_list_fence_value, command_list_fence_event));
	WaitForSingleObject(command_list_fence_event, INFINITE);
}

ID3D12Resource* d3d12::create_buffer(uint64 size, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_FLAGS resource_flags, D3D12_RESOURCE_STATES resource_state) {
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

	ID3D12Resource* buffer = nullptr;
	m_d3d_assert(device->CreateCommittedResource(&heap_prop, D3D12_HEAP_FLAG_NONE, &resource_desc, resource_state, nullptr, IID_PPV_ARGS(&buffer)));
	return buffer;
}

void d3d12::copy_buffer(ID3D12Resource* dst_buffer, const void* src_buffer, D3D12_RESOURCE_STATES resource_state) {
	D3D12_RESOURCE_DESC dst_buffer_desc = dst_buffer->GetDesc();
	D3D12_RESOURCE_DESC copy_buffer_desc = copy_temp_buffer->GetDesc();

	if (dst_buffer_desc.Width > copy_buffer_desc.Width) {
		copy_temp_buffer->Release();
		copy_buffer_desc.Width = dst_buffer_desc.Width * 2;
		D3D12_HEAP_PROPERTIES heap_props = { D3D12_HEAP_TYPE_UPLOAD };
		m_d3d_assert(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &copy_buffer_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&copy_temp_buffer)));
	}

	uint8* copy_buffer_ptr = nullptr;
	D3D12_RANGE copy_range = { 0, dst_buffer_desc.Width };
	m_d3d_assert(copy_temp_buffer->Map(0, &copy_range, (void**)&copy_buffer_ptr));
	memcpy(copy_buffer_ptr, src_buffer, dst_buffer_desc.Width);
	copy_temp_buffer->Unmap(0, &copy_range);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dst_buffer;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = resource_state;

	m_d3d_assert(command_allocator->Reset());
	m_d3d_assert(command_list->Reset(command_allocator, nullptr));
	command_list->CopyBufferRegion(dst_buffer, 0, copy_temp_buffer, 0, dst_buffer_desc.Width);
	command_list->ResourceBarrier(1, &barrier);
	m_d3d_assert(command_list->Close());
	command_queue->ExecuteCommandLists(1, (ID3D12CommandList**)&command_list);

	wait_command_list_completion();
}

ID3D12Resource* d3d12::create_texture_2d(uint32 width, uint32 height, uint32 array_size, uint32 mips, DXGI_FORMAT format, D3D12_HEAP_TYPE heap_type, D3D12_RESOURCE_FLAGS resource_flags, D3D12_RESOURCE_STATES resource_state) {
	D3D12_HEAP_PROPERTIES heap_props = {};
	heap_props.Type = heap_type;

	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resource_desc.Alignment = 0;
	resource_desc.Width = width;
	resource_desc.Height = height;
	resource_desc.DepthOrArraySize = array_size;
	resource_desc.MipLevels = mips;
	resource_desc.Format = format;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resource_desc.Flags = resource_flags;

	ID3D12Resource* texture = nullptr;
	if (resource_flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || resource_flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) {
		D3D12_CLEAR_VALUE clear_value = {};
		clear_value.Format = format;
		m_d3d_assert(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc, resource_state, &clear_value, IID_PPV_ARGS(&texture)));
	}
	else {
		m_d3d_assert(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc, resource_state, nullptr, IID_PPV_ARGS(&texture)));
	}
	return texture;
}

void d3d12::copy_texture_2d(ID3D12Resource* dst_texture, uint8* src_texture, D3D12_RESOURCE_STATES resource_state) {
	D3D12_RESOURCE_DESC texture_desc = dst_texture->GetDesc();

	const uint32 max_mip_count = 16;
	m_assert(texture_desc.MipLevels <= max_mip_count);
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprints[max_mip_count] = {};
	uint32 row_counts[max_mip_count] = {};
	uint64 row_sizes[max_mip_count] = {};
	uint64 total_size = 0;
	device->GetCopyableFootprints(&texture_desc, 0, texture_desc.MipLevels, 0, footprints, row_counts, row_sizes, &total_size);

	D3D12_RESOURCE_DESC copy_buffer_desc = copy_temp_buffer->GetDesc();
	if (copy_buffer_desc.Width < total_size) {
		copy_temp_buffer->Release();
		copy_buffer_desc.Width = (uint64)(total_size * 1.5);
		D3D12_HEAP_PROPERTIES heap_props = { D3D12_HEAP_TYPE_UPLOAD };
		m_d3d_assert(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &copy_buffer_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&copy_temp_buffer)));
	}

	uint8* copy_buffer_ptr = nullptr;
	D3D12_RANGE copy_range = { 0, total_size };
	m_d3d_assert(copy_temp_buffer->Map(0, &copy_range, (void**)&copy_buffer_ptr));
	uint64 dst_texture_offset = 0;
	uint64 src_texture_offset = 0;
	for (uint32 i = 0; i < texture_desc.MipLevels; i += 1) {
		for (uint32 j = 0; j < row_counts[i]; j += 1) {
			memcpy(copy_buffer_ptr + dst_texture_offset, src_texture + src_texture_offset, row_sizes[i]);
			dst_texture_offset += footprints[i].Footprint.RowPitch;
			src_texture_offset += row_sizes[i];
		}
	}
	copy_temp_buffer->Unmap(0, &copy_range);

	m_d3d_assert(command_allocator->Reset());
	m_d3d_assert(command_list->Reset(command_allocator, nullptr));

	for (uint32 i = 0; i < texture_desc.MipLevels; i += 1) {
		D3D12_TEXTURE_COPY_LOCATION src_location = {};
		src_location.pResource = copy_temp_buffer;
		src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src_location.PlacedFootprint = footprints[i];

		D3D12_TEXTURE_COPY_LOCATION dst_location = {};
		dst_location.pResource = dst_texture;
		dst_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst_location.SubresourceIndex = i;

		command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);
	}

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dst_texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = resource_state;
	command_list->ResourceBarrier(1, &barrier);

	m_d3d_assert(command_list->Close());
	command_queue->ExecuteCommandLists(1, (ID3D12CommandList**)&command_list);

	wait_command_list_completion();
}

void d3d12::append_rtv_descriptor(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE* cpu_descriptor_ptr, D3D12_GPU_DESCRIPTOR_HANDLE* gpu_descriptor_ptr) {
	m_assert(rtv_descriptor_count < rtv_descriptor_capacity);
	D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor = rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
	cpu_descriptor.ptr += rtv_descriptor_count * rtv_descriptor_size;
	device->CreateRenderTargetView(resource, nullptr, cpu_descriptor);
	if (cpu_descriptor_ptr) {
		*cpu_descriptor_ptr = cpu_descriptor;
	}
	if (gpu_descriptor_ptr) {
		D3D12_GPU_DESCRIPTOR_HANDLE gpu_descriptor = rtv_descriptor_heap->GetGPUDescriptorHandleForHeapStart();
		gpu_descriptor.ptr += rtv_descriptor_count * rtv_descriptor_size;
		*gpu_descriptor_ptr = gpu_descriptor;
	}
	rtv_descriptor_count += 1;
}

void d3d12::append_dsv_descriptor(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE* cpu_descriptor_ptr, D3D12_GPU_DESCRIPTOR_HANDLE* gpu_descriptor_ptr) {
	m_assert(dsv_descriptor_count < dsv_descriptor_capacity);
	D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor = dsv_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
	cpu_descriptor.ptr += dsv_descriptor_count * dsv_descriptor_size;
	device->CreateDepthStencilView(resource, nullptr, cpu_descriptor);
	if (cpu_descriptor_ptr) {
		*cpu_descriptor_ptr = cpu_descriptor;
	}
	if (gpu_descriptor_ptr) {
		D3D12_GPU_DESCRIPTOR_HANDLE gpu_descriptor = dsv_descriptor_heap->GetGPUDescriptorHandleForHeapStart();
		gpu_descriptor.ptr += dsv_descriptor_count * dsv_descriptor_size;
		*gpu_descriptor_ptr = gpu_descriptor;
	}
	dsv_descriptor_count += 1;
}

void d3d12::append_srv_descriptor(ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE* cpu_descriptor_ptr, D3D12_GPU_DESCRIPTOR_HANDLE* gpu_descriptor_ptr) {
	m_assert(cbv_srv_uav_descriptor_count < cbv_srv_uav_descriptor_capacity);
	D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor = cbv_srv_uav_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
	cpu_descriptor.ptr += cbv_srv_uav_descriptor_count * cbv_srv_uav_descriptor_size;
	device->CreateShaderResourceView(resource, nullptr, cpu_descriptor);
	if (cpu_descriptor_ptr) {
		*cpu_descriptor_ptr = cpu_descriptor;
	}
	if (gpu_descriptor_ptr) {
		D3D12_GPU_DESCRIPTOR_HANDLE gpu_descriptor = cbv_srv_uav_descriptor_heap->GetGPUDescriptorHandleForHeapStart();
		gpu_descriptor.ptr += cbv_srv_uav_descriptor_count * cbv_srv_uav_descriptor_size;
		*gpu_descriptor_ptr = gpu_descriptor;
	}
	cbv_srv_uav_descriptor_count += 1;
}

void d3d12::resize_swap_chain(uint32 width, uint32 height) {
	wait_command_list_completion();
	for (uint32 i = 0; i < d3d12_swap_chain_buffer_count; i += 1) {
		swap_chain_textures[i]->Release();
	}
	m_d3d_assert(swap_chain->ResizeBuffers(d3d12_swap_chain_buffer_count, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	for (uint32 i = 0; i < d3d12_swap_chain_buffer_count; i += 1) {
		swap_chain->GetBuffer(i, IID_PPV_ARGS(&swap_chain_textures[i]));
		device->CreateRenderTargetView(swap_chain_textures[i], nullptr, swap_chain_texture_descriptors[i]);
	}
}

void d3d12::init_pipelines() {
	{
		D3D12_DESCRIPTOR_RANGE desc_ranges[2] = {};
		desc_ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		desc_ranges[0].NumDescriptors = 1;
		desc_ranges[0].BaseShaderRegister = 0;
		desc_ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		desc_ranges[1].NumDescriptors = 1;
		desc_ranges[1].BaseShaderRegister = 1;

		D3D12_ROOT_PARAMETER params[2] = {};
		params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[0].DescriptorTable.NumDescriptorRanges = 1;
		params[0].DescriptorTable.pDescriptorRanges = &desc_ranges[0];
		params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[1].DescriptorTable.NumDescriptorRanges = 1;
		params[1].DescriptorTable.pDescriptorRanges = &desc_ranges[1];
		params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

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

		ID3DBlob* sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&blit_to_swap_chain_root_signature));
		sig_blob->Release();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = blit_to_swap_chain_root_signature;
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
		m_d3d_assert(device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&blit_to_swap_chain_pipeline_state)));
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
		range.NumDescriptors = 5;
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

		ID3DBlob* sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&gbuffer_root_signature));
		sig_blob->Release();

		D3D12_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = gbuffer_root_signature;
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
		pso_desc.NumRenderTargets = 5;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso_desc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		pso_desc.RTVFormats[2] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		pso_desc.RTVFormats[3] = DXGI_FORMAT_R8G8_UNORM;
		pso_desc.RTVFormats[4] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		pso_desc.SampleDesc.Count = 1;
		m_d3d_assert(device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&gbuffer_pipeline_state)));
	}
	{
		D3D12_DESCRIPTOR_RANGE desc_ranges[1] = {};
		desc_ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		desc_ranges[0].NumDescriptors = 6;
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

		ID3DBlob* sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&gbuffer_direct_lit_root_signature));
		sig_blob->Release();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = gbuffer_direct_lit_root_signature;
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
		m_d3d_assert(device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&gbuffer_direct_lit_pipeline_state)));
	}

	const wchar_t* dxr_ray_gen_str = L"ray_gen";
	const wchar_t* dxr_any_hit_str = L"any_hit";
	const wchar_t* dxr_closest_hit_str = L"closest_hit";
	const wchar_t* dxr_miss_str = L"miss";
	const wchar_t* dxr_hit_group_str = L"hit_group";
	if (dxr_enabled) {
		hlsl_bytecode_file bytecode_file("hlsl/shadow.rt.bytecode");
		D3D12_STATE_SUBOBJECT state_subobjects[10] = {};
		{
			D3D12_EXPORT_DESC export_descs[3] = { {dxr_ray_gen_str}, {dxr_miss_str}, {dxr_any_hit_str} };
			D3D12_DXIL_LIBRARY_DESC dxil_lib_desc = {};
			dxil_lib_desc.DXILLibrary.pShaderBytecode = bytecode_file.bytecode.pShaderBytecode;
			dxil_lib_desc.DXILLibrary.BytecodeLength = bytecode_file.bytecode.BytecodeLength;
			dxil_lib_desc.NumExports = m_countof(export_descs);
			dxil_lib_desc.pExports = export_descs;
			state_subobjects[0].Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
			state_subobjects[0].pDesc = &dxil_lib_desc;
		}
		{
			D3D12_HIT_GROUP_DESC hit_group_desc = {};
			hit_group_desc.HitGroupExport = dxr_hit_group_str;
			hit_group_desc.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES;
			hit_group_desc.AnyHitShaderImport = dxr_any_hit_str;
			state_subobjects[1].Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
			state_subobjects[1].pDesc = &hit_group_desc;
		}
		{
			D3D12_DESCRIPTOR_RANGE ranges[2] = {};
			ranges[1].NumDescriptors = 3;
			ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			ranges[1].OffsetInDescriptorsFromTableStart = 0;
			ranges[0].NumDescriptors = 1;
			ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			ranges[0].OffsetInDescriptorsFromTableStart = 3;

			D3D12_ROOT_PARAMETER root_params[1] = {};
			root_params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			root_params[0].DescriptorTable.NumDescriptorRanges = m_countof(ranges);
			root_params[0].DescriptorTable.pDescriptorRanges = ranges;

			D3D12_ROOT_SIGNATURE_DESC root_sig_desc = {};
			root_sig_desc.NumParameters = m_countof(root_params);
			root_sig_desc.pParameters = root_params;
			root_sig_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
			ID3D12RootSignature* root_sig = nullptr;
			ID3DBlob* sig_blob = nullptr;
			ID3DBlob* error_blob = nullptr;
			m_d3d_assert(D3D12SerializeRootSignature(&root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, &error_blob));
			m_d3d_assert(device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&root_sig)));
			sig_blob->Release();
			state_subobjects[2].Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
			state_subobjects[2].pDesc = &root_sig;

			const wchar_t* export_names[1] = { dxr_ray_gen_str };
			D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
			association.NumExports = m_countof(export_names);
			association.pExports = export_names;
			association.pSubobjectToAssociate = &state_subobjects[2];
			state_subobjects[3].Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			state_subobjects[3].pDesc = &association;
		}
		{
			D3D12_ROOT_SIGNATURE_DESC root_sig_desc = {};
			root_sig_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
			ID3D12RootSignature* root_sig = nullptr;
			ID3DBlob* sig_blob = nullptr;
			ID3DBlob* error_blob = nullptr;
			m_d3d_assert(D3D12SerializeRootSignature(&root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, &error_blob));
			m_d3d_assert(device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&root_sig)));
			sig_blob->Release();
			state_subobjects[4].Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
			state_subobjects[4].pDesc = &root_sig;

			const wchar_t* export_names[2] = { dxr_miss_str, dxr_any_hit_str };
			D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
			association.NumExports = m_countof(export_names);
			association.pExports = export_names;
			association.pSubobjectToAssociate = &state_subobjects[4];
			state_subobjects[5].Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			state_subobjects[5].pDesc = &association;
		}
		{
			D3D12_RAYTRACING_SHADER_CONFIG shader_config = {};
			shader_config.MaxPayloadSizeInBytes = sizeof(float);
			shader_config.MaxAttributeSizeInBytes = 2 * sizeof(float);
			state_subobjects[6].Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
			state_subobjects[6].pDesc = &shader_config;

			const wchar_t* export_names[3] = { dxr_miss_str, dxr_any_hit_str, dxr_ray_gen_str };
			D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
			association.NumExports = m_countof(export_names);
			association.pExports = export_names;
			association.pSubobjectToAssociate = &state_subobjects[6];
			state_subobjects[7].Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			state_subobjects[7].pDesc = &association;
		}
		{
			D3D12_RAYTRACING_PIPELINE_CONFIG pipeline_config = {};
			pipeline_config.MaxTraceRecursionDepth = 1;
			state_subobjects[8].Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
			state_subobjects[8].pDesc = &pipeline_config;
		}
		{
			D3D12_ROOT_PARAMETER root_params[1] = {};
			root_params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			root_params[0].Descriptor.ShaderRegister = 0;
			D3D12_ROOT_SIGNATURE_DESC global_root_sig_desc = {};
			global_root_sig_desc.NumParameters = m_countof(root_params);
			global_root_sig_desc.pParameters = root_params;
			ID3DBlob* global_root_sig_blob = nullptr;
			ID3DBlob* global_root_sig_error = nullptr;
			m_d3d_assert(D3D12SerializeRootSignature(&global_root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &global_root_sig_blob, &global_root_sig_error));
			m_d3d_assert(device->CreateRootSignature(0, global_root_sig_blob->GetBufferPointer(), global_root_sig_blob->GetBufferSize(), IID_PPV_ARGS(&dxr_shadow_global_root_sig)));
			global_root_sig_blob->Release();
			state_subobjects[9].Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
			state_subobjects[9].pDesc = &dxr_shadow_global_root_sig;
		}
		D3D12_STATE_OBJECT_DESC state_object_desc = {};
		state_object_desc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
		state_object_desc.NumSubobjects = m_countof(state_subobjects);
		state_object_desc.pSubobjects = state_subobjects;
		m_d3d_assert(device->CreateStateObject(&state_object_desc, IID_PPV_ARGS(&dxr_shadow_pipeline_state)));
	}
	if (dxr_enabled) {
		hlsl_bytecode_file bytecode_file("hlsl/ao.rt.bytecode");
		D3D12_STATE_SUBOBJECT state_subobjects[10] = {};
		{
			D3D12_EXPORT_DESC export_descs[3] = { {dxr_ray_gen_str}, {dxr_miss_str}, {dxr_any_hit_str} };
			D3D12_DXIL_LIBRARY_DESC dxil_lib_desc = {};
			dxil_lib_desc.DXILLibrary.pShaderBytecode = bytecode_file.bytecode.pShaderBytecode;
			dxil_lib_desc.DXILLibrary.BytecodeLength = bytecode_file.bytecode.BytecodeLength;
			dxil_lib_desc.NumExports = m_countof(export_descs);
			dxil_lib_desc.pExports = export_descs;
			state_subobjects[0].Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
			state_subobjects[0].pDesc = &dxil_lib_desc;
		}
		{
			D3D12_HIT_GROUP_DESC hit_group_desc = {};
			hit_group_desc.HitGroupExport = dxr_hit_group_str;
			hit_group_desc.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES;
			hit_group_desc.AnyHitShaderImport = dxr_any_hit_str;
			state_subobjects[1].Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
			state_subobjects[1].pDesc = &hit_group_desc;
		}
		{
			D3D12_DESCRIPTOR_RANGE ranges[2] = {};
			ranges[1].NumDescriptors = 3;
			ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			ranges[1].OffsetInDescriptorsFromTableStart = 0;
			ranges[0].NumDescriptors = 1;
			ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			ranges[0].OffsetInDescriptorsFromTableStart = 4;

			D3D12_ROOT_PARAMETER root_params[1] = {};
			root_params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			root_params[0].DescriptorTable.NumDescriptorRanges = m_countof(ranges);
			root_params[0].DescriptorTable.pDescriptorRanges = ranges;

			D3D12_ROOT_SIGNATURE_DESC root_sig_desc = {};
			root_sig_desc.NumParameters = m_countof(root_params);
			root_sig_desc.pParameters = root_params;
			root_sig_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
			ID3D12RootSignature* root_sig = nullptr;
			ID3DBlob* sig_blob = nullptr;
			ID3DBlob* error_blob = nullptr;
			m_d3d_assert(D3D12SerializeRootSignature(&root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, &error_blob));
			m_d3d_assert(device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&root_sig)));
			sig_blob->Release();
			state_subobjects[2].Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
			state_subobjects[2].pDesc = &root_sig;

			const wchar_t* export_names[1] = { dxr_ray_gen_str };
			D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
			association.NumExports = m_countof(export_names);
			association.pExports = export_names;
			association.pSubobjectToAssociate = &state_subobjects[2];
			state_subobjects[3].Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			state_subobjects[3].pDesc = &association;
		}
		{
			D3D12_ROOT_SIGNATURE_DESC root_sig_desc = {};
			root_sig_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
			ID3D12RootSignature* root_sig = nullptr;
			ID3DBlob* sig_blob = nullptr;
			ID3DBlob* error_blob = nullptr;
			m_d3d_assert(D3D12SerializeRootSignature(&root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, &error_blob));
			m_d3d_assert(device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&root_sig)));
			sig_blob->Release();
			state_subobjects[4].Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
			state_subobjects[4].pDesc = &root_sig;

			const wchar_t* export_names[2] = { dxr_miss_str, dxr_any_hit_str };
			D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
			association.NumExports = m_countof(export_names);
			association.pExports = export_names;
			association.pSubobjectToAssociate = &state_subobjects[4];
			state_subobjects[5].Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			state_subobjects[5].pDesc = &association;
		}
		{
			D3D12_RAYTRACING_SHADER_CONFIG shader_config = {};
			shader_config.MaxPayloadSizeInBytes = sizeof(float);
			shader_config.MaxAttributeSizeInBytes = 2 * sizeof(float);
			state_subobjects[6].Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
			state_subobjects[6].pDesc = &shader_config;

			const wchar_t* export_names[3] = { dxr_miss_str, dxr_any_hit_str, dxr_ray_gen_str };
			D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
			association.NumExports = m_countof(export_names);
			association.pExports = export_names;
			association.pSubobjectToAssociate = &state_subobjects[6];
			state_subobjects[7].Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			state_subobjects[7].pDesc = &association;
		}
		{
			D3D12_RAYTRACING_PIPELINE_CONFIG pipeline_config = {};
			pipeline_config.MaxTraceRecursionDepth = 1;
			state_subobjects[8].Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
			state_subobjects[8].pDesc = &pipeline_config;
		}
		{
			D3D12_ROOT_SIGNATURE_DESC global_root_sig_desc = {};
			ID3DBlob* global_root_sig_blob = nullptr;
			ID3DBlob* global_root_sig_error = nullptr;
			m_d3d_assert(D3D12SerializeRootSignature(&global_root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &global_root_sig_blob, &global_root_sig_error));
			m_d3d_assert(device->CreateRootSignature(0, global_root_sig_blob->GetBufferPointer(), global_root_sig_blob->GetBufferSize(), IID_PPV_ARGS(&dxr_ao_global_root_sig)));
			global_root_sig_blob->Release();
			state_subobjects[9].Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
			state_subobjects[9].pDesc = &dxr_ao_global_root_sig;
		}
		D3D12_STATE_OBJECT_DESC state_object_desc = {};
		state_object_desc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
		state_object_desc.NumSubobjects = m_countof(state_subobjects);
		state_object_desc.pSubobjects = state_subobjects;
		m_d3d_assert(device->CreateStateObject(&state_object_desc, IID_PPV_ARGS(&dxr_ao_pipeline_state)));
	}
	if (dxr_enabled) {
		dxr_shadow_shader_table_entry_size = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + 8;
		dxr_shadow_shader_table_entry_size = round_up(dxr_shadow_shader_table_entry_size, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);

		dxr_shadow_shader_table = create_buffer(dxr_shadow_shader_table_entry_size * 3, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);

		uint8* shader_table_ptr = nullptr;
		m_d3d_assert(dxr_shadow_shader_table->Map(0, nullptr, (void**)&shader_table_ptr));
		ID3D12StateObjectProperties* state_object_properties = nullptr;
		dxr_shadow_pipeline_state->QueryInterface(IID_PPV_ARGS(&state_object_properties));
		memcpy(shader_table_ptr, state_object_properties->GetShaderIdentifier(dxr_ray_gen_str), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		uint64 gpu_descriptor_handle = dxr_descriptor_heap->GetGPUDescriptorHandleForHeapStart().ptr;
		*(uint64*)(shader_table_ptr + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) = gpu_descriptor_handle;
		memcpy(shader_table_ptr + dxr_shadow_shader_table_entry_size, state_object_properties->GetShaderIdentifier(dxr_miss_str), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		memcpy(shader_table_ptr + dxr_shadow_shader_table_entry_size * 2, state_object_properties->GetShaderIdentifier(dxr_hit_group_str), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		dxr_shadow_shader_table->Unmap(0, nullptr);
	}
	if (dxr_enabled) {
		dxr_ao_shader_table_entry_size = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + 8;
		dxr_ao_shader_table_entry_size = round_up(dxr_ao_shader_table_entry_size, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);

		dxr_ao_shader_table = create_buffer(dxr_ao_shader_table_entry_size * 3, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);

		uint8* shader_table_ptr = nullptr;
		m_d3d_assert(dxr_ao_shader_table->Map(0, nullptr, (void**)&shader_table_ptr));
		ID3D12StateObjectProperties* state_object_properties = nullptr;
		dxr_ao_pipeline_state->QueryInterface(IID_PPV_ARGS(&state_object_properties));
		memcpy(shader_table_ptr, state_object_properties->GetShaderIdentifier(dxr_ray_gen_str), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		uint64 gpu_descriptor_handle = dxr_descriptor_heap->GetGPUDescriptorHandleForHeapStart().ptr;
		*(uint64*)(shader_table_ptr + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) = gpu_descriptor_handle;
		memcpy(shader_table_ptr + dxr_ao_shader_table_entry_size, state_object_properties->GetShaderIdentifier(dxr_miss_str), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		memcpy(shader_table_ptr + dxr_ao_shader_table_entry_size * 2, state_object_properties->GetShaderIdentifier(dxr_hit_group_str), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		dxr_ao_shader_table->Unmap(0, nullptr);
	}
}

void d3d12::init(window* window) {
	{ // device
		uint32 dxgi_factory_flags = 0;

		ID3D12Debug1* debug_controller = nullptr;
		m_d3d_assert(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)));
		debug_controller->EnableDebugLayer();
		debug_controller->SetEnableGPUBasedValidation(true);
		debug_controller->SetEnableSynchronizedCommandQueueValidation(true);
		dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		debug_controller->Release();

		m_d3d_assert(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgi_debug)));
		m_d3d_assert(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgi_info_queue)));
		dxgi_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		dxgi_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
		dxgi_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true);

		m_d3d_assert(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory)));

		for (uint32 i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i += 1) {
			DXGI_ADAPTER_DESC1 adapter_desc;
			adapter->GetDesc1(&adapter_desc);
			if (!(adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) {
				if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr))) {
					break;
				}
			}
		}
		m_d3d_assert(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));
	}
	{ // features
		D3D12_FEATURE_DATA_D3D12_OPTIONS5 features = {};
		m_d3d_assert(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &features, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)));
		dxr_enabled = features.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
	}
	{ // command queues, fences
		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		m_d3d_assert(device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue)));
		m_d3d_assert(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)));
		m_d3d_assert(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator, nullptr, IID_PPV_ARGS(&command_list)));
		command_list->Close();

		m_d3d_assert(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&command_list_fence)));
		command_list_fence_value = 0;
		command_list_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		m_assert(command_list_fence_event);
	}
	{ // descriptor heaps
		D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
		heap_desc.NumDescriptors = 16;
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_d3d_assert(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&rtv_descriptor_heap)));
		rtv_descriptor_count = 0;
		rtv_descriptor_capacity = heap_desc.NumDescriptors;
		rtv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		heap_desc.NumDescriptors = 4;
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_d3d_assert(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&dsv_descriptor_heap)));
		dsv_descriptor_count = 0;
		dsv_descriptor_capacity = heap_desc.NumDescriptors;
		dsv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		heap_desc.NumDescriptors = 1000000;
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		m_d3d_assert(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&cbv_srv_uav_descriptor_heap)));
		cbv_srv_uav_descriptor_count = 0;
		cbv_srv_uav_descriptor_capacity = heap_desc.NumDescriptors;
		cbv_srv_uav_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	{ // swap chain
		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
		swap_chain_desc.Width = window->width;
		swap_chain_desc.Height = window->height;
		swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = d3d12_swap_chain_buffer_count;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		m_d3d_assert(factory->CreateSwapChainForHwnd(command_queue, window->handle, &swap_chain_desc, nullptr, nullptr, (IDXGISwapChain1**)&swap_chain));
		for (uint32 i = 0; i < d3d12_swap_chain_buffer_count; i += 1) {
			swap_chain->GetBuffer(i, IID_PPV_ARGS(&swap_chain_textures[i]));
			append_rtv_descriptor(swap_chain_textures[i], &swap_chain_texture_descriptors[i], nullptr);
		}
	}
	{ // buffers
		frame_constants_buffer_size = 0;
		frame_constants_buffer_capacity = m_megabytes(32);
		frame_constants_buffer = create_buffer(frame_constants_buffer_capacity, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);

		uint32 copy_buffer_size = m_megabytes(32);
		copy_temp_buffer = create_buffer(copy_buffer_size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);
	}
	{ // textures
		render_texture = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		render_texture->SetName(L"render_target");
		append_rtv_descriptor(render_texture, &render_texture_rtv_descriptor, nullptr);
		append_srv_descriptor(render_texture, nullptr, &render_texture_srv_descriptor);

		depth_texture = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_D32_FLOAT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		depth_texture->SetName(L"depth_target");
		append_dsv_descriptor(depth_texture, &depth_texture_dsv_descriptor, nullptr);

		gbuffer_textures[0] = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		gbuffer_textures[1] = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		gbuffer_textures[2] = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		gbuffer_textures[3] = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_R8G8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		gbuffer_textures[4] = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		gbuffer_textures[0]->SetName(L"gbuffer_diffuse_render_target");
		gbuffer_textures[1]->SetName(L"gbuffer_position_render_target");
		gbuffer_textures[2]->SetName(L"gbuffer_normal_render_target");
		gbuffer_textures[3]->SetName(L"gbuffer_roughness_metallic_render_target");
		gbuffer_textures[4]->SetName(L"gbuffer_emissive_render_target");
		for (uint32 i = 0; i < m_countof(gbuffer_textures); i += 1) {
			append_rtv_descriptor(gbuffer_textures[i], &gbuffer_texture_rtv_descriptors[i], nullptr);
			append_srv_descriptor(gbuffer_textures[i], nullptr, &gbuffer_texture_srv_descriptors[i]);
		}

		uint8 dither_texture_data[] = {
			0, 32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
			48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
			12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
			60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
			3, 35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
			51, 19, 59, 27, 49, 17, 57, 25,
			15, 47,  7, 39, 13, 45,  5, 37,
			63, 31, 55, 23, 61, 29, 53, 21
		};
		dither_texture = create_texture_2d(8, 8, 1, 1, DXGI_FORMAT_R8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		copy_texture_2d(dither_texture, dither_texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		append_srv_descriptor(dither_texture, nullptr, &dither_texture_srv_descriptor);
	}
	if (dxr_enabled) {
		dxr_shadow_output_texture_array = create_texture_2d(window->width, window->height, 4, 1, DXGI_FORMAT_R8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		dxr_shadow_output_texture_array->SetName(L"dxr_shadow_output_texture_array");

		dxr_ao_output_texture = create_texture_2d(window->width, window->height, 1, 1, DXGI_FORMAT_R8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		dxr_ao_output_texture->SetName(L"dxr_ao_output_texture");

		D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc = {};
		descriptor_heap_desc.NumDescriptors = 16;
		descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		m_d3d_assert(device->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&dxr_descriptor_heap)));

		D3D12_CPU_DESCRIPTOR_HANDLE dxr_cpu_descriptor_handle = dxr_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
		uint32 dxr_cpu_descriptor_handle_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		device->CreateShaderResourceView(gbuffer_textures[1], nullptr, { dxr_cpu_descriptor_handle.ptr + 1 * dxr_cpu_descriptor_handle_size });
		device->CreateShaderResourceView(gbuffer_textures[2], nullptr, { dxr_cpu_descriptor_handle.ptr + 2 * dxr_cpu_descriptor_handle_size });

		D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
		uav_desc.Format = DXGI_FORMAT_R8_UNORM;
		uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
		uav_desc.Texture2DArray.ArraySize = 4;
		device->CreateUnorderedAccessView(dxr_shadow_output_texture_array, nullptr, &uav_desc, { dxr_cpu_descriptor_handle.ptr + 3 * dxr_cpu_descriptor_handle_size });
		uav_desc.Format = DXGI_FORMAT_R8_UNORM;
		uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		uav_desc.Texture2DArray.ArraySize = 1;
		device->CreateUnorderedAccessView(dxr_ao_output_texture, nullptr, &uav_desc, { dxr_cpu_descriptor_handle.ptr + 4 * dxr_cpu_descriptor_handle_size });
	}

	init_pipelines();
}

#endif // __D3D_CPP__
