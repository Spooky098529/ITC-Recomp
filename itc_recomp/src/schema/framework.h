// The views of docs/functions/framework.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/framework.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Framework, sub_820908F8.ret, App::Run.r4, sub_82090350.r3.+0, sub_82090760.r3, sub_82090768.r3
#pragma pack(push, 1)
struct FrameworkData {
  uint32_t vtable = 0;  // +0
  uint32_t vtable_4 = 0;  // +4
  uint32_t vtable_8 = 0;  // +8
  uint32_t vtable_12 = 0;  // +12
  uint32_t vtable_16 = 0;  // +16
  uint32_t vtable_20 = 0;  // +20
  uint32_t vtable_24 = 0;  // +24
  uint32_t vtable_28 = 0;  // +28
  uint32_t vtable_32 = 0;  // +32
  uint32_t vtable_36 = 0;  // +36
  uint32_t vtable_40 = 0;  // +40
  uint32_t vtable_44 = 0;  // +44
  uint32_t vtable_48 = 0;  // +48
  uint32_t vtable_52 = 0;  // +52
  uint32_t vtable_56 = 0;  // +56
  uint32_t vtable_60 = 0;  // +60
  uint32_t run_loop = 0;  // +64
  uint32_t worker_pool = 0;  // +68
  uint32_t worker_pool_2 = 0;  // +72
  uint32_t frame_driver = 0;  // +76
  uint32_t scene = 0;  // +80
  uint32_t frame_start = 0;  // +84
  uint32_t input_manager = 0;  // +88
  uint32_t debug_text = 0;  // +92
  uint32_t unknown_96 = 0;  // +96
  uint32_t unknown_100 = 0;  // +100
  uint32_t unknown_104 = 0;  // +104
  uint32_t frame_passes = 0;  // +108
  uint32_t pool_maker = 0;  // +112
  uint32_t morph_job_maker = 0;  // +116
  uint32_t app = 0;  // +120
  uint32_t save_devices = 0;  // +124
  uint32_t users = 0;  // +128
  uint8_t unknown_132 = 0;  // +132
  uint8_t restart_requested = 0;  // +133
  uint8_t app_finished = 0;  // +134
  uint8_t pad_135[1] = {};
};
#pragma pack(pop)
static_assert(sizeof(FrameworkData) == 136);
inline constexpr memory::LayoutField kFrameworkFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}};
inline constexpr memory::Layout kFrameworkLayout = {kFrameworkFields, 33};

struct Framework : View {
  using View::View;
  static constexpr uint32_t kSize = 136;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82000B90;
  FrameworkData* data() const { return static_cast<FrameworkData*>(memory::HandleObject(address)); }
  static Framework New(uint8_t* base) {
    return Framework(base, memory::MakeHandle(new FrameworkData(), sizeof(FrameworkData), "Framework", &kFrameworkLayout));
  }
  template <uint32_t Stride = kSize>
  static Framework NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FrameworkData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrameworkData*>(::operator new(sizeof(FrameworkData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrameworkData();
    return Framework(base, memory::MakeHandle(p, sizeof(FrameworkData) * count, "Framework", &kFrameworkLayout, sizeof(FrameworkData)));
  }
  Framework at(uint32_t i) const { return Framework(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFrameworkLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFrameworkLayout, offset, width, v); }
  // 0x82000B90
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = FrameworkVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // 0x82000B8C (slot 0 only)
  uint32_t vtable_4_address() const { return address + 4; }
  uint32_t vtable_4() const { return data()->vtable_4; }
  void set_vtable_4(uint32_t v) const { data()->vtable_4 = v; }
  uint32_t vtable_4_bits() const { return data()->vtable_4; }
  void set_vtable_4_bits(uint32_t v) const { data()->vtable_4 = v; }
  // 0x82000B84: slot 1 the frame driver; the scene's interface
  uint32_t vtable_8_address() const { return address + 8; }
  uint32_t vtable_8() const { return data()->vtable_8; }
  void set_vtable_8(uint32_t v) const { data()->vtable_8 = v; }
  uint32_t vtable_8_bits() const { return data()->vtable_8; }
  void set_vtable_8_bits(uint32_t v) const { data()->vtable_8 = v; }
  // 0x82000B80 (slot 0 only): the worker pools' interface
  uint32_t vtable_12_address() const { return address + 12; }
  uint32_t vtable_12() const { return data()->vtable_12; }
  void set_vtable_12(uint32_t v) const { data()->vtable_12 = v; }
  uint32_t vtable_12_bits() const { return data()->vtable_12; }
  void set_vtable_12_bits(uint32_t v) const { data()->vtable_12 = v; }
  // 0x82000B7C (slot 0 only): the input manager's interface
  uint32_t vtable_16_address() const { return address + 16; }
  uint32_t vtable_16() const { return data()->vtable_16; }
  void set_vtable_16(uint32_t v) const { data()->vtable_16 = v; }
  uint32_t vtable_16_bits() const { return data()->vtable_16; }
  void set_vtable_16_bits(uint32_t v) const { data()->vtable_16 = v; }
  // 0x82000B70: 1 the frame driver, 2 the scene; the debug text's interface
  uint32_t vtable_20_address() const { return address + 20; }
  uint32_t vtable_20() const { return data()->vtable_20; }
  void set_vtable_20(uint32_t v) const { data()->vtable_20 = v; }
  uint32_t vtable_20_bits() const { return data()->vtable_20; }
  void set_vtable_20_bits(uint32_t v) const { data()->vtable_20 = v; }
  // 0x82000B4C: 1 +68, 2 +72, 3 +88, 4 +76, 5 +80, 6 +92, 7 +100, 8 +104
  uint32_t vtable_24_address() const { return address + 24; }
  uint32_t vtable_24() const { return data()->vtable_24; }
  void set_vtable_24(uint32_t v) const { data()->vtable_24 = v; }
  uint32_t vtable_24_bits() const { return data()->vtable_24; }
  void set_vtable_24_bits(uint32_t v) const { data()->vtable_24 = v; }
  // 0x82000B38: 1 +88, 2 +76, 3 +80, 4 +104
  uint32_t vtable_28_address() const { return address + 28; }
  uint32_t vtable_28() const { return data()->vtable_28; }
  void set_vtable_28(uint32_t v) const { data()->vtable_28 = v; }
  uint32_t vtable_28_bits() const { return data()->vtable_28; }
  void set_vtable_28_bits(uint32_t v) const { data()->vtable_28 = v; }
  // 0x82000B28: 1 +76, 2 +88, 3 +92
  uint32_t vtable_32_address() const { return address + 32; }
  uint32_t vtable_32() const { return data()->vtable_32; }
  void set_vtable_32(uint32_t v) const { data()->vtable_32 = v; }
  uint32_t vtable_32_bits() const { return data()->vtable_32; }
  void set_vtable_32_bits(uint32_t v) const { data()->vtable_32 = v; }
  // 0x82000AFC: the app's interface (1 the input manager ... 9 +128, 10 +124)
  uint32_t vtable_36_address() const { return address + 36; }
  uint32_t vtable_36() const { return data()->vtable_36; }
  void set_vtable_36(uint32_t v) const { data()->vtable_36 = v; }
  uint32_t vtable_36_bits() const { return data()->vtable_36; }
  void set_vtable_36_bits(uint32_t v) const { data()->vtable_36 = v; }
  // 0x82000AF4: 1 the debug text; the frame start's interface
  uint32_t vtable_40_address() const { return address + 40; }
  uint32_t vtable_40() const { return data()->vtable_40; }
  void set_vtable_40(uint32_t v) const { data()->vtable_40 = v; }
  uint32_t vtable_40_bits() const { return data()->vtable_40; }
  void set_vtable_40_bits(uint32_t v) const { data()->vtable_40 = v; }
  // 0x82000AEC: 1 the second worker pool; the first pool maker's
  uint32_t vtable_44_address() const { return address + 44; }
  uint32_t vtable_44() const { return data()->vtable_44; }
  void set_vtable_44(uint32_t v) const { data()->vtable_44 = v; }
  uint32_t vtable_44_bits() const { return data()->vtable_44; }
  void set_vtable_44_bits(uint32_t v) const { data()->vtable_44 = v; }
  // 0x82000AE4: 1 the second worker pool; the morph job maker's
  uint32_t vtable_48_address() const { return address + 48; }
  uint32_t vtable_48() const { return data()->vtable_48; }
  void set_vtable_48(uint32_t v) const { data()->vtable_48 = v; }
  uint32_t vtable_48_bits() const { return data()->vtable_48; }
  void set_vtable_48_bits(uint32_t v) const { data()->vtable_48 = v; }
  // 0x82000AD4: 1 +68, 2 +76, 3 +80; the frame passes' interface
  uint32_t vtable_52_address() const { return address + 52; }
  uint32_t vtable_52() const { return data()->vtable_52; }
  void set_vtable_52(uint32_t v) const { data()->vtable_52 = v; }
  uint32_t vtable_52_bits() const { return data()->vtable_52; }
  void set_vtable_52_bits(uint32_t v) const { data()->vtable_52 = v; }
  // 0x82000ACC: 1 the users; the save devices' interface
  uint32_t vtable_56_address() const { return address + 56; }
  uint32_t vtable_56() const { return data()->vtable_56; }
  void set_vtable_56(uint32_t v) const { data()->vtable_56 = v; }
  uint32_t vtable_56_bits() const { return data()->vtable_56; }
  void set_vtable_56_bits(uint32_t v) const { data()->vtable_56 = v; }
  // 0x82000AC8 (slot 0 only): the users' interface
  uint32_t vtable_60_address() const { return address + 60; }
  uint32_t vtable_60() const { return data()->vtable_60; }
  void set_vtable_60(uint32_t v) const { data()->vtable_60 = v; }
  uint32_t vtable_60_bits() const { return data()->vtable_60; }
  void set_vtable_60_bits(uint32_t v) const { data()->vtable_60 = v; }
  // Framework::Init's r4
  uint32_t run_loop_address() const { return address + 64; }
  template <typename T = RunLoop>
  T run_loop() const { return T(base, data()->run_loop); }
  void set_run_loop(const View& w) const { const uint32_t v = w.address; data()->run_loop = v; }
  void set_run_loop(uint32_t v) const { data()->run_loop = v; }
  uint32_t run_loop_bits() const { return data()->run_loop; }
  void set_run_loop_bits(uint32_t v) const { data()->run_loop = v; }
  // the first worker pool + 8
  uint32_t worker_pool_address() const { return address + 68; }
  template <typename T = WorkerPoolInterface>
  T worker_pool() const { return T(base, data()->worker_pool); }
  void set_worker_pool(const View& w) const { const uint32_t v = w.address; data()->worker_pool = v; }
  void set_worker_pool(uint32_t v) const { data()->worker_pool = v; }
  uint32_t worker_pool_bits() const { return data()->worker_pool; }
  void set_worker_pool_bits(uint32_t v) const { data()->worker_pool = v; }
  // the second worker pool + 8
  uint32_t worker_pool_2_address() const { return address + 72; }
  template <typename T = WorkerPoolInterface>
  T worker_pool_2() const { return T(base, data()->worker_pool_2); }
  void set_worker_pool_2(const View& w) const { const uint32_t v = w.address; data()->worker_pool_2 = v; }
  void set_worker_pool_2(uint32_t v) const { data()->worker_pool_2 = v; }
  uint32_t worker_pool_2_bits() const { return data()->worker_pool_2; }
  void set_worker_pool_2_bits(uint32_t v) const { data()->worker_pool_2 = v; }
  // the frame driver + 20 (frame-driver.md)
  uint32_t frame_driver_address() const { return address + 76; }
  uint32_t frame_driver() const { return data()->frame_driver; }
  void set_frame_driver(uint32_t v) const { data()->frame_driver = v; }
  uint32_t frame_driver_bits() const { return data()->frame_driver; }
  void set_frame_driver_bits(uint32_t v) const { data()->frame_driver = v; }
  // the scene (Scene::Create, frame-driver.md)
  uint32_t scene_address() const { return address + 80; }
  uint32_t scene() const { return data()->scene; }
  void set_scene(uint32_t v) const { data()->scene = v; }
  uint32_t scene_bits() const { return data()->scene; }
  void set_scene_bits(uint32_t v) const { data()->scene = v; }
  uint32_t frame_start_address() const { return address + 84; }
  template <typename T = FrameStart>
  T frame_start() const { return T(base, data()->frame_start); }
  void set_frame_start(const View& w) const { const uint32_t v = w.address; data()->frame_start = v; }
  void set_frame_start(uint32_t v) const { data()->frame_start = v; }
  uint32_t frame_start_bits() const { return data()->frame_start; }
  void set_frame_start_bits(uint32_t v) const { data()->frame_start = v; }
  // the input manager (input.md)
  uint32_t input_manager_address() const { return address + 88; }
  uint32_t input_manager() const { return data()->input_manager; }
  void set_input_manager(uint32_t v) const { data()->input_manager = v; }
  uint32_t input_manager_bits() const { return data()->input_manager; }
  void set_input_manager_bits(uint32_t v) const { data()->input_manager = v; }
  uint32_t debug_text_address() const { return address + 92; }
  template <typename T = DebugText>
  T debug_text() const { return T(base, data()->debug_text); }
  void set_debug_text(const View& w) const { const uint32_t v = w.address; data()->debug_text = v; }
  void set_debug_text(uint32_t v) const { data()->debug_text = v; }
  uint32_t debug_text_bits() const { return data()->debug_text; }
  void set_debug_text_bits(uint32_t v) const { data()->debug_text = v; }
  // 0, never set by the set-up
  uint32_t unknown_96_address() const { return address + 96; }
  uint32_t unknown_96() const { return data()->unknown_96; }
  void set_unknown_96(uint32_t v) const { data()->unknown_96 = v; }
  uint32_t unknown_96_bits() const { return data()->unknown_96; }
  void set_unknown_96_bits(uint32_t v) const { data()->unknown_96 = v; }
  // 0, never set by the set-up
  uint32_t unknown_100_address() const { return address + 100; }
  uint32_t unknown_100() const { return data()->unknown_100; }
  void set_unknown_100(uint32_t v) const { data()->unknown_100 = v; }
  uint32_t unknown_100_bits() const { return data()->unknown_100; }
  void set_unknown_100_bits(uint32_t v) const { data()->unknown_100 = v; }
  // 0, never set by the set-up
  uint32_t unknown_104_address() const { return address + 104; }
  uint32_t unknown_104() const { return data()->unknown_104; }
  void set_unknown_104(uint32_t v) const { data()->unknown_104 = v; }
  uint32_t unknown_104_bits() const { return data()->unknown_104; }
  void set_unknown_104_bits(uint32_t v) const { data()->unknown_104 = v; }
  // the frame passes (44 bytes, frame-passes.md)
  uint32_t frame_passes_address() const { return address + 108; }
  uint32_t frame_passes() const { return data()->frame_passes; }
  void set_frame_passes(uint32_t v) const { data()->frame_passes = v; }
  uint32_t frame_passes_bits() const { return data()->frame_passes; }
  void set_frame_passes_bits(uint32_t v) const { data()->frame_passes = v; }
  // also at 0x8296B910 (PoolMaker, vtable 0x82002848)
  uint32_t pool_maker_address() const { return address + 112; }
  uint32_t pool_maker() const { return data()->pool_maker; }
  void set_pool_maker(uint32_t v) const { data()->pool_maker = v; }
  uint32_t pool_maker_bits() const { return data()->pool_maker; }
  void set_pool_maker_bits(uint32_t v) const { data()->pool_maker = v; }
  // also at 0x8296B914
  uint32_t morph_job_maker_address() const { return address + 116; }
  template <typename T = MorphJobMaker>
  T morph_job_maker() const { return T(base, data()->morph_job_maker); }
  void set_morph_job_maker(const View& w) const { const uint32_t v = w.address; data()->morph_job_maker = v; }
  void set_morph_job_maker(uint32_t v) const { data()->morph_job_maker = v; }
  uint32_t morph_job_maker_bits() const { return data()->morph_job_maker; }
  void set_morph_job_maker_bits(uint32_t v) const { data()->morph_job_maker = v; }
  // the app + 20
  uint32_t app_address() const { return address + 120; }
  template <typename T = FrameworkAppInterface>
  T app() const { return T(base, data()->app); }
  void set_app(const View& w) const { const uint32_t v = w.address; data()->app = v; }
  void set_app(uint32_t v) const { data()->app = v; }
  uint32_t app_bits() const { return data()->app; }
  void set_app_bits(uint32_t v) const { data()->app = v; }
  // save-interface.md
  uint32_t save_devices_address() const { return address + 124; }
  uint32_t save_devices() const { return data()->save_devices; }
  void set_save_devices(uint32_t v) const { data()->save_devices = v; }
  uint32_t save_devices_bits() const { return data()->save_devices; }
  void set_save_devices_bits(uint32_t v) const { data()->save_devices = v; }
  // the users and sign-in object (system-services.md)
  uint32_t users_address() const { return address + 128; }
  uint32_t users() const { return data()->users; }
  void set_users(uint32_t v) const { data()->users = v; }
  uint32_t users_bits() const { return data()->users; }
  void set_users_bits(uint32_t v) const { data()->users = v; }
  // cleared by the constructor, not read
  uint32_t unknown_132_address() const { return address + 132; }
  uint8_t unknown_132() const { return data()->unknown_132; }
  void set_unknown_132(uint8_t v) const { data()->unknown_132 = v; }
  uint8_t unknown_132_bits() const { return data()->unknown_132; }
  void set_unknown_132_bits(uint8_t v) const { data()->unknown_132 = v; }
  // slot 4: cleared by Framework::OnTick; when set, App::Run sends event 0 and calls Framework::Start again
  uint32_t restart_requested_address() const { return address + 133; }
  uint8_t restart_requested() const { return data()->restart_requested; }
  void set_restart_requested(uint8_t v) const { data()->restart_requested = v; }
  uint8_t restart_requested_bits() const { return data()->restart_requested; }
  void set_restart_requested_bits(uint8_t v) const { data()->restart_requested = v; }
  // slot 5: set when Framework::OnTick removes itself (the app answered 0); App::Run then sends event 1
  uint32_t app_finished_address() const { return address + 134; }
  uint8_t app_finished() const { return data()->app_finished; }
  void set_app_finished(uint8_t v) const { data()->app_finished = v; }
  uint8_t app_finished_bits() const { return data()->app_finished; }
  void set_app_finished_bits(uint8_t v) const { data()->app_finished = v; }
};

// image; measure: App::Run.r4.+0, Framework@+48.+0, Framework@+44.+0
struct FrameworkVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // slot 0, sub_82090790
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1, Framework::Init (measure fault: Framework.+0 is the +44 or +48 interface's vtable read by the pool makers, slot 1 the second worker pool)
  uint32_t init() const { return guest::LoadU32(base, address + 4); }
  void set_init(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t init_address() const { return address + 4; }
  // slot 2, Framework::Start
  uint32_t start() const { return guest::LoadU32(base, address + 8); }
  void set_start(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t start_address() const { return address + 8; }
  // slot 3, sub_82090910 (releases the services)
  uint32_t release() const { return guest::LoadU32(base, address + 12); }
  void set_release(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t release_address() const { return address + 12; }
  // slot 4, sub_82090760 (the byte +133)
  uint32_t restart_requested() const { return guest::LoadU32(base, address + 16); }
  void set_restart_requested(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t restart_requested_address() const { return address + 16; }
  // slot 5, sub_82090768 (the byte +134)
  uint32_t app_finished() const { return guest::LoadU32(base, address + 20); }
  void set_app_finished(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t app_finished_address() const { return address + 20; }
};

// embedded; measure: sub_82090350.r3
struct FrameworkHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = Framework>
  T framework() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_framework(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_framework(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t framework_address() const { return address + 0; }
};

// embedded; measure: sub_82090770.r3, sub_82090778.r3, sub_82090788.r3, sub_824892F8.r3, sub_8248F030.r3
#pragma pack(push, 1)
struct FrameworkGetterSelfData {
  uint8_t pad_0[44] = {};
  uint32_t word_44 = 0;  // +44
  uint8_t pad_48[12] = {};
  uint32_t word_60 = 0;  // +60
  uint32_t word_64 = 0;  // +64
  uint32_t word_68 = 0;  // +68
  uint8_t pad_72[16] = {};
  uint32_t word_88 = 0;  // +88
  uint32_t word_92 = 0;  // +92
};
#pragma pack(pop)
static_assert(sizeof(FrameworkGetterSelfData) == 96);
inline constexpr memory::LayoutField kFrameworkGetterSelfFields[] = {{44, 4}, {60, 4}, {64, 4}, {68, 4}, {88, 4}, {92, 4}};
inline constexpr memory::Layout kFrameworkGetterSelfLayout = {kFrameworkGetterSelfFields, 6};

struct FrameworkGetterSelf : View {
  using View::View;
  static constexpr uint32_t kSize = 96;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  FrameworkGetterSelfData* data() const { return static_cast<FrameworkGetterSelfData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static FrameworkGetterSelf New(uint8_t* base) {
    return FrameworkGetterSelf(base, memory::MakeHandle(new FrameworkGetterSelfData(), sizeof(FrameworkGetterSelfData), "FrameworkGetterSelf", &kFrameworkGetterSelfLayout));
  }
  template <uint32_t Stride = kSize>
  static FrameworkGetterSelf NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FrameworkGetterSelfData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrameworkGetterSelfData*>(::operator new(sizeof(FrameworkGetterSelfData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrameworkGetterSelfData();
    return FrameworkGetterSelf(base, memory::MakeHandle(p, sizeof(FrameworkGetterSelfData) * count, "FrameworkGetterSelf", &kFrameworkGetterSelfLayout, sizeof(FrameworkGetterSelfData)));
  }
  FrameworkGetterSelf at(uint32_t i) const { return FrameworkGetterSelf(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFrameworkGetterSelfLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFrameworkGetterSelfLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // sub_824892F8: from framework + 24, the first worker pool
  uint32_t word_44_address() const { return address + 44; }
  uint32_t word_44() const { return on_host() ? uint32_t(data()->word_44) : uint32_t(guest::LoadU32(base, address + 44)); }
  void set_word_44(uint32_t v) const { if (on_host()) data()->word_44 = v; else guest::StoreU32(base, address + 44, v); }
  uint32_t word_44_bits() const { return on_host() ? data()->word_44 : guest::LoadU32(base, address + 44); }
  void set_word_44_bits(uint32_t v) const { if (on_host()) data()->word_44 = v; else guest::StoreU32(base, address + 44, v); }
  // sub_82090770: from framework + 20, the scene
  uint32_t word_60_address() const { return address + 60; }
  uint32_t word_60() const { return on_host() ? uint32_t(data()->word_60) : uint32_t(guest::LoadU32(base, address + 60)); }
  void set_word_60(uint32_t v) const { if (on_host()) data()->word_60 = v; else guest::StoreU32(base, address + 60, v); }
  uint32_t word_60_bits() const { return on_host() ? data()->word_60 : guest::LoadU32(base, address + 60); }
  void set_word_60_bits(uint32_t v) const { if (on_host()) data()->word_60 = v; else guest::StoreU32(base, address + 60, v); }
  // sub_82090778: on the framework, the run loop; from + 24, the input manager
  uint32_t word_64_address() const { return address + 64; }
  uint32_t word_64() const { return on_host() ? uint32_t(data()->word_64) : uint32_t(guest::LoadU32(base, address + 64)); }
  void set_word_64(uint32_t v) const { if (on_host()) data()->word_64 = v; else guest::StoreU32(base, address + 64, v); }
  uint32_t word_64_bits() const { return on_host() ? data()->word_64 : guest::LoadU32(base, address + 64); }
  void set_word_64_bits(uint32_t v) const { if (on_host()) data()->word_64 = v; else guest::StoreU32(base, address + 64, v); }
  // sub_8248F030: from framework + 8 or + 24, the frame driver or the debug text
  uint32_t word_68_address() const { return address + 68; }
  uint32_t word_68() const { return on_host() ? uint32_t(data()->word_68) : uint32_t(guest::LoadU32(base, address + 68)); }
  void set_word_68(uint32_t v) const { if (on_host()) data()->word_68 = v; else guest::StoreU32(base, address + 68, v); }
  uint32_t word_68_bits() const { return on_host() ? data()->word_68 : guest::LoadU32(base, address + 68); }
  void set_word_68_bits(uint32_t v) const { if (on_host()) data()->word_68 = v; else guest::StoreU32(base, address + 68, v); }
  // sub_82090788: on the framework, the input manager
  uint32_t word_88_address() const { return address + 88; }
  uint32_t word_88() const { return on_host() ? uint32_t(data()->word_88) : uint32_t(guest::LoadU32(base, address + 88)); }
  void set_word_88(uint32_t v) const { if (on_host()) data()->word_88 = v; else guest::StoreU32(base, address + 88, v); }
  uint32_t word_88_bits() const { return on_host() ? data()->word_88 : guest::LoadU32(base, address + 88); }
  void set_word_88_bits(uint32_t v) const { if (on_host()) data()->word_88 = v; else guest::StoreU32(base, address + 88, v); }
  // sub_82493700: from framework + 36, the users (framework + 128)
  uint32_t word_92_address() const { return address + 92; }
  uint32_t word_92() const { return on_host() ? uint32_t(data()->word_92) : uint32_t(guest::LoadU32(base, address + 92)); }
  void set_word_92(uint32_t v) const { if (on_host()) data()->word_92 = v; else guest::StoreU32(base, address + 92, v); }
  uint32_t word_92_bits() const { return on_host() ? data()->word_92 : guest::LoadU32(base, address + 92); }
  void set_word_92_bits(uint32_t v) const { if (on_host()) data()->word_92 = v; else guest::StoreU32(base, address + 92, v); }
};

// embedded; measure: FrameStart.+4, sub_820944C8.r3@-12.+20
#pragma pack(push, 1)
struct FrameworkInterfaceData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(FrameworkInterfaceData) == 4);
inline constexpr memory::LayoutField kFrameworkInterfaceFields[] = {{0, 4}};
inline constexpr memory::Layout kFrameworkInterfaceLayout = {kFrameworkInterfaceFields, 1};

struct FrameworkInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  FrameworkInterfaceData* data() const { return static_cast<FrameworkInterfaceData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static FrameworkInterface New(uint8_t* base) {
    return FrameworkInterface(base, memory::MakeHandle(new FrameworkInterfaceData(), sizeof(FrameworkInterfaceData), "FrameworkInterface", &kFrameworkInterfaceLayout));
  }
  template <uint32_t Stride = kSize>
  static FrameworkInterface NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FrameworkInterfaceData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrameworkInterfaceData*>(::operator new(sizeof(FrameworkInterfaceData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrameworkInterfaceData();
    return FrameworkInterface(base, memory::MakeHandle(p, sizeof(FrameworkInterfaceData) * count, "FrameworkInterface", &kFrameworkInterfaceLayout, sizeof(FrameworkInterfaceData)));
  }
  FrameworkInterface at(uint32_t i) const { return FrameworkInterface(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFrameworkInterfaceLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFrameworkInterfaceLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = FrameworkInterfaceVtable>
  T vtable() const { return T(base, on_host() ? data()->vtable : guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
};

// image; measure: FrameStart.+4.+0, sub_820944C8.r3@-12.+20.+0
struct FrameworkInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // slot 0, a thunk to the destructor
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // +40's the debug text (sub_82090860), +36's the input manager (sub_82090830)
  uint32_t slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
  uint32_t slot_2() const { return guest::LoadU32(base, address + 8); }
  void set_slot_2(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t slot_2_address() const { return address + 8; }
  uint32_t slot_3() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_address() const { return address + 12; }
  uint32_t slot_4() const { return guest::LoadU32(base, address + 16); }
  void set_slot_4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t slot_4_address() const { return address + 16; }
};

// embedded; measure: Framework.+120
struct FrameworkAppInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0x82000E08
  template <typename T = FrameworkAppInterfaceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: Framework.+120.+0
struct FrameworkAppInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // sub_82092878: the game's first state set up (Framework::Start)
  uint32_t start() const { return guest::LoadU32(base, address + 36); }
  void set_start(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t start_address() const { return address + 36; }
  // sub_82092928 (Framework::OnTick: 0 removes tick handler 7)
  uint32_t is_running() const { return guest::LoadU32(base, address + 44); }
  void set_is_running(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t is_running_address() const { return address + 44; }
};

// embedded; measure: sub_820944C8.r3@-12
struct AppPart4 : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  // app + 24: the framework's +36 interface
  template <typename T = FrameworkInterface>
  T framework_interface() const { return T(base, guest::LoadU32(base, address + 20)); }
  void set_framework_interface(const View& v) const { guest::StoreU32(base, address + 20, v.address); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t framework_interface_address() const { return address + 20; }
};

// embedded; measure: RunLoop, Framework::Init.r4, sub_8209A6E0.r3, sub_8209A008.r3
struct RunLoop : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr uint32_t kVtable = 0x82000FE8;
  // 0x82000FE8 (0x82000FC8 its base's)
  template <typename T = RunLoopVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a HandlerMap by key: App::Run runs while it isn't empty
  uint32_t tick_handlers_address() const { return address + 4; }
  // a HandlerMap of the event listeners
  uint32_t listeners_address() const { return address + 16; }
  // a RequestVector: the pending requests
  uint32_t requests_address() const { return address + 28; }
  // the event slot 7 sends
  uint32_t last_event() const { return guest::LoadU32(base, address + 44); }
  void set_last_event(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t last_event_address() const { return address + 44; }
  // cleared by the constructor
  uint8_t unknown_48() const { return guest::LoadU8(base, address + 48); }
  void set_unknown_48(uint8_t v) const { guest::StoreU8(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  // cleared by the constructor
  uint8_t unknown_49() const { return guest::LoadU8(base, address + 49); }
  void set_unknown_49(uint8_t v) const { guest::StoreU8(base, address + 49, v); }
  uint32_t unknown_49_address() const { return address + 49; }
  // cleared by the constructor
  uint8_t unknown_50() const { return guest::LoadU8(base, address + 50); }
  void set_unknown_50(uint8_t v) const { guest::StoreU8(base, address + 50, v); }
  uint32_t unknown_50_address() const { return address + 50; }
};

// image; measure: Framework::Init.r4.+0, FrameStart::FrameStart.r4.+0, sub_82094F30.r4.+0
struct RunLoopVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // measure fault: the run loop's own vtable word (the run loop and its vtable joined)
  uint32_t joined_vtable() const { return guest::LoadU32(base, address + 0); }
  void set_joined_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t joined_vtable_address() const { return address + 0; }
  // slot 1, RunLoop::AddListener
  uint32_t add_listener() const { return guest::LoadU32(base, address + 4); }
  void set_add_listener(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t add_listener_address() const { return address + 4; }
  // slot 2, RunLoop::AddTickHandler
  uint32_t add_tick_handler() const { return guest::LoadU32(base, address + 8); }
  void set_add_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t add_tick_handler_address() const { return address + 8; }
  // slot 3, sub_8209ABA8 (never ran)
  uint32_t remove_listener() const { return guest::LoadU32(base, address + 12); }
  void set_remove_listener(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t remove_listener_address() const { return address + 12; }
  // slot 4, sub_8209AB08
  uint32_t remove_tick_handler() const { return guest::LoadU32(base, address + 16); }
  void set_remove_tick_handler(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t remove_tick_handler_address() const { return address + 16; }
  // a folded getter
  uint32_t slot_5() const { return guest::LoadU32(base, address + 20); }
  void set_slot_5(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot_5_address() const { return address + 20; }
  // a folded getter
  uint32_t slot_6() const { return guest::LoadU32(base, address + 24); }
  void set_slot_6(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot_6_address() const { return address + 24; }
  // slot 7, sub_8209A008 (never ran)
  uint32_t send_event() const { return guest::LoadU32(base, address + 28); }
  void set_send_event(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t send_event_address() const { return address + 28; }
};

// image; measure: App.+0
struct AppOrRunLoopVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // the app's +16 interface's slot +16 (the first worker pool)
  uint32_t app_slot_4() const { return guest::LoadU32(base, address + 16); }
  void set_app_slot_4(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t app_slot_4_address() const { return address + 16; }
  // measure fault: the run loop's slot 7 (App::Run)
  uint32_t send_event() const { return guest::LoadU32(base, address + 28); }
  void set_send_event(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t send_event_address() const { return address + 28; }
  // the app's +16 interface's slot +48 (the sound manager)
  uint32_t app_slot_12() const { return guest::LoadU32(base, address + 48); }
  void set_app_slot_12(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t app_slot_12_address() const { return address + 48; }
};

// embedded; measure: sub_8209A008.r4
struct RunLoopEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0 or 1 from App::Run
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: RunLoop::AddTickHandler.r4, RunLoop::AddListener.r4, sub_8209AB08.r4, sub_8209ABA8.r4, sub_82094890.r4, sub_82094930.r4, sub_820949D0.r4, sub_82094A70.r4
struct RunLoopHandlerKey : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t key() const { return guest::LoadU32(base, address + 0); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_address() const { return address + 0; }
};

// embedded; measure: RunLoop::AddTickHandler.r5, RunLoop::AddListener.r5
struct RunLoopDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = RunLoopDelegateVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // Delegate's target object
  uint32_t object() const { return guest::LoadU32(base, address + 4); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_address() const { return address + 4; }
  // Delegate's function, called with the object
  uint32_t function() const { return guest::LoadU32(base, address + 8); }
  void set_function(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t function_address() const { return address + 8; }
};

// image; measure: RunLoop::AddTickHandler.r5.+0, RunLoop::AddListener.r5.+0
struct RunLoopDelegateVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // slot 1, Delegate::Invoke
  uint32_t invoke() const { return guest::LoadU32(base, address + 4); }
  void set_invoke(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t invoke_address() const { return address + 4; }
  // slot 2, Delegate::Clone
  uint32_t clone() const { return guest::LoadU32(base, address + 8); }
  void set_clone(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t clone_address() const { return address + 8; }
  // sub_820AC9E8 for Delegate
  uint32_t slot_3() const { return guest::LoadU32(base, address + 12); }
  void set_slot_3(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t slot_3_address() const { return address + 12; }
};

// embedded; measure: sub_82096330.r3, sub_82096340.r3
#pragma pack(push, 1)
struct DelegateTargetData {
  uint32_t vtable = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(DelegateTargetData) == 4);
inline constexpr memory::LayoutField kDelegateTargetFields[] = {{0, 4}};
inline constexpr memory::Layout kDelegateTargetLayout = {kDelegateTargetFields, 1};

struct DelegateTarget : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  DelegateTargetData* data() const { return static_cast<DelegateTargetData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static DelegateTarget New(uint8_t* base) {
    return DelegateTarget(base, memory::MakeHandle(new DelegateTargetData(), sizeof(DelegateTargetData), "DelegateTarget", &kDelegateTargetLayout));
  }
  template <uint32_t Stride = kSize>
  static DelegateTarget NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(DelegateTargetData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<DelegateTargetData*>(::operator new(sizeof(DelegateTargetData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) DelegateTargetData();
    return DelegateTarget(base, memory::MakeHandle(p, sizeof(DelegateTargetData) * count, "DelegateTarget", &kDelegateTargetLayout, sizeof(DelegateTargetData)));
  }
  DelegateTarget at(uint32_t i) const { return DelegateTarget(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kDelegateTargetLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kDelegateTargetLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = DelegateTargetVtable>
  T vtable() const { return T(base, on_host() ? data()->vtable : guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  void set_vtable(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_bits() const { return on_host() ? data()->vtable : guest::LoadU32(base, address + 0); }
  void set_vtable_bits(uint32_t v) const { if (on_host()) data()->vtable = v; else guest::StoreU32(base, address + 0, v); }
};

// image; measure: sub_82096330.r3.+0, sub_82096340.r3.+0
struct DelegateTargetVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // FrameStart::OnTick for the frame start
  uint32_t slot_0() const { return guest::LoadU32(base, address + 0); }
  void set_slot_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t slot_0_address() const { return address + 0; }
  // sub_82092650 for the frame start
  uint32_t slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
};

// embedded; measure: HandlerMap, HandlerMap::Insert.r4, sub_82093638.r4, sub_82093850.r3, HandlerMap::InsertNode.r4, HandlerMap::HandlerMap.r3
#pragma pack(push, 1)
struct HandlerMapData {
  uint32_t fault_result_map = 0;  // +0
  uint32_t head = 0;  // +4
  uint32_t size = 0;  // +8
  uint8_t pad_12[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(HandlerMapData) == 16);
inline constexpr memory::LayoutField kHandlerMapFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kHandlerMapLayout = {kHandlerMapFields, 3};

struct HandlerMap : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  HandlerMapData* data() const { return static_cast<HandlerMapData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static HandlerMap New(uint8_t* base) {
    return HandlerMap(base, memory::MakeHandle(new HandlerMapData(), sizeof(HandlerMapData), "HandlerMap", &kHandlerMapLayout));
  }
  template <uint32_t Stride = kSize>
  static HandlerMap NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(HandlerMapData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<HandlerMapData*>(::operator new(sizeof(HandlerMapData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) HandlerMapData();
    return HandlerMap(base, memory::MakeHandle(p, sizeof(HandlerMapData) * count, "HandlerMap", &kHandlerMapLayout, sizeof(HandlerMapData)));
  }
  HandlerMap at(uint32_t i) const { return HandlerMap(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kHandlerMapLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // measure fault: an insert result's iterator as one doubleword (the map, the node)
  uint32_t fault_result_where_address() const { return address + 0; }
  uint64_t fault_result_where_bits() const { return on_host() ? uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapLayout, 0, 8)) : guest::LoadU64(base, address + 0); }
  void set_fault_result_where_bits(uint64_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapLayout, 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t fault_result_where() const { return uint64_t(fault_result_where_bits()); }
  void set_fault_result_where(uint64_t v) const { set_fault_result_where_bits(uint64_t(v)); }
  // measure fault: an insert result's map
  uint32_t fault_result_map_address() const { return address + 0; }
  uint32_t fault_result_map() const { return on_host() ? uint32_t(data()->fault_result_map) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_fault_result_map(uint32_t v) const { if (on_host()) data()->fault_result_map = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t fault_result_map_bits() const { return on_host() ? data()->fault_result_map : guest::LoadU32(base, address + 0); }
  void set_fault_result_map_bits(uint32_t v) const { if (on_host()) data()->fault_result_map = v; else guest::StoreU32(base, address + 0, v); }
  // the head node (left the leftmost, parent the root, right the rightmost); also an insert result's node
  uint32_t head_address() const { return address + 4; }
  template <typename T = HandlerMapNode>
  T head() const { return T(base, on_host() ? data()->head : guest::LoadU32(base, address + 4)); }
  void set_head(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->head = v; else guest::StoreU32(base, address + 4, v); }
  void set_head(uint32_t v) const { if (on_host()) data()->head = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t head_bits() const { return on_host() ? data()->head : guest::LoadU32(base, address + 4); }
  void set_head_bits(uint32_t v) const { if (on_host()) data()->head = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t size_address() const { return address + 8; }
  uint32_t size() const { return on_host() ? uint32_t(data()->size) : uint32_t(guest::LoadU32(base, address + 8)); }
  void set_size(uint32_t v) const { if (on_host()) data()->size = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t size_bits() const { return on_host() ? data()->size : guest::LoadU32(base, address + 8); }
  void set_size_bits(uint32_t v) const { if (on_host()) data()->size = v; else guest::StoreU32(base, address + 8, v); }
  // measure fault: HandlerMap::Insert's inserted byte
  uint32_t fault_result_inserted_address() const { return address + 8; }
  uint8_t fault_result_inserted_bits() const { return on_host() ? uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapLayout, 8, 1)) : guest::LoadU8(base, address + 8); }
  void set_fault_result_inserted_bits(uint8_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapLayout, 8, 1, v); else guest::StoreU8(base, address + 8, v); }
  uint8_t fault_result_inserted() const { return uint8_t(fault_result_inserted_bits()); }
  void set_fault_result_inserted(uint8_t v) const { set_fault_result_inserted_bits(uint8_t(v)); }
  // measure fault: HandlerMap::ForEach's returned functor's second doubleword
  uint32_t fault_result_functor_8_address() const { return address + 8; }
  uint64_t fault_result_functor_8_bits() const { return on_host() ? uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapLayout, 8, 8)) : guest::LoadU64(base, address + 8); }
  void set_fault_result_functor_8_bits(uint64_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapLayout, 8, 8, v); else guest::StoreU64(base, address + 8, v); }
  uint64_t fault_result_functor_8() const { return uint64_t(fault_result_functor_8_bits()); }
  void set_fault_result_functor_8(uint64_t v) const { set_fault_result_functor_8_bits(uint64_t(v)); }
};

// allocated; measure: new<?>@HandlerMap::AllocateNodes, HandlerMap::Insert.r4.+4, HandlerMap::Insert.r4.+4.+0, HandlerMap::Insert.r4.+4.+0.+8, HandlerMap::NewNode.ret, HandlerMap::NewNode.ret.+4, HandlerMap::NewNode.ret.+4.+8, HandlerMap::RotateLeft.r4, HandlerMap::RotateLeft.r4.+4, HandlerMap::RotateLeft.r4.+8, HandlerMap::RotateLeft.r4.+8.+0, sub_82091AE8.r3.+4, sub_82091AE8.r3.+4.+0, sub_82091AE8.r3.+4.+8, sub_82099B18.r3.+4, sub_82099B18.r3.+4.+0, sub_82099B18.r3.+4.+8, sub_82099BA8.r4, sub_82099BA8.r4.+0, sub_82093638.r4.+4, sub_82093850.r3.+4, sub_8209A008.r3.+20, App.+8, HandlerMap::InsertNode.r4.+4, HandlerMap::InsertNode.r6.+8, HandlerMap::InsertNode.r6.+8.+4, HandlerMap::InsertNode.r6.+8.+0, HandlerMap::InsertNode.r6.+8.+0.+8, HandlerMap.+4
#pragma pack(push, 1)
struct HandlerMapNodeData {
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
static_assert(sizeof(HandlerMapNodeData) == 24);
inline constexpr memory::LayoutField kHandlerMapNodeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}};
inline constexpr memory::Layout kHandlerMapNodeLayout = {kHandlerMapNodeFields, 5};

struct HandlerMapNode : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  HandlerMapNodeData* data() const { return static_cast<HandlerMapNodeData*>(memory::HandleObject(address)); }
  static HandlerMapNode New(uint8_t* base) {
    return HandlerMapNode(base, memory::MakeHandle(new HandlerMapNodeData(), sizeof(HandlerMapNodeData), "HandlerMapNode", &kHandlerMapNodeLayout));
  }
  template <uint32_t Stride = kSize>
  static HandlerMapNode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(HandlerMapNodeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<HandlerMapNodeData*>(::operator new(sizeof(HandlerMapNodeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) HandlerMapNodeData();
    return HandlerMapNode(base, memory::MakeHandle(p, sizeof(HandlerMapNodeData) * count, "HandlerMapNode", &kHandlerMapNodeLayout, sizeof(HandlerMapNodeData)));
  }
  HandlerMapNode at(uint32_t i) const { return HandlerMapNode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kHandlerMapNodeLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kHandlerMapNodeLayout, offset, width, v); }
  uint32_t left_address() const { return address + 0; }
  template <typename T = HandlerMapNode>
  T left() const { return T(base, data()->left); }
  void set_left(const View& w) const { const uint32_t v = w.address; data()->left = v; }
  void set_left(uint32_t v) const { data()->left = v; }
  uint32_t left_bits() const { return data()->left; }
  void set_left_bits(uint32_t v) const { data()->left = v; }
  uint32_t parent_address() const { return address + 4; }
  template <typename T = HandlerMapNode>
  T parent() const { return T(base, data()->parent); }
  void set_parent(const View& w) const { const uint32_t v = w.address; data()->parent = v; }
  void set_parent(uint32_t v) const { data()->parent = v; }
  uint32_t parent_bits() const { return data()->parent; }
  void set_parent_bits(uint32_t v) const { data()->parent = v; }
  uint32_t right_address() const { return address + 8; }
  template <typename T = HandlerMapNode>
  T right() const { return T(base, data()->right); }
  void set_right(const View& w) const { const uint32_t v = w.address; data()->right = v; }
  void set_right(uint32_t v) const { data()->right = v; }
  uint32_t right_bits() const { return data()->right; }
  void set_right_bits(uint32_t v) const { data()->right = v; }
  uint32_t key_address() const { return address + 12; }
  uint32_t key() const { return data()->key; }
  void set_key(uint32_t v) const { data()->key = v; }
  uint32_t key_bits() const { return data()->key; }
  void set_key_bits(uint32_t v) const { data()->key = v; }
  uint32_t delegate_address() const { return address + 16; }
  template <typename T = RunLoopDelegate>
  T delegate() const { return T(base, data()->delegate); }
  void set_delegate(const View& w) const { const uint32_t v = w.address; data()->delegate = v; }
  void set_delegate(uint32_t v) const { data()->delegate = v; }
  uint32_t delegate_bits() const { return data()->delegate; }
  void set_delegate_bits(uint32_t v) const { data()->delegate = v; }
  // 0 red, 1 black
  uint32_t color_address() const { return address + 20; }
  uint8_t color() const { return data()->color; }
  void set_color(uint8_t v) const { data()->color = v; }
  uint8_t color_bits() const { return data()->color; }
  void set_color_bits(uint8_t v) const { data()->color = v; }
  // 1 only for the head
  uint32_t is_nil_address() const { return address + 21; }
  uint8_t is_nil() const { return data()->is_nil; }
  void set_is_nil(uint8_t v) const { data()->is_nil = v; }
  uint8_t is_nil_bits() const { return data()->is_nil; }
  void set_is_nil_bits(uint8_t v) const { data()->is_nil = v; }
};

// embedded; measure: sub_82091AE8.r3, sub_82099B18.r3, HandlerMap::InsertNode.ret, sub_82093638.r3
struct HandlerMapIterator : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the map and the node as one doubleword
  uint64_t both() const { return guest::LoadU64(base, address + 0); }
  void set_both(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t both_address() const { return address + 0; }
  template <typename T = HandlerMap>
  T map() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_map(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_map(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t map_address() const { return address + 0; }
  template <typename T = HandlerMapNode>
  T node() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_node(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_node(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t node_address() const { return address + 4; }
  // HandlerMap::Insert's result only: 1 when the value was added
  uint8_t inserted() const { return guest::LoadU8(base, address + 8); }
  void set_inserted(uint8_t v) const { guest::StoreU8(base, address + 8, v); }
  uint32_t inserted_address() const { return address + 8; }
};

// embedded; measure: HandlerMap::NewNode.r7, HandlerMap::Insert.r5
struct HandlerMapValue : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t key() const { return guest::LoadU32(base, address + 0); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key_address() const { return address + 0; }
  template <typename T = RunLoopDelegate>
  T delegate() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delegate(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delegate(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_address() const { return address + 4; }
};

// embedded; measure: HandlerMap::Insert.ret
struct HandlerMapOwner : View {
  using View::View;
  static constexpr uint32_t kSize = 72;
  // the run loop's tick handler map's head
  uint32_t tick_handler_head() const { return guest::LoadU32(base, address + 8); }
  void set_tick_handler_head(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t tick_handler_head_address() const { return address + 8; }
  // the app's +28 map's head
  uint32_t app_handlers_head() const { return guest::LoadU32(base, address + 32); }
  void set_app_handlers_head(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t app_handlers_head_address() const { return address + 32; }
  // the app's +64 map's head
  uint32_t app_listeners_head() const { return guest::LoadU32(base, address + 68); }
  void set_app_listeners_head(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t app_listeners_head_address() const { return address + 68; }
};

// embedded; measure: RequestVector, RequestVector::PushBack.r3, RequestVector::Erase.r4, RequestVector::Clear.r3
#pragma pack(push, 1)
struct RequestVectorData {
  uint8_t pad_0[4] = {};
  uint32_t first = 0;  // +4
  uint32_t last = 0;  // +8
  uint32_t end = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(RequestVectorData) == 16);
inline constexpr memory::LayoutField kRequestVectorFields[] = {{4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kRequestVectorLayout = {kRequestVectorFields, 3};

struct RequestVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  RequestVectorData* data() const { return static_cast<RequestVectorData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static RequestVector New(uint8_t* base) {
    return RequestVector(base, memory::MakeHandle(new RequestVectorData(), sizeof(RequestVectorData), "RequestVector", &kRequestVectorLayout));
  }
  template <uint32_t Stride = kSize>
  static RequestVector NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RequestVectorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RequestVectorData*>(::operator new(sizeof(RequestVectorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RequestVectorData();
    return RequestVector(base, memory::MakeHandle(p, sizeof(RequestVectorData) * count, "RequestVector", &kRequestVectorLayout, sizeof(RequestVectorData)));
  }
  RequestVector at(uint32_t i) const { return RequestVector(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kRequestVectorLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRequestVectorLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // measure fault: a result iterator (the vector, the element)
  uint32_t fault_result_where_address() const { return address + 0; }
  uint64_t fault_result_where_bits() const { return on_host() ? uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kRequestVectorLayout, 0, 8)) : guest::LoadU64(base, address + 0); }
  void set_fault_result_where_bits(uint64_t v) const { if (on_host()) memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kRequestVectorLayout, 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t fault_result_where() const { return uint64_t(fault_result_where_bits()); }
  void set_fault_result_where(uint64_t v) const { set_fault_result_where_bits(uint64_t(v)); }
  uint32_t first_address() const { return address + 4; }
  template <typename T = Request>
  T first() const { return T(base, on_host() ? data()->first : guest::LoadU32(base, address + 4)); }
  void set_first(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->first = v; else guest::StoreU32(base, address + 4, v); }
  void set_first(uint32_t v) const { if (on_host()) data()->first = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t first_bits() const { return on_host() ? data()->first : guest::LoadU32(base, address + 4); }
  void set_first_bits(uint32_t v) const { if (on_host()) data()->first = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t last_address() const { return address + 8; }
  template <typename T = Request>
  T last() const { return T(base, on_host() ? data()->last : guest::LoadU32(base, address + 8)); }
  void set_last(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->last = v; else guest::StoreU32(base, address + 8, v); }
  void set_last(uint32_t v) const { if (on_host()) data()->last = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t last_bits() const { return on_host() ? data()->last : guest::LoadU32(base, address + 8); }
  void set_last_bits(uint32_t v) const { if (on_host()) data()->last = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t end_address() const { return address + 12; }
  template <typename T = Request>
  T end() const { return T(base, on_host() ? data()->end : guest::LoadU32(base, address + 12)); }
  void set_end(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->end = v; else guest::StoreU32(base, address + 12, v); }
  void set_end(uint32_t v) const { if (on_host()) data()->end = v; else guest::StoreU32(base, address + 12, v); }
  uint32_t end_bits() const { return on_host() ? data()->end : guest::LoadU32(base, address + 12); }
  void set_end_bits(uint32_t v) const { if (on_host()) data()->end = v; else guest::StoreU32(base, address + 12, v); }
};

// embedded; measure: RequestVector.+8, RequestVector::InsertN.r6, RequestVector::PushBack.r4, RequestVector::PushBack.r3.+8
struct Request : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 0 add a tick handler, 1 remove one, 2 add a listener, 3 remove one
  uint32_t type() const { return guest::LoadU32(base, address + 0); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  uint32_t key() const { return guest::LoadU32(base, address + 4); }
  void set_key(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t key_address() const { return address + 4; }
  template <typename T = RunLoopDelegate>
  T delegate() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_delegate(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_delegate(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_address() const { return address + 8; }
};

// embedded; measure: ForEachTickHandler.r3, ForEachRequest.r3
struct FrameworkMemberFunctor : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the function and the object as one doubleword
  uint64_t both() const { return guest::LoadU64(base, address + 0); }
  void set_both(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t both_address() const { return address + 0; }
  uint32_t function() const { return guest::LoadU32(base, address + 0); }
  void set_function(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t function_address() const { return address + 0; }
  uint32_t object() const { return guest::LoadU32(base, address + 4); }
  void set_object(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t object_address() const { return address + 4; }
};

// allocated; measure: FrameStart
#pragma pack(push, 1)
struct FrameStartData {
  uint32_t vtable = 0;  // +0
  uint32_t framework_interface = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(FrameStartData) == 8);
inline constexpr memory::LayoutField kFrameStartFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kFrameStartLayout = {kFrameStartFields, 2};

struct FrameStart : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82000D84;
  FrameStartData* data() const { return static_cast<FrameStartData*>(memory::HandleObject(address)); }
  static FrameStart New(uint8_t* base) {
    return FrameStart(base, memory::MakeHandle(new FrameStartData(), sizeof(FrameStartData), "FrameStart", &kFrameStartLayout));
  }
  template <uint32_t Stride = kSize>
  static FrameStart NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FrameStartData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrameStartData*>(::operator new(sizeof(FrameStartData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrameStartData();
    return FrameStart(base, memory::MakeHandle(p, sizeof(FrameStartData) * count, "FrameStart", &kFrameStartLayout, sizeof(FrameStartData)));
  }
  FrameStart at(uint32_t i) const { return FrameStart(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFrameStartLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFrameStartLayout, offset, width, v); }
  // 0x82000D84: 0 FrameStart::OnTick, 1 sub_82092650, 2 sub_820926C8
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = DelegateTargetVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the framework's +40 interface (slot 1 the debug text)
  uint32_t framework_interface_address() const { return address + 4; }
  template <typename T = FrameworkInterface>
  T framework_interface() const { return T(base, data()->framework_interface); }
  void set_framework_interface(const View& w) const { const uint32_t v = w.address; data()->framework_interface = v; }
  void set_framework_interface(uint32_t v) const { data()->framework_interface = v; }
  uint32_t framework_interface_bits() const { return data()->framework_interface; }
  void set_framework_interface_bits(uint32_t v) const { data()->framework_interface = v; }
};

// embedded; measure: FrameStart::FrameStart.r4
struct FrameStartBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = RunLoop>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // the +40 interface
  template <typename T = FrameworkInterface>
  T framework_interface() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_framework_interface(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
};

// allocated; measure: DebugText, sub_82091070.r3
struct DebugText : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr uint32_t kVtable = 0x82000BC8;
  // 0x82000BC8: 0 sub_82091110, 1 sub_82090F88, 2 DebugText::SetPosition, 3 sub_82091060
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the block's +0: the framework's +20 interface
  template <typename T = FrameworkInterface>
  T framework_interface() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_framework_interface(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
  // 0x820066DC (0x82000EB8 after its destructor)
  uint32_t job_vtable() const { return guest::LoadU32(base, address + 8); }
  void set_job_vtable(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t job_vtable_address() const { return address + 8; }
  // 0x82002908 (0x82000AB4 its base's)
  uint32_t job_vtable_4() const { return guest::LoadU32(base, address + 12); }
  void set_job_vtable_4(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t job_vtable_4_address() const { return address + 12; }
  // 0 at construction
  uint32_t job_unknown_8() const { return guest::LoadU32(base, address + 16); }
  void set_job_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t job_unknown_8_address() const { return address + 16; }
  // 0 at construction; sub_82090F88 prints when 3
  uint32_t job_state() const { return guest::LoadU32(base, address + 20); }
  void set_job_state(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t job_state_address() const { return address + 20; }
  // 0 at construction
  uint8_t job_unknown_16() const { return guest::LoadU8(base, address + 24); }
  void set_job_unknown_16(uint8_t v) const { guest::StoreU8(base, address + 24, v); }
  uint32_t job_unknown_16_address() const { return address + 24; }
  // 1 at construction
  uint8_t job_unknown_17() const { return guest::LoadU8(base, address + 25); }
  void set_job_unknown_17(uint8_t v) const { guest::StoreU8(base, address + 25, v); }
  uint32_t job_unknown_17_address() const { return address + 25; }
  // 0 at construction
  uint8_t job_unknown_18() const { return guest::LoadU8(base, address + 26); }
  void set_job_unknown_18(uint8_t v) const { guest::StoreU8(base, address + 26, v); }
  uint32_t job_unknown_18_address() const { return address + 26; }
  // a word the constructor copies from its own frame, never set there
  uint32_t job_unknown_20() const { return guest::LoadU32(base, address + 28); }
  void set_job_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t job_unknown_20_address() const { return address + 28; }
  // 0x82002850 (Delegate)
  uint32_t job_delegate_vtable() const { return guest::LoadU32(base, address + 32); }
  void set_job_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t job_delegate_vtable_address() const { return address + 32; }
  template <typename T = DebugText>
  T job_delegate_object() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_job_delegate_object(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_job_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t job_delegate_object_address() const { return address + 36; }
  // sub_82090CC0, DebugText::LoadFont
  uint32_t job_delegate_function() const { return guest::LoadU32(base, address + 40); }
  void set_job_delegate_function(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t job_delegate_function_address() const { return address + 40; }
  // DEBFONT.XPR's size
  uint32_t font_size() const { return guest::LoadU32(base, address + 44); }
  void set_font_size(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t font_size_address() const { return address + 44; }
  // its contents (operator new; freed with operator delete[])
  uint32_t font_data() const { return guest::LoadU32(base, address + 48); }
  void set_font_data(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t font_data_address() const { return address + 48; }
};

// embedded; measure: DebugText::DebugText.r4
struct DebugTextBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the +20 interface
  template <typename T = FrameworkInterface>
  T framework_interface() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_framework_interface(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t framework_interface_address() const { return address + 0; }
  // the first worker pool's interface
  template <typename T = WorkerPoolInterface>
  T worker_pool() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_worker_pool(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_worker_pool(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t worker_pool_address() const { return address + 4; }
};

// embedded; measure: DebugText::SetPosition.r4, DebugText::SetPosition.r5
struct DebugTextCoordinate : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// allocated; measure: new<296>@WorkerPool::Create
struct WorkerPool : View {
  using View::View;
  static constexpr uint32_t kSize = 296;
  static constexpr uint32_t kVtable = 0x82000F0C;
  // 0x82000F0C (0x82000EDC its base's)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82000F04 (0x82000AB4 its base's)
  uint32_t vtable_4() const { return guest::LoadU32(base, address + 4); }
  void set_vtable_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vtable_4_address() const { return address + 4; }
  // 0x82000EE8, the interface WorkerPool::Create returns (0x82000EB8 its base's)
  uint32_t vtable_8() const { return guest::LoadU32(base, address + 8); }
  void set_vtable_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t vtable_8_address() const { return address + 8; }
  // the block's +4 (the framework's +12 interface)
  uint32_t framework_interface() const { return guest::LoadU32(base, address + 12); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t framework_interface_address() const { return address + 12; }
  // the block's +8 (13 or 12)
  uint32_t unknown_16() const { return guest::LoadU32(base, address + 16); }
  void set_unknown_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t unknown_16_address() const { return address + 16; }
  // the block's +12 (4 or 3)
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // the block's +20
  uint32_t thread_count() const { return guest::LoadU32(base, address + 244); }
  void set_thread_count(uint32_t v) const { guest::StoreU32(base, address + 244, v); }
  uint32_t thread_count_address() const { return address + 244; }
  uint32_t unknown_248() const { return guest::LoadU32(base, address + 248); }
  void set_unknown_248(uint32_t v) const { guest::StoreU32(base, address + 248, v); }
  uint32_t unknown_248_address() const { return address + 248; }
  // a job as the debug text's at +8: 0x820066DC
  uint32_t job_vtable() const { return guest::LoadU32(base, address + 256); }
  void set_job_vtable(uint32_t v) const { guest::StoreU32(base, address + 256, v); }
  uint32_t job_vtable_address() const { return address + 256; }
  // 0x82002908 (0x82000AB4 its base's)
  uint32_t job_vtable_4() const { return guest::LoadU32(base, address + 260); }
  void set_job_vtable_4(uint32_t v) const { guest::StoreU32(base, address + 260, v); }
  uint32_t job_vtable_4_address() const { return address + 260; }
  uint32_t job_unknown_8() const { return guest::LoadU32(base, address + 264); }
  void set_job_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 264, v); }
  uint32_t job_unknown_8_address() const { return address + 264; }
  uint32_t job_state() const { return guest::LoadU32(base, address + 268); }
  void set_job_state(uint32_t v) const { guest::StoreU32(base, address + 268, v); }
  uint32_t job_state_address() const { return address + 268; }
  uint8_t job_unknown_16() const { return guest::LoadU8(base, address + 272); }
  void set_job_unknown_16(uint8_t v) const { guest::StoreU8(base, address + 272, v); }
  uint32_t job_unknown_16_address() const { return address + 272; }
  uint8_t job_unknown_17() const { return guest::LoadU8(base, address + 273); }
  void set_job_unknown_17(uint8_t v) const { guest::StoreU8(base, address + 273, v); }
  uint32_t job_unknown_17_address() const { return address + 273; }
  uint8_t job_unknown_18() const { return guest::LoadU8(base, address + 274); }
  void set_job_unknown_18(uint8_t v) const { guest::StoreU8(base, address + 274, v); }
  uint32_t job_unknown_18_address() const { return address + 274; }
  uint32_t job_unknown_20() const { return guest::LoadU32(base, address + 276); }
  void set_job_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 276, v); }
  uint32_t job_unknown_20_address() const { return address + 276; }
  // 0x8200833C
  uint32_t job_delegate_vtable() const { return guest::LoadU32(base, address + 280); }
  void set_job_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 280, v); }
  uint32_t job_delegate_vtable_address() const { return address + 280; }
  template <typename T = WorkerPool>
  T job_delegate_object() const { return T(base, guest::LoadU32(base, address + 284)); }
  void set_job_delegate_object(const View& v) const { guest::StoreU32(base, address + 284, v.address); }
  void set_job_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 284, v); }
  uint32_t job_delegate_object_address() const { return address + 284; }
  // a member function pointer (8 bytes)
  uint64_t job_delegate_function() const { return guest::LoadU64(base, address + 288); }
  void set_job_delegate_function(uint64_t v) const { guest::StoreU64(base, address + 288, v); }
  uint32_t job_delegate_function_address() const { return address + 288; }
};

// embedded; measure: sub_82094F30.r4
struct WorkerPoolBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  template <typename T = RunLoop>
  T run_loop() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_run_loop(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_run_loop(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t run_loop_address() const { return address + 0; }
  // the framework's +12 interface
  uint32_t framework_interface() const { return guest::LoadU32(base, address + 4); }
  void set_framework_interface(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_interface_address() const { return address + 4; }
  // 13 or 12
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  // 4 or 3
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // a table of 8 bytes a thread ({2, 2}, {3, 2, 5, 2})
  uint32_t threads() const { return guest::LoadU32(base, address + 16); }
  void set_threads(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t threads_address() const { return address + 16; }
  uint32_t thread_count() const { return guest::LoadU32(base, address + 20); }
  void set_thread_count(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t thread_count_address() const { return address + 20; }
};

// embedded; measure: DebugText::DebugText.r4.+4
struct WorkerPoolInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = WorkerPoolInterfaceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: DebugText::DebugText.r4.+4.+0
struct WorkerPoolInterfaceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 1: a job posted (r5 = 1 from DebugText::DebugText)
  uint32_t post() const { return guest::LoadU32(base, address + 4); }
  void set_post(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t post_address() const { return address + 4; }
};

// allocated; measure: new<8>@Framework::Init#3
#pragma pack(push, 1)
struct MorphJobMakerData {
  uint32_t vtable = 0;  // +0
  uint32_t worker_pool = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(MorphJobMakerData) == 8);
inline constexpr memory::LayoutField kMorphJobMakerFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kMorphJobMakerLayout = {kMorphJobMakerFields, 2};

struct MorphJobMaker : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82002840;
  MorphJobMakerData* data() const { return static_cast<MorphJobMakerData*>(memory::HandleObject(address)); }
  static MorphJobMaker New(uint8_t* base) {
    return MorphJobMaker(base, memory::MakeHandle(new MorphJobMakerData(), sizeof(MorphJobMakerData), "MorphJobMaker", &kMorphJobMakerLayout));
  }
  template <uint32_t Stride = kSize>
  static MorphJobMaker NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(MorphJobMakerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<MorphJobMakerData*>(::operator new(sizeof(MorphJobMakerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) MorphJobMakerData();
    return MorphJobMaker(base, memory::MakeHandle(p, sizeof(MorphJobMakerData) * count, "MorphJobMaker", &kMorphJobMakerLayout, sizeof(MorphJobMakerData)));
  }
  MorphJobMaker at(uint32_t i) const { return MorphJobMaker(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kMorphJobMakerLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kMorphJobMakerLayout, offset, width, v); }
  // 0x82002840
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the +48 interface's slot 1: the second worker pool
  uint32_t worker_pool_address() const { return address + 4; }
  template <typename T = WorkerPoolInterface>
  T worker_pool() const { return T(base, data()->worker_pool); }
  void set_worker_pool(const View& w) const { const uint32_t v = w.address; data()->worker_pool = v; }
  void set_worker_pool(uint32_t v) const { data()->worker_pool = v; }
  uint32_t worker_pool_bits() const { return data()->worker_pool; }
  void set_worker_pool_bits(uint32_t v) const { data()->worker_pool = v; }
};

// embedded; measure: Framework@+44
#pragma pack(push, 1)
struct FrameworkAt44Data {
  uint32_t unknown_0 = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(FrameworkAt44Data) == 4);
inline constexpr memory::LayoutField kFrameworkAt44Fields[] = {{0, 4}};
inline constexpr memory::Layout kFrameworkAt44Layout = {kFrameworkAt44Fields, 1};

struct FrameworkAt44 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  FrameworkAt44Data* data() const { return static_cast<FrameworkAt44Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static FrameworkAt44 New(uint8_t* base) {
    return FrameworkAt44(base, memory::MakeHandle(new FrameworkAt44Data(), sizeof(FrameworkAt44Data), "FrameworkAt44", &kFrameworkAt44Layout));
  }
  template <uint32_t Stride = kSize>
  static FrameworkAt44 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FrameworkAt44Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrameworkAt44Data*>(::operator new(sizeof(FrameworkAt44Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrameworkAt44Data();
    return FrameworkAt44(base, memory::MakeHandle(p, sizeof(FrameworkAt44Data) * count, "FrameworkAt44", &kFrameworkAt44Layout, sizeof(FrameworkAt44Data)));
  }
  FrameworkAt44 at(uint32_t i) const { return FrameworkAt44(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFrameworkAt44Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFrameworkAt44Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0() const { return on_host() ? uint32_t(data()->unknown_0) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_unknown_0(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_bits() const { return on_host() ? data()->unknown_0 : guest::LoadU32(base, address + 0); }
  void set_unknown_0_bits(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
};

// embedded; measure: Framework@+48
#pragma pack(push, 1)
struct FrameworkAt48Data {
  uint32_t unknown_0 = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(FrameworkAt48Data) == 4);
inline constexpr memory::LayoutField kFrameworkAt48Fields[] = {{0, 4}};
inline constexpr memory::Layout kFrameworkAt48Layout = {kFrameworkAt48Fields, 1};

struct FrameworkAt48 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  FrameworkAt48Data* data() const { return static_cast<FrameworkAt48Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static FrameworkAt48 New(uint8_t* base) {
    return FrameworkAt48(base, memory::MakeHandle(new FrameworkAt48Data(), sizeof(FrameworkAt48Data), "FrameworkAt48", &kFrameworkAt48Layout));
  }
  template <uint32_t Stride = kSize>
  static FrameworkAt48 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(FrameworkAt48Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<FrameworkAt48Data*>(::operator new(sizeof(FrameworkAt48Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) FrameworkAt48Data();
    return FrameworkAt48(base, memory::MakeHandle(p, sizeof(FrameworkAt48Data) * count, "FrameworkAt48", &kFrameworkAt48Layout, sizeof(FrameworkAt48Data)));
  }
  FrameworkAt48 at(uint32_t i) const { return FrameworkAt48(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kFrameworkAt48Layout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kFrameworkAt48Layout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0() const { return on_host() ? uint32_t(data()->unknown_0) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_unknown_0(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_bits() const { return on_host() ? data()->unknown_0 : guest::LoadU32(base, address + 0); }
  void set_unknown_0_bits(uint32_t v) const { if (on_host()) data()->unknown_0 = v; else guest::StoreU32(base, address + 0, v); }
};

// embedded; measure: new<296>@WorkerPool::Create@+28
struct WorkerPoolAt28 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/framework.md's code uses.
struct FrameworkGlobals {
  // stored by DebugText::DebugText
  template <typename T = DebugText>
  static T debug_text(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8266D080u)); }
  static void set_debug_text(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8266D080u, v.address); }
  static void set_debug_text(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8266D080u, v); }
  static constexpr uint32_t debug_text_address() { return 0x8266D080u; }
  // the first pool maker (framework + 112)
  static uint32_t pool_maker(uint8_t* base) { return guest::LoadU32(base, 0x8296B910u); }
  static void set_pool_maker(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B910u, v); }
  static constexpr uint32_t pool_maker_address() { return 0x8296B910u; }
  // framework + 116
  template <typename T = MorphJobMaker>
  static T morph_job_maker(uint8_t* base) { return T(base, guest::LoadU32(base, 0x8296B914u)); }
  static void set_morph_job_maker(uint8_t* base, const View& v) { guest::StoreU32(base, 0x8296B914u, v.address); }
  static void set_morph_job_maker(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296B914u, v); }
  static constexpr uint32_t morph_job_maker_address() { return 0x8296B914u; }
  // 1 once DebugFont::Init ran
  static uint8_t debug_font_loaded(uint8_t* base) { return guest::LoadU8(base, 0x8265ED40u); }
  static void set_debug_font_loaded(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x8265ED40u, v); }
  static constexpr uint32_t debug_font_loaded_address() { return 0x8265ED40u; }
  // the debug font (0x8296BCE0): +0, +4 probably the cursor
  static uint32_t debug_font_cursor_x(uint8_t* base) { return guest::LoadU32(base, 0x8296BCE0u); }
  static void set_debug_font_cursor_x(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BCE0u, v); }
  static constexpr uint32_t debug_font_cursor_x_address() { return 0x8296BCE0u; }
  static uint32_t debug_font_cursor_y(uint8_t* base) { return guest::LoadU32(base, 0x8296BCE4u); }
  static void set_debug_font_cursor_y(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BCE4u, v); }
  static constexpr uint32_t debug_font_cursor_y_address() { return 0x8296BCE4u; }
  // +8, +12 probably the line start
  static uint32_t debug_font_line_x(uint8_t* base) { return guest::LoadU32(base, 0x8296BCE8u); }
  static void set_debug_font_line_x(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BCE8u, v); }
  static constexpr uint32_t debug_font_line_x_address() { return 0x8296BCE8u; }
  static uint32_t debug_font_line_y(uint8_t* base) { return guest::LoadU32(base, 0x8296BCECu); }
  static void set_debug_font_line_y(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BCECu, v); }
  static constexpr uint32_t debug_font_line_y_address() { return 0x8296BCECu; }
  // cleared as a word by DebugFont::Init, then its bytes set to 255
  static uint32_t debug_font_word_16(uint8_t* base) { return guest::LoadU32(base, 0x8296BCF0u); }
  static void set_debug_font_word_16(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BCF0u, v); }
  static constexpr uint32_t debug_font_word_16_address() { return 0x8296BCF0u; }
  static uint8_t debug_font_byte_16(uint8_t* base) { return guest::LoadU8(base, 0x8296BCF0u); }
  static void set_debug_font_byte_16(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x8296BCF0u, v); }
  static constexpr uint32_t debug_font_byte_16_address() { return 0x8296BCF0u; }
  static uint8_t debug_font_byte_17(uint8_t* base) { return guest::LoadU8(base, 0x8296BCF1u); }
  static void set_debug_font_byte_17(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x8296BCF1u, v); }
  static constexpr uint32_t debug_font_byte_17_address() { return 0x8296BCF1u; }
  static uint8_t debug_font_byte_18(uint8_t* base) { return guest::LoadU8(base, 0x8296BCF2u); }
  static void set_debug_font_byte_18(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x8296BCF2u, v); }
  static constexpr uint32_t debug_font_byte_18_address() { return 0x8296BCF2u; }
  static uint8_t debug_font_byte_19(uint8_t* base) { return guest::LoadU8(base, 0x8296BCF3u); }
  static void set_debug_font_byte_19(uint8_t* base, uint8_t v) { guest::StoreU8(base, 0x8296BCF3u, v); }
  static constexpr uint32_t debug_font_byte_19_address() { return 0x8296BCF3u; }
  // +20: the texture set (24 bytes, XPR)
  static uint32_t debug_font_xpr(uint8_t* base) { return guest::LoadU32(base, 0x8296BCF4u); }
  static void set_debug_font_xpr(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BCF4u, v); }
  static constexpr uint32_t debug_font_xpr_address() { return 0x8296BCF4u; }
  // +32, probably a scale: the float at 0x82001FAC
  static float debug_font_scale_x(uint8_t* base) { return guest::LoadFloat(base, 0x8296BD00u); }
  static void set_debug_font_scale_x(uint8_t* base, float v) { guest::StoreFloat(base, 0x8296BD00u, v); }
  static constexpr uint32_t debug_font_scale_x_address() { return 0x8296BD00u; }
  // +36
  static float debug_font_scale_y(uint8_t* base) { return guest::LoadFloat(base, 0x8296BD04u); }
  static void set_debug_font_scale_y(uint8_t* base, float v) { guest::StoreFloat(base, 0x8296BD04u, v); }
  static constexpr uint32_t debug_font_scale_y_address() { return 0x8296BD04u; }
  // +40: 681 (sub_8258BE48)
  static uint32_t debug_font_word_40(uint8_t* base) { return guest::LoadU32(base, 0x8296BD08u); }
  static void set_debug_font_word_40(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8296BD08u, v); }
  static constexpr uint32_t debug_font_word_40_address() { return 0x8296BD08u; }
  // read-only data
  static float debug_font_default_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001FACu); }
  static void set_debug_font_default_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FACu, v); }
  static constexpr uint32_t debug_font_default_scale_address() { return 0x82001FACu; }
};

}  // namespace schema
