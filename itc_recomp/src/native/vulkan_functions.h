// Vulkan, loaded at runtime through SDL (see docs/native-renderer.md): no
// Vulkan library at build time, only the headers.

#pragma once

// Through the SDK's Vulkan header, so every Vulkan include in the build
// agrees on its defines (no prototypes, beta extensions, the Wayland and XCB
// platforms): the shader translator's headers use the C++ bindings.
#include <rex/ui/vulkan/api.h>

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
  X(vkEnumerateDeviceExtensionProperties)

#define NATIVE_VK_DEVICE_FUNCTIONS(X) \
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
  X(vkCmdDrawIndexed)                  \
  X(vkCmdSetViewport)                  \
  X(vkCmdSetScissor)                   \
  X(vkCmdBlitImage)                    \
  X(vkCmdCopyBufferToImage)            \
  X(vkResetCommandPool)                \
  X(vkResetDescriptorPool)

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
