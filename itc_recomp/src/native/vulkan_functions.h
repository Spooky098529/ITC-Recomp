// Vulkan, loaded at runtime through SDL (see docs/native-renderer.md): no
// Vulkan library at build time, only the headers.

#pragma once

// Our own Vulkan-Headers (itc_vulkan_headers), without prototypes: every
// function is loaded below. The surface comes from SDL, so no platform's
// defines are needed.
#ifndef VK_NO_PROTOTYPES
#define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>

// Functions by the level they're loaded at.
#define NATIVE_VK_GLOBAL_FUNCTIONS(X) \
  X(vkCreateInstance)                 \
  X(vkEnumerateInstanceVersion)

#define NATIVE_VK_INSTANCE_FUNCTIONS(X)        \
  X(vkDestroyInstance)                         \
  X(vkEnumeratePhysicalDevices)                \
  X(vkGetPhysicalDeviceProperties)             \
  X(vkGetPhysicalDeviceQueueFamilyProperties)  \
  X(vkGetPhysicalDeviceSurfaceSupportKHR)      \
  X(vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
  X(vkGetPhysicalDeviceSurfaceFormatsKHR)      \
  X(vkGetPhysicalDeviceSurfacePresentModesKHR) \
  X(vkCreateDevice)                            \
  X(vkGetDeviceProcAddr)                       \
  X(vkDestroySurfaceKHR)                       \
  X(vkGetPhysicalDeviceFeatures2)              \
  X(vkGetPhysicalDeviceProperties2)            \
  X(vkGetPhysicalDeviceMemoryProperties)       \
  X(vkGetPhysicalDeviceFormatProperties)       \
  X(vkEnumerateDeviceExtensionProperties)

#define NATIVE_VK_DEVICE_FUNCTIONS(X) \
  X(vkCreatePipelineCache)            \
  X(vkDestroyPipelineCache)           \
  X(vkGetPipelineCacheData)           \
  X(vkDestroyDevice)                  \
  X(vkGetDeviceQueue)                 \
  X(vkDeviceWaitIdle)                 \
  X(vkCreateSwapchainKHR)             \
  X(vkDestroySwapchainKHR)            \
  X(vkGetSwapchainImagesKHR)          \
  X(vkAcquireNextImageKHR)            \
  X(vkQueuePresentKHR)                \
  X(vkQueueSubmit)                    \
  X(vkCreateCommandPool)              \
  X(vkDestroyCommandPool)             \
  X(vkAllocateCommandBuffers)         \
  X(vkResetCommandBuffer)             \
  X(vkBeginCommandBuffer)             \
  X(vkEndCommandBuffer)               \
  X(vkCmdPipelineBarrier)             \
  X(vkCmdClearColorImage)             \
  X(vkCmdClearDepthStencilImage)      \
  X(vkCmdResolveImage)                \
  X(vkCmdPushConstants)               \
  X(vkCmdDraw)                        \
  X(vkCreateFence)                    \
  X(vkDestroyFence)                   \
  X(vkWaitForFences)                  \
  X(vkResetFences)                    \
  X(vkCreateSemaphore)                \
  X(vkDestroySemaphore)                \
  X(vkCreateBuffer)                    \
  X(vkDestroyBuffer)                   \
  X(vkGetBufferMemoryRequirements)     \
  X(vkBindBufferMemory)                \
  X(vkCreateImage)                     \
  X(vkDestroyImage)                    \
  X(vkGetImageMemoryRequirements)      \
  X(vkBindImageMemory)                 \
  X(vkCreateImageView)                 \
  X(vkDestroyImageView)                \
  X(vkAllocateMemory)                  \
  X(vkFreeMemory)                      \
  X(vkMapMemory)                       \
  X(vkCreateSampler)                   \
  X(vkCreateDescriptorSetLayout)       \
  X(vkCreatePipelineLayout)            \
  X(vkCreateDescriptorPool)            \
  X(vkAllocateDescriptorSets)          \
  X(vkUpdateDescriptorSets)            \
  X(vkCreateShaderModule)              \
  X(vkCreateGraphicsPipelines)         \
  X(vkCmdBeginRendering)               \
  X(vkCmdEndRendering)                 \
  X(vkCmdBindPipeline)                 \
  X(vkCmdBindDescriptorSets)           \
  X(vkCmdBindIndexBuffer)              \
  X(vkCmdBindVertexBuffers)            \
  X(vkCmdDrawIndexed)                  \
  X(vkCmdSetViewport)                  \
  X(vkCmdSetScissor)                   \
  X(vkCmdSetDepthBias)                 \
  X(vkCmdSetBlendConstants)            \
  X(vkCmdBlitImage)                    \
  X(vkCmdCopyBuffer)                   \
  X(vkCmdCopyImage)                    \
  X(vkCmdCopyBufferToImage)            \
  X(vkCmdCopyImageToBuffer)            \
  X(vkQueueWaitIdle)                   \
  X(vkResetCommandPool)                \
  X(vkResetDescriptorPool)             \
  X(vkCreateComputePipelines)          \
  X(vkDestroyShaderModule)             \
  X(vkCmdDispatch)                     \
  X(vkCmdUpdateBuffer)

namespace native::vk {

#define NATIVE_VK_DECLARE(name) extern PFN_##name name;
NATIVE_VK_GLOBAL_FUNCTIONS(NATIVE_VK_DECLARE)
NATIVE_VK_INSTANCE_FUNCTIONS(NATIVE_VK_DECLARE)
NATIVE_VK_DEVICE_FUNCTIONS(NATIVE_VK_DECLARE)
#undef NATIVE_VK_DECLARE

// Each returns false (and logs) if a function is missing.
bool LoadGlobal(PFN_vkGetInstanceProcAddr get_instance_proc_addr);
bool LoadInstance(VkInstance instance);
bool LoadDevice(VkDevice device);

}  // namespace native::vk
