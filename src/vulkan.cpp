/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#define m_vulkan_procs                                                                    \
  m_x(PFN_vkEnumerateInstanceLayerProperties, vkEnumerateInstanceLayerProperties)         \
  m_x(PFN_vkEnumerateInstanceExtensionProperties, vkEnumerateInstanceExtensionProperties) \
  m_x(PFN_vkCreateInstance, vkCreateInstance)                                             \
  m_x(PFN_vkGetInstanceProcAddr, vkGetInstanceProcAddr)

#define m_vulkan_instance_procs                                                                 \
  m_x(PFN_vkEnumeratePhysicalDevices, vkEnumeratePhysicalDevices)                               \
  m_x(PFN_vkGetPhysicalDeviceProperties, vkGetPhysicalDeviceProperties)                         \
  m_x(PFN_vkGetPhysicalDeviceQueueFamilyProperties, vkGetPhysicalDeviceQueueFamilyProperties)   \
  m_x(PFN_vkGetPhysicalDeviceMemoryProperties, vkGetPhysicalDeviceMemoryProperties)             \
  m_x(PFN_vkGetPhysicalDeviceFormatProperties, vkGetPhysicalDeviceFormatProperties)             \
  m_x(PFN_vkGetPhysicalDeviceFeatures, vkGetPhysicalDeviceFeatures)                             \
  m_x(PFN_vkCreateDevice, vkCreateDevice)                                                       \
  m_x(PFN_vkGetDeviceProcAddr, vkGetDeviceProcAddr)                                             \
  m_x(PFN_vkEnumerateDeviceExtensionProperties, vkEnumerateDeviceExtensionProperties)           \
  m_x(PFN_vkCreateWin32SurfaceKHR, vkCreateWin32SurfaceKHR)                                     \
  m_x(PFN_vkDestroySurfaceKHR, vkDestroySurfaceKHR)                                             \
  m_x(PFN_vkGetPhysicalDeviceSurfaceSupportKHR, vkGetPhysicalDeviceSurfaceSupportKHR)           \
  m_x(PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR, vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
  m_x(PFN_vkGetPhysicalDeviceSurfaceFormatsKHR, vkGetPhysicalDeviceSurfaceFormatsKHR)           \
  m_x(PFN_vkGetPhysicalDeviceSurfacePresentModesKHR, vkGetPhysicalDeviceSurfacePresentModesKHR) \
  m_x(PFN_vkBeginCommandBuffer, vkBeginCommandBuffer)                                           \
  m_x(PFN_vkEndCommandBuffer, vkEndCommandBuffer)                                               \
  m_x(PFN_vkResetCommandBuffer, vkResetCommandBuffer)                                           \
  m_x(PFN_vkCmdPipelineBarrier, vkCmdPipelineBarrier)                                           \
  m_x(PFN_vkCmdClearColorImage, vkCmdClearColorImage)                                           \
  m_x(PFN_vkCmdClearAttachments, vkCmdClearAttachments)                                         \
  m_x(PFN_vkCmdBlitImage, vkCmdBlitImage)                                                       \
  m_x(PFN_vkCmdExecuteCommands, vkCmdExecuteCommands)                                           \
  m_x(PFN_vkCmdBeginRenderPass, vkCmdBeginRenderPass)                                           \
  m_x(PFN_vkCmdNextSubpass, vkCmdNextSubpass)																										\
  m_x(PFN_vkCmdEndRenderPass, vkCmdEndRenderPass)                                               \
  m_x(PFN_vkCmdBindDescriptorSets, vkCmdBindDescriptorSets)                                     \
  m_x(PFN_vkCmdBindPipeline, vkCmdBindPipeline)                                                 \
  m_x(PFN_vkCmdSetViewport, vkCmdSetViewport)                                                   \
  m_x(PFN_vkCmdSetScissor, vkCmdSetScissor)                                                     \
  m_x(PFN_vkCmdPushConstants, vkCmdPushConstants)                                               \
  m_x(PFN_vkCmdBindVertexBuffers, vkCmdBindVertexBuffers)                                       \
  m_x(PFN_vkCmdBindIndexBuffer, vkCmdBindIndexBuffer)                                           \
  m_x(PFN_vkCmdDraw, vkCmdDraw)                                                                 \
  m_x(PFN_vkCmdDrawIndexed, vkCmdDrawIndexed)                                                   \
  m_x(PFN_vkCmdCopyBuffer, vkCmdCopyBuffer)                                                     \
  m_x(PFN_vkCmdCopyBufferToImage, vkCmdCopyBufferToImage)                                       \
  m_x(PFN_vkCmdCopyImageToBuffer, vkCmdCopyImageToBuffer)                                       \
  m_x(PFN_vkCmdDispatch, vkCmdDispatch)

#define m_vulkan_device_procs                                           \
  m_x(PFN_vkCreateSwapchainKHR, vkCreateSwapchainKHR)                   \
  m_x(PFN_vkDestroySwapchainKHR, vkDestroySwapchainKHR )                \
  m_x(PFN_vkGetSwapchainImagesKHR, vkGetSwapchainImagesKHR)             \
  m_x(PFN_vkAcquireNextImageKHR, vkAcquireNextImageKHR)                 \
  m_x(PFN_vkGetDeviceQueue, vkGetDeviceQueue)                           \
  m_x(PFN_vkQueueWaitIdle, vkQueueWaitIdle)                             \
  m_x(PFN_vkQueueSubmit, vkQueueSubmit)                                 \
  m_x(PFN_vkQueuePresentKHR, vkQueuePresentKHR)                         \
  m_x(PFN_vkCreateFence, vkCreateFence)                                 \
  m_x(PFN_vkDestroyFence, vkDestroyFence)                               \
  m_x(PFN_vkWaitForFences, vkWaitForFences)                             \
  m_x(PFN_vkResetFences, vkResetFences)                                 \
  m_x(PFN_vkCreateSemaphore, vkCreateSemaphore)                         \
  m_x(PFN_vkDestroySemaphore, vkDestroySemaphore)                       \
  m_x(PFN_vkCreateCommandPool, vkCreateCommandPool)                     \
  m_x(PFN_vkResetCommandPool, vkResetCommandPool)                       \
  m_x(PFN_vkAllocateCommandBuffers, vkAllocateCommandBuffers)           \
  m_x(PFN_vkFreeCommandBuffers, vkFreeCommandBuffers)                   \
  m_x(PFN_vkCreateRenderPass, vkCreateRenderPass)                       \
  m_x(PFN_vkAllocateMemory, vkAllocateMemory)                           \
  m_x(PFN_vkFreeMemory, vkFreeMemory)                                   \
  m_x(PFN_vkCreateImage, vkCreateImage)                                 \
  m_x(PFN_vkDestroyImage, vkDestroyImage)                               \
  m_x(PFN_vkGetImageMemoryRequirements, vkGetImageMemoryRequirements)   \
  m_x(PFN_vkBindImageMemory, vkBindImageMemory)                         \
  m_x(PFN_vkCreateImageView, vkCreateImageView)                         \
  m_x(PFN_vkDestroyImageView, vkDestroyImageView)                       \
  m_x(PFN_vkGetImageSubresourceLayout, vkGetImageSubresourceLayout)     \
  m_x(PFN_vkCreateSampler, vkCreateSampler)                             \
  m_x(PFN_vkDestroySampler, vkDestroySampler)                           \
  m_x(PFN_vkCreateFramebuffer, vkCreateFramebuffer)                     \
  m_x(PFN_vkDestroyFramebuffer, vkDestroyFramebuffer)                   \
  m_x(PFN_vkCreateGraphicsPipelines, vkCreateGraphicsPipelines)         \
  m_x(PFN_vkCreateComputePipelines, vkCreateComputePipelines)           \
  m_x(PFN_vkCreateDescriptorSetLayout, vkCreateDescriptorSetLayout)     \
  m_x(PFN_vkCreateDescriptorPool, vkCreateDescriptorPool)               \
  m_x(PFN_vkAllocateDescriptorSets, vkAllocateDescriptorSets)           \
  m_x(PFN_vkFreeDescriptorSets, vkFreeDescriptorSets)                   \
  m_x(PFN_vkUpdateDescriptorSets, vkUpdateDescriptorSets)               \
  m_x(PFN_vkCreatePipelineLayout, vkCreatePipelineLayout)               \
  m_x(PFN_vkCreateShaderModule, vkCreateShaderModule)                   \
  m_x(PFN_vkCreateBuffer, vkCreateBuffer)                               \
  m_x(PFN_vkDestroyBuffer, vkDestroyBuffer)                             \
  m_x(PFN_vkGetBufferMemoryRequirements, vkGetBufferMemoryRequirements) \
  m_x(PFN_vkBindBufferMemory, vkBindBufferMemory)                       \
  m_x(PFN_vkMapMemory, vkMapMemory)                                     \
  m_x(PFN_vkUnmapMemory, vkUnmapMemory)                                 \
  m_x(PFN_vkFlushMappedMemoryRanges, vkFlushMappedMemoryRanges)

#define m_x(shape, name) shape name = nullptr;
m_vulkan_procs
m_vulkan_instance_procs
m_vulkan_device_procs
#undef m_x

const uint32 vulkan_swap_chain_image_count = 3;
const uint32 vulkan_buffering_count = 1;

struct vulkan_device {
	VkDevice device;
	VkInstance instance;
	VkQueue queue;
	uint32 queue_family_index;
	VkPhysicalDevice physical_device;
	VkPhysicalDeviceProperties physical_device_properties;
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
};

struct vulkan_swap_chain {
	VkImage images[vulkan_swap_chain_image_count];
	VkImageView image_views[vulkan_swap_chain_image_count];
	uint32 image_width;
	uint32 image_height;
	VkPresentModeKHR present_mode;
	VkSwapchainKHR swap_chain;
	VkSurfaceKHR surface;
	VkSurfaceCapabilitiesKHR surface_capabilities;
};

struct vulkan_render_passes {
	VkRenderPass shadow_map_render_passes[3];
	VkRenderPass main_render_pass;
	VkRenderPass swap_chain_render_pass;
};

struct vulkan_framebuffers {
	struct {
		VkFramebuffer framebuffers[3];
		uint32 width;
		uint32 height;
		struct {
			VkImage image;
			VkImageView image_view;
			VkDeviceMemory image_device_memory;
		} color_attachments[3], depth_stencil_attachment;
	} shadow_map_framebuffers[vulkan_buffering_count];
	struct {
		VkFramebuffer framebuffer;
		uint32 width;
		uint32 height;
		VkSampleCountFlagBits sample_count;
		struct {
			VkImage image;
			VkImageView image_view;
			VkDeviceMemory image_device_memory;
		} color_attachment, depth_stencil_attachment, color_resolve_attachment;
	} main_framebuffers[vulkan_buffering_count];
	VkFramebuffer swap_chain_framebuffers[vulkan_swap_chain_image_count];
};

struct vulkan_cmd_buffers {
	VkCommandBuffer graphic_cmd_buffers[vulkan_buffering_count];
	VkCommandBuffer transfer_cmd_buffer;
	VkCommandPool cmd_pool;
};

struct vulkan_syncs {
	VkSemaphore swap_chain_image_acquire_semaphores[vulkan_buffering_count];
	VkSemaphore queue_submit_semaphores[vulkan_buffering_count];
	VkFence queue_submit_fences[vulkan_buffering_count];
};

struct vulkan_memory {
	VkDeviceMemory memory;
	uint64 size;
	uint64 capacity;
	uint32 memory_type_index;
};

struct vulkan_memories {
	vulkan_memory common_images_memory;
	vulkan_memory level_images_memory;
	vulkan_memory level_vertex_buffer_memory;
	vulkan_memory frame_vertex_buffer_memories[vulkan_buffering_count];
	vulkan_memory frame_uniform_buffer_memories[vulkan_buffering_count];
};

struct vulkan_buffer {
	VkBuffer buffer;
	uint64 capacity;
};

struct vulkan_buffers {
	vulkan_buffer level_vertex_buffer;
	uint32 level_vertex_buffer_offset;

	vulkan_buffer frame_vertex_buffers[vulkan_buffering_count];
	uint8 *frame_vertex_buffer_ptrs[vulkan_buffering_count];
	uint32 frame_vertex_buffer_offsets[vulkan_buffering_count];

	vulkan_buffer frame_uniform_buffers[vulkan_buffering_count];
	uint8 *frame_uniform_buffer_ptrs[vulkan_buffering_count];
	uint32 frame_uniform_buffer_offsets[vulkan_buffering_count];
};

struct vulkan_image {
	VkImage image;
	VkImageView view;
	uint32 width;
	uint32 height;
	VkFormat pixel_format;
	uint32 mipmap_count;
	uint32 layer_count;
};

struct vulkan_images {
	vulkan_image default_albedo_map_image;
	vulkan_image default_normal_map_image;
	vulkan_image default_metallic_map_image;
	vulkan_image default_roughness_map_image;
	vulkan_image default_ao_map_image;
	vulkan_image default_height_map_image;
	vulkan_image imgui_font_atlas_image;
	vulkan_image font_atlas_image;
};

struct vulkan_samplers {
	VkSampler shadow_map_sampler;
	VkSampler gaussian_blur_sampler;
	VkSampler skybox_cubemap_sampler;
	VkSampler mipmap_image_samplers[16];
};

struct vulkan_descriptors {
	VkDescriptorPool pool;
	VkDescriptorSetLayout uniform_buffers_set_layout;
	VkDescriptorSetLayout sampled_images_set_layouts[8];

	VkDescriptorSet frame_uniform_buffer_offsets[vulkan_buffering_count];
	VkDescriptorSet main_framebuffer_images[vulkan_swap_chain_image_count];
	VkDescriptorSet shadow_map_framebuffer_images[vulkan_buffering_count][3];
	VkDescriptorSet model_default_material_images;
	VkDescriptorSet imgui_font_atlas_image;
	VkDescriptorSet font_atlas_image;
};

struct vulkan_pipeline {
	VkPipeline pipeline;
	VkPipelineLayout layout;
};

struct vulkan_pipelines {
	vulkan_pipeline static_model_pipeline;
	vulkan_pipeline static_model_wireframe_pipeline;
	vulkan_pipeline static_model_shadow_map_pipeline;
	vulkan_pipeline animated_model_pipeline;
	vulkan_pipeline animated_model_shadow_map_pipeline;
	vulkan_pipeline shadow_map_gaussian_blur_pipelines[2];
	vulkan_pipeline skybox_pipeline;
	vulkan_pipeline imgui_pipeline;
	vulkan_pipeline text_pipeline;
	vulkan_pipeline collision_shape_pipeline;
	vulkan_pipeline lines_pipeline;
	vulkan_pipeline swap_chain_pipeline;
};

struct vulkan {
	vulkan_device device;
	vulkan_swap_chain swap_chain;
	vulkan_render_passes render_passes;
	vulkan_framebuffers framebuffers;
	vulkan_cmd_buffers cmd_buffers;
	vulkan_syncs syncs;
	vulkan_memories memories;
	vulkan_buffers buffers;
	vulkan_images images;
	vulkan_samplers samplers;
	vulkan_descriptors descriptors;
	vulkan_pipelines pipelines;

	uint32 frame_index;
	uint32 swap_chain_image_index;
};

#define m_vk_assert(vk_call) { VkResult result = vk_call; if (result != VK_SUCCESS) { fatal("Vulkan API Error:\n\nCode: %s\nError: %s\nFile: %s\nLine: %d", #vk_call, vk_result_to_str(result), __FILE__, __LINE__); } }

const char *vk_result_to_str(VkResult result) {
	switch (result) {
		case VK_SUCCESS: return "VK_SUCCESS";
		case VK_NOT_READY: return "VK_NOT_READY";
		case VK_TIMEOUT: return "VK_TIMEOUT";
		case VK_EVENT_SET: return "VK_EVENT_SET";
		case VK_EVENT_RESET: return "VK_EVENT_RESET";
		case VK_INCOMPLETE: return "VK_INCOMPLETE";
		case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
		case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
		case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
		case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
		case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
		case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
		case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
		case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
		case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
		case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
		case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
		case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
		case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
		case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
		case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
		case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
		case VK_ERROR_OUT_OF_POOL_MEMORY_KHR: return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
		case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: return "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR";
		default: return "INVALID VKRESULT";
	}
}

VkBool32 vulkan_debug_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, uint64_t object, size_t location, int32 messageCode, const char *layer_prefix, const char *message, void *user_data) {
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		console("vulkan error: %s\n", message);
	}
	else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		console("vulkan warning: %s\n", message);
	}
	else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		console("vulkan performance warning: %s\n", message);
	}
	return VK_FALSE;
};

bool vulkan_allocate_buffer(vulkan_device *vulkan_device, VkBufferCreateInfo buffer_create_info, VkDeviceMemory *vulkan_device_memory, VkBuffer *vulkan_buffer) {
	if (vkCreateBuffer(vulkan_device->device, &buffer_create_info, nullptr, vulkan_buffer) != VK_SUCCESS) {
		return false;
	}
	VkMemoryRequirements memory_requirements = {};
	vkGetBufferMemoryRequirements(vulkan_device->device, *vulkan_buffer, &memory_requirements);
	VkPhysicalDeviceMemoryProperties *memory_properties = &vulkan_device->physical_device_memory_properties;
	uint32 memory_type_index = UINT_MAX;
	for (uint32 i = 0; i < memory_properties->memoryTypeCount; i += 1) {
		if ((memory_requirements.memoryTypeBits & (1 << i)) && 
			  (memory_properties->memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) && (memory_properties->memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
			memory_type_index = i;
		  break;
		}
	}
	if (memory_type_index == UINT_MAX) {
		vkDestroyBuffer(vulkan_device->device, *vulkan_buffer, nullptr);
		return false;
	}
	VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
	memory_allocate_info.allocationSize = memory_requirements.size;
	memory_allocate_info.memoryTypeIndex = memory_type_index;
	if (vkAllocateMemory(vulkan_device->device, &memory_allocate_info, nullptr, vulkan_device_memory) != VK_SUCCESS) {
		vkDestroyBuffer(vulkan_device->device, *vulkan_buffer, nullptr);
		return false;
	}
	if (vkBindBufferMemory(vulkan_device->device, *vulkan_buffer, *vulkan_device_memory, 0) != VK_SUCCESS) {
		vkDestroyBuffer(vulkan_device->device, *vulkan_buffer, nullptr);
		vkFreeMemory(vulkan_device->device, *vulkan_device_memory, nullptr);
		return false;
	}
	return true;
}

bool vulkan_allocate_image(vulkan_device *vulkan_device, VkImageCreateInfo image_create_info, VkImageViewCreateInfo image_view_create_info, VkDeviceMemory *vulkan_device_memory, VkImage *image, VkImageView *image_view) {
	if (vkCreateImage(vulkan_device->device, &image_create_info, nullptr, image) != VK_SUCCESS) {
		return false;
	}
	VkMemoryRequirements requirements = {};
	vkGetImageMemoryRequirements(vulkan_device->device, *image, &requirements);
	VkPhysicalDeviceMemoryProperties *properties = &vulkan_device->physical_device_memory_properties;
	uint32 memory_type_index = UINT_MAX;
	for (uint32 i = 0; i < properties->memoryTypeCount; i += 1) {
		if ((requirements.memoryTypeBits & (1 << i)) && properties->memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			memory_type_index = i;
			break;
		}
	}
	if (memory_type_index == UINT_MAX) {
		vkDestroyImage(vulkan_device->device, *image, nullptr);
		return false;
	}
	VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
	memory_allocate_info.allocationSize = requirements.size;
	memory_allocate_info.memoryTypeIndex = memory_type_index;
	if (vkAllocateMemory(vulkan_device->device, &memory_allocate_info, nullptr, vulkan_device_memory) != VK_SUCCESS) {
		vkDestroyImage(vulkan_device->device, *image, nullptr);
		return false;		
	}
	if (vkBindImageMemory(vulkan_device->device, *image, *vulkan_device_memory, 0) != VK_SUCCESS) {
		vkDestroyImage(vulkan_device->device, *image, nullptr);
		vkFreeMemory(vulkan_device->device, *vulkan_device_memory, nullptr);
		return false;		
	}
	image_view_create_info.image = *image;
	if (vkCreateImageView(vulkan_device->device, &image_view_create_info, nullptr, image_view) != VK_SUCCESS) {
		vkDestroyImage(vulkan_device->device, *image, nullptr);
		vkFreeMemory(vulkan_device->device, *vulkan_device_memory, nullptr);
		return false;		
	}
	return true;
}

void vulkan_device_initialize(vulkan_device *vulkan_device) {
	bool enable_validation_layer = true;
	bool enable_renderdoc_layer = true;
	bool enable_nsight_layer = false;

	struct memory_arena memory_arena = {};
	string info_str = {};
	memory_arena.capacity = m_megabytes(1);
	memory_arena.memory = allocate_virtual_memory(memory_arena.capacity);
	m_assert(memory_arena.memory);
	info_str.capacity = memory_arena.capacity / 2;
	info_str.buf = allocate_memory<char>(&memory_arena, info_str.capacity);
	m_scope_exit(
		console("%s\n", info_str.buf);
		free_virtual_memory(memory_arena.memory);
	);
	{ // dynamic load vulkan DLL
		HMODULE vulkan_1_dll = LoadLibraryA("vulkan-1.dll");
		if (!vulkan_1_dll) {
			fatal("call to LoadLibrary(\"vulkan-1.dll\") failed");
		}
		#define m_x(shape, name) name = (shape)GetProcAddress(vulkan_1_dll, #name); if (!name) { fatal("GetProcAddress(vulkan-1.dll, \"name\") failed"); }
		m_vulkan_procs
		#undef m_x      
	}
	{ // query/enable layers/extensions, create instance
		uint32 layer_count = 0;
		m_vk_assert(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
		VkLayerProperties *layer_properties = allocate_memory<VkLayerProperties>(&memory_arena, layer_count);
		m_vk_assert(vkEnumerateInstanceLayerProperties(&layer_count, layer_properties));
		string_catf(&info_str, "Instance Layers:\n");
		for (uint32 i = 0; i < layer_count; i += 1) {
			string_catf(&info_str, "  %s\n", layer_properties[i].layerName);
		}
		string_catf(&info_str, "\n");
		uint32 extension_count = 0;
		m_vk_assert(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr));
		VkExtensionProperties *extension_properties = allocate_memory<VkExtensionProperties>(&memory_arena, extension_count);
		m_vk_assert(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties));
		string_catf(&info_str, "Instance Extensions:\n");
		for (uint32 i = 0; i < extension_count; i += 1) {
			string_catf(&info_str, "  %s\n", extension_properties[i].extensionName);
		}
		string_catf(&info_str, "\n");

		const char *enabled_layers[16] = {};
		uint32 enabled_layer_count = 0;
		const char *enabled_extensions[16] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
		uint32 enabled_extension_count = 2;
		for (uint32 i = 0; i < layer_count; i += 1) {
			if (enable_validation_layer && !strcmp(layer_properties[i].layerName, "VK_LAYER_LUNARG_standard_validation")) {
				enabled_layers[enabled_layer_count++] = "VK_LAYER_LUNARG_standard_validation";
			}
			else if (enable_nsight_layer && !strcmp(layer_properties[i].layerName, "VK_LAYER_NV_nsight")) {
				enabled_layers[enabled_layer_count++] = "VK_LAYER_NV_nsight";
			}
			else if (enable_renderdoc_layer && !strcmp(layer_properties[i].layerName, "VK_LAYER_RENDERDOC_Capture")) {
				enabled_layers[enabled_layer_count++] = "VK_LAYER_RENDERDOC_Capture";
			}
		}
		for (uint32 i = 0; i < extension_count; i += 1) {
			if (enable_validation_layer && !strcmp(extension_properties[i].extensionName, "VK_EXT_debug_report")) {
				enabled_extensions[enabled_extension_count++] = "VK_EXT_debug_report";
			}
		}
		VkInstanceCreateInfo instance_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
		VkApplicationInfo application_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
		application_info.pApplicationName = "agby";
		application_info.apiVersion = VK_API_VERSION_1_0;
		instance_info.pApplicationInfo = &application_info;
		instance_info.enabledLayerCount = enabled_layer_count;
		instance_info.ppEnabledLayerNames = enabled_layers;
		instance_info.enabledExtensionCount = enabled_extension_count;
		instance_info.ppEnabledExtensionNames = enabled_extensions;
		m_vk_assert(vkCreateInstance(&instance_info, nullptr, &vulkan_device->instance));

		#define m_x(shape, name) name = (shape)vkGetInstanceProcAddr(vulkan_device->instance, #name); if (!name) { fatal("cannot get \"" #name "\" from vulkan-1.dll"); }
		m_vulkan_instance_procs
		#undef m_x

		for (uint32 i = 0; i < enabled_extension_count; i += 1) {
			if (!strcmp(enabled_extensions[i], "VK_EXT_debug_report")) {
				PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vulkan_device->instance, "vkCreateDebugReportCallbackEXT");
				if (vkCreateDebugReportCallbackEXT) {
					VkDebugReportCallbackCreateInfoEXT debug_callback_info = {VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT};
					debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
					debug_callback_info.pfnCallback = vulkan_debug_callback;
					VkDebugReportCallbackEXT debug_callback = {};
					m_vk_assert(vkCreateDebugReportCallbackEXT(vulkan_device->instance, &debug_callback_info, nullptr, &debug_callback));
				}
				break;
			}
		}
		string_catf(&info_str, "Enabled Layers: ");
		for (uint32 i = 0; i < enabled_layer_count; i += 1) {
			string_catf(&info_str, "\"%s\" ", enabled_layers[i]);
		}
		string_catf(&info_str, "\nEnabled Extensions: ");
		for (uint32 i = 0; i < enabled_extension_count; i += 1) {
			string_catf(&info_str, "\"%s\" ", enabled_extensions[i]);
		}
		string_catf(&info_str, "\n\n");
	}
	{ // create physical device
		uint32 physical_device_count = 0;
		m_vk_assert(vkEnumeratePhysicalDevices(vulkan_device->instance, &physical_device_count, nullptr));
		VkPhysicalDevice *physical_devices = allocate_memory<VkPhysicalDevice>(&memory_arena, physical_device_count);
		m_vk_assert(vkEnumeratePhysicalDevices(vulkan_device->instance, &physical_device_count, physical_devices));
		VkPhysicalDeviceProperties physical_device_properties = {};
		VkPhysicalDeviceFeatures physical_device_features = {};
		string_catf(&info_str, "Physical Devices: ");
		for (uint32 i = 0; i < physical_device_count; i += 1) {
			vkGetPhysicalDeviceProperties(physical_devices[i], &physical_device_properties);
			string_catf(&info_str, "\"%s\" ", physical_device_properties.deviceName);
		}
		string_catf(&info_str, "\n");
		for (uint32 i = 0; i < physical_device_count; i += 1) {
			VkPhysicalDevice physical_device = physical_devices[i];
			vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);
			if (physical_device_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				continue;
			}
			uint32 physical_device_extensions_count = 0;
			m_vk_assert(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &physical_device_extensions_count, nullptr));
			VkExtensionProperties *physical_device_extensions = allocate_memory<VkExtensionProperties>(&memory_arena, physical_device_extensions_count);
			m_vk_assert(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &physical_device_extensions_count, physical_device_extensions));
			VkBool32 support_swap_chain_extension = VK_FALSE;
			for (uint32 i = 0; i < physical_device_extensions_count; i += 1) {
				if (!strcmp(physical_device_extensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
					support_swap_chain_extension = VK_TRUE;
					break;
				}
			}
			vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);
			if (support_swap_chain_extension && physical_device_features.samplerAnisotropy && physical_device_features.textureCompressionBC && physical_device_features.wideLines && physical_device_features.fillModeNonSolid) {
				string_catf(&info_str, "Using Physical Device: %s\n", physical_device_properties.deviceName);
				string_catf(&info_str, "\nPhysical Device Extensions:\n");
				for (uint32 i = 0; i < physical_device_extensions_count; i += 1) {
					string_catf(&info_str, "  %s\n", physical_device_extensions[i].extensionName);
				}
				string_catf(&info_str, "\nAPI Version: %d.%d.%d\n", VK_VERSION_MAJOR(physical_device_properties.apiVersion), VK_VERSION_MINOR(physical_device_properties.apiVersion), VK_VERSION_PATCH(physical_device_properties.apiVersion));
				string_catf(&info_str, "Driver Version: %d.%d.%d\n", VK_VERSION_MAJOR(physical_device_properties.driverVersion), VK_VERSION_MINOR(physical_device_properties.driverVersion), VK_VERSION_PATCH(physical_device_properties.driverVersion));
				vulkan_device->physical_device = physical_device;
				vulkan_device->physical_device_properties = physical_device_properties;
				vkGetPhysicalDeviceMemoryProperties(physical_device, &vulkan_device->physical_device_memory_properties);
				break;
			}
		}
		if (!vulkan_device->physical_device) {
			fatal("cannot find a capable physical GPU device");
		}
	}
	{ // create device, graphic/compute/transfer queues
		uint32 queue_families_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(vulkan_device->physical_device, &queue_families_count, nullptr);
		VkQueueFamilyProperties *queue_familiy_properties = allocate_memory<VkQueueFamilyProperties>(&memory_arena, queue_families_count);
		vkGetPhysicalDeviceQueueFamilyProperties(vulkan_device->physical_device, &queue_families_count, queue_familiy_properties);
		for (uint32 i = 0; i < queue_families_count; i += 1) {
			string_catf(&info_str, "Queue familiy %d properties: %d queues, ", i, queue_familiy_properties[i].queueCount);
			if (queue_familiy_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				string_catf(&info_str, "\"graphic\" ");
			}
			if (queue_familiy_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
				string_catf(&info_str, "\"transfer\" ");
			}
			if (queue_familiy_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
				string_catf(&info_str, "\"compute\" ");
			}
			if (queue_familiy_properties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
				string_catf(&info_str, "\"sparse binding\" ");
			}
			string_catf(&info_str, "\n");
		}
		vulkan_device->queue_family_index = -1;
		for (uint32 i = 0; i < queue_families_count; i += 1) {
			if ((queue_familiy_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queue_familiy_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
				vulkan_device->queue_family_index = i;
				break;
			}
		}
		if (vulkan_device->queue_family_index == -1) {
			fatal("vulkan error, cannot find a GPU queue");
		}

		float queue_priority = 1;
		VkDeviceQueueCreateInfo queue_create_info = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
		queue_create_info.queueFamilyIndex = vulkan_device->queue_family_index;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;

		const char *device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

		VkPhysicalDeviceFeatures device_features = {};
		device_features.samplerAnisotropy = VK_TRUE;
		device_features.textureCompressionBC = VK_TRUE;
		device_features.wideLines = VK_TRUE;
		device_features.fillModeNonSolid = VK_TRUE;

		VkDeviceCreateInfo device_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
		device_info.queueCreateInfoCount = 1;
		device_info.pQueueCreateInfos = &queue_create_info;
		device_info.enabledExtensionCount = m_countof(device_extensions);
		device_info.ppEnabledExtensionNames = device_extensions;
		device_info.pEnabledFeatures = &device_features;
		m_vk_assert(vkCreateDevice(vulkan_device->physical_device, &device_info, nullptr, &vulkan_device->device));
		#define m_x(shape, name) name = (shape)vkGetDeviceProcAddr(vulkan_device->device, #name); if (!name) { fatal("cannot get \"" #name "\" from vulkan-1.dll"); }
		m_vulkan_device_procs
		#undef m_x
		vkGetDeviceQueue(vulkan_device->device, vulkan_device->queue_family_index, 0, &vulkan_device->queue);
	}
}

void vulkan_swap_chain_initialize(window window, bool vsync_on, vulkan *vulkan) {
	struct memory_arena memory_arena = {};
	string info_str = {};
	memory_arena.capacity = m_megabytes(1);
	memory_arena.memory = allocate_virtual_memory(memory_arena.capacity);
	m_assert(memory_arena.memory);
	info_str.capacity = memory_arena.capacity / 2;
	info_str.buf = allocate_memory<char>(&memory_arena, info_str.capacity);
	m_scope_exit(
		console("%s\n", info_str.buf);
	  free_virtual_memory(memory_arena.memory);
	);
	{ // create surface
		VkWin32SurfaceCreateInfoKHR surface_create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
		surface_create_info.hinstance = GetModuleHandle(nullptr);
		surface_create_info.hwnd = window.handle;
		m_vk_assert(vkCreateWin32SurfaceKHR(vulkan->device.instance, &surface_create_info, nullptr, &vulkan->swap_chain.surface));
		VkBool32 physical_device_surface_support = VK_FALSE;
		m_vk_assert(vkGetPhysicalDeviceSurfaceSupportKHR(vulkan->device.physical_device, vulkan->device.queue_family_index, vulkan->swap_chain.surface, &physical_device_surface_support));
		if (physical_device_surface_support == VK_FALSE) {
			fatal("GPU physical device does not support vulkan surface");
		}
	}
	{ // check surface capabilities
		m_vk_assert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &vulkan->swap_chain.surface_capabilities));
		if (vulkan->swap_chain.surface_capabilities.maxImageCount < vulkan_swap_chain_image_count) {
			fatal("vulkan surface does not support at least %d images", vulkan_swap_chain_image_count);
		}
		if (!(vulkan->swap_chain.surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) || !(vulkan->swap_chain.surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) {
			fatal("vulkan surface images lack required capabilities");
		}
		uint32 surface_image_formats_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_image_formats_count, nullptr);
		if (surface_image_formats_count == 0) {
			fatal("call to \"vkGetPhysicalDeviceSurfaceFormatsKHR\" return zero format");
		}
		VkSurfaceFormatKHR *surface_image_formats = allocate_memory<VkSurfaceFormatKHR>(&memory_arena, surface_image_formats_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_image_formats_count, surface_image_formats);
		if (!(surface_image_formats_count == 1 && surface_image_formats[0].format == VK_FORMAT_UNDEFINED)) {
			bool support_bgra_unorm = false;
			for (uint32 i = 0; i < surface_image_formats_count; i += 1) {
				if (surface_image_formats[i].format == VK_FORMAT_B8G8R8A8_UNORM) {
					support_bgra_unorm = true;
					break;
				}
			}
			if (!support_bgra_unorm) {
				fatal("vulkan surface does not support B8G8R8A8_UNORM format");
			}
		}
	}
	{ // set present mode
		uint32 surface_present_modes_count = 0;
		m_vk_assert(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_present_modes_count, nullptr));
		VkPresentModeKHR *surface_present_modes = allocate_memory<VkPresentModeKHR>(&memory_arena, surface_present_modes_count);
		m_vk_assert(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_present_modes_count, surface_present_modes));
		string_catf(&info_str, "Swapchain Present Modes: ");
		const char *present_mode_strs[4] = {"Immediate", "Mailbox", "FIFO", "FIFO_Relaxed"};
		for (uint32 i = 0; i < surface_present_modes_count; i += 1) {
			string_catf(&info_str, "\"%s\" ", present_mode_strs[surface_present_modes[i]]);
		}
		if (vsync_on) {
			vulkan->swap_chain.present_mode = VK_PRESENT_MODE_FIFO_KHR;
		}
		else {
			bool mailbox_mode_exist = false;
			bool immediate_mode_exist = false;
			for (uint32 i = 0; i < surface_present_modes_count; i += 1) {
				if (surface_present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
					mailbox_mode_exist = true;
				}
				if (surface_present_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
					immediate_mode_exist = true;
				}
			}
			if (mailbox_mode_exist) {
				vulkan->swap_chain.present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			}
			else if (immediate_mode_exist) {
				vulkan->swap_chain.present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
			else {
				vulkan->swap_chain.present_mode = VK_PRESENT_MODE_FIFO_KHR;
			}
		}
		string_catf(&info_str, "\nUsing: \"%s\"\n", present_mode_strs[vulkan->swap_chain.present_mode]);
	}
	{ // create swap chain, swap chain images
		VkSwapchainCreateInfoKHR swap_chain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
		swap_chain_create_info.surface = vulkan->swap_chain.surface;
		swap_chain_create_info.minImageCount = vulkan_swap_chain_image_count;
		swap_chain_create_info.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
		swap_chain_create_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swap_chain_create_info.imageExtent = vulkan->swap_chain.surface_capabilities.currentExtent;
		swap_chain_create_info.imageArrayLayers = 1;
		swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swap_chain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swap_chain_create_info.presentMode = vulkan->swap_chain.present_mode;
		swap_chain_create_info.clipped = VK_TRUE;
		m_vk_assert(vkCreateSwapchainKHR(vulkan->device.device, &swap_chain_create_info, nullptr, &vulkan->swap_chain.swap_chain));
		uint32 swap_chain_image_count = 0;
		m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, &swap_chain_image_count, nullptr));
		m_assert(swap_chain_image_count == vulkan_swap_chain_image_count);
		m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, &swap_chain_image_count, vulkan->swap_chain.images));
		for (uint32 i = 0; i < vulkan_swap_chain_image_count; i += 1) {
			VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_create_info.image = vulkan->swap_chain.images[i];
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = swap_chain_create_info.imageFormat;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.layerCount = 1;
			m_vk_assert(vkCreateImageView(vulkan->device.device, &image_view_create_info, nullptr, &vulkan->swap_chain.image_views[i]));
		}
		vulkan->swap_chain.image_width = swap_chain_create_info.imageExtent.width;
		vulkan->swap_chain.image_height = swap_chain_create_info.imageExtent.height;
		string_catf(&info_str, "\nSwapchain Image Size: %dx%d\n", vulkan->swap_chain.image_width, vulkan->swap_chain.image_height);
	}
}

void vulkan_swap_chain_resize_images(vulkan *vulkan, uint32 new_width, uint32 new_height) {
	// vkWaitForFences(vulkan->device.device, 1, &vulkan->syncs.queue_submit_fence, VK_TRUE, UINT64_MAX);

	// vkDestroySwapchainKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, nullptr);
	// for (uint32 i = 0; i < vulkan_swap_chain_image_count; i += 1) {
	// 	vkDestroyImageView(vulkan->device.device, vulkan->swap_chain.image_views[i], nullptr);
	// }
	// for (uint32 i = 0; i < vulkan_swap_chain_image_count; i += 1) {
	// 	vkDestroyFramebuffer(vulkan->device.device, vulkan->framebuffers.swap_chain_framebuffers[i], nullptr);
	// }

	// VkSurfaceCapabilitiesKHR surface_capabilities = {};
	// vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_capabilities);
	// m_assert(surface_capabilities.currentExtent.width == new_width && surface_capabilities.currentExtent.height == new_height);
	// VkSwapchainCreateInfoKHR swap_chain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
	// swap_chain_create_info.surface = vulkan->swap_chain.surface;
	// swap_chain_create_info.minImageCount = vulkan_swap_chain_image_count;
	// swap_chain_create_info.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
	// swap_chain_create_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	// swap_chain_create_info.imageExtent = surface_capabilities.currentExtent;
	// swap_chain_create_info.imageArrayLayers = 1;
	// swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	// swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	// swap_chain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	// swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	// swap_chain_create_info.presentMode = vulkan->swap_chain.present_mode;
	// swap_chain_create_info.clipped = VK_TRUE;
	// m_vk_assert(vkCreateSwapchainKHR(vulkan->device.device, &swap_chain_create_info, nullptr, &vulkan->swap_chain.swap_chain));
	// uint32 new_swap_chain_image_count = 0;
	// m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, &new_swap_chain_image_count, nullptr));
	// m_assert(new_swap_chain_image_count == vulkan_swap_chain_image_count);
	// m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, &new_swap_chain_image_count, vulkan->swap_chain.images));
	// for (uint32 i = 0; i < vulkan_swap_chain_image_count; i += 1) {
	// 	VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
	// 	image_view_create_info.image = vulkan->swap_chain.images[i];
	// 	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	// 	image_view_create_info.format = swap_chain_create_info.imageFormat;
	// 	image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	// 	image_view_create_info.subresourceRange.levelCount = 1;
	// 	image_view_create_info.subresourceRange.layerCount = 1;
	// 	m_vk_assert(vkCreateImageView(vulkan->device.device, &image_view_create_info, nullptr, &vulkan->swap_chain.image_views[i]));
	// }
	// vulkan->swap_chain.image_width = swap_chain_create_info.imageExtent.width;
	// vulkan->swap_chain.image_height = swap_chain_create_info.imageExtent.height;

	// for (uint32 i = 0; i < vulkan_swap_chain_image_count; i += 1) {
	// 	VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
	// 	framebuffer_create_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
	// 	framebuffer_create_info.attachmentCount = 1;
	// 	framebuffer_create_info.pAttachments = &vulkan->swap_chain.image_views[i];
	// 	framebuffer_create_info.width = vulkan->swap_chain.image_width;
	// 	framebuffer_create_info.height = vulkan->swap_chain.image_height;
	// 	framebuffer_create_info.layers = 1;
	// 	m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_create_info, nullptr, &vulkan->framebuffers.swap_chain_framebuffers[i]));
	// }
}

void vulkan_render_passes_initialize(VkSampleCountFlagBits sample_count, vulkan *vulkan) {
	{ // shadow map render passes
		{
			VkAttachmentDescription attachments[2] = {};
			attachments[0].format = VK_FORMAT_R32G32_SFLOAT;
			attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
			attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			attachments[1].format = VK_FORMAT_D32_SFLOAT;
			attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
			attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkAttachmentReference color_attachment_ref = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
			VkAttachmentReference depth_stencil_attachment_ref = {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

			VkSubpassDescription subpasses[1] = {};
			subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpasses[0].colorAttachmentCount = 1;
			subpasses[0].pColorAttachments = &color_attachment_ref;
			subpasses[0].pDepthStencilAttachment = &depth_stencil_attachment_ref;

			VkRenderPassCreateInfo render_pass_create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
			render_pass_create_info.attachmentCount = m_countof(attachments);
			render_pass_create_info.pAttachments = attachments;
			render_pass_create_info.subpassCount = m_countof(subpasses);
			render_pass_create_info.pSubpasses = subpasses;
			m_vk_assert(vkCreateRenderPass(vulkan->device.device, &render_pass_create_info, nullptr, &vulkan->render_passes.shadow_map_render_passes[0]));
		}
		{
			VkAttachmentDescription attachments[1] = {};
			attachments[0].format = VK_FORMAT_R32G32_SFLOAT;
			attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
			attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			VkAttachmentReference color_attachment_ref = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

			VkSubpassDescription subpasses[1] = {};
			subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpasses[0].colorAttachmentCount = 1;
			subpasses[0].pColorAttachments = &color_attachment_ref;

			VkSubpassDependency dependencies[1] = {};
			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			VkRenderPassCreateInfo render_pass_create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
			render_pass_create_info.attachmentCount = m_countof(attachments);
			render_pass_create_info.pAttachments = attachments;
			render_pass_create_info.subpassCount = m_countof(subpasses);
			render_pass_create_info.pSubpasses = subpasses;
			render_pass_create_info.dependencyCount = m_countof(dependencies);
			render_pass_create_info.pDependencies = dependencies;
			m_vk_assert(vkCreateRenderPass(vulkan->device.device, &render_pass_create_info, nullptr, &vulkan->render_passes.shadow_map_render_passes[1]));
			m_vk_assert(vkCreateRenderPass(vulkan->device.device, &render_pass_create_info, nullptr, &vulkan->render_passes.shadow_map_render_passes[2]));
		}
	}
	{ // main render pass
		VkAttachmentDescription attachments[3] = {};
		attachments[0].format = VK_FORMAT_R16G16B16A16_SFLOAT;
		attachments[0].samples = sample_count;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		if (sample_count == VK_SAMPLE_COUNT_1_BIT) {
			attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}

		attachments[1].format = VK_FORMAT_D32_SFLOAT;
		attachments[1].samples = sample_count;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		attachments[2].format = attachments[0].format;
		attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[2].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkAttachmentReference color_attachment_refs[1] = {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};
		VkAttachmentReference depth_stencil_attachment_refs[1] = {{1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL}};
		VkAttachmentReference resolve_attachment_refs[1] = {{2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};

		VkSubpassDescription subpasses[1] = {};
		subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[0].colorAttachmentCount = m_countof(color_attachment_refs);
		subpasses[0].pColorAttachments = color_attachment_refs;
		subpasses[0].pDepthStencilAttachment = depth_stencil_attachment_refs;
		if (sample_count != VK_SAMPLE_COUNT_1_BIT) {
			subpasses[0].pResolveAttachments = resolve_attachment_refs;
		}

		VkSubpassDependency dependencies[1] = {};
		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		VkRenderPassCreateInfo create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
		create_info.attachmentCount = (sample_count == VK_SAMPLE_COUNT_1_BIT) ? 2 : 3;
		create_info.pAttachments = attachments;
		create_info.subpassCount = m_countof(subpasses);
		create_info.pSubpasses = subpasses;
		create_info.dependencyCount = m_countof(dependencies);
		create_info.pDependencies = dependencies;
		m_vk_assert(vkCreateRenderPass(vulkan->device.device, &create_info, nullptr, &vulkan->render_passes.main_render_pass));
	}
	{ // swap chain render pass
		VkAttachmentDescription attachments[1] = {};
		attachments[0].format = VK_FORMAT_B8G8R8A8_SRGB;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment_refs[1] = {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};

		VkSubpassDescription subpasses[1] = {};
		subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[0].colorAttachmentCount = m_countof(color_attachment_refs);
		subpasses[0].pColorAttachments = color_attachment_refs;

		VkSubpassDependency dependencies[1] = {};
		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		VkRenderPassCreateInfo create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
		create_info.attachmentCount = m_countof(attachments);
		create_info.pAttachments = attachments;
		create_info.subpassCount = m_countof(subpasses);
		create_info.pSubpasses = subpasses;
		create_info.dependencyCount = m_countof(dependencies);
		create_info.pDependencies = dependencies;
		m_vk_assert(vkCreateRenderPass(vulkan->device.device, &create_info, nullptr, &vulkan->render_passes.swap_chain_render_pass));
	}
}

void vulkan_framebuffers_initialize(VkSampleCountFlagBits sample_count, vulkan *vulkan) {
	{ // shadow map framebuffers
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {auto *framebuffer = &vulkan->framebuffers.shadow_map_framebuffers[i];
			uint32 shadow_map_size = 2048;
			for (uint32 i = 0; i < m_countof(framebuffer->color_attachments); i += 1) {
				VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_create_info.imageType = VK_IMAGE_TYPE_2D;
				image_create_info.format = VK_FORMAT_R32G32_SFLOAT;
				image_create_info.extent = {shadow_map_size, shadow_map_size, 1};
				image_create_info.mipLevels = 1;
				image_create_info.arrayLayers = 1;
				image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
				image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_create_info.format = image_create_info.format;
				image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				image_view_create_info.subresourceRange.levelCount = 1;
				image_view_create_info.subresourceRange.layerCount = 1;
				m_assert(vulkan_allocate_image(&vulkan->device, image_create_info, image_view_create_info, &framebuffer->color_attachments[i].image_device_memory, &framebuffer->color_attachments[i].image, &framebuffer->color_attachments[i].image_view));
			}
			{
				VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_create_info.imageType = VK_IMAGE_TYPE_2D;
				image_create_info.format = VK_FORMAT_D32_SFLOAT;
				image_create_info.extent = {shadow_map_size, shadow_map_size, 1};
				image_create_info.mipLevels = 1;
				image_create_info.arrayLayers = 1;
				image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
				image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_create_info.format = image_create_info.format;
				image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				image_view_create_info.subresourceRange.levelCount = 1;
				image_view_create_info.subresourceRange.layerCount = 1;
				m_assert(vulkan_allocate_image(&vulkan->device, image_create_info, image_view_create_info, &framebuffer->depth_stencil_attachment.image_device_memory, &framebuffer->depth_stencil_attachment.image, &framebuffer->depth_stencil_attachment.image_view));
			}
			{
				VkImageView image_views[] = {framebuffer->color_attachments[0].image_view, framebuffer->depth_stencil_attachment.image_view};
				VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
				framebuffer_create_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
				framebuffer_create_info.attachmentCount = m_countof(image_views);
				framebuffer_create_info.pAttachments = image_views;
				framebuffer_create_info.width = shadow_map_size;
				framebuffer_create_info.height = shadow_map_size;
				framebuffer_create_info.layers = 1;
				m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_create_info, nullptr, &framebuffer->framebuffers[0]));
			}
			{
				VkImageView image_views[] = {framebuffer->color_attachments[1].image_view};
				VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
				framebuffer_create_info.renderPass = vulkan->render_passes.shadow_map_render_passes[1];
				framebuffer_create_info.attachmentCount = m_countof(image_views);
				framebuffer_create_info.pAttachments = image_views;
				framebuffer_create_info.width = shadow_map_size;
				framebuffer_create_info.height = shadow_map_size;
				framebuffer_create_info.layers = 1;
				m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_create_info, nullptr, &framebuffer->framebuffers[1]));
			}
			{
				VkImageView image_views[] = {framebuffer->color_attachments[2].image_view};
				VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
				framebuffer_create_info.renderPass = vulkan->render_passes.shadow_map_render_passes[2];
				framebuffer_create_info.attachmentCount = m_countof(image_views);
				framebuffer_create_info.pAttachments = image_views;
				framebuffer_create_info.width = shadow_map_size;
				framebuffer_create_info.height = shadow_map_size;
				framebuffer_create_info.layers = 1;
				m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_create_info, nullptr, &framebuffer->framebuffers[2]));
			}
			framebuffer->width = shadow_map_size;
			framebuffer->height = shadow_map_size;
		}
	}
	{ // main framebuffers
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			auto *framebuffer = &vulkan->framebuffers.main_framebuffers[i];
			uint32 framebuffer_width = vulkan->swap_chain.image_width;
			uint32 framebuffer_height = vulkan->swap_chain.image_height;
			{
				VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_create_info.imageType = VK_IMAGE_TYPE_2D;
				image_create_info.format = VK_FORMAT_R16G16B16A16_SFLOAT;
				image_create_info.extent = {framebuffer_width, framebuffer_height, 1};
				image_create_info.mipLevels = 1;
				image_create_info.arrayLayers = 1;
				image_create_info.samples = sample_count;
				image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_create_info.format = image_create_info.format;
				image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				image_view_create_info.subresourceRange.levelCount = 1;
				image_view_create_info.subresourceRange.layerCount = 1;
				m_assert(vulkan_allocate_image(&vulkan->device, image_create_info, image_view_create_info, &framebuffer->color_attachment.image_device_memory, &framebuffer->color_attachment.image, &framebuffer->color_attachment.image_view));
			}
			{
				VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_create_info.imageType = VK_IMAGE_TYPE_2D;
				image_create_info.format = VK_FORMAT_D32_SFLOAT;
				image_create_info.extent = {framebuffer_width, framebuffer_height, 1};
				image_create_info.mipLevels = 1;
				image_create_info.arrayLayers = 1;
				image_create_info.samples = sample_count;
				image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_create_info.format = image_create_info.format;
				image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				image_view_create_info.subresourceRange.levelCount = 1;
				image_view_create_info.subresourceRange.layerCount = 1;
				m_assert(vulkan_allocate_image(&vulkan->device, image_create_info, image_view_create_info, &framebuffer->depth_stencil_attachment.image_device_memory, &framebuffer->depth_stencil_attachment.image, &framebuffer->depth_stencil_attachment.image_view));
			}
			if (sample_count != VK_SAMPLE_COUNT_1_BIT) {
				VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_create_info.imageType = VK_IMAGE_TYPE_2D;
				image_create_info.format = VK_FORMAT_R16G16B16A16_SFLOAT;
				image_create_info.extent = {framebuffer_width, framebuffer_height, 1};
				image_create_info.mipLevels = 1;
				image_create_info.arrayLayers = 1;
				image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
				image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_create_info.format = image_create_info.format;
				image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				image_view_create_info.subresourceRange.levelCount = 1;
				image_view_create_info.subresourceRange.layerCount = 1;
				m_assert(vulkan_allocate_image(&vulkan->device, image_create_info, image_view_create_info, &framebuffer->color_resolve_attachment.image_device_memory, &framebuffer->color_resolve_attachment.image, &framebuffer->color_resolve_attachment.image_view));
			}
			VkImageView image_views[3] = {framebuffer->color_attachment.image_view, framebuffer->depth_stencil_attachment.image_view, framebuffer->color_resolve_attachment.image_view};
			VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
			framebuffer_create_info.renderPass = vulkan->render_passes.main_render_pass;
			framebuffer_create_info.attachmentCount = (sample_count == VK_SAMPLE_COUNT_1_BIT) ? 2 : 3;
			framebuffer_create_info.pAttachments = image_views;
			framebuffer_create_info.width = framebuffer_width;
			framebuffer_create_info.height = framebuffer_height;
			framebuffer_create_info.layers = 1;
			m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_create_info, nullptr, &framebuffer->framebuffer));
			framebuffer->width = framebuffer_width;
			framebuffer->height = framebuffer_height;
			framebuffer->sample_count = sample_count;
		}
	}
	{ // swap chain framebuffers
		for (uint32 i = 0; i < vulkan_swap_chain_image_count; i += 1) {
			VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
			framebuffer_create_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
			framebuffer_create_info.attachmentCount = 1;
			framebuffer_create_info.pAttachments = &vulkan->swap_chain.image_views[i];
			framebuffer_create_info.width = vulkan->swap_chain.image_width;
			framebuffer_create_info.height = vulkan->swap_chain.image_height;
			framebuffer_create_info.layers = 1;
			m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_create_info, nullptr, &vulkan->framebuffers.swap_chain_framebuffers[i]));
		}
	}
}

void vulkan_cmd_buffers_initialize(vulkan *vulkan) {
	VkCommandPoolCreateInfo cmd_pool_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
	cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	cmd_pool_info.queueFamilyIndex = vulkan->device.queue_family_index;
	m_vk_assert(vkCreateCommandPool(vulkan->device.device, &cmd_pool_info, nullptr, &vulkan->cmd_buffers.cmd_pool));

	VkCommandBufferAllocateInfo cmd_buffer_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
	cmd_buffer_info.commandPool = vulkan->cmd_buffers.cmd_pool;
	cmd_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd_buffer_info.commandBufferCount = vulkan_buffering_count;
	m_vk_assert(vkAllocateCommandBuffers(vulkan->device.device, &cmd_buffer_info, vulkan->cmd_buffers.graphic_cmd_buffers));
	cmd_buffer_info.commandBufferCount = 1;
	m_vk_assert(vkAllocateCommandBuffers(vulkan->device.device, &cmd_buffer_info, &vulkan->cmd_buffers.transfer_cmd_buffer));
}

void vulkan_syncs_initialize(vulkan *vulkan) {
	VkSemaphoreCreateInfo semaphore_create_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
	for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
		m_vk_assert(vkCreateSemaphore(vulkan->device.device, &semaphore_create_info, nullptr, &vulkan->syncs.swap_chain_image_acquire_semaphores[i]));
		m_vk_assert(vkCreateSemaphore(vulkan->device.device, &semaphore_create_info, nullptr, &vulkan->syncs.queue_submit_semaphores[i]));
		VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
		fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_vk_assert(vkCreateFence(vulkan->device.device, &fence_create_info, nullptr, &vulkan->syncs.queue_submit_fences[i]));
	}
}

void vulkan_memories_initialize(vulkan *vulkan) {
	auto find_memory_type_index = [vulkan](VkMemoryRequirements requirements, VkMemoryPropertyFlags flags, uint32 *memory_type_index) -> bool {
		VkPhysicalDeviceMemoryProperties *properties = &vulkan->device.physical_device_memory_properties;
		for (uint32 i = 0; i < properties->memoryTypeCount; i += 1) {
			if ((requirements.memoryTypeBits & (1 << i)) && properties->memoryTypes[i].propertyFlags == flags) {
				*memory_type_index = i;
				return true;
			}
		}
		return false;
	};
	auto allocate_memory_type = [vulkan](uint32 memory_type_index, uint64 size, vulkan_memory *vulkan_memory) {
		VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
		memory_allocate_info.allocationSize = size;
		memory_allocate_info.memoryTypeIndex = memory_type_index;
		VkDeviceMemory vk_device_memory = {};
		VkResult vk_result = vkAllocateMemory(vulkan->device.device, &memory_allocate_info, nullptr, &vk_device_memory);
		if (vk_result == VK_SUCCESS) {
			vulkan_memory->size = 0;
			vulkan_memory->capacity = size;
			vulkan_memory->memory = vk_device_memory;
			vulkan_memory->memory_type_index = memory_type_index;
			return true;
		}
		return false;
	};
	{ // common images
		VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = VK_FORMAT_B8G8R8A8_SRGB;
		image_create_info.extent = {1024, 1024, 1};
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImage vulkan_image = {};
		m_vk_assert(vkCreateImage(vulkan->device.device, &image_create_info, nullptr, &vulkan_image));
		VkMemoryRequirements requirements = {};
		vkGetImageMemoryRequirements(vulkan->device.device, vulkan_image, &requirements);
		vkDestroyImage(vulkan->device.device, vulkan_image, nullptr);
		uint32 memory_type_index = 0;
		m_assert(find_memory_type_index(requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memory_type_index));
		m_assert(allocate_memory_type(memory_type_index, m_megabytes(16), &vulkan->memories.common_images_memory));
	}
	{ // level images
		VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = VK_FORMAT_B8G8R8A8_SRGB;
		image_create_info.extent = {1024, 1024, 1};
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImage vulkan_image = {};
		m_vk_assert(vkCreateImage(vulkan->device.device, &image_create_info, nullptr, &vulkan_image));
		VkMemoryRequirements requirements = {};
		vkGetImageMemoryRequirements(vulkan->device.device, vulkan_image, &requirements);
		vkDestroyImage(vulkan->device.device, vulkan_image, nullptr);
		uint32 memory_type_index = 0;
		m_assert(find_memory_type_index(requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memory_type_index));
		m_assert(allocate_memory_type(memory_type_index, m_gigabytes(1), &vulkan->memories.level_images_memory));
	}
	{ // level vertices 
		VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_create_info.size = m_megabytes(32);
		buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		VkBuffer vulkan_buffer = {};
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_create_info, nullptr, &vulkan_buffer));
		VkMemoryRequirements requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, vulkan_buffer, &requirements);
		vkDestroyBuffer(vulkan->device.device, vulkan_buffer, nullptr);
		uint32 memory_type_index = 0;
		m_assert(find_memory_type_index(requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memory_type_index));
		m_assert(allocate_memory_type(memory_type_index, requirements.size, &vulkan->memories.level_vertex_buffer_memory));
	}
	{ // frame vertices
		VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_create_info.size = m_megabytes(4);
		buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		VkBuffer vulkan_buffer = {};
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_create_info, nullptr, &vulkan_buffer));
		VkMemoryRequirements requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, vulkan_buffer, &requirements);
		vkDestroyBuffer(vulkan->device.device, vulkan_buffer, nullptr);
		uint32 memory_type_index = 0;
		m_assert(find_memory_type_index(requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memory_type_index));
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			m_assert(allocate_memory_type(memory_type_index, requirements.size, &vulkan->memories.frame_vertex_buffer_memories[i]));
		}
	}
	{ // frame uniforms
		VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_create_info.size = m_megabytes(4);
		buffer_create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		VkBuffer vulkan_buffer = {};
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_create_info, nullptr, &vulkan_buffer));
		VkMemoryRequirements requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, vulkan_buffer, &requirements);
		vkDestroyBuffer(vulkan->device.device, vulkan_buffer, nullptr);
		uint32 memory_type_index = 0;
		m_assert(find_memory_type_index(requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memory_type_index));
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			m_assert(allocate_memory_type(memory_type_index, requirements.size, &vulkan->memories.frame_uniform_buffer_memories[i]));
		}
	}
}

void vulkan_buffer_create(vulkan_device *vulkan_device, vulkan_memory *vulkan_memory, VkBufferCreateInfo buffer_create_info, vulkan_buffer *vulkan_buffer) {
	m_vk_assert(vkCreateBuffer(vulkan_device->device, &buffer_create_info, nullptr, &vulkan_buffer->buffer));
	VkMemoryRequirements buffer_memory_requirements = {};
	vkGetBufferMemoryRequirements(vulkan_device->device, vulkan_buffer->buffer, &buffer_memory_requirements);
	uint64 memory_offset = round_up(vulkan_memory->size, buffer_memory_requirements.alignment);
	vulkan_memory->size = memory_offset + buffer_memory_requirements.size;
	m_assert(vulkan_memory->size <= vulkan_memory->capacity);
	m_vk_assert(vkBindBufferMemory(vulkan_device->device, vulkan_buffer->buffer, vulkan_memory->memory, memory_offset));
	vulkan_buffer->capacity = buffer_memory_requirements.size;
}

void vulkan_buffer_transfer(vulkan *vulkan, vulkan_buffer *vulkan_buffer, uint64 buffer_offset, const void *data, uint64 data_size) {
	m_assert((buffer_offset + data_size) <= vulkan_buffer->capacity);
	VkDeviceMemory staging_memory = {};
	VkBuffer staging_buffer = {};
	uint8 *staging_buffer_ptr = nullptr;
	VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
	buffer_create_info.size = data_size;
	buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	m_assert(vulkan_allocate_buffer(&vulkan->device, buffer_create_info, &staging_memory, &staging_buffer));
	vkMapMemory(vulkan->device.device, staging_memory, 0, VK_WHOLE_SIZE, 0, (void **)&staging_buffer_ptr);
	memcpy(staging_buffer_ptr, data, data_size);
	vkUnmapMemory(vulkan->device.device, staging_memory);

	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);
	VkBufferCopy buffer_copy = {0, buffer_offset, data_size};
	vkCmdCopyBuffer(cmd_buffer, staging_buffer, vulkan_buffer->buffer, 1, &buffer_copy);
	vkEndCommandBuffer(cmd_buffer);
	VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;
	vkQueueSubmit(vulkan->device.queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkan->device.queue);

	vkDestroyBuffer(vulkan->device.device, staging_buffer, nullptr);
	vkFreeMemory(vulkan->device.device, staging_memory, nullptr);
}

void vulkan_buffers_initialize(vulkan *vulkan) {
	VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
	buffer_create_info.size = vulkan->memories.level_vertex_buffer_memory.capacity;
	buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	vulkan_buffer_create(&vulkan->device, &vulkan->memories.level_vertex_buffer_memory, buffer_create_info, &vulkan->buffers.level_vertex_buffer);
	vulkan->buffers.level_vertex_buffer_offset = 0;
	for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
		buffer_create_info.size = vulkan->memories.frame_vertex_buffer_memories[i].capacity;
		buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		vulkan_buffer_create(&vulkan->device, &vulkan->memories.frame_vertex_buffer_memories[i], buffer_create_info, &vulkan->buffers.frame_vertex_buffers[i]);
		vkMapMemory(vulkan->device.device, vulkan->memories.frame_vertex_buffer_memories[i].memory, 0, VK_WHOLE_SIZE, 0, (void **)&vulkan->buffers.frame_vertex_buffer_ptrs[i]);
		vulkan->buffers.frame_vertex_buffer_offsets[i] = 0;
	}
	for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
		buffer_create_info.size = vulkan->memories.frame_uniform_buffer_memories[i].capacity;
		buffer_create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		vulkan_buffer_create(&vulkan->device, &vulkan->memories.frame_uniform_buffer_memories[i], buffer_create_info, &vulkan->buffers.frame_uniform_buffers[i]);
		vkMapMemory(vulkan->device.device, vulkan->memories.frame_uniform_buffer_memories[i].memory, 0, VK_WHOLE_SIZE, 0, (void **)&vulkan->buffers.frame_uniform_buffer_ptrs[i]);
		vulkan->buffers.frame_uniform_buffer_offsets[i] = 0;
	}
}

void vulkan_image_create(vulkan *vulkan, vulkan_memory *vulkan_memory, VkImageCreateInfo image_create_info, VkImageViewCreateInfo image_view_create_info, vulkan_image *vulkan_image) {
	m_vk_assert(vkCreateImage(vulkan->device.device, &image_create_info, nullptr, &vulkan_image->image));
	VkMemoryRequirements image_memory_requirements = {};
	vkGetImageMemoryRequirements(vulkan->device.device, vulkan_image->image, &image_memory_requirements);
	uint64 memory_offset = round_up(vulkan_memory->size, image_memory_requirements.alignment);
	vulkan_memory->size = memory_offset + image_memory_requirements.size;
	m_assert(vulkan_memory->size <= vulkan_memory->capacity);
	m_vk_assert(vkBindImageMemory(vulkan->device.device, vulkan_image->image, vulkan_memory->memory, memory_offset));

	image_view_create_info.image = vulkan_image->image;
	m_vk_assert(vkCreateImageView(vulkan->device.device, &image_view_create_info, nullptr, &vulkan_image->view));

	vulkan_image->width = image_create_info.extent.width;
	vulkan_image->height = image_create_info.extent.height;
	vulkan_image->pixel_format = image_create_info.format;
	vulkan_image->mipmap_count = image_create_info.mipLevels;
	vulkan_image->layer_count = image_create_info.arrayLayers;
}

void vulkan_image_destroy(vulkan_device *vulkan_device, vulkan_image *vulkan_image) {
	vkDestroyImageView(vulkan_device->device, vulkan_image->view, nullptr);
	vkDestroyImage(vulkan_device->device, vulkan_image->image, nullptr);
}

void vulkan_image_transfer(vulkan *vulkan, vulkan_image *vulkan_image, uint8 *image_data, uint64 image_data_size, uint32 pixel_bit_size, uint32 pixel_block_size) {
	VkDeviceMemory staging_memory = {};
	VkBuffer staging_buffer = {};
	uint8 *staging_buffer_ptr = nullptr;
	VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
	buffer_create_info.size = image_data_size;
	buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	m_assert(vulkan_allocate_buffer(&vulkan->device, buffer_create_info, &staging_memory, &staging_buffer));
	m_scope_exit(
		vkDestroyBuffer(vulkan->device.device, staging_buffer, nullptr);
		vkFreeMemory(vulkan->device.device, staging_memory, nullptr);
	);
	vkMapMemory(vulkan->device.device, staging_memory, 0, VK_WHOLE_SIZE, 0, (void **)&staging_buffer_ptr);
	memcpy(staging_buffer_ptr, image_data, image_data_size);
	vkUnmapMemory(vulkan->device.device, staging_memory);

	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);
	{
		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.image = vulkan_image->image;
		image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_memory_barrier.subresourceRange.levelCount = vulkan_image->mipmap_count;
		image_memory_barrier.subresourceRange.layerCount = vulkan_image->layer_count;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
	}
	{
		uint32 buffer_offset = 0;
		for (uint32 i = 0; i < vulkan_image->layer_count; i += 1) {
			uint32 mipmap_width = vulkan_image->width;
			uint32 mipmap_height = vulkan_image->height;
			for (uint32 j = 0; j < vulkan_image->mipmap_count; j += 1) {
				VkBufferImageCopy buffer_image_copy = {};
				buffer_image_copy.bufferOffset = buffer_offset;
				buffer_image_copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				buffer_image_copy.imageSubresource.mipLevel = j;
				buffer_image_copy.imageSubresource.baseArrayLayer = i;
				buffer_image_copy.imageSubresource.layerCount = 1;
				buffer_image_copy.imageExtent = {mipmap_width, mipmap_height, 1};
				vkCmdCopyBufferToImage(cmd_buffer, staging_buffer, vulkan_image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);
				buffer_offset = buffer_offset + (mipmap_width * mipmap_height * pixel_bit_size) / 8;
				mipmap_width = max(mipmap_width / 2, pixel_block_size);
				mipmap_height = max(mipmap_height / 2, pixel_block_size);
			}
		}
	}
	{
		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		image_memory_barrier.image = vulkan_image->image;
		image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_memory_barrier.subresourceRange.levelCount = vulkan_image->mipmap_count;
		image_memory_barrier.subresourceRange.layerCount = vulkan_image->layer_count;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
	}
	vkEndCommandBuffer(cmd_buffer);
	VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;
	vkQueueSubmit(vulkan->device.queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkan->device.queue);
}

void vulkan_images_initialize(vulkan *vulkan) {
	{
		uint8 default_albedo_map_data[] = {200, 200, 200, 255, 200, 200, 200, 255, 200, 200, 200, 255, 200, 200, 200, 255};
		uint8 default_normal_map_data[] = {128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0};
		uint8 default_metallic_map_data[] = {0, 0, 0, 0};
		uint8 default_roughness_map_data[] = {128, 128, 128, 128};
		uint8 default_ao_map_data[] = {255, 255, 255, 255};
		uint8 default_height_map_data[] = {0, 0, 0, 0};

		VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_create_info.extent = {2, 2, 1};
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = image_create_info.format;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.layerCount = 1;
		vulkan_image_create(vulkan, &vulkan->memories.common_images_memory, image_create_info, image_view_create_info, &vulkan->images.default_albedo_map_image);
		vulkan_image_transfer(vulkan, &vulkan->images.default_albedo_map_image, default_albedo_map_data, sizeof(default_albedo_map_data), 32, 1);

		image_create_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_view_create_info.format = image_create_info.format;
		vulkan_image_create(vulkan, &vulkan->memories.common_images_memory, image_create_info, image_view_create_info, &vulkan->images.default_normal_map_image);
		vulkan_image_transfer(vulkan, &vulkan->images.default_normal_map_image, default_normal_map_data, sizeof(default_normal_map_data), 32, 1);

		image_create_info.format = VK_FORMAT_R8_UNORM;
		image_view_create_info.format = image_create_info.format;
		vulkan_image_create(vulkan, &vulkan->memories.common_images_memory, image_create_info, image_view_create_info, &vulkan->images.default_metallic_map_image);
		vulkan_image_transfer(vulkan, &vulkan->images.default_metallic_map_image, default_metallic_map_data, sizeof(default_metallic_map_data), 8, 1);

		image_create_info.format = VK_FORMAT_R8_UNORM;
		image_view_create_info.format = image_create_info.format;
		vulkan_image_create(vulkan, &vulkan->memories.common_images_memory, image_create_info, image_view_create_info, &vulkan->images.default_roughness_map_image);
		vulkan_image_transfer(vulkan, &vulkan->images.default_roughness_map_image, default_roughness_map_data, sizeof(default_roughness_map_data), 8, 1);

		image_create_info.format = VK_FORMAT_R8_UNORM;
		image_view_create_info.format = image_create_info.format;
		vulkan_image_create(vulkan, &vulkan->memories.common_images_memory, image_create_info, image_view_create_info, &vulkan->images.default_ao_map_image);
		vulkan_image_transfer(vulkan, &vulkan->images.default_ao_map_image, default_ao_map_data, sizeof(default_ao_map_data), 8, 1);

		image_create_info.format = VK_FORMAT_R8_UNORM;
		image_view_create_info.format = image_create_info.format;
		vulkan_image_create(vulkan, &vulkan->memories.common_images_memory, image_create_info, image_view_create_info, &vulkan->images.default_height_map_image);
		vulkan_image_transfer(vulkan, &vulkan->images.default_height_map_image, default_height_map_data, sizeof(default_height_map_data), 8, 1);
	}
}

void vulkan_samplers_initialize(vulkan *vulkan) {
	{ // shadow map sampler
		VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
		sampler_create_info.magFilter = VK_FILTER_LINEAR;
		sampler_create_info.minFilter = VK_FILTER_LINEAR;
		sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		sampler_create_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.shadow_map_sampler));
	}
	{ // gaussian blur
		VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
		sampler_create_info.magFilter = VK_FILTER_LINEAR;
		sampler_create_info.minFilter = VK_FILTER_LINEAR;
		sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.gaussian_blur_sampler));
	}
	{ // skybox cubemap sampler
		VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
		sampler_create_info.magFilter = VK_FILTER_LINEAR;
		sampler_create_info.minFilter = VK_FILTER_LINEAR;
		sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.skybox_cubemap_sampler));
	}
	{ // mipmap images ? 
		for (uint32 i = 0; i < m_countof(vulkan->samplers.mipmap_image_samplers); i += 1) {
			VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
			sampler_create_info.magFilter = VK_FILTER_LINEAR;
			sampler_create_info.minFilter = VK_FILTER_LINEAR;
			sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			sampler_create_info.maxLod = (float)i;
			m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.mipmap_image_samplers[i]));
		}
	}
}

void vulkan_descriptors_initialize(vulkan *vulkan) {
	{ // pool
		VkDescriptorPoolCreateInfo descriptor_pool_create_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
		descriptor_pool_create_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptor_pool_create_info.maxSets = 4096;
		VkDescriptorPoolSize descriptor_pool_sizes[] = {{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4096}, {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 32}};
		descriptor_pool_create_info.poolSizeCount = m_countof(descriptor_pool_sizes);
		descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes;
		m_vk_assert(vkCreateDescriptorPool(vulkan->device.device, &descriptor_pool_create_info, nullptr, &vulkan->descriptors.pool));
	}
	{ // set layouts
		{
			VkDescriptorSetLayoutBinding descriptor_set_layout_bindings[3] = {};
			for (uint32 i = 0; i < m_countof(descriptor_set_layout_bindings); i += 1) {
				descriptor_set_layout_bindings[i] = {i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
			}
			VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
			descriptor_set_layout_info.bindingCount = m_countof(descriptor_set_layout_bindings);
			descriptor_set_layout_info.pBindings = descriptor_set_layout_bindings;
			m_vk_assert(vkCreateDescriptorSetLayout(vulkan->device.device, &descriptor_set_layout_info, nullptr, &vulkan->descriptors.uniform_buffers_set_layout));
		}
		{
			VkDescriptorSetLayoutBinding descriptor_set_layout_bindings[m_countof(vulkan->descriptors.sampled_images_set_layouts)] = {};
			for (uint32 i = 0; i < m_countof(vulkan->descriptors.sampled_images_set_layouts); i += 1) {
				descriptor_set_layout_bindings[i] = {i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
			}
			for (uint32 i = 0; i < m_countof(vulkan->descriptors.sampled_images_set_layouts); i += 1) {
				VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
				descriptor_set_layout_info.bindingCount = i + 1;
				descriptor_set_layout_info.pBindings = descriptor_set_layout_bindings;
				m_vk_assert(vkCreateDescriptorSetLayout(vulkan->device.device, &descriptor_set_layout_info, nullptr, &vulkan->descriptors.sampled_images_set_layouts[i]));
			}
		}
	}
	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
	descriptor_set_allocate_info.descriptorPool = vulkan->descriptors.pool;
	{ // frame uniform buffer offsets
		VkDescriptorSetLayout set_layouts[vulkan_buffering_count];
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			set_layouts[i] = vulkan->descriptors.uniform_buffers_set_layout;
		}
		descriptor_set_allocate_info.descriptorSetCount = vulkan_buffering_count;
		descriptor_set_allocate_info.pSetLayouts = set_layouts;
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, vulkan->descriptors.frame_uniform_buffer_offsets));
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			VkDescriptorBufferInfo descriptor_buffer_infos[3] = {};
			descriptor_buffer_infos[0] = {vulkan->buffers.frame_uniform_buffers[i].buffer, 0, 128 * sizeof(union mat4)};
			descriptor_buffer_infos[1] = {vulkan->buffers.frame_uniform_buffers[i].buffer, 0, 128 * sizeof(union mat4)};
			descriptor_buffer_infos[2] = {vulkan->buffers.frame_uniform_buffers[i].buffer, 0, 128 * sizeof(union mat4)};
			VkWriteDescriptorSet write_descriptor_sets[3] = {{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET}, {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET}, {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET}};
			write_descriptor_sets[0].dstSet = vulkan->descriptors.frame_uniform_buffer_offsets[i];
			write_descriptor_sets[0].dstBinding = 0;
			write_descriptor_sets[0].descriptorCount = 1;
			write_descriptor_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			write_descriptor_sets[0].pBufferInfo = &descriptor_buffer_infos[0];
			write_descriptor_sets[1].dstSet = vulkan->descriptors.frame_uniform_buffer_offsets[i];
			write_descriptor_sets[1].dstBinding = 1;
			write_descriptor_sets[1].descriptorCount = 1;
			write_descriptor_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			write_descriptor_sets[1].pBufferInfo = &descriptor_buffer_infos[1];
			write_descriptor_sets[2].dstSet = vulkan->descriptors.frame_uniform_buffer_offsets[i];
			write_descriptor_sets[2].dstBinding = 2;
			write_descriptor_sets[2].descriptorCount = 1;
			write_descriptor_sets[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			write_descriptor_sets[2].pBufferInfo = &descriptor_buffer_infos[2];
			vkUpdateDescriptorSets(vulkan->device.device, m_countof(write_descriptor_sets), write_descriptor_sets, 0, nullptr);
		}
	}
	{ // main framebuffer images
		VkDescriptorSetLayout set_layouts[vulkan_buffering_count];
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			set_layouts[i] = vulkan->descriptors.sampled_images_set_layouts[0];
		}
		descriptor_set_allocate_info.descriptorSetCount = vulkan_buffering_count;
		descriptor_set_allocate_info.pSetLayouts = set_layouts;
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, vulkan->descriptors.main_framebuffer_images));
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			auto *framebuffer = &vulkan->framebuffers.main_framebuffers[i];
			VkImageView image_view = (framebuffer->sample_count == VK_SAMPLE_COUNT_1_BIT) ? framebuffer->color_attachment.image_view : framebuffer->color_resolve_attachment.image_view;
			VkDescriptorImageInfo descriptor_image_info = {vulkan->samplers.mipmap_image_samplers[0], image_view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
			VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
			write_descriptor_set.dstSet = vulkan->descriptors.main_framebuffer_images[i];
			write_descriptor_set.dstBinding = 0;
			write_descriptor_set.descriptorCount = 1;
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_descriptor_set.pImageInfo = &descriptor_image_info;
			vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
		}
	}
	{ // shadow map framebuffer images
		VkDescriptorSetLayout set_layouts[vulkan_buffering_count * 3];
		for (uint32 i = 0; i < vulkan_buffering_count * 3; i += 1) {
			set_layouts[i] = vulkan->descriptors.sampled_images_set_layouts[0];
		}
		descriptor_set_allocate_info.descriptorSetCount = vulkan_buffering_count * 3;
		descriptor_set_allocate_info.pSetLayouts = set_layouts;
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &vulkan->descriptors.shadow_map_framebuffer_images[0][0]));
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			auto *framebuffer = &vulkan->framebuffers.shadow_map_framebuffers[i];
			VkDescriptorSet *images = vulkan->descriptors.shadow_map_framebuffer_images[i];
			VkSampler samplers[3] = {vulkan->samplers.gaussian_blur_sampler, vulkan->samplers.gaussian_blur_sampler, vulkan->samplers.shadow_map_sampler};
			for (uint32 i = 0; i < 3; i += 1) {
				VkDescriptorImageInfo descriptor_image_info = {samplers[i], framebuffer->color_attachments[i].image_view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
				VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
				write_descriptor_set.dstSet = images[i];
				write_descriptor_set.dstBinding = 0;
				write_descriptor_set.descriptorCount = 1;
				write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				write_descriptor_set.pImageInfo = &descriptor_image_info;
				vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
			}
		}
	}
	{ // model default material images
		descriptor_set_allocate_info.descriptorSetCount = 1;
		descriptor_set_allocate_info.pSetLayouts = &vulkan->descriptors.sampled_images_set_layouts[5];
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &vulkan->descriptors.model_default_material_images));
		VkDescriptorImageInfo descriptor_image_infos[6] = {};
		descriptor_image_infos[0] = {vulkan->samplers.mipmap_image_samplers[0], vulkan->images.default_albedo_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[1] = {vulkan->samplers.mipmap_image_samplers[0], vulkan->images.default_normal_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[2] = {vulkan->samplers.mipmap_image_samplers[0], vulkan->images.default_metallic_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[3] = {vulkan->samplers.mipmap_image_samplers[0], vulkan->images.default_roughness_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[4] = {vulkan->samplers.mipmap_image_samplers[0], vulkan->images.default_ao_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[5] = {vulkan->samplers.mipmap_image_samplers[0], vulkan->images.default_height_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		VkWriteDescriptorSet write_descriptor_sets[6] = {};
		for (uint32 i = 0; i < m_countof(write_descriptor_sets); i += 1) {
			write_descriptor_sets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_sets[i].dstSet = vulkan->descriptors.model_default_material_images;
			write_descriptor_sets[i].dstBinding = i;
			write_descriptor_sets[i].descriptorCount = 1;
			write_descriptor_sets[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_descriptor_sets[i].pImageInfo = &descriptor_image_infos[i];
		}
		vkUpdateDescriptorSets(vulkan->device.device, m_countof(write_descriptor_sets), write_descriptor_sets, 0, nullptr);
	}
	// { // text
	// 	descriptor_set_allocate_info.descriptorSetCount = 1;
	// 	descriptor_set_allocate_info.pSetLayouts = vulkan->pipelines.text_pipeline.descriptor_set_layouts;
	// 	m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &vulkan->descriptor_sets.text_uniform_descriptor_set));
	// 	VkDescriptorBufferInfo descriptor_buffer_infos[1] = {{vulkan->buffers.frame_uniforms_buffer.buffer, 0, 256 * sizeof(union mat4)}};
	// 	VkWriteDescriptorSet write_descriptor_sets[1] = {{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET}};
	// 	write_descriptor_sets[0].dstSet = vulkan->descriptor_sets.text_uniform_descriptor_set;
	// 	write_descriptor_sets[0].dstBinding = 0;
	// 	write_descriptor_sets[0].descriptorCount = 1;
	// 	write_descriptor_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	// 	write_descriptor_sets[0].pBufferInfo = &descriptor_buffer_infos[0];
	// 	vkUpdateDescriptorSets(vulkan->device.device, m_countof(write_descriptor_sets), write_descriptor_sets, 0, nullptr);
	// }
	// { // bound
	// 	descriptor_set_allocate_info.descriptorSetCount = 1;
	// 	descriptor_set_allocate_info.pSetLayouts = vulkan->pipelines.bound_pipeline.descriptor_set_layouts;
	// 	m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &vulkan->descriptor_sets.bound_uniform_descriptor_set));
	// 	VkDescriptorBufferInfo descriptor_buffer_info = {vulkan->buffers.frame_uniforms_buffer.buffer, 0, 128};
	// 	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	// 	write_descriptor_set.dstSet = vulkan->descriptor_sets.bound_uniform_descriptor_set;
	// 	write_descriptor_set.dstBinding = 0;
	// 	write_descriptor_set.descriptorCount = 1;
	// 	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	// 	write_descriptor_set.pBufferInfo = &descriptor_buffer_info;
	// 	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	// }
	// { // reference grid
	// 	descriptor_set_allocate_info.descriptorSetCount = 1;
	// 	descriptor_set_allocate_info.pSetLayouts = vulkan->pipelines.reference_grid_pipeline.descriptor_set_layouts;
	// 	m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &vulkan->descriptor_sets.reference_grid_uniform_descriptor_set));
	// 	VkDescriptorBufferInfo descriptor_buffer_info = {vulkan->buffers.frame_uniforms_buffer.buffer, 0, 128};
	// 	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	// 	write_descriptor_set.dstSet = vulkan->descriptor_sets.reference_grid_uniform_descriptor_set;
	// 	write_descriptor_set.dstBinding = 0;
	// 	write_descriptor_set.descriptorCount = 1;
	// 	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	// 	write_descriptor_set.pBufferInfo = &descriptor_buffer_info;
	// 	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	// }
}

void vulkan_pipelines_initialize(VkSampleCountFlagBits sample_count, vulkan *vulkan) {
	auto shader_module_from_file = [vulkan](const char *file_path) {
		VkShaderModule shader_module = {};
		file_mapping file_mapping = {};
		if (!open_file_mapping(file_path, &file_mapping)) {
			fatal("cannot open shader file \"%s\"", file_path);
		}
		VkShaderModuleCreateInfo info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
		info.codeSize = file_mapping.size;
		info.pCode = (const uint32 *)file_mapping.ptr;
		m_vk_assert(vkCreateShaderModule(vulkan->device.device, &info, nullptr, &shader_module));
		close_file_mapping(file_mapping);
		return shader_module;
	};
	{ // static model
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\static_model.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\static_model.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 32, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[4] = {};
		vertex_attributes[0] = {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0};
		vertex_attributes[1] = {1, 0, VK_FORMAT_R32G32_SFLOAT, 12};
		vertex_attributes[2] = {2, 0, VK_FORMAT_R16G16B16_SNORM, 20};
		vertex_attributes[3] = {3, 0, VK_FORMAT_R16G16B16_SNORM, 26};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = sample_count;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_TRUE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout, vulkan->descriptors.sampled_images_set_layouts[5], vulkan->descriptors.sampled_images_set_layouts[0]};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = 0;
		layout_info.pPushConstantRanges = nullptr;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.main_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.static_model_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // static model wireframe
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\static_model_wireframe.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\static_model_wireframe.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 32, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[1] = {};
		vertex_attributes[0] = {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_LINE;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.depthBiasEnable = VK_TRUE;
		rasterization_state.depthBiasSlopeFactor = 1;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = sample_count;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = 0;
		layout_info.pPushConstantRanges = nullptr;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.main_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.static_model_wireframe_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // static model shadow map
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\static_model_shadow_map.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\static_model_shadow_map.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 32, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[1] = {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_TRUE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_LESS;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_VERTEX_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.static_model_shadow_map_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // animated model
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\animated_model.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\animated_model.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 44, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[6] = {};
		vertex_attributes[0] = {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0};
		vertex_attributes[1] = {1, 0, VK_FORMAT_R32G32_SFLOAT, 12};
		vertex_attributes[2] = {2, 0, VK_FORMAT_R16G16B16_SNORM, 20};
		vertex_attributes[3] = {3, 0, VK_FORMAT_R16G16B16_SNORM, 26};
		vertex_attributes[4] = {4, 0, VK_FORMAT_R8G8B8A8_UINT, 32};
		vertex_attributes[5] = {5, 0, VK_FORMAT_R16G16B16A16_SFLOAT, 36};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = sample_count;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_TRUE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout, vulkan->descriptors.sampled_images_set_layouts[5]};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.main_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.animated_model_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // animated model shadow map
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\animated_model_shadow_map.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\animated_model_shadow_map.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 44, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[3] = {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}, {1, 0, VK_FORMAT_R8G8B8A8_UINT, 32}, {2, 0, VK_FORMAT_R16G16B16A16_SFLOAT, 36}};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_TRUE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_LESS;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_VERTEX_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.animated_model_shadow_map_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // shadow map gaussian blur
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\gaussian_blur.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\gaussian_blur.frag.spv");
		shader_stages[1].pName = "main";

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = 0;
		vertex_input_state.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_FALSE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.sampled_images_set_layouts[0]};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_FRAGMENT_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.shadow_map_render_passes[1];
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline_0 = &vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0];
		vulkan_pipeline *pipeline_1 = &vulkan->pipelines.shadow_map_gaussian_blur_pipelines[1];
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline_0->pipeline));
		create_info.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
		create_info.renderPass = vulkan->render_passes.shadow_map_render_passes[2];
		create_info.basePipelineHandle = pipeline_0->pipeline;
		create_info.basePipelineIndex = -1;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline_1->pipeline));
		pipeline_0->layout = layout;
		pipeline_1->layout = layout;
	}
	{ // skybox
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\skybox.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\skybox.frag.spv");
		shader_stages[1].pName = "main";

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = 0;
		vertex_input_state.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_NONE;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = sample_count;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.sampled_images_set_layouts[0]};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_VERTEX_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.main_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.skybox_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // imgui
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\imgui.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\imgui.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 20, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[3] = {};
		vertex_attributes[0] = {0, 0, VK_FORMAT_R32G32_SFLOAT, 0};
		vertex_attributes[1] = {1, 0, VK_FORMAT_R32G32_SFLOAT, 8};
		vertex_attributes[2] = {2, 0, VK_FORMAT_R8G8B8A8_UNORM, 16};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_NONE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_FALSE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_TRUE;
		color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.sampled_images_set_layouts[0]};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_VERTEX_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.imgui_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // text
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\text.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\text.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 24, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[4] = {};
		vertex_attributes[0] = {0, 0, VK_FORMAT_R32G32_SFLOAT, 0};
		vertex_attributes[1] = {1, 0, VK_FORMAT_R32G32_SFLOAT, 8};
		vertex_attributes[2] = {2, 0, VK_FORMAT_R8G8B8A8_UNORM, 16};
		vertex_attributes[3] = {3, 0, VK_FORMAT_R32_UINT, 20};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = sample_count;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_TRUE;
		color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout, vulkan->descriptors.sampled_images_set_layouts[0]};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = 0;
		layout_info.pPushConstantRanges = nullptr;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.main_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.text_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // collision shape
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\collision_shape.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\collision_shape.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[] = {0, 12, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[] = {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 4;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_TRUE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_TRUE;
		color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.main_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.collision_shape_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // lines
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\lines.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\lines.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[] = {0, 16, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[] = {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}, {1, 0, VK_FORMAT_R8G8B8A8_UNORM, 12}};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 4;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.uniform_buffers_set_layout};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_VERTEX_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.main_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.lines_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
	{ // swap chain
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\swap_chain.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\swap_chain.frag.spv");
		shader_stages[1].pName = "main";

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = 0;
		vertex_input_state.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_FALSE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;

		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkDescriptorSetLayout set_layouts[] = {vulkan->descriptors.sampled_images_set_layouts[0]};
		VkPushConstantRange push_constant_ranges[] = {{VK_SHADER_STAGE_FRAGMENT_BIT, 0, 128}};
		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = m_countof(set_layouts);
		layout_info.pSetLayouts = set_layouts;
		layout_info.pushConstantRangeCount = m_countof(push_constant_ranges);
		layout_info.pPushConstantRanges = push_constant_ranges;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &layout_info, nullptr, &layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = layout;
		create_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		vulkan_pipeline *pipeline = &vulkan->pipelines.swap_chain_pipeline;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline->pipeline));
		pipeline->layout = layout;
	}
}

void initialize_vulkan(vulkan *vulkan, const window &window) {
	VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT;
	bool vsync_on = false;
	vulkan_device_initialize(&vulkan->device);
	vulkan_swap_chain_initialize(window, vsync_on, vulkan);
	vulkan_render_passes_initialize(sample_count, vulkan);
	vulkan_framebuffers_initialize(sample_count, vulkan);
	vulkan_cmd_buffers_initialize(vulkan);
	vulkan_syncs_initialize(vulkan);
	vulkan_memories_initialize(vulkan);
	vulkan_buffers_initialize(vulkan);
	vulkan_images_initialize(vulkan);
	vulkan_samplers_initialize(vulkan);
	vulkan_descriptors_initialize(vulkan);
	vulkan_pipelines_initialize(sample_count, vulkan);
}

void vulkan_begin_render(vulkan *vulkan) {
	vkWaitForFences(vulkan->device.device, 1, &vulkan->syncs.queue_submit_fences[vulkan->frame_index], VK_TRUE, UINT64_MAX);
	vkResetFences(vulkan->device.device, 1, &vulkan->syncs.queue_submit_fences[vulkan->frame_index]);

	m_vk_assert(vkAcquireNextImageKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, 0, vulkan->syncs.swap_chain_image_acquire_semaphores[vulkan->frame_index], VK_NULL_HANDLE, &vulkan->swap_chain_image_index));

	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	m_vk_assert(vkBeginCommandBuffer(vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index], &cmd_buffer_begin_info));
}

void vulkan_end_render(vulkan *vulkan, bool screen_shot = false) {
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];

	VkDeviceMemory screen_shot_staging_memory = {};
	VkBuffer screen_shot_staging_buffer = {};
	if (screen_shot) {
		{
			VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
			image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			image_memory_barrier.image = vulkan->swap_chain.images[vulkan->frame_index];
			image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_memory_barrier.subresourceRange.levelCount = 1;
			image_memory_barrier.subresourceRange.layerCount = 1;
			vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
		}
		{
			VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
			buffer_create_info.size = vulkan->swap_chain.image_width * vulkan->swap_chain.image_height * 4;
			buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			m_assert(vulkan_allocate_buffer(&vulkan->device, buffer_create_info, &screen_shot_staging_memory, &screen_shot_staging_buffer));
			VkBufferImageCopy copy_region = {0, 0, 0, {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1}, {0, 0, 0}, {vulkan->swap_chain.image_width, vulkan->swap_chain.image_height, 1}};
			vkCmdCopyImageToBuffer(cmd_buffer, vulkan->swap_chain.images[vulkan->frame_index], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, screen_shot_staging_buffer, 1, &copy_region);
		}
		{
			VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
			image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			image_memory_barrier.image = vulkan->swap_chain.images[vulkan->frame_index];
			image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_memory_barrier.subresourceRange.levelCount = 1;
			image_memory_barrier.subresourceRange.layerCount = 1;
			vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
		}
	}
	m_vk_assert(vkEndCommandBuffer(cmd_buffer));

	VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo queue_submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	queue_submit_info.waitSemaphoreCount = 1;
	queue_submit_info.pWaitSemaphores = &vulkan->syncs.swap_chain_image_acquire_semaphores[vulkan->frame_index];
	queue_submit_info.pWaitDstStageMask = &wait_dst_stage_mask;
	queue_submit_info.commandBufferCount = 1;
	queue_submit_info.pCommandBuffers = &cmd_buffer;
	queue_submit_info.signalSemaphoreCount = 1;
	queue_submit_info.pSignalSemaphores = &vulkan->syncs.queue_submit_semaphores[vulkan->frame_index];
	m_vk_assert(vkQueueSubmit(vulkan->device.queue, 1, &queue_submit_info, vulkan->syncs.queue_submit_fences[vulkan->frame_index]));

	if (screen_shot) {
		vkWaitForFences(vulkan->device.device, 1, &vulkan->syncs.queue_submit_fences[vulkan->frame_index], VK_TRUE, UINT64_MAX);
		uint8 *screen_shot_staging_memory_ptr = nullptr;
		vkMapMemory(vulkan->device.device, screen_shot_staging_memory, 0, VK_WHOLE_SIZE, 0, (void **)&screen_shot_staging_memory_ptr);
		flip_rgba_image(screen_shot_staging_memory_ptr, vulkan->swap_chain.image_width, vulkan->swap_chain.image_height);
		rgba_image_to_bmp_file("screen_shot.bmp", screen_shot_staging_memory_ptr, vulkan->swap_chain.image_width, vulkan->swap_chain.image_height);
		vkUnmapMemory(vulkan->device.device, screen_shot_staging_memory);
		vkDestroyBuffer(vulkan->device.device, screen_shot_staging_buffer, nullptr);
		vkFreeMemory(vulkan->device.device, screen_shot_staging_memory, nullptr);
	}

	VkPresentInfoKHR device_queue_present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
	device_queue_present_info.waitSemaphoreCount = 1;
	device_queue_present_info.pWaitSemaphores = &vulkan->syncs.queue_submit_semaphores[vulkan->frame_index];
	device_queue_present_info.swapchainCount = 1;
	device_queue_present_info.pSwapchains = &vulkan->swap_chain.swap_chain;
	device_queue_present_info.pImageIndices = &vulkan->swap_chain_image_index;
	m_vk_assert(vkQueuePresentKHR(vulkan->device.queue, &device_queue_present_info));

	vulkan->frame_index = (vulkan->frame_index + 1) % vulkan_buffering_count;
}
