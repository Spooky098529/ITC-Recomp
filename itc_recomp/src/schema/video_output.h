// The views of docs/functions/video-output.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/video-output.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// gpu; measure: VideoOutput
struct VideoOutput : View {
  using View::View;
  static constexpr uint32_t kSize = 308;
  static constexpr uint32_t kVtable = 0x8206CAF8;
  // 0x8206CAF8 (also where the measure files CreateDevice's store through +140)
  uint32_t vtable() const { return guest::LoadU32(address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 1280
  uint32_t params_width() const { return guest::LoadU32(address + 16); }
  void set_params_width(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t params_width_address() const { return address + 16; }
  // 720
  uint32_t params_height() const { return guest::LoadU32(address + 20); }
  void set_params_height(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t params_height_address() const { return address + 20; }
  // 0x18280186
  uint32_t params_render_target_format() const { return guest::LoadU32(address + 24); }
  void set_params_render_target_format(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t params_render_target_format_address() const { return address + 24; }
  // 0
  uint32_t params_unknown_12() const { return guest::LoadU32(address + 28); }
  void set_params_unknown_12(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t params_unknown_12_address() const { return address + 28; }
  // 0; passed to both surfaces
  uint32_t params_surface_argument() const { return guest::LoadU32(address + 32); }
  void set_params_surface_argument(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t params_surface_argument_address() const { return address + 32; }
  // 1
  uint32_t params_unknown_24() const { return guest::LoadU32(address + 40); }
  void set_params_unknown_24(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t params_unknown_24_address() const { return address + 40; }
  // non-zero for a depth surface (1)
  uint32_t params_depth_surface() const { return guest::LoadU32(address + 52); }
  void set_params_depth_surface(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t params_depth_surface_address() const { return address + 52; }
  // 0x1A220197
  uint32_t params_depth_format() const { return guest::LoadU32(address + 56); }
  void set_params_depth_format(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t params_depth_format_address() const { return address + 56; }
  // 0x80000000
  uint32_t params_unknown_52() const { return guest::LoadU32(address + 68); }
  void set_params_unknown_52(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t params_unknown_52_address() const { return address + 68; }
  // 0 (memset)
  uint32_t params_skip_render_target() const { return guest::LoadU32(address + 72); }
  void set_params_skip_render_target(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t params_skip_render_target_address() const { return address + 72; }
  // 1: no front buffer from the library
  uint32_t params_no_front_buffer() const { return guest::LoadU32(address + 76); }
  void set_params_no_front_buffer(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t params_no_front_buffer_address() const { return address + 76; }
  // 0 (memset)
  uint32_t params_front_buffer_format() const { return guest::LoadU32(address + 80); }
  void set_params_front_buffer_format(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t params_front_buffer_format_address() const { return address + 80; }
  // 0 (memset)
  uint32_t params_display_format() const { return guest::LoadU32(address + 84); }
  void set_params_display_format(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t params_display_format_address() const { return address + 84; }
  // 0; the +72 block Device::SetUpRing reads
  uint32_t params_ring_unknown_0() const { return guest::LoadU32(address + 88); }
  void set_params_ring_unknown_0(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t params_ring_unknown_0_address() const { return address + 88; }
  // 65536
  uint32_t params_ring_size() const { return guest::LoadU32(address + 92); }
  void set_params_ring_size(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t params_ring_size_address() const { return address + 92; }
  // 64 KB of physical memory
  uint32_t params_ring() const { return guest::LoadU32(address + 96); }
  void set_params_ring(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t params_ring_address() const { return address + 96; }
  // 1048576
  uint32_t params_command_buffer_size() const { return guest::LoadU32(address + 100); }
  void set_params_command_buffer_size(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t params_command_buffer_size_address() const { return address + 100; }
  // 1 MB of physical memory
  uint32_t params_command_buffer() const { return guest::LoadU32(address + 104); }
  void set_params_command_buffer(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t params_command_buffer_address() const { return address + 104; }
  // 256
  uint32_t params_segments() const { return guest::LoadU32(address + 108); }
  void set_params_segments(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t params_segments_address() const { return address + 108; }
  // made by VideoOutput::SetUpDisplay (CreateDevice), 0 on failure
  template <typename T = Device>
  T device() const { return T(guest::LoadU32(address + 140)); }
  void set_device(const View& v) const { guest::StoreU32(address + 140, v.address); }
  void set_device(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t device_address() const { return address + 140; }
  // 0 at construction
  uint32_t unknown_144() const { return guest::LoadU32(address + 144); }
  void set_unknown_144(uint32_t v) const { guest::StoreU32(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  // 0 at construction
  uint32_t unknown_148() const { return guest::LoadU32(address + 148); }
  void set_unknown_148(uint32_t v) const { guest::StoreU32(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  // where XGetVideoMode's answer went; never read (zeroed by the set-up)
  uint32_t video_mode_address() const { return address + 152; }
  // the safe area: 5% of the width (64)
  uint32_t safe_x() const { return guest::LoadU32(address + 208); }
  void set_safe_x(uint32_t v) const { guest::StoreU32(address + 208, v); }
  uint32_t safe_x_address() const { return address + 208; }
  // 5% of the height (36)
  uint32_t safe_y() const { return guest::LoadU32(address + 212); }
  void set_safe_y(uint32_t v) const { guest::StoreU32(address + 212, v); }
  uint32_t safe_y_address() const { return address + 212; }
  // 90% of the width (1152)
  uint32_t safe_width() const { return guest::LoadU32(address + 216); }
  void set_safe_width(uint32_t v) const { guest::StoreU32(address + 216, v); }
  uint32_t safe_width_address() const { return address + 216; }
  // 90% of the height (648)
  uint32_t safe_height() const { return guest::LoadU32(address + 220); }
  void set_safe_height(uint32_t v) const { guest::StoreU32(address + 220, v); }
  uint32_t safe_height_address() const { return address + 220; }
  // the game's Semaphore of count 1; a host std::binary_semaphore in its place since 2026-10-24, these bytes no longer written
  uint32_t device_lock_address() const { return address + 224; }
  // 0 at construction
  uint8_t unknown_264() const { return guest::LoadU8(address + 264); }
  void set_unknown_264(uint8_t v) const { guest::StoreU8(address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  // 0 at construction
  uint8_t unknown_265() const { return guest::LoadU8(address + 265); }
  void set_unknown_265(uint8_t v) const { guest::StoreU8(address + 265, v); }
  uint32_t unknown_265_address() const { return address + 265; }
  // 0 at construction
  uint8_t unknown_266() const { return guest::LoadU8(address + 266); }
  void set_unknown_266(uint8_t v) const { guest::StoreU8(address + 266, v); }
  uint32_t unknown_266_address() const { return address + 266; }
  // 255 at construction
  uint8_t unknown_267() const { return guest::LoadU8(address + 267); }
  void set_unknown_267(uint8_t v) const { guest::StoreU8(address + 267, v); }
  uint32_t unknown_267_address() const { return address + 267; }
  // 0 or 1, flipped by VideoOutput::Present
  uint32_t front_buffer_index() const { return guest::LoadU32(address + 268); }
  void set_front_buffer_index(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t front_buffer_index_address() const { return address + 268; }
  // the front buffers' textures (Device::CreateTexture, format 0x28280106), indexed by front_buffer_index
  uint32_t front_buffer_0() const { return guest::LoadU32(address + 272); }
  void set_front_buffer_0(uint32_t v) const { guest::StoreU32(address + 272, v); }
  uint32_t front_buffer_0_address() const { return address + 272; }
  uint32_t front_buffer_1() const { return guest::LoadU32(address + 276); }
  void set_front_buffer_1(uint32_t v) const { guest::StoreU32(address + 276, v); }
  uint32_t front_buffer_1_address() const { return address + 276; }
  // 0 at construction
  uint32_t unknown_280() const { return guest::LoadU32(address + 280); }
  void set_unknown_280(uint32_t v) const { guest::StoreU32(address + 280, v); }
  uint32_t unknown_280_address() const { return address + 280; }
  // 0 at construction
  uint32_t unknown_284() const { return guest::LoadU32(address + 284); }
  void set_unknown_284(uint32_t v) const { guest::StoreU32(address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
  // 0 at construction
  uint32_t unknown_288() const { return guest::LoadU32(address + 288); }
  void set_unknown_288(uint32_t v) const { guest::StoreU32(address + 288, v); }
  uint32_t unknown_288_address() const { return address + 288; }
  // 0 at construction
  uint8_t unknown_292() const { return guest::LoadU8(address + 292); }
  void set_unknown_292(uint8_t v) const { guest::StoreU8(address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
  // read by the camera passes and Viewport::Init
  uint32_t display_width() const { return guest::LoadU32(address + 296); }
  void set_display_width(uint32_t v) const { guest::StoreU32(address + 296, v); }
  uint32_t display_width_address() const { return address + 296; }
  uint32_t display_height() const { return guest::LoadU32(address + 300); }
  void set_display_height(uint32_t v) const { guest::StoreU32(address + 300, v); }
  uint32_t display_height_address() const { return address + 300; }
  // 1 for 0 or 1, 2, 3 for 4, 0 for 0x80000000
  uint32_t vblank_interval() const { return guest::LoadU32(address + 304); }
  void set_vblank_interval(uint32_t v) const { guest::StoreU32(address + 304, v); }
  uint32_t vblank_interval_address() const { return address + 304; }
};

// embedded; measure: Device::SetUpDisplay.r4, Device::SetUpDisplay.r4@+72
struct VideoPresentParameters : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  uint32_t width() const { return guest::LoadU32(address + 0); }
  void set_width(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t width_address() const { return address + 0; }
  // (SetUpRing: the ring's size)
  uint32_t height() const { return guest::LoadU32(address + 4); }
  void set_height(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t height_address() const { return address + 4; }
  // (SetUpRing: the ring)
  uint32_t render_target_format() const { return guest::LoadU32(address + 8); }
  void set_render_target_format(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t render_target_format_address() const { return address + 8; }
  // (SetUpRing: the command buffer's size)
  uint32_t unknown_12() const { return guest::LoadU32(address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // passed to both surfaces (SetUpRing: the command buffer)
  uint32_t surface_argument() const { return guest::LoadU32(address + 16); }
  void set_surface_argument(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t surface_argument_address() const { return address + 16; }
  // (SetUpRing: the segments)
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // non-zero for a depth surface
  uint32_t depth_surface() const { return guest::LoadU32(address + 36); }
  void set_depth_surface(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t depth_surface_address() const { return address + 36; }
  uint32_t depth_format() const { return guest::LoadU32(address + 40); }
  void set_depth_format(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t depth_format_address() const { return address + 40; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  // non-zero to skip the render target
  uint32_t skip_render_target() const { return guest::LoadU32(address + 56); }
  void set_skip_render_target(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t skip_render_target_address() const { return address + 56; }
  // non-zero to skip the front buffer (the game sets it)
  uint32_t no_front_buffer() const { return guest::LoadU32(address + 60); }
  void set_no_front_buffer(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t no_front_buffer_address() const { return address + 60; }
  uint32_t front_buffer_format() const { return guest::LoadU32(address + 64); }
  void set_front_buffer_format(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t front_buffer_format_address() const { return address + 64; }
  uint32_t display_format() const { return guest::LoadU32(address + 68); }
  void set_display_format(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t display_format_address() const { return address + 68; }
  uint32_t ring_unknown_0() const { return guest::LoadU32(address + 72); }
  void set_ring_unknown_0(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t ring_unknown_0_address() const { return address + 72; }
  uint32_t ring_size() const { return guest::LoadU32(address + 76); }
  void set_ring_size(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t ring_size_address() const { return address + 76; }
  uint32_t ring() const { return guest::LoadU32(address + 80); }
  void set_ring(uint32_t v) const { guest::StoreU32(address + 80, v); }
  uint32_t ring_address() const { return address + 80; }
  uint32_t command_buffer_size() const { return guest::LoadU32(address + 84); }
  void set_command_buffer_size(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t command_buffer_size_address() const { return address + 84; }
  uint32_t command_buffer() const { return guest::LoadU32(address + 88); }
  void set_command_buffer(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t command_buffer_address() const { return address + 88; }
  uint32_t segments() const { return guest::LoadU32(address + 92); }
  void set_segments(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t segments_address() const { return address + 92; }
  // the scaler's parameters
  uint32_t scaler() const { return guest::LoadU32(address + 96); }
  void set_scaler(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t scaler_address() const { return address + 96; }
};

// embedded; measure: Event::Create.r3
struct Event : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // CreateEvent's handle (auto-reset); -1 when none
  uint32_t handle() const { return guest::LoadU32(address + 0); }
  void set_handle(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t handle_address() const { return address + 0; }
};

// embedded; measure: VideoOutput@+224, VideoOutput::UnlockDevice.r3@+224
struct Semaphore : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // its Event's handle (auto-reset, not set)
  uint32_t event() const { return guest::LoadU32(address + 0); }
  void set_event(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t event_address() const { return address + 0; }
  // RtlInitializeCriticalSection's
  uint32_t critical_section_address() const { return address + 4; }
  // 1 at Init; Wait waits while 0 or less; mirrored from the host semaphores for the archive's four
  int32_t count() const { return int32_t(guest::LoadU32(address + 32)); }
  void set_count(int32_t v) const { guest::StoreU32(address + 32, uint32_t(v)); }
  uint32_t count_address() const { return address + 32; }
  // 1 (probably the maximum; not read by Wait or Signal)
  uint32_t maximum() const { return guest::LoadU32(address + 36); }
  void set_maximum(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t maximum_address() const { return address + 36; }
};

// embedded; measure: ScopedLock
struct ScopedLock : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the critical section entered
  uint32_t critical_section() const { return guest::LoadU32(address + 0); }
  void set_critical_section(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t critical_section_address() const { return address + 0; }
};

// embedded; measure: Event::Wait.r4
struct EventWaitTimeout : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // -1 forever
  uint32_t milliseconds() const { return guest::LoadU32(address + 0); }
  void set_milliseconds(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t milliseconds_address() const { return address + 0; }
};

// embedded; measure: QueryPerformanceFrequency.r3
struct PerformanceFrequency : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 50,000,000 (ours)
  uint64_t frequency() const { return guest::LoadU64(address + 0); }
  void set_frequency(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t frequency_address() const { return address + 0; }
};

// embedded; measure: VideoOutput::UnlockDevice.r4
struct VideoOutputDeviceHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 after UnlockDevice
  template <typename T = Device>
  T device() const { return T(guest::LoadU32(address + 0)); }
  void set_device(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_device(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t device_address() const { return address + 0; }
};

// gpu; measure: Device.+20084
struct DeviceSharedBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // down by 1; at 0 the block goes
  uint32_t ref_count() const { return guest::LoadU32(address + 0); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t ref_count_address() const { return address + 0; }
  // freed (XMemFree)
  uint32_t buffer() const { return guest::LoadU32(address + 4); }
  void set_buffer(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t buffer_address() const { return address + 4; }
  // waited on (Device::BlockOnFence) when non-zero, probably
  uint32_t fence() const { return guest::LoadU32(address + 12); }
  void set_fence(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t fence_address() const { return address + 12; }
};

// gpu; measure: sub_822B4680.r3
struct DeviceDisplayCapture : View {
  using View::View;
  static constexpr uint32_t kSize = 20436;
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_292() const { return guest::LoadU32(address + 292); }
  void set_unknown_292(uint32_t v) const { guest::StoreU32(address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
  uint8_t unknown_296() const { return guest::LoadU8(address + 296); }
  void set_unknown_296(uint8_t v) const { guest::StoreU8(address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
  // freed (XMemFree or MmFreePhysicalMemory, by +20428's top bits)
  uint32_t capture_memory_0() const { return guest::LoadU32(address + 20128); }
  void set_capture_memory_0(uint32_t v) const { guest::StoreU32(address + 20128, v); }
  uint32_t capture_memory_0_address() const { return address + 20128; }
  // freed likewise
  uint32_t capture_memory_1() const { return guest::LoadU32(address + 20132); }
  void set_capture_memory_1(uint32_t v) const { guest::StoreU32(address + 20132, v); }
  uint32_t capture_memory_1_address() const { return address + 20132; }
  // -1 after the reset (six words)
  uint32_t unknown_20136() const { return guest::LoadU32(address + 20136); }
  void set_unknown_20136(uint32_t v) const { guest::StoreU32(address + 20136, v); }
  uint32_t unknown_20136_address() const { return address + 20136; }
  // two bits a block: how each was allocated
  uint32_t capture_flags() const { return guest::LoadU32(address + 20428); }
  void set_capture_flags(uint32_t v) const { guest::StoreU32(address + 20428, v); }
  uint32_t capture_flags_address() const { return address + 20428; }
  // kept across the reset
  uint32_t unknown_20432() const { return guest::LoadU32(address + 20432); }
  void set_unknown_20432(uint32_t v) const { guest::StoreU32(address + 20432, v); }
  uint32_t unknown_20432_address() const { return address + 20432; }
};

// gpu; measure: sub_822B4F10.r3
struct DeviceDisplayCaptureLoose : View {
  using View::View;
  static constexpr uint32_t kSize = 20425;
  uint32_t unknown_20136() const { return guest::LoadU32(address + 20136); }
  void set_unknown_20136(uint32_t v) const { guest::StoreU32(address + 20136, v); }
  uint32_t unknown_20136_address() const { return address + 20136; }
  uint32_t unknown_20168() const { return guest::LoadU32(address + 20168); }
  void set_unknown_20168(uint32_t v) const { guest::StoreU32(address + 20168, v); }
  uint32_t unknown_20168_address() const { return address + 20168; }
  uint32_t unknown_20172() const { return guest::LoadU32(address + 20172); }
  void set_unknown_20172(uint32_t v) const { guest::StoreU32(address + 20172, v); }
  uint32_t unknown_20172_address() const { return address + 20172; }
  // an event handle waited on (WaitForSingleObject), probably
  uint32_t unknown_20176() const { return guest::LoadU32(address + 20176); }
  void set_unknown_20176(uint32_t v) const { guest::StoreU32(address + 20176, v); }
  uint32_t unknown_20176_address() const { return address + 20176; }
  uint16_t unknown_20180() const { return guest::LoadU16(address + 20180); }
  void set_unknown_20180(uint16_t v) const { guest::StoreU16(address + 20180, v); }
  uint32_t unknown_20180_address() const { return address + 20180; }
  uint16_t unknown_20182() const { return guest::LoadU16(address + 20182); }
  void set_unknown_20182(uint16_t v) const { guest::StoreU16(address + 20182, v); }
  uint32_t unknown_20182_address() const { return address + 20182; }
  uint32_t unknown_20192() const { return guest::LoadU32(address + 20192); }
  void set_unknown_20192(uint32_t v) const { guest::StoreU32(address + 20192, v); }
  uint32_t unknown_20192_address() const { return address + 20192; }
  uint32_t unknown_20196() const { return guest::LoadU32(address + 20196); }
  void set_unknown_20196(uint32_t v) const { guest::StoreU32(address + 20196, v); }
  uint32_t unknown_20196_address() const { return address + 20196; }
  uint32_t unknown_20412() const { return guest::LoadU32(address + 20412); }
  void set_unknown_20412(uint32_t v) const { guest::StoreU32(address + 20412, v); }
  uint32_t unknown_20412_address() const { return address + 20412; }
  // a fence (Device::BlockOnFence), probably
  uint32_t unknown_20416() const { return guest::LoadU32(address + 20416); }
  void set_unknown_20416(uint32_t v) const { guest::StoreU32(address + 20416, v); }
  uint32_t unknown_20416_address() const { return address + 20416; }
  uint32_t unknown_20420() const { return guest::LoadU32(address + 20420); }
  void set_unknown_20420(uint32_t v) const { guest::StoreU32(address + 20420, v); }
  uint32_t unknown_20420_address() const { return address + 20420; }
  uint8_t unknown_20424() const { return guest::LoadU8(address + 20424); }
  void set_unknown_20424(uint8_t v) const { guest::StoreU8(address + 20424, v); }
  uint32_t unknown_20424_address() const { return address + 20424; }
};

// gpu; measure: sub_822AD058.r3
struct DeviceFreeChainNode : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // freed (XMemFree)
  uint32_t block() const { return guest::LoadU32(address + 8); }
  void set_block(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t block_address() const { return address + 8; }
  // freed first (sub_822AD058 on it), then cleared
  uint32_t next() const { return guest::LoadU32(address + 12); }
  void set_next(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t next_address() const { return address + 12; }
};

// gpu; measure: sub_82461F20.r3
struct PerfTimerSet : View {
  using View::View;
  static constexpr uint32_t kSize = 309936;
  template <typename T = PerfTimerSetEntry>
  T entry() const { return T(guest::LoadU32(address + 16)); }
  void set_entry(const View& v) const { guest::StoreU32(address + 16, v.address); }
  void set_entry(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t entry_address() const { return address + 16; }
  uint64_t unknown_309920() const { return guest::LoadU64(address + 309920); }
  void set_unknown_309920(uint64_t v) const { guest::StoreU64(address + 309920, v); }
  uint32_t unknown_309920_address() const { return address + 309920; }
  uint64_t unknown_309928() const { return guest::LoadU64(address + 309928); }
  void set_unknown_309928(uint64_t v) const { guest::StoreU64(address + 309928, v); }
  uint32_t unknown_309928_address() const { return address + 309928; }
};

// gpu; measure: sub_82461F20.r3.+16
struct PerfTimerSetEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
};

// embedded; measure: sub_82461F20.r6
struct PerfTimerResult : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float value() const { return guest::LoadFloat(address + 0); }
  void set_value(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: sub_8258BFA0.r4
struct DebugDrawTargetHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = DebugDrawTarget>
  T target() const { return T(guest::LoadU32(address + 0)); }
  void set_target(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_target(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t target_address() const { return address + 0; }
};

// gpu; measure: sub_8258BFA0.r4.+0
struct DebugDrawTarget : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // read and called through, probably
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
};

// embedded; measure: sub_8258C0B8.r5
struct DebugDrawText : View {
  using View::View;
  static constexpr uint32_t kSize = 6;
  // characters (as far as the measure saw)
  uint32_t text_address() const { return address + 0; }
};

// image; measure: [8296BCFC]
struct DebugTextQuad : View {
  using View::View;
  static constexpr uint32_t kSize = 144;
  float vertex0_x() const { return guest::LoadFloat(address + 0); }
  void set_vertex0_x(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t vertex0_x_address() const { return address + 0; }
  float vertex0_y() const { return guest::LoadFloat(address + 4); }
  void set_vertex0_y(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t vertex0_y_address() const { return address + 4; }
  float vertex0_z() const { return guest::LoadFloat(address + 8); }
  void set_vertex0_z(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t vertex0_z_address() const { return address + 8; }
  uint32_t vertex0_color() const { return guest::LoadU32(address + 12); }
  void set_vertex0_color(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t vertex0_color_address() const { return address + 12; }
  float vertex0_u() const { return guest::LoadFloat(address + 16); }
  void set_vertex0_u(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t vertex0_u_address() const { return address + 16; }
  float vertex0_v() const { return guest::LoadFloat(address + 20); }
  void set_vertex0_v(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t vertex0_v_address() const { return address + 20; }
  float vertex1_x() const { return guest::LoadFloat(address + 24); }
  void set_vertex1_x(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t vertex1_x_address() const { return address + 24; }
  float vertex1_y() const { return guest::LoadFloat(address + 28); }
  void set_vertex1_y(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t vertex1_y_address() const { return address + 28; }
  float vertex1_z() const { return guest::LoadFloat(address + 32); }
  void set_vertex1_z(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t vertex1_z_address() const { return address + 32; }
  uint32_t vertex1_color() const { return guest::LoadU32(address + 36); }
  void set_vertex1_color(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t vertex1_color_address() const { return address + 36; }
  float vertex1_u() const { return guest::LoadFloat(address + 40); }
  void set_vertex1_u(float v) const { guest::StoreFloat(address + 40, v); }
  uint32_t vertex1_u_address() const { return address + 40; }
  float vertex1_v() const { return guest::LoadFloat(address + 44); }
  void set_vertex1_v(float v) const { guest::StoreFloat(address + 44, v); }
  uint32_t vertex1_v_address() const { return address + 44; }
  float vertex2_x() const { return guest::LoadFloat(address + 48); }
  void set_vertex2_x(float v) const { guest::StoreFloat(address + 48, v); }
  uint32_t vertex2_x_address() const { return address + 48; }
  float vertex2_y() const { return guest::LoadFloat(address + 52); }
  void set_vertex2_y(float v) const { guest::StoreFloat(address + 52, v); }
  uint32_t vertex2_y_address() const { return address + 52; }
  float vertex2_z() const { return guest::LoadFloat(address + 56); }
  void set_vertex2_z(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t vertex2_z_address() const { return address + 56; }
  uint32_t vertex2_color() const { return guest::LoadU32(address + 60); }
  void set_vertex2_color(uint32_t v) const { guest::StoreU32(address + 60, v); }
  uint32_t vertex2_color_address() const { return address + 60; }
  float vertex2_u() const { return guest::LoadFloat(address + 64); }
  void set_vertex2_u(float v) const { guest::StoreFloat(address + 64, v); }
  uint32_t vertex2_u_address() const { return address + 64; }
  float vertex2_v() const { return guest::LoadFloat(address + 68); }
  void set_vertex2_v(float v) const { guest::StoreFloat(address + 68, v); }
  uint32_t vertex2_v_address() const { return address + 68; }
  float vertex3_x() const { return guest::LoadFloat(address + 72); }
  void set_vertex3_x(float v) const { guest::StoreFloat(address + 72, v); }
  uint32_t vertex3_x_address() const { return address + 72; }
  float vertex3_y() const { return guest::LoadFloat(address + 76); }
  void set_vertex3_y(float v) const { guest::StoreFloat(address + 76, v); }
  uint32_t vertex3_y_address() const { return address + 76; }
  float vertex3_z() const { return guest::LoadFloat(address + 80); }
  void set_vertex3_z(float v) const { guest::StoreFloat(address + 80, v); }
  uint32_t vertex3_z_address() const { return address + 80; }
  uint32_t vertex3_color() const { return guest::LoadU32(address + 84); }
  void set_vertex3_color(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t vertex3_color_address() const { return address + 84; }
  float vertex3_u() const { return guest::LoadFloat(address + 88); }
  void set_vertex3_u(float v) const { guest::StoreFloat(address + 88, v); }
  uint32_t vertex3_u_address() const { return address + 88; }
  float vertex3_v() const { return guest::LoadFloat(address + 92); }
  void set_vertex3_v(float v) const { guest::StoreFloat(address + 92, v); }
  uint32_t vertex3_v_address() const { return address + 92; }
  float vertex4_x() const { return guest::LoadFloat(address + 96); }
  void set_vertex4_x(float v) const { guest::StoreFloat(address + 96, v); }
  uint32_t vertex4_x_address() const { return address + 96; }
  float vertex4_y() const { return guest::LoadFloat(address + 100); }
  void set_vertex4_y(float v) const { guest::StoreFloat(address + 100, v); }
  uint32_t vertex4_y_address() const { return address + 100; }
  float vertex4_z() const { return guest::LoadFloat(address + 104); }
  void set_vertex4_z(float v) const { guest::StoreFloat(address + 104, v); }
  uint32_t vertex4_z_address() const { return address + 104; }
  uint32_t vertex4_color() const { return guest::LoadU32(address + 108); }
  void set_vertex4_color(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t vertex4_color_address() const { return address + 108; }
  float vertex4_u() const { return guest::LoadFloat(address + 112); }
  void set_vertex4_u(float v) const { guest::StoreFloat(address + 112, v); }
  uint32_t vertex4_u_address() const { return address + 112; }
  float vertex4_v() const { return guest::LoadFloat(address + 116); }
  void set_vertex4_v(float v) const { guest::StoreFloat(address + 116, v); }
  uint32_t vertex4_v_address() const { return address + 116; }
  float vertex5_x() const { return guest::LoadFloat(address + 120); }
  void set_vertex5_x(float v) const { guest::StoreFloat(address + 120, v); }
  uint32_t vertex5_x_address() const { return address + 120; }
  float vertex5_y() const { return guest::LoadFloat(address + 124); }
  void set_vertex5_y(float v) const { guest::StoreFloat(address + 124, v); }
  uint32_t vertex5_y_address() const { return address + 124; }
  float vertex5_z() const { return guest::LoadFloat(address + 128); }
  void set_vertex5_z(float v) const { guest::StoreFloat(address + 128, v); }
  uint32_t vertex5_z_address() const { return address + 128; }
  uint32_t vertex5_color() const { return guest::LoadU32(address + 132); }
  void set_vertex5_color(uint32_t v) const { guest::StoreU32(address + 132, v); }
  uint32_t vertex5_color_address() const { return address + 132; }
  float vertex5_u() const { return guest::LoadFloat(address + 136); }
  void set_vertex5_u(float v) const { guest::StoreFloat(address + 136, v); }
  uint32_t vertex5_u_address() const { return address + 136; }
  float vertex5_v() const { return guest::LoadFloat(address + 140); }
  void set_vertex5_v(float v) const { guest::StoreFloat(address + 140, v); }
  uint32_t vertex5_v_address() const { return address + 140; }
};

// embedded; measure: Thread@+64@+140
struct VideoOutputAt140 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: sub_822B4680.r3@+20128
struct DeviceDisplayCaptureAt20128 : View {
  using View::View;
  static constexpr uint32_t kSize = 297;
  uint32_t unknown_64() const { return guest::LoadU32(address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_292() const { return guest::LoadU32(address + 292); }
  void set_unknown_292(uint32_t v) const { guest::StoreU32(address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
  uint8_t unknown_296() const { return guest::LoadU8(address + 296); }
  void set_unknown_296(uint8_t v) const { guest::StoreU8(address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
};

// embedded; measure: Thread@+64
struct VideoOutputAt64 : View {
  using View::View;
  static constexpr uint32_t kSize = 308;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_16() const { return guest::LoadU32(address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  uint32_t unknown_20() const { return guest::LoadU32(address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_68() const { return guest::LoadU32(address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_76() const { return guest::LoadU32(address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint32_t unknown_88() const { return guest::LoadU32(address + 88); }
  void set_unknown_88(uint32_t v) const { guest::StoreU32(address + 88, v); }
  uint32_t unknown_88_address() const { return address + 88; }
  uint32_t unknown_92() const { return guest::LoadU32(address + 92); }
  void set_unknown_92(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t unknown_92_address() const { return address + 92; }
  uint32_t unknown_96() const { return guest::LoadU32(address + 96); }
  void set_unknown_96(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_100() const { return guest::LoadU32(address + 100); }
  void set_unknown_100(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t unknown_104() const { return guest::LoadU32(address + 104); }
  void set_unknown_104(uint32_t v) const { guest::StoreU32(address + 104, v); }
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_108() const { return guest::LoadU32(address + 108); }
  void set_unknown_108(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t unknown_108_address() const { return address + 108; }
  uint32_t unknown_140() const { return guest::LoadU32(address + 140); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  uint32_t unknown_144() const { return guest::LoadU32(address + 144); }
  void set_unknown_144(uint32_t v) const { guest::StoreU32(address + 144, v); }
  uint32_t unknown_144_address() const { return address + 144; }
  uint32_t unknown_148() const { return guest::LoadU32(address + 148); }
  void set_unknown_148(uint32_t v) const { guest::StoreU32(address + 148, v); }
  uint32_t unknown_148_address() const { return address + 148; }
  uint64_t unknown_152() const { return guest::LoadU64(address + 152); }
  void set_unknown_152(uint64_t v) const { guest::StoreU64(address + 152, v); }
  uint32_t unknown_152_address() const { return address + 152; }
  uint32_t unknown_208() const { return guest::LoadU32(address + 208); }
  void set_unknown_208(uint32_t v) const { guest::StoreU32(address + 208, v); }
  uint32_t unknown_208_address() const { return address + 208; }
  uint32_t unknown_212() const { return guest::LoadU32(address + 212); }
  void set_unknown_212(uint32_t v) const { guest::StoreU32(address + 212, v); }
  uint32_t unknown_212_address() const { return address + 212; }
  uint32_t unknown_216() const { return guest::LoadU32(address + 216); }
  void set_unknown_216(uint32_t v) const { guest::StoreU32(address + 216, v); }
  uint32_t unknown_216_address() const { return address + 216; }
  uint32_t unknown_220() const { return guest::LoadU32(address + 220); }
  void set_unknown_220(uint32_t v) const { guest::StoreU32(address + 220, v); }
  uint32_t unknown_220_address() const { return address + 220; }
  uint8_t unknown_264() const { return guest::LoadU8(address + 264); }
  void set_unknown_264(uint8_t v) const { guest::StoreU8(address + 264, v); }
  uint32_t unknown_264_address() const { return address + 264; }
  uint8_t unknown_265() const { return guest::LoadU8(address + 265); }
  void set_unknown_265(uint8_t v) const { guest::StoreU8(address + 265, v); }
  uint32_t unknown_265_address() const { return address + 265; }
  uint8_t unknown_266() const { return guest::LoadU8(address + 266); }
  void set_unknown_266(uint8_t v) const { guest::StoreU8(address + 266, v); }
  uint32_t unknown_266_address() const { return address + 266; }
  uint8_t unknown_267() const { return guest::LoadU8(address + 267); }
  void set_unknown_267(uint8_t v) const { guest::StoreU8(address + 267, v); }
  uint32_t unknown_267_address() const { return address + 267; }
  uint32_t unknown_268() const { return guest::LoadU32(address + 268); }
  void set_unknown_268(uint32_t v) const { guest::StoreU32(address + 268, v); }
  uint32_t unknown_268_address() const { return address + 268; }
  uint32_t unknown_272() const { return guest::LoadU32(address + 272); }
  void set_unknown_272(uint32_t v) const { guest::StoreU32(address + 272, v); }
  uint32_t unknown_272_address() const { return address + 272; }
  uint32_t unknown_276() const { return guest::LoadU32(address + 276); }
  void set_unknown_276(uint32_t v) const { guest::StoreU32(address + 276, v); }
  uint32_t unknown_276_address() const { return address + 276; }
  uint32_t unknown_280() const { return guest::LoadU32(address + 280); }
  void set_unknown_280(uint32_t v) const { guest::StoreU32(address + 280, v); }
  uint32_t unknown_280_address() const { return address + 280; }
  uint32_t unknown_284() const { return guest::LoadU32(address + 284); }
  void set_unknown_284(uint32_t v) const { guest::StoreU32(address + 284, v); }
  uint32_t unknown_284_address() const { return address + 284; }
  uint32_t unknown_288() const { return guest::LoadU32(address + 288); }
  void set_unknown_288(uint32_t v) const { guest::StoreU32(address + 288, v); }
  uint32_t unknown_288_address() const { return address + 288; }
  uint8_t unknown_292() const { return guest::LoadU8(address + 292); }
  void set_unknown_292(uint8_t v) const { guest::StoreU8(address + 292, v); }
  uint32_t unknown_292_address() const { return address + 292; }
  uint32_t unknown_296() const { return guest::LoadU32(address + 296); }
  void set_unknown_296(uint32_t v) const { guest::StoreU32(address + 296, v); }
  uint32_t unknown_296_address() const { return address + 296; }
  uint32_t unknown_300() const { return guest::LoadU32(address + 300); }
  void set_unknown_300(uint32_t v) const { guest::StoreU32(address + 300, v); }
  uint32_t unknown_300_address() const { return address + 300; }
  uint32_t unknown_304() const { return guest::LoadU32(address + 304); }
  void set_unknown_304(uint32_t v) const { guest::StoreU32(address + 304, v); }
  uint32_t unknown_304_address() const { return address + 304; }
};

// The image words docs/functions/video-output.md's code uses.
struct VideoOutputGlobals {
  // 0 at the video output's construction and once a frame (VideoOutput::BeginFrame); read only by the original WaitForVblank
  static uint32_t vblank_count() { return guest::LoadU32(0x8296BD30u); }
  static void set_vblank_count(uint32_t v) { guest::StoreU32(0x8296BD30u, v); }
  static constexpr uint32_t vblank_count_address() { return 0x8296BD30u; }
  // the other vblank count, up by 1 a vblank in VideoOutput::OnVblank (never called now)
  static uint32_t vblank_count_total() { return guest::LoadU32(0x8296BD2Cu); }
  static void set_vblank_count_total(uint32_t v) { guest::StoreU32(0x8296BD2Cu, v); }
  static constexpr uint32_t vblank_count_total_address() { return 0x8296BD2Cu; }
  // the vblank Event's handle, waited on only by the original WaitForVblank
  static uint32_t vblank_event() { return guest::LoadU32(0x8296BD34u); }
  static void set_vblank_event(uint32_t v) { guest::StoreU32(0x8296BD34u, v); }
  static constexpr uint32_t vblank_event_address() { return 0x8296BD34u; }
  // 0.0: the viewport's min Z; Device::Resolve's f1 in ResolveFrontBuffer
  static float resolve_z() { return guest::LoadFloat(0x82000A34u); }
  static void set_resolve_z(float v) { guest::StoreFloat(0x82000A34u, v); }
  static constexpr uint32_t resolve_z_address() { return 0x82000A34u; }
};

}  // namespace schema
