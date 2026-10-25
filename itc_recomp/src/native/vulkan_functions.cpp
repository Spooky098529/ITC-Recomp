#include "vulkan_functions.h"

#include "base/log.h"

namespace native::vk {

#define NATIVE_VK_DEFINE(name) PFN_##name name = nullptr;
NATIVE_VK_GLOBAL_FUNCTIONS(NATIVE_VK_DEFINE)
NATIVE_VK_INSTANCE_FUNCTIONS(NATIVE_VK_DEFINE)
NATIVE_VK_DEVICE_FUNCTIONS(NATIVE_VK_DEFINE)
#undef NATIVE_VK_DEFINE

namespace {
PFN_vkGetInstanceProcAddr g_get_instance_proc_addr = nullptr;
}  // namespace

#define NATIVE_VK_LOAD(name, getter, handle)                                   \
  name = reinterpret_cast<PFN_##name>(getter(handle, #name));                  \
  if (!name) {                                                                 \
    ITC_LOG_ERROR("native renderer: Vulkan function {} is missing", #name);    \
    ok = false;                                                                \
  }

bool LoadGlobal(PFN_vkGetInstanceProcAddr get_instance_proc_addr) {
  g_get_instance_proc_addr = get_instance_proc_addr;
  bool ok = true;
#define X(name) NATIVE_VK_LOAD(name, g_get_instance_proc_addr, VK_NULL_HANDLE)
  NATIVE_VK_GLOBAL_FUNCTIONS(X)
#undef X
  return ok;
}

bool LoadInstance(VkInstance instance) {
  bool ok = true;
#define X(name) NATIVE_VK_LOAD(name, g_get_instance_proc_addr, instance)
  NATIVE_VK_INSTANCE_FUNCTIONS(X)
#undef X
  return ok;
}

bool LoadDevice(VkDevice device) {
  bool ok = true;
#define X(name) NATIVE_VK_LOAD(name, vkGetDeviceProcAddr, device)
  NATIVE_VK_DEVICE_FUNCTIONS(X)
#undef X
  return ok;
}

}  // namespace native::vk
