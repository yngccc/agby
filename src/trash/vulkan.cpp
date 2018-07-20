/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define VK_NO_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "math.cpp"

#define m_vulkan_procs                                                                    \
  m_x(PFN_vkEnumerateInstanceLayerProperties, vkEnumerateInstanceLayerProperties)         \
  m_x(PFN_vkEnumerateInstanceExtensionProperties, vkEnumerateInstanceExtensionProperties) \
  m_x(PFN_vkCreateInstance, vkCreateInstance)                                             \
  m_x(PFN_vkGetInstanceProcAddr, vkGetInstanceProcAddr)

#define m_vulkan_instance_procs                                                                 \
  m_x(PFN_vkEnumeratePhysicalDevices, vkEnumeratePhysicalDevices)                               \
  m_x(PFN_vkCreateDevice, vkCreateDevice)                                                       \
  m_x(PFN_vkGetDeviceProcAddr, vkGetDeviceProcAddr)                                             \
	m_x(PFN_vkEnumerateDeviceLayerProperties, vkEnumerateDeviceLayerProperties)	                  \
  m_x(PFN_vkEnumerateDeviceExtensionProperties, vkEnumerateDeviceExtensionProperties)           \
	m_x(PFN_vkGetPhysicalDeviceProperties, vkGetPhysicalDeviceProperties)                     	  \
  m_x(PFN_vkGetPhysicalDeviceQueueFamilyProperties, vkGetPhysicalDeviceQueueFamilyProperties)   \
  m_x(PFN_vkGetPhysicalDeviceMemoryProperties, vkGetPhysicalDeviceMemoryProperties)             \
  m_x(PFN_vkGetPhysicalDeviceFormatProperties, vkGetPhysicalDeviceFormatProperties)             \
  m_x(PFN_vkGetPhysicalDeviceFeatures, vkGetPhysicalDeviceFeatures)                             \
  m_x(PFN_vkCreateWin32SurfaceKHR, vkCreateWin32SurfaceKHR)                                     \
  m_x(PFN_vkDestroySurfaceKHR, vkDestroySurfaceKHR)                                             \
  m_x(PFN_vkGetPhysicalDeviceSurfaceSupportKHR, vkGetPhysicalDeviceSurfaceSupportKHR)           \
  m_x(PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR, vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
  m_x(PFN_vkGetPhysicalDeviceSurfaceFormatsKHR, vkGetPhysicalDeviceSurfaceFormatsKHR)           \
  m_x(PFN_vkGetPhysicalDeviceSurfacePresentModesKHR, vkGetPhysicalDeviceSurfacePresentModesKHR) \
  m_x(PFN_vkQueueWaitIdle, vkQueueWaitIdle)                                                     \
  m_x(PFN_vkQueueSubmit, vkQueueSubmit)                                                         \
  m_x(PFN_vkQueuePresentKHR, vkQueuePresentKHR)                                                 \
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
  m_x(PFN_vkCmdDrawIndirect, vkCmdDrawIndirect)													                        \
  m_x(PFN_vkCmdDrawIndexedIndirect, vkCmdDrawIndexedIndirect)						                        \
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

const uint32 vulkan_swapchain_image_count = 3;
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

struct vulkan_swapchain {
	VkSwapchainKHR swapchain;
	VkImage images[vulkan_swapchain_image_count];
	VkImageView image_views[vulkan_swapchain_image_count];
	uint32 width;
	uint32 height;
	VkPresentModeKHR present_mode;
	VkSurfaceKHR surface;
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
	VkSemaphore swapchain_image_acquire_semaphores[vulkan_buffering_count];
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

struct vulkan_image_memory_region {
	vulkan_image *images;
	uint32 image_count;
	uint32 image_capacity;
	VkDeviceMemory memory;
	uint64 memory_size;
	uint64 memory_capacity;
};

struct vulkan_buffer_memory_region {
	VkBuffer buffer;
	uint8 *buffer_ptr;
	uint32 buffer_size;
	uint32 buffer_capacity;
	VkDeviceMemory memory;
};

struct vulkan_memory_regions {
	vulkan_image_memory_region images;
	vulkan_buffer_memory_region vertex_buffer;
	vulkan_buffer_memory_region frame_uniform_buffers[vulkan_buffering_count];
	vulkan_buffer_memory_region frame_vertex_buffers[vulkan_buffering_count];
	vulkan_buffer_memory_region staging_buffer;
};

struct vulkan_samplers {
	VkSampler shadow_map_sampler;
	VkSampler gaussian_blur_sampler;
	VkSampler skybox_cubemap_sampler;
	VkSampler terrain_height_map_sampler;
	VkSampler terrain_diffuse_map_sampler;
	VkSampler color_framebuffer_sampler;
	VkSampler mipmap_samplers[16];
};

struct vulkan_descriptor_sets {
	VkDescriptorSet frame_uniform_buffers[vulkan_buffering_count];
	VkDescriptorSetLayout uniform_buffer_layout;

	VkDescriptorSet image_sampler_2d;
	VkDescriptorSetLayout image_sampler_2d_layout;
	uint32 image_sampler_2d_count;
	uint32 image_sampler_2d_capacity;
	uint32 *image_sampler_2d_image_indices;

	VkDescriptorSet image_sampler_cube;
	VkDescriptorSetLayout image_sampler_cube_layout;
	uint32 image_sampler_cube_count;
	uint32 image_sampler_cube_capacity;
	uint32 *image_sampler_cube_image_indices;

	VkDescriptorPool pool;
};

struct vulkan_render_passes {
	VkRenderPass shadow_map_render_passes[3];
	VkRenderPass color_render_pass;
	VkRenderPass swapchain_render_pass;
};

struct vulkan_framebuffer {
	VkFramebuffer framebuffer;
	uint32 color_attachment_image_indices[4];
	uint32 color_attachment_count;
	uint32 depth_stencil_attachment_image_index;
	uint32 color_resolve_attachment_image_index;
};

struct vulkan_framebuffers {
	vulkan_framebuffer shadow_map_framebuffers[vulkan_buffering_count];
	vulkan_framebuffer shadow_map_blur_1_framebuffers[vulkan_buffering_count];
	vulkan_framebuffer shadow_map_blur_2_framebuffers[vulkan_buffering_count];
	VkSampleCountFlagBits shadow_map_framebuffer_msaa;
	uint32 shadow_map_framebuffer_width;
	uint32 shadow_map_framebuffer_height;

	vulkan_framebuffer color_framebuffers[vulkan_buffering_count];
	VkSampleCountFlagBits color_framebuffer_msaa;
	uint32 color_framebuffer_width;
	uint32 color_framebuffer_height;

	VkFramebuffer swapchain_framebuffers[vulkan_swapchain_image_count];

	uint32 shadow_map_blur_1_descriptor_indices[vulkan_buffering_count];
	uint32 shadow_map_blur_2_descriptor_indices[vulkan_buffering_count];
	uint32 color_descriptor_indices[vulkan_buffering_count];
};

struct vulkan_pipelines {
	VkPipelineLayout pipeline_layout;
	
	VkPipeline model_pipeline;
	VkPipeline model_wireframe_pipeline;
	VkPipeline model_shadow_map_pipeline;
	VkPipeline shadow_map_gaussian_blur_pipelines[2];
	VkPipeline skybox_pipeline;
	VkPipeline terrain_pipeline;
	VkPipeline terrain_brush_pipeline;
	VkPipeline text_pipeline;
	VkPipeline collision_shape_pipeline;
	VkPipeline reference_grid_pipeline;
	VkPipeline swapchain_pipeline;
	VkPipeline imgui_pipeline;
};

struct vulkan_memory_config {
	uint32 image_capacity;
	uint64 image_memory_size;
	uint64 vertex_buffer_memory_size;
	uint64 frame_uniform_buffer_memory_size;
	uint64 frame_vertex_buffer_memory_size;
	uint64 staging_buffer_memory_size;
};

struct vulkan_descriptor_sets_config {
	uint32 image_sampler_2d_capacity;
	uint32 image_sampler_cube_capacity;
};

struct vulkan_framebuffers_config {
	uint32 color_framebuffer_width;
	uint32 color_framebuffer_height;
	VkSampleCountFlagBits color_framebuffer_msaa;
	uint32 shadow_map_framebuffer_width;
	uint32 shadow_map_framebuffer_height;
};

struct vulkan_config {
	bool validation_layer;
	bool vsync;
	vulkan_memory_config memory_config;
	vulkan_descriptor_sets_config descriptor_set_config;
	vulkan_framebuffers_config framebuffers_config;
};

struct vulkan {
	vulkan_device device;
	vulkan_swapchain swapchain;
	vulkan_memory_regions memory_regions;
	vulkan_render_passes render_passes;
	vulkan_framebuffers framebuffers;
	vulkan_cmd_buffers cmd_buffers;
	vulkan_syncs syncs;
	vulkan_samplers samplers;
	vulkan_descriptor_sets descriptor_sets;
	vulkan_pipelines pipelines;

	uint32 buffering_index;
	uint32 swapchain_image_index;
	vec4 swapchain_framebuffer_region;
};

#define m_vk_assert(vk_call) { VkResult result = vk_call; if (result != VK_SUCCESS) { fatal("Vulkan Error:\n\nCode: %s\nError: %s\nFile: %s\nLine: %d", #vk_call, vk_result_to_str(result), __FILE__, __LINE__); } }

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
		default: return "Unknown VkResult";
	}
}

VkBool32 vulkan_debug_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, uint64_t object, size_t location, int32 message_code, const char *layer_prefix, const char *message, void *user_data) {
	if (IsDebuggerPresent()) {
		__debugbreak();
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
	else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		printf("vulkan information code %d: %s\n", message_code, message);
	}
	else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		printf("vulkan debug code %d: %s\n", message_code, message);
	}
	return VK_FALSE;
};

void initialize_vulkan_device(vulkan *vulkan, bool validation_layer) {
	{ // dynamic load vulkan DLL
		HMODULE vulkan_1_dll = LoadLibraryA("vulkan-1.dll");
		m_assert(vulkan_1_dll, "vulkan-1.dll cannot be found");
		#define m_x(shape, name) name = (shape)GetProcAddress(vulkan_1_dll, #name); m_assert(name, "");
		m_vulkan_procs
		#undef m_x
	}
	{ // instance
		uint32 instance_layer_count = 0;
		m_vk_assert(vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr));
		std::vector<VkLayerProperties> instance_layers(instance_layer_count);
		m_vk_assert(vkEnumerateInstanceLayerProperties(&instance_layer_count, &instance_layers[0]));
		printf("Instance Layers:\n");
		for (auto &layer : instance_layers) {
			printf("  %s\n", layer.layerName);
		}
		printf("\n");
		uint32 instance_extension_count = 0;
		m_vk_assert(vkEnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr));
		std::vector<VkExtensionProperties> instance_extensions(instance_extension_count);
		m_vk_assert(vkEnumerateInstanceExtensionProperties(nullptr, &instance_extension_count, &instance_extensions[0]));
		printf("Instance Extensions:\n");
		for (auto &extension : instance_extensions) {
			printf("  %s\n", extension.extensionName);
		}
		printf("\n");
		
		const char *enabled_instance_layers[] = {"VK_LAYER_LUNARG_standard_validation"};
		const char *enabled_instance_extensions[] = {"VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_debug_report"};
		VkApplicationInfo application_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
		application_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);
		VkInstanceCreateInfo instance_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
		instance_info.pApplicationInfo = &application_info;
		instance_info.enabledLayerCount = m_countof(enabled_instance_layers);
		if (!validation_layer) {
			instance_info.enabledLayerCount = 0;
		}
		instance_info.ppEnabledLayerNames = enabled_instance_layers;
		instance_info.enabledExtensionCount = m_countof(enabled_instance_extensions);
		instance_info.ppEnabledExtensionNames = enabled_instance_extensions;
		m_vk_assert(vkCreateInstance(&instance_info, nullptr, &vulkan->device.instance));

		#define m_x(shape, name) name = (shape)vkGetInstanceProcAddr(vulkan->device.instance, #name); m_assert(name, "");
		m_vulkan_instance_procs
		#undef m_x
			
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vulkan->device.instance, "vkCreateDebugReportCallbackEXT");
		if (vkCreateDebugReportCallbackEXT) {
			VkDebugReportCallbackCreateInfoEXT debug_callback_info = {VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT};
			debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			debug_callback_info.pfnCallback = vulkan_debug_callback;
			VkDebugReportCallbackEXT debug_callback;
			m_vk_assert(vkCreateDebugReportCallbackEXT(vulkan->device.instance, &debug_callback_info, nullptr, &debug_callback));
		}
	}
	{ // physical device
		uint32 physical_device_count = 0;
		m_vk_assert(vkEnumeratePhysicalDevices(vulkan->device.instance, &physical_device_count, nullptr));
		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		m_vk_assert(vkEnumeratePhysicalDevices(vulkan->device.instance, &physical_device_count, &physical_devices[0]));
		printf("Physical Devices: ");
		for (auto &physical_device : physical_devices) {
			VkPhysicalDeviceProperties physical_device_properties = {};
			vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);
			printf("\"%s\" ", physical_device_properties.deviceName);
		}
		printf("\n");
		for (auto &physical_device : physical_devices) {
			VkPhysicalDeviceProperties physical_device_properties = {};
			vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);
			if (physical_device_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				continue;
			}
			uint32 physical_device_layer_count = 0;
			m_vk_assert(vkEnumerateDeviceLayerProperties(physical_device, &physical_device_layer_count, nullptr));
			std::vector<VkLayerProperties> physical_device_layers(physical_device_layer_count);
			m_vk_assert(vkEnumerateDeviceLayerProperties(physical_device, &physical_device_layer_count, &physical_device_layers[0]));
			uint32 physical_device_extension_count = 0;
			m_vk_assert(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &physical_device_extension_count, nullptr));
			std::vector<VkExtensionProperties> physical_device_extensions(physical_device_extension_count);
			m_vk_assert(vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &physical_device_extension_count, &physical_device_extensions[0]));
			VkPhysicalDeviceFeatures physical_device_features = {};
			vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

			VkBool32 support_swapchain_extension = VK_FALSE;
			for (auto &extension : physical_device_extensions) {
				if (!strcmp(extension.extensionName, "VK_KHR_swapchain")) {
					support_swapchain_extension = VK_TRUE;
					break;
				}
			}
			bool physical_device_capable =
				support_swapchain_extension &&
				physical_device_features.samplerAnisotropy &&
				physical_device_features.textureCompressionBC &&
				physical_device_features.wideLines &&
				physical_device_features.fillModeNonSolid &&
				physical_device_features.shaderUniformBufferArrayDynamicIndexing &&
				physical_device_features.shaderSampledImageArrayDynamicIndexing;

			if (physical_device_capable) {
				printf("Using Physical Device: %s\n", physical_device_properties.deviceName);
				printf("\nPhysical Device Layers:\n");
				for (auto &layer : physical_device_layers) {
					printf("  %s\n", layer.layerName);
				}
				printf("\nPhysical Device Extensions:\n");
				for (auto &extension : physical_device_extensions) {
					printf("  %s\n", extension.extensionName);
				}
				printf("\nAPI Version: %d.%d.%d\n", VK_VERSION_MAJOR(physical_device_properties.apiVersion), VK_VERSION_MINOR(physical_device_properties.apiVersion), VK_VERSION_PATCH(physical_device_properties.apiVersion));
				printf("Driver Version: %d.%d.%d\n", VK_VERSION_MAJOR(physical_device_properties.driverVersion), VK_VERSION_MINOR(physical_device_properties.driverVersion), VK_VERSION_PATCH(physical_device_properties.driverVersion));
				vulkan->device.physical_device = physical_device;
				vulkan->device.physical_device_properties = physical_device_properties;
				vkGetPhysicalDeviceMemoryProperties(physical_device, &vulkan->device.physical_device_memory_properties);

				const char *enabled_device_layers[] = {"VK_LAYER_LUNARG_standard_validation"};
				const char *enabled_device_extensions[] = {"VK_KHR_swapchain"};

				VkPhysicalDeviceFeatures enabled_device_features = {};
				enabled_device_features.samplerAnisotropy = VK_TRUE;
				enabled_device_features.textureCompressionBC = VK_TRUE;
				enabled_device_features.wideLines = VK_TRUE;
				enabled_device_features.fillModeNonSolid = VK_TRUE;
				enabled_device_features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
				enabled_device_features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;

				uint32 queue_family_count = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(vulkan->device.physical_device, &queue_family_count, nullptr);
				std::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count);
				vkGetPhysicalDeviceQueueFamilyProperties(vulkan->device.physical_device, &queue_family_count, &queue_family_properties[0]);
				for (auto &family : queue_family_properties) {
					printf("Queue familiy properties: %d queues, ", family.queueCount);
					if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						printf("\"graphic\" ");
					}
					if (family.queueFlags & VK_QUEUE_TRANSFER_BIT) {
						printf("\"transfer\" ");
					}
					if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
						printf("\"compute\" ");
					}
					if (family.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
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
				m_assert(vulkan->device.graphic_queue_family != UINT32_MAX, "");
				m_assert(vulkan->device.compute_queue_family != UINT32_MAX, "");
				m_assert(vulkan->device.transfer_queue_family != UINT32_MAX, "");

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

				VkDeviceCreateInfo device_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
				device_info.queueCreateInfoCount = queue_info_count;
				device_info.pQueueCreateInfos = queue_infos;
				device_info.enabledLayerCount = m_countof(enabled_device_layers);
				if (!validation_layer) {
					device_info.enabledLayerCount = 0;
				}
				device_info.ppEnabledLayerNames = enabled_device_layers;
				device_info.enabledExtensionCount = m_countof(enabled_device_extensions);
				device_info.ppEnabledExtensionNames = enabled_device_extensions;
				device_info.pEnabledFeatures = &enabled_device_features;
				m_vk_assert(vkCreateDevice(vulkan->device.physical_device, &device_info, nullptr, &vulkan->device.device));

        #define m_x(shape, name) name = (shape)vkGetDeviceProcAddr(vulkan->device.device, #name); m_assert(name, "");
				m_vulkan_device_procs
        #undef m_x

				vkGetDeviceQueue(vulkan->device.device, vulkan->device.graphic_queue_family, graphic_queue_index, &vulkan->device.graphic_queue);
				vkGetDeviceQueue(vulkan->device.device, vulkan->device.compute_queue_family, compute_queue_index, &vulkan->device.compute_queue);
				vkGetDeviceQueue(vulkan->device.device, vulkan->device.transfer_queue_family, transfer_queue_index, &vulkan->device.transfer_queue);

				break;
			}
		}
		m_assert(vulkan->device.physical_device, "cannot find a vulkan physical device");
	}
}

void initialize_vulkan_swapchain(vulkan *vulkan, window *window, bool vsync_on) {
	{ // create surface
		VkWin32SurfaceCreateInfoKHR surface_create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
		surface_create_info.hinstance = GetModuleHandle(nullptr);
		surface_create_info.hwnd = window->handle;
		m_vk_assert(vkCreateWin32SurfaceKHR(vulkan->device.instance, &surface_create_info, nullptr, &vulkan->swapchain.surface));
		VkBool32 physical_device_surface_support = VK_FALSE;
		m_vk_assert(vkGetPhysicalDeviceSurfaceSupportKHR(vulkan->device.physical_device, vulkan->device.graphic_queue_family, vulkan->swapchain.surface, &physical_device_surface_support));
		m_assert(physical_device_surface_support, "");
	}
	VkSurfaceCapabilitiesKHR surface_capabilities;
	{ // check surface capabilities
		m_vk_assert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan->device.physical_device, vulkan->swapchain.surface, &surface_capabilities));
		m_assert(surface_capabilities.maxImageCount >= vulkan_swapchain_image_count, "");
		m_assert((surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) && (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT), "");
		uint32 surface_image_formats_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->device.physical_device, vulkan->swapchain.surface, &surface_image_formats_count, nullptr);
		std::vector<VkSurfaceFormatKHR> surface_image_formats(surface_image_formats_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->device.physical_device, vulkan->swapchain.surface, &surface_image_formats_count, &surface_image_formats[0]);
		bool surface_support_bgra_srgb = false;
		for (auto &format : surface_image_formats) {
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				surface_support_bgra_srgb = true;
				break;
			}
		}
		m_assert(surface_support_bgra_srgb, "");
	}
	{ // set present mode
		uint32 surface_present_modes_count = 0;
		m_vk_assert(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->device.physical_device, vulkan->swapchain.surface, &surface_present_modes_count, nullptr));
		std::vector<VkPresentModeKHR> surface_present_modes(surface_present_modes_count);
		m_vk_assert(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->device.physical_device, vulkan->swapchain.surface, &surface_present_modes_count, &surface_present_modes[0]));
		printf("Swapchain Present Modes: ");
		const char *present_mode_strs[4] = {"Immediate", "Mailbox", "FIFO", "FIFO_Relaxed"};
		for (auto &mode : surface_present_modes) {
			printf("\"%s\" ", present_mode_strs[mode]);
		}
		if (vsync_on) {
			vulkan->swapchain.present_mode = VK_PRESENT_MODE_FIFO_KHR;
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
				vulkan->swapchain.present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			}
			else if (immediate_mode_exist) {
				vulkan->swapchain.present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
			else {
				vulkan->swapchain.present_mode = VK_PRESENT_MODE_FIFO_KHR;
			}
		}
		printf("\nUsing: \"%s\"\n", present_mode_strs[vulkan->swapchain.present_mode]);
	}
	{ // create swap chain, swap chain images
		VkSwapchainCreateInfoKHR swapchain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
		swapchain_create_info.surface = vulkan->swapchain.surface;
		swapchain_create_info.minImageCount = vulkan_swapchain_image_count;
		swapchain_create_info.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
		swapchain_create_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchain_create_info.imageExtent = surface_capabilities.currentExtent;
		swapchain_create_info.imageArrayLayers = 1;
		swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchain_create_info.presentMode = vulkan->swapchain.present_mode;
		m_vk_assert(vkCreateSwapchainKHR(vulkan->device.device, &swapchain_create_info, nullptr, &vulkan->swapchain.swapchain));
		uint32 swapchain_image_count = 0;
		m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swapchain.swapchain, &swapchain_image_count, nullptr));
		m_assert(swapchain_image_count == vulkan_swapchain_image_count, "");
		m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swapchain.swapchain, &swapchain_image_count, vulkan->swapchain.images));
		for (uint32 i = 0; i < vulkan_swapchain_image_count; i += 1) {
			VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_create_info.image = vulkan->swapchain.images[i];
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = swapchain_create_info.imageFormat;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.layerCount = 1;
			m_vk_assert(vkCreateImageView(vulkan->device.device, &image_view_create_info, nullptr, &vulkan->swapchain.image_views[i]));
		}
		vulkan->swapchain.width = swapchain_create_info.imageExtent.width;
		vulkan->swapchain.height = swapchain_create_info.imageExtent.height;
		printf("\nSwapchain Image Size: %dx%d\n", vulkan->swapchain.width, vulkan->swapchain.height);
	}
}

void recreate_vulkan_swapchain(vulkan *vulkan) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	for (auto &view : vulkan->swapchain.image_views) {
		vkDestroyImageView(vulkan->device.device, view, nullptr);
	}
	for (auto &framebuffer : vulkan->framebuffers.swapchain_framebuffers) {
		vkDestroyFramebuffer(vulkan->device.device, framebuffer, nullptr);
	}
	vkDestroySwapchainKHR(vulkan->device.device, vulkan->swapchain.swapchain, nullptr);

	VkSurfaceCapabilitiesKHR surface_capabilities = {};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan->device.physical_device, vulkan->swapchain.surface, &surface_capabilities);
	VkSwapchainCreateInfoKHR swapchain_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
	swapchain_info.surface = vulkan->swapchain.surface;
	swapchain_info.minImageCount = vulkan_swapchain_image_count;
	swapchain_info.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
	swapchain_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	swapchain_info.imageExtent = surface_capabilities.currentExtent;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = vulkan->swapchain.present_mode;
	m_vk_assert(vkCreateSwapchainKHR(vulkan->device.device, &swapchain_info, nullptr, &vulkan->swapchain.swapchain));
	uint32 new_swapchain_image_count = 0;
	m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swapchain.swapchain, &new_swapchain_image_count, nullptr));
	m_assert(new_swapchain_image_count == vulkan_swapchain_image_count, "");
	m_vk_assert(vkGetSwapchainImagesKHR(vulkan->device.device, vulkan->swapchain.swapchain, &new_swapchain_image_count, vulkan->swapchain.images));
	for (uint32 i = 0; i < vulkan_swapchain_image_count; i += 1) {
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.image = vulkan->swapchain.images[i];
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = swapchain_info.imageFormat;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;
		m_vk_assert(vkCreateImageView(vulkan->device.device, &image_view_info, nullptr, &vulkan->swapchain.image_views[i]));
	}
	vulkan->swapchain.width = swapchain_info.imageExtent.width;
	vulkan->swapchain.height = swapchain_info.imageExtent.height;
	for (uint32 i = 0; i < vulkan_swapchain_image_count; i += 1) {
		VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
		framebuffer_info.renderPass = vulkan->render_passes.swapchain_render_pass;
		framebuffer_info.attachmentCount = 1;
		framebuffer_info.pAttachments = &vulkan->swapchain.image_views[i];
		framebuffer_info.width = vulkan->swapchain.width;
		framebuffer_info.height = vulkan->swapchain.height;
		framebuffer_info.layers = 1;
		m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &vulkan->framebuffers.swapchain_framebuffers[i]));
	}

	float width_1 = (float)vulkan->swapchain.width;
	float height_1 = (float)vulkan->swapchain.height;
	float width_2 = (float)vulkan->framebuffers.color_framebuffer_width;
	float height_2 = (float)vulkan->framebuffers.color_framebuffer_height;
	vulkan->swapchain_framebuffer_region = fit_rect_into_rect(width_1, height_1, width_2, height_2);
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
		m_vk_assert(vkCreateSemaphore(vulkan->device.device, &semaphore_create_info, nullptr, &vulkan->syncs.swapchain_image_acquire_semaphores[i]));
		m_vk_assert(vkCreateSemaphore(vulkan->device.device, &semaphore_create_info, nullptr, &vulkan->syncs.graphic_queue_submit_semaphores[i]));
		VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
		fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_vk_assert(vkCreateFence(vulkan->device.device, &fence_create_info, nullptr, &vulkan->syncs.graphic_queue_submit_fences[i]));
	}
}

VkDeviceMemory allocate_vulkan_memory(vulkan *vulkan, uint64 size, uint32 type_bits, uint32 flags) {
	VkDeviceMemory device_memory = nullptr;
	VkPhysicalDeviceMemoryProperties *properties = &vulkan->device.physical_device_memory_properties;
	for (uint32 i = 0; i < properties->memoryTypeCount; i += 1) {
		if (type_bits & (1 << i) && properties->memoryTypes[i].propertyFlags == flags) {
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
}

void initialize_vulkan_memory_regions(vulkan *vulkan, vulkan_memory_config memory_config) {
	{ // images
		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
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

		vulkan_image_memory_region *images = &vulkan->memory_regions.images;
		images->image_count = 0;
		images->image_capacity = memory_config.image_capacity;
		images->images = new vulkan_image[images->image_capacity]();
		images->memory_size = 0;
		images->memory_capacity = memory_config.image_memory_size;
		images->memory = allocate_vulkan_memory(vulkan, images->memory_capacity, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_assert(images->memory, "");
	}
	{ // vertex buffer
		vulkan_buffer_memory_region *buffer = &vulkan->memory_regions.vertex_buffer;
		VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size = memory_config.vertex_buffer_memory_size;
		buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &buffer->buffer));
		VkMemoryRequirements memory_requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, buffer->buffer, &memory_requirements);
		buffer->buffer_size = 0;
		buffer->buffer_capacity = (uint32)buffer_info.size;
		buffer->memory = allocate_vulkan_memory(vulkan, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_assert(buffer->memory, "");
		m_vk_assert(vkBindBufferMemory(vulkan->device.device, buffer->buffer, buffer->memory, 0));
	}
	// frame uniform buffer
	for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
		auto *buffer = &vulkan->memory_regions.frame_uniform_buffers[i];
		VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size = memory_config.frame_uniform_buffer_memory_size;
		buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &buffer->buffer));
		VkMemoryRequirements memory_requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, buffer->buffer, &memory_requirements);
		buffer->buffer_size = 0;
		buffer->buffer_capacity = (uint32)buffer_info.size;
		buffer->memory = allocate_vulkan_memory(vulkan, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_assert(buffer->memory, "");
		m_vk_assert(vkBindBufferMemory(vulkan->device.device, buffer->buffer, buffer->memory, 0));
		m_vk_assert(vkMapMemory(vulkan->device.device, buffer->memory, 0, VK_WHOLE_SIZE, 0, (void **)&buffer->buffer_ptr));
	}
	// frame vertex buffer
	for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
		auto *buffer = &vulkan->memory_regions.frame_vertex_buffers[i];
		VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size = memory_config.frame_vertex_buffer_memory_size;
		buffer_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &buffer->buffer));
		VkMemoryRequirements memory_requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, buffer->buffer, &memory_requirements);
		buffer->buffer_size = 0;
		buffer->buffer_capacity = (uint32)buffer_info.size;
		buffer->memory = allocate_vulkan_memory(vulkan, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_assert(buffer->memory, "");
		m_vk_assert(vkBindBufferMemory(vulkan->device.device, buffer->buffer, buffer->memory, 0));
		m_vk_assert(vkMapMemory(vulkan->device.device, buffer->memory, 0, VK_WHOLE_SIZE, 0, (void **)&buffer->buffer_ptr));
	}
	{ // staging buffer
		auto *buffer = &vulkan->memory_regions.staging_buffer;
		VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size = memory_config.staging_buffer_memory_size;
		buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		m_vk_assert(vkCreateBuffer(vulkan->device.device, &buffer_info, nullptr, &buffer->buffer));
		VkMemoryRequirements memory_requirements = {};
		vkGetBufferMemoryRequirements(vulkan->device.device, buffer->buffer, &memory_requirements);
		buffer->buffer_size = 0;
		buffer->buffer_capacity = (uint32)buffer_info.size;
		buffer->memory = allocate_vulkan_memory(vulkan, memory_requirements.size, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_assert(buffer->memory, "");
		m_vk_assert(vkBindBufferMemory(vulkan->device.device, buffer->buffer, buffer->memory, 0));
		m_vk_assert(vkMapMemory(vulkan->device.device, buffer->memory, 0, VK_WHOLE_SIZE, 0, (void **)&buffer->buffer_ptr));
	}
}

uint32 append_vulkan_images(vulkan *vulkan, VkImageCreateInfo image_info, VkImageViewCreateInfo image_view_info, uint8 *image_data, uint32 image_data_size, uint32 format_block_dimension, uint32 format_block_size) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	vulkan_image_memory_region *images = &vulkan->memory_regions.images;
	vulkan_buffer_memory_region *staging_buffer = &vulkan->memory_regions.staging_buffer;

	VkImageLayout layout = image_info.initialLayout;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	m_assert(images->image_count < images->image_capacity, "");
	uint32 image_index = images->image_count;
	vulkan_image *image = &images->images[images->image_count++];
	m_vk_assert(vkCreateImage(vulkan->device.device, &image_info, nullptr, &image->image));

	VkMemoryRequirements memory_requirements = {};
	vkGetImageMemoryRequirements(vulkan->device.device, image->image, &memory_requirements);
	round_up(&images->memory_size, memory_requirements.alignment);
  uint64 memory_offset = images->memory_size;
  images->memory_size += memory_requirements.size;
  m_assert(images->memory_size < images->memory_capacity, "");

	m_vk_assert(vkBindImageMemory(vulkan->device.device, image->image, images->memory, memory_offset));
	image_view_info.image = image->image;
	m_vk_assert(vkCreateImageView(vulkan->device.device, &image_view_info, nullptr, &image->view));

	image->width = image_info.extent.width;
	image->height = image_info.extent.height;
	image->mipmap_count = image_info.mipLevels;
	image->layer_count = image_info.arrayLayers;
	image->format = image_info.format;
	image->format_block_dimension = format_block_dimension;
	image->format_block_size = format_block_size;
	image->layout = layout;
	image->aspect_flags = image_view_info.subresourceRange.aspectMask;

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);
	if (image_data && image_data_size > 0) {
		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.image = image->image;
		image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.subresourceRange.aspectMask = image_view_info.subresourceRange.aspectMask;
		image_memory_barrier.subresourceRange.levelCount = image->mipmap_count;
		image_memory_barrier.subresourceRange.layerCount = image->layer_count;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

		m_assert(image_data_size <= staging_buffer->buffer_capacity, "");
		memcpy(staging_buffer->buffer_ptr, image_data, image_data_size);

		uint32 staging_buffer_offset = 0;
		uint32 mipmap_width = image->width;
		uint32 mipmap_height = image->height;
		for (uint32 i = 0; i < image->mipmap_count; i += 1) {
			VkBufferImageCopy buffer_image_copy = {};
			buffer_image_copy.bufferOffset = staging_buffer_offset;
			buffer_image_copy.imageSubresource.aspectMask = image_view_info.subresourceRange.aspectMask;
			buffer_image_copy.imageSubresource.mipLevel = i;
			buffer_image_copy.imageSubresource.layerCount = image->layer_count;
			buffer_image_copy.imageExtent = {mipmap_width, mipmap_height, 1};
			vkCmdCopyBufferToImage(cmd_buffer, staging_buffer->buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);
			uint32 block_count = (mipmap_width * mipmap_height) / (image->format_block_dimension * image->format_block_dimension);
			staging_buffer_offset += image->format_block_size * block_count * image->layer_count;
			mipmap_width = max(mipmap_width / 2, image->format_block_dimension);
			mipmap_height = max(mipmap_height / 2, image->format_block_dimension);
		}

		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = 0;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		image_memory_barrier.newLayout = layout;
		vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
	}
	else {
		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_memory_barrier.newLayout = layout;
		image_memory_barrier.image = image->image;
		image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_memory_barrier.subresourceRange.aspectMask = image_view_info.subresourceRange.aspectMask;
		image_memory_barrier.subresourceRange.levelCount = image->mipmap_count;
		image_memory_barrier.subresourceRange.layerCount = image->layer_count;
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

void retrieve_vulkan_image(vulkan *vulkan, uint32 image_index, uint8* image_data, uint32 image_data_size) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	vulkan_image *image = &vulkan->memory_regions.images.images[image_index];
	vulkan_buffer_memory_region *staging_buffer = &vulkan->memory_regions.staging_buffer;

	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(vulkan->device.device, image->image, &memory_requirements);
	m_assert(memory_requirements.size == image_data_size, "");
	m_assert(memory_requirements.size <= staging_buffer->buffer_capacity, "");

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);

	VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	image_memory_barrier.oldLayout = image->layout;
	image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	image_memory_barrier.image = image->image;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.subresourceRange.aspectMask = image->aspect_flags;
	image_memory_barrier.subresourceRange.levelCount = image->mipmap_count;
	image_memory_barrier.subresourceRange.layerCount = image->layer_count;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	uint32 staging_buffer_offset = 0;
	uint32 mipmap_width = image->width;
	uint32 mipmap_height = image->height;
	for (uint32 i = 0; i < image->mipmap_count; i += 1) {
		VkBufferImageCopy buffer_image_copy = {};
		buffer_image_copy.bufferOffset = staging_buffer_offset;
		buffer_image_copy.imageSubresource.aspectMask = image->aspect_flags;
		buffer_image_copy.imageSubresource.mipLevel = i;
		buffer_image_copy.imageSubresource.layerCount = image->layer_count;
		buffer_image_copy.imageExtent = {mipmap_width, mipmap_height, 1};
		vkCmdCopyImageToBuffer(cmd_buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, staging_buffer->buffer, 1, &buffer_image_copy);
		uint32 block_count = (mipmap_width * mipmap_height) / (image->format_block_dimension * image->format_block_dimension);
		staging_buffer_offset += image->format_block_size * block_count * image->layer_count;
		mipmap_width = max(mipmap_width / 2, image->format_block_dimension);
		mipmap_height = max(mipmap_height / 2, image->format_block_dimension);
	}

	image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	image_memory_barrier.dstAccessMask = 0;
	image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	image_memory_barrier.newLayout = image->layout;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	vkEndCommandBuffer(cmd_buffer);

	VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;
	vkQueueSubmit(vulkan->device.transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkan->device.transfer_queue);

	memcpy(image_data, staging_buffer->buffer_ptr, image_data_size);
}

void update_vulkan_image(vulkan *vulkan, uint32 image_index, uint8 *image_data, uint32 image_data_size) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	vulkan_image *image = &vulkan->memory_regions.images.images[image_index];
	vulkan_buffer_memory_region *staging_buffer = &vulkan->memory_regions.staging_buffer;

	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(vulkan->device.device, image->image, &memory_requirements);
	m_assert(memory_requirements.size == image_data_size, "");
	m_assert(memory_requirements.size <= staging_buffer->buffer_capacity, "");
	memcpy(staging_buffer->buffer_ptr, image_data, image_data_size);

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);

	VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
	image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.oldLayout = image->layout;
	image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.image = image->image;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.subresourceRange.aspectMask = image->aspect_flags;
	image_memory_barrier.subresourceRange.levelCount = image->mipmap_count;
	image_memory_barrier.subresourceRange.layerCount = image->layer_count;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	uint32 staging_buffer_offset = 0;
	uint32 mipmap_width = image->width;
	uint32 mipmap_height = image->height;
	for (uint32 i = 0; i < image->mipmap_count; i += 1) {
		VkBufferImageCopy buffer_image_copy = {};
		buffer_image_copy.bufferOffset = staging_buffer_offset;
		buffer_image_copy.imageSubresource.aspectMask = image->aspect_flags;
		buffer_image_copy.imageSubresource.mipLevel = i;
		buffer_image_copy.imageSubresource.layerCount = image->layer_count;
		buffer_image_copy.imageExtent = {mipmap_width, mipmap_height, 1};
		vkCmdCopyBufferToImage(cmd_buffer, staging_buffer->buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);
		uint32 block_count = (mipmap_width * mipmap_height) / (image->format_block_dimension * image->format_block_dimension);
		staging_buffer_offset += image->format_block_size * block_count * image->layer_count;
		mipmap_width = max(mipmap_width / 2, image->format_block_dimension);
		mipmap_height = max(mipmap_height / 2, image->format_block_dimension);
	}

	image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.dstAccessMask = 0;
	image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.newLayout = image->layout;
	vkCmdPipelineBarrier(cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

	vkEndCommandBuffer(cmd_buffer);

	VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;
	vkQueueSubmit(vulkan->device.transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vulkan->device.transfer_queue);
}

uint32 append_vulkan_vertex_buffer(vulkan *vulkan, const void *data, uint32 data_size, uint32 data_alignment) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	vulkan_buffer_memory_region *buffer = &vulkan->memory_regions.vertex_buffer;
	vulkan_buffer_memory_region *staging_buffer = &vulkan->memory_regions.staging_buffer;

	round_up(&buffer->buffer_size, data_alignment);
	uint32 buffer_offset = buffer->buffer_size;
	buffer->buffer_size += data_size;
	m_assert(buffer->buffer_size < buffer->buffer_capacity, "");

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.transfer_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	uint32 data_size_remain = data_size;
	uint32 current_buffer_offset = buffer_offset;
	while (data_size_remain > 0) {
		uint32 copy_size = min(data_size_remain, staging_buffer->buffer_capacity);
		memcpy(staging_buffer->buffer_ptr, data, copy_size);

		vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);
		VkBufferCopy buffer_copy = {0, current_buffer_offset, copy_size};
		vkCmdCopyBuffer(cmd_buffer, staging_buffer->buffer, buffer->buffer, 1, &buffer_copy);
		vkEndCommandBuffer(cmd_buffer);
		VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &cmd_buffer;
		vkQueueSubmit(vulkan->device.transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
		vkQueueWaitIdle(vulkan->device.transfer_queue);

		data_size_remain -= copy_size;
		data = (uint8 *)data + copy_size;
		current_buffer_offset += copy_size;
	}
	return buffer_offset;
}

uint32 append_vulkan_frame_uniform_buffer(vulkan *vulkan, const void *data, uint32 data_size) {
	vulkan_buffer_memory_region *buffer = &vulkan->memory_regions.frame_uniform_buffers[vulkan->buffering_index];
	round_up(&buffer->buffer_size, (uint32)vulkan->device.physical_device_properties.limits.minUniformBufferOffsetAlignment);
	uint32 buffer_offset = buffer->buffer_size;
	m_assert(buffer->buffer_size + data_size < buffer->buffer_capacity, "");
	memcpy(buffer->buffer_ptr + buffer->buffer_size, data, data_size);
	buffer->buffer_size += data_size;
	return buffer_offset;
}

uint32 append_vulkan_frame_vertex_buffer(vulkan *vulkan, const void *data, uint32 data_size, uint32 alignment) {
	vulkan_buffer_memory_region *buffer = &vulkan->memory_regions.frame_vertex_buffers[vulkan->buffering_index];
	round_up(&buffer->buffer_size, alignment);
	uint32 buffer_offset = buffer->buffer_size;
	m_assert(buffer->buffer_size + data_size < buffer->buffer_capacity, "");
	memcpy(buffer->buffer_ptr + buffer->buffer_size, data, data_size);
	buffer->buffer_size += data_size;
	return buffer_offset;
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
		sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.skybox_cubemap_sampler));
	}
	{ // terrain height/diffuse sampler
		VkSamplerCreateInfo sampler_create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
		sampler_create_info.magFilter = VK_FILTER_LINEAR;
		sampler_create_info.minFilter = VK_FILTER_LINEAR;
		sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.terrain_height_map_sampler));
		sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		m_vk_assert(vkCreateSampler(vulkan->device.device, &sampler_create_info, nullptr, &vulkan->samplers.terrain_diffuse_map_sampler));
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

void initialize_vulkan_descriptors(vulkan *vulkan, vulkan_descriptor_sets_config descriptor_sets_config) {
	{ // pool
		VkDescriptorPoolCreateInfo descriptor_pool_create_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
		descriptor_pool_create_info.maxSets = vulkan_buffering_count + 2;
		VkDescriptorPoolSize descriptor_pool_sizes[] = {
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vulkan_buffering_count},
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, vulkan_buffering_count * 2},
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptor_sets_config.image_sampler_2d_capacity + descriptor_sets_config.image_sampler_cube_capacity}
		};
		descriptor_pool_create_info.poolSizeCount = m_countof(descriptor_pool_sizes);
		descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes;
		m_vk_assert(vkCreateDescriptorPool(vulkan->device.device, &descriptor_pool_create_info, nullptr, &vulkan->descriptor_sets.pool));
	}
	{ // image sampler 2d
		vulkan->descriptor_sets.image_sampler_2d_count = 0;
		vulkan->descriptor_sets.image_sampler_2d_capacity = descriptor_sets_config.image_sampler_2d_capacity;
		VkDescriptorSetLayoutBinding layout_bindings[] = {
			{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptor_sets_config.image_sampler_2d_capacity, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
		};
		VkDescriptorSetLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
		layout_info.bindingCount = m_countof(layout_bindings);
		layout_info.pBindings = layout_bindings;
		m_vk_assert(vkCreateDescriptorSetLayout(vulkan->device.device, &layout_info, nullptr, &vulkan->descriptor_sets.image_sampler_2d_layout));

		VkDescriptorSetAllocateInfo set_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
		set_info.descriptorPool = vulkan->descriptor_sets.pool;
		set_info.descriptorSetCount = 1;
		set_info.pSetLayouts = &vulkan->descriptor_sets.image_sampler_2d_layout;
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &set_info, &vulkan->descriptor_sets.image_sampler_2d));

		vulkan->descriptor_sets.image_sampler_2d_image_indices = new uint32[descriptor_sets_config.image_sampler_2d_capacity];

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_info.extent = {2, 2, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;
		uint8 image_data[16] = {255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255};
		uint32 image_index = append_vulkan_images(vulkan, image_info, image_view_info, image_data, sizeof(image_data), 1, 4);
		VkImageView image_view = vulkan->memory_regions.images.images[image_index].view;
		VkSampler sampler = vulkan->samplers.mipmap_samplers[0];
		VkDescriptorImageInfo descriptor_image_info = {sampler, image_view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
		write_descriptor_set.dstSet = vulkan->descriptor_sets.image_sampler_2d;
		write_descriptor_set.dstBinding = 0;
		write_descriptor_set.descriptorCount = 1;
		write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_descriptor_set.pImageInfo = &descriptor_image_info;
		for (uint32 i = 0; i < descriptor_sets_config.image_sampler_2d_capacity; i += 1) {
			write_descriptor_set.dstArrayElement = i;
			vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
		}
	}
	{ // image sampler cube
		vulkan->descriptor_sets.image_sampler_cube_count = 0;
		vulkan->descriptor_sets.image_sampler_cube_capacity = descriptor_sets_config.image_sampler_cube_capacity;
		VkDescriptorSetLayoutBinding layout_bindings[] = {
			{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptor_sets_config.image_sampler_cube_capacity, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
		};
		VkDescriptorSetLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
		layout_info.bindingCount = m_countof(layout_bindings);
		layout_info.pBindings = layout_bindings;
		m_vk_assert(vkCreateDescriptorSetLayout(vulkan->device.device, &layout_info, nullptr, &vulkan->descriptor_sets.image_sampler_cube_layout));

		VkDescriptorSetAllocateInfo set_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
		set_info.descriptorPool = vulkan->descriptor_sets.pool;
		set_info.descriptorSetCount = 1;
		set_info.pSetLayouts = &vulkan->descriptor_sets.image_sampler_cube_layout;
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &set_info, &vulkan->descriptor_sets.image_sampler_cube));

		vulkan->descriptor_sets.image_sampler_cube_image_indices = new uint32[descriptor_sets_config.image_sampler_cube_capacity];

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_info.extent = {2, 2, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 6;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 6;
		uint8 cubemap_data[16 * 6] = {255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255,
																	255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255,
																	255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255,
																	255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255,
																	255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255,
																	255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255, 255};
		uint32 image_index = append_vulkan_images(vulkan, image_info, image_view_info, cubemap_data, sizeof(cubemap_data), 1, 4);
		VkImageView image_view = vulkan->memory_regions.images.images[image_index].view;
		VkSampler sampler = vulkan->samplers.mipmap_samplers[0];
		VkDescriptorImageInfo descriptor_image_info = {sampler, image_view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
		write_descriptor_set.dstSet = vulkan->descriptor_sets.image_sampler_cube;
		write_descriptor_set.dstBinding = 0;
		write_descriptor_set.descriptorCount = 1;
		write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_descriptor_set.pImageInfo = &descriptor_image_info;
		for (uint32 i = 0; i < descriptor_sets_config.image_sampler_cube_capacity; i += 1) {
			write_descriptor_set.dstArrayElement = i;
			vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
		}
	}
	{ // frame uniform buffer
		VkDescriptorSetLayoutBinding layout_bindings[] = {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
			{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
			{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr}
		};
		VkDescriptorSetLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
		layout_info.bindingCount = m_countof(layout_bindings);
		layout_info.pBindings = layout_bindings;
		m_vk_assert(vkCreateDescriptorSetLayout(vulkan->device.device, &layout_info, nullptr, &vulkan->descriptor_sets.uniform_buffer_layout));

		for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
			VkDescriptorSetAllocateInfo set_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
			set_info.descriptorPool = vulkan->descriptor_sets.pool;
			set_info.descriptorSetCount = 1;
			set_info.pSetLayouts = &vulkan->descriptor_sets.uniform_buffer_layout;
			m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &set_info, &vulkan->descriptor_sets.frame_uniform_buffers[i]));

			VkDescriptorSet descriptor_set = vulkan->descriptor_sets.frame_uniform_buffers[i];				
			VkDescriptorBufferInfo buffer_info = {vulkan->memory_regions.frame_uniform_buffers[i].buffer, 0, vulkan->device.physical_device_properties.limits.maxUniformBufferRange};
			VkWriteDescriptorSet write_sets[3] = {};
			for (uint32 i = 0; i < m_countof(write_sets); i += 1) {
				write_sets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write_sets[i].dstSet = descriptor_set;
				write_sets[i].dstBinding = i;
				write_sets[i].descriptorCount = 1;
				write_sets[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				write_sets[i].pBufferInfo = &buffer_info;
			}
			write_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			vkUpdateDescriptorSets(vulkan->device.device, m_countof(write_sets), write_sets, 0, nullptr);
		}
	}
}

uint32 append_vulkan_image_sampler_2d(vulkan *vulkan, uint32 image_index, VkSampler sampler) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	m_assert(vulkan->descriptor_sets.image_sampler_2d_count < vulkan->descriptor_sets.image_sampler_2d_capacity, "");
	VkDescriptorImageInfo descriptor_image_info = {sampler, vulkan->memory_regions.images.images[image_index].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = vulkan->descriptor_sets.image_sampler_2d;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.dstArrayElement = vulkan->descriptor_sets.image_sampler_2d_count;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	vulkan->descriptor_sets.image_sampler_2d_image_indices[vulkan->descriptor_sets.image_sampler_2d_count] = image_index;
	return vulkan->descriptor_sets.image_sampler_2d_count++;
}

void update_vulkan_image_sampler_2d(vulkan *vulkan, uint32 index, uint32 image_index, VkSampler sampler) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	m_assert(index < vulkan->descriptor_sets.image_sampler_2d_count, "");
	VkDescriptorImageInfo descriptor_image_info = {sampler, vulkan->memory_regions.images.images[image_index].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = vulkan->descriptor_sets.image_sampler_2d;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.dstArrayElement = index;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	vulkan->descriptor_sets.image_sampler_2d_image_indices[index] = image_index;
}

uint32 append_vulkan_image_sampler_cube(vulkan *vulkan, uint32 image_index, VkSampler sampler) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	m_assert(vulkan->descriptor_sets.image_sampler_cube_count < vulkan->descriptor_sets.image_sampler_cube_capacity, "");
	VkDescriptorImageInfo descriptor_image_info = {sampler, vulkan->memory_regions.images.images[image_index].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = vulkan->descriptor_sets.image_sampler_cube;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.dstArrayElement = vulkan->descriptor_sets.image_sampler_cube_count;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	vulkan->descriptor_sets.image_sampler_cube_image_indices[vulkan->descriptor_sets.image_sampler_cube_count] = image_index;
	return vulkan->descriptor_sets.image_sampler_cube_count++;
}

void update_vulkan_image_sampler_cube(vulkan *vulkan, uint32 index, uint32 image_index, VkSampler sampler) {
	vkWaitForFences(vulkan->device.device, m_countof(vulkan->syncs.graphic_queue_submit_fences), vulkan->syncs.graphic_queue_submit_fences, VK_TRUE, UINT64_MAX);

	m_assert(index < vulkan->descriptor_sets.image_sampler_cube_count, "");
	VkDescriptorImageInfo descriptor_image_info = {sampler, vulkan->memory_regions.images.images[image_index].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = vulkan->descriptor_sets.image_sampler_cube;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.dstArrayElement = index;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
	vulkan->descriptor_sets.image_sampler_cube_image_indices[index] = image_index;
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
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		attachments[1].format = VK_FORMAT_D32_SFLOAT;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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
	{ // color render pass
		VkAttachmentDescription attachments[3] = {};
		attachments[0].format = VK_FORMAT_R16G16B16A16_SFLOAT;
		attachments[0].samples = sample_count;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		if (sample_count != VK_SAMPLE_COUNT_1_BIT) {
			attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		attachments[1].format = VK_FORMAT_D32_SFLOAT;
		attachments[1].samples = sample_count;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		attachments[2].format = attachments[0].format;
		attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[2].initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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
		dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
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
		dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		VkRenderPassCreateInfo create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
		create_info.attachmentCount = m_countof(attachments);
		create_info.pAttachments = attachments;
		create_info.subpassCount = m_countof(subpasses);
		create_info.pSubpasses = subpasses;
		create_info.dependencyCount = m_countof(dependencies);
		create_info.pDependencies = dependencies;
		m_vk_assert(vkCreateRenderPass(vulkan->device.device, &create_info, nullptr, &vulkan->render_passes.swapchain_render_pass));
	}
}

void initialize_vulkan_framebuffers(vulkan *vulkan, vulkan_framebuffers_config framebuffers_config) {
	// shadow map, blur1, blur2
	vulkan->framebuffers.shadow_map_framebuffer_msaa = VK_SAMPLE_COUNT_1_BIT;
	vulkan->framebuffers.shadow_map_framebuffer_width = framebuffers_config.shadow_map_framebuffer_width;
	vulkan->framebuffers.shadow_map_framebuffer_height = framebuffers_config.shadow_map_framebuffer_height;
	for (uint32 i = 0; i < m_countof(vulkan->framebuffers.shadow_map_framebuffers); i += 1) {
		vulkan_framebuffer *framebuffer = &vulkan->framebuffers.shadow_map_framebuffers[i];
		*framebuffer = {};
		framebuffer->color_attachment_count = 1;

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R32G32_SFLOAT;
		image_info.extent = {vulkan->framebuffers.shadow_map_framebuffer_width, vulkan->framebuffers.shadow_map_framebuffer_height, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;
		framebuffer->color_attachment_image_indices[0] = append_vulkan_images(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);

		image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_D32_SFLOAT;
		image_info.extent = {vulkan->framebuffers.shadow_map_framebuffer_width, vulkan->framebuffers.shadow_map_framebuffer_height, 1};
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
		framebuffer->depth_stencil_attachment_image_index = append_vulkan_images(vulkan, image_info, image_view_info, nullptr, 0, 1, 4);

		auto vulkan_images = vulkan->memory_regions.images.images;
		VkImageView image_views[2] = {vulkan_images[framebuffer->color_attachment_image_indices[0]].view,
																	vulkan_images[framebuffer->depth_stencil_attachment_image_index].view};
		VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
		framebuffer_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
		framebuffer_info.attachmentCount = m_countof(image_views);
		framebuffer_info.pAttachments = image_views;
		framebuffer_info.width = vulkan->framebuffers.shadow_map_framebuffer_width;
		framebuffer_info.height = vulkan->framebuffers.shadow_map_framebuffer_height;
		framebuffer_info.layers = 1;
		m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer->framebuffer));
	}
	for (uint32 i = 0; i < m_countof(vulkan->framebuffers.shadow_map_blur_1_framebuffers); i += 1) {
		vulkan_framebuffer *framebuffer = &vulkan->framebuffers.shadow_map_blur_1_framebuffers[i];
		*framebuffer = {};
		framebuffer->color_attachment_count = 1;

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R32G32_SFLOAT;
		image_info.extent = {vulkan->framebuffers.shadow_map_framebuffer_width, vulkan->framebuffers.shadow_map_framebuffer_height, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;
		framebuffer->color_attachment_image_indices[0] = append_vulkan_images(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);

		auto vulkan_images = vulkan->memory_regions.images.images;
		VkImageView image_views[1] = {vulkan_images[framebuffer->color_attachment_image_indices[0]].view};
		VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
		framebuffer_info.renderPass = vulkan->render_passes.shadow_map_render_passes[1];
		framebuffer_info.attachmentCount = m_countof(image_views);
		framebuffer_info.pAttachments = image_views;
		framebuffer_info.width = vulkan->framebuffers.shadow_map_framebuffer_width;
		framebuffer_info.height = vulkan->framebuffers.shadow_map_framebuffer_height;
		framebuffer_info.layers = 1;
		m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer->framebuffer));
	}
	for (uint32 i = 0; i < m_countof(vulkan->framebuffers.shadow_map_blur_2_framebuffers); i += 1) {
		vulkan_framebuffer *framebuffer = &vulkan->framebuffers.shadow_map_blur_2_framebuffers[i];
		*framebuffer = {};
		framebuffer->color_attachment_count = 1;
		framebuffer->color_attachment_image_indices[0] = vulkan->framebuffers.shadow_map_framebuffers[i].color_attachment_image_indices[0];

		auto vulkan_images = vulkan->memory_regions.images.images;
		VkImageView image_views[1] = {vulkan_images[framebuffer->color_attachment_image_indices[0]].view};
		VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
		framebuffer_info.renderPass = vulkan->render_passes.shadow_map_render_passes[2];
		framebuffer_info.attachmentCount = m_countof(image_views);
		framebuffer_info.pAttachments = image_views;
		framebuffer_info.width = vulkan->framebuffers.shadow_map_framebuffer_width;
		framebuffer_info.height = vulkan->framebuffers.shadow_map_framebuffer_height;
		framebuffer_info.layers = 1;
		m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer->framebuffer));
	}
	// color
	vulkan->framebuffers.color_framebuffer_msaa = framebuffers_config.color_framebuffer_msaa;
	vulkan->framebuffers.color_framebuffer_width = framebuffers_config.color_framebuffer_width;
	vulkan->framebuffers.color_framebuffer_height = framebuffers_config.color_framebuffer_height;
	for (uint32 i = 0; i < m_countof(vulkan->framebuffers.color_framebuffers); i += 1) {
		vulkan_framebuffer *framebuffer = &vulkan->framebuffers.color_framebuffers[i];
		framebuffer->color_attachment_count = 1;
		{
			VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = VK_FORMAT_R16G16B16A16_SFLOAT;
			image_info.extent = {vulkan->framebuffers.color_framebuffer_width, vulkan->framebuffers.color_framebuffer_height, 1};
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = framebuffers_config.color_framebuffer_msaa;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			if (framebuffers_config.color_framebuffer_msaa != VK_SAMPLE_COUNT_1_BIT) {
				image_info.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}
			VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = image_info.format;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.layerCount = 1;
			framebuffer->color_attachment_image_indices[0] = append_vulkan_images(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);
		}
		{
			VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = VK_FORMAT_D32_SFLOAT;
			image_info.extent = {vulkan->framebuffers.color_framebuffer_width, vulkan->framebuffers.color_framebuffer_height, 1};
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = framebuffers_config.color_framebuffer_msaa;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			image_info.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = image_info.format;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.layerCount = 1;
			framebuffer->depth_stencil_attachment_image_index = append_vulkan_images(vulkan, image_info, image_view_info, nullptr, 0, 1, 4);
		}
		if (framebuffers_config.color_framebuffer_msaa != VK_SAMPLE_COUNT_1_BIT) {
			VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = VK_FORMAT_R16G16B16A16_SFLOAT;
			image_info.extent = {vulkan->framebuffers.color_framebuffer_width, vulkan->framebuffers.color_framebuffer_height, 1};
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;;
			VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = image_info.format;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.layerCount = 1;
			framebuffer->color_resolve_attachment_image_index = append_vulkan_images(vulkan, image_info, image_view_info, nullptr, 0, 1, 8);
		}
		{
			auto vulkan_images = vulkan->memory_regions.images.images;
			VkImageView image_views[3] = {vulkan_images[framebuffer->color_attachment_image_indices[0]].view, vulkan_images[framebuffer->depth_stencil_attachment_image_index].view, nullptr};
			if (framebuffers_config.color_framebuffer_msaa != VK_SAMPLE_COUNT_1_BIT) {
				image_views[2] = vulkan_images[framebuffer->color_resolve_attachment_image_index].view;
			}
			VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
			framebuffer_info.renderPass = vulkan->render_passes.color_render_pass;
			framebuffer_info.attachmentCount = (framebuffers_config.color_framebuffer_msaa == VK_SAMPLE_COUNT_1_BIT) ? 2 : 3;
			framebuffer_info.pAttachments = image_views;
			framebuffer_info.width = vulkan->framebuffers.color_framebuffer_width;
			framebuffer_info.height = vulkan->framebuffers.color_framebuffer_height;
			framebuffer_info.layers = 1;
			m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &framebuffer->framebuffer));
		}
	}
	// swap chain
	for (uint32 i = 0; i < vulkan_swapchain_image_count; i += 1) {
		VkFramebufferCreateInfo framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
		framebuffer_info.renderPass = vulkan->render_passes.swapchain_render_pass;
		framebuffer_info.attachmentCount = 1;
		framebuffer_info.pAttachments = &vulkan->swapchain.image_views[i];
		framebuffer_info.width = vulkan->swapchain.width;
		framebuffer_info.height = vulkan->swapchain.height;
		framebuffer_info.layers = 1;
		m_vk_assert(vkCreateFramebuffer(vulkan->device.device, &framebuffer_info, nullptr, &vulkan->framebuffers.swapchain_framebuffers[i]));
	}
	// descriptor indices
	for (uint32 i = 0; i < vulkan_buffering_count; i += 1) {
		uint32 blur_1_image_index = vulkan->framebuffers.shadow_map_framebuffers[i].color_attachment_image_indices[0];
		uint32 blur_1_descriptor_index = append_vulkan_image_sampler_2d(vulkan, blur_1_image_index, vulkan->samplers.shadow_map_sampler);
		vulkan->framebuffers.shadow_map_blur_1_descriptor_indices[i] = blur_1_descriptor_index;

		uint32 blur_2_image_index = vulkan->framebuffers.shadow_map_blur_1_framebuffers[i].color_attachment_image_indices[0];
		uint32 blur_2_descriptor_index = append_vulkan_image_sampler_2d(vulkan, blur_2_image_index, vulkan->samplers.shadow_map_sampler);
		vulkan->framebuffers.shadow_map_blur_2_descriptor_indices[i] = blur_2_descriptor_index;

		uint32 color_image_index = vulkan->framebuffers.color_framebuffers[i].color_attachment_image_indices[0];
		if (framebuffers_config.color_framebuffer_msaa != VK_SAMPLE_COUNT_1_BIT) {
			color_image_index = vulkan->framebuffers.color_framebuffers[i].color_resolve_attachment_image_index;
		}
		uint32 color_descriptor_index = append_vulkan_image_sampler_2d(vulkan, color_image_index, vulkan->samplers.color_framebuffer_sampler);
		vulkan->framebuffers.color_descriptor_indices[i] = color_descriptor_index;
	}
}

void initialize_vulkan_pipelines(vulkan *vulkan, VkSampleCountFlagBits color_framebuffer_sample_count) {
	auto shader_module_from_file = [vulkan](const char *file_path) {
		VkShaderModule shader_module = {};
		file_mapping file_mapping = {};
		m_assert(open_file_mapping(file_path, &file_mapping, true), "");
		VkShaderModuleCreateInfo info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
		info.codeSize = file_mapping.size;
		info.pCode = (const uint32 *)file_mapping.ptr;
		m_vk_assert(vkCreateShaderModule(vulkan->device.device, &info, nullptr, &shader_module));
		close_file_mapping(file_mapping);
		return shader_module;
	};
	{ // pipeline layout
		VkDescriptorSetLayout layouts[] = {vulkan->descriptor_sets.uniform_buffer_layout, vulkan->descriptor_sets.image_sampler_2d_layout, vulkan->descriptor_sets.image_sampler_cube_layout};
		VkPushConstantRange push_const_range = {VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, 128};
		VkPipelineLayoutCreateInfo pipeline_layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		pipeline_layout_info.setLayoutCount = m_countof(layouts);
		pipeline_layout_info.pSetLayouts = layouts;
		pipeline_layout_info.pushConstantRangeCount = 1;
		pipeline_layout_info.pPushConstantRanges = &push_const_range;
		m_vk_assert(vkCreatePipelineLayout(vulkan->device.device, &pipeline_layout_info, nullptr, &vulkan->pipelines.pipeline_layout));
	}
	{ // model
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/model.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/model.frag.spv");
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
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

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
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.model_pipeline));
	}
	{ // model wireframe
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/model_wireframe.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/model_wireframe.frag.spv");
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
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

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
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.model_wireframe_pipeline));
	}
	{ // model shadow map
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/model_shadow_map.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/model_shadow_map.frag.spv");
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
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vulkan->pipelines.model_shadow_map_pipeline));
	}
	{ // shadow map gaussian blur
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/gaussian_blur.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/gaussian_blur.frag.spv");
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
		shader_stages[0].module = shader_module_from_file("glsl/skybox.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/skybox.frag.spv");
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
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

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
		shader_stages[0].module = shader_module_from_file("glsl/terrain.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/terrain.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[] = {0, 16, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[] = {{0, 0, VK_FORMAT_R32G32_SFLOAT, 0}, {1, 0, VK_FORMAT_R32G32_SFLOAT, 8}};
		
		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

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
	{ // terrain brush
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/terrain_brush.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/terrain_brush.frag.spv");
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
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_FALSE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
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
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.terrain_brush_pipeline));
	}
	{ // text
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/text.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/text.frag.spv");
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
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

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
	{ // collision shape
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/collision_shape.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/collision_shape.frag.spv");
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
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

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
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.collision_shape_pipeline));
	}
	{ // reference grid
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/reference_grid.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/reference_grid.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[] = {0, 12, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[] = {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}};

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
		multisample_state.rasterizationSamples = color_framebuffer_sample_count;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_TRUE;
		depth_stencil_state.depthWriteEnable = VK_TRUE;
		depth_stencil_state.depthCompareOp = VK_COMPARE_OP_GREATER;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
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
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.reference_grid_pipeline));
	}
	{ // swapchain
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/swapchain.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/swapchain.frag.spv");
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
		create_info.renderPass = vulkan->render_passes.swapchain_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.swapchain_pipeline));
	}
	{ // imgui
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("glsl/imgui.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("glsl/imgui.frag.spv");
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
		create_info.renderPass = vulkan->render_passes.swapchain_render_pass;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan->device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan->pipelines.imgui_pipeline));
	}
}

void initialize_vulkan(vulkan *vulkan, vulkan_config *config, window *window) {
	*vulkan = {};
	
	initialize_vulkan_device(vulkan, config->validation_layer);
	initialize_vulkan_swapchain(vulkan, window, config->vsync);
	initialize_vulkan_memory_regions(vulkan, config->memory_config);
	initialize_vulkan_cmd_buffers(vulkan);
	initialize_vulkan_syncs(vulkan);
	initialize_vulkan_samplers(vulkan);
	initialize_vulkan_descriptors(vulkan, config->descriptor_set_config);
	initialize_vulkan_render_passes(vulkan, config->framebuffers_config.color_framebuffer_msaa);
	initialize_vulkan_framebuffers(vulkan, config->framebuffers_config);
	initialize_vulkan_pipelines(vulkan, config->framebuffers_config.color_framebuffer_msaa);

	vulkan->buffering_index = 0;
	vulkan->swapchain_image_index = 0;

	float width_1 = (float)vulkan->swapchain.width;
	float height_1 = (float)vulkan->swapchain.height;
	float width_2 = (float)vulkan->framebuffers.color_framebuffer_width;
	float height_2 = (float)vulkan->framebuffers.color_framebuffer_height;
	vulkan->swapchain_framebuffer_region = fit_rect_into_rect(width_1, height_1, width_2, height_2);
}
