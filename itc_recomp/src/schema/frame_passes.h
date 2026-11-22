// The views of docs/functions/frame-passes.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/frame-passes.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: FramePasses
struct FramePasses : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  static constexpr uint32_t kVtable = 0x82000F20;
  template <typename T = FramePassesVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the framework's +52 interface (slots 1 the first worker pool, 2 the renderer, 3 the scene)
  template <typename T = FramePassesServices>
  T services() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_services(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_services(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t services_address() const { return address + 4; }
  // the job's +0: 0x820066DC
  uint32_t job_vtable() const { return guest::LoadU32(base, address + 8); }
  void set_job_vtable(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t job_vtable_address() const { return address + 8; }
  // the job's +4: 0x82000AB4, then 0x82002908
  uint32_t job_interface() const { return guest::LoadU32(base, address + 12); }
  void set_job_interface(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t job_interface_address() const { return address + 12; }
  // the job's +8: a worker's value (set when it starts)
  uint32_t job_worker_value() const { return guest::LoadU32(base, address + 16); }
  void set_job_worker_value(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t job_worker_value_address() const { return address + 16; }
  // the job's +12: 1 queued, 2 running, 3 done
  uint32_t job_state() const { return guest::LoadU32(base, address + 20); }
  void set_job_state(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t job_state_address() const { return address + 20; }
  // the job's +16: the run isn't made when set
  uint8_t job_skip() const { return guest::LoadU8(base, address + 24); }
  void set_job_skip(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t job_skip_address() const { return address + 24; }
  // the job's +17: the post's r5 (1)
  uint8_t job_post_arg() const { return guest::LoadU8(base, address + 25); }
  void set_job_post_arg(uint8_t v) const { guest::StoreU8(base, address + 25, v); }
  uint32_t job_post_arg_address() const { return address + 25; }
  // the job's +18: set while posted, cleared by Job::Wait
  uint8_t job_posted() const { return guest::LoadU8(base, address + 26); }
  void set_job_posted(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t job_posted_address() const { return address + 26; }
  // the job's +20: the pool's waiter (copied unset from the constructor's frame until the post sets it)
  template <typename T = FramePassesJobWaiter>
  T job_waiter() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_job_waiter(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_job_waiter(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t job_waiter_address() const { return address + 28; }
  // the job's +24: a member delegate's vtable 0x82002850 (0x82000ABC when destroyed)
  uint32_t job_delegate_vtable() const { return guest::LoadU32(base, address + 32); }
  void set_job_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t job_delegate_vtable_address() const { return address + 32; }
  // the delegate's object: this
  template <typename T = FramePasses>
  T job_delegate_object() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_job_delegate_object(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_job_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t job_delegate_object_address() const { return address + 36; }
  // sub_82096330, the thunk to slot 0 (FramePasses::Boot)
  uint32_t job_delegate_target() const { return guest::LoadU32(base, address + 40); }
  void set_job_delegate_target(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t job_delegate_target_address() const { return address + 40; }
};

// image; measure: FramePasses.+0
struct FramePassesVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t slot_0_boot() const { return guest::LoadU32(base, address + 0); }
  void set_slot_0_boot(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t slot_0_boot_address() const { return address + 0; }
  uint32_t slot_1_record() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1_record(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_record_address() const { return address + 4; }
  uint32_t slot_2_flip_work_mem() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2_flip_work_mem(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_flip_work_mem_address() const { return address + 8; }
  uint32_t slot_3_on_event() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3_on_event(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_on_event_address() const { return address + 12; }
  uint32_t slot_4_deleting_destructor() const { return guest::LoadU32(base, address + 16); }
  void set_slot_4_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot_4_deleting_destructor_address() const { return address + 16; }
  // FramePasses::Record declines unless it is sub_82096360
  uint32_t slot_5_record_constants() const { return guest::LoadU32(base, address + 20); }
  void set_slot_5_record_constants(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot_5_record_constants_address() const { return address + 20; }
  // FramePasses::Record declines unless it is sub_82096030
  uint32_t slot_6_record_samplers() const { return guest::LoadU32(base, address + 24); }
  void set_slot_6_record_samplers(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot_6_record_samplers_address() const { return address + 24; }
};

// embedded; measure: FramePasses.+4
struct FramePassesServices : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = FramePassesServicesVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: FramePasses.+4.+0
struct FramePassesServicesVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the first worker pool (framework + 68)
  uint32_t slot_1_worker_pool() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1_worker_pool(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_worker_pool_address() const { return address + 4; }
  // the renderer (framework + 76)
  uint32_t slot_2_renderer() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2_renderer(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_renderer_address() const { return address + 8; }
  // the scene (framework + 80)
  uint32_t slot_3_scene() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3_scene(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_scene_address() const { return address + 12; }
};

// stack; measure: FramePasses::FramePasses.r4
struct FramePassesBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = FramePassesRunLoop>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // the framework's +52 interface, kept at +4
  template <typename T = FramePassesServices>
  T services() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_services(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_services(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t services_address() const { return address + 4; }
};

// embedded; measure: FramePasses::FramePasses.r4.+0, FramePasses::OnEvent.r4
struct FramePassesRunLoop : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr uint32_t kVtable = 0x82000FE8;
  template <typename T = FramePassesRunLoopVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the tick handler map (a std::map by key): its first word
  uint32_t tick_handlers() const { return guest::LoadU32(base, address + 4); }
  void set_tick_handlers(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t tick_handlers_address() const { return address + 4; }
  // the tick handler map's head node
  uint32_t tick_handlers_head() const { return guest::LoadU32(base, address + 8); }
  void set_tick_handlers_head(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tick_handlers_head_address() const { return address + 8; }
};

// image; measure: FramePasses::OnEvent.r4.+0
struct FramePassesRunLoopVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint32_t slot_1_add_listener() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1_add_listener(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_add_listener_address() const { return address + 4; }
  uint32_t slot_2_add_tick_handler() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2_add_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_add_tick_handler_address() const { return address + 8; }
  uint32_t slot_3_remove_listener() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3_remove_listener(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_remove_listener_address() const { return address + 12; }
  uint32_t slot_4_remove_tick_handler() const { return guest::LoadU32(base, address + 16); }
  void set_slot_4_remove_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot_4_remove_tick_handler_address() const { return address + 16; }
};

// embedded; measure: FramePasses::OnEvent.r5
struct FramePassesEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 1: the tick handlers 3 and 20 and the listener for event 8 removed
  uint32_t code() const { return guest::LoadU32(base, address + 0); }
  void set_code(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t code_address() const { return address + 0; }
};

// embedded; measure: sub_82096320.r3, sub_82096350.r3
struct FramePassesDelegateObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = FramePassesDelegateObjectVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: sub_82096320.r3.+0, sub_82096350.r3.+0
struct FramePassesDelegateObjectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // sub_82096350's target
  uint32_t slot_2() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_address() const { return address + 8; }
  // sub_82096320's target
  uint32_t slot_3() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_address() const { return address + 12; }
};

// embedded; measure: Job::Wait.r3.+20
struct FramePassesJobWaiter : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = FramePassesJobWaiterVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: Job::Wait.r3.+20.+0
struct FramePassesJobWaiterVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // called by Job::Wait with r4 = the job (probably the pool's wait)
  uint32_t slot_1_wait() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1_wait(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_wait_address() const { return address + 4; }
};

// The image words docs/functions/frame-passes.md's code uses.
struct FramePassesGlobals {
  // the work memory's current buffer (0 or 1), flipped by WorkMem::Flip
  static uint32_t work_mem_current(uint8_t* base) { return guest::LoadU32(base, 0x8296B780u); }
  static void set_work_mem_current(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B780u, v); }
  static constexpr uint32_t work_mem_current_address() { return 0x8296B780u; }
  // a word per buffer, zeroed for the new one by WorkMem::Flip (not read further)
  static constexpr uint32_t work_mem_buffer_words_address() { return 0x8296B784u; }
  // the vertex buffer headers used, a word per buffer
  static constexpr uint32_t work_mem_headers_used_address() { return 0x8296B798u; }
  // the bytes used, a word per buffer
  static constexpr uint32_t work_mem_bytes_used_address() { return 0x8296B7A0u; }
  // the samplers' LOD bias
  static float lod_bias(uint8_t* base) { return guest::LoadFloat(base, 0x8296B8F4u); }
  static void set_lod_bias(uint8_t* base, float v) { guest::StoreFloat(base, 0x8296B8F4u, v); }
  static constexpr uint32_t lod_bias_address() { return 0x8296B8F4u; }
  // the render queue's group 1 key (270 every pass)
  static uint32_t sort_key_group1(uint8_t* base) { return guest::LoadU32(base, 0x8296B900u); }
  static void set_sort_key_group1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B900u, v); }
  static constexpr uint32_t sort_key_group1_address() { return 0x8296B900u; }
  // the render queue's depth key (163 every pass)
  static uint32_t sort_key_depth(uint8_t* base) { return guest::LoadU32(base, 0x8296B908u); }
  static void set_sort_key_depth(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B908u, v); }
  static constexpr uint32_t sort_key_depth_address() { return 0x8296B908u; }
  // the render queue's group 0 key (168 every pass)
  static uint32_t sort_key_group0(uint8_t* base) { return guest::LoadU32(base, 0x8296B90Cu); }
  static void set_sort_key_group0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B90Cu, v); }
  static constexpr uint32_t sort_key_group0_address() { return 0x8296B90Cu; }
  // the command list the primitive reset is recorded on
  template <typename T = CommandList>
  static T command_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8296B918u)); }
  static void set_command_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8296B918u, v.address); }
  static void set_command_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B918u, v); }
  static constexpr uint32_t command_list_address() { return 0x8296B918u; }
  // the render queue the primitive reset is added to (its slot 4)
  static uint32_t render_queue(uint8_t* base) { return guest::LoadU32(base, 0x8296B91Cu); }
  static void set_render_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B91Cu, v); }
  static constexpr uint32_t render_queue_address() { return 0x8296B91Cu; }
  // 0.0: Clear Z, the queue key, viewport min Z
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1.0: viewport max Z
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // -8.0 (render state 204)
  static float slope_bias(uint8_t* base) { return guest::LoadFloat(base, 0x82000F18u); }
  static void set_slope_bias(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000F18u, v); }
  static constexpr uint32_t slope_bias_address() { return 0x82000F18u; }
  // -0.0001 (render state 208)
  static float depth_bias(uint8_t* base) { return guest::LoadFloat(base, 0x82000F1Cu); }
  static void set_depth_bias(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000F1Cu, v); }
  static constexpr uint32_t depth_bias_address() { return 0x82000F1Cu; }
  // vertex constant c86 (zero in the image, written at run time)
  static constexpr uint32_t vertex_c86_address() { return 0x8265ED20u; }
  // pixel constant c76 (zero in the image, written at run time)
  static constexpr uint32_t pixel_c76_address() { return 0x8265ED30u; }
  // the scene target SceneTarget::RecordPasses is given
  static uint32_t scene_target(uint8_t* base) { return guest::LoadU32(base, 0x8266D08Cu); }
  static void set_scene_target(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D08Cu, v); }
  static constexpr uint32_t scene_target_address() { return 0x8266D08Cu; }
};

}  // namespace schema
