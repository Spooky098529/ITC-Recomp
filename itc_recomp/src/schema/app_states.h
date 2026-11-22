// The views of docs/functions/app-states.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/app-states.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: App, AppStates.+4
struct App : View {
  using View::View;
  static constexpr uint32_t kSize = 132;
  static constexpr uint32_t kVtable = 0x82000EA0;
  // 0x82000EA0
  template <typename T = AppVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: App::ForEachMessage's and ForEachRequest's r3 is the result functor, not the app
  uint64_t fault_for_each_result_0() const { return guest::LoadU64(base, address + 0); }
  void set_fault_for_each_result_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_for_each_result_0_address() const { return address + 0; }
  // the +4 interface's: 0x82000E94 (base 0x82000ABC)
  uint32_t vtable_4() const { return guest::LoadU32(base, address + 4); }
  void set_vtable_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vtable_4_address() const { return address + 4; }
  // the +8 interface's: 0x82000E80 (base 0x82000A74)
  uint32_t vtable_8() const { return guest::LoadU32(base, address + 8); }
  void set_vtable_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t vtable_8_address() const { return address + 8; }
  // measure fault: as fault_for_each_result_0
  uint64_t fault_for_each_result_8() const { return guest::LoadU64(base, address + 8); }
  void set_fault_for_each_result_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t fault_for_each_result_8_address() const { return address + 8; }
  // the +12 interface's: 0x82000E70 (base 0x82000D48)
  uint32_t vtable_12() const { return guest::LoadU32(base, address + 12); }
  void set_vtable_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t vtable_12_address() const { return address + 12; }
  // the +16 interface's: 0x82000E38 (base 0x82000DD0); the owner a layer keeps (StateLayer +8)
  uint32_t vtable_16() const { return guest::LoadU32(base, address + 16); }
  void set_vtable_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t vtable_16_address() const { return address + 16; }
  // the +20 interface's: 0x82000E08 (base 0x82000DA0); the interface the framework keeps at +120 (AppInterface)
  template <typename T = AppInterfaceVtable>
  T interface_vtable() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_interface_vtable(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_interface_vtable(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t interface_vtable_address() const { return address + 20; }
  // the framework's +36 interface (its slot 1 the input manager); also App::FrameDriver's +20 read (r3 = app + 4)
  template <typename T = AppFrameworkIface>
  T framework_input() const { return T(base, guest::LoadU32(base, address + 24)); }
  void set_framework_input(const View& v) const { guest::StoreU32(base, address + 24, v.address); }
  void set_framework_input(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t framework_input_address() const { return address + 24; }
  // a handler map (std::map of delegates by key), run while the toggle is clear or a step was asked: its first word, not read
  uint32_t handlers() const { return guest::LoadU32(base, address + 28); }
  void set_handlers(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t handlers_address() const { return address + 28; }
  template <typename T = AppHandlerMapHead>
  T handlers_head() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_handlers_head(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_handlers_head(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t handlers_head_address() const { return address + 32; }
  uint32_t handlers_size() const { return guest::LoadU32(base, address + 36); }
  void set_handlers_size(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t handlers_size_address() const { return address + 36; }
  // the handler map run while the toggle is set: its first word, not read
  uint32_t toggled_handlers() const { return guest::LoadU32(base, address + 40); }
  void set_toggled_handlers(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t toggled_handlers_address() const { return address + 40; }
  template <typename T = AppHandlerMapHead>
  T toggled_handlers_head() const { return T(base, guest::LoadU32(base, address + 44)); }
  void set_toggled_handlers_head(const View& v) const { guest::StoreU32(base, address + 44, v.address); }
  void set_toggled_handlers_head(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t toggled_handlers_head_address() const { return address + 44; }
  uint32_t toggled_handlers_size() const { return guest::LoadU32(base, address + 48); }
  void set_toggled_handlers_size(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t toggled_handlers_size_address() const { return address + 48; }
  // the handler map run every pass: its first word, not read
  uint32_t always_handlers() const { return guest::LoadU32(base, address + 52); }
  void set_always_handlers(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t always_handlers_address() const { return address + 52; }
  template <typename T = AppHandlerMapHead>
  T always_handlers_head() const { return T(base, guest::LoadU32(base, address + 56)); }
  void set_always_handlers_head(const View& v) const { guest::StoreU32(base, address + 56, v.address); }
  void set_always_handlers_head(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t always_handlers_head_address() const { return address + 56; }
  uint32_t always_handlers_size() const { return guest::LoadU32(base, address + 60); }
  void set_always_handlers_size(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t always_handlers_size_address() const { return address + 60; }
  // the listener map (given the messages): its first word, not read
  uint32_t listeners() const { return guest::LoadU32(base, address + 64); }
  void set_listeners(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t listeners_address() const { return address + 64; }
  template <typename T = AppHandlerMapHead>
  T listeners_head() const { return T(base, guest::LoadU32(base, address + 68)); }
  void set_listeners_head(const View& v) const { guest::StoreU32(base, address + 68, v.address); }
  void set_listeners_head(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t listeners_head_address() const { return address + 68; }
  uint32_t listeners_size() const { return guest::LoadU32(base, address + 72); }
  void set_listeners_size(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t listeners_size_address() const { return address + 72; }
  // the message vector (MessageVector) starts here: its first word, not read
  uint32_t messages() const { return guest::LoadU32(base, address + 76); }
  void set_messages(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t messages_address() const { return address + 76; }
  template <typename T = AppMessageWord>
  T messages_first() const { return T(base, guest::LoadU32(base, address + 80)); }
  void set_messages_first(const View& v) const { guest::StoreU32(base, address + 80, v.address); }
  void set_messages_first(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t messages_first_address() const { return address + 80; }
  template <typename T = AppMessageWord>
  T messages_last() const { return T(base, guest::LoadU32(base, address + 84)); }
  void set_messages_last(const View& v) const { guest::StoreU32(base, address + 84, v.address); }
  void set_messages_last(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t messages_last_address() const { return address + 84; }
  template <typename T = AppMessageWord>
  T messages_end() const { return T(base, guest::LoadU32(base, address + 88)); }
  void set_messages_end(const View& v) const { guest::StoreU32(base, address + 88, v.address); }
  void set_messages_end(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t messages_end_address() const { return address + 88; }
  // the message being delivered (the listeners get its address)
  uint32_t message() const { return guest::LoadU32(base, address + 92); }
  void set_message(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t message_address() const { return address + 92; }
  // the request vector (12-byte requests {type, key, delegate}) starts here: its first word, not read by the app's code; the measure's +96 reads are the +20 interface's +96, the state stack (App::IsRunning, App::Start: a measure fault)
  uint32_t requests() const { return guest::LoadU32(base, address + 96); }
  void set_requests(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t requests_address() const { return address + 96; }
  uint32_t requests_first() const { return guest::LoadU32(base, address + 100); }
  void set_requests_first(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t requests_first_address() const { return address + 100; }
  uint32_t requests_last() const { return guest::LoadU32(base, address + 104); }
  void set_requests_last(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t requests_last_address() const { return address + 104; }
  uint32_t requests_end() const { return guest::LoadU32(base, address + 108); }
  void set_requests_end(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t requests_end_address() const { return address + 108; }
  // the toggle player 1's Start flips (message 0 when set, 2 when cleared)
  uint8_t toggled() const { return guest::LoadU8(base, address + 112); }
  void set_toggled(uint8_t v) const { guest::StoreU8(base, address + 112, v); }
  uint32_t toggled_address() const { return address + 112; }
  // a step asked (Back while toggled, message 1); cleared after the pass
  uint8_t step() const { return guest::LoadU8(base, address + 113); }
  void set_step(uint8_t v) const { guest::StoreU8(base, address + 113, v); }
  uint32_t step_address() const { return address + 113; }
  // 0 at construction, not read
  uint8_t unknown_114() const { return guest::LoadU8(base, address + 114); }
  void set_unknown_114(uint8_t v) const { guest::StoreU8(base, address + 114, v); }
  uint32_t unknown_114_address() const { return address + 114; }
  // the state stack (AppStates::Create)
  template <typename T = AppStates>
  T states() const { return T(base, guest::LoadU32(base, address + 116)); }
  void set_states(const View& v) const { guest::StoreU32(base, address + 116, v.address); }
  void set_states(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t states_address() const { return address + 116; }
  // a second camera class (368 bytes, sub_820A8B88)
  uint32_t camera() const { return guest::LoadU32(base, address + 120); }
  void set_camera(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t camera_address() const { return address + 120; }
  // AudioManager::Create
  uint32_t sound_manager() const { return guest::LoadU32(base, address + 124); }
  void set_sound_manager(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t sound_manager_address() const { return address + 124; }
  // 312 bytes, Movie::Movie
  uint32_t movie() const { return guest::LoadU32(base, address + 128); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t movie_address() const { return address + 128; }
};

// image; measure: AppStates.+4.+0
struct AppVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: the +20 interface (AppInterface)
  uint32_t interface() const { return guest::LoadU32(base, address + 4); }
  void set_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t interface_address() const { return address + 4; }
  // slot 2: the interface at app + 16, a layer's owner
  uint32_t owner() const { return guest::LoadU32(base, address + 8); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t owner_address() const { return address + 8; }
};

// embedded; measure: AppStates::OnMessage.r4
struct AppInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  template <typename T = AppInterfaceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the app's message vector (app + 76)
  uint32_t messages() const { return guest::LoadU32(base, address + 56); }
  void set_messages(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t messages_address() const { return address + 56; }
  // the app's request vector (app + 96)
  uint32_t requests() const { return guest::LoadU32(base, address + 76); }
  void set_requests(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t requests_address() const { return address + 76; }
  // the app's +116
  template <typename T = AppStates>
  T states() const { return T(base, guest::LoadU32(base, address + 96)); }
  void set_states(const View& v) const { guest::StoreU32(base, address + 96, v.address); }
  void set_states(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t states_address() const { return address + 96; }
};

// image; measure: App.+20, AppStates::OnMessage.r4.+0
struct AppInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // measure fault: App::FrameDriver's read is the framework interface's vtable (r3 = app + 4, its +20 the app's +24)
  uint32_t fault_framework_vtable() const { return guest::LoadU32(base, address + 0); }
  void set_fault_framework_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_framework_vtable_address() const { return address + 0; }
  // App::AddListener
  uint32_t add_listener() const { return guest::LoadU32(base, address + 4); }
  void set_add_listener(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t add_listener_address() const { return address + 4; }
  // App::AddAlwaysHandler
  uint32_t add_always_handler() const { return guest::LoadU32(base, address + 8); }
  void set_add_always_handler(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t add_always_handler_address() const { return address + 8; }
  // App::AddToggledHandler
  uint32_t add_toggled_handler() const { return guest::LoadU32(base, address + 12); }
  void set_add_toggled_handler(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t add_toggled_handler_address() const { return address + 12; }
  // App::AddHandler
  uint32_t add_handler() const { return guest::LoadU32(base, address + 16); }
  void set_add_handler(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t add_handler_address() const { return address + 16; }
  // probably (AppStates::OnMessage's teardown, listener 9); never ran
  uint32_t remove_listener() const { return guest::LoadU32(base, address + 20); }
  void set_remove_listener(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t remove_listener_address() const { return address + 20; }
  // probably; never ran
  uint32_t remove_always_handler() const { return guest::LoadU32(base, address + 24); }
  void set_remove_always_handler(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t remove_always_handler_address() const { return address + 24; }
  // probably (key 2); never ran
  uint32_t remove_toggled_handler() const { return guest::LoadU32(base, address + 28); }
  void set_remove_toggled_handler(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t remove_toggled_handler_address() const { return address + 28; }
  // probably (key 8); never ran
  uint32_t remove_handler() const { return guest::LoadU32(base, address + 32); }
  void set_remove_handler(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t remove_handler_address() const { return address + 32; }
  // App::Start
  uint32_t start() const { return guest::LoadU32(base, address + 36); }
  void set_start(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t start_address() const { return address + 36; }
  // App::SendMessage
  uint32_t send_message() const { return guest::LoadU32(base, address + 40); }
  void set_send_message(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t send_message_address() const { return address + 40; }
  // App::IsRunning
  uint32_t is_running() const { return guest::LoadU32(base, address + 44); }
  void set_is_running(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t is_running_address() const { return address + 44; }
};

// embedded; measure: App.+24
struct AppFrameworkIface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AppFrameworkIfaceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: App.+24.+0
struct AppFrameworkIfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 1: the input manager
  uint32_t input_manager() const { return guest::LoadU32(base, address + 4); }
  void set_input_manager(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t input_manager_address() const { return address + 4; }
};

// allocated; measure: App.+32, App.+44, App.+56, App.+68
struct AppHandlerMapHead : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the leftmost node (begin)
  uint32_t left() const { return guest::LoadU32(base, address + 0); }
  void set_left(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t left_address() const { return address + 0; }
};

// embedded; measure: App::AddHandler.r4, App::AddToggledHandler.r4, App::AddAlwaysHandler.r4, App::AddListener.r4
struct AppHandlerKey : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t key() const { return guest::LoadU32(base, address + 0); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_address() const { return address + 0; }
};

// embedded; measure: MessageVector::PushBack.r4, MessageVector::InsertN.r6, AppStates::OnMessage.r5, MessageVector.+8
struct AppMessageWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 toggle set, 1 a step, 2 toggle cleared, 5 and 6 the teardowns
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// allocated; measure: App::AddHandler.r5, App::AddToggledHandler.r5, App::AddAlwaysHandler.r5, App::AddListener.r5, new<12>@Delegate::Clone
struct AppDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0x82002850 (base 0x82000ABC)
  template <typename T = AppDelegateVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  uint32_t object() const { return guest::LoadU32(base, address + 4); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_address() const { return address + 4; }
  uint32_t function() const { return guest::LoadU32(base, address + 8); }
  void set_function(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t function_address() const { return address + 8; }
};

// image; measure: App::AddHandler.r5.+0, App::AddToggledHandler.r5.+0, App::AddAlwaysHandler.r5.+0, App::AddListener.r5.+0
struct AppDelegateVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the base's sub_82094D20, never ran
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // Delegate::Invoke
  uint32_t invoke() const { return guest::LoadU32(base, address + 4); }
  void set_invoke(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t invoke_address() const { return address + 4; }
  // Delegate::Clone
  uint32_t clone() const { return guest::LoadU32(base, address + 8); }
  void set_clone(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t clone_address() const { return address + 8; }
  // sub_820AC9E8, never ran
  uint32_t deleting_destructor() const { return guest::LoadU32(base, address + 12); }
  void set_deleting_destructor(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t deleting_destructor_address() const { return address + 12; }
};

// embedded; measure: MessageVector
struct MessageVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // measure fault: Erase's and Insert's r3 is the result iterator (vector, element), not the vector
  uint64_t fault_result() const { return guest::LoadU64(base, address + 0); }
  void set_fault_result(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_result_address() const { return address + 0; }
  template <typename T = AppMessageWord>
  T first() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_first(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_first(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_address() const { return address + 4; }
  template <typename T = AppMessageWord>
  T last() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_last(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_last(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t last_address() const { return address + 8; }
  template <typename T = AppMessageWord>
  T end() const { return T(base, guest::LoadU32(base, address + 12)); }
  void set_end(const View& v) const { guest::StoreU32(base, address + 12, v.address); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t end_address() const { return address + 12; }
};

// allocated; measure: AppStates, App.+96, sub_820AD8D0.ret
struct AppStates : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr uint32_t kVtable = 0x820028F8;
  template <typename T = AppStatesVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = App>
  T app() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_app(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_app(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t app_address() const { return address + 4; }
  // the root layer (no states; one child in play); sub_820AD8D0.ret's read is AppStates::OnMessage's r3, the stack (a measure fault)
  template <typename T = StateLayer>
  T root() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_root(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_root(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t root_address() const { return address + 8; }
};

// image; measure: App.+96.+0
struct AppStatesVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // sub_820AE2F8, never ran
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // AppStates::AddLayer
  uint32_t add_layer() const { return guest::LoadU32(base, address + 4); }
  void set_add_layer(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t add_layer_address() const { return address + 4; }
  // sub_820AE198, never ran
  uint32_t count_states() const { return guest::LoadU32(base, address + 8); }
  void set_count_states(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t count_states_address() const { return address + 8; }
  // AppStates::IsEmpty
  uint32_t is_empty() const { return guest::LoadU32(base, address + 12); }
  void set_is_empty(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t is_empty_address() const { return address + 12; }
};

// embedded; measure: AppStates::AppStates.r4
struct AppStatesSetup : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = App>
  T app() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_app(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_app(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t app_address() const { return address + 0; }
};

// allocated; measure: StateLayer, AppStates.+8, sub_820ADBD8.r3, sub_820AD350.ret, sub_820AD5B0.r3, sub_820AD8D0.r3, sub_820ADA70.r3, sub_820ADB28.r3, sub_820ADB80.r3, sub_820ACD60.r4, sub_820ACD80.r4, sub_820ACDE8.r4, StateLayer::AddChild.r4
struct StateLayer : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr uint32_t kVtable = 0x820028E0;
  // 0x820028E0
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x820028B0 (the state base's 0x82002880 first): the layer + 4 is a state
  template <typename T = AppStateVtable>
  T state_vtable() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_state_vtable(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_state_vtable(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t state_vtable_address() const { return address + 4; }
  // the app's interface at app + 16 (the app's slot 2)
  uint32_t owner() const { return guest::LoadU32(base, address + 8); }
  void set_owner(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t owner_address() const { return address + 8; }
  // the child layers (a PtrList): its first word, not read
  uint32_t children() const { return guest::LoadU32(base, address + 12); }
  void set_children(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t children_address() const { return address + 12; }
  template <typename T = StateListNode>
  T children_head() const { return T(base, guest::LoadU32(base, address + 16)); }
  void set_children_head(const View& v) const { guest::StoreU32(base, address + 16, v.address); }
  void set_children_head(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t children_head_address() const { return address + 16; }
  uint32_t children_size() const { return guest::LoadU32(base, address + 20); }
  void set_children_size(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t children_size_address() const { return address + 20; }
  // the states (a PtrList, the top at the front): its first word, not read
  uint32_t states() const { return guest::LoadU32(base, address + 24); }
  void set_states(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t states_address() const { return address + 24; }
  template <typename T = StateListNode>
  T states_head() const { return T(base, guest::LoadU32(base, address + 28)); }
  void set_states_head(const View& v) const { guest::StoreU32(base, address + 28, v.address); }
  void set_states_head(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t states_head_address() const { return address + 28; }
  uint32_t states_size() const { return guest::LoadU32(base, address + 32); }
  void set_states_size(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t states_size_address() const { return address + 32; }
  // the queued operations (an OpList): its first word, not read
  uint32_t ops() const { return guest::LoadU32(base, address + 36); }
  void set_ops(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t ops_address() const { return address + 36; }
  template <typename T = AppOpListNode>
  T ops_head() const { return T(base, guest::LoadU32(base, address + 40)); }
  void set_ops_head(const View& v) const { guest::StoreU32(base, address + 40, v.address); }
  void set_ops_head(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t ops_head_address() const { return address + 40; }
  uint32_t ops_size() const { return guest::LoadU32(base, address + 44); }
  void set_ops_size(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t ops_size_address() const { return address + 44; }
};

// allocated; measure: StateLayer.+28.+0.+8, sub_820AD5B0.r3.+28.+0.+8
struct AppState : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AppStateVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: StateLayer.+28.+0.+8.+0, sub_820AD5B0.r3.+28.+0.+8.+0, sub_820ACD60.r4.+4, sub_820ACD80.r4.+4, sub_820ACDE8.r4.+4, StateLayer::AddChild.r4.+4, AppStates.+8.+4
struct AppStateVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // sub_820ACCF0 in the base
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  uint32_t on_enter() const { return guest::LoadU32(base, address + 4); }
  void set_on_enter(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t on_enter_address() const { return address + 4; }
  uint32_t on_exit() const { return guest::LoadU32(base, address + 8); }
  void set_on_exit(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t on_exit_address() const { return address + 8; }
  // message 0
  uint32_t on_toggle_on() const { return guest::LoadU32(base, address + 12); }
  void set_on_toggle_on(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t on_toggle_on_address() const { return address + 12; }
  // message 1
  uint32_t on_step() const { return guest::LoadU32(base, address + 16); }
  void set_on_step(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t on_step_address() const { return address + 16; }
  // message 2
  uint32_t on_toggle_off() const { return guest::LoadU32(base, address + 20); }
  void set_on_toggle_off(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t on_toggle_off_address() const { return address + 20; }
  uint32_t on_covered() const { return guest::LoadU32(base, address + 24); }
  void set_on_covered(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t on_covered_address() const { return address + 24; }
  uint32_t on_uncovered() const { return guest::LoadU32(base, address + 28); }
  void set_on_uncovered(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t on_uncovered_address() const { return address + 28; }
  uint32_t update() const { return guest::LoadU32(base, address + 32); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t update_address() const { return address + 32; }
  uint32_t update_toggled() const { return guest::LoadU32(base, address + 36); }
  void set_update_toggled(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t update_toggled_address() const { return address + 36; }
  uint32_t tick() const { return guest::LoadU32(base, address + 40); }
  void set_tick(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t tick_address() const { return address + 40; }
  // sub_825C8390 (1) in the base
  uint32_t is_empty() const { return guest::LoadU32(base, address + 44); }
  void set_is_empty(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t is_empty_address() const { return address + 44; }
};

// embedded; measure: PtrList, PtrList::Erase.r4
struct PtrList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // measure fault: PtrList::ForEach's and Erase's r3 is the result (functor or iterator), not the list; HandleList::Remove's and sub_82118850's reads are an iterator's
  uint64_t fault_result_0() const { return guest::LoadU64(base, address + 0); }
  void set_fault_result_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_result_0_address() const { return address + 0; }
  // the head node (new<?>@PtrList::AllocateNodes, input's)
  template <typename T = StateListNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
  // measure fault: as fault_result_0
  uint64_t fault_result_8() const { return guest::LoadU64(base, address + 8); }
  void set_fault_result_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t fault_result_8_address() const { return address + 8; }
};

// allocated; measure: StateLayer.+16, StateLayer.+28, StateLayer.+28.+0, sub_820AD5B0.r3.+16, sub_820AD5B0.r3.+28, sub_820AD5B0.r3.+28.+0, sub_820AD8D0.r3.+16, sub_820AD8D0.r3.+28, List.+4.+0
struct StateListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = StateListNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = StateListNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // a child layer (its +4 a state: AppState) or a state
  uint32_t value() const { return guest::LoadU32(base, address + 8); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t value_address() const { return address + 8; }
};

// embedded; measure: OpList
struct OpList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // measure fault: OpList::ForEach's r3 is the result functor, not the list
  uint64_t fault_result_0() const { return guest::LoadU64(base, address + 0); }
  void set_fault_result_0(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t fault_result_0_address() const { return address + 0; }
  template <typename T = AppOpListNode>
  T head() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_head(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t head_address() const { return address + 4; }
  uint32_t size() const { return guest::LoadU32(base, address + 8); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t size_address() const { return address + 8; }
  // measure fault: as fault_result_0
  uint64_t fault_result_8() const { return guest::LoadU64(base, address + 8); }
  void set_fault_result_8(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t fault_result_8_address() const { return address + 8; }
};

// allocated; measure: OpList::BuyNode.ret, OpList::BuyNode.ret.+4, StateLayer.+40, sub_820ADA70.r3.+40, sub_820ADB28.r3.+40, sub_820ADB80.r3.+40, new<?>@OpList::AllocateNodes
struct AppOpListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  template <typename T = AppOpListNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = AppOpListNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // the operation: its type (high word), then a state
  uint64_t op() const { return guest::LoadU64(base, address + 8); }
  void set_op(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t op_address() const { return address + 8; }
  // 0 add a child layer, 1 pop, 2 push, 3 replace
  uint32_t op_type() const { return guest::LoadU32(base, address + 8); }
  void set_op_type(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t op_type_address() const { return address + 8; }
  uint32_t op_state() const { return guest::LoadU32(base, address + 12); }
  void set_op_state(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t op_state_address() const { return address + 12; }
};

// embedded; measure: OpList::BuyNode.r6
struct AppStateOp : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0 add a child layer, 1 pop, 2 push, 3 replace
  uint32_t type() const { return guest::LoadU32(base, address + 0); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  uint32_t state() const { return guest::LoadU32(base, address + 4); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t state_address() const { return address + 4; }
};

// embedded; measure: sub_820AD808.r3
struct AppMemberFunctor : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the function (high word) and the object
  uint64_t function_object() const { return guest::LoadU64(base, address + 0); }
  void set_function_object(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t function_object_address() const { return address + 0; }
  // the adjustment (high word)
  uint64_t adjust() const { return guest::LoadU64(base, address + 8); }
  void set_adjust(uint64_t v) const { guest::StoreU64(base, address + 8, v); }
  uint32_t adjust_address() const { return address + 8; }
};

// allocated; measure: sub_82098F30.r3, sub_82098FF8.ret@+792
struct InputManagerAtTeardown : View {
  using View::View;
  static constexpr uint32_t kSize = 3976;
  // 0x82000F48, then the base's 0x82000A38
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the last of its four lists' head (the lists from +3932, 12 bytes each), freed and cleared
  uint32_t list_head() const { return guest::LoadU32(base, address + 3972); }
  void set_list_head(uint32_t v) const { guest::StoreU32(base, address + 3972, v); }
  uint32_t list_head_address() const { return address + 3972; }
};

// allocated; measure: sub_8258CEA0.r3, sub_8258CF40.r3, sub_82592178.r3
struct InputProcessorAtTeardown : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x8206C0C4, then the base's 0x8206CC00
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// allocated; measure: sub_82591BB8.r3
struct PhysicalPadAtTeardown : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x8206CBF4, then 0x8206CBE4
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

}  // namespace schema
