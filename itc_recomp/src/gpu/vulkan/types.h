// The renderer's own state values (src/gpu/types.h) as Vulkan's.

#pragma once

#include "gpu/types.h"
#include "gpu/vulkan/functions.h"

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

inline VkFormat Format(gpu::Format format) {
  using F = gpu::Format;
  switch (format) {
    case F::kUndefined: return VK_FORMAT_UNDEFINED;
    case F::kR8Unorm: return VK_FORMAT_R8_UNORM;
    case F::kR8G8Unorm: return VK_FORMAT_R8G8_UNORM;
    case F::kR8G8B8A8Unorm: return VK_FORMAT_R8G8B8A8_UNORM;
    case F::kA2B10G10R10Unorm: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    case F::kR16Unorm: return VK_FORMAT_R16_UNORM;
    case F::kR16G16Unorm: return VK_FORMAT_R16G16_UNORM;
    case F::kR16G16B16A16Unorm: return VK_FORMAT_R16G16B16A16_UNORM;
    case F::kR16Float: return VK_FORMAT_R16_SFLOAT;
    case F::kR16G16Float: return VK_FORMAT_R16G16_SFLOAT;
    case F::kR16G16B16A16Float: return VK_FORMAT_R16G16B16A16_SFLOAT;
    case F::kR32Float: return VK_FORMAT_R32_SFLOAT;
    case F::kR32G32Float: return VK_FORMAT_R32G32_SFLOAT;
    case F::kR32G32B32A32Float: return VK_FORMAT_R32G32B32A32_SFLOAT;
    case F::kR32Uint: return VK_FORMAT_R32_UINT;
    case F::kR32G32Uint: return VK_FORMAT_R32G32_UINT;
    case F::kR32G32B32Uint: return VK_FORMAT_R32G32B32_UINT;
    case F::kR32G32B32A32Uint: return VK_FORMAT_R32G32B32A32_UINT;
    case F::kD32FloatS8Uint: return VK_FORMAT_D32_SFLOAT_S8_UINT;
    case F::kBC1: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case F::kBC2: return VK_FORMAT_BC2_UNORM_BLOCK;
    case F::kBC3: return VK_FORMAT_BC3_UNORM_BLOCK;
    case F::kBC5: return VK_FORMAT_BC5_UNORM_BLOCK;
  }
  return VK_FORMAT_UNDEFINED;
}

// The renderer's format for a Vulkan one (kUndefined for one it lacks): for
// the code still holding Vulkan's formats while it moves.
inline gpu::Format FormatFrom(VkFormat format) {
  for (uint32_t i = 0; i <= uint32_t(gpu::Format::kBC5); i++) {
    if (Format(gpu::Format(i)) == format) {
      return gpu::Format(i);
    }
  }
  return gpu::Format::kUndefined;
}

inline VkImageLayout Layout(gpu::Usage usage) {
  using U = gpu::Usage;
  switch (usage) {
    case U::kUndefined: return VK_IMAGE_LAYOUT_UNDEFINED;
    case U::kSampled: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case U::kDepthSampled: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case U::kColorTarget: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case U::kDepthTarget: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case U::kCopySource: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case U::kCopyDest: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case U::kStorage: return VK_IMAGE_LAYOUT_GENERAL;
  }
  return VK_IMAGE_LAYOUT_UNDEFINED;
}

}  // namespace vk_types
