// The game's render command list, as far as interpolation needs it.
//
// Each tick the game records render commands into one of two buffers (see
// "Render command recording" in docs/functions.md). Commands are 16-byte
// slots: a header [id, arg1, arg2, unused] followed by payload slots. The
// render thread replays the list once per drawn frame.
//
// The list is double-buffered: while a tick records into one buffer, the
// render thread replays the other, which holds the previous tick's list.
// What is drawn is therefore one tick behind the game logic.
//
// Command 7 uploads shader constants, which carry each object's transform
// matrices: arg1 is the number of vec4 registers, arg2 packs the shader
// stage (top byte), constant type, start register and register count (low
// byte), and the payload holds the values.

#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace interpolation {

constexpr uint32_t kSetShaderConstantsCommand = 7;
constexpr uint32_t kCommandSlotSize = 16;

enum class ShaderStage : uint8_t { kVertex = 0, kPixel = 1 };

// The type that marks dirty 4-register blocks on upload, as float
// constants do. Every transform upload seen in a race uses it.
constexpr uint8_t kFloatConstantType = 1;

struct ShaderConstantUpload {
  ShaderStage stage;
  uint8_t type;
  uint8_t start_register;
  uint32_t register_count;

  // Identifies uploads that can correspond between ticks.
  uint64_t Signature() const;
};

ShaderConstantUpload DecodeShaderConstantUpload(uint32_t register_count, uint32_t packed);

// Reads the shader constant upload whose header is at `header`, if the slot
// holds one.
std::optional<ShaderConstantUpload> ReadShaderConstantHeader(uint8_t* base, uint32_t header);

// The guest address the next recorded command will be written to.
uint32_t CurrentRecordingWritePointer(uint8_t* base);

struct RecordedUpload {
  uint32_t payload;  // guest address of the constant data
  ShaderConstantUpload upload;
};

// Finds the shader constant uploads in the list the render thread replays
// (recorded on the previous tick), in recording order. `scene` owns the
// command list. Call after a tick, before its frames are drawn.
std::vector<RecordedUpload> FindReplayedShaderConstantUploads(uint8_t* base, uint32_t scene);

}  // namespace interpolation
