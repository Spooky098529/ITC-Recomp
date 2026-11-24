// The views of docs/functions/input.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/input.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: InputManager, Input, InputManager::AddListener.r3
#pragma pack(push, 1)
struct InputManagerData {
  uint32_t vtable = 0;  // +0
  uint32_t framework_part = 0;  // +4
  uint8_t logical_pads[980] = {};  // +8, guest order
  uint8_t physical_pads[852] = {};  // +988, guest order
  uint8_t processors[2080] = {};  // +1840, guest order
  uint8_t listener_lists[60] = {};  // +3920, guest order
  uint32_t unknown_3980 = 0;  // +3980
};
#pragma pack(pop)
static_assert(sizeof(InputManagerData) == 3984);
inline constexpr memory::LayoutField kInputManagerFields[] = {{0, 4}, {4, 4}, {8, 4}, {16, 4}, {24, 4}, {32, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {204, 4}, {212, 4}, {220, 4}, {228, 4}, {236, 4}, {240, 4}, {244, 4}, {248, 4}, {252, 4}, {256, 4}, {260, 4}, {264, 4}, {400, 4}, {408, 4}, {416, 4}, {424, 4}, {432, 4}, {436, 4}, {440, 4}, {444, 4}, {448, 4}, {452, 4}, {456, 4}, {460, 4}, {596, 4}, {604, 4}, {612, 4}, {620, 4}, {628, 4}, {632, 4}, {636, 4}, {640, 4}, {644, 4}, {648, 4}, {652, 4}, {656, 4}, {792, 4}, {800, 4}, {808, 4}, {816, 4}, {824, 4}, {828, 4}, {832, 4}, {836, 4}, {840, 4}, {844, 4}, {848, 4}, {852, 4}, {988, 4}, {992, 4}, {996, 4}, {1000, 4}, {1004, 4}, {1008, 4}, {1012, 4}, {1016, 4}, {1020, 4}, {1156, 4}, {1160, 4}, {1164, 4}, {1168, 4}, {1172, 4}, {1176, 4}, {1200, 4}, {1204, 4}, {1208, 4}, {1212, 4}, {1216, 4}, {1220, 4}, {1224, 4}, {1228, 4}, {1232, 4}, {1368, 4}, {1372, 4}, {1376, 4}, {1380, 4}, {1384, 4}, {1388, 4}, {1412, 4}, {1416, 4}, {1420, 4}, {1424, 4}, {1428, 4}, {1432, 4}, {1436, 4}, {1440, 4}, {1444, 4}, {1580, 4}, {1584, 4}, {1588, 4}, {1592, 4}, {1596, 4}, {1600, 4}, {1624, 4}, {1628, 4}, {1632, 4}, {1636, 4}, {1640, 4}, {1644, 4}, {1648, 4}, {1652, 4}, {1656, 4}, {1792, 4}, {1796, 4}, {1800, 4}, {1804, 4}, {1808, 4}, {1812, 4}, {1840, 4}, {1856, 4}, {1860, 4}, {1864, 4}, {1868, 4}, {1872, 4}, {1876, 4}, {1880, 4}, {1884, 4}, {1888, 4}, {1892, 4}, {1896, 4}, {1900, 4}, {1904, 4}, {1908, 4}, {1912, 4}, {1916, 4}, {1920, 4}, {1924, 4}, {1928, 4}, {1932, 4}, {1936, 4}, {1940, 4}, {1944, 4}, {1948, 4}, {1952, 4}, {1956, 4}, {1960, 4}, {1964, 4}, {1968, 4}, {1972, 4}, {1976, 4}, {2104, 4}, {2108, 4}, {2112, 4}, {2116, 4}, {2120, 4}, {2256, 4}, {2272, 4}, {2276, 4}, {2280, 4}, {2284, 4}, {2288, 4}, {2292, 4}, {2296, 4}, {2300, 4}, {2304, 4}, {2308, 4}, {2312, 4}, {2316, 4}, {2320, 4}, {2324, 4}, {2328, 4}, {2332, 4}, {2336, 4}, {2340, 4}, {2344, 4}, {2348, 4}, {2352, 4}, {2356, 4}, {2360, 4}, {2364, 4}, {2368, 4}, {2372, 4}, {2376, 4}, {2380, 4}, {2384, 4}, {2388, 4}, {2392, 4}, {2520, 4}, {2524, 4}, {2528, 4}, {2532, 4}, {2536, 4}, {2672, 4}, {2688, 4}, {2692, 4}, {2696, 4}, {2700, 4}, {2704, 4}, {2708, 4}, {2712, 4}, {2716, 4}, {2720, 4}, {2724, 4}, {2728, 4}, {2732, 4}, {2736, 4}, {2740, 4}, {2744, 4}, {2748, 4}, {2752, 4}, {2756, 4}, {2760, 4}, {2764, 4}, {2768, 4}, {2772, 4}, {2776, 4}, {2780, 4}, {2784, 4}, {2788, 4}, {2792, 4}, {2796, 4}, {2800, 4}, {2804, 4}, {2808, 4}, {2936, 4}, {2940, 4}, {2944, 4}, {2948, 4}, {2952, 4}, {3088, 4}, {3104, 4}, {3108, 4}, {3112, 4}, {3116, 4}, {3120, 4}, {3124, 4}, {3128, 4}, {3132, 4}, {3136, 4}, {3140, 4}, {3144, 4}, {3148, 4}, {3152, 4}, {3156, 4}, {3160, 4}, {3164, 4}, {3168, 4}, {3172, 4}, {3176, 4}, {3180, 4}, {3184, 4}, {3188, 4}, {3192, 4}, {3196, 4}, {3200, 4}, {3204, 4}, {3208, 4}, {3212, 4}, {3216, 4}, {3220, 4}, {3224, 4}, {3352, 4}, {3356, 4}, {3360, 4}, {3364, 4}, {3368, 4}, {3504, 4}, {3520, 4}, {3524, 4}, {3528, 4}, {3532, 4}, {3536, 4}, {3540, 4}, {3544, 4}, {3548, 4}, {3552, 4}, {3556, 4}, {3560, 4}, {3564, 4}, {3568, 4}, {3572, 4}, {3576, 4}, {3580, 4}, {3584, 4}, {3588, 4}, {3592, 4}, {3596, 4}, {3600, 4}, {3604, 4}, {3608, 4}, {3612, 4}, {3616, 4}, {3620, 4}, {3624, 4}, {3628, 4}, {3632, 4}, {3636, 4}, {3640, 4}, {3768, 4}, {3772, 4}, {3776, 4}, {3780, 4}, {3784, 4}, {3924, 4}, {3928, 4}, {3936, 4}, {3940, 4}, {3948, 4}, {3952, 4}, {3960, 4}, {3964, 4}, {3972, 4}, {3976, 4}, {3980, 4}};
inline constexpr memory::Layout kInputManagerLayout = {kInputManagerFields, 318};

struct InputManager : View {
  using View::View;
  static constexpr uint32_t kSize = 3984;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82000F48;
  InputManagerData* data() const { return static_cast<InputManagerData*>(memory::HandleObject(address)); }
  static InputManager New(uint8_t* base) {
    return InputManager(base, memory::MakeHandle(new InputManagerData(), sizeof(InputManagerData), "InputManager", &kInputManagerLayout));
  }
  template <uint32_t Stride = kSize>
  static InputManager NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(InputManagerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InputManagerData*>(::operator new(sizeof(InputManagerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InputManagerData();
    return InputManager(base, memory::MakeHandle(p, sizeof(InputManagerData) * count, "InputManager", &kInputManagerLayout, sizeof(InputManagerData)));
  }
  InputManager at(uint32_t i) const { return InputManager(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kInputManagerLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputManagerLayout, offset, width, v); }
  // 0x82000F48: 0 sub_82098FF8, 1 GetProcessor, 2 GetLogicalPad, 3 AddListener, 4 sub_82099048, 5 GetPhysicalPad
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the set-up block's +4: a pointer to the framework's +16
  uint32_t framework_part_address() const { return address + 4; }
  uint32_t framework_part() const { return data()->framework_part; }
  void set_framework_part(uint32_t v) const { data()->framework_part = v; }
  uint32_t framework_part_bits() const { return data()->framework_part; }
  void set_framework_part_bits(uint32_t v) const { data()->framework_part = v; }
  // five LogicalPads (196 bytes): 0-3 the players', 4 the any pad (the embedded logical_pad)
  uint32_t logical_pads_address() const { return address + 8; }
  uint8_t* logical_pads() const { return reinterpret_cast<uint8_t*>(data()) + 8; }
  template <typename T = LogicalPad>
  T logical_pad(uint32_t i) const { return T(base, address + 8 + i * T::kSize); }
  // four physical pads (212 bytes, model-animation.toml's PhysicalPad; the embedded physical_pad), then 4 bytes not used
  uint32_t physical_pads_address() const { return address + 988; }
  uint8_t* physical_pads() const { return reinterpret_cast<uint8_t*>(data()) + 988; }
  template <typename T = PhysicalPad>
  T physical_pad(uint32_t i) const { return T(base, address + 988 + i * T::kSize); }
  // five InputProcessors (416 bytes; the embedded processor): Input::Update reads processor 0's vtable here
  uint32_t processors_address() const { return address + 1840; }
  uint8_t* processors() const { return reinterpret_cast<uint8_t*>(data()) + 1840; }
  template <typename T = InputProcessor>
  T processor(uint32_t i) const { return T(base, address + 1840 + i * T::kSize); }
  // five PtrLists (12 bytes) of listeners, one per logical pad (the embedded listener_list): Input::Update reads list 0's head and size at +3924 and +3928
  uint32_t listener_lists_address() const { return address + 3920; }
  uint8_t* listener_lists() const { return reinterpret_cast<uint8_t*>(data()) + 3920; }
  template <typename T = PtrList>
  T listener_list(uint32_t i) const { return T(base, address + 3920 + i * T::kSize); }
  // not set by the constructor
  uint32_t unknown_3980_address() const { return address + 3980; }
  uint32_t unknown_3980() const { return data()->unknown_3980; }
  void set_unknown_3980(uint32_t v) const { data()->unknown_3980 = v; }
  uint32_t unknown_3980_bits() const { return data()->unknown_3980; }
  void set_unknown_3980_bits(uint32_t v) const { data()->unknown_3980 = v; }
};

// embedded; measure: InputManager::InputManager.r4
struct InputManagerArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the message dispatcher
  template <typename T = InputDispatcher>
  T dispatcher() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_dispatcher(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_dispatcher(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t dispatcher_address() const { return address + 0; }
  // a pointer to the framework's +16, kept at the manager's +4
  uint32_t framework_part() const { return guest::LoadU32(base, address + 4); }
  void set_framework_part(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t framework_part_address() const { return address + 4; }
};

// embedded; measure: InputManager::InputManager.r4.+0
struct InputDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = InputDispatcherVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the vtable's slot +4 (InputDispatcherVtable), read through r11 after the vtable's load
  uint32_t fault_vtable_4() const { return guest::LoadU32(base, address + 4); }
  void set_fault_vtable_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t fault_vtable_4_address() const { return address + 4; }
  // measure fault: the vtable's slot +8, as fault_vtable_4
  uint32_t fault_vtable_8() const { return guest::LoadU32(base, address + 8); }
  void set_fault_vtable_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fault_vtable_8_address() const { return address + 8; }
};

// image; measure: -
struct InputDispatcherVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // slot +4: registers message 7's handler (sub_82098CE8)
  uint32_t register_7() const { return guest::LoadU32(base, address + 4); }
  void set_register_7(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t register_7_address() const { return address + 4; }
  // slot +8: registers message 5's handler (Input::Update sub_820990F0)
  uint32_t register_5() const { return guest::LoadU32(base, address + 8); }
  void set_register_5(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t register_5_address() const { return address + 8; }
};

// embedded; measure: InputProcessor::Update.r4, LogicalPad::Bind.r3, InputManager@+8
#pragma pack(push, 1)
struct LogicalPadData {
  uint32_t vtable = 0;  // +0
  uint8_t bindings[32] = {};  // +4, guest order
  uint8_t values[160] = {};  // +36, 1 x ActionPadValues
};
#pragma pack(pop)
static_assert(sizeof(LogicalPadData) == 196);
inline constexpr memory::LayoutField kLogicalPadFields[] = {{0, 4}, {8, 4}, {16, 4}, {24, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}};
inline constexpr memory::Layout kLogicalPadLayout = {kLogicalPadFields, 12};

struct LogicalPad : View {
  using View::View;
  static constexpr uint32_t kSize = 196;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8206CBEC;
  LogicalPadData* data() const { return static_cast<LogicalPadData*>(memory::HandleObject(address)); }
  static LogicalPad New(uint8_t* base) {
    return LogicalPad(base, memory::MakeHandle(new LogicalPadData(), sizeof(LogicalPadData), "LogicalPad", &kLogicalPadLayout));
  }
  template <uint32_t Stride = kSize>
  static LogicalPad NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(LogicalPadData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<LogicalPadData*>(::operator new(sizeof(LogicalPadData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) LogicalPadData();
    return LogicalPad(base, memory::MakeHandle(p, sizeof(LogicalPadData) * count, "LogicalPad", &kLogicalPadLayout, sizeof(LogicalPadData)));
  }
  LogicalPad at(uint32_t i) const { return LogicalPad(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kLogicalPadLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kLogicalPadLayout, offset, width, v); }
  // 0x8206CBEC (InputProcessor::Update's r4: any device, a logical pad in play)
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = InputDeviceVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // four InputPadBindings (8 bytes; the embedded binding), cleared word by word by the constructor
  uint32_t bindings_address() const { return address + 4; }
  uint8_t* bindings() const { return reinterpret_cast<uint8_t*>(data()) + 4; }
  template <typename T = InputPadBinding>
  T binding(uint32_t i) const { return T(base, address + 4 + i * T::kSize); }
  // the 40 values (to +195): 0-7 the sticks and triggers (LX, LY, LT, 0, RX, RY, RT, 0), 8-39 the buttons (action-mappers.toml's ActionPadValues, what the mappers read)
  template <typename T = ActionPadValues>
  T values() const { return T(base, address + 36); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct InputPadBindingData {
  uint8_t enabled = 0;  // +0
  uint8_t pad_1[3] = {};
  uint32_t device = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(InputPadBindingData) == 8);
inline constexpr memory::LayoutField kInputPadBindingFields[] = {{4, 4}};
inline constexpr memory::Layout kInputPadBindingLayout = {kInputPadBindingFields, 1};

struct InputPadBinding : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  InputPadBindingData* data() const { return static_cast<InputPadBindingData*>(memory::HandleObject(address)); }
  static InputPadBinding New(uint8_t* base) {
    return InputPadBinding(base, memory::MakeHandle(new InputPadBindingData(), sizeof(InputPadBindingData), "InputPadBinding", &kInputPadBindingLayout));
  }
  template <uint32_t Stride = kSize>
  static InputPadBinding NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(InputPadBindingData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InputPadBindingData*>(::operator new(sizeof(InputPadBindingData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InputPadBindingData();
    return InputPadBinding(base, memory::MakeHandle(p, sizeof(InputPadBindingData) * count, "InputPadBinding", &kInputPadBindingLayout, sizeof(InputPadBindingData)));
  }
  InputPadBinding at(uint32_t i) const { return InputPadBinding(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kInputPadBindingLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputPadBindingLayout, offset, width, v); }
  uint32_t enabled_address() const { return address + 0; }
  uint8_t enabled() const { return data()->enabled; }
  void set_enabled(uint8_t v) const { data()->enabled = v; }
  uint8_t enabled_bits() const { return data()->enabled; }
  void set_enabled_bits(uint8_t v) const { data()->enabled = v; }
  // a physical pad, or for the any pad a logical pad 0-3
  uint32_t device_address() const { return address + 4; }
  uint32_t device() const { return data()->device; }
  void set_device(uint32_t v) const { data()->device = v; }
  uint32_t device_bits() const { return data()->device; }
  void set_device_bits(uint32_t v) const { data()->device = v; }
};

// image; measure: InputProcessor::Update.r4.+0
struct InputDeviceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0, probably the destructor (sub_82591898, sub_82592058)
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: the values' address (LogicalPad::Values sub_82591B30, PhysicalPad::Values sub_820988B8)
  uint32_t values() const { return guest::LoadU32(base, address + 4); }
  void set_values(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t values_address() const { return address + 4; }
};

// embedded; measure: InputProcessor, InputProcessorBase, InputProcessor::Assign.r4, InputProcessor::InputProcessor.r3
#pragma pack(push, 1)
struct InputProcessorData {
  uint32_t vtable = 0;  // +0
  uint8_t pad_4[12] = {};
  float dead_zone_x = 0;  // +16
  float dead_zone_y = 0;  // +20
  float dead_zone_trigger = 0;  // +24
  float dead_zone_unused = 0;  // +28
  float raw_lx = 0;  // +32
  float raw_ly = 0;  // +36
  float raw_lt = 0;  // +40
  float raw_3 = 0;  // +44
  float raw_rx = 0;  // +48
  float raw_ry = 0;  // +52
  float raw_rt = 0;  // +56
  float raw_7 = 0;  // +60
  float adjusted_lx = 0;  // +64
  float adjusted_ly = 0;  // +68
  float adjusted_lt = 0;  // +72
  float adjusted_3 = 0;  // +76
  float adjusted_rx = 0;  // +80
  float adjusted_ry = 0;  // +84
  float adjusted_rt = 0;  // +88
  float adjusted_7 = 0;  // +92
  float left_direction_x = 0;  // +96
  float left_direction_y = 0;  // +100
  float left_direction_z = 0;  // +104
  float left_direction_w = 0;  // +108
  float right_direction_x = 0;  // +112
  float right_direction_y = 0;  // +116
  float right_direction_z = 0;  // +120
  float right_direction_w = 0;  // +124
  float left_length = 0;  // +128
  float right_length = 0;  // +132
  float buttons = 0;  // +136
  uint8_t pad_140[124] = {};
  uint32_t pressed = 0;  // +264
  uint32_t released = 0;  // +268
  uint32_t held = 0;  // +272
  int32_t repeat_delay = 0;  // +276
  int32_t repeat_period = 0;  // +280
  uint8_t held_ticks[128] = {};  // +284, guest order
  uint8_t pad_412[4] = {};
};
#pragma pack(pop)
static_assert(sizeof(InputProcessorData) == 416);
inline constexpr memory::LayoutField kInputProcessorFields[] = {{0, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {264, 4}, {268, 4}, {272, 4}, {276, 4}, {280, 4}};
inline constexpr memory::Layout kInputProcessorLayout = {kInputProcessorFields, 37};

struct InputProcessor : View {
  using View::View;
  static constexpr uint32_t kSize = 416;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x8206C0C4;
  InputProcessorData* data() const { return static_cast<InputProcessorData*>(memory::HandleObject(address)); }
  static InputProcessor New(uint8_t* base) {
    return InputProcessor(base, memory::MakeHandle(new InputProcessorData(), sizeof(InputProcessorData), "InputProcessor", &kInputProcessorLayout));
  }
  template <uint32_t Stride = kSize>
  static InputProcessor NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(InputProcessorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InputProcessorData*>(::operator new(sizeof(InputProcessorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InputProcessorData();
    return InputProcessor(base, memory::MakeHandle(p, sizeof(InputProcessorData) * count, "InputProcessor", &kInputProcessorLayout, sizeof(InputProcessorData)));
  }
  InputProcessor at(uint32_t i) const { return InputProcessor(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kInputProcessorLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, offset, width, v); }
  // 0x8206C0C4 (the base's 0x8206CC00 first)
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = InputProcessorVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // 0.5 at construction; the four dead zones apply to both sticks
  uint32_t dead_zone_x_address() const { return address + 16; }
  float dead_zone_x() const { return data()->dead_zone_x; }
  void set_dead_zone_x(float v) const { data()->dead_zone_x = v; }
  uint32_t dead_zone_x_bits() const { return std::bit_cast<uint32_t>(data()->dead_zone_x); }
  void set_dead_zone_x_bits(uint32_t v) const { data()->dead_zone_x = std::bit_cast<float>(v); }
  uint32_t dead_zone_x_pair_address() const { return address + 16; }
  uint64_t dead_zone_x_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 16, 8)); }
  void set_dead_zone_x_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 16, 8, v); }
  uint64_t dead_zone_x_pair() const { return uint64_t(dead_zone_x_pair_bits()); }
  void set_dead_zone_x_pair(uint64_t v) const { set_dead_zone_x_pair_bits(uint64_t(v)); }
  // 0.5
  uint32_t dead_zone_y_address() const { return address + 20; }
  float dead_zone_y() const { return data()->dead_zone_y; }
  void set_dead_zone_y(float v) const { data()->dead_zone_y = v; }
  uint32_t dead_zone_y_bits() const { return std::bit_cast<uint32_t>(data()->dead_zone_y); }
  void set_dead_zone_y_bits(uint32_t v) const { data()->dead_zone_y = std::bit_cast<float>(v); }
  // 0.0
  uint32_t dead_zone_trigger_address() const { return address + 24; }
  float dead_zone_trigger() const { return data()->dead_zone_trigger; }
  void set_dead_zone_trigger(float v) const { data()->dead_zone_trigger = v; }
  uint32_t dead_zone_trigger_bits() const { return std::bit_cast<uint32_t>(data()->dead_zone_trigger); }
  void set_dead_zone_trigger_bits(uint32_t v) const { data()->dead_zone_trigger = std::bit_cast<float>(v); }
  uint32_t dead_zone_trigger_pair_address() const { return address + 24; }
  uint64_t dead_zone_trigger_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 24, 8)); }
  void set_dead_zone_trigger_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 24, 8, v); }
  uint64_t dead_zone_trigger_pair() const { return uint64_t(dead_zone_trigger_pair_bits()); }
  void set_dead_zone_trigger_pair(uint64_t v) const { set_dead_zone_trigger_pair_bits(uint64_t(v)); }
  // 0.0
  uint32_t dead_zone_unused_address() const { return address + 28; }
  float dead_zone_unused() const { return data()->dead_zone_unused; }
  void set_dead_zone_unused(float v) const { data()->dead_zone_unused = v; }
  uint32_t dead_zone_unused_bits() const { return std::bit_cast<uint32_t>(data()->dead_zone_unused); }
  void set_dead_zone_unused_bits(uint32_t v) const { data()->dead_zone_unused = std::bit_cast<float>(v); }
  // the pad's values 0-7 (to +63)
  uint32_t raw_lx_address() const { return address + 32; }
  float raw_lx() const { return data()->raw_lx; }
  void set_raw_lx(float v) const { data()->raw_lx = v; }
  uint32_t raw_lx_bits() const { return std::bit_cast<uint32_t>(data()->raw_lx); }
  void set_raw_lx_bits(uint32_t v) const { data()->raw_lx = std::bit_cast<float>(v); }
  uint32_t raw_lx_pair_address() const { return address + 32; }
  uint64_t raw_lx_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 32, 8)); }
  void set_raw_lx_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 32, 8, v); }
  uint64_t raw_lx_pair() const { return uint64_t(raw_lx_pair_bits()); }
  void set_raw_lx_pair(uint64_t v) const { set_raw_lx_pair_bits(uint64_t(v)); }
  uint32_t raw_ly_address() const { return address + 36; }
  float raw_ly() const { return data()->raw_ly; }
  void set_raw_ly(float v) const { data()->raw_ly = v; }
  uint32_t raw_ly_bits() const { return std::bit_cast<uint32_t>(data()->raw_ly); }
  void set_raw_ly_bits(uint32_t v) const { data()->raw_ly = std::bit_cast<float>(v); }
  uint32_t raw_lt_address() const { return address + 40; }
  float raw_lt() const { return data()->raw_lt; }
  void set_raw_lt(float v) const { data()->raw_lt = v; }
  uint32_t raw_lt_bits() const { return std::bit_cast<uint32_t>(data()->raw_lt); }
  void set_raw_lt_bits(uint32_t v) const { data()->raw_lt = std::bit_cast<float>(v); }
  uint32_t raw_lt_pair_address() const { return address + 40; }
  uint64_t raw_lt_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 40, 8)); }
  void set_raw_lt_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 40, 8, v); }
  uint64_t raw_lt_pair() const { return uint64_t(raw_lt_pair_bits()); }
  void set_raw_lt_pair(uint64_t v) const { set_raw_lt_pair_bits(uint64_t(v)); }
  // 1.0 at construction (a vector's w)
  uint32_t raw_3_address() const { return address + 44; }
  float raw_3() const { return data()->raw_3; }
  void set_raw_3(float v) const { data()->raw_3 = v; }
  uint32_t raw_3_bits() const { return std::bit_cast<uint32_t>(data()->raw_3); }
  void set_raw_3_bits(uint32_t v) const { data()->raw_3 = std::bit_cast<float>(v); }
  uint32_t raw_rx_address() const { return address + 48; }
  float raw_rx() const { return data()->raw_rx; }
  void set_raw_rx(float v) const { data()->raw_rx = v; }
  uint32_t raw_rx_bits() const { return std::bit_cast<uint32_t>(data()->raw_rx); }
  void set_raw_rx_bits(uint32_t v) const { data()->raw_rx = std::bit_cast<float>(v); }
  uint32_t raw_rx_pair_address() const { return address + 48; }
  uint64_t raw_rx_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 48, 8)); }
  void set_raw_rx_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 48, 8, v); }
  uint64_t raw_rx_pair() const { return uint64_t(raw_rx_pair_bits()); }
  void set_raw_rx_pair(uint64_t v) const { set_raw_rx_pair_bits(uint64_t(v)); }
  uint32_t raw_ry_address() const { return address + 52; }
  float raw_ry() const { return data()->raw_ry; }
  void set_raw_ry(float v) const { data()->raw_ry = v; }
  uint32_t raw_ry_bits() const { return std::bit_cast<uint32_t>(data()->raw_ry); }
  void set_raw_ry_bits(uint32_t v) const { data()->raw_ry = std::bit_cast<float>(v); }
  uint32_t raw_rt_address() const { return address + 56; }
  float raw_rt() const { return data()->raw_rt; }
  void set_raw_rt(float v) const { data()->raw_rt = v; }
  uint32_t raw_rt_bits() const { return std::bit_cast<uint32_t>(data()->raw_rt); }
  void set_raw_rt_bits(uint32_t v) const { data()->raw_rt = std::bit_cast<float>(v); }
  uint32_t raw_rt_pair_address() const { return address + 56; }
  uint64_t raw_rt_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 56, 8)); }
  void set_raw_rt_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 56, 8, v); }
  uint64_t raw_rt_pair() const { return uint64_t(raw_rt_pair_bits()); }
  void set_raw_rt_pair(uint64_t v) const { set_raw_rt_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t raw_7_address() const { return address + 60; }
  float raw_7() const { return data()->raw_7; }
  void set_raw_7(float v) const { data()->raw_7 = v; }
  uint32_t raw_7_bits() const { return std::bit_cast<uint32_t>(data()->raw_7); }
  void set_raw_7_bits(uint32_t v) const { data()->raw_7 = std::bit_cast<float>(v); }
  // the raw values with the dead zone taken out (to +95)
  uint32_t adjusted_lx_address() const { return address + 64; }
  float adjusted_lx() const { return data()->adjusted_lx; }
  void set_adjusted_lx(float v) const { data()->adjusted_lx = v; }
  uint32_t adjusted_lx_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_lx); }
  void set_adjusted_lx_bits(uint32_t v) const { data()->adjusted_lx = std::bit_cast<float>(v); }
  uint32_t adjusted_lx_pair_address() const { return address + 64; }
  uint64_t adjusted_lx_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 64, 8)); }
  void set_adjusted_lx_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 64, 8, v); }
  uint64_t adjusted_lx_pair() const { return uint64_t(adjusted_lx_pair_bits()); }
  void set_adjusted_lx_pair(uint64_t v) const { set_adjusted_lx_pair_bits(uint64_t(v)); }
  uint32_t adjusted_ly_address() const { return address + 68; }
  float adjusted_ly() const { return data()->adjusted_ly; }
  void set_adjusted_ly(float v) const { data()->adjusted_ly = v; }
  uint32_t adjusted_ly_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_ly); }
  void set_adjusted_ly_bits(uint32_t v) const { data()->adjusted_ly = std::bit_cast<float>(v); }
  uint32_t adjusted_lt_address() const { return address + 72; }
  float adjusted_lt() const { return data()->adjusted_lt; }
  void set_adjusted_lt(float v) const { data()->adjusted_lt = v; }
  uint32_t adjusted_lt_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_lt); }
  void set_adjusted_lt_bits(uint32_t v) const { data()->adjusted_lt = std::bit_cast<float>(v); }
  uint32_t adjusted_lt_pair_address() const { return address + 72; }
  uint64_t adjusted_lt_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 72, 8)); }
  void set_adjusted_lt_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 72, 8, v); }
  uint64_t adjusted_lt_pair() const { return uint64_t(adjusted_lt_pair_bits()); }
  void set_adjusted_lt_pair(uint64_t v) const { set_adjusted_lt_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t adjusted_3_address() const { return address + 76; }
  float adjusted_3() const { return data()->adjusted_3; }
  void set_adjusted_3(float v) const { data()->adjusted_3 = v; }
  uint32_t adjusted_3_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_3); }
  void set_adjusted_3_bits(uint32_t v) const { data()->adjusted_3 = std::bit_cast<float>(v); }
  uint32_t adjusted_rx_address() const { return address + 80; }
  float adjusted_rx() const { return data()->adjusted_rx; }
  void set_adjusted_rx(float v) const { data()->adjusted_rx = v; }
  uint32_t adjusted_rx_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_rx); }
  void set_adjusted_rx_bits(uint32_t v) const { data()->adjusted_rx = std::bit_cast<float>(v); }
  uint32_t adjusted_rx_pair_address() const { return address + 80; }
  uint64_t adjusted_rx_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 80, 8)); }
  void set_adjusted_rx_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 80, 8, v); }
  uint64_t adjusted_rx_pair() const { return uint64_t(adjusted_rx_pair_bits()); }
  void set_adjusted_rx_pair(uint64_t v) const { set_adjusted_rx_pair_bits(uint64_t(v)); }
  uint32_t adjusted_ry_address() const { return address + 84; }
  float adjusted_ry() const { return data()->adjusted_ry; }
  void set_adjusted_ry(float v) const { data()->adjusted_ry = v; }
  uint32_t adjusted_ry_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_ry); }
  void set_adjusted_ry_bits(uint32_t v) const { data()->adjusted_ry = std::bit_cast<float>(v); }
  uint32_t adjusted_rt_address() const { return address + 88; }
  float adjusted_rt() const { return data()->adjusted_rt; }
  void set_adjusted_rt(float v) const { data()->adjusted_rt = v; }
  uint32_t adjusted_rt_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_rt); }
  void set_adjusted_rt_bits(uint32_t v) const { data()->adjusted_rt = std::bit_cast<float>(v); }
  uint32_t adjusted_rt_pair_address() const { return address + 88; }
  uint64_t adjusted_rt_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 88, 8)); }
  void set_adjusted_rt_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 88, 8, v); }
  uint64_t adjusted_rt_pair() const { return uint64_t(adjusted_rt_pair_bits()); }
  void set_adjusted_rt_pair(uint64_t v) const { set_adjusted_rt_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t adjusted_7_address() const { return address + 92; }
  float adjusted_7() const { return data()->adjusted_7; }
  void set_adjusted_7(float v) const { data()->adjusted_7 = v; }
  uint32_t adjusted_7_bits() const { return std::bit_cast<uint32_t>(data()->adjusted_7); }
  void set_adjusted_7_bits(uint32_t v) const { data()->adjusted_7 = std::bit_cast<float>(v); }
  // the left stick's (x, y) normalized, or 0 (stvx: 16 bytes)
  uint32_t left_direction_x_address() const { return address + 96; }
  float left_direction_x() const { return data()->left_direction_x; }
  void set_left_direction_x(float v) const { data()->left_direction_x = v; }
  uint32_t left_direction_x_bits() const { return std::bit_cast<uint32_t>(data()->left_direction_x); }
  void set_left_direction_x_bits(uint32_t v) const { data()->left_direction_x = std::bit_cast<float>(v); }
  uint32_t left_direction_x_pair_address() const { return address + 96; }
  uint64_t left_direction_x_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 96, 8)); }
  void set_left_direction_x_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 96, 8, v); }
  uint64_t left_direction_x_pair() const { return uint64_t(left_direction_x_pair_bits()); }
  void set_left_direction_x_pair(uint64_t v) const { set_left_direction_x_pair_bits(uint64_t(v)); }
  uint32_t left_direction_y_address() const { return address + 100; }
  float left_direction_y() const { return data()->left_direction_y; }
  void set_left_direction_y(float v) const { data()->left_direction_y = v; }
  uint32_t left_direction_y_bits() const { return std::bit_cast<uint32_t>(data()->left_direction_y); }
  void set_left_direction_y_bits(uint32_t v) const { data()->left_direction_y = std::bit_cast<float>(v); }
  uint32_t left_direction_z_address() const { return address + 104; }
  float left_direction_z() const { return data()->left_direction_z; }
  void set_left_direction_z(float v) const { data()->left_direction_z = v; }
  uint32_t left_direction_z_bits() const { return std::bit_cast<uint32_t>(data()->left_direction_z); }
  void set_left_direction_z_bits(uint32_t v) const { data()->left_direction_z = std::bit_cast<float>(v); }
  uint32_t left_direction_z_pair_address() const { return address + 104; }
  uint64_t left_direction_z_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 104, 8)); }
  void set_left_direction_z_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 104, 8, v); }
  uint64_t left_direction_z_pair() const { return uint64_t(left_direction_z_pair_bits()); }
  void set_left_direction_z_pair(uint64_t v) const { set_left_direction_z_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t left_direction_w_address() const { return address + 108; }
  float left_direction_w() const { return data()->left_direction_w; }
  void set_left_direction_w(float v) const { data()->left_direction_w = v; }
  uint32_t left_direction_w_bits() const { return std::bit_cast<uint32_t>(data()->left_direction_w); }
  void set_left_direction_w_bits(uint32_t v) const { data()->left_direction_w = std::bit_cast<float>(v); }
  // the right stick's, as the left's
  uint32_t right_direction_x_address() const { return address + 112; }
  float right_direction_x() const { return data()->right_direction_x; }
  void set_right_direction_x(float v) const { data()->right_direction_x = v; }
  uint32_t right_direction_x_bits() const { return std::bit_cast<uint32_t>(data()->right_direction_x); }
  void set_right_direction_x_bits(uint32_t v) const { data()->right_direction_x = std::bit_cast<float>(v); }
  uint32_t right_direction_x_pair_address() const { return address + 112; }
  uint64_t right_direction_x_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 112, 8)); }
  void set_right_direction_x_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 112, 8, v); }
  uint64_t right_direction_x_pair() const { return uint64_t(right_direction_x_pair_bits()); }
  void set_right_direction_x_pair(uint64_t v) const { set_right_direction_x_pair_bits(uint64_t(v)); }
  uint32_t right_direction_y_address() const { return address + 116; }
  float right_direction_y() const { return data()->right_direction_y; }
  void set_right_direction_y(float v) const { data()->right_direction_y = v; }
  uint32_t right_direction_y_bits() const { return std::bit_cast<uint32_t>(data()->right_direction_y); }
  void set_right_direction_y_bits(uint32_t v) const { data()->right_direction_y = std::bit_cast<float>(v); }
  uint32_t right_direction_z_address() const { return address + 120; }
  float right_direction_z() const { return data()->right_direction_z; }
  void set_right_direction_z(float v) const { data()->right_direction_z = v; }
  uint32_t right_direction_z_bits() const { return std::bit_cast<uint32_t>(data()->right_direction_z); }
  void set_right_direction_z_bits(uint32_t v) const { data()->right_direction_z = std::bit_cast<float>(v); }
  uint32_t right_direction_z_pair_address() const { return address + 120; }
  uint64_t right_direction_z_pair_bits() const { return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 120, 8)); }
  void set_right_direction_z_pair_bits(uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputProcessorLayout, 120, 8, v); }
  uint64_t right_direction_z_pair() const { return uint64_t(right_direction_z_pair_bits()); }
  void set_right_direction_z_pair(uint64_t v) const { set_right_direction_z_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t right_direction_w_address() const { return address + 124; }
  float right_direction_w() const { return data()->right_direction_w; }
  void set_right_direction_w(float v) const { data()->right_direction_w = v; }
  uint32_t right_direction_w_bits() const { return std::bit_cast<uint32_t>(data()->right_direction_w); }
  void set_right_direction_w_bits(uint32_t v) const { data()->right_direction_w = std::bit_cast<float>(v); }
  uint32_t left_length_address() const { return address + 128; }
  float left_length() const { return data()->left_length; }
  void set_left_length(float v) const { data()->left_length = v; }
  uint32_t left_length_bits() const { return std::bit_cast<uint32_t>(data()->left_length); }
  void set_left_length_bits(uint32_t v) const { data()->left_length = std::bit_cast<float>(v); }
  uint32_t right_length_address() const { return address + 132; }
  float right_length() const { return data()->right_length; }
  void set_right_length(float v) const { data()->right_length = v; }
  uint32_t right_length_bits() const { return std::bit_cast<uint32_t>(data()->right_length); }
  void set_right_length_bits(uint32_t v) const { data()->right_length = std::bit_cast<float>(v); }
  // the pad's values 8-39 (32 floats, to +263)
  uint32_t buttons_address() const { return address + 136; }
  float buttons() const { return data()->buttons; }
  void set_buttons(float v) const { data()->buttons = v; }
  uint32_t buttons_bits() const { return std::bit_cast<uint32_t>(data()->buttons); }
  void set_buttons_bits(uint32_t v) const { data()->buttons = std::bit_cast<float>(v); }
  // bits newly set this tick
  uint32_t pressed_address() const { return address + 264; }
  uint32_t pressed() const { return data()->pressed; }
  void set_pressed(uint32_t v) const { data()->pressed = v; }
  uint32_t pressed_bits() const { return data()->pressed; }
  void set_pressed_bits(uint32_t v) const { data()->pressed = v; }
  // bits newly clear this tick
  uint32_t released_address() const { return address + 268; }
  uint32_t released() const { return data()->released; }
  void set_released(uint32_t v) const { data()->released = v; }
  uint32_t released_bits() const { return data()->released; }
  void set_released_bits(uint32_t v) const { data()->released = v; }
  // bit i: buttons[i] above 0.5
  uint32_t held_address() const { return address + 272; }
  uint32_t held() const { return data()->held; }
  void set_held(uint32_t v) const { data()->held = v; }
  uint32_t held_bits() const { return data()->held; }
  void set_held_bits(uint32_t v) const { data()->held = v; }
  // 20 at construction
  uint32_t repeat_delay_address() const { return address + 276; }
  int32_t repeat_delay() const { return data()->repeat_delay; }
  void set_repeat_delay(int32_t v) const { data()->repeat_delay = v; }
  uint32_t repeat_delay_bits() const { return std::bit_cast<uint32_t>(data()->repeat_delay); }
  void set_repeat_delay_bits(uint32_t v) const { data()->repeat_delay = std::bit_cast<int32_t>(v); }
  // 4 at construction
  uint32_t repeat_period_address() const { return address + 280; }
  int32_t repeat_period() const { return data()->repeat_period; }
  void set_repeat_period(int32_t v) const { data()->repeat_period = v; }
  uint32_t repeat_period_bits() const { return std::bit_cast<uint32_t>(data()->repeat_period); }
  void set_repeat_period_bits(uint32_t v) const { data()->repeat_period = std::bit_cast<int32_t>(v); }
  // 32 counters (s32): the ticks each bit has been held
  uint32_t held_ticks_address() const { return address + 284; }
  uint8_t* held_ticks() const { return reinterpret_cast<uint8_t*>(data()) + 284; }
};

// image; measure: Input.+1840
struct InputProcessorVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0: sub_8258CF40 (the base's sub_825925A8)
  uint32_t destroy() const { return guest::LoadU32(base, address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: InputProcessor::Update sub_82592188
  uint32_t update() const { return guest::LoadU32(base, address + 4); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t update_address() const { return address + 4; }
};

// embedded; measure: InputManager::GetProcessor.r4, InputManager::GetLogicalPad.r4, InputManager::GetPhysicalPad.r4, InputManager::AddListener.r4, LogicalPad::Bind.r4
struct InputIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: InputProcessor::TestPressed.r4, InputProcessor::TestRepeat.r4, InputProcessor::AnyPressed.r4, InputProcessor::PressedOrRepeat.r4
struct InputButtonMask : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t mask() const { return guest::LoadU32(base, address + 0); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t mask_address() const { return address + 0; }
};

// embedded; measure: PtrList::Insert.r5
struct PtrListInsertValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// allocated; measure: new<?>@PtrList::AllocateNodes
#pragma pack(push, 1)
struct PtrListNodeData {
  uint32_t next = 0;  // +0
  uint32_t previous = 0;  // +4
  uint32_t value = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(PtrListNodeData) == 12);
inline constexpr memory::LayoutField kPtrListNodeFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kPtrListNodeLayout = {kPtrListNodeFields, 3};

struct PtrListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  PtrListNodeData* data() const { return static_cast<PtrListNodeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static PtrListNode New(uint8_t* base) {
    return PtrListNode(base, memory::MakeHandle(new PtrListNodeData(), sizeof(PtrListNodeData), "PtrListNode", &kPtrListNodeLayout));
  }
  template <uint32_t Stride = kSize>
  static PtrListNode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(PtrListNodeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PtrListNodeData*>(::operator new(sizeof(PtrListNodeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PtrListNodeData();
    return PtrListNode(base, memory::MakeHandle(p, sizeof(PtrListNodeData) * count, "PtrListNode", &kPtrListNodeLayout, sizeof(PtrListNodeData)));
  }
  PtrListNode at(uint32_t i) const { return PtrListNode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kPtrListNodeLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kPtrListNodeLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = PtrListNode>
  T next() const { return T(base, on_host() ? data()->next : guest::LoadU32(base, address + 0)); }
  void set_next(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->next = v; else guest::StoreU32(base, address + 0, v); }
  void set_next(uint32_t v) const { if (on_host()) data()->next = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t next_bits() const { return on_host() ? data()->next : guest::LoadU32(base, address + 0); }
  void set_next_bits(uint32_t v) const { if (on_host()) data()->next = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t previous_address() const { return address + 4; }
  template <typename T = PtrListNode>
  T previous() const { return T(base, on_host() ? data()->previous : guest::LoadU32(base, address + 4)); }
  void set_previous(const View& w) const { const uint32_t v = w.address; if (on_host()) data()->previous = v; else guest::StoreU32(base, address + 4, v); }
  void set_previous(uint32_t v) const { if (on_host()) data()->previous = v; else guest::StoreU32(base, address + 4, v); }
  uint32_t previous_bits() const { return on_host() ? data()->previous : guest::LoadU32(base, address + 4); }
  void set_previous_bits(uint32_t v) const { if (on_host()) data()->previous = v; else guest::StoreU32(base, address + 4, v); }
  // a listener (an InputProcessor) in the input manager's lists
  uint32_t value_address() const { return address + 8; }
  uint32_t value() const { return on_host() ? uint32_t(data()->value) : uint32_t(guest::LoadU32(base, address + 8)); }
  void set_value(uint32_t v) const { if (on_host()) data()->value = v; else guest::StoreU32(base, address + 8, v); }
  uint32_t value_bits() const { return on_host() ? data()->value : guest::LoadU32(base, address + 8); }
  void set_value_bits(uint32_t v) const { if (on_host()) data()->value = v; else guest::StoreU32(base, address + 8, v); }
};

}  // namespace schema

#include "schema/action_mappers.h"
#include "schema/app_states.h"
#include "schema/model_animation.h"
