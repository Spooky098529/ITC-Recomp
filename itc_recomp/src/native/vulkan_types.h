// The renderer's own state values (src/gpu/types.h) as Vulkan's.

#pragma once

#include "gpu/types.h"
#include "native/vulkan_functions.h"

namespace vk_types {

inline VkCompareOp CompareOp(gpu::CompareOp op) {
  // The same order.
  return VkCompareOp(op);
}

inline VkBlendFactor BlendFactor(gpu::BlendFactor factor) {
  using F = gpu::BlendFactor;
  switch (factor) {
    case F::kZero: return VK_BLEND_FACTOR_ZERO;
    case F::kOne: return VK_BLEND_FACTOR_ONE;
    case F::kSrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
    case F::kOneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case F::kSrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
    case F::kOneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case F::kDstColor: return VK_BLEND_FACTOR_DST_COLOR;
    case F::kOneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case F::kDstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
    case F::kOneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case F::kConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case F::kOneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case F::kConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case F::kOneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case F::kSrcAlphaSaturate: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
  }
  return VK_BLEND_FACTOR_ZERO;
}

inline VkBlendOp BlendOp(gpu::BlendOp op) {
  using O = gpu::BlendOp;
  switch (op) {
    case O::kAdd: return VK_BLEND_OP_ADD;
    case O::kSubtract: return VK_BLEND_OP_SUBTRACT;
    case O::kReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
    case O::kMin: return VK_BLEND_OP_MIN;
    case O::kMax: return VK_BLEND_OP_MAX;
  }
  return VK_BLEND_OP_ADD;
}

inline VkCullModeFlags CullMode(gpu::CullMode mode) {
  // The same bits.
  return VkCullModeFlags(mode);
}

inline VkFilter Filter(gpu::Filter filter) {
  return filter == gpu::Filter::kLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
}

inline VkSamplerMipmapMode MipMode(gpu::MipMode mode) {
  return mode == gpu::MipMode::kLinear ? VK_SAMPLER_MIPMAP_MODE_LINEAR
                                       : VK_SAMPLER_MIPMAP_MODE_NEAREST;
}

inline VkSamplerAddressMode AddressMode(gpu::AddressMode mode) {
  using A = gpu::AddressMode;
  switch (mode) {
    case A::kRepeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case A::kMirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case A::kClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case A::kClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case A::kMirrorClampToEdge: return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
  }
  return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

inline VkBorderColor BorderColor(gpu::BorderColor color) {
  return color == gpu::BorderColor::kOpaqueWhite ? VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE
                                                 : VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
}

}  // namespace vk_types
