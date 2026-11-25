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
  static InputManager New() {
    return InputManager(memory::MakeHandle(new InputManagerData(), sizeof(InputManagerData), "InputManager", &kInputManagerLayout));
  }
  template <uint32_t Stride = kSize>
  static InputManager NewArray(uint32_t count) {
    static_assert(sizeof(InputManagerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InputManagerData*>(::operator new(sizeof(InputManagerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InputManagerData();
    return InputManager(memory::MakeHandle(p, sizeof(InputManagerData) * count, "InputManager", &kInputManagerLayout, sizeof(InputManagerData)));
  }
  InputManager at(uint32_t i) const { return InputManager(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  InputManagerData* direct() const { return static_cast<InputManagerData*>(memory::HandleOwn(address, &kInputManagerLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kInputManagerLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputManagerLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82000F48: 0 sub_82098FF8, 1 GetProcessor, 2 GetLogicalPad, 3 AddListener, 4 sub_82099048, 5 GetPhysicalPad
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the set-up block's +4: a pointer to the framework's +16
  uint32_t framework_part_address() const { return address + 4; }
  uint32_t framework_part_bits() const { if (auto* d = direct()) return d->framework_part; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_framework_part_bits(uint32_t v) const { if (auto* d = direct()) { d->framework_part = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t framework_part() const { if (auto* d = direct()) return d->framework_part; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_framework_part(uint32_t v) const { if (auto* d = direct()) { d->framework_part = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // five LogicalPads (196 bytes): 0-3 the players', 4 the any pad (the embedded logical_pad)
  uint32_t logical_pads_address() const { return address + 8; }
  uint8_t* logical_pads() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 8 : guest::Pointer(address + 8); }
  template <typename T = LogicalPad>
  T logical_pad(uint32_t i) const { return T(address + 8 + i * T::kSize); }
  // four physical pads (212 bytes, model-animation.toml's PhysicalPad; the embedded physical_pad), then 4 bytes not used
  uint32_t physical_pads_address() const { return address + 988; }
  uint8_t* physical_pads() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 988 : guest::Pointer(address + 988); }
  template <typename T = PhysicalPad>
  T physical_pad(uint32_t i) const { return T(address + 988 + i * T::kSize); }
  // five InputProcessors (416 bytes; the embedded processor): Input::Update reads processor 0's vtable here
  uint32_t processors_address() const { return address + 1840; }
  uint8_t* processors() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1840 : guest::Pointer(address + 1840); }
  template <typename T = InputProcessor>
  T processor(uint32_t i) const { return T(address + 1840 + i * T::kSize); }
  // five PtrLists (12 bytes) of listeners, one per logical pad (the embedded listener_list): Input::Update reads list 0's head and size at +3924 and +3928
  uint32_t listener_lists_address() const { return address + 3920; }
  uint8_t* listener_lists() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 3920 : guest::Pointer(address + 3920); }
  template <typename T = PtrList>
  T listener_list(uint32_t i) const { return T(address + 3920 + i * T::kSize); }
  // not set by the constructor
  uint32_t unknown_3980_address() const { return address + 3980; }
  uint32_t unknown_3980_bits() const { if (auto* d = direct()) return d->unknown_3980; return uint32_t((memory::IsHandle(address + 3980) ? uint32_t(memory::OwnerLoad(address + 3980, 4)) : guest::LoadU32(address + 3980))); }
  void set_unknown_3980_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_3980 = v; return; } if (memory::IsHandle(address + 3980)) memory::OwnerStore(address + 3980, 4, v); else guest::StoreU32(address + 3980, v); }
  uint32_t unknown_3980() const { if (auto* d = direct()) return d->unknown_3980; return uint32_t((memory::IsHandle(address + 3980) ? uint32_t(memory::OwnerLoad(address + 3980, 4)) : guest::LoadU32(address + 3980))); }
  void set_unknown_3980(uint32_t v) const { if (auto* d = direct()) { d->unknown_3980 = v; return; } if (memory::IsHandle(address + 3980)) memory::OwnerStore(address + 3980, 4, v); else guest::StoreU32(address + 3980, v); }
};

// embedded; measure: InputManager::InputManager.r4
struct InputManagerArgs : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the message dispatcher
  template <typename T = InputDispatcher>
  T dispatcher() const { return T(guest::LoadU32(address + 0)); }
  void set_dispatcher(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_dispatcher(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t dispatcher_address() const { return address + 0; }
  // a pointer to the framework's +16, kept at the manager's +4
  uint32_t framework_part() const { return guest::LoadU32(address + 4); }
  void set_framework_part(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t framework_part_address() const { return address + 4; }
};

// embedded; measure: InputManager::InputManager.r4.+0
struct InputDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = InputDispatcherVtable>
  T vtable() const { return T(guest::LoadU32(address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the vtable's slot +4 (InputDispatcherVtable), read through r11 after the vtable's load
  uint32_t fault_vtable_4() const { return guest::LoadU32(address + 4); }
  void set_fault_vtable_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t fault_vtable_4_address() const { return address + 4; }
  // measure fault: the vtable's slot +8, as fault_vtable_4
  uint32_t fault_vtable_8() const { return guest::LoadU32(address + 8); }
  void set_fault_vtable_8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t fault_vtable_8_address() const { return address + 8; }
};

// image; measure: -
struct InputDispatcherVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // slot +4: registers message 7's handler (sub_82098CE8)
  uint32_t register_7() const { return guest::LoadU32(address + 4); }
  void set_register_7(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t register_7_address() const { return address + 4; }
  // slot +8: registers message 5's handler (Input::Update sub_820990F0)
  uint32_t register_5() const { return guest::LoadU32(address + 8); }
  void set_register_5(uint32_t v) const { guest::StoreU32(address + 8, v); }
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
  static LogicalPad New() {
    return LogicalPad(memory::MakeHandle(new LogicalPadData(), sizeof(LogicalPadData), "LogicalPad", &kLogicalPadLayout));
  }
  template <uint32_t Stride = kSize>
  static LogicalPad NewArray(uint32_t count) {
    static_assert(sizeof(LogicalPadData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<LogicalPadData*>(::operator new(sizeof(LogicalPadData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) LogicalPadData();
    return LogicalPad(memory::MakeHandle(p, sizeof(LogicalPadData) * count, "LogicalPad", &kLogicalPadLayout, sizeof(LogicalPadData)));
  }
  LogicalPad at(uint32_t i) const { return LogicalPad(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  LogicalPadData* direct() const { return static_cast<LogicalPadData*>(memory::HandleOwn(address, &kLogicalPadLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kLogicalPadLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kLogicalPadLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x8206CBEC (InputProcessor::Update's r4: any device, a logical pad in play)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = InputDeviceVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // four InputPadBindings (8 bytes; the embedded binding), cleared word by word by the constructor
  uint32_t bindings_address() const { return address + 4; }
  uint8_t* bindings() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4 : guest::Pointer(address + 4); }
  template <typename T = InputPadBinding>
  T binding(uint32_t i) const { return T(address + 4 + i * T::kSize); }
  // the 40 values (to +195): 0-7 the sticks and triggers (LX, LY, LT, 0, RX, RY, RT, 0), 8-39 the buttons (action-mappers.toml's ActionPadValues, what the mappers read)
  template <typename T = ActionPadValues>
  T values() const { return T(address + 36); }
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
  static InputPadBinding New() {
    return InputPadBinding(memory::MakeHandle(new InputPadBindingData(), sizeof(InputPadBindingData), "InputPadBinding", &kInputPadBindingLayout));
  }
  template <uint32_t Stride = kSize>
  static InputPadBinding NewArray(uint32_t count) {
    static_assert(sizeof(InputPadBindingData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InputPadBindingData*>(::operator new(sizeof(InputPadBindingData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InputPadBindingData();
    return InputPadBinding(memory::MakeHandle(p, sizeof(InputPadBindingData) * count, "InputPadBinding", &kInputPadBindingLayout, sizeof(InputPadBindingData)));
  }
  InputPadBinding at(uint32_t i) const { return InputPadBinding(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  InputPadBindingData* direct() const { return static_cast<InputPadBindingData*>(memory::HandleOwn(address, &kInputPadBindingLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kInputPadBindingLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputPadBindingLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t enabled_address() const { return address + 0; }
  uint8_t enabled_bits() const { if (auto* d = direct()) return d->enabled; return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(address + 0))); }
  void set_enabled_bits(uint8_t v) const { if (auto* d = direct()) { d->enabled = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(address + 0, v); }
  uint8_t enabled() const { if (auto* d = direct()) return d->enabled; return uint8_t((memory::IsHandle(address + 0) ? uint8_t(memory::OwnerLoad(address + 0, 1)) : guest::LoadU8(address + 0))); }
  void set_enabled(uint8_t v) const { if (auto* d = direct()) { d->enabled = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 1, v); else guest::StoreU8(address + 0, v); }
  // a physical pad, or for the any pad a logical pad 0-3
  uint32_t device_address() const { return address + 4; }
  uint32_t device_bits() const { if (auto* d = direct()) return d->device; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_device_bits(uint32_t v) const { if (auto* d = direct()) { d->device = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t device() const { if (auto* d = direct()) return d->device; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_device(uint32_t v) const { if (auto* d = direct()) { d->device = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
};

// image; measure: InputProcessor::Update.r4.+0
struct InputDeviceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0, probably the destructor (sub_82591898, sub_82592058)
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: the values' address (LogicalPad::Values sub_82591B30, PhysicalPad::Values sub_820988B8)
  uint32_t values() const { return guest::LoadU32(address + 4); }
  void set_values(uint32_t v) const { guest::StoreU32(address + 4, v); }
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
  static InputProcessor New() {
    return InputProcessor(memory::MakeHandle(new InputProcessorData(), sizeof(InputProcessorData), "InputProcessor", &kInputProcessorLayout));
  }
  template <uint32_t Stride = kSize>
  static InputProcessor NewArray(uint32_t count) {
    static_assert(sizeof(InputProcessorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InputProcessorData*>(::operator new(sizeof(InputProcessorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InputProcessorData();
    return InputProcessor(memory::MakeHandle(p, sizeof(InputProcessorData) * count, "InputProcessor", &kInputProcessorLayout, sizeof(InputProcessorData)));
  }
  InputProcessor at(uint32_t i) const { return InputProcessor(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  InputProcessorData* direct() const { return static_cast<InputProcessorData*>(memory::HandleOwn(address, &kInputProcessorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kInputProcessorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x8206C0C4 (the base's 0x8206CC00 first)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = InputProcessorVtable>
  T vtable() const { return T(vtable_bits()); }
  void set_vtable(const View& w) const { set_vtable_bits(w.address); }
  void set_vtable(uint32_t v) const { set_vtable_bits(v); }
  // 0.5 at construction; the four dead zones apply to both sticks
  uint32_t dead_zone_x_address() const { return address + 16; }
  uint32_t dead_zone_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->dead_zone_x); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_dead_zone_x_bits(uint32_t v) const { if (auto* d = direct()) { d->dead_zone_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float dead_zone_x() const { return std::bit_cast<float>(dead_zone_x_bits()); }
  void set_dead_zone_x(float v) const { set_dead_zone_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t dead_zone_x_pair_address() const { return address + 16; }
  uint64_t dead_zone_x_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 16, 8)); return uint64_t((memory::IsHandle(address + 16) ? uint64_t(memory::OwnerLoad(address + 16, 8)) : guest::LoadU64(address + 16))); }
  void set_dead_zone_x_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 16, 8, v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 8, v); else guest::StoreU64(address + 16, v); }
  uint64_t dead_zone_x_pair() const { return uint64_t(dead_zone_x_pair_bits()); }
  void set_dead_zone_x_pair(uint64_t v) const { set_dead_zone_x_pair_bits(uint64_t(v)); }
  // 0.5
  uint32_t dead_zone_y_address() const { return address + 20; }
  uint32_t dead_zone_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->dead_zone_y); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_dead_zone_y_bits(uint32_t v) const { if (auto* d = direct()) { d->dead_zone_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float dead_zone_y() const { return std::bit_cast<float>(dead_zone_y_bits()); }
  void set_dead_zone_y(float v) const { set_dead_zone_y_bits(std::bit_cast<uint32_t>(v)); }
  // 0.0
  uint32_t dead_zone_trigger_address() const { return address + 24; }
  uint32_t dead_zone_trigger_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->dead_zone_trigger); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_dead_zone_trigger_bits(uint32_t v) const { if (auto* d = direct()) { d->dead_zone_trigger = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float dead_zone_trigger() const { return std::bit_cast<float>(dead_zone_trigger_bits()); }
  void set_dead_zone_trigger(float v) const { set_dead_zone_trigger_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t dead_zone_trigger_pair_address() const { return address + 24; }
  uint64_t dead_zone_trigger_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 24, 8)); return uint64_t((memory::IsHandle(address + 24) ? uint64_t(memory::OwnerLoad(address + 24, 8)) : guest::LoadU64(address + 24))); }
  void set_dead_zone_trigger_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 24, 8, v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 8, v); else guest::StoreU64(address + 24, v); }
  uint64_t dead_zone_trigger_pair() const { return uint64_t(dead_zone_trigger_pair_bits()); }
  void set_dead_zone_trigger_pair(uint64_t v) const { set_dead_zone_trigger_pair_bits(uint64_t(v)); }
  // 0.0
  uint32_t dead_zone_unused_address() const { return address + 28; }
  uint32_t dead_zone_unused_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->dead_zone_unused); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_dead_zone_unused_bits(uint32_t v) const { if (auto* d = direct()) { d->dead_zone_unused = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float dead_zone_unused() const { return std::bit_cast<float>(dead_zone_unused_bits()); }
  void set_dead_zone_unused(float v) const { set_dead_zone_unused_bits(std::bit_cast<uint32_t>(v)); }
  // the pad's values 0-7 (to +63)
  uint32_t raw_lx_address() const { return address + 32; }
  uint32_t raw_lx_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_lx); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_raw_lx_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_lx = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  float raw_lx() const { return std::bit_cast<float>(raw_lx_bits()); }
  void set_raw_lx(float v) const { set_raw_lx_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t raw_lx_pair_address() const { return address + 32; }
  uint64_t raw_lx_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 32, 8)); return uint64_t((memory::IsHandle(address + 32) ? uint64_t(memory::OwnerLoad(address + 32, 8)) : guest::LoadU64(address + 32))); }
  void set_raw_lx_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 32, 8, v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 8, v); else guest::StoreU64(address + 32, v); }
  uint64_t raw_lx_pair() const { return uint64_t(raw_lx_pair_bits()); }
  void set_raw_lx_pair(uint64_t v) const { set_raw_lx_pair_bits(uint64_t(v)); }
  uint32_t raw_ly_address() const { return address + 36; }
  uint32_t raw_ly_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_ly); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_raw_ly_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_ly = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  float raw_ly() const { return std::bit_cast<float>(raw_ly_bits()); }
  void set_raw_ly(float v) const { set_raw_ly_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t raw_lt_address() const { return address + 40; }
  uint32_t raw_lt_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_lt); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_raw_lt_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_lt = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  float raw_lt() const { return std::bit_cast<float>(raw_lt_bits()); }
  void set_raw_lt(float v) const { set_raw_lt_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t raw_lt_pair_address() const { return address + 40; }
  uint64_t raw_lt_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 40, 8)); return uint64_t((memory::IsHandle(address + 40) ? uint64_t(memory::OwnerLoad(address + 40, 8)) : guest::LoadU64(address + 40))); }
  void set_raw_lt_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 40, 8, v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 8, v); else guest::StoreU64(address + 40, v); }
  uint64_t raw_lt_pair() const { return uint64_t(raw_lt_pair_bits()); }
  void set_raw_lt_pair(uint64_t v) const { set_raw_lt_pair_bits(uint64_t(v)); }
  // 1.0 at construction (a vector's w)
  uint32_t raw_3_address() const { return address + 44; }
  uint32_t raw_3_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_3); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_raw_3_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_3 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  float raw_3() const { return std::bit_cast<float>(raw_3_bits()); }
  void set_raw_3(float v) const { set_raw_3_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t raw_rx_address() const { return address + 48; }
  uint32_t raw_rx_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_rx); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_raw_rx_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_rx = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float raw_rx() const { return std::bit_cast<float>(raw_rx_bits()); }
  void set_raw_rx(float v) const { set_raw_rx_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t raw_rx_pair_address() const { return address + 48; }
  uint64_t raw_rx_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 48, 8)); return uint64_t((memory::IsHandle(address + 48) ? uint64_t(memory::OwnerLoad(address + 48, 8)) : guest::LoadU64(address + 48))); }
  void set_raw_rx_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 48, 8, v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 8, v); else guest::StoreU64(address + 48, v); }
  uint64_t raw_rx_pair() const { return uint64_t(raw_rx_pair_bits()); }
  void set_raw_rx_pair(uint64_t v) const { set_raw_rx_pair_bits(uint64_t(v)); }
  uint32_t raw_ry_address() const { return address + 52; }
  uint32_t raw_ry_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_ry); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_raw_ry_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_ry = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  float raw_ry() const { return std::bit_cast<float>(raw_ry_bits()); }
  void set_raw_ry(float v) const { set_raw_ry_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t raw_rt_address() const { return address + 56; }
  uint32_t raw_rt_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_rt); return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_raw_rt_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_rt = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  float raw_rt() const { return std::bit_cast<float>(raw_rt_bits()); }
  void set_raw_rt(float v) const { set_raw_rt_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t raw_rt_pair_address() const { return address + 56; }
  uint64_t raw_rt_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 56, 8)); return uint64_t((memory::IsHandle(address + 56) ? uint64_t(memory::OwnerLoad(address + 56, 8)) : guest::LoadU64(address + 56))); }
  void set_raw_rt_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 56, 8, v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 8, v); else guest::StoreU64(address + 56, v); }
  uint64_t raw_rt_pair() const { return uint64_t(raw_rt_pair_bits()); }
  void set_raw_rt_pair(uint64_t v) const { set_raw_rt_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t raw_7_address() const { return address + 60; }
  uint32_t raw_7_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->raw_7); return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_raw_7_bits(uint32_t v) const { if (auto* d = direct()) { d->raw_7 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  float raw_7() const { return std::bit_cast<float>(raw_7_bits()); }
  void set_raw_7(float v) const { set_raw_7_bits(std::bit_cast<uint32_t>(v)); }
  // the raw values with the dead zone taken out (to +95)
  uint32_t adjusted_lx_address() const { return address + 64; }
  uint32_t adjusted_lx_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_lx); return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_adjusted_lx_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_lx = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  float adjusted_lx() const { return std::bit_cast<float>(adjusted_lx_bits()); }
  void set_adjusted_lx(float v) const { set_adjusted_lx_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t adjusted_lx_pair_address() const { return address + 64; }
  uint64_t adjusted_lx_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 64, 8)); return uint64_t((memory::IsHandle(address + 64) ? uint64_t(memory::OwnerLoad(address + 64, 8)) : guest::LoadU64(address + 64))); }
  void set_adjusted_lx_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 64, 8, v); return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 8, v); else guest::StoreU64(address + 64, v); }
  uint64_t adjusted_lx_pair() const { return uint64_t(adjusted_lx_pair_bits()); }
  void set_adjusted_lx_pair(uint64_t v) const { set_adjusted_lx_pair_bits(uint64_t(v)); }
  uint32_t adjusted_ly_address() const { return address + 68; }
  uint32_t adjusted_ly_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_ly); return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_adjusted_ly_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_ly = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  float adjusted_ly() const { return std::bit_cast<float>(adjusted_ly_bits()); }
  void set_adjusted_ly(float v) const { set_adjusted_ly_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t adjusted_lt_address() const { return address + 72; }
  uint32_t adjusted_lt_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_lt); return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_adjusted_lt_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_lt = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  float adjusted_lt() const { return std::bit_cast<float>(adjusted_lt_bits()); }
  void set_adjusted_lt(float v) const { set_adjusted_lt_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t adjusted_lt_pair_address() const { return address + 72; }
  uint64_t adjusted_lt_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 72, 8)); return uint64_t((memory::IsHandle(address + 72) ? uint64_t(memory::OwnerLoad(address + 72, 8)) : guest::LoadU64(address + 72))); }
  void set_adjusted_lt_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 72, 8, v); return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 8, v); else guest::StoreU64(address + 72, v); }
  uint64_t adjusted_lt_pair() const { return uint64_t(adjusted_lt_pair_bits()); }
  void set_adjusted_lt_pair(uint64_t v) const { set_adjusted_lt_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t adjusted_3_address() const { return address + 76; }
  uint32_t adjusted_3_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_3); return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_adjusted_3_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_3 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  float adjusted_3() const { return std::bit_cast<float>(adjusted_3_bits()); }
  void set_adjusted_3(float v) const { set_adjusted_3_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t adjusted_rx_address() const { return address + 80; }
  uint32_t adjusted_rx_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_rx); return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_adjusted_rx_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_rx = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  float adjusted_rx() const { return std::bit_cast<float>(adjusted_rx_bits()); }
  void set_adjusted_rx(float v) const { set_adjusted_rx_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t adjusted_rx_pair_address() const { return address + 80; }
  uint64_t adjusted_rx_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 80, 8)); return uint64_t((memory::IsHandle(address + 80) ? uint64_t(memory::OwnerLoad(address + 80, 8)) : guest::LoadU64(address + 80))); }
  void set_adjusted_rx_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 80, 8, v); return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 8, v); else guest::StoreU64(address + 80, v); }
  uint64_t adjusted_rx_pair() const { return uint64_t(adjusted_rx_pair_bits()); }
  void set_adjusted_rx_pair(uint64_t v) const { set_adjusted_rx_pair_bits(uint64_t(v)); }
  uint32_t adjusted_ry_address() const { return address + 84; }
  uint32_t adjusted_ry_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_ry); return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_adjusted_ry_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_ry = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  float adjusted_ry() const { return std::bit_cast<float>(adjusted_ry_bits()); }
  void set_adjusted_ry(float v) const { set_adjusted_ry_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t adjusted_rt_address() const { return address + 88; }
  uint32_t adjusted_rt_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_rt); return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_adjusted_rt_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_rt = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  float adjusted_rt() const { return std::bit_cast<float>(adjusted_rt_bits()); }
  void set_adjusted_rt(float v) const { set_adjusted_rt_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t adjusted_rt_pair_address() const { return address + 88; }
  uint64_t adjusted_rt_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 88, 8)); return uint64_t((memory::IsHandle(address + 88) ? uint64_t(memory::OwnerLoad(address + 88, 8)) : guest::LoadU64(address + 88))); }
  void set_adjusted_rt_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 88, 8, v); return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 8, v); else guest::StoreU64(address + 88, v); }
  uint64_t adjusted_rt_pair() const { return uint64_t(adjusted_rt_pair_bits()); }
  void set_adjusted_rt_pair(uint64_t v) const { set_adjusted_rt_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t adjusted_7_address() const { return address + 92; }
  uint32_t adjusted_7_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->adjusted_7); return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(address + 92))); }
  void set_adjusted_7_bits(uint32_t v) const { if (auto* d = direct()) { d->adjusted_7 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(address + 92, v); }
  float adjusted_7() const { return std::bit_cast<float>(adjusted_7_bits()); }
  void set_adjusted_7(float v) const { set_adjusted_7_bits(std::bit_cast<uint32_t>(v)); }
  // the left stick's (x, y) normalized, or 0 (stvx: 16 bytes)
  uint32_t left_direction_x_address() const { return address + 96; }
  uint32_t left_direction_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->left_direction_x); return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(address + 96))); }
  void set_left_direction_x_bits(uint32_t v) const { if (auto* d = direct()) { d->left_direction_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(address + 96, v); }
  float left_direction_x() const { return std::bit_cast<float>(left_direction_x_bits()); }
  void set_left_direction_x(float v) const { set_left_direction_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t left_direction_x_pair_address() const { return address + 96; }
  uint64_t left_direction_x_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 96, 8)); return uint64_t((memory::IsHandle(address + 96) ? uint64_t(memory::OwnerLoad(address + 96, 8)) : guest::LoadU64(address + 96))); }
  void set_left_direction_x_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 96, 8, v); return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 8, v); else guest::StoreU64(address + 96, v); }
  uint64_t left_direction_x_pair() const { return uint64_t(left_direction_x_pair_bits()); }
  void set_left_direction_x_pair(uint64_t v) const { set_left_direction_x_pair_bits(uint64_t(v)); }
  uint32_t left_direction_y_address() const { return address + 100; }
  uint32_t left_direction_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->left_direction_y); return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(address + 100))); }
  void set_left_direction_y_bits(uint32_t v) const { if (auto* d = direct()) { d->left_direction_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(address + 100, v); }
  float left_direction_y() const { return std::bit_cast<float>(left_direction_y_bits()); }
  void set_left_direction_y(float v) const { set_left_direction_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t left_direction_z_address() const { return address + 104; }
  uint32_t left_direction_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->left_direction_z); return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(address + 104))); }
  void set_left_direction_z_bits(uint32_t v) const { if (auto* d = direct()) { d->left_direction_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(address + 104, v); }
  float left_direction_z() const { return std::bit_cast<float>(left_direction_z_bits()); }
  void set_left_direction_z(float v) const { set_left_direction_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t left_direction_z_pair_address() const { return address + 104; }
  uint64_t left_direction_z_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 104, 8)); return uint64_t((memory::IsHandle(address + 104) ? uint64_t(memory::OwnerLoad(address + 104, 8)) : guest::LoadU64(address + 104))); }
  void set_left_direction_z_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 104, 8, v); return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 8, v); else guest::StoreU64(address + 104, v); }
  uint64_t left_direction_z_pair() const { return uint64_t(left_direction_z_pair_bits()); }
  void set_left_direction_z_pair(uint64_t v) const { set_left_direction_z_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t left_direction_w_address() const { return address + 108; }
  uint32_t left_direction_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->left_direction_w); return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(address + 108))); }
  void set_left_direction_w_bits(uint32_t v) const { if (auto* d = direct()) { d->left_direction_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(address + 108, v); }
  float left_direction_w() const { return std::bit_cast<float>(left_direction_w_bits()); }
  void set_left_direction_w(float v) const { set_left_direction_w_bits(std::bit_cast<uint32_t>(v)); }
  // the right stick's, as the left's
  uint32_t right_direction_x_address() const { return address + 112; }
  uint32_t right_direction_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->right_direction_x); return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(address + 112))); }
  void set_right_direction_x_bits(uint32_t v) const { if (auto* d = direct()) { d->right_direction_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(address + 112, v); }
  float right_direction_x() const { return std::bit_cast<float>(right_direction_x_bits()); }
  void set_right_direction_x(float v) const { set_right_direction_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t right_direction_x_pair_address() const { return address + 112; }
  uint64_t right_direction_x_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 112, 8)); return uint64_t((memory::IsHandle(address + 112) ? uint64_t(memory::OwnerLoad(address + 112, 8)) : guest::LoadU64(address + 112))); }
  void set_right_direction_x_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 112, 8, v); return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 8, v); else guest::StoreU64(address + 112, v); }
  uint64_t right_direction_x_pair() const { return uint64_t(right_direction_x_pair_bits()); }
  void set_right_direction_x_pair(uint64_t v) const { set_right_direction_x_pair_bits(uint64_t(v)); }
  uint32_t right_direction_y_address() const { return address + 116; }
  uint32_t right_direction_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->right_direction_y); return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(address + 116))); }
  void set_right_direction_y_bits(uint32_t v) const { if (auto* d = direct()) { d->right_direction_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(address + 116, v); }
  float right_direction_y() const { return std::bit_cast<float>(right_direction_y_bits()); }
  void set_right_direction_y(float v) const { set_right_direction_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t right_direction_z_address() const { return address + 120; }
  uint32_t right_direction_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->right_direction_z); return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(address + 120))); }
  void set_right_direction_z_bits(uint32_t v) const { if (auto* d = direct()) { d->right_direction_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(address + 120, v); }
  float right_direction_z() const { return std::bit_cast<float>(right_direction_z_bits()); }
  void set_right_direction_z(float v) const { set_right_direction_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t right_direction_z_pair_address() const { return address + 120; }
  uint64_t right_direction_z_pair_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 120, 8)); return uint64_t((memory::IsHandle(address + 120) ? uint64_t(memory::OwnerLoad(address + 120, 8)) : guest::LoadU64(address + 120))); }
  void set_right_direction_z_pair_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kInputProcessorLayout, 120, 8, v); return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 8, v); else guest::StoreU64(address + 120, v); }
  uint64_t right_direction_z_pair() const { return uint64_t(right_direction_z_pair_bits()); }
  void set_right_direction_z_pair(uint64_t v) const { set_right_direction_z_pair_bits(uint64_t(v)); }
  // 1.0 at construction
  uint32_t right_direction_w_address() const { return address + 124; }
  uint32_t right_direction_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->right_direction_w); return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(address + 124))); }
  void set_right_direction_w_bits(uint32_t v) const { if (auto* d = direct()) { d->right_direction_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(address + 124, v); }
  float right_direction_w() const { return std::bit_cast<float>(right_direction_w_bits()); }
  void set_right_direction_w(float v) const { set_right_direction_w_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t left_length_address() const { return address + 128; }
  uint32_t left_length_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->left_length); return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(address + 128))); }
  void set_left_length_bits(uint32_t v) const { if (auto* d = direct()) { d->left_length = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(address + 128, v); }
  float left_length() const { return std::bit_cast<float>(left_length_bits()); }
  void set_left_length(float v) const { set_left_length_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t right_length_address() const { return address + 132; }
  uint32_t right_length_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->right_length); return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(address + 132))); }
  void set_right_length_bits(uint32_t v) const { if (auto* d = direct()) { d->right_length = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(address + 132, v); }
  float right_length() const { return std::bit_cast<float>(right_length_bits()); }
  void set_right_length(float v) const { set_right_length_bits(std::bit_cast<uint32_t>(v)); }
  // the pad's values 8-39 (32 floats, to +263)
  uint32_t buttons_address() const { return address + 136; }
  uint32_t buttons_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->buttons); return uint32_t((memory::IsHandle(address + 136) ? uint32_t(memory::OwnerLoad(address + 136, 4)) : guest::LoadU32(address + 136))); }
  void set_buttons_bits(uint32_t v) const { if (auto* d = direct()) { d->buttons = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 136)) memory::OwnerStore(address + 136, 4, v); else guest::StoreU32(address + 136, v); }
  float buttons() const { return std::bit_cast<float>(buttons_bits()); }
  void set_buttons(float v) const { set_buttons_bits(std::bit_cast<uint32_t>(v)); }
  // bits newly set this tick
  uint32_t pressed_address() const { return address + 264; }
  uint32_t pressed_bits() const { if (auto* d = direct()) return d->pressed; return uint32_t((memory::IsHandle(address + 264) ? uint32_t(memory::OwnerLoad(address + 264, 4)) : guest::LoadU32(address + 264))); }
  void set_pressed_bits(uint32_t v) const { if (auto* d = direct()) { d->pressed = v; return; } if (memory::IsHandle(address + 264)) memory::OwnerStore(address + 264, 4, v); else guest::StoreU32(address + 264, v); }
  uint32_t pressed() const { if (auto* d = direct()) return d->pressed; return uint32_t((memory::IsHandle(address + 264) ? uint32_t(memory::OwnerLoad(address + 264, 4)) : guest::LoadU32(address + 264))); }
  void set_pressed(uint32_t v) const { if (auto* d = direct()) { d->pressed = v; return; } if (memory::IsHandle(address + 264)) memory::OwnerStore(address + 264, 4, v); else guest::StoreU32(address + 264, v); }
  // bits newly clear this tick
  uint32_t released_address() const { return address + 268; }
  uint32_t released_bits() const { if (auto* d = direct()) return d->released; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_released_bits(uint32_t v) const { if (auto* d = direct()) { d->released = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  uint32_t released() const { if (auto* d = direct()) return d->released; return uint32_t((memory::IsHandle(address + 268) ? uint32_t(memory::OwnerLoad(address + 268, 4)) : guest::LoadU32(address + 268))); }
  void set_released(uint32_t v) const { if (auto* d = direct()) { d->released = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 4, v); else guest::StoreU32(address + 268, v); }
  // bit i: buttons[i] above 0.5
  uint32_t held_address() const { return address + 272; }
  uint32_t held_bits() const { if (auto* d = direct()) return d->held; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_held_bits(uint32_t v) const { if (auto* d = direct()) { d->held = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  uint32_t held() const { if (auto* d = direct()) return d->held; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(address + 272))); }
  void set_held(uint32_t v) const { if (auto* d = direct()) { d->held = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(address + 272, v); }
  // 20 at construction
  uint32_t repeat_delay_address() const { return address + 276; }
  uint32_t repeat_delay_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->repeat_delay); return uint32_t((memory::IsHandle(address + 276) ? uint32_t(memory::OwnerLoad(address + 276, 4)) : guest::LoadU32(address + 276))); }
  void set_repeat_delay_bits(uint32_t v) const { if (auto* d = direct()) { d->repeat_delay = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 4, v); else guest::StoreU32(address + 276, v); }
  int32_t repeat_delay() const { return int32_t(repeat_delay_bits()); }
  void set_repeat_delay(int32_t v) const { set_repeat_delay_bits(uint32_t(v)); }
  // 4 at construction
  uint32_t repeat_period_address() const { return address + 280; }
  uint32_t repeat_period_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->repeat_period); return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(address + 280))); }
  void set_repeat_period_bits(uint32_t v) const { if (auto* d = direct()) { d->repeat_period = std::bit_cast<int32_t>(v); return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(address + 280, v); }
  int32_t repeat_period() const { return int32_t(repeat_period_bits()); }
  void set_repeat_period(int32_t v) const { set_repeat_period_bits(uint32_t(v)); }
  // 32 counters (s32): the ticks each bit has been held
  uint32_t held_ticks_address() const { return address + 284; }
  uint8_t* held_ticks() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 284 : guest::Pointer(address + 284); }
};

// image; measure: Input.+1840
struct InputProcessorVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0: sub_8258CF40 (the base's sub_825925A8)
  uint32_t destroy() const { return guest::LoadU32(address + 0); }
  void set_destroy(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destroy_address() const { return address + 0; }
  // slot 1: InputProcessor::Update sub_82592188
  uint32_t update() const { return guest::LoadU32(address + 4); }
  void set_update(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t update_address() const { return address + 4; }
};

// embedded; measure: InputManager::GetProcessor.r4, InputManager::GetLogicalPad.r4, InputManager::GetPhysicalPad.r4, InputManager::AddListener.r4, LogicalPad::Bind.r4
struct InputIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t index() const { return guest::LoadU32(address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: InputProcessor::TestPressed.r4, InputProcessor::TestRepeat.r4, InputProcessor::AnyPressed.r4, InputProcessor::PressedOrRepeat.r4
struct InputButtonMask : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t mask() const { return guest::LoadU32(address + 0); }
  void set_mask(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t mask_address() const { return address + 0; }
};

// embedded; measure: PtrList::Insert.r5
struct PtrListInsertValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
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
  static PtrListNode New() {
    return PtrListNode(memory::MakeHandle(new PtrListNodeData(), sizeof(PtrListNodeData), "PtrListNode", &kPtrListNodeLayout));
  }
  template <uint32_t Stride = kSize>
  static PtrListNode NewArray(uint32_t count) {
    static_assert(sizeof(PtrListNodeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PtrListNodeData*>(::operator new(sizeof(PtrListNodeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PtrListNodeData();
    return PtrListNode(memory::MakeHandle(p, sizeof(PtrListNodeData) * count, "PtrListNode", &kPtrListNodeLayout, sizeof(PtrListNodeData)));
  }
  PtrListNode at(uint32_t i) const { return PtrListNode(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  PtrListNodeData* direct() const { return static_cast<PtrListNodeData*>(memory::HandleOwn(address, &kPtrListNodeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPtrListNodeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPtrListNodeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t next_address() const { return address + 0; }
  uint32_t next_bits() const { if (auto* d = direct()) return d->next; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_next_bits(uint32_t v) const { if (auto* d = direct()) { d->next = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = PtrListNode>
  T next() const { return T(next_bits()); }
  void set_next(const View& w) const { set_next_bits(w.address); }
  void set_next(uint32_t v) const { set_next_bits(v); }
  uint32_t previous_address() const { return address + 4; }
  uint32_t previous_bits() const { if (auto* d = direct()) return d->previous; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_previous_bits(uint32_t v) const { if (auto* d = direct()) { d->previous = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = PtrListNode>
  T previous() const { return T(previous_bits()); }
  void set_previous(const View& w) const { set_previous_bits(w.address); }
  void set_previous(uint32_t v) const { set_previous_bits(v); }
  // a listener (an InputProcessor) in the input manager's lists
  uint32_t value_address() const { return address + 8; }
  uint32_t value_bits() const { if (auto* d = direct()) return d->value; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_value_bits(uint32_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t value() const { if (auto* d = direct()) return d->value; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_value(uint32_t v) const { if (auto* d = direct()) { d->value = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
};

}  // namespace schema

#include "schema/action_mappers.h"
#include "schema/app_states.h"
#include "schema/model_animation.h"
