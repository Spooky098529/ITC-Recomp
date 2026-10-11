#include "command_list.h"

#include "guest.h"

namespace interpolation {
namespace {

// Scene object (Scene::Update, sub_820988C8).
constexpr uint32_t kSceneCommandListOffset = 8;

// Command list object. sub_8258C9C0 flips the buffer index each tick;
// sub_8258C998 then saves the previous recording's slot count and starts a
// new recording at the start of buffers[index].
constexpr uint32_t kCommandListIndexOffset = 4;
constexpr uint32_t kCommandListPreviousUsedSlotsOffset = 16;
constexpr uint32_t kCommandListBuffersOffset = 24;  // two buffer pointers
constexpr uint32_t kCommandListWritePointerOffset = 44;

// The list recorders write to (sub_82595550 and others load it).
constexpr uint32_t kCurrentCommandListAddress = 0x8296B918;

constexpr uint32_t kMaxRegisters = 256;

// Plausibility checks for a command 7 header found by scanning, since
// payload data elsewhere in the buffer could contain the value 7.
bool LooksLikeShaderConstantHeader(uint32_t id, uint32_t register_count, uint32_t packed) {
  if (id != kSetShaderConstantsCommand) {
    return false;
  }
  if (register_count == 0 || register_count > kMaxRegisters) {
    return false;
  }
  ShaderConstantUpload upload = DecodeShaderConstantUpload(register_count, packed);
  if (upload.stage != ShaderStage::kVertex && upload.stage != ShaderStage::kPixel) {
    return false;
  }
  if (upload.type > 3) {
    return false;
  }
  if ((packed & 0xFF) != (register_count & 0xFF)) {
    return false;
  }
  return upload.start_register + register_count <= kMaxRegisters;
}

}  // namespace

std::optional<ShaderConstantUpload> ReadShaderConstantHeader(uint8_t* base, uint32_t header) {
  uint32_t id = guest::LoadU32(base, header);
  uint32_t register_count = guest::LoadU32(base, header + 4);
  uint32_t packed = guest::LoadU32(base, header + 8);
  if (!LooksLikeShaderConstantHeader(id, register_count, packed)) {
    return std::nullopt;
  }
  return DecodeShaderConstantUpload(register_count, packed);
}

uint32_t CurrentRecordingWritePointer(uint8_t* base) {
  uint32_t list = guest::LoadU32(base, kCurrentCommandListAddress);
  return list ? guest::LoadU32(base, list + kCommandListWritePointerOffset) : 0;
}

uint64_t ShaderConstantUpload::Signature() const {
  return (uint64_t(stage) << 48) | (uint64_t(type) << 40) | (uint64_t(start_register) << 32) |
         register_count;
}

ShaderConstantUpload DecodeShaderConstantUpload(uint32_t register_count, uint32_t packed) {
  return {
      .stage = ShaderStage(uint8_t(packed >> 24)),
      .type = uint8_t(packed >> 16),
      .start_register = uint8_t(packed >> 8),
      .register_count = register_count,
  };
}

std::vector<RecordedUpload> FindReplayedShaderConstantUploads(uint8_t* base, uint32_t scene) {
  std::vector<RecordedUpload> uploads;
  uint32_t list = scene + kSceneCommandListOffset;
  uint32_t replayed_index = guest::LoadU32(base, list + kCommandListIndexOffset) ^ 1;
  uint32_t start = guest::LoadU32(base, list + kCommandListBuffersOffset + 4 * replayed_index);
  uint32_t used_slots = guest::LoadU32(base, list + kCommandListPreviousUsedSlotsOffset);
  for (uint32_t slot = 0; slot < used_slots; slot++) {
    uint32_t header = start + slot * kCommandSlotSize;
    std::optional<ShaderConstantUpload> upload = ReadShaderConstantHeader(base, header);
    if (!upload) {
      continue;
    }
    uploads.push_back({header + kCommandSlotSize, *upload});
    slot += upload->register_count;  // skip the payload
  }
  return uploads;
}

}  // namespace interpolation
