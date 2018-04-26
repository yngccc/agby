/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "shader_type.cpp"

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
  m_x(PFN_vkCmdUpdateBuffer, vkCmdUpdateBuffer)                                                 \
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
const uint32 vulkan_buffering_count = 2;

struct vulkan_device {
	VkDevice device;
	VkInstance instance;
	VkQueue graphic_queue;
	VkQueue compute_queue;
	VkQueue transfer_queue;
	uint32 graphic_queue_family;
	uint32 compute_queue_family;
	uint32 transfer_queue_family;
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

struct vulkan_cmd_buffers {
	VkCommandBuffer graphic_cmd_buffers[vulkan_buffering_count];
	VkCommandBuffer compute_cmd_buffer;
	VkCommandBuffer transfer_cmd_buffer;
	VkCommandPool graphic_cmd_pool;
	VkCommandPool compute_cmd_pool;
	VkCommandPool transfer_cmd_pool;
};

struct vulkan_syncs {
	VkSemaphore swap_chain_image_acquire_semaphores[vulkan_buffering_count];
	VkSemaphore graphic_queue_submit_semaphores[vulkan_buffering_count];
	VkFence graphic_queue_submit_fences[vulkan_buffering_count];
};

struct vulkan_image {
	VkImage image;
	VkImageView view;
	uint32 width;
	uint32 height;
	uint32 mipmap_count;
	uint32 layer_count;
	VkFormat format;
	uint32 format_block_dimension;
	uint32 format_block_size;
	VkImageLayout layout;
	VkImageAspectFlags aspect_flags;
};

struct vulkan_memory_regions {
	VkDeviceMemory image_region;
	uint64 image_region_size;
	uint64 image_region_capacity;
	vulkan_image *image_region_images;
	uint32 image_region_image_count;
	uint32 image_region_image_capacity;

	VkDeviceMemory vertex_region;
	uint32 vertex_region_size;
	uint32 vertex_region_capacity;
	VkBuffer vertex_region_buffer;

	VkDeviceMemory uniform_regions[vulkan_buffering_count];
	uint32 uniform_region_sizes[vulkan_buffering_count];
	uint32 uniform_region_capacities[vulkan_buffering_count];
	VkBuffer uniform_region_buffers[vulkan_buffering_count];
	uint8 *uniform_region_buffer_ptrs[vulkan_buffering_count];

	VkDeviceMemory dynamic_vertex_regions[vulkan_buffering_count];
	uint32 dynamic_vertex_region_sizes[vulkan_buffering_count];
	uint32 dynamic_vertex_region_capacities[vulkan_buffering_count];
	VkBuffer dynamic_vertex_region_buffers[vulkan_buffering_count];
	uint8 *dynamic_vertex_region_buffer_ptrs[vulkan_buffering_count];

	VkDeviceMemory staging_region;
	uint32 staging_region_capacity;
	VkBuffer staging_region_buffer;
	uint8 *staging_region_buffer_ptr;
};

struct vulkan_samplers {
	VkSampler shadow_map_sampler;
	VkSampler gaussian_blur_sampler;
	VkSampler skybox_cubemap_sampler;
	VkSampler terrain_texture_sampler;
	VkSampler color_framebuffer_sampler;
	VkSampler mipmap_samplers[16];
};

struct vulkan_descriptors {
	VkDescriptorSet uniform_buffers[vulkan_buffering_count];
	VkDescriptorSetLayout uniform_buffer_layout;

	VkDescriptorSet combined_image_samplers_descriptor_set;
	VkDescriptorSetLayout combined_image_samplers_layout;
	uint32 *combined_2d_image_sampler_image_indices;
	uint32 combined_2d_image_sampler_count;
	uint32 combined_2d_image_sampler_capacity;
	uint32 *combined_cube_image_sampler_image_indices;
	uint32 combined_cube_image_sampler_count;
	uint32 combined_cube_image_sampler_capacity;

	VkDescriptorPool pool;
};

struct vulkan_render_passes {
	VkRenderPass shadow_map_render_passes[3];
	VkRenderPass color_render_pass;
	VkRenderPass swap_chain_render_pass;
};

struct vulkan_framebuffer {
	VkFramebuffer framebuffer;
	uint32 width;
	uint32 height;
	VkSampleCountFlagBits sample_count;
	uint32 color_attachment_image_indices[4];
	uint32 color_attachment_count;
	uint32 depth_stencil_attachment_image_index;
	uint32 color_resolve_attachment_image_index;
};

struct vulkan_framebuffers {
	vulkan_framebuffer shadow_map_framebuffers[vulkan_buffering_count];
	vulkan_framebuffer shadow_map_blur_1_framebuffers[vulkan_buffering_count];
	vulkan_framebuffer shadow_map_blur_2_framebuffers[vulkan_buffering_count];
	vulkan_framebuffer color_framebuffers[vulkan_buffering_count];
	VkFramebuffer swap_chain_framebuffers[vulkan_swap_chain_image_count];

	uint32 shadow_map_blur_1_descriptor_indices[vulkan_buffering_count];
	uint32 shadow_map_blur_2_descriptor_indices[vulkan_buffering_count];
	uint32 color_descriptor_indices[vulkan_buffering_count];
};

struct vulkan_pipelines {
	VkPipelineLayout pipeline_layout;
	
	VkPipeline static_model_pipeline;
	VkPipeline static_model_wireframe_pipeline;
	VkPipeline static_model_shadow_map_pipeline;
	VkPipeline shadow_map_gaussian_blur_pipelines[2];
	VkPipeline skybox_pipeline;
	VkPipeline terrain_pipeline;
	VkPipeline imgui_pipeline;
	VkPipeline text_pipeline;
	VkPipeline color_vertex_pipeline;
	VkPipeline swap_chain_pipeline;
};

struct vulkan {
	vulkan_device device;
	vulkan_swap_chain swap_chain;
	vulkan_memory_regions memory_regions;
	vulkan_render_passes render_passes;
	vulkan_framebuffers framebuffers;
	vulkan_cmd_buffers cmd_buffers;
	vulkan_syncs syncs;
	vulkan_samplers samplers;
	vulkan_descriptors descriptors;
	vulkan_pipelines pipelines;

	uint32 frame_index;
	uint32 swap_chain_image_index;

	memory_arena frame_memory_arena;
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

VkBool32 vulkan_debug_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, uint64_t object, size_t location, int32 message_code, const char *layer_prefix, const char *message, void *user_data) {
	int32 ignore_list[] = {6, 61};
	for (auto &code : ignore_list) {
		if (message_code == code) {
			return VK_FALSE;
		}
	}
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		printf("vulkan error code %d: %s\n", message_code, message);
	}
	else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		printf("vulkan warning code %d: %s\n", message_code, message);
	}
	else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		printf("vulkan performance warning code %d: %s\n", message_code, message);
	}
	return VK_FALSE;
};

void initialize_vulkan_device(vulkan *vulkan) {
	bool enable_validation_layer = true;
	bool enable_renderdoc_layer = false;
	bool enable_nsight_layer = true;

	m_memory_arena_undo_allocations_at_scope_exit(&vulkan->frame_memory_arena);
	{ // dynamic load vulkan DLL
		HMODULE vulkan_1_dll = LoadLibraryA("vulkan-1.dll");
		m_assert(vulkan_1_dll);
		#define m_x(shape, name) name = (shape)GetProcAddress(vulkan_1_dll, #name); m_assert(name);
		m_vulkan_procs
		#undef m_x
	}
	{ // query/enable layers/extensions, create instance
		uint32 layer_count = 0;
		m_vk_assert(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
		VkLayerProperties *layer_properties = allocate_memory<VkLayerProperties>(&vulkan->frame_memory_arena, layer_count);
		m_vk_assert(vkEnumerateInstanceLayerProperties(&layer_count, layer_properties));
		printf("Instance Layers:\n");
		for (uint32 i = 0; i < layer_count; i += 1) {
			printf("  %s\n", layer_properties[i].layerName);
		}
		printf("\n");
		uint32 extension_count = 0;
		m_vk_assert(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr));
		VkExtensionProperties *extension_properties = allocate_memory<VkExtensionProperties>(&vulkan->frame_memory_arena, extension_count);
		m_vk_assert(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties));
		printf("Instance Extensions:\n");
		for (uint32 i = 0; i < extension_count; i += 1) {
			printf("  %s\n", extension_properties[i].extensionName);
		}
		printf("\n");

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
		m_vk_assert(vkCreateInstance(&instance_info, nullptr, &vulkan->device.instance));

		#define m_x(shape, name) name = (shape)vkGetInstanceProcAddr(vulkan->device.instance, #name); m_assert(name);
		m_vulkan_instance_procs
		#undef m_x

		for (uint32 i = 0; i < enabled_extension_count; i += 1) {
			if (!strcmp(enabled_extensions[i], "VK_EXT_debug_report")) {
				PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vulkan->device.instance, "vkCreateDebugReportCallbackEXT");
				if (vkCreateDebugReportCallbackEXT) {
					VkDebugReportCallbackCreateInfoEXT debug_callback_info = {VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT};
					debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
					debug_callback_info.pfnCallback = vulkan_debug_callback;
					VkDebugReportCallbackEXT debug_callback = {};
					m_vk_assert(vkCreateDebugReportCallbackEXT(vulkan->device.instance, &debug_callback_info, nullptr, &debug_callback));
				}
				break;
			}
		}
		printf("Enabled Layers: ");
		for (uint32 i = 0; i < enabled_layer_count; i += 1) {
			printf("\"%s\" ", enabled_layers[i]);
		}
		printf("\nEnabled Extensions: ");
		for (uint32 i = 0; i < enabled_extension_count; i += 1) {
			printf("\"%s\" ", enabled_extensions[i]);
		}
		printf("\n\n");
	}
	{ // create physical device
		uint32 physical_device_count = 0;
		m_vk_assert(vkEnumeratePhysicalDevices(vulkan->device.instance, &physical_device_count, nullptr));
		VkPhysicalDevice *physical_devices = allocate_memory<VkPhysicalDevice>(&vulkan->frame_memory_arena, physical_device_count);
		m_vk_assert(vkEnumeratePhysicalDevices(vulkan->device.instance, &physical_device_count, physical_devices));
		VkPhysicalDeviceProperties physical_device_properties = {};
		VkPhysicalDeviceFeatures physical_device_features = {};
		printf("Physical Devices: ");
		for (uint32 i = 0; i < physical_device_count; i += 1) {
			vkGetPhysicalDeviceProperties(physical_devices[i], &physical_device_properties);
			printf("\"%s\" ", physical_device_properties.deviceName);
		}
		printf("\n");
		for (uint32 i = 0; i < physical_device_count; i += 1) {
			VkPhysicalDevice physical_device = physical_devices[i];
			vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);
			if (physical_device_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				continue;
			}
			uint32 physical_device_extensions_count = 0;
			m_vk_assert(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &physical_device_extensions_count, nullptr));
			VkExtensionProperties *physical_device_extensions = allocate_memory<VkExtensionProperties>(&vulkan->frame_memory_arena, physical_device_extensions_count);
			m_vk_assert(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &physical_device_extensions_count, physical_device_extensions));
			VkBool32 support_swap_chain_extension = VK_FALSE;
			for (uint32 i = 0; i < physical_device_extensions_count; i += 1) {
				if (!strcmp(physical_device_extensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
					support_swap_chain_extension = VK_TRUE;
					break;
				}
			}
			vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);
			if (support_swap_chain_extension &&
				  physical_device_features.samplerAnisotropy &&
				  physical_device_features.textureCompressionBC &&
				  physical_device_features.wideLines &&
				  physical_device_features.fillModeNonSolid &&
				  physical_device_features.shaderUniformBufferArrayDynamicIndexing &&
				  physical_device_features.shaderSampledImageArrayDynamicIndexing) {
				printf("Using Physical Device: %s\n", physical_device_properties.deviceName);
				printf("\nPhysical Device Extensions:\n");
				for (uint32 i = 0; i < physical_device_extensions_count; i += 1) {
					printf("  %s\n", physical_device_extensions[i].extensionName);
				}
				printf("\nAPI Version: %d.%d.%d\n", VK_VERSION_MAJOR(physical_device_properties.apiVersion), VK_VERSION_MINOR(physical_device_properties.apiVersion), VK_VERSION_PATCH(physical_device_properties.apiVersion));
				printf("Driver Version: %d.%d.%d\n", VK_VERSION_MAJOR(physical_device_properties.driverVersion), VK_VERSION_MINOR(physical_device_properties.driverVersion), VK_VERSION_PATCH(physical_device_properties.driverVersion));
				vulkan->device.physical_device = physical_device;
				vulkan->device.physical_device_properties = physical_device_properties;
				vkGetPhysicalDeviceMemoryProperties(physical_device, &vulkan->device.physical_device_memory_properties);
				break;
			}
		}
		m_assert(vulkan->device.physical_device);
	}
	{ // create device, graphic/compute/transfer queues
		uint32 queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(vulkan->device.physical_device, &queue_family_count, nullptr);
		VkQueueFamilyProperties *queue_family_properties = allocate_memory<VkQueueFamilyProperties>(&vulkan->frame_memory_arena, queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(vulkan->device.physical_device, &queue_family_count, queue_family_properties);
		for (uint32 i = 0; i < queue_family_count; i += 1) {
			printf("Queue familiy %d properties: %d queues, ", i, queue_family_properties[i].queueCount);
			if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				printf("\"graphic\" ");
			}
			if (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
				printf("\"transfer\" ");
			}
			if (queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
				printf("\"compute\" ");
			}
			if (queue_family_properties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
				printf("\"sparse binding\" ");
			}
			printf("\n");
		}
		vulkan->device.graphic_queue_family = UINT32_MAX;
		vulkan->device.compute_queue_family = UINT32_MAX;
		vulkan->device.transfer_queue_family = UINT32_MAX;
		for (uint32 i = 0; i < queue_family_count; i += 1) {
			VkQueueFamilyProperties *properties = &queue_family_properties[i];
			if (properties->queueFlags & VK_QUEUE_GRAPHICS_BIT && properties->queueCount > 0) {
				vulkan->device.graphic_queue_family = i;
				properties->queueCount -= 1;
			}
			if (properties->queueFlags & VK_QUEUE_COMPUTE_BIT && properties->queueCount > 0) {
				vulkan->device.compute_queue_family = i;
				properties->queueCount -= 1;
			}
		}
		for (uint32 i = 0; i < queue_family_count; i += 1) {
			VkQueueFamilyProperties *properties = &queue_family_properties[i];
			if (properties->queueFlags == VK_QUEUE_TRANSFER_BIT && properties->queueCount > 0) {
				vulkan->device.transfer_queue_family = i;
				properties->queueCount -= 1;
				break;
			}
		}
		if (vulkan->device.transfer_queue_family == UINT32_MAX) {
			for (uint32 i = 0; i < queue_family_count; i += 1) {
				VkQueueFamilyProperties *properties = &queue_family_properties[i];
				if (properties->queueFlags & VK_QUEUE_TRANSFER_BIT && properties->queueCount > 0) {
					vulkan->device.transfer_queue_family = i;
					properties->queueCount -= 1;
					break;
				}
			}
		}
		m_assert(vulkan->device.graphic_queue_family != UINT32_MAX);
		m_assert(vulkan->device.compute_queue_family != UINT32_MAX);
		m_assert(vulkan->device.transfer_queue_family != UINT32_MAX);

		float queue_priorities[3] = {1, 1, 1};
		uint32 queue_info_count = 1;
		uint32 graphic_queue_index = 0;
		uint32 compute_queue_index = 0;
		uint32 transfer_queue_index = 0;
		VkDeviceQueueCreateInfo queue_infos[3] = {};
		queue_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_infos[0].queueFamilyIndex = vulkan->device.graphic_queue_family;
		queue_infos[0].queueCount = 1;
		queue_infos[0].pQueuePriorities = queue_priorities;
		if (vulkan->device.compute_queue_family == vulkan->device.graphic_queue_family) {
			queue_infos[0].queueCount += 1;
			compute_queue_index = 1;
		}
		else {
			queue_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_infos[1].queueFamilyIndex = vulkan->device.compute_queue_family;
			queue_infos[1].queueCount = 1;
			queue_infos[1].pQueuePriorities = queue_priorities;
			queue_info_count += 1;
		}
		if (vulkan->device.transfer_queue_family == vulkan->device.graphic_queue_family) {
			queue_infos[0].queueCount += 1;
			transfer_queue_index = compute_queue_index + 1;
		}
		else if (vulkan->device.transfer_queue_family == vulkan->device.compute_queue_family) {
			queue_infos[1].queueCount += 1;
			transfer_queue_index = compute_queue_index + 1;
		}
		else {
			queue_infos[queue_info_count].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_infos[queue_info_count].queueFamilyIndex = vulkan->device.transfer_queue_family;
			queue_infos[queue_info_count].queueCount = 1;
			queue_infos[queue_info_count].pQueuePriorities = queue_priorities;
			queue_info_count += 1;
		}

		const char *device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

		VkPhysicalDeviceFeatures device_features = {};
		device_features.samplerAnisotropy = VK_TRUE;
		device_features.textureCompressionBC = VK_TRUE;
		device_features.wideLines = VK_TRUE;
		device_features.fillModeNonSolid = VK_TRUE;
		device_features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
		device_features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;

		VkDeviceCreateInfo device_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
		device_info.queueCreateInfoCount = queue_info_count;
		device_info.pQueueCreateInfos = queue_infos;
		device_info.enabledExtensionCount = m_countof(device_extensions);
		device_info.ppEnabledExtensionNames = device_extensions;
		device_info.pEnabledFeatures = &device_features;
		m_vk_assert(vkCreateDevice(vulkan->device.physical_device, &device_info, nullptr, &vulkan->device.device));
		#define m_x(shape, name) name = (shape)vkGetDeviceProcAddr(vulkan->device.device, #name); m_assert(name);
		m_vulkan_device_procs
		#undef m_x
		vkGetDeviceQueue(vulkan->device.device, vulkan->device.graphic_queue_family, graphic_queue_index, &vulkan->device.graphic_queue);
		vkGetDeviceQueue(vulkan->device.device, vulkan->device.compute_queue_family, compute_queue_index, &vulkan->device.compute_queue);
		vkGetDeviceQueue(vulkan->device.device, vulkan->device.transfer_queue_family, transfer_queue_index, &vulkan->device.transfer_queue);
	}
}

void initialize_vulkan_swap_chain(vulkan *vulkan, window window, bool vsync_on) {
	struct memory_arena memory_arena = {};
	string info_str = {};
	memory_arena.capacity = m_megabytes(1);
	memory_arena.memory = allocate_virtual_memory(memory_arena.capacity);
	m_assert(memory_arena.memory);
	info_str.capacity = memory_arena.capacity / 2;
	info_str.buf = allocate_memory<char>(&memory_arena, info_str.capacity);
	m_scope_exit(
		printf("%s\n", info_str.buf);
	  free_virtual_memory(memory_arena.memory);
	);
	{ // create surface
		VkWin32SurfaceCreateInfoKHR surface_create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
		surface_create_info.hinstance = GetModuleHandle(nullptr);
		surface_create_info.hwnd = window.handle;
		m_vk_assert(vkCreateWin32SurfaceKHR(vulkan->device.instance, &surface_create_info, nullptr, &vulkan->swap_chain.surface));
		VkBool32 physical_device_surface_support = VK_FALSE;
		m_vk_assert(vkGetPhysicalDeviceSurfaceSupportKHR(vulkan->device.physical_device, vulkan->device.graphic_queue_family, vulkan->swap_chain.surface, &physical_device_surface_support));
		m_assert(physical_device_surface_support);
	}
	{ // check surface capabilities
		m_vk_assert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &vulkan->swap_chain.surface_capabilities));
		m_assert(vulkan->swap_chain.surface_capabilities.maxImageCount >= vulkan_swap_chain_image_count);
		m_assert((vulkan->swap_chain.surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) && (vulkan->swap_chain.surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT));
		uint32 surface_image_formats_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_image_formats_count, nullptr);
		m_assert(surface_image_formats_count > 0);
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
			m_assert(support_bgra_unorm);
		}
	}
	{ // set present mode
		uint32 surface_present_modes_count = 0;
		m_vk_assert(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_present_modes_count, nullptr));
		VkPresentModeKHR *surface_present_modes = allocate_memory<VkPresentModeKHR>(&memory_arena, surface_present_modes_count);
		m_vk_assert(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->device.physical_device, vulkan->swap_chain.surface, &surface_present_modes_count, surface_present_modes));
		printf("Swapchain Present Modes: ");
		const char *present_mode_strs[4] = {"Immediate", "Mailbox", "FIFO", "FIFO_Relaxed"};
		for (uint32 i = 0; i < surface_present_modes_count; i += 1) {
			printf("\"%s\" ", present_mode_strs[surface_present_modes[i]]);
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
		printf("\nUsing: \"%s\"\n", present_mode_strs[vulkan->swap_chain.present_mode]);
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
		printf("\nSwapchain Image Size: %dx%d\n", vulkan->swap_chain.image_width, vulkan->swap_chain.image_height);
	}
}

void resize_vulkan_swap_chain(vulkan *vulkan, uint32 new_width, uint32 new_height) {
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

void initialize_vulkan_cmd_buffers(vulkan *vulkan) {
	VkCommandPoolCreateInfo cmd_pool_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
	cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	cmd_pool_info.queueFamilyIndex = vulkan->device.graphic_queue_family;
	m_vk_assert(vkCreateCommandPool(vulkan->device.device, &cmd_pool_info, nullptr, &vulkan->cmd_buffers.graphic_cmd_pool));
	cmd_pool_info.queueFamilyIndex = vulkan->device.compute_queue_family;
	m_vk_assert(vkCreateCommandPool(vulkan->device.device, &cmd_pool_info, nullptr, &vulkan->cmd_buffers.compute_cmd_pool));
	cmd_pool_info.queueFamilyIndex = vulkan->device.transfer_queue_family;
	m_vk_assert(vkCreateCommandPool(vulkan->device.device, &cmd_pool_info, nullptr, &vulkan->cmd_buffers.transfer_cmd_pool));

	VkCommandBufferAllocateInfo cmd_buffer_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
	cmd_buffer_info.commandPool = vulkan->cmd_buffers.graphic_cmd_pool;
	cmd_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd_buffer_info.commandBufferCount = vulkan_buffering_count;
	m_vk_assert(vkAllocateCommandBuffers(vulkan->device.device, &cmd_buffer_info, vulkan->cmd_buffers.graphic_cmd_buffers));

	cmd_buffer_info.commandPool = vulkan->cmd_buffers.compute_cmd_pool;
	cmd_buffer_info.commandBufferCount = 1;
	m_vk_assert(vkAllocateCommandBuffers(vulkan->device.device, &cmd_buffer_info, &vulkan->cmd_buffers.compute_cmd_buffer));

	cmd_buffer_info.commandPool = vulkan->cmd_buffers.transfer_cmd_pool;
	cmd_buffer_info.commandBufferCount = 1;
	m_vk_assert(vkAllocateCommandBuffers(vulkan->device.device, &cmd_buffer_info, &vulkan->cmd_buffers.transfer_cmd_buffer));
}

void initialize_vulkan_syncs(vulkan *vulkan) {
	VkSemaphoreCreateInfo semaphore_create_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
	for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
		m_vk_assert(vkCreateSemaphore(vulkan->device.device, &semaphore_create_info, nullptr, &vulkan->syncs.swap_chain_image_acquire_semaphores[i]));
		m_vk_assert(vkCreateSemaphore(vulkan->device.device, &semaphore_create_info, nullptr, &vulkan->syncs.graphic_queue_submit_semaphores[i]));
		VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
		fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_vk_assert(vkCreateFence(vulkan->device.device, &fence_create_info, nullptr, &vulkan->syncs.graphic_queue_submit_fences[i]));
	}
}

void initialize_vulkan_memory_regions(vulkan *vulkan) {
	auto allocate_memory = [vulkan](uint64 size, uint32 type_bits, uint32 flags) {
		VkDeviceMemory device_memory = nullptr;
		const auto &properties = vulkan->device.physical_device_memory_properties;
		for (uint32 i = 0; i < properties.memoryTypeCount; i += 1) {
			if (type_bits & (1 << i) && properties.memoryTypes[i].propertyFlags == flags) {
				VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
				memory_allocate_info.allocationSize = size;
				memory_allocate_info.memoryTypeIndex = i;
				VkResult result = vkAllocateMemory(vulkan->device.device, &memory_allocate_info, nullptr, &device_memory);
				if (result == VK_SUCCESS) {
					break;
				}
			}
		}
		return device_memory;
	};
	vulkan_memory_regions *regions = &vulkan->memory_regions;
	{ // image
		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_B8G8R8A8_SRGB;
		image_info.extent = {1024, 1024, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImage image = {};
		m_vk_assert(vkCreateImage(vulkan->device.device, &image_info, nullptr, &image));
		VkMemoryRequirements memory_requirements = {};
		vkGetImageMemoryRequirements(vulkan->device.device, image, &memory_requirements);
		vkDestroyImage(vulkan->device.device, image, nullptr);

		regions->image_region_size = 0;
		regions->image_region_capacity = m_megabytes(512);
		regions->image_region = allocate_memory(regions->image_region_capacity, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_assert(regions->image_region);

		regions->image_region_image_count = 0;
		regions->image_region_image_capacity = 1024;
		regions->image_region_images = new vulkan_image[regions->image_region_image_capacity]();
	}
	{ // vertex
		VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size = m_megabytes(32);
		buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &regions->vertex_region_buffer));
		VkMemoryRequirements memory_requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, regions->vertex_region_buffer, &memory_requirements);

		regions->vertex_region_size = 0;
		regions->vertex_region_capacity = (uint32)buffer_info.size;
		regions->vertex_region = allocate_memory(memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_assert(regions->vertex_region);
		m_vk_assert(vkBindBufferMemory(vulkan->device.device, regions->vertex_region_buffer, regions->vertex_region, 0));
	}
	{ // uniform
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
			buffer_info.size = vulkan->device.physical_device_properties.limits.maxUniformBufferRange;
			buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &regions->uniform_region_buffers[i]));
			VkMemoryRequirements memory_requirements = {};
			vkGetBufferMemoryRequirements(vulkan->device.device, regions->uniform_region_buffers[i], &memory_requirements);

			regions->uniform_region_sizes[i] = 0;
			regions->uniform_region_capacities[i] = (uint32)buffer_info.size;
			regions->uniform_regions[i] = allocate_memory(memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			m_assert(regions->uniform_regions[i]);
			m_vk_assert(vkBindBufferMemory(vulkan->device.device, regions->uniform_region_buffers[i], regions->uniform_regions[i], 0));
			m_vk_assert(vkMapMemory(vulkan->device.device, regions->uniform_regions[i], 0, VK_WHOLE_SIZE, 0, (void **)&regions->uniform_region_buffer_ptrs[i]));
		}
	}
	{ // dynamic vertex
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
			buffer_info.size = m_megabytes(8);
			buffer_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &regions->dynamic_vertex_region_buffers[i]));
			VkMemoryRequirements memory_requirements = {};
			vkGetBufferMemoryRequirements(vulkan->device.device, regions->dynamic_vertex_region_buffers[i], &memory_requirements);

			regions->dynamic_vertex_region_sizes[i] = 0;
			regions->dynamic_vertex_region_capacities[i] = (uint32)buffer_info.size;
			regions->dynamic_vertex_regions[i] = allocate_memory(memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			m_assert(regions->dynamic_vertex_regions[i]);
			m_vk_assert(vkBindBufferMemory(vulkan->device.device, regions->dynamic_vertex_region_buffers[i], regions->dynamic_vertex_regions[i], 0));
			m_vk_assert(vkMapMemory(vulkan->device.device, regions->dynamic_vertex_regions[i], 0, VK_WHOLE_SIZE, 0, (void **)&regions->dynamic_vertex_region_buffer_ptrs[i]));
		}
	}
	{ // staging
		VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size = m_megabytes(32);
		buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &regions->staging_region_buffer));
		VkMemoryRequirements memory_requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, regions->staging_region_buffer, &memory_requirements);

		regions->staging_region_capacity = (uint32)buffer_info.size;
		regions->staging_region = allocate_memory(memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_assert(regions->staging_region);
		m_vk_assert(vkBindBufferMemory(vulkan->device.device, regions->staging_region_buffer, regions->staging_region, 0));
		m_vk_assert(vkMapMemory(vulkan->device.device, regions->staging_region, 0, VK_WHOLE_SIZE, 0, (void **)&regions->staging_region_buffer_ptr));
	}
}

uint32 append_vulkan_vertex_region(vulkan *vulkan, const void *data, uint32 data_size, uint32 data_alignment) {
	vulkan_memory_regions &regions = vulkan->memory_regions;

	round_up(&regions.vertex_region_size, data_alignment);
	uint32 region_offset = regions.vertex_region_size;
	regions.vertex_region_size += data_size;
	m_assert(regions.vertex_region_size < regions.vertex_region_capacity);

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	uint32 data_size_remain = data_size;
	uint32 current_region_offset = region_offset;
	while (data_size_remain > 0) {
		uint32 copy_size = min(data_size_remain, regions.staging_region_capacity);
		memcpy(regions.staging_region_buffer_ptr, data, copy_size);

		vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);
		VkBufferCopy buffer_copy = {0, current_region_offset, copy_size};
		vkCmdCopyBuffer(cmd_buffer, regions.staging_region_buffer, regions.vertex_region_buffer, 1, &buffer_copy);
		vkEndCommandBuffer(cmd_buffer);
		VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &cmd_buffer;
		vkQueueSubmit(vulkan->device.transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
		vkQueueWaitIdle(vulkan->device.transfer_queue);

		data_size_remain -= copy_size;
		data = (uint8 *)data + copy_size;
		current_region_offset += copy_size;
	}
	return region_offset;
}

uint32 append_vulkan_image_region(vulkan *vulkan, VkImageCreateInfo image_info, VkImageViewCreateInfo image_view_info, uint8 *image_data, uint32 image_data_size, uint32 format_block_dimension, uint32 format_block_size) {
	vulkan_memory_regions &regions = vulkan->memory_regions;
	m_assert(regions.image_region_image_count < regions.image_region_image_capacity);

	VkImageLayout layout = image_info.initialLayout;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	uint32 image_index = regions.image_region_image_count;
	vulkan_image &image = regions.image_region_images[regions.image_region_image_count++];
	m_vk_assert(vkCreateImage(vulkan->device.device, &image_info, nullptr, &image.image));

	VkMemoryRequirements memory_requirements = {};
	vkGetImageMemoryRequirements(vulkan->device.device, image.image, &memory_requirements);
	round_up(&regions.image_region_size, memory_requirements.alignment);
  uint64 region_offset = regions.image_region_size;
  regions.image_region_size += memory_requirements.size;
  m_assert(regions.image_region_size < regions.image_region_capacity);

	m_vk_assert(vkBindImageMemory(vulkan->device.device, image.image, regions.image_region, region_offset));
	image_view_info.image = image.image;
	m_vk_assert(vkCreateImageView(vulkan->device.device, &image_view_info, nullptr, &image.view));

	image.width = image_info.extent.width;
	image.height = image_info.extent.height;
	image.mipmap_count = image_info.mipLevels;
	image.layer_count = image_info.arrayLayers;
	image.format = image_info.format;
	image.format_block_dimension = format_block_dimension;
	image.format_block_size = format_block_size;
	image.layout = layout;
	image.aspect_flags = image_view_info.subresourceRange.aspectMask;

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);

	if (image_data && image_data_size > 0) {
		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.image = image.image;
		image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.subresourceRange.aspectMask = image_view_info.subresourceRange.aspectMask;
		image_memory_barrier.subresourceRange.levelCount = image.mipmap_count;
		image_memory_barrier.subresourceRange.layerCount = image.layer_count;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

		m_assert(image_data_size < regions.staging_region_capacity);
		memcpy(regions.staging_region_buffer_ptr, image_data, image_data_size);

		uint32 staging_buffer_offset = 0;
		uint32 mipmap_width = image.width;
		uint32 mipmap_height = image.height;
		for (uint32 i = 0; i < image.mipmap_count; i += 1) {
			VkBufferImageCopy buffer_image_copy = {};
			buffer_image_copy.bufferOffset = staging_buffer_offset;
			buffer_image_copy.imageSubresource.aspectMask = image_view_info.subresourceRange.aspectMask;
			buffer_image_copy.imageSubresource.mipLevel = i;
			buffer_image_copy.imageSubresource.layerCount = image.layer_count;
			buffer_image_copy.imageExtent = {mipmap_width, mipmap_height, 1};
			vkCmdCopyBufferToImage(cmd_buffer, regions.staging_region_buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);
			uint32 block_count = (mipmap_width * mipmap_height) / (image.format_block_dimension * image.format_block_dimension);
			staging_buffer_offset += image.format_block_size * block_count * image.layer_count;
			mipmap_width = max(mipmap_width / 2, image.format_block_dimension);
			mipmap_height = max(mipmap_height / 2, image.format_block_dimension);
		}

		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = 0;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.newLayout = layout;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
	} else {
		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_memory_barrier.newLayout = layout;
		image_memory_barrier.image = image.image;
		image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.subresourceRange.aspectMask = image_view_info.subresourceRange.aspectMask;
		image_memory_barrier.subresourceRange.levelCount = image.mipmap_count;
		image_memory_barrier.subresourceRange.layerCount = image.layer_count;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
	}

	vkEndCommandBuffer(cmd_buffer);
	VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;
	vkQueueSubmit(vulkan->device.transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkan->device.transfer_queue);

	return image_index;
}

void retrieve_vulkan_image_region(vulkan *vulkan, uint32 image_index, uint8* image_data, uint32 image_data_size) {
	const vulkan_image &image = vulkan->memory_regions.image_region_images[image_index];
	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(vulkan->device.device, image.image, &memory_requirements);
	m_assert(memory_requirements.size == image_data_size);
	m_assert(memory_requirements.size <= vulkan->memory_regions.staging_region_capacity);

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);

	VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	image_memory_barrier.oldLayout = image.layout;
	image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	image_memory_barrier.image = image.image;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.subresourceRange.aspectMask = image.aspect_flags;
	image_memory_barrier.subresourceRange.levelCount = image.mipmap_count;
	image_memory_barrier.subresourceRange.layerCount = image.layer_count;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	uint32 staging_buffer_offset = 0;
	uint32 mipmap_width = image.width;
	uint32 mipmap_height = image.height;
	for (uint32 i = 0; i < image.mipmap_count; i += 1) {
		VkBufferImageCopy buffer_image_copy = {};
		buffer_image_copy.bufferOffset = staging_buffer_offset;
		buffer_image_copy.imageSubresource.aspectMask = image.aspect_flags;
		buffer_image_copy.imageSubresource.mipLevel = i;
		buffer_image_copy.imageSubresource.layerCount = image.layer_count;
		buffer_image_copy.imageExtent = {mipmap_width, mipmap_height, 1};
		vkCmdCopyImageToBuffer(cmd_buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vulkan->memory_regions.staging_region_buffer, 1, &buffer_image_copy);
		uint32 block_count = (mipmap_width * mipmap_height) / (image.format_block_dimension * image.format_block_dimension);
		staging_buffer_offset += image.format_block_size * block_count * image.layer_count;
		mipmap_width = max(mipmap_width / 2, image.format_block_dimension);
		mipmap_height = max(mipmap_height / 2, image.format_block_dimension);
	}

	image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	image_memory_barrier.dstAccessMask = 0;
	image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	image_memory_barrier.newLayout = image.layout;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	vkEndCommandBuffer(cmd_buffer);
	VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;
	vkQueueSubmit(vulkan->device.transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkan->device.transfer_queue);

	memcpy(image_data, vulkan->memory_regions.staging_region_buffer_ptr, image_data_size);
}

void update_vulkan_image_region(vulkan *vulkan, uint32 image_index, uint8 *image_data, uint32 image_data_size) {
	const vulkan_image &image = vulkan->memory_regions.image_region_images[image_index];
	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(vulkan->device.device, image.image, &memory_requirements);
	m_assert(memory_requirements.size == image_data_size);
	m_assert(memory_requirements.size <= vulkan->memory_regions.staging_region_capacity);

	memcpy(vulkan->memory_regions.staging_region_buffer_ptr, image_data, image_data_size);

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);

	VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.oldLayout = image.layout;
	image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.image = image.image;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.subresourceRange.aspectMask = image.aspect_flags;
	image_memory_barrier.subresourceRange.levelCount = image.mipmap_count;
	image_memory_barrier.subresourceRange.layerCount = image.layer_count;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	uint32 staging_buffer_offset = 0;
	uint32 mipmap_width = image.width;
	uint32 mipmap_height = image.height;
	for (uint32 i = 0; i < image.mipmap_count; i += 1) {
		VkBufferImageCopy buffer_image_copy = {};
		buffer_image_copy.bufferOffset = staging_buffer_offset;
		buffer_image_copy.imageSubresource.aspectMask = image.aspect_flags;
		buffer_image_copy.imageSubresource.mipLevel = i;
		buffer_image_copy.imageSubresource.layerCount = image.layer_count;
		buffer_image_copy.imageExtent = {mipmap_width, mipmap_height, 1};
		vkCmdCopyBufferToImage(cmd_buffer, vulkan->memory_regions.staging_region_buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);
		uint32 block_count = (mipmap_width * mipmap_height) / (image.format_block_dimension * image.format_block_dimension);
		staging_buffer_offset += image.format_block_size * block_count * image.layer_count;
		mipmap_width = max(mipmap_width / 2, image.format_block_dimension);
		mipmap_height = max(mipmap_height / 2, image.format_block_dimension);
	}

	image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.newLayout = image.layout;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	vkEndCommandBuffer(cmd_buffer);
	VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;
	vkQueueSubmit(vulkan->device.transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkan->device.transfer_queue);
}

uint32 append_vulkan_uniform_region(vulkan *vulkan, const void *data, uint32 data_size) {
	uint32 &size = vulkan->memory_regions.uniform_region_sizes[vulkan->frame_index];
	uint8 *ptr = vulkan->memory_regions.uniform_region_buffer_ptrs[vulkan->frame_index];
	uint32 capacity = vulkan->memory_regions.uniform_region_capacities[vulkan->frame_index];
	round_up(&size, 64u);
	m_assert(size + data_size < capacity);
	memcpy(ptr + size, data, data_size);
	uint32 offset = size;
	size += data_size;
	return offset / 64;;
}

uint32 append_vulkan_dynamic_vertex_region(vulkan *vulkan, const void *data, uint32 data_size, uint32 alignment) {
	uint32 &size = vulkan->memory_regions.dynamic_vertex_region_sizes[vulkan->frame_index];
	uint8 *ptr = vulkan->memory_regions.dynamic_vertex_region_buffer_ptrs[vulkan->frame_index];
	uint32 capacity = vulkan->memory_regions.dynamic_vertex_region_capacities[vulkan->frame_index];
	round_up(&size, alignment);
	m_assert(size + data_size < capacity);
	memcpy(ptr + size, data, data_size);
	uint32 offset = size;
	size += data_size;
	return offset;
}

void initialize_vulkan_samplers(vulkan *vulkan) {
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
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.skybox_cubemap_sampler));
	}
	{ // terrain texture sampler
		VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
		sampler_create_info.magFilter = VK_FILTER_LINEAR;
		sampler_create_info.minFilter = VK_FILTER_LINEAR;
		sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.terrain_texture_sampler));
	}
	{ // color framebuffer
		VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
		sampler_create_info.magFilter = VK_FILTER_LINEAR;
		sampler_create_info.minFilter = VK_FILTER_LINEAR;
		sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.color_framebuffer_sampler));
	}	
	{ // mipmap images
		for (uint32 i = 0; i < m_countof(vulkan->samplers.mipmap_samplers); i += 1) {
			VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
			sampler_create_info.magFilter = VK_FILTER_LINEAR;
			sampler_create_info.minFilter = VK_FILTER_LINEAR;
			sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			sampler_create_info.maxLod = (float)i;
			m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.mipmap_samplers[i]));
		}
	}
}

void initialize_vulkan_descriptors(vulkan *vulkan) {
	{ // pool
		VkDescriptorPoolCreateInfo descriptor_pool_create_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
		// descriptor_pool_create_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptor_pool_create_info.maxSets = 1 + vulkan_buffering_count;
		VkDescriptorPoolSize descriptor_pool_sizes[2] = {
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vulkan_buffering_count},
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024 + 128}
		};
		descriptor_pool_create_info.poolSizeCount = m_countof(descriptor_pool_sizes);
		descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes;
		m_vk_assert(vkCreateDescriptorPool(vulkan->device.device, &descriptor_pool_create_info, nullptr, &vulkan->descriptors.pool));
	}
	{ // uniform buffer set
		VkDescriptorSetLayoutBinding layout_binding = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
		VkDescriptorSetLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
		layout_info.bindingCount = 1;
		layout_info.pBindings = &layout_binding;
		m_vk_assert(vkCreateDescriptorSetLayout(vulkan->device.device, &layout_info, nullptr, &vulkan->descriptors.uniform_buffer_layout));

		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			VkDescriptorSetAllocateInfo set_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
			set_info.descriptorPool = vulkan->descriptors.pool;
			set_info.descriptorSetCount = 1;
			set_info.pSetLayouts = &vulkan->descriptors.uniform_buffer_layout;
			m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &set_info, &vulkan->descriptors.uniform_buffers[i]));

			VkDescriptorBufferInfo buffer_info = {vulkan->memory_regions.uniform_region_buffers[i], 0, vulkan->memory_regions.uniform_region_capacities[i]};
			VkWriteDescriptorSet write_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
			write_set.dstSet = vulkan->descriptors.uniform_buffers[i];
			write_set.dstBinding = 0;
			write_set.descriptorCount = 1;
			write_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write_set.pBufferInfo = &buffer_info;
			vkUpdateDescriptorSets(vulkan->device.device, 1, &write_set, 0, nullptr);
		}
	}
	{ // combined image samplers set
		vulkan->descriptors.combined_2d_image_sampler_count = 0;
		vulkan->descriptors.combined_2d_image_sampler_capacity = 1024;
		vulkan->descriptors.combined_2d_image_sampler_image_indices = new uint32[1024]();
		vulkan->descriptors.combined_cube_image_sampler_count = 0;
		vulkan->descriptors.combined_cube_image_sampler_capacity = 128;
		vulkan->descriptors.combined_cube_image_sampler_image_indices = new uint32[128]();
		VkDescriptorSetLayoutBinding layout_bindings[2] = {
			{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 128, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr}
		};
		VkDescriptorSetLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
		layout_info.bindingCount = m_countof(layout_bindings);
		layout_info.pBindings = layout_bindings;
		m_vk_assert(vkCreateDescriptorSetLayout(vulkan->device.device, &layout_info, nullptr, &vulkan->descriptors.combined_image_samplers_layout));

		VkDescriptorSetAllocateInfo set_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
		set_info.descriptorPool = vulkan->descriptors.pool;
		set_info.descriptorSetCount = 1;
		set_info.pSetLayouts = &vulkan->descriptors.combined_image_samplers_layout;
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &set_info, &vulkan->descriptors.combined_image_samplers_descriptor_set));
	}
}

uint32 append_vulkan_combined_2d_image_samplers(vulkan *vulkan, uint32 image_index, VkSampler sampler) {
	m_assert(vulkan->descriptors.combined_2d_image_sampler_count < vulkan->descriptors.combined_2d_image_sampler_capacity);
	VkDescriptorImageInfo descriptor_image_info = {sampler, vulkan->memory_regions.image_region_images[image_index].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = vulkan->descriptors.combined_image_samplers_descriptor_set;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.dstArrayElement = vulkan->descriptors.combined_2d_image_sampler_count;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	for (auto &fence : vulkan->syncs.graphic_queue_submit_fences) {
		vkWaitForFences(vulkan->device.device, 1, &fence, VK_TRUE, UINT64_MAX);
	}
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	vulkan->descriptors.combined_2d_image_sampler_image_indices[vulkan->descriptors.combined_2d_image_sampler_count] = image_index;
	return vulkan->descriptors.combined_2d_image_sampler_count++;
}

void update_vulkan_combined_2d_image_samplers(vulkan *vulkan, uint32 index, uint32 image_index, VkSampler sampler) {
	m_assert(index < vulkan->descriptors.combined_2d_image_sampler_count);
	VkDescriptorImageInfo descriptor_image_info = {sampler, vulkan->memory_regions.image_region_images[image_index].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = vulkan->descriptors.combined_image_samplers_descriptor_set;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.dstArrayElement = index;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	for (auto &fence : vulkan->syncs.graphic_queue_submit_fences) {
		vkWaitForFences(vulkan->device.device, 1, &fence, VK_TRUE, UINT64_MAX);
	}
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	vulkan->descriptors.combined_2d_image_sampler_image_indices[index] = image_index;
}

uint32 append_vulkan_combined_cube_image_samplers(vulkan *vulkan, uint32 image_index, VkSampler sampler) {
	m_assert(vulkan->descriptors.combined_cube_image_sampler_count < vulkan->descriptors.combined_cube_image_sampler_capacity);
	VkDescriptorImageInfo descriptor_image_info = {sampler, vulkan->memory_regions.image_region_images[image_index].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = vulkan->descriptors.combined_image_samplers_descriptor_set;
	write_descriptor_set.dstBinding = 1;
	write_descriptor_set.dstArrayElement = vulkan->descriptors.combined_cube_image_sampler_count;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	for (auto &fence : vulkan->syncs.graphic_queue_submit_fences) {
		vkWaitForFences(vulkan->device.device, 1, &fence, VK_TRUE, UINT64_MAX);
	}
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	vulkan->descriptors.combined_cube_image_sampler_image_indices[vulkan->descriptors.combined_cube_image_sampler_count] = image_index;
	return vulkan->descriptors.combined_cube_image_sampler_count++;
}

void initialize_vulkan_render_passes(vulkan *vulkan, VkSampleCountFlagBits sample_count) {
	{ // shadow map
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
	{ // shadow map gaussian blur
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
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
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
		m_vk_assert(vkCreateRenderPass(vulkan->device.device, &create_info, nullptr, &vulkan->render_passes.color_render_pass));
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

void initialize_vulkan_framebuffers(vulkan *vulkan, VkSampleCountFlagBits sample_count) {
	{ // shadow map, blur1, blur2
		const uint32 shadow_map_size = 1024;
		for (uint32 i = 0; i < m_countof(vulkan->framebuffers.shadow_map_framebuffers); i += 1) {
			vulkan_framebuffer &framebuffer = vulkan->framebuffers.shadow_map_framebuffers[i];
			framebuffer = {};
			framebuffer.width = shadow_map_size;
			framebuffer.height = shadow_map_size;
			framebuffer.sample_count = VK_SAMPLE_COUNT_1_BIT;
			framebuffer.color_attachment_count = 1;
			framebuffer.color_resolve_attachment_image_index = UINT32_MAX;

			VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = VK_FORMAT_R32G32_SFLOAT;
			image_info.extent = {shadow_map_size, shadow_map_size, 1};
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			image_info.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = image_info.format;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.layerCount = 1;
			framebuffer.color_attachment_image_indices[0] = append_vulkan_image_region(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);

			image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = VK_FORMAT_D32_SFLOAT;
			image_info.extent = {shadow_map_size, shadow_map_size, 1};
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			image_info.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = image_info.format;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.layerCount = 1;
			framebuffer.depth_stencil_attachment_image_index = append_vulkan_image_region(vulkan, image_info, image_view_info, nullptr, 0, 1, 4);

			vulkan_image &color_image = vulkan->memory_regions.image_region_images[framebuffer.color_attachment_image_indices[0]];
			vulkan_image &depth_stencil_image = vulkan->memory_regions.image_region_images[framebuffer.depth_stencil_attachment_image_index];
			VkImageView image_views[2] = {color_image.view, depth_stencil_image.view};
			VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
			framebuffer_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
			framebuffer_info.attachmentCount = m_countof(image_views);
			framebuffer_info.pAttachments = image_views;
			framebuffer_info.width = shadow_map_size;
			framebuffer_info.height = shadow_map_size;
			framebuffer_info.layers = 1;
			m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer.framebuffer));
		}
		for (uint32 i = 0; i < m_countof(vulkan->framebuffers.shadow_map_blur_1_framebuffers); i += 1) {
			vulkan_framebuffer &framebuffer = vulkan->framebuffers.shadow_map_blur_1_framebuffers[i];
			framebuffer = {};
			framebuffer.width = shadow_map_size;
			framebuffer.height = shadow_map_size;
			framebuffer.sample_count = VK_SAMPLE_COUNT_1_BIT;
			framebuffer.color_attachment_count = 1;
			framebuffer.depth_stencil_attachment_image_index = UINT32_MAX;
			framebuffer.color_resolve_attachment_image_index = UINT32_MAX;

			VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = VK_FORMAT_R32G32_SFLOAT;
			image_info.extent = {shadow_map_size, shadow_map_size, 1};
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			image_info.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = image_info.format;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.layerCount = 1;
			framebuffer.color_attachment_image_indices[0] = append_vulkan_image_region(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);

			vulkan_image &color_image = vulkan->memory_regions.image_region_images[framebuffer.color_attachment_image_indices[0]];
			VkImageView image_views[1] = {color_image.view};
			VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
			framebuffer_info.renderPass = vulkan->render_passes.shadow_map_render_passes[1];
			framebuffer_info.attachmentCount = m_countof(image_views);
			framebuffer_info.pAttachments = image_views;
			framebuffer_info.width = shadow_map_size;
			framebuffer_info.height = shadow_map_size;
			framebuffer_info.layers = 1;
			m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer.framebuffer));
		}
		for (uint32 i = 0; i < m_countof(vulkan->framebuffers.shadow_map_blur_2_framebuffers); i += 1) {
			vulkan_framebuffer &framebuffer = vulkan->framebuffers.shadow_map_blur_2_framebuffers[i];
			framebuffer = {};
			framebuffer.width = shadow_map_size;
			framebuffer.height = shadow_map_size;
			framebuffer.sample_count = VK_SAMPLE_COUNT_1_BIT;
			framebuffer.color_attachment_count = 1;
			framebuffer.depth_stencil_attachment_image_index = UINT32_MAX;
			framebuffer.color_resolve_attachment_image_index = UINT32_MAX;
			framebuffer.color_attachment_image_indices[0] = vulkan->framebuffers.shadow_map_framebuffers[i].color_attachment_image_indices[0];

			vulkan_image &color_image = vulkan->memory_regions.image_region_images[framebuffer.color_attachment_image_indices[0]];
			VkImageView image_views[1] = {color_image.view};
			VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
			framebuffer_info.renderPass = vulkan->render_passes.shadow_map_render_passes[2];
			framebuffer_info.attachmentCount = m_countof(image_views);
			framebuffer_info.pAttachments = image_views;
			framebuffer_info.width = shadow_map_size;
			framebuffer_info.height = shadow_map_size;
			framebuffer_info.layers = 1;
			m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer.framebuffer));
		}
	}
	{ // main
		for (uint32 i = 0; i < m_countof(vulkan->framebuffers.color_framebuffers); i += 1) {
			vulkan_framebuffer &framebuffer = vulkan->framebuffers.color_framebuffers[i];
			framebuffer.width = vulkan->swap_chain.image_width;
			framebuffer.height = vulkan->swap_chain.image_height;
			framebuffer.sample_count = sample_count;
			framebuffer.color_attachment_count = 1;
			framebuffer.color_resolve_attachment_image_index = UINT32_MAX;
			{
				VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_info.imageType = VK_IMAGE_TYPE_2D;
				image_info.format = VK_FORMAT_R16G16B16A16_SFLOAT;
				image_info.extent = {framebuffer.width, framebuffer.height, 1};
				image_info.mipLevels = 1;
				image_info.arrayLayers = 1;
				image_info.samples = sample_count;
				image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				image_info.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_info.format = image_info.format;
				image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				image_view_info.subresourceRange.levelCount = 1;
				image_view_info.subresourceRange.layerCount = 1;
				framebuffer.color_attachment_image_indices[0] = append_vulkan_image_region(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);
			}
			{
				VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_info.imageType = VK_IMAGE_TYPE_2D;
				image_info.format = VK_FORMAT_D32_SFLOAT;
				image_info.extent = {framebuffer.width, framebuffer.height, 1};
				image_info.mipLevels = 1;
				image_info.arrayLayers = 1;
				image_info.samples = sample_count;
				image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				image_info.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_info.format = image_info.format;
				image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				image_view_info.subresourceRange.levelCount = 1;
				image_view_info.subresourceRange.layerCount = 1;
				framebuffer.depth_stencil_attachment_image_index = append_vulkan_image_region(vulkan, image_info, image_view_info, nullptr, 0, 1, 4);
			}
			if (sample_count != VK_SAMPLE_COUNT_1_BIT) {
				VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
				image_info.imageType = VK_IMAGE_TYPE_2D;
				image_info.format = VK_FORMAT_R16G16B16A16_SFLOAT;
				image_info.extent = {framebuffer.width, framebuffer.height, 1};
				image_info.mipLevels = 1;
				image_info.arrayLayers = 1;
				image_info.samples = VK_SAMPLE_COUNT_1_BIT;
				image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
				image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				image_info.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
				image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				image_view_info.format = image_info.format;
				image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				image_view_info.subresourceRange.levelCount = 1;
				image_view_info.subresourceRange.layerCount = 1;
				framebuffer.color_resolve_attachment_image_index = append_vulkan_image_region(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);
			}
			{
				vulkan_image &color_image = vulkan->memory_regions.image_region_images[framebuffer.color_attachment_image_indices[0]];
				vulkan_image &depth_stencil_image = vulkan->memory_regions.image_region_images[framebuffer.depth_stencil_attachment_image_index];
				VkImageView image_views[3] = {color_image.view, depth_stencil_image.view, nullptr};
				if (sample_count != VK_SAMPLE_COUNT_1_BIT) {
					vulkan_image &color_resolve_image = vulkan->memory_regions.image_region_images[framebuffer.color_resolve_attachment_image_index];
					image_views[2] = color_resolve_image.view;
				}
				VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
				framebuffer_info.renderPass = vulkan->render_passes.color_render_pass;
				framebuffer_info.attachmentCount = (sample_count == VK_SAMPLE_COUNT_1_BIT) ? 2 : 3;
				framebuffer_info.pAttachments = image_views;
				framebuffer_info.width = framebuffer.width;
				framebuffer_info.height = framebuffer.height;
				framebuffer_info.layers = 1;
				m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer.framebuffer));
			}
		}
	}
	{ // swap chain
		for (uint32 i = 0; i < m_countof(vulkan->framebuffers.swap_chain_framebuffers); i += 1) {
			VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
			framebuffer_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
			framebuffer_info.attachmentCount = 1;
			framebuffer_info.pAttachments = &vulkan->swap_chain.image_views[i];
			framebuffer_info.width = vulkan->swap_chain.image_width;
			framebuffer_info.height = vulkan->swap_chain.image_height;
			framebuffer_info.layers = 1;
			m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &vulkan->framebuffers.swap_chain_framebuffers[i]));
		}
	}
	{ // descriptor indices
		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			uint32 blur_1_image_index = vulkan->framebuffers.shadow_map_framebuffers[i].color_attachment_image_indices[0];
			uint32 blur_1_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, blur_1_image_index, vulkan->samplers.shadow_map_sampler);
			vulkan->framebuffers.shadow_map_blur_1_descriptor_indices[i] = blur_1_descriptor_index;

			uint32 blur_2_image_index = vulkan->framebuffers.shadow_map_blur_1_framebuffers[i].color_attachment_image_indices[0];
			uint32 blur_2_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, blur_2_image_index, vulkan->samplers.shadow_map_sampler);
			vulkan->framebuffers.shadow_map_blur_2_descriptor_indices[i] = blur_2_descriptor_index;

			uint32 color_image_index = vulkan->framebuffers.color_framebuffers[i].color_attachment_image_indices[0];
			uint32 color_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, color_image_index, vulkan->samplers.color_framebuffer_sampler);
			vulkan->framebuffers.color_descriptor_indices[i] = color_descriptor_index;
		}
	}
}

void initialize_vulkan_pipelines(vulkan *vulkan, VkSampleCountFlagBits sample_count) {
	auto shader_module_from_file = [vulkan](const char *file_path) {
		VkShaderModule shader_module = {};
		file_mapping file_mapping = {};
		m_assert(open_file_mapping(file_path, &file_mapping));
		VkShaderModuleCreateInfo info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
		info.codeSize = file_mapping.size;
		info.pCode = (const uint32 *)file_mapping.ptr;
		m_vk_assert(vkCreateShaderModule(vulkan->device.device, &info, nullptr, &shader_module));
		close_file_mapping(&file_mapping);
		return shader_module;
	};
	{ // pipeline layout
		VkDescriptorSetLayout layouts[2] = {vulkan->descriptors.uniform_buffer_layout, vulkan->descriptors.combined_image_samplers_layout};
		VkPushConstantRange push_const_range = {VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, 128};
		VkPipelineLayoutCreateInfo pipeline_layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		pipeline_layout_info.setLayoutCount = m_countof(layouts);
		pipeline_layout_info.pSetLayouts = layouts;
		pipeline_layout_info.pushConstantRangeCount = 1;
		pipeline_layout_info.pPushConstantRanges = &push_const_range;
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &pipeline_layout_info, nullptr, &vulkan->pipelines.pipeline_layout));
	}
	{ // static model
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\static_model.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\static_model.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 44, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[6] = {
			{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
			{1, 0, VK_FORMAT_R32G32_SFLOAT, 12},
			{2, 0, VK_FORMAT_R16G16B16_SNORM, 20},
			{3, 0, VK_FORMAT_R16G16B16_SNORM, 26},
			{4, 0, VK_FORMAT_R8G8B8A8_UINT, 32},
			{5, 0, VK_FORMAT_R16G16B16A16_UNORM, 36}
		};

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

		VkGraphicsPipelineCreateInfo pipeline_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		pipeline_info.stageCount = m_countof(shader_stages);
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_state;
		pipeline_info.pInputAssemblyState = &input_assembly_state;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterization_state;
		pipeline_info.pMultisampleState = &multisample_state;
		pipeline_info.pDepthStencilState = &depth_stencil_state;
		pipeline_info.pColorBlendState = &color_blend_state;
		pipeline_info.pDynamicState = &dynamic_state;
		pipeline_info.layout = vulkan->pipelines.pipeline_layout;
		pipeline_info.renderPass = vulkan->render_passes.color_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.static_model_pipeline));
	}
	{ // static model wireframe
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\static_model_wireframe.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\static_model_wireframe.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 44, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[3] = {
			{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
			{1, 0, VK_FORMAT_R8G8B8A8_UINT, 32},
			{2, 0, VK_FORMAT_R16G16B16A16_UNORM, 36}
		};

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

		VkGraphicsPipelineCreateInfo pipeline_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		pipeline_info.stageCount = m_countof(shader_stages);
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_state;
		pipeline_info.pInputAssemblyState = &input_assembly_state;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterization_state;
		pipeline_info.pMultisampleState = &multisample_state;
		pipeline_info.pDepthStencilState = &depth_stencil_state;
		pipeline_info.pColorBlendState = &color_blend_state;
		pipeline_info.pDynamicState = &dynamic_state;
		pipeline_info.layout = vulkan->pipelines.pipeline_layout;
		pipeline_info.renderPass = vulkan->render_passes.color_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.static_model_wireframe_pipeline));
	}
	{ // static model shadow map
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\static_model_shadow_map.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\static_model_shadow_map.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 44, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[3] = {
			{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
			{1, 0, VK_FORMAT_R8G8B8A8_UINT, 32},
			{2, 0, VK_FORMAT_R16G16B16A16_UNORM, 36}
		};

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

		VkGraphicsPipelineCreateInfo pipeline_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		pipeline_info.stageCount = m_countof(shader_stages);
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_state;
		pipeline_info.pInputAssemblyState = &input_assembly_state;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterization_state;
		pipeline_info.pMultisampleState = &multisample_state;
		pipeline_info.pDepthStencilState = &depth_stencil_state;
		pipeline_info.pColorBlendState = &color_blend_state;
		pipeline_info.pDynamicState = &dynamic_state;
		pipeline_info.layout = vulkan->pipelines.pipeline_layout;
		pipeline_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.static_model_shadow_map_pipeline));
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

		VkGraphicsPipelineCreateInfo pipeline_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		pipeline_info.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
		pipeline_info.stageCount = m_countof(shader_stages);
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_state;
		pipeline_info.pInputAssemblyState = &input_assembly_state;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterization_state;
		pipeline_info.pMultisampleState = &multisample_state;
		pipeline_info.pDepthStencilState = &depth_stencil_state;
		pipeline_info.pColorBlendState = &color_blend_state;
		pipeline_info.pDynamicState = &dynamic_state;
		pipeline_info.layout = vulkan->pipelines.pipeline_layout;
		pipeline_info.renderPass = vulkan->render_passes.shadow_map_render_passes[1];
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0]));
		pipeline_info.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
		pipeline_info.renderPass = vulkan->render_passes.shadow_map_render_passes[2];
		pipeline_info.basePipelineHandle = vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0];
		pipeline_info.basePipelineIndex = -1;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.shadow_map_gaussian_blur_pipelines[1]));
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

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = vulkan->pipelines.pipeline_layout;
		create_info.renderPass = vulkan->render_passes.color_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.skybox_pipeline));
	}
	{ // terrain
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\terrain.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\terrain.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 20, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[2] = {
			{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
			{1, 0, VK_FORMAT_R32G32_SFLOAT, 12},
		};

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

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = vulkan->pipelines.pipeline_layout;
		create_info.renderPass = vulkan->render_passes.color_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.terrain_pipeline));
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

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = vulkan->pipelines.pipeline_layout;
		create_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.imgui_pipeline));
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

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = vulkan->pipelines.pipeline_layout;
		create_info.renderPass = vulkan->render_passes.color_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.text_pipeline));
	}
	{ // color vertex
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\color_vertex.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\color_vertex.frag.spv");
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

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = vulkan->pipelines.pipeline_layout;
		create_info.renderPass = vulkan->render_passes.color_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.color_vertex_pipeline));
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

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = vulkan->pipelines.pipeline_layout;
		create_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.swap_chain_pipeline));
	}
}

void initialize_vulkan(vulkan *vulkan, const window &window) {
	vulkan->frame_memory_arena.name = "vulkan frame";
	vulkan->frame_memory_arena.capacity = m_megabytes(4);
	vulkan->frame_memory_arena.memory = allocate_virtual_memory(vulkan->frame_memory_arena.capacity);
	m_assert(vulkan->frame_memory_arena.memory);

	bool vsync_on = false;
	VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT;
	initialize_vulkan_device(vulkan);
	initialize_vulkan_swap_chain(vulkan, window, vsync_on);
	initialize_vulkan_memory_regions(vulkan);
	initialize_vulkan_cmd_buffers(vulkan);
	initialize_vulkan_syncs(vulkan);
	initialize_vulkan_samplers(vulkan);
	initialize_vulkan_descriptors(vulkan);
	initialize_vulkan_render_passes(vulkan, sample_count);
	initialize_vulkan_framebuffers(vulkan, sample_count);
	initialize_vulkan_pipelines(vulkan, sample_count);
}

void vulkan_begin_render(vulkan *vulkan) {
	m_vk_assert(vkAcquireNextImageKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, 0, vulkan->syncs.swap_chain_image_acquire_semaphores[vulkan->frame_index], VK_NULL_HANDLE, &vulkan->swap_chain_image_index));

	vkWaitForFences(vulkan->device.device, 1, &vulkan->syncs.graphic_queue_submit_fences[vulkan->frame_index], VK_TRUE, UINT64_MAX);
	vkResetFences(vulkan->device.device, 1, &vulkan->syncs.graphic_queue_submit_fences[vulkan->frame_index]);

	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	m_vk_assert(vkBeginCommandBuffer(vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index], &cmd_buffer_begin_info));
}

void vulkan_end_render(vulkan *vulkan, bool screen_shot = false) {
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];

	if (screen_shot) {
		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		image_memory_barrier.image = vulkan->swap_chain.images[vulkan->frame_index];
		image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_memory_barrier.subresourceRange.levelCount = 1;
		image_memory_barrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

		uint32 image_size = vulkan->swap_chain.image_width * vulkan->swap_chain.image_height * 4;
		m_assert(vulkan->memory_regions.staging_region_capacity > image_size);
		VkBufferImageCopy image_copy = {};
		image_copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
		image_copy.imageExtent = {vulkan->swap_chain.image_width, vulkan->swap_chain.image_height, 1};
		vkCmdCopyImageToBuffer(cmd_buffer, vulkan->swap_chain.images[vulkan->frame_index], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vulkan->memory_regions.staging_region_buffer, 1, &image_copy);

		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		image_memory_barrier.dstAccessMask = 0;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
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
	queue_submit_info.pSignalSemaphores = &vulkan->syncs.graphic_queue_submit_semaphores[vulkan->frame_index];
	m_vk_assert(vkQueueSubmit(vulkan->device.graphic_queue, 1, &queue_submit_info, vulkan->syncs.graphic_queue_submit_fences[vulkan->frame_index]));

	if (screen_shot) {
		vkWaitForFences(vulkan->device.device, 1, &vulkan->syncs.graphic_queue_submit_fences[vulkan->frame_index], VK_TRUE, UINT64_MAX);
		flip_rgba_image(vulkan->memory_regions.staging_region_buffer_ptr, vulkan->swap_chain.image_width, vulkan->swap_chain.image_height);
		rgba_image_to_bmp_file("screen_shot.bmp", vulkan->memory_regions.staging_region_buffer_ptr, vulkan->swap_chain.image_width, vulkan->swap_chain.image_height);
	}

	VkPresentInfoKHR device_queue_present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
	device_queue_present_info.waitSemaphoreCount = 1;
	device_queue_present_info.pWaitSemaphores = &vulkan->syncs.graphic_queue_submit_semaphores[vulkan->frame_index];
	device_queue_present_info.swapchainCount = 1;
	device_queue_present_info.pSwapchains = &vulkan->swap_chain.swap_chain;
	device_queue_present_info.pImageIndices = &vulkan->swap_chain_image_index;
	m_vk_assert(vkQueuePresentKHR(vulkan->device.graphic_queue, &device_queue_present_info));

	vulkan->frame_index = (vulkan->frame_index + 1) % vulkan_buffering_count;
}
