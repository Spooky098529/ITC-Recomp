// The views of docs/functions/frame-driver.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/frame-driver.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Thread
struct FrameDriver : View {
  using View::View;
  static constexpr uint32_t kSize = 400;
  static constexpr uint32_t kVtable = 0x82000D7C;
  // 0x82000D7C (0 the deleting destructor, 1 FrameDriver::ThreadLoop); the Thread base's 0x8206CB30 first
  template <typename T = GameThreadVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the Thread's handle, -1 for none
  uint32_t thread_handle() const { return guest::LoadU32(base, address + 4); }
  void set_thread_handle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t thread_handle_address() const { return address + 4; }
  // the Thread's object whose slot 1 runs (the thread itself when 0 is passed)
  template <typename T = FrameDriver>
  T thread_object() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_thread_object(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_thread_object(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t thread_object_address() const { return address + 8; }
  // the Thread's hardware thread (1 for the render thread)
  uint32_t thread_processor() const { return guest::LoadU32(base, address + 12); }
  void set_thread_processor(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t thread_processor_address() const { return address + 12; }
  // the Thread's priority index into {2, 1, 0, -1, -2} (2 for the render thread)
  uint32_t thread_priority() const { return guest::LoadU32(base, address + 16); }
  void set_thread_priority(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t thread_priority_address() const { return address + 16; }
  // the interface (FrameDriverInterface) framework + 76 holds
  uint32_t interface_address() const { return address + 20; }
  // 0x82000D6C (0x82000D48, the base's, first): 1 AddListener, 2 RemoveListener, 3 GetVideoOutput
  uint32_t interface_vtable() const { return guest::LoadU32(base, address + 20); }
  void set_interface_vtable(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t interface_vtable_address() const { return address + 20; }
  // the framework's +4 interface (FrameDriver::Init's block +4)
  uint32_t framework_interface() const { return guest::LoadU32(base, address + 24); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t framework_interface_address() const { return address + 24; }
  // the listener map (a HandlerMap, framework.md): keys 1 Scene::Draw, 3 the front buffer resolve, 5 FrameDriver::Present
  uint32_t listeners_address() const { return address + 28; }
  // the request vector (12-byte requests {type, key, delegate}; interface + 20)
  uint32_t requests_address() const { return address + 40; }
  uint32_t requests_first() const { return guest::LoadU32(base, address + 44); }
  void set_requests_first(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t requests_first_address() const { return address + 44; }
  uint32_t requests_last() const { return guest::LoadU32(base, address + 48); }
  void set_requests_last(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t requests_last_address() const { return address + 48; }
  uint32_t requests_end() const { return guest::LoadU32(base, address + 52); }
  void set_requests_end(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t requests_end_address() const { return address + 52; }
  // the video output (VideoOutput, video-output.md)
  uint32_t video_output_address() const { return address + 64; }
  // the device while FrameDriver::DrawFrame holds the device lock
  uint32_t device() const { return guest::LoadU32(base, address + 384); }
  void set_device(uint32_t v) const { guest::StoreU32(base, address + 384, v); }
  uint32_t device_address() const { return address + 384; }
  // signaled by tick handler 2 (FrameDriver::StartFrame)
  uint32_t frame_start_event() const { return guest::LoadU32(base, address + 388); }
  void set_frame_start_event(uint32_t v) const { guest::StoreU32(base, address + 388, v); }
  uint32_t frame_start_event_address() const { return address + 388; }
  // waited on by tick handler 16 (FrameDriver::WaitForFrame)
  uint32_t frame_done_event() const { return guest::LoadU32(base, address + 392); }
  void set_frame_done_event(uint32_t v) const { guest::StoreU32(base, address + 392, v); }
  uint32_t frame_done_event_address() const { return address + 392; }
  // 1 while it runs (set by FrameDriver::Init)
  uint8_t running() const { return guest::LoadU8(base, address + 396); }
  void set_running(uint8_t v) const { guest::StoreU8(base, address + 396, v); }
  uint32_t running_address() const { return address + 396; }
};

// image; measure: Thread.+0
struct GameThreadVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // slot 1, what Thread::Start runs (FrameDriver::ThreadLoop)
  uint32_t run() const { return guest::LoadU32(base, address + 4); }
  void set_run(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t run_address() const { return address + 4; }
};

// embedded; measure: Thread::Begin.r5
struct GameThreadArgWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: sub_82091A10.r3, Scene::Draw.r4
struct FrameDriverInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x82000D6C (0x82000D48 the base's)
  template <typename T = FrameDriverInterfaceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: Scene::Draw.r4.+0
struct FrameDriverInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // a thunk to the frame driver's deleting destructor
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 0); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t deleting_destructor_address() const { return address + 0; }
  // FrameDriver::AddListener sub_820922B8
  uint32_t add_listener() const { return guest::LoadU32(base, address + 4); }
  void set_add_listener(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t add_listener_address() const { return address + 4; }
  // FrameDriver::RemoveListener
  uint32_t remove_listener() const { return guest::LoadU32(base, address + 8); }
  void set_remove_listener(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t remove_listener_address() const { return address + 8; }
  // FrameDriver::GetVideoOutput sub_824960E0
  uint32_t get_video_output() const { return guest::LoadU32(base, address + 12); }
  void set_get_video_output(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t get_video_output_address() const { return address + 12; }
};

// embedded; measure: FrameDriver::AddListener.r4, sub_82091BF8.r4, sub_82091F58.r5
struct FrameDriverListenerKey : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t key() const { return guest::LoadU32(base, address + 0); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_address() const { return address + 0; }
};

// embedded; measure: FrameDriver::AddListener.r5
struct FrameDriverDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0x82002850 (base 0x82000ABC)
  template <typename T = FrameDriverDelegateVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: FrameDriver::AddListener.r5.+0
struct FrameDriverDelegateVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // slot 2, Delegate::Clone
  uint32_t clone() const { return guest::LoadU32(base, address + 8); }
  void set_clone(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t clone_address() const { return address + 8; }
};

// embedded; measure: FrameDriver::Init.r4
struct FrameDriverInitBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = FrameDriverRunLoopRef>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // the framework's +4 interface
  uint32_t framework_interface() const { return guest::LoadU32(base, address + 4); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
};

// embedded; measure: Scene::Scene.r4
struct AppSceneInitBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = FrameDriverRunLoopRef>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // the framework's +8 interface
  template <typename T = AppSceneFrameworkInterface>
  T framework_interface() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_framework_interface(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
};

// embedded; measure: FrameDriver::Init.r4.+0, Scene::Scene.r4.+0, SaveDevices::SaveDevices.r4.+0, sub_820998B8.r4.+0
struct FrameDriverRunLoopRef : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the run loop's vtable 0x82000FE8
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the vtable's slot 1 (AddListener)
  uint32_t add_listener() const { return guest::LoadU32(base, address + 4); }
  void set_add_listener(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t add_listener_address() const { return address + 4; }
  // measure fault: the vtable's slot 2 (AddTickHandler)
  uint32_t add_tick_handler() const { return guest::LoadU32(base, address + 8); }
  void set_add_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t add_tick_handler_address() const { return address + 8; }
};

// embedded; measure: Scene::OnEvent5.r4, SaveDevices::OnEvent.r4
struct FrameDriverRunLoop : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x82000FE8
  template <typename T = FrameDriverRunLoopVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: Scene::OnEvent5.r4.+0, SaveDevices::OnEvent.r4.+0
struct FrameDriverRunLoopVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // slot 3, removes a listener
  uint32_t remove_listener() const { return guest::LoadU32(base, address + 12); }
  void set_remove_listener(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t remove_listener_address() const { return address + 12; }
  // slot 4, removes a tick handler (sub_8209AB08)
  uint32_t remove_tick_handler() const { return guest::LoadU32(base, address + 16); }
  void set_remove_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t remove_tick_handler_address() const { return address + 16; }
};

// embedded; measure: Scene::OnEvent5.r5, SaveDevices::OnEvent.r5
struct FrameDriverEventWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 1 the shutdown
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: sub_82090380.r3
struct FrameDriverOwnedPointer : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = FrameDriverOwnedObject>
  T object() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_object(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t object_address() const { return address + 0; }
};

// embedded; measure: sub_82090380.r3.+0
struct FrameDriverOwnedObject : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // measure fault: also its vtable's slot 0 (the deleting destructor), the object and its vtable joined
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// embedded; measure: sub_82091F58.r3, sub_82099F50.r3, sub_82093000.r3
struct FrameDriverMapIterator : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the map and the node as one doubleword
  uint64_t both() const { return guest::LoadU64(base, address + 0); }
  void set_both(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t both_address() const { return address + 0; }
  template <typename T = FrameDriverListenerMap>
  T map() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_map(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_map(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t map_address() const { return address + 0; }
  template <typename T = FrameDriverMapNode>
  T node() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_node(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_address() const { return address + 4; }
};

// embedded; measure: sub_82091F58.r4, sub_82093000.r4, sub_82099F50.ret.+0
struct FrameDriverListenerMap : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the head node (+0 the leftmost, +4 the root, +8 the rightmost)
  template <typename T = FrameDriverMapNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
};

// allocated; measure: sub_82093000.r4.+4, sub_82093000.r4.+4.+0, sub_82093000.r4.+4.+8, sub_82099F50.ret.+0.+4, sub_82099F50.ret.+0.+4.+8, sub_82099F50.ret.+0.+4.+8.+0
#pragma pack(push, 1)
struct FrameDriverMapNodeData {
  uint32_t left = 0;  // +0
  uint32_t parent = 0;  // +4
  uint32_t right = 0;  // +8
  uint32_t key = 0;  // +12
  uint32_t delegate = 0;  // +16
  uint8_t color = 0;  // +20
  uint8_t is_nil = 0;  // +21
  uint8_t pad_22[2] = {};
};
#pragma pack(pop)
static_assert(sizeof(FrameDriverMapNodeData) == 24);
inline constexpr memory::LayoutField kFrameDriverMapNodeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}};
inline constexpr memory::Layout kFrameDriverMapNodeLayout = {kFrameDriverMapNodeFields, 5};

struct FrameDriverMapNode : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  FrameDriverMapNodeData* data() const { return static_cast<FrameDriverMapNodeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static FrameDriverMapNode New(uint8_t* base) {
    return FrameDriverMapNode(base, memory::MakeHandle(new FrameDriverMapNodeData(), sizeof(FrameDriverMapNodeData), "FrameDriverMapNode", &kFrameDriverMapNodeLayout));
  }
  template <uint32_t Stride = kSize>
  static FrameDriverMapNode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FrameDriverMapNodeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrameDriverMapNodeData*>(::operator new(sizeof(FrameDriverMapNodeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrameDriverMapNodeData();
    return FrameDriverMapNode(base, memory::MakeHandle(p, sizeof(FrameDriverMapNodeData) * count, "FrameDriverMapNode", &kFrameDriverMapNodeLayout, sizeof(FrameDriverMapNodeData)));
  }
  FrameDriverMapNode at(uint32_t i) const { return FrameDriverMapNode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFrameDriverMapNodeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFrameDriverMapNodeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t left_address() const { return address + 0; }
  template <typename T = FrameDriverMapNode>
  T left() const { return T(base, on_host() ? data()->left : guest::LoadU32(base, address + 0)); }
  void set_left(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->left = v; else guest::StoreU32(base, address + 0, v); }
  void set_left(uint32_t v) const { if (on_host()) data()->left = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t left_bits() const { return on_host() ? data()->left : guest::LoadU32(base, address + 0); }
  void set_left_bits(uint32_t v) const { if (on_host()) data()->left = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t parent_address() const { return address + 4; }
  template <typename T = FrameDriverMapNode>
  T parent() const { return T(base, on_host() ? data()->parent : guest::LoadU32(base, address + 4)); }
  void set_parent(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->parent = v; else guest::StoreU32(base, address + 4, v); }
  void set_parent(uint32_t v) const { if (on_host()) data()->parent = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t parent_bits() const { return on_host() ? data()->parent : guest::LoadU32(base, address + 4); }
  void set_parent_bits(uint32_t v) const { if (on_host()) data()->parent = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t right_address() const { return address + 8; }
  template <typename T = FrameDriverMapNode>
  T right() const { return T(base, on_host() ? data()->right : guest::LoadU32(base, address + 8)); }
  void set_right(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->right = v; else guest::StoreU32(base, address + 8, v); }
  void set_right(uint32_t v) const { if (on_host()) data()->right = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t right_bits() const { return on_host() ? data()->right : guest::LoadU32(base, address + 8); }
  void set_right_bits(uint32_t v) const { if (on_host()) data()->right = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t key_address() const { return address + 12; }
  uint32_t key() const { return on_host() ? uint32_t(data()->key) : uint32_t(guest::LoadU32(base, address + 12)); }
  void set_key(uint32_t v) const { if (on_host()) data()->key = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t key_bits() const { return on_host() ? data()->key : guest::LoadU32(base, address + 12); }
  void set_key_bits(uint32_t v) const { if (on_host()) data()->key = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_address() const { return address + 16; }
  uint32_t delegate() const { return on_host() ? uint32_t(data()->delegate) : uint32_t(guest::LoadU32(base, address + 16)); }
  void set_delegate(uint32_t v) const { if (on_host()) data()->delegate = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_bits() const { return on_host() ? data()->delegate : guest::LoadU32(base, address + 16); }
  void set_delegate_bits(uint32_t v) const { if (on_host()) data()->delegate = v; else guest::StoreU32(base, address + 16, v); }
  uint32_t color_address() const { return address + 20; }
  uint8_t color() const { return on_host() ? uint8_t(data()->color) : uint8_t(guest::LoadU8(base, address + 20)); }
  void set_color(uint8_t v) const { if (on_host()) data()->color = v; else guest::StoreU8(base, address + 20, v); }
  uint8_t color_bits() const { return on_host() ? data()->color : guest::LoadU8(base, address + 20); }
  void set_color_bits(uint8_t v) const { if (on_host()) data()->color = v; else guest::StoreU8(base, address + 20, v); }
  uint32_t is_nil_address() const { return address + 21; }
  uint8_t is_nil() const { return on_host() ? uint8_t(data()->is_nil) : uint8_t(guest::LoadU8(base, address + 21)); }
  void set_is_nil(uint8_t v) const { if (on_host()) data()->is_nil = v; else guest::StoreU8(base, address + 21, v); }
  uint8_t is_nil_bits() const { return on_host() ? data()->is_nil : guest::LoadU8(base, address + 21); }
  void set_is_nil_bits(uint8_t v) const { if (on_host()) data()->is_nil = v; else guest::StoreU8(base, address + 21, v); }
};

// allocated; measure: Scene
struct AppScene : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  static constexpr uint32_t kVtable = 0x82000F3C;
  // 0x82000F3C (0 the deleting destructor, 1 the command list's getter, 2 the render queue's)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the framework's +8 interface
  template <typename T = AppSceneFrameworkInterface>
  T framework_interface() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_framework_interface(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
  // the command list (CommandList, command-list.toml)
  uint32_t command_list_address() const { return address + 8; }
  // the command list's +12, the slots used this tick
  uint32_t command_list_used() const { return guest::LoadU32(base, address + 20); }
  void set_command_list_used(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t command_list_used_address() const { return address + 20; }
  // the command list's +16
  uint32_t command_list_last_used() const { return guest::LoadU32(base, address + 24); }
  void set_command_list_last_used(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t command_list_last_used_address() const { return address + 24; }
  // the command list's +20, both buffers' block
  uint32_t command_list_allocation() const { return guest::LoadU32(base, address + 28); }
  void set_command_list_allocation(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t command_list_allocation_address() const { return address + 28; }
  // the render queue (RenderQueue, device-shaders.toml)
  uint32_t render_queue_address() const { return address + 64; }
  // the render queue's +0, 0x8206CB8C
  template <typename T = AppSceneRenderQueueVtable>
  T render_queue_vtable() const { return T(base, guest::LoadU32(base, address + 64)); }
  void set_render_queue_vtable(const View& v) const { guest::StoreU32(base, address + 64, v.address); }
  void set_render_queue_vtable(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t render_queue_vtable_address() const { return address + 64; }
};

// image; measure: Scene.+64
struct AppSceneRenderQueueVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // slot 0, RenderQueue::Init
  uint32_t init() const { return guest::LoadU32(base, address + 0); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t init_address() const { return address + 0; }
  // slot 2, RenderQueue::ClearRecording
  uint32_t clear_recording() const { return guest::LoadU32(base, address + 8); }
  void set_clear_recording(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t clear_recording_address() const { return address + 8; }
  // slot 5, RenderQueue::Replay
  uint32_t replay() const { return guest::LoadU32(base, address + 20); }
  void set_replay(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t replay_address() const { return address + 20; }
  // slot 6, RenderQueue::Flip
  uint32_t flip() const { return guest::LoadU32(base, address + 24); }
  void set_flip(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t flip_address() const { return address + 24; }
};

// embedded; measure: Scene.+4, Scene::Scene.r4.+4
#pragma pack(push, 1)
struct AppSceneFrameworkInterfaceData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(AppSceneFrameworkInterfaceData) == 4);
inline constexpr memory::LayoutField kAppSceneFrameworkInterfaceFields[] = {{0, 4}};
inline constexpr memory::Layout kAppSceneFrameworkInterfaceLayout = {kAppSceneFrameworkInterfaceFields, 1};

struct AppSceneFrameworkInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AppSceneFrameworkInterfaceData* data() const { return static_cast<AppSceneFrameworkInterfaceData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AppSceneFrameworkInterface New(uint8_t* base) {
    return AppSceneFrameworkInterface(base, memory::MakeHandle(new AppSceneFrameworkInterfaceData(), sizeof(AppSceneFrameworkInterfaceData), "AppSceneFrameworkInterface", &kAppSceneFrameworkInterfaceLayout));
  }
  template <uint32_t Stride = kSize>
  static AppSceneFrameworkInterface NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AppSceneFrameworkInterfaceData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AppSceneFrameworkInterfaceData*>(::operator new(sizeof(AppSceneFrameworkInterfaceData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AppSceneFrameworkInterfaceData();
    return AppSceneFrameworkInterface(base, memory::MakeHandle(p, sizeof(AppSceneFrameworkInterfaceData) * count, "AppSceneFrameworkInterface", &kAppSceneFrameworkInterfaceLayout, sizeof(AppSceneFrameworkInterfaceData)));
  }
  AppSceneFrameworkInterface at(uint32_t i) const { return AppSceneFrameworkInterface(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kAppSceneFrameworkInterfaceLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kAppSceneFrameworkInterfaceLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = AppSceneFrameworkInterfaceVtable>
  T vtable() const { return T(base, on_host() ? data()->vtable : guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
};

// image; measure: Scene.+4.+0, Scene::Scene.r4.+4.+0
struct AppSceneFrameworkInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 1, the frame driver's interface (frame driver + 20)
  uint32_t frame_driver() const { return guest::LoadU32(base, address + 4); }
  void set_frame_driver(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t frame_driver_address() const { return address + 4; }
};

// embedded; measure: Thread::Begin.r5@+4
struct GameThreadArgWordAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/frame-driver.md's code uses.
struct FrameDriverGlobals {
  // the kernel's thread object type, for ObReferenceObjectByHandle
  static uint32_t thread_object_type(uint8_t* base) { return guest::LoadU32(base, 0x820007FCu); }
  static void set_thread_object_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x820007FCu, v); }
  static constexpr uint32_t thread_object_type_address() { return 0x820007FCu; }
  // the color rectangles' render queue (the scene's +64)
  static uint32_t color_rect_queue(uint8_t* base) { return guest::LoadU32(base, 0x826D83D0u); }
  static void set_color_rect_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D0u, v); }
  static constexpr uint32_t color_rect_queue_address() { return 0x826D83D0u; }
  // the color rectangles' command list (the scene's +8)
  static uint32_t color_rect_list(uint8_t* base) { return guest::LoadU32(base, 0x826D83D4u); }
  static void set_color_rect_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83D4u, v); }
  static constexpr uint32_t color_rect_list_address() { return 0x826D83D4u; }
  // set to the identity by DrawGlobals::Init
  static constexpr uint32_t identity_matrix_address() { return 0x8265ECA0u; }
  // DrawGlobals::Init's float4 defaults
  static constexpr uint32_t default_float4s_address() { return 0x8265ECE0u; }
  // two float4 defaults DrawGlobals::Init sets last
  static constexpr uint32_t draw_defaults_address() { return 0x8265ED20u; }
  // eleven tables of seven vertex element list pointers (+28k)
  static constexpr uint32_t declaration_elements_address() { return 0x8265EACCu; }
  // the work memory's current buffer (0 or 1)
  static uint32_t work_buffer(uint8_t* base) { return guest::LoadU32(base, 0x8296B780u); }
  static void set_work_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B780u, v); }
  static constexpr uint32_t work_buffer_address() { return 0x8296B780u; }
  // a word per buffer zeroed by WorkMem::Flip
  static constexpr uint32_t work_cleared_address() { return 0x8296B784u; }
  // a 1 MB Memory::AllocVirtual block (count x 64 bytes)
  static uint32_t work_virtual0(uint8_t* base) { return guest::LoadU32(base, 0x8296B78Cu); }
  static void set_work_virtual0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B78Cu, v); }
  static constexpr uint32_t work_virtual0_address() { return 0x8296B78Cu; }
  // the second
  static uint32_t work_virtual1(uint8_t* base) { return guest::LoadU32(base, 0x8296B790u); }
  static void set_work_virtual1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B790u, v); }
  static constexpr uint32_t work_virtual1_address() { return 0x8296B790u; }
  // 16,384
  static uint32_t work_count(uint8_t* base) { return guest::LoadU32(base, 0x8296B794u); }
  static void set_work_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B794u, v); }
  static constexpr uint32_t work_count_address() { return 0x8296B794u; }
  // per buffer
  static constexpr uint32_t work_headers_used_address() { return 0x8296B798u; }
  // per buffer, rounded to 128
  static constexpr uint32_t work_bytes_used_address() { return 0x8296B7A0u; }
  // per buffer: the 2 MB physical blocks
  static constexpr uint32_t work_start_address() { return 0x8296B7A8u; }
  // each buffer's bytes (2 MB)
  static uint32_t work_capacity(uint8_t* base) { return guest::LoadU32(base, 0x8296B7B0u); }
  static void set_work_capacity(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B7B0u, v); }
  static constexpr uint32_t work_capacity_address() { return 0x8296B7B0u; }
  // the 77 vertex declarations, eleven tables of seven at 0x8296B8D4 - 28k
  static constexpr uint32_t declarations_address() { return 0x8296B7BCu; }
  // a render queue sort key, 1 at boot
  static uint32_t sort_key_b(uint8_t* base) { return guest::LoadU32(base, 0x8296B900u); }
  static void set_sort_key_b(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B900u, v); }
  static constexpr uint32_t sort_key_b_address() { return 0x8296B900u; }
  // a render queue sort key, 0 at boot
  static uint32_t sort_key_a(uint8_t* base) { return guest::LoadU32(base, 0x8296B90Cu); }
  static void set_sort_key_a(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B90Cu, v); }
  static constexpr uint32_t sort_key_a_address() { return 0x8296B90Cu; }
  // the scene's command list (scene + 8), for the recorders
  static uint32_t scene_list(uint8_t* base) { return guest::LoadU32(base, 0x8296B918u); }
  static void set_scene_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B918u, v); }
  static constexpr uint32_t scene_list_address() { return 0x8296B918u; }
  // the scene's render queue (scene + 64)
  static uint32_t scene_queue(uint8_t* base) { return guest::LoadU32(base, 0x8296B91Cu); }
  static void set_scene_queue(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B91Cu, v); }
  static constexpr uint32_t scene_queue_address() { return 0x8296B91Cu; }
  // DrawGlobals::Init's lighting float4s (to 0x8296BCDF)
  static constexpr uint32_t lighting_defaults_address() { return 0x8296B950u; }
  // cleared by VideoOutput::BeginFrame once a frame
  static uint32_t vblank_count(uint8_t* base) { return guest::LoadU32(base, 0x8296BD30u); }
  static void set_vblank_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BD30u, v); }
  static constexpr uint32_t vblank_count_address() { return 0x8296BD30u; }
  // flipped once a tick by sub_82591838 (0 when it was 1, else 1)
  static uint32_t primitive_flip(uint8_t* base) { return guest::LoadU32(base, 0x82970BC4u); }
  static void set_primitive_flip(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970BC4u, v); }
  static constexpr uint32_t primitive_flip_address() { return 0x82970BC4u; }
  // the primitive vertex declaration (probably)
  static uint32_t primitive_declaration(uint8_t* base) { return guest::LoadU32(base, 0x82970BC8u); }
  static void set_primitive_declaration(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970BC8u, v); }
  static constexpr uint32_t primitive_declaration_address() { return 0x82970BC8u; }
  // the scene's render queue again
  static uint32_t scene_queue_2(uint8_t* base) { return guest::LoadU32(base, 0x82970BD4u); }
  static void set_scene_queue_2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970BD4u, v); }
  static constexpr uint32_t scene_queue_2_address() { return 0x82970BD4u; }
  // the scene's command list again
  static uint32_t scene_list_2(uint8_t* base) { return guest::LoadU32(base, 0x82970BD8u); }
  static void set_scene_list_2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970BD8u, v); }
  static constexpr uint32_t scene_list_2_address() { return 0x82970BD8u; }
  // 1 once the primitive declaration is made
  static uint32_t primitive_declaration_ready(uint8_t* base) { return guest::LoadU32(base, 0x82970BDCu); }
  static void set_primitive_declaration_ready(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970BDCu, v); }
  static constexpr uint32_t primitive_declaration_ready_address() { return 0x82970BDCu; }
};

}  // namespace schema
