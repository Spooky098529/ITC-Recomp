// The views of docs/functions/action-mappers.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/action-mappers.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ActionMapper, ActionMapper::ResetWords.r3, ActionMapper::ResetAnalog.r3, ActionMapper::ResetWords.r3.+24, ActionMapper@+448.+24, ActionMapper::ResetAnalog.r3.+180
#pragma pack(push, 1)
struct ActionMapperData {
  uint32_t vtable = 0;  // +0
  uint32_t client_type = 0;  // +4
  uint32_t next_client = 0;  // +8
  uint8_t kinds[120] = {};  // +12, guest order
  uint8_t analog_kinds[120] = {};  // +132, guest order
  uint8_t links[120] = {};  // +252, guest order
  uint32_t config = 0;  // +372
  uint32_t config_links = 0;  // +376
  uint32_t analog_config = 0;  // +380
  uint32_t analog_config_links = 0;  // +384
  uint16_t raw = 0;  // +388
  uint16_t raw_start = 0;  // +390
  uint16_t raw_dpad_up = 0;  // +392
  uint16_t raw_dpad_down = 0;  // +394
  uint16_t raw_dpad_left = 0;  // +396
  uint16_t raw_dpad_right = 0;  // +398
  uint16_t raw_y = 0;  // +400
  uint16_t raw_b = 0;  // +402
  uint16_t raw_a = 0;  // +404
  uint16_t raw_x = 0;  // +406
  uint16_t raw_left_shoulder = 0;  // +408
  uint16_t raw_right_shoulder = 0;  // +410
  uint16_t raw_left_trigger = 0;  // +412
  uint16_t raw_right_trigger = 0;  // +414
  uint16_t raw_left_stick_click = 0;  // +416
  uint16_t raw_right_stick_click = 0;  // +418
  uint16_t raw_left_y_plus = 0;  // +420
  uint16_t raw_left_y_minus = 0;  // +422
  uint16_t raw_left_x_minus = 0;  // +424
  uint16_t raw_left_x_plus = 0;  // +426
  uint16_t raw_right_y_plus = 0;  // +428
  uint16_t raw_right_y_minus = 0;  // +430
  uint16_t raw_right_x_minus = 0;  // +432
  uint16_t raw_right_x_plus = 0;  // +434
  uint16_t raw_dpad_x = 0;  // +436
  uint16_t raw_dpad_y = 0;  // +438
  uint16_t raw_left_x = 0;  // +440
  uint16_t raw_left_y = 0;  // +442
  uint16_t raw_right_x = 0;  // +444
  uint16_t raw_right_y = 0;  // +446
  uint32_t held = 0;  // +448
  uint32_t held_before = 0;  // +452
  uint32_t pressed = 0;  // +456
  uint32_t released = 0;  // +460
  uint32_t fired = 0;  // +464
  uint32_t pending = 0;  // +468
  uint8_t repeat_pointers[120] = {};  // +472, guest order
  uint8_t repeats[480] = {};  // +592, guest order
  uint8_t analog_raw[60] = {};  // +1072, guest order
  uint8_t analog_values[120] = {};  // +1132, guest order
  uint8_t curve_pointers[120] = {};  // +1252, guest order
  uint8_t curves[480] = {};  // +1372, guest order
  uint8_t thresholds[600] = {};  // +1852, guest order
  uint8_t unknown_2452[12] = {};  // +2452, guest order
  uint8_t listener[416] = {};  // +2464, guest order
  uint32_t pad_values = 0;  // +2880
  uint8_t bound = 0;  // +2884
  uint8_t pad_2885[3] = {};
  uint32_t player = 0;  // +2888
  uint32_t input_manager = 0;  // +2892
};
#pragma pack(pop)
static_assert(sizeof(ActionMapperData) == 2896);
inline constexpr memory::LayoutField kActionMapperFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}, {160, 4}, {164, 4}, {168, 4}, {172, 4}, {176, 4}, {180, 4}, {184, 4}, {188, 4}, {192, 4}, {196, 4}, {200, 4}, {204, 4}, {208, 4}, {212, 4}, {216, 4}, {220, 4}, {224, 4}, {228, 4}, {232, 4}, {236, 4}, {240, 4}, {244, 4}, {248, 4}, {252, 4}, {256, 4}, {260, 4}, {264, 4}, {268, 4}, {272, 4}, {276, 4}, {280, 4}, {284, 4}, {288, 4}, {292, 4}, {296, 4}, {300, 4}, {304, 4}, {308, 4}, {312, 4}, {316, 4}, {320, 4}, {324, 4}, {328, 4}, {332, 4}, {336, 4}, {340, 4}, {344, 4}, {348, 4}, {352, 4}, {356, 4}, {360, 4}, {364, 4}, {368, 4}, {372, 4}, {376, 4}, {380, 4}, {384, 4}, {388, 2}, {390, 2}, {392, 2}, {394, 2}, {396, 2}, {398, 2}, {400, 2}, {402, 2}, {404, 2}, {406, 2}, {408, 2}, {410, 2}, {412, 2}, {414, 2}, {416, 2}, {418, 2}, {420, 2}, {422, 2}, {424, 2}, {426, 2}, {428, 2}, {430, 2}, {432, 2}, {434, 2}, {436, 2}, {438, 2}, {440, 2}, {442, 2}, {444, 2}, {446, 2}, {448, 4}, {452, 4}, {456, 4}, {460, 4}, {464, 4}, {468, 4}, {472, 4}, {476, 4}, {480, 4}, {484, 4}, {488, 4}, {492, 4}, {496, 4}, {500, 4}, {504, 4}, {508, 4}, {512, 4}, {516, 4}, {520, 4}, {524, 4}, {528, 4}, {532, 4}, {536, 4}, {540, 4}, {544, 4}, {548, 4}, {552, 4}, {556, 4}, {560, 4}, {564, 4}, {568, 4}, {572, 4}, {576, 4}, {580, 4}, {584, 4}, {588, 4}, {592, 4}, {596, 4}, {600, 4}, {604, 4}, {608, 4}, {612, 4}, {616, 4}, {620, 4}, {624, 4}, {628, 4}, {632, 4}, {636, 4}, {640, 4}, {644, 4}, {648, 4}, {652, 4}, {656, 4}, {660, 4}, {664, 4}, {668, 4}, {672, 4}, {676, 4}, {680, 4}, {684, 4}, {688, 4}, {692, 4}, {696, 4}, {700, 4}, {704, 4}, {708, 4}, {712, 4}, {716, 4}, {720, 4}, {724, 4}, {728, 4}, {732, 4}, {736, 4}, {740, 4}, {744, 4}, {748, 4}, {752, 4}, {756, 4}, {760, 4}, {764, 4}, {768, 4}, {772, 4}, {776, 4}, {780, 4}, {784, 4}, {788, 4}, {792, 4}, {796, 4}, {800, 4}, {804, 4}, {808, 4}, {812, 4}, {816, 4}, {820, 4}, {824, 4}, {828, 4}, {832, 4}, {836, 4}, {840, 4}, {844, 4}, {848, 4}, {852, 4}, {856, 4}, {860, 4}, {864, 4}, {868, 4}, {872, 4}, {876, 4}, {880, 4}, {884, 4}, {888, 4}, {892, 4}, {896, 4}, {900, 4}, {904, 4}, {908, 4}, {912, 4}, {916, 4}, {920, 4}, {924, 4}, {928, 4}, {932, 4}, {936, 4}, {940, 4}, {944, 4}, {948, 4}, {952, 4}, {956, 4}, {960, 4}, {964, 4}, {968, 4}, {972, 4}, {976, 4}, {980, 4}, {984, 4}, {988, 4}, {992, 4}, {996, 4}, {1000, 4}, {1004, 4}, {1008, 4}, {1012, 4}, {1016, 4}, {1020, 4}, {1024, 4}, {1028, 4}, {1032, 4}, {1036, 4}, {1040, 4}, {1044, 4}, {1048, 4}, {1052, 4}, {1056, 4}, {1060, 4}, {1064, 4}, {1068, 4}, {1072, 2}, {1074, 2}, {1076, 2}, {1078, 2}, {1080, 2}, {1082, 2}, {1084, 2}, {1086, 2}, {1088, 2}, {1090, 2}, {1092, 2}, {1094, 2}, {1096, 2}, {1098, 2}, {1100, 2}, {1102, 2}, {1104, 2}, {1106, 2}, {1108, 2}, {1110, 2}, {1112, 2}, {1114, 2}, {1116, 2}, {1118, 2}, {1120, 2}, {1122, 2}, {1124, 2}, {1126, 2}, {1128, 2}, {1130, 2}, {1132, 4}, {1136, 4}, {1140, 4}, {1144, 4}, {1148, 4}, {1152, 4}, {1156, 4}, {1160, 4}, {1164, 4}, {1168, 4}, {1172, 4}, {1176, 4}, {1180, 4}, {1184, 4}, {1188, 4}, {1192, 4}, {1196, 4}, {1200, 4}, {1204, 4}, {1208, 4}, {1212, 4}, {1216, 4}, {1220, 4}, {1224, 4}, {1228, 4}, {1232, 4}, {1236, 4}, {1240, 4}, {1244, 4}, {1248, 4}, {1252, 4}, {1256, 4}, {1260, 4}, {1264, 4}, {1268, 4}, {1272, 4}, {1276, 4}, {1280, 4}, {1284, 4}, {1288, 4}, {1292, 4}, {1296, 4}, {1300, 4}, {1304, 4}, {1308, 4}, {1312, 4}, {1316, 4}, {1320, 4}, {1324, 4}, {1328, 4}, {1332, 4}, {1336, 4}, {1340, 4}, {1344, 4}, {1348, 4}, {1352, 4}, {1356, 4}, {1360, 4}, {1364, 4}, {1368, 4}, {1372, 4}, {1376, 4}, {1380, 4}, {1384, 4}, {1388, 4}, {1392, 4}, {1396, 4}, {1400, 4}, {1404, 4}, {1408, 4}, {1412, 4}, {1416, 4}, {1420, 4}, {1424, 4}, {1428, 4}, {1432, 4}, {1436, 4}, {1440, 4}, {1444, 4}, {1448, 4}, {1452, 4}, {1456, 4}, {1460, 4}, {1464, 4}, {1468, 4}, {1472, 4}, {1476, 4}, {1480, 4}, {1484, 4}, {1488, 4}, {1492, 4}, {1496, 4}, {1500, 4}, {1504, 4}, {1508, 4}, {1512, 4}, {1516, 4}, {1520, 4}, {1524, 4}, {1528, 4}, {1532, 4}, {1536, 4}, {1540, 4}, {1544, 4}, {1548, 4}, {1552, 4}, {1556, 4}, {1560, 4}, {1564, 4}, {1568, 4}, {1572, 4}, {1576, 4}, {1580, 4}, {1584, 4}, {1588, 4}, {1592, 4}, {1596, 4}, {1600, 4}, {1604, 4}, {1608, 4}, {1612, 4}, {1616, 4}, {1620, 4}, {1624, 4}, {1628, 4}, {1632, 4}, {1636, 4}, {1640, 4}, {1644, 4}, {1648, 4}, {1652, 4}, {1656, 4}, {1660, 4}, {1664, 4}, {1668, 4}, {1672, 4}, {1676, 4}, {1680, 4}, {1684, 4}, {1688, 4}, {1692, 4}, {1696, 4}, {1700, 4}, {1704, 4}, {1708, 4}, {1712, 4}, {1716, 4}, {1720, 4}, {1724, 4}, {1728, 4}, {1732, 4}, {1736, 4}, {1740, 4}, {1744, 4}, {1748, 4}, {1752, 4}, {1756, 4}, {1760, 4}, {1764, 4}, {1768, 4}, {1772, 4}, {1776, 4}, {1780, 4}, {1784, 4}, {1788, 4}, {1792, 4}, {1796, 4}, {1800, 4}, {1804, 4}, {1808, 4}, {1812, 4}, {1816, 4}, {1820, 4}, {1824, 4}, {1828, 4}, {1832, 4}, {1836, 4}, {1840, 4}, {1844, 4}, {1848, 4}, {1852, 4}, {1856, 4}, {1860, 4}, {1864, 4}, {1872, 4}, {1876, 4}, {1880, 4}, {1884, 4}, {1892, 4}, {1896, 4}, {1900, 4}, {1904, 4}, {1912, 4}, {1916, 4}, {1920, 4}, {1924, 4}, {1932, 4}, {1936, 4}, {1940, 4}, {1944, 4}, {1952, 4}, {1956, 4}, {1960, 4}, {1964, 4}, {1972, 4}, {1976, 4}, {1980, 4}, {1984, 4}, {1992, 4}, {1996, 4}, {2000, 4}, {2004, 4}, {2012, 4}, {2016, 4}, {2020, 4}, {2024, 4}, {2032, 4}, {2036, 4}, {2040, 4}, {2044, 4}, {2052, 4}, {2056, 4}, {2060, 4}, {2064, 4}, {2072, 4}, {2076, 4}, {2080, 4}, {2084, 4}, {2092, 4}, {2096, 4}, {2100, 4}, {2104, 4}, {2112, 4}, {2116, 4}, {2120, 4}, {2124, 4}, {2132, 4}, {2136, 4}, {2140, 4}, {2144, 4}, {2152, 4}, {2156, 4}, {2160, 4}, {2164, 4}, {2172, 4}, {2176, 4}, {2180, 4}, {2184, 4}, {2192, 4}, {2196, 4}, {2200, 4}, {2204, 4}, {2212, 4}, {2216, 4}, {2220, 4}, {2224, 4}, {2232, 4}, {2236, 4}, {2240, 4}, {2244, 4}, {2252, 4}, {2256, 4}, {2260, 4}, {2264, 4}, {2272, 4}, {2276, 4}, {2280, 4}, {2284, 4}, {2292, 4}, {2296, 4}, {2300, 4}, {2304, 4}, {2312, 4}, {2316, 4}, {2320, 4}, {2324, 4}, {2332, 4}, {2336, 4}, {2340, 4}, {2344, 4}, {2352, 4}, {2356, 4}, {2360, 4}, {2364, 4}, {2372, 4}, {2376, 4}, {2380, 4}, {2384, 4}, {2392, 4}, {2396, 4}, {2400, 4}, {2404, 4}, {2412, 4}, {2416, 4}, {2420, 4}, {2424, 4}, {2432, 4}, {2436, 4}, {2440, 4}, {2444, 4}, {2464, 4}, {2480, 4}, {2484, 4}, {2488, 4}, {2492, 4}, {2496, 4}, {2500, 4}, {2504, 4}, {2508, 4}, {2512, 4}, {2516, 4}, {2520, 4}, {2524, 4}, {2528, 4}, {2532, 4}, {2536, 4}, {2540, 4}, {2544, 4}, {2548, 4}, {2552, 4}, {2556, 4}, {2560, 4}, {2564, 4}, {2568, 4}, {2572, 4}, {2576, 4}, {2580, 4}, {2584, 4}, {2588, 4}, {2592, 4}, {2596, 4}, {2600, 4}, {2728, 4}, {2732, 4}, {2736, 4}, {2740, 4}, {2744, 4}, {2880, 4}, {2888, 4}, {2892, 4}};
inline constexpr memory::Layout kActionMapperLayout = {kActionMapperFields, 653};

struct ActionMapper : View {
  using View::View;
  static constexpr uint32_t kSize = 2896;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82007F78;
  ActionMapperData* data() const { return static_cast<ActionMapperData*>(memory::HandleObject(address)); }
  static ActionMapper New(uint8_t* base) {
    return ActionMapper(base, memory::MakeHandle(new ActionMapperData(), sizeof(ActionMapperData), "ActionMapper", &kActionMapperLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionMapper NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionMapperData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionMapperData*>(::operator new(sizeof(ActionMapperData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionMapperData();
    return ActionMapper(base, memory::MakeHandle(p, sizeof(ActionMapperData) * count, "ActionMapper", &kActionMapperLayout, sizeof(ActionMapperData)));
  }
  ActionMapper at(uint32_t i) const { return ActionMapper(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionMapperLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, offset, width, v); }
  // 0x82007F78
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // the client node's type: 2
  uint32_t client_type_address() const { return address + 4; }
  uint32_t client_type() const { return data()->client_type; }
  void set_client_type(uint32_t v) const { data()->client_type = v; }
  uint32_t client_type_bits() const { return data()->client_type; }
  void set_client_type_bits(uint32_t v) const { data()->client_type = v; }
  // the next node of the client list
  uint32_t next_client_address() const { return address + 8; }
  template <typename T = InputClientNode>
  T next_client() const { return T(base, data()->next_client); }
  void set_next_client(const View& w) const { const uint32_t v = w.address; data()->next_client = v; }
  void set_next_client(uint32_t v) const { data()->next_client = v; }
  uint32_t next_client_bits() const { return data()->next_client; }
  void set_next_client_bits(uint32_t v) const { data()->next_client = v; }
  // 30 words: each action's kind (1, digital); the embedded kind
  uint32_t kinds_address() const { return address + 12; }
  uint8_t* kinds() const { return reinterpret_cast<uint8_t*>(data()) + 12; }
  template <typename T = ActionWord>
  T kind(uint32_t i) const { return T(base, address + 12 + i * T::kSize); }
  // 30 words: each action's analog kind (2 for actions 24-29, else 1); the embedded analog_kind
  uint32_t analog_kinds_address() const { return address + 132; }
  uint8_t* analog_kinds() const { return reinterpret_cast<uint8_t*>(data()) + 132; }
  template <typename T = ActionWord>
  T analog_kind(uint32_t i) const { return T(base, address + 132 + i * T::kSize); }
  // 30 words: each action's linked action (probably each direction's opposite; 30 none); the embedded link
  uint32_t links_address() const { return address + 252; }
  uint8_t* links() const { return reinterpret_cast<uint8_t*>(data()) + 252; }
  template <typename T = ActionWord>
  T link(uint32_t i) const { return T(base, address + 252 + i * T::kSize); }
  // an ActionMapperConfig (the kinds and links pointers), then at +380 the analog stage's
  uint32_t config_address() const { return address + 372; }
  uint32_t config() const { return data()->config; }
  void set_config(uint32_t v) const { data()->config = v; }
  uint32_t config_bits() const { return data()->config; }
  void set_config_bits(uint32_t v) const { data()->config = v; }
  // the config's links pointer (mapper + 252)
  uint32_t config_links_address() const { return address + 376; }
  uint32_t config_links() const { return data()->config_links; }
  void set_config_links(uint32_t v) const { data()->config_links = v; }
  uint32_t config_links_bits() const { return data()->config_links; }
  void set_config_links_bits(uint32_t v) const { data()->config_links = v; }
  // the analog stage's ActionMapperConfig: its kinds pointer (mapper + 132)
  uint32_t analog_config_address() const { return address + 380; }
  uint32_t analog_config() const { return data()->analog_config; }
  void set_analog_config(uint32_t v) const { data()->analog_config = v; }
  uint32_t analog_config_bits() const { return data()->analog_config; }
  void set_analog_config_bits(uint32_t v) const { data()->analog_config = v; }
  // its links pointer (mapper + 252)
  uint32_t analog_config_links_address() const { return address + 384; }
  uint32_t analog_config_links() const { return data()->analog_config_links; }
  void set_analog_config_links(uint32_t v) const { data()->analog_config_links = v; }
  uint32_t analog_config_links_bits() const { return data()->analog_config_links; }
  void set_analog_config_links_bits(uint32_t v) const { data()->analog_config_links = v; }
  // 30 u16s: this tick's raw value per action; [0] Back (151)
  uint32_t raw_address() const { return address + 388; }
  uint16_t raw() const { return data()->raw; }
  void set_raw(uint16_t v) const { data()->raw = v; }
  uint16_t raw_bits() const { return data()->raw; }
  void set_raw_bits(uint16_t v) const { data()->raw = v; }
  // action 1 (152)
  uint32_t raw_start_address() const { return address + 390; }
  uint16_t raw_start() const { return data()->raw_start; }
  void set_raw_start(uint16_t v) const { data()->raw_start = v; }
  uint16_t raw_start_bits() const { return data()->raw_start; }
  void set_raw_start_bits(uint16_t v) const { data()->raw_start = v; }
  // action 2 (153)
  uint32_t raw_dpad_up_address() const { return address + 392; }
  uint16_t raw_dpad_up() const { return data()->raw_dpad_up; }
  void set_raw_dpad_up(uint16_t v) const { data()->raw_dpad_up = v; }
  uint16_t raw_dpad_up_bits() const { return data()->raw_dpad_up; }
  void set_raw_dpad_up_bits(uint16_t v) const { data()->raw_dpad_up = v; }
  // action 3 (154)
  uint32_t raw_dpad_down_address() const { return address + 394; }
  uint16_t raw_dpad_down() const { return data()->raw_dpad_down; }
  void set_raw_dpad_down(uint16_t v) const { data()->raw_dpad_down = v; }
  uint16_t raw_dpad_down_bits() const { return data()->raw_dpad_down; }
  void set_raw_dpad_down_bits(uint16_t v) const { data()->raw_dpad_down = v; }
  // action 4 (155)
  uint32_t raw_dpad_left_address() const { return address + 396; }
  uint16_t raw_dpad_left() const { return data()->raw_dpad_left; }
  void set_raw_dpad_left(uint16_t v) const { data()->raw_dpad_left = v; }
  uint16_t raw_dpad_left_bits() const { return data()->raw_dpad_left; }
  void set_raw_dpad_left_bits(uint16_t v) const { data()->raw_dpad_left = v; }
  // action 5 (156)
  uint32_t raw_dpad_right_address() const { return address + 398; }
  uint16_t raw_dpad_right() const { return data()->raw_dpad_right; }
  void set_raw_dpad_right(uint16_t v) const { data()->raw_dpad_right = v; }
  uint16_t raw_dpad_right_bits() const { return data()->raw_dpad_right; }
  void set_raw_dpad_right_bits(uint16_t v) const { data()->raw_dpad_right = v; }
  // action 6 (157)
  uint32_t raw_y_address() const { return address + 400; }
  uint16_t raw_y() const { return data()->raw_y; }
  void set_raw_y(uint16_t v) const { data()->raw_y = v; }
  uint16_t raw_y_bits() const { return data()->raw_y; }
  void set_raw_y_bits(uint16_t v) const { data()->raw_y = v; }
  // action 7 (158)
  uint32_t raw_b_address() const { return address + 402; }
  uint16_t raw_b() const { return data()->raw_b; }
  void set_raw_b(uint16_t v) const { data()->raw_b = v; }
  uint16_t raw_b_bits() const { return data()->raw_b; }
  void set_raw_b_bits(uint16_t v) const { data()->raw_b = v; }
  // action 8 (159)
  uint32_t raw_a_address() const { return address + 404; }
  uint16_t raw_a() const { return data()->raw_a; }
  void set_raw_a(uint16_t v) const { data()->raw_a = v; }
  uint16_t raw_a_bits() const { return data()->raw_a; }
  void set_raw_a_bits(uint16_t v) const { data()->raw_a = v; }
  // action 9 (160)
  uint32_t raw_x_address() const { return address + 406; }
  uint16_t raw_x() const { return data()->raw_x; }
  void set_raw_x(uint16_t v) const { data()->raw_x = v; }
  uint16_t raw_x_bits() const { return data()->raw_x; }
  void set_raw_x_bits(uint16_t v) const { data()->raw_x = v; }
  // action 10 (161)
  uint32_t raw_left_shoulder_address() const { return address + 408; }
  uint16_t raw_left_shoulder() const { return data()->raw_left_shoulder; }
  void set_raw_left_shoulder(uint16_t v) const { data()->raw_left_shoulder = v; }
  uint16_t raw_left_shoulder_bits() const { return data()->raw_left_shoulder; }
  void set_raw_left_shoulder_bits(uint16_t v) const { data()->raw_left_shoulder = v; }
  // action 11 (162)
  uint32_t raw_right_shoulder_address() const { return address + 410; }
  uint16_t raw_right_shoulder() const { return data()->raw_right_shoulder; }
  void set_raw_right_shoulder(uint16_t v) const { data()->raw_right_shoulder = v; }
  uint16_t raw_right_shoulder_bits() const { return data()->raw_right_shoulder; }
  void set_raw_right_shoulder_bits(uint16_t v) const { data()->raw_right_shoulder = v; }
  // action 12
  uint32_t raw_left_trigger_address() const { return address + 412; }
  uint16_t raw_left_trigger() const { return data()->raw_left_trigger; }
  void set_raw_left_trigger(uint16_t v) const { data()->raw_left_trigger = v; }
  uint16_t raw_left_trigger_bits() const { return data()->raw_left_trigger; }
  void set_raw_left_trigger_bits(uint16_t v) const { data()->raw_left_trigger = v; }
  // action 13
  uint32_t raw_right_trigger_address() const { return address + 414; }
  uint16_t raw_right_trigger() const { return data()->raw_right_trigger; }
  void set_raw_right_trigger(uint16_t v) const { data()->raw_right_trigger = v; }
  uint16_t raw_right_trigger_bits() const { return data()->raw_right_trigger; }
  void set_raw_right_trigger_bits(uint16_t v) const { data()->raw_right_trigger = v; }
  // action 14 (165)
  uint32_t raw_left_stick_click_address() const { return address + 416; }
  uint16_t raw_left_stick_click() const { return data()->raw_left_stick_click; }
  void set_raw_left_stick_click(uint16_t v) const { data()->raw_left_stick_click = v; }
  uint16_t raw_left_stick_click_bits() const { return data()->raw_left_stick_click; }
  void set_raw_left_stick_click_bits(uint16_t v) const { data()->raw_left_stick_click = v; }
  // action 15 (166)
  uint32_t raw_right_stick_click_address() const { return address + 418; }
  uint16_t raw_right_stick_click() const { return data()->raw_right_stick_click; }
  void set_raw_right_stick_click(uint16_t v) const { data()->raw_right_stick_click = v; }
  uint16_t raw_right_stick_click_bits() const { return data()->raw_right_stick_click; }
  void set_raw_right_stick_click_bits(uint16_t v) const { data()->raw_right_stick_click = v; }
  // action 16: the left stick's y above 0
  uint32_t raw_left_y_plus_address() const { return address + 420; }
  uint16_t raw_left_y_plus() const { return data()->raw_left_y_plus; }
  void set_raw_left_y_plus(uint16_t v) const { data()->raw_left_y_plus = v; }
  uint16_t raw_left_y_plus_bits() const { return data()->raw_left_y_plus; }
  void set_raw_left_y_plus_bits(uint16_t v) const { data()->raw_left_y_plus = v; }
  // action 17
  uint32_t raw_left_y_minus_address() const { return address + 422; }
  uint16_t raw_left_y_minus() const { return data()->raw_left_y_minus; }
  void set_raw_left_y_minus(uint16_t v) const { data()->raw_left_y_minus = v; }
  uint16_t raw_left_y_minus_bits() const { return data()->raw_left_y_minus; }
  void set_raw_left_y_minus_bits(uint16_t v) const { data()->raw_left_y_minus = v; }
  // action 18
  uint32_t raw_left_x_minus_address() const { return address + 424; }
  uint16_t raw_left_x_minus() const { return data()->raw_left_x_minus; }
  void set_raw_left_x_minus(uint16_t v) const { data()->raw_left_x_minus = v; }
  uint16_t raw_left_x_minus_bits() const { return data()->raw_left_x_minus; }
  void set_raw_left_x_minus_bits(uint16_t v) const { data()->raw_left_x_minus = v; }
  // action 19
  uint32_t raw_left_x_plus_address() const { return address + 426; }
  uint16_t raw_left_x_plus() const { return data()->raw_left_x_plus; }
  void set_raw_left_x_plus(uint16_t v) const { data()->raw_left_x_plus = v; }
  uint16_t raw_left_x_plus_bits() const { return data()->raw_left_x_plus; }
  void set_raw_left_x_plus_bits(uint16_t v) const { data()->raw_left_x_plus = v; }
  // action 20
  uint32_t raw_right_y_plus_address() const { return address + 428; }
  uint16_t raw_right_y_plus() const { return data()->raw_right_y_plus; }
  void set_raw_right_y_plus(uint16_t v) const { data()->raw_right_y_plus = v; }
  uint16_t raw_right_y_plus_bits() const { return data()->raw_right_y_plus; }
  void set_raw_right_y_plus_bits(uint16_t v) const { data()->raw_right_y_plus = v; }
  // action 21
  uint32_t raw_right_y_minus_address() const { return address + 430; }
  uint16_t raw_right_y_minus() const { return data()->raw_right_y_minus; }
  void set_raw_right_y_minus(uint16_t v) const { data()->raw_right_y_minus = v; }
  uint16_t raw_right_y_minus_bits() const { return data()->raw_right_y_minus; }
  void set_raw_right_y_minus_bits(uint16_t v) const { data()->raw_right_y_minus = v; }
  // action 22
  uint32_t raw_right_x_minus_address() const { return address + 432; }
  uint16_t raw_right_x_minus() const { return data()->raw_right_x_minus; }
  void set_raw_right_x_minus(uint16_t v) const { data()->raw_right_x_minus = v; }
  uint16_t raw_right_x_minus_bits() const { return data()->raw_right_x_minus; }
  void set_raw_right_x_minus_bits(uint16_t v) const { data()->raw_right_x_minus = v; }
  // action 23
  uint32_t raw_right_x_plus_address() const { return address + 434; }
  uint16_t raw_right_x_plus() const { return data()->raw_right_x_plus; }
  void set_raw_right_x_plus(uint16_t v) const { data()->raw_right_x_plus = v; }
  uint16_t raw_right_x_plus_bits() const { return data()->raw_right_x_plus; }
  void set_raw_right_x_plus_bits(uint16_t v) const { data()->raw_right_x_plus = v; }
  // action 24: the d-pad's axis from actions 4 and 5
  uint32_t raw_dpad_x_address() const { return address + 436; }
  uint16_t raw_dpad_x() const { return data()->raw_dpad_x; }
  void set_raw_dpad_x(uint16_t v) const { data()->raw_dpad_x = v; }
  uint16_t raw_dpad_x_bits() const { return data()->raw_dpad_x; }
  void set_raw_dpad_x_bits(uint16_t v) const { data()->raw_dpad_x = v; }
  // action 25: from actions 2 and 3
  uint32_t raw_dpad_y_address() const { return address + 438; }
  uint16_t raw_dpad_y() const { return data()->raw_dpad_y; }
  void set_raw_dpad_y(uint16_t v) const { data()->raw_dpad_y = v; }
  uint16_t raw_dpad_y_bits() const { return data()->raw_dpad_y; }
  void set_raw_dpad_y_bits(uint16_t v) const { data()->raw_dpad_y = v; }
  // action 26: the left stick's axis from actions 18 and 19
  uint32_t raw_left_x_address() const { return address + 440; }
  uint16_t raw_left_x() const { return data()->raw_left_x; }
  void set_raw_left_x(uint16_t v) const { data()->raw_left_x = v; }
  uint16_t raw_left_x_bits() const { return data()->raw_left_x; }
  void set_raw_left_x_bits(uint16_t v) const { data()->raw_left_x = v; }
  // action 27: from actions 16 and 17
  uint32_t raw_left_y_address() const { return address + 442; }
  uint16_t raw_left_y() const { return data()->raw_left_y; }
  void set_raw_left_y(uint16_t v) const { data()->raw_left_y = v; }
  uint16_t raw_left_y_bits() const { return data()->raw_left_y; }
  void set_raw_left_y_bits(uint16_t v) const { data()->raw_left_y = v; }
  // action 28: from actions 22 and 23
  uint32_t raw_right_x_address() const { return address + 444; }
  uint16_t raw_right_x() const { return data()->raw_right_x; }
  void set_raw_right_x(uint16_t v) const { data()->raw_right_x = v; }
  uint16_t raw_right_x_bits() const { return data()->raw_right_x; }
  void set_raw_right_x_bits(uint16_t v) const { data()->raw_right_x = v; }
  // action 29: from actions 20 and 21
  uint32_t raw_right_y_address() const { return address + 446; }
  uint16_t raw_right_y() const { return data()->raw_right_y; }
  void set_raw_right_y(uint16_t v) const { data()->raw_right_y = v; }
  uint16_t raw_right_y_bits() const { return data()->raw_right_y; }
  void set_raw_right_y_bits(uint16_t v) const { data()->raw_right_y = v; }
  // the input state (an ActionMapperState) starts here: bit i, action i held
  uint32_t held_address() const { return address + 448; }
  uint32_t held() const { return data()->held; }
  void set_held(uint32_t v) const { data()->held = v; }
  uint32_t held_bits() const { return data()->held; }
  void set_held_bits(uint32_t v) const { data()->held = v; }
  uint32_t held_before_address() const { return address + 452; }
  uint32_t held_before() const { return data()->held_before; }
  void set_held_before(uint32_t v) const { data()->held_before = v; }
  uint32_t held_before_bits() const { return data()->held_before; }
  void set_held_before_bits(uint32_t v) const { data()->held_before = v; }
  uint32_t pressed_address() const { return address + 456; }
  uint32_t pressed() const { return data()->pressed; }
  void set_pressed(uint32_t v) const { data()->pressed = v; }
  uint32_t pressed_bits() const { return data()->pressed; }
  void set_pressed_bits(uint32_t v) const { data()->pressed = v; }
  uint32_t released_address() const { return address + 460; }
  uint32_t released() const { return data()->released; }
  void set_released(uint32_t v) const { data()->released = v; }
  uint32_t released_bits() const { return data()->released; }
  void set_released_bits(uint32_t v) const { data()->released = v; }
  // pressed or repeating
  uint32_t fired_address() const { return address + 464; }
  uint32_t fired() const { return data()->fired; }
  void set_fired(uint32_t v) const { data()->fired = v; }
  uint32_t fired_bits() const { return data()->fired; }
  void set_fired_bits(uint32_t v) const { data()->fired = v; }
  // ORed into the next update's held before and pressed
  uint32_t pending_address() const { return address + 468; }
  uint32_t pending() const { return data()->pending; }
  void set_pending(uint32_t v) const { data()->pending = v; }
  uint32_t pending_bits() const { return data()->pending; }
  void set_pending_bits(uint32_t v) const { data()->pending = v; }
  // 30 pointers to the repeat objects; the embedded repeat_pointer
  uint32_t repeat_pointers_address() const { return address + 472; }
  uint8_t* repeat_pointers() const { return reinterpret_cast<uint8_t*>(data()) + 472; }
  template <typename T = ActionWord>
  T repeat_pointer(uint32_t i) const { return T(base, address + 472 + i * T::kSize); }
  // 30 ActionRepeats (16 bytes); the embedded repeat (the constructor's store to the first's vtable is the measure's +592)
  uint32_t repeats_address() const { return address + 592; }
  uint8_t* repeats() const { return reinterpret_cast<uint8_t*>(data()) + 592; }
  template <typename T = ActionRepeat>
  T repeat(uint32_t i) const { return T(base, address + 592 + i * T::kSize); }
  // the analog stage (an ActionMapperAnalog) starts here: 30 u16s; the embedded analog_raw_value
  uint32_t analog_raw_address() const { return address + 1072; }
  uint8_t* analog_raw() const { return reinterpret_cast<uint8_t*>(data()) + 1072; }
  template <typename T = ActionHalf>
  T analog_raw_value(uint32_t i) const { return T(base, address + 1072 + i * T::kSize); }
  // 30 floats: the analog values through each action's curve; the embedded analog_value
  uint32_t analog_values_address() const { return address + 1132; }
  uint8_t* analog_values() const { return reinterpret_cast<uint8_t*>(data()) + 1132; }
  template <typename T = ActionFloat>
  T analog_value(uint32_t i) const { return T(base, address + 1132 + i * T::kSize); }
  // 30 pointers to the curve objects; the embedded curve_pointer
  uint32_t curve_pointers_address() const { return address + 1252; }
  uint8_t* curve_pointers() const { return reinterpret_cast<uint8_t*>(data()) + 1252; }
  template <typename T = ActionWord>
  T curve_pointer(uint32_t i) const { return T(base, address + 1252 + i * T::kSize); }
  // 30 ActionCurves (16 bytes); the embedded curve (the constructor's store to the first's vtable is the measure's +1372)
  uint32_t curves_address() const { return address + 1372; }
  uint8_t* curves() const { return reinterpret_cast<uint8_t*>(data()) + 1372; }
  template <typename T = ActionCurve>
  T curve(uint32_t i) const { return T(base, address + 1372 + i * T::kSize); }
  // 30 ActionThresholds (20 bytes); the embedded threshold
  uint32_t thresholds_address() const { return address + 1852; }
  uint8_t* thresholds() const { return reinterpret_cast<uint8_t*>(data()) + 1852; }
  template <typename T = ActionThreshold>
  T threshold(uint32_t i) const { return T(base, address + 1852 + i * T::kSize); }
  uint32_t unknown_2452_address() const { return address + 2452; }
  uint8_t* unknown_2452() const { return reinterpret_cast<uint8_t*>(data()) + 2452; }
  // the listener: an input processor (vtable 0x8206C0C4, input.md); the embedded listener_processor
  uint32_t listener_address() const { return address + 2464; }
  uint8_t* listener() const { return reinterpret_cast<uint8_t*>(data()) + 2464; }
  template <typename T = InputProcessor>
  T listener_processor() const { return T(base, address + 2464); }
  // measure fault: the analog stage's raw[0] (ActionMapperAnalog +0, mapper + 1072)
  uint32_t fault_analog_raw_address() const { return address + 0; }
  uint16_t fault_analog_raw_bits() const { return uint16_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 0, 2)); }
  void set_fault_analog_raw_bits(uint16_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 0, 2, v); }
  uint16_t fault_analog_raw() const { return uint16_t(fault_analog_raw_bits()); }
  void set_fault_analog_raw(uint16_t v) const { set_fault_analog_raw_bits(uint16_t(v)); }
  // measure fault: ActionMapperState's fired (mapper + 464)
  uint32_t fault_state_fired_address() const { return address + 16; }
  uint32_t fault_state_fired_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 16, 4)); }
  void set_fault_state_fired_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 16, 4, v); }
  uint32_t fault_state_fired() const { return uint32_t(fault_state_fired_bits()); }
  void set_fault_state_fired(uint32_t v) const { set_fault_state_fired_bits(uint32_t(v)); }
  // measure fault: ActionMapperState's pending (mapper + 468)
  uint32_t fault_state_pending_address() const { return address + 20; }
  uint32_t fault_state_pending_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 20, 4)); }
  void set_fault_state_pending_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 20, 4, v); }
  uint32_t fault_state_pending() const { return uint32_t(fault_state_pending_bits()); }
  void set_fault_state_pending(uint32_t v) const { set_fault_state_pending_bits(uint32_t(v)); }
  // measure fault: ActionMapperState's repeat pointers (mapper + 472)
  uint32_t fault_state_repeat_pointers_address() const { return address + 24; }
  uint32_t fault_state_repeat_pointers_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 24, 4)); }
  void set_fault_state_repeat_pointers_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 24, 4, v); }
  uint32_t fault_state_repeat_pointers() const { return uint32_t(fault_state_repeat_pointers_bits()); }
  void set_fault_state_repeat_pointers(uint32_t v) const { set_fault_state_repeat_pointers_bits(uint32_t(v)); }
  // measure fault: the analog stage's values (mapper + 1132)
  uint32_t fault_analog_values_address() const { return address + 60; }
  uint32_t fault_analog_values_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 60, 4)); }
  void set_fault_analog_values_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 60, 4, v); }
  float fault_analog_values() const { return std::bit_cast<float>(fault_analog_values_bits()); }
  void set_fault_analog_values(float v) const { set_fault_analog_values_bits(std::bit_cast<uint32_t>(v)); }
  // measure fault: the analog stage's curve pointers (mapper + 1252)
  uint32_t fault_analog_curve_pointers_address() const { return address + 180; }
  uint32_t fault_analog_curve_pointers_bits() const { return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 180, 4)); }
  void set_fault_analog_curve_pointers_bits(uint32_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperLayout, 180, 4, v); }
  uint32_t fault_analog_curve_pointers() const { return uint32_t(fault_analog_curve_pointers_bits()); }
  void set_fault_analog_curve_pointers(uint32_t v) const { set_fault_analog_curve_pointers_bits(uint32_t(v)); }
  // the logical pad's 40 values (input manager + 44 + 196 × player)
  uint32_t pad_values_address() const { return address + 2880; }
  template <typename T = ActionPadValues>
  T pad_values() const { return T(base, data()->pad_values); }
  void set_pad_values(const View& w) const { const uint32_t v = w.address; data()->pad_values = v; }
  void set_pad_values(uint32_t v) const { data()->pad_values = v; }
  uint32_t pad_values_bits() const { return data()->pad_values; }
  void set_pad_values_bits(uint32_t v) const { data()->pad_values = v; }
  uint32_t bound_address() const { return address + 2884; }
  uint8_t bound() const { return data()->bound; }
  void set_bound(uint8_t v) const { data()->bound = v; }
  uint8_t bound_bits() const { return data()->bound; }
  void set_bound_bits(uint8_t v) const { data()->bound = v; }
  // 0-3
  uint32_t player_address() const { return address + 2888; }
  uint32_t player() const { return data()->player; }
  void set_player(uint32_t v) const { data()->player = v; }
  uint32_t player_bits() const { return data()->player; }
  void set_player_bits(uint32_t v) const { data()->player = v; }
  // the input object it's bound to
  uint32_t input_manager_address() const { return address + 2892; }
  template <typename T = ActionMapperInputManager>
  T input_manager() const { return T(base, data()->input_manager); }
  void set_input_manager(const View& w) const { const uint32_t v = w.address; data()->input_manager = v; }
  void set_input_manager(uint32_t v) const { data()->input_manager = v; }
  uint32_t input_manager_bits() const { return data()->input_manager; }
  void set_input_manager_bits(uint32_t v) const { data()->input_manager = v; }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ActionMapperStateData {
  uint32_t held = 0;  // +0
  uint32_t held_before = 0;  // +4
  uint32_t pressed = 0;  // +8
  uint32_t released = 0;  // +12
  uint32_t fired = 0;  // +16
  uint32_t pending = 0;  // +20
  uint32_t repeat_pointers = 0;  // +24
  uint8_t pad_28[116] = {};
  uint32_t repeats = 0;  // +144
  uint8_t pad_148[476] = {};
  uint16_t analog = 0;  // +624
  uint8_t pad_626[58] = {};
  float analog_values = 0;  // +684
  uint8_t pad_688[716] = {};
  float thresholds = 0;  // +1404
  uint8_t pad_1408[596] = {};
};
#pragma pack(pop)
static_assert(sizeof(ActionMapperStateData) == 2004);
inline constexpr memory::LayoutField kActionMapperStateFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {144, 4}, {624, 2}, {684, 4}, {1404, 4}};
inline constexpr memory::Layout kActionMapperStateLayout = {kActionMapperStateFields, 11};

struct ActionMapperState : View {
  using View::View;
  static constexpr uint32_t kSize = 2004;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ActionMapperStateData* data() const { return static_cast<ActionMapperStateData*>(memory::HandleObject(address)); }
  static ActionMapperState New(uint8_t* base) {
    return ActionMapperState(base, memory::MakeHandle(new ActionMapperStateData(), sizeof(ActionMapperStateData), "ActionMapperState", &kActionMapperStateLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionMapperState NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionMapperStateData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionMapperStateData*>(::operator new(sizeof(ActionMapperStateData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionMapperStateData();
    return ActionMapperState(base, memory::MakeHandle(p, sizeof(ActionMapperStateData) * count, "ActionMapperState", &kActionMapperStateLayout, sizeof(ActionMapperStateData)));
  }
  ActionMapperState at(uint32_t i) const { return ActionMapperState(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionMapperStateLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperStateLayout, offset, width, v); }
  // bit i, action i held
  uint32_t held_address() const { return address + 0; }
  uint32_t held() const { return data()->held; }
  void set_held(uint32_t v) const { data()->held = v; }
  uint32_t held_bits() const { return data()->held; }
  void set_held_bits(uint32_t v) const { data()->held = v; }
  uint32_t held_before_address() const { return address + 4; }
  uint32_t held_before() const { return data()->held_before; }
  void set_held_before(uint32_t v) const { data()->held_before = v; }
  uint32_t held_before_bits() const { return data()->held_before; }
  void set_held_before_bits(uint32_t v) const { data()->held_before = v; }
  uint32_t pressed_address() const { return address + 8; }
  uint32_t pressed() const { return data()->pressed; }
  void set_pressed(uint32_t v) const { data()->pressed = v; }
  uint32_t pressed_bits() const { return data()->pressed; }
  void set_pressed_bits(uint32_t v) const { data()->pressed = v; }
  uint32_t released_address() const { return address + 12; }
  uint32_t released() const { return data()->released; }
  void set_released(uint32_t v) const { data()->released = v; }
  uint32_t released_bits() const { return data()->released; }
  void set_released_bits(uint32_t v) const { data()->released = v; }
  // pressed or repeating
  uint32_t fired_address() const { return address + 16; }
  uint32_t fired() const { return data()->fired; }
  void set_fired(uint32_t v) const { data()->fired = v; }
  uint32_t fired_bits() const { return data()->fired; }
  void set_fired_bits(uint32_t v) const { data()->fired = v; }
  // ORed into the next update's held before and pressed
  uint32_t pending_address() const { return address + 20; }
  uint32_t pending() const { return data()->pending; }
  void set_pending(uint32_t v) const { data()->pending = v; }
  uint32_t pending_bits() const { return data()->pending; }
  void set_pending_bits(uint32_t v) const { data()->pending = v; }
  // 30 pointers
  uint32_t repeat_pointers_address() const { return address + 24; }
  template <typename T = ActionRepeat>
  T repeat_pointers() const { return T(base, data()->repeat_pointers); }
  void set_repeat_pointers(const View& w) const { const uint32_t v = w.address; data()->repeat_pointers = v; }
  void set_repeat_pointers(uint32_t v) const { data()->repeat_pointers = v; }
  uint32_t repeat_pointers_bits() const { return data()->repeat_pointers; }
  void set_repeat_pointers_bits(uint32_t v) const { data()->repeat_pointers = v; }
  // 30 ActionRepeats (16 bytes): the first's vtable
  uint32_t repeats_address() const { return address + 144; }
  uint32_t repeats() const { return data()->repeats; }
  void set_repeats(uint32_t v) const { data()->repeats = v; }
  uint32_t repeats_bits() const { return data()->repeats; }
  void set_repeats_bits(uint32_t v) const { data()->repeats = v; }
  // the analog stage (an ActionMapperAnalog, mapper + 1072): its first raw value
  uint32_t analog_address() const { return address + 624; }
  uint16_t analog() const { return data()->analog; }
  void set_analog(uint16_t v) const { data()->analog = v; }
  uint16_t analog_bits() const { return data()->analog; }
  void set_analog_bits(uint16_t v) const { data()->analog = v; }
  // the analog stage's 30 floats (its +60)
  uint32_t analog_values_address() const { return address + 684; }
  float analog_values() const { return data()->analog_values; }
  void set_analog_values(float v) const { data()->analog_values = v; }
  uint32_t analog_values_bits() const { return std::bit_cast<uint32_t>(data()->analog_values); }
  void set_analog_values_bits(uint32_t v) const { data()->analog_values = std::bit_cast<float>(v); }
  // 30 ActionThresholds (20 bytes): the first's on
  uint32_t thresholds_address() const { return address + 1404; }
  float thresholds() const { return data()->thresholds; }
  void set_thresholds(float v) const { data()->thresholds = v; }
  uint32_t thresholds_bits() const { return std::bit_cast<uint32_t>(data()->thresholds); }
  void set_thresholds_bits(uint32_t v) const { data()->thresholds = std::bit_cast<float>(v); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ActionMapperAnalogData {
  uint16_t raw = 0;  // +0
  uint8_t pad_2[58] = {};
  float values = 0;  // +60
  uint8_t pad_64[116] = {};
  uint32_t curve_pointers = 0;  // +180
  uint8_t pad_184[116] = {};
  uint32_t curves = 0;  // +300
  uint8_t pad_304[476] = {};
};
#pragma pack(pop)
static_assert(sizeof(ActionMapperAnalogData) == 780);
inline constexpr memory::LayoutField kActionMapperAnalogFields[] = {{0, 2}, {60, 4}, {180, 4}, {300, 4}};
inline constexpr memory::Layout kActionMapperAnalogLayout = {kActionMapperAnalogFields, 4};

struct ActionMapperAnalog : View {
  using View::View;
  static constexpr uint32_t kSize = 780;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ActionMapperAnalogData* data() const { return static_cast<ActionMapperAnalogData*>(memory::HandleObject(address)); }
  static ActionMapperAnalog New(uint8_t* base) {
    return ActionMapperAnalog(base, memory::MakeHandle(new ActionMapperAnalogData(), sizeof(ActionMapperAnalogData), "ActionMapperAnalog", &kActionMapperAnalogLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionMapperAnalog NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionMapperAnalogData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionMapperAnalogData*>(::operator new(sizeof(ActionMapperAnalogData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionMapperAnalogData();
    return ActionMapperAnalog(base, memory::MakeHandle(p, sizeof(ActionMapperAnalogData) * count, "ActionMapperAnalog", &kActionMapperAnalogLayout, sizeof(ActionMapperAnalogData)));
  }
  ActionMapperAnalog at(uint32_t i) const { return ActionMapperAnalog(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionMapperAnalogLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperAnalogLayout, offset, width, v); }
  // 30 u16s: the raw values the stage took
  uint32_t raw_address() const { return address + 0; }
  uint16_t raw() const { return data()->raw; }
  void set_raw(uint16_t v) const { data()->raw = v; }
  uint16_t raw_bits() const { return data()->raw; }
  void set_raw_bits(uint16_t v) const { data()->raw = v; }
  // 30 floats: through each action's curve
  uint32_t values_address() const { return address + 60; }
  float values() const { return data()->values; }
  void set_values(float v) const { data()->values = v; }
  uint32_t values_bits() const { return std::bit_cast<uint32_t>(data()->values); }
  void set_values_bits(uint32_t v) const { data()->values = std::bit_cast<float>(v); }
  // 30 pointers
  uint32_t curve_pointers_address() const { return address + 180; }
  template <typename T = ActionCurve>
  T curve_pointers() const { return T(base, data()->curve_pointers); }
  void set_curve_pointers(const View& w) const { const uint32_t v = w.address; data()->curve_pointers = v; }
  void set_curve_pointers(uint32_t v) const { data()->curve_pointers = v; }
  uint32_t curve_pointers_bits() const { return data()->curve_pointers; }
  void set_curve_pointers_bits(uint32_t v) const { data()->curve_pointers = v; }
  // 30 ActionCurves (16 bytes): the first's vtable
  uint32_t curves_address() const { return address + 300; }
  uint32_t curves() const { return data()->curves; }
  void set_curves(uint32_t v) const { data()->curves = v; }
  uint32_t curves_bits() const { return data()->curves; }
  void set_curves_bits(uint32_t v) const { data()->curves = v; }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ActionThresholdData {
  float on = 0;  // +0
  float off = 0;  // +4
  uint32_t suppress_ticks = 0;  // +8
  uint32_t counter = 0;  // +12
  uint8_t state = 0;  // +16
  uint8_t pad_17[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(ActionThresholdData) == 20);
inline constexpr memory::LayoutField kActionThresholdFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kActionThresholdLayout = {kActionThresholdFields, 4};

struct ActionThreshold : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ActionThresholdData* data() const { return static_cast<ActionThresholdData*>(memory::HandleObject(address)); }
  static ActionThreshold New(uint8_t* base) {
    return ActionThreshold(base, memory::MakeHandle(new ActionThresholdData(), sizeof(ActionThresholdData), "ActionThreshold", &kActionThresholdLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionThreshold NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionThresholdData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionThresholdData*>(::operator new(sizeof(ActionThresholdData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionThresholdData();
    return ActionThreshold(base, memory::MakeHandle(p, sizeof(ActionThresholdData) * count, "ActionThreshold", &kActionThresholdLayout, sizeof(ActionThresholdData)));
  }
  ActionThreshold at(uint32_t i) const { return ActionThreshold(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionThresholdLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionThresholdLayout, offset, width, v); }
  uint32_t on_address() const { return address + 0; }
  float on() const { return data()->on; }
  void set_on(float v) const { data()->on = v; }
  uint32_t on_bits() const { return std::bit_cast<uint32_t>(data()->on); }
  void set_on_bits(uint32_t v) const { data()->on = std::bit_cast<float>(v); }
  uint32_t off_address() const { return address + 4; }
  float off() const { return data()->off; }
  void set_off(float v) const { data()->off = v; }
  uint32_t off_bits() const { return std::bit_cast<uint32_t>(data()->off); }
  void set_off_bits(uint32_t v) const { data()->off = std::bit_cast<float>(v); }
  // set into a linked action's counter when this one turns on
  uint32_t suppress_ticks_address() const { return address + 8; }
  uint32_t suppress_ticks() const { return data()->suppress_ticks; }
  void set_suppress_ticks(uint32_t v) const { data()->suppress_ticks = v; }
  uint32_t suppress_ticks_bits() const { return data()->suppress_ticks; }
  void set_suppress_ticks_bits(uint32_t v) const { data()->suppress_ticks = v; }
  // while nonzero, counts down and holds the state at 0
  uint32_t counter_address() const { return address + 12; }
  uint32_t counter() const { return data()->counter; }
  void set_counter(uint32_t v) const { data()->counter = v; }
  uint32_t counter_bits() const { return data()->counter; }
  void set_counter_bits(uint32_t v) const { data()->counter = v; }
  uint32_t state_address() const { return address + 16; }
  uint8_t state() const { return data()->state; }
  void set_state(uint8_t v) const { data()->state = v; }
  uint8_t state_bits() const { return data()->state; }
  void set_state_bits(uint8_t v) const { data()->state = v; }
};

// embedded; measure: ActionMapper::Hysteresis.r6
#pragma pack(push, 1)
struct ActionMapperConfigData {
  uint32_t kinds = 0;  // +0
  uint32_t links = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(ActionMapperConfigData) == 8);
inline constexpr memory::LayoutField kActionMapperConfigFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kActionMapperConfigLayout = {kActionMapperConfigFields, 2};

struct ActionMapperConfig : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ActionMapperConfigData* data() const { return static_cast<ActionMapperConfigData*>(memory::HandleObject(address)); }
  static ActionMapperConfig New(uint8_t* base) {
    return ActionMapperConfig(base, memory::MakeHandle(new ActionMapperConfigData(), sizeof(ActionMapperConfigData), "ActionMapperConfig", &kActionMapperConfigLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionMapperConfig NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionMapperConfigData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionMapperConfigData*>(::operator new(sizeof(ActionMapperConfigData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionMapperConfigData();
    return ActionMapperConfig(base, memory::MakeHandle(p, sizeof(ActionMapperConfigData) * count, "ActionMapperConfig", &kActionMapperConfigLayout, sizeof(ActionMapperConfigData)));
  }
  ActionMapperConfig at(uint32_t i) const { return ActionMapperConfig(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionMapperConfigLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperConfigLayout, offset, width, v); }
  // the address of 30 kinds
  uint32_t kinds_address() const { return address + 0; }
  uint32_t kinds() const { return data()->kinds; }
  void set_kinds(uint32_t v) const { data()->kinds = v; }
  uint32_t kinds_bits() const { return data()->kinds; }
  void set_kinds_bits(uint32_t v) const { data()->kinds = v; }
  // the address of 30 links (30 none)
  uint32_t links_address() const { return address + 4; }
  uint32_t links() const { return data()->links; }
  void set_links(uint32_t v) const { data()->links = v; }
  uint32_t links_bits() const { return data()->links; }
  void set_links_bits(uint32_t v) const { data()->links = v; }
};

// embedded; measure: ActionRepeat
#pragma pack(push, 1)
struct ActionRepeatData {
  uint32_t vtable = 0;  // +0
  uint32_t count = 0;  // +4
  uint32_t delay = 0;  // +8
  uint32_t period = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(ActionRepeatData) == 16);
inline constexpr memory::LayoutField kActionRepeatFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kActionRepeatLayout = {kActionRepeatFields, 4};

struct ActionRepeat : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82007F68;
  ActionRepeatData* data() const { return static_cast<ActionRepeatData*>(memory::HandleObject(address)); }
  static ActionRepeat New(uint8_t* base) {
    return ActionRepeat(base, memory::MakeHandle(new ActionRepeatData(), sizeof(ActionRepeatData), "ActionRepeat", &kActionRepeatLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionRepeat NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionRepeatData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionRepeatData*>(::operator new(sizeof(ActionRepeatData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionRepeatData();
    return ActionRepeat(base, memory::MakeHandle(p, sizeof(ActionRepeatData) * count, "ActionRepeat", &kActionRepeatLayout, sizeof(ActionRepeatData)));
  }
  ActionRepeat at(uint32_t i) const { return ActionRepeat(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionRepeatLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionRepeatLayout, offset, width, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = ActionObjectVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  // ticks held
  uint32_t count_address() const { return address + 4; }
  uint32_t count() const { return data()->count; }
  void set_count(uint32_t v) const { data()->count = v; }
  uint32_t count_bits() const { return data()->count; }
  void set_count_bits(uint32_t v) const { data()->count = v; }
  uint32_t delay_address() const { return address + 8; }
  uint32_t delay() const { return data()->delay; }
  void set_delay(uint32_t v) const { data()->delay = v; }
  uint32_t delay_bits() const { return data()->delay; }
  void set_delay_bits(uint32_t v) const { data()->delay = v; }
  uint32_t period_address() const { return address + 12; }
  uint32_t period() const { return data()->period; }
  void set_period(uint32_t v) const { data()->period = v; }
  uint32_t period_bits() const { return data()->period; }
  void set_period_bits(uint32_t v) const { data()->period = v; }
};

// embedded; measure: ActionCurve
#pragma pack(push, 1)
struct ActionCurveData {
  uint32_t vtable = 0;  // +0
  float center = 0;  // +4
  float dead_zone = 0;  // +8
  float maximum = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(ActionCurveData) == 16);
inline constexpr memory::LayoutField kActionCurveFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kActionCurveLayout = {kActionCurveFields, 4};

struct ActionCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82007F70;
  ActionCurveData* data() const { return static_cast<ActionCurveData*>(memory::HandleObject(address)); }
  static ActionCurve New(uint8_t* base) {
    return ActionCurve(base, memory::MakeHandle(new ActionCurveData(), sizeof(ActionCurveData), "ActionCurve", &kActionCurveLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionCurve NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionCurveData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionCurveData*>(::operator new(sizeof(ActionCurveData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionCurveData();
    return ActionCurve(base, memory::MakeHandle(p, sizeof(ActionCurveData) * count, "ActionCurve", &kActionCurveLayout, sizeof(ActionCurveData)));
  }
  ActionCurve at(uint32_t i) const { return ActionCurve(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionCurveLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionCurveLayout, offset, width, v); }
  uint32_t vtable_address() const { return address + 0; }
  template <typename T = ActionObjectVtable>
  T vtable() const { return T(base, data()->vtable); }
  void set_vtable(const View& w) const { const uint32_t v = w.address; data()->vtable = v; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  uint32_t center_address() const { return address + 4; }
  float center() const { return data()->center; }
  void set_center(float v) const { data()->center = v; }
  uint32_t center_bits() const { return std::bit_cast<uint32_t>(data()->center); }
  void set_center_bits(uint32_t v) const { data()->center = std::bit_cast<float>(v); }
  uint32_t dead_zone_address() const { return address + 8; }
  float dead_zone() const { return data()->dead_zone; }
  void set_dead_zone(float v) const { data()->dead_zone = v; }
  uint32_t dead_zone_bits() const { return std::bit_cast<uint32_t>(data()->dead_zone); }
  void set_dead_zone_bits(uint32_t v) const { data()->dead_zone = std::bit_cast<float>(v); }
  uint32_t maximum_address() const { return address + 12; }
  float maximum() const { return data()->maximum; }
  void set_maximum(float v) const { data()->maximum = v; }
  uint32_t maximum_bits() const { return std::bit_cast<uint32_t>(data()->maximum); }
  void set_maximum_bits(uint32_t v) const { data()->maximum = std::bit_cast<float>(v); }
};

// image; measure: ActionMapper@+448.+144, ActionMapper@+448@+624.+300, ActionMapper::ResetWords.r3.+24.+0, ActionMapper@+448.+24.+0, ActionMapper::ResetAnalog.r3.+180.+0
struct ActionObjectVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // slot 0: sub_8243DC88 (repeats: the count 0) or sub_82106540 (curves: nothing)
  uint32_t reset() const { return guest::LoadU32(base, address + 0); }
  void set_reset(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t reset_address() const { return address + 0; }
  // slot 1: ActionRepeat::Update sub_8210B568 or ActionCurve::Apply sub_8210B5B8
  uint32_t update() const { return guest::LoadU32(base, address + 4); }
  void set_update(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t update_address() const { return address + 4; }
};

// embedded; measure: ActionMapper.+2880
#pragma pack(push, 1)
struct ActionPadValuesData {
  float left_x = 0;  // +0
  float left_y = 0;  // +4
  float left_trigger = 0;  // +8
  float unknown_12 = 0;  // +12
  float right_x = 0;  // +16
  float right_y = 0;  // +20
  float right_trigger = 0;  // +24
  uint8_t pad_28[132] = {};
};
#pragma pack(pop)
static_assert(sizeof(ActionPadValuesData) == 160);
inline constexpr memory::LayoutField kActionPadValuesFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}};
inline constexpr memory::Layout kActionPadValuesLayout = {kActionPadValuesFields, 7};

struct ActionPadValues : View {
  using View::View;
  static constexpr uint32_t kSize = 160;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ActionPadValuesData* data() const { return static_cast<ActionPadValuesData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ActionPadValues New(uint8_t* base) {
    return ActionPadValues(base, memory::MakeHandle(new ActionPadValuesData(), sizeof(ActionPadValuesData), "ActionPadValues", &kActionPadValuesLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionPadValues NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionPadValuesData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionPadValuesData*>(::operator new(sizeof(ActionPadValuesData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionPadValuesData();
    return ActionPadValues(base, memory::MakeHandle(p, sizeof(ActionPadValuesData) * count, "ActionPadValues", &kActionPadValuesLayout, sizeof(ActionPadValuesData)));
  }
  ActionPadValues at(uint32_t i) const { return ActionPadValues(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionPadValuesLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionPadValuesLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t left_x_address() const { return address + 0; }
  float left_x() const { return on_host() ? float(data()->left_x) : float(guest::LoadFloat(base, address + 0)); }
  void set_left_x(float v) const { if (on_host()) data()->left_x = v; else guest::StoreFloat(base, address + 0, v); }
  uint32_t left_x_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->left_x) : guest::LoadU32(base, address + 0); }
  void set_left_x_bits(uint32_t v) const { if (on_host()) data()->left_x = std::bit_cast<float>(v); else guest::StoreU32(base, address + 0, v); }
  uint32_t left_y_address() const { return address + 4; }
  float left_y() const { return on_host() ? float(data()->left_y) : float(guest::LoadFloat(base, address + 4)); }
  void set_left_y(float v) const { if (on_host()) data()->left_y = v; else guest::StoreFloat(base, address + 4, v); }
  uint32_t left_y_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->left_y) : guest::LoadU32(base, address + 4); }
  void set_left_y_bits(uint32_t v) const { if (on_host()) data()->left_y = std::bit_cast<float>(v); else guest::StoreU32(base, address + 4, v); }
  uint32_t left_trigger_address() const { return address + 8; }
  float left_trigger() const { return on_host() ? float(data()->left_trigger) : float(guest::LoadFloat(base, address + 8)); }
  void set_left_trigger(float v) const { if (on_host()) data()->left_trigger = v; else guest::StoreFloat(base, address + 8, v); }
  uint32_t left_trigger_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->left_trigger) : guest::LoadU32(base, address + 8); }
  void set_left_trigger_bits(uint32_t v) const { if (on_host()) data()->left_trigger = std::bit_cast<float>(v); else guest::StoreU32(base, address + 8, v); }
  // 0
  uint32_t unknown_12_address() const { return address + 12; }
  float unknown_12() const { return on_host() ? float(data()->unknown_12) : float(guest::LoadFloat(base, address + 12)); }
  void set_unknown_12(float v) const { if (on_host()) data()->unknown_12 = v; else guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->unknown_12) : guest::LoadU32(base, address + 12); }
  void set_unknown_12_bits(uint32_t v) const { if (on_host()) data()->unknown_12 = std::bit_cast<float>(v); else guest::StoreU32(base, address + 12, v); }
  uint32_t right_x_address() const { return address + 16; }
  float right_x() const { return on_host() ? float(data()->right_x) : float(guest::LoadFloat(base, address + 16)); }
  void set_right_x(float v) const { if (on_host()) data()->right_x = v; else guest::StoreFloat(base, address + 16, v); }
  uint32_t right_x_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->right_x) : guest::LoadU32(base, address + 16); }
  void set_right_x_bits(uint32_t v) const { if (on_host()) data()->right_x = std::bit_cast<float>(v); else guest::StoreU32(base, address + 16, v); }
  uint32_t right_y_address() const { return address + 20; }
  float right_y() const { return on_host() ? float(data()->right_y) : float(guest::LoadFloat(base, address + 20)); }
  void set_right_y(float v) const { if (on_host()) data()->right_y = v; else guest::StoreFloat(base, address + 20, v); }
  uint32_t right_y_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->right_y) : guest::LoadU32(base, address + 20); }
  void set_right_y_bits(uint32_t v) const { if (on_host()) data()->right_y = std::bit_cast<float>(v); else guest::StoreU32(base, address + 20, v); }
  uint32_t right_trigger_address() const { return address + 24; }
  float right_trigger() const { return on_host() ? float(data()->right_trigger) : float(guest::LoadFloat(base, address + 24)); }
  void set_right_trigger(float v) const { if (on_host()) data()->right_trigger = v; else guest::StoreFloat(base, address + 24, v); }
  uint32_t right_trigger_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->right_trigger) : guest::LoadU32(base, address + 24); }
  void set_right_trigger_bits(uint32_t v) const { if (on_host()) data()->right_trigger = std::bit_cast<float>(v); else guest::StoreU32(base, address + 24, v); }
};

// allocated; measure: ActionMapper.+2892, ActionMapper::Bind.r4
struct ActionMapperInputManager : View {
  using View::View;
  static constexpr uint32_t kSize = 3984;
  template <typename T = ActionMapperInputManagerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: ActionMapper.+2892.+0, ActionMapper::Bind.r4.+0
struct ActionMapperInputManagerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // slot 2: InputManager::GetLogicalPad 0x82098CD0
  uint32_t get_logical_pad() const { return guest::LoadU32(base, address + 8); }
  void set_get_logical_pad(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t get_logical_pad_address() const { return address + 8; }
  // slot 3: InputManager::AddListener 0x82099430
  uint32_t add_listener() const { return guest::LoadU32(base, address + 12); }
  void set_add_listener(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t add_listener_address() const { return address + 12; }
};

// allocated; measure: ClientList, InputClientList
#pragma pack(push, 1)
struct InputClientListData {
  uint32_t vtable = 0;  // +0
  uint32_t first = 0;  // +4
  uint32_t last = 0;  // +8
  uint32_t added_counts = 0;  // +12
  uint32_t added_count_1 = 0;  // +16
  uint32_t added_count_2 = 0;  // +20
  uint32_t bound_counts = 0;  // +24
  uint32_t bound_count_1 = 0;  // +28
  uint32_t bound_count_2 = 0;  // +32
  uint32_t manager = 0;  // +36
  uint32_t unknown_40 = 0;  // +40
  uint32_t unknown_44 = 0;  // +44
  uint32_t unknown_48 = 0;  // +48
};
#pragma pack(pop)
static_assert(sizeof(InputClientListData) == 52);
inline constexpr memory::LayoutField kInputClientListFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}};
inline constexpr memory::Layout kInputClientListLayout = {kInputClientListFields, 13};

struct InputClientList : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82007F18;
  InputClientListData* data() const { return static_cast<InputClientListData*>(memory::HandleObject(address)); }
  static InputClientList New(uint8_t* base) {
    return InputClientList(base, memory::MakeHandle(new InputClientListData(), sizeof(InputClientListData), "InputClientList", &kInputClientListLayout));
  }
  template <uint32_t Stride = kSize>
  static InputClientList NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(InputClientListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<InputClientListData*>(::operator new(sizeof(InputClientListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) InputClientListData();
    return InputClientList(base, memory::MakeHandle(p, sizeof(InputClientListData) * count, "InputClientList", &kInputClientListLayout, sizeof(InputClientListData)));
  }
  InputClientList at(uint32_t i) const { return InputClientList(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kInputClientListLayout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kInputClientListLayout, offset, width, v); }
  // 0x820713C0, then 0x82007F18
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable() const { return data()->vtable; }
  void set_vtable(uint32_t v) const { data()->vtable = v; }
  uint32_t vtable_bits() const { return data()->vtable; }
  void set_vtable_bits(uint32_t v) const { data()->vtable = v; }
  uint32_t first_address() const { return address + 4; }
  template <typename T = InputClientNode>
  T first() const { return T(base, data()->first); }
  void set_first(const View& w) const { const uint32_t v = w.address; data()->first = v; }
  void set_first(uint32_t v) const { data()->first = v; }
  uint32_t first_bits() const { return data()->first; }
  void set_first_bits(uint32_t v) const { data()->first = v; }
  uint32_t last_address() const { return address + 8; }
  template <typename T = InputClientNode>
  T last() const { return T(base, data()->last); }
  void set_last(const View& w) const { const uint32_t v = w.address; data()->last = v; }
  void set_last(uint32_t v) const { data()->last = v; }
  uint32_t last_bits() const { return data()->last; }
  void set_last_bits(uint32_t v) const { data()->last = v; }
  // 3 words: the nodes added of types 0-2
  uint32_t added_counts_address() const { return address + 12; }
  uint32_t added_counts() const { return data()->added_counts; }
  void set_added_counts(uint32_t v) const { data()->added_counts = v; }
  uint32_t added_counts_bits() const { return data()->added_counts; }
  void set_added_counts_bits(uint32_t v) const { data()->added_counts = v; }
  uint32_t added_count_1_address() const { return address + 16; }
  uint32_t added_count_1() const { return data()->added_count_1; }
  void set_added_count_1(uint32_t v) const { data()->added_count_1 = v; }
  uint32_t added_count_1_bits() const { return data()->added_count_1; }
  void set_added_count_1_bits(uint32_t v) const { data()->added_count_1 = v; }
  // the mappers
  uint32_t added_count_2_address() const { return address + 20; }
  uint32_t added_count_2() const { return data()->added_count_2; }
  void set_added_count_2(uint32_t v) const { data()->added_count_2 = v; }
  uint32_t added_count_2_bits() const { return data()->added_count_2; }
  void set_added_count_2_bits(uint32_t v) const { data()->added_count_2 = v; }
  // 3 words: those bound, types 0-2
  uint32_t bound_counts_address() const { return address + 24; }
  uint32_t bound_counts() const { return data()->bound_counts; }
  void set_bound_counts(uint32_t v) const { data()->bound_counts = v; }
  uint32_t bound_counts_bits() const { return data()->bound_counts; }
  void set_bound_counts_bits(uint32_t v) const { data()->bound_counts = v; }
  uint32_t bound_count_1_address() const { return address + 28; }
  uint32_t bound_count_1() const { return data()->bound_count_1; }
  void set_bound_count_1(uint32_t v) const { data()->bound_count_1 = v; }
  uint32_t bound_count_1_bits() const { return data()->bound_count_1; }
  void set_bound_count_1_bits(uint32_t v) const { data()->bound_count_1 = v; }
  // the mappers bound
  uint32_t bound_count_2_address() const { return address + 32; }
  uint32_t bound_count_2() const { return data()->bound_count_2; }
  void set_bound_count_2(uint32_t v) const { data()->bound_count_2 = v; }
  uint32_t bound_count_2_bits() const { return data()->bound_count_2; }
  void set_bound_count_2_bits(uint32_t v) const { data()->bound_count_2 = v; }
  // the object ActionMappers::Init was given
  uint32_t manager_address() const { return address + 36; }
  template <typename T = ActionMappersOwner>
  T manager() const { return T(base, data()->manager); }
  void set_manager(const View& w) const { const uint32_t v = w.address; data()->manager = v; }
  void set_manager(uint32_t v) const { data()->manager = v; }
  uint32_t manager_bits() const { return data()->manager; }
  void set_manager_bits(uint32_t v) const { data()->manager = v; }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_40() const { return data()->unknown_40; }
  void set_unknown_40(uint32_t v) const { data()->unknown_40 = v; }
  uint32_t unknown_40_bits() const { return data()->unknown_40; }
  void set_unknown_40_bits(uint32_t v) const { data()->unknown_40 = v; }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_44() const { return data()->unknown_44; }
  void set_unknown_44(uint32_t v) const { data()->unknown_44 = v; }
  uint32_t unknown_44_bits() const { return data()->unknown_44; }
  void set_unknown_44_bits(uint32_t v) const { data()->unknown_44 = v; }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_48() const { return data()->unknown_48; }
  void set_unknown_48(uint32_t v) const { data()->unknown_48 = v; }
  uint32_t unknown_48_bits() const { return data()->unknown_48; }
  void set_unknown_48_bits(uint32_t v) const { data()->unknown_48 = v; }
};

// embedded; measure: ClientNode, InputClient
struct InputClientNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr uint32_t kVtable = 0x82007ED8;
  // 0x82071380, then 0x82007ED8, then the derived class's
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 2 for an action mapper
  uint32_t type() const { return guest::LoadU32(base, address + 4); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t type_address() const { return address + 4; }
  template <typename T = InputClientNode>
  T next() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_address() const { return address + 8; }
};

// allocated; measure: InputClientList.+36
struct ActionMappersOwner : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = ActionMappersOwnerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: InputClientList.+36.+0
struct ActionMappersOwnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // probably: what InputClientList::BindAll binds the mappers to
  uint32_t get_input_manager() const { return guest::LoadU32(base, address + 36); }
  void set_get_input_manager(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t get_input_manager_address() const { return address + 36; }
};

// embedded; measure: InputProcessor::AllHeld.r4, InputProcessor::TestHeld.r4
struct ActionButtonMask : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t mask() const { return guest::LoadU32(base, address + 0); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t mask_address() const { return address + 0; }
};

// image; measure: -
struct ActionSettings : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  float on() const { return guest::LoadFloat(base, address + 0); }
  void set_on(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t on_address() const { return address + 0; }
  float off() const { return guest::LoadFloat(base, address + 4); }
  void set_off(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t off_address() const { return address + 4; }
  uint32_t suppress_ticks() const { return guest::LoadU32(base, address + 8); }
  void set_suppress_ticks(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t suppress_ticks_address() const { return address + 8; }
  uint32_t repeat_delay() const { return guest::LoadU32(base, address + 12); }
  void set_repeat_delay(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t repeat_delay_address() const { return address + 12; }
  uint32_t repeat_period() const { return guest::LoadU32(base, address + 16); }
  void set_repeat_period(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t repeat_period_address() const { return address + 16; }
  float curve_center() const { return guest::LoadFloat(base, address + 20); }
  void set_curve_center(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t curve_center_address() const { return address + 20; }
  float curve_dead_zone() const { return guest::LoadFloat(base, address + 24); }
  void set_curve_dead_zone(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t curve_dead_zone_address() const { return address + 24; }
  float curve_maximum() const { return guest::LoadFloat(base, address + 28); }
  void set_curve_maximum(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t curve_maximum_address() const { return address + 28; }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ActionWordData {
  uint32_t value = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(ActionWordData) == 4);
inline constexpr memory::LayoutField kActionWordFields[] = {{0, 4}};
inline constexpr memory::Layout kActionWordLayout = {kActionWordFields, 1};

struct ActionWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ActionWordData* data() const { return static_cast<ActionWordData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ActionWord New(uint8_t* base) {
    return ActionWord(base, memory::MakeHandle(new ActionWordData(), sizeof(ActionWordData), "ActionWord", &kActionWordLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionWord NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionWordData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionWordData*>(::operator new(sizeof(ActionWordData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionWordData();
    return ActionWord(base, memory::MakeHandle(p, sizeof(ActionWordData) * count, "ActionWord", &kActionWordLayout, sizeof(ActionWordData)));
  }
  ActionWord at(uint32_t i) const { return ActionWord(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionWordLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionWordLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t value_address() const { return address + 0; }
  uint32_t value() const { return on_host() ? uint32_t(data()->value) : uint32_t(guest::LoadU32(base, address + 0)); }
  void set_value(uint32_t v) const { if (on_host()) data()->value = v; else guest::StoreU32(base, address + 0, v); }
  uint32_t value_bits() const { return on_host() ? data()->value : guest::LoadU32(base, address + 0); }
  void set_value_bits(uint32_t v) const { if (on_host()) data()->value = v; else guest::StoreU32(base, address + 0, v); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ActionHalfData {
  uint16_t value = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(ActionHalfData) == 2);
inline constexpr memory::LayoutField kActionHalfFields[] = {{0, 2}};
inline constexpr memory::Layout kActionHalfLayout = {kActionHalfFields, 1};

struct ActionHalf : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ActionHalfData* data() const { return static_cast<ActionHalfData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ActionHalf New(uint8_t* base) {
    return ActionHalf(base, memory::MakeHandle(new ActionHalfData(), sizeof(ActionHalfData), "ActionHalf", &kActionHalfLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionHalf NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionHalfData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionHalfData*>(::operator new(sizeof(ActionHalfData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionHalfData();
    return ActionHalf(base, memory::MakeHandle(p, sizeof(ActionHalfData) * count, "ActionHalf", &kActionHalfLayout, sizeof(ActionHalfData)));
  }
  ActionHalf at(uint32_t i) const { return ActionHalf(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionHalfLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionHalfLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t value_address() const { return address + 0; }
  uint16_t value() const { return on_host() ? uint16_t(data()->value) : uint16_t(guest::LoadU16(base, address + 0)); }
  void set_value(uint16_t v) const { if (on_host()) data()->value = v; else guest::StoreU16(base, address + 0, v); }
  uint16_t value_bits() const { return on_host() ? data()->value : guest::LoadU16(base, address + 0); }
  void set_value_bits(uint16_t v) const { if (on_host()) data()->value = v; else guest::StoreU16(base, address + 0, v); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct ActionFloatData {
  float value = 0;  // +0
};
#pragma pack(pop)
static_assert(sizeof(ActionFloatData) == 4);
inline constexpr memory::LayoutField kActionFloatFields[] = {{0, 4}};
inline constexpr memory::Layout kActionFloatLayout = {kActionFloatFields, 1};

struct ActionFloat : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  ActionFloatData* data() const { return static_cast<ActionFloatData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static ActionFloat New(uint8_t* base) {
    return ActionFloat(base, memory::MakeHandle(new ActionFloatData(), sizeof(ActionFloatData), "ActionFloat", &kActionFloatLayout));
  }
  template <uint32_t Stride = kSize>
  static ActionFloat NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionFloatData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionFloatData*>(::operator new(sizeof(ActionFloatData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionFloatData();
    return ActionFloat(base, memory::MakeHandle(p, sizeof(ActionFloatData) * count, "ActionFloat", &kActionFloatLayout, sizeof(ActionFloatData)));
  }
  ActionFloat at(uint32_t i) const { return ActionFloat(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (on_host()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionFloatLayout, offset, width); return width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (on_host()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionFloatLayout, offset, width, v); return; } if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t value_address() const { return address + 0; }
  float value() const { return on_host() ? float(data()->value) : float(guest::LoadFloat(base, address + 0)); }
  void set_value(float v) const { if (on_host()) data()->value = v; else guest::StoreFloat(base, address + 0, v); }
  uint32_t value_bits() const { return on_host() ? std::bit_cast<uint32_t>(data()->value) : guest::LoadU32(base, address + 0); }
  void set_value_bits(uint32_t v) const { if (on_host()) data()->value = std::bit_cast<float>(v); else guest::StoreU32(base, address + 0, v); }
};

// embedded; measure: ActionMapper::Words.r4@+8
#pragma pack(push, 1)
struct ActionMapperConfigAt8Data {
  uint32_t unknown_0 = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(ActionMapperConfigAt8Data) == 8);
inline constexpr memory::LayoutField kActionMapperConfigAt8Fields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kActionMapperConfigAt8Layout = {kActionMapperConfigAt8Fields, 2};

struct ActionMapperConfigAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ActionMapperConfigAt8Data* data() const { return static_cast<ActionMapperConfigAt8Data*>(memory::HandleObject(address)); }
  static ActionMapperConfigAt8 New(uint8_t* base) {
    return ActionMapperConfigAt8(base, memory::MakeHandle(new ActionMapperConfigAt8Data(), sizeof(ActionMapperConfigAt8Data), "ActionMapperConfigAt8", &kActionMapperConfigAt8Layout));
  }
  template <uint32_t Stride = kSize>
  static ActionMapperConfigAt8 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionMapperConfigAt8Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionMapperConfigAt8Data*>(::operator new(sizeof(ActionMapperConfigAt8Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionMapperConfigAt8Data();
    return ActionMapperConfigAt8(base, memory::MakeHandle(p, sizeof(ActionMapperConfigAt8Data) * count, "ActionMapperConfigAt8", &kActionMapperConfigAt8Layout, sizeof(ActionMapperConfigAt8Data)));
  }
  ActionMapperConfigAt8 at(uint32_t i) const { return ActionMapperConfigAt8(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionMapperConfigAt8Layout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperConfigAt8Layout, offset, width, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0() const { return data()->unknown_0; }
  void set_unknown_0(uint32_t v) const { data()->unknown_0 = v; }
  uint32_t unknown_0_bits() const { return data()->unknown_0; }
  void set_unknown_0_bits(uint32_t v) const { data()->unknown_0 = v; }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
};

// embedded; measure: ActionMapper@+448
#pragma pack(push, 1)
struct ActionMapperAt448Data {
  uint32_t unknown_0 = 0;  // +0
  uint32_t unknown_4 = 0;  // +4
  uint32_t unknown_8 = 0;  // +8
  uint32_t unknown_12 = 0;  // +12
  uint32_t state_fired = 0;  // +16
  uint32_t state_pending = 0;  // +20
  uint32_t state_repeat_pointers = 0;  // +24
  uint8_t pad_28[116] = {};
  uint32_t state_repeats = 0;  // +144
  uint8_t pad_148[4] = {};
  uint32_t state_repeat_delay = 0;  // +152
  uint32_t state_repeat_period = 0;  // +156
  uint8_t pad_160[524] = {};
  float state_analog_values = 0;  // +684
  uint8_t pad_688[716] = {};
  float threshold_on = 0;  // +1404
  float threshold_off = 0;  // +1408
  uint32_t threshold_suppress_ticks = 0;  // +1412
  uint32_t threshold_counter = 0;  // +1416
  uint8_t threshold_state = 0;  // +1420
};
#pragma pack(pop)
static_assert(sizeof(ActionMapperAt448Data) == 1421);
inline constexpr memory::LayoutField kActionMapperAt448Fields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {144, 4}, {152, 4}, {156, 4}, {684, 4}, {1404, 4}, {1408, 4}, {1412, 4}, {1416, 4}};
inline constexpr memory::Layout kActionMapperAt448Layout = {kActionMapperAt448Fields, 15};

struct ActionMapperAt448 : View {
  using View::View;
  static constexpr uint32_t kSize = 1421;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ActionMapperAt448Data* data() const { return static_cast<ActionMapperAt448Data*>(memory::HandleObject(address)); }
  static ActionMapperAt448 New(uint8_t* base) {
    return ActionMapperAt448(base, memory::MakeHandle(new ActionMapperAt448Data(), sizeof(ActionMapperAt448Data), "ActionMapperAt448", &kActionMapperAt448Layout));
  }
  template <uint32_t Stride = kSize>
  static ActionMapperAt448 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionMapperAt448Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionMapperAt448Data*>(::operator new(sizeof(ActionMapperAt448Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionMapperAt448Data();
    return ActionMapperAt448(base, memory::MakeHandle(p, sizeof(ActionMapperAt448Data) * count, "ActionMapperAt448", &kActionMapperAt448Layout, sizeof(ActionMapperAt448Data)));
  }
  ActionMapperAt448 at(uint32_t i) const { return ActionMapperAt448(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionMapperAt448Layout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperAt448Layout, offset, width, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_0() const { return data()->unknown_0; }
  void set_unknown_0(uint32_t v) const { data()->unknown_0 = v; }
  uint32_t unknown_0_bits() const { return data()->unknown_0; }
  void set_unknown_0_bits(uint32_t v) const { data()->unknown_0 = v; }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4() const { return data()->unknown_4; }
  void set_unknown_4(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_4_bits() const { return data()->unknown_4; }
  void set_unknown_4_bits(uint32_t v) const { data()->unknown_4 = v; }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_8() const { return data()->unknown_8; }
  void set_unknown_8(uint32_t v) const { data()->unknown_8 = v; }
  uint32_t unknown_8_bits() const { return data()->unknown_8; }
  void set_unknown_8_bits(uint32_t v) const { data()->unknown_8 = v; }
  uint32_t unknown_12_address() const { return address + 12; }
  uint32_t unknown_12() const { return data()->unknown_12; }
  void set_unknown_12(uint32_t v) const { data()->unknown_12 = v; }
  uint32_t unknown_12_bits() const { return data()->unknown_12; }
  void set_unknown_12_bits(uint32_t v) const { data()->unknown_12 = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): ActionMapperState's fired (mapper + 464)
  uint32_t state_fired_address() const { return address + 16; }
  uint32_t state_fired() const { return data()->state_fired; }
  void set_state_fired(uint32_t v) const { data()->state_fired = v; }
  uint32_t state_fired_bits() const { return data()->state_fired; }
  void set_state_fired_bits(uint32_t v) const { data()->state_fired = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): ActionMapperState's pending (mapper + 468)
  uint32_t state_pending_address() const { return address + 20; }
  uint32_t state_pending() const { return data()->state_pending; }
  void set_state_pending(uint32_t v) const { data()->state_pending = v; }
  uint32_t state_pending_bits() const { return data()->state_pending; }
  void set_state_pending_bits(uint32_t v) const { data()->state_pending = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): ActionMapperState's repeat pointers (mapper + 472)
  uint32_t state_repeat_pointers_address() const { return address + 24; }
  uint32_t state_repeat_pointers() const { return data()->state_repeat_pointers; }
  void set_state_repeat_pointers(uint32_t v) const { data()->state_repeat_pointers = v; }
  uint32_t state_repeat_pointers_bits() const { return data()->state_repeat_pointers; }
  void set_state_repeat_pointers_bits(uint32_t v) const { data()->state_repeat_pointers = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): ActionMapperState's first repeat's vtable (mapper + 592)
  uint32_t state_repeats_address() const { return address + 144; }
  uint32_t state_repeats() const { return data()->state_repeats; }
  void set_state_repeats(uint32_t v) const { data()->state_repeats = v; }
  uint32_t state_repeats_bits() const { return data()->state_repeats; }
  void set_state_repeats_bits(uint32_t v) const { data()->state_repeats = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first repeat's delay (mapper + 600)
  uint32_t state_repeat_delay_address() const { return address + 152; }
  uint32_t state_repeat_delay() const { return data()->state_repeat_delay; }
  void set_state_repeat_delay(uint32_t v) const { data()->state_repeat_delay = v; }
  uint32_t state_repeat_delay_bits() const { return data()->state_repeat_delay; }
  void set_state_repeat_delay_bits(uint32_t v) const { data()->state_repeat_delay = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first repeat's period (mapper + 604)
  uint32_t state_repeat_period_address() const { return address + 156; }
  uint32_t state_repeat_period() const { return data()->state_repeat_period; }
  void set_state_repeat_period(uint32_t v) const { data()->state_repeat_period = v; }
  uint32_t state_repeat_period_bits() const { return data()->state_repeat_period; }
  void set_state_repeat_period_bits(uint32_t v) const { data()->state_repeat_period = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): ActionMapperState +684, the analog stage's values (mapper + 1132)
  uint32_t state_analog_values_address() const { return address + 684; }
  float state_analog_values() const { return data()->state_analog_values; }
  void set_state_analog_values(float v) const { data()->state_analog_values = v; }
  uint32_t state_analog_values_bits() const { return std::bit_cast<uint32_t>(data()->state_analog_values); }
  void set_state_analog_values_bits(uint32_t v) const { data()->state_analog_values = std::bit_cast<float>(v); }
  // the owner's fault_ field before the measure's fixes (2026-11-22): ActionMapperState +1404, the first threshold's on (mapper + 1852)
  uint32_t threshold_on_address() const { return address + 1404; }
  float threshold_on() const { return data()->threshold_on; }
  void set_threshold_on(float v) const { data()->threshold_on = v; }
  uint32_t threshold_on_bits() const { return std::bit_cast<uint32_t>(data()->threshold_on); }
  void set_threshold_on_bits(uint32_t v) const { data()->threshold_on = std::bit_cast<float>(v); }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first threshold's off (mapper + 1856)
  uint32_t threshold_off_address() const { return address + 1408; }
  float threshold_off() const { return data()->threshold_off; }
  void set_threshold_off(float v) const { data()->threshold_off = v; }
  uint32_t threshold_off_bits() const { return std::bit_cast<uint32_t>(data()->threshold_off); }
  void set_threshold_off_bits(uint32_t v) const { data()->threshold_off = std::bit_cast<float>(v); }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first threshold's suppress ticks (mapper + 1860)
  uint32_t threshold_suppress_ticks_address() const { return address + 1412; }
  uint32_t threshold_suppress_ticks() const { return data()->threshold_suppress_ticks; }
  void set_threshold_suppress_ticks(uint32_t v) const { data()->threshold_suppress_ticks = v; }
  uint32_t threshold_suppress_ticks_bits() const { return data()->threshold_suppress_ticks; }
  void set_threshold_suppress_ticks_bits(uint32_t v) const { data()->threshold_suppress_ticks = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first threshold's counter (mapper + 1864)
  uint32_t threshold_counter_address() const { return address + 1416; }
  uint32_t threshold_counter() const { return data()->threshold_counter; }
  void set_threshold_counter(uint32_t v) const { data()->threshold_counter = v; }
  uint32_t threshold_counter_bits() const { return data()->threshold_counter; }
  void set_threshold_counter_bits(uint32_t v) const { data()->threshold_counter = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first threshold's state (mapper + 1868)
  uint32_t threshold_state_address() const { return address + 1420; }
  uint8_t threshold_state() const { return data()->threshold_state; }
  void set_threshold_state(uint8_t v) const { data()->threshold_state = v; }
  uint8_t threshold_state_bits() const { return data()->threshold_state; }
  void set_threshold_state_bits(uint8_t v) const { data()->threshold_state = v; }
};

// embedded; measure: ActionMapper@+448@+624
#pragma pack(push, 1)
struct ActionMapperAt624Data {
  uint16_t analog_raw = 0;  // +0
  uint8_t pad_2[58] = {};
  float analog_values = 0;  // +60
  uint8_t pad_64[116] = {};
  uint32_t analog_curve_pointers = 0;  // +180
  uint8_t pad_184[116] = {};
  uint32_t analog_curves = 0;  // +300
  float analog_curve_center = 0;  // +304
  float analog_curve_dead_zone = 0;  // +308
  float analog_curve_maximum = 0;  // +312
};
#pragma pack(pop)
static_assert(sizeof(ActionMapperAt624Data) == 316);
inline constexpr memory::LayoutField kActionMapperAt624Fields[] = {{0, 2}, {60, 4}, {180, 4}, {300, 4}, {304, 4}, {308, 4}, {312, 4}};
inline constexpr memory::Layout kActionMapperAt624Layout = {kActionMapperAt624Fields, 7};

struct ActionMapperAt624 : View {
  using View::View;
  static constexpr uint32_t kSize = 316;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  ActionMapperAt624Data* data() const { return static_cast<ActionMapperAt624Data*>(memory::HandleObject(address)); }
  static ActionMapperAt624 New(uint8_t* base) {
    return ActionMapperAt624(base, memory::MakeHandle(new ActionMapperAt624Data(), sizeof(ActionMapperAt624Data), "ActionMapperAt624", &kActionMapperAt624Layout));
  }
  template <uint32_t Stride = kSize>
  static ActionMapperAt624 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(ActionMapperAt624Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<ActionMapperAt624Data*>(::operator new(sizeof(ActionMapperAt624Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) ActionMapperAt624Data();
    return ActionMapperAt624(base, memory::MakeHandle(p, sizeof(ActionMapperAt624Data) * count, "ActionMapperAt624", &kActionMapperAt624Layout, sizeof(ActionMapperAt624Data)));
  }
  ActionMapperAt624 at(uint32_t i) const { return ActionMapperAt624(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(data()), kActionMapperAt624Layout, offset, width); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { memory::StoreOverlay(reinterpret_cast<uint8_t*>(data()), kActionMapperAt624Layout, offset, width, v); }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the analog stage's raw[0] (ActionMapperAnalog +0, mapper + 1072)
  uint32_t analog_raw_address() const { return address + 0; }
  uint16_t analog_raw() const { return data()->analog_raw; }
  void set_analog_raw(uint16_t v) const { data()->analog_raw = v; }
  uint16_t analog_raw_bits() const { return data()->analog_raw; }
  void set_analog_raw_bits(uint16_t v) const { data()->analog_raw = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the analog stage's values (mapper + 1132)
  uint32_t analog_values_address() const { return address + 60; }
  float analog_values() const { return data()->analog_values; }
  void set_analog_values(float v) const { data()->analog_values = v; }
  uint32_t analog_values_bits() const { return std::bit_cast<uint32_t>(data()->analog_values); }
  void set_analog_values_bits(uint32_t v) const { data()->analog_values = std::bit_cast<float>(v); }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the analog stage's curve pointers (mapper + 1252)
  uint32_t analog_curve_pointers_address() const { return address + 180; }
  uint32_t analog_curve_pointers() const { return data()->analog_curve_pointers; }
  void set_analog_curve_pointers(uint32_t v) const { data()->analog_curve_pointers = v; }
  uint32_t analog_curve_pointers_bits() const { return data()->analog_curve_pointers; }
  void set_analog_curve_pointers_bits(uint32_t v) const { data()->analog_curve_pointers = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the analog stage's first curve's vtable (mapper + 1372)
  uint32_t analog_curves_address() const { return address + 300; }
  uint32_t analog_curves() const { return data()->analog_curves; }
  void set_analog_curves(uint32_t v) const { data()->analog_curves = v; }
  uint32_t analog_curves_bits() const { return data()->analog_curves; }
  void set_analog_curves_bits(uint32_t v) const { data()->analog_curves = v; }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first curve's center (mapper + 1376)
  uint32_t analog_curve_center_address() const { return address + 304; }
  float analog_curve_center() const { return data()->analog_curve_center; }
  void set_analog_curve_center(float v) const { data()->analog_curve_center = v; }
  uint32_t analog_curve_center_bits() const { return std::bit_cast<uint32_t>(data()->analog_curve_center); }
  void set_analog_curve_center_bits(uint32_t v) const { data()->analog_curve_center = std::bit_cast<float>(v); }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first curve's dead zone (mapper + 1380)
  uint32_t analog_curve_dead_zone_address() const { return address + 308; }
  float analog_curve_dead_zone() const { return data()->analog_curve_dead_zone; }
  void set_analog_curve_dead_zone(float v) const { data()->analog_curve_dead_zone = v; }
  uint32_t analog_curve_dead_zone_bits() const { return std::bit_cast<uint32_t>(data()->analog_curve_dead_zone); }
  void set_analog_curve_dead_zone_bits(uint32_t v) const { data()->analog_curve_dead_zone = std::bit_cast<float>(v); }
  // the owner's fault_ field before the measure's fixes (2026-11-22): the first curve's maximum (mapper + 1384)
  uint32_t analog_curve_maximum_address() const { return address + 312; }
  float analog_curve_maximum() const { return data()->analog_curve_maximum; }
  void set_analog_curve_maximum(float v) const { data()->analog_curve_maximum = v; }
  uint32_t analog_curve_maximum_bits() const { return std::bit_cast<uint32_t>(data()->analog_curve_maximum); }
  void set_analog_curve_maximum_bits(uint32_t v) const { data()->analog_curve_maximum = std::bit_cast<float>(v); }
};

// The image words docs/functions/action-mappers.md's code uses.
struct ActionMappersGlobals {
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // probably 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // probably -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // probably 0.5
  static float half(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE4u); }
  static void set_half(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE4u, v); }
  static constexpr uint32_t half_address() { return 0x82001BE4u; }
  // a new mapper's threshold off
  static float threshold_off(uint8_t* base) { return guest::LoadFloat(base, 0x8200296Cu); }
  static void set_threshold_off(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200296Cu, v); }
  static constexpr uint32_t threshold_off_address() { return 0x8200296Cu; }
  // a new mapper's threshold on
  static float threshold_on(uint8_t* base) { return guest::LoadFloat(base, 0x82002A10u); }
  static void set_threshold_on(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A10u, v); }
  static constexpr uint32_t threshold_on_address() { return 0x82002A10u; }
  // probably 1/32767: CurveUnsigned's raw value to the curve
  static float unsigned_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002BD4u); }
  static void set_unsigned_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002BD4u, v); }
  static constexpr uint32_t unsigned_scale_address() { return 0x82002BD4u; }
  // a pad value to a raw u16 (probably 32767)
  static float pad_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82007F58u); }
  static void set_pad_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F58u, v); }
  static constexpr uint32_t pad_scale_address() { return 0x82007F58u; }
  // the axes' factor on the direction pairs
  static float axis_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82007F5Cu); }
  static void set_axis_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F5Cu, v); }
  static constexpr uint32_t axis_scale_address() { return 0x82007F5Cu; }
  // CurveCentered's factor
  static float centered_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82007F60u); }
  static void set_centered_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F60u, v); }
  static constexpr uint32_t centered_scale_address() { return 0x82007F60u; }
  // CurveCentered's center, taken from the raw value
  static float centered_offset(uint8_t* base) { return guest::LoadFloat(base, 0x82007F64u); }
  static void set_centered_offset(uint8_t* base, float v) { guest::StoreFloat(base, 0x82007F64u, v); }
  static constexpr uint32_t centered_offset_address() { return 0x82007F64u; }
  // the object ActionMappers::Init was given
  template <typename T = ActionMappersOwner>
  static T owner(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BAB78u)); }
  static void set_owner(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BAB78u, v.address); }
  static void set_owner(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB78u, v); }
  static constexpr uint32_t owner_address() { return 0x826BAB78u; }
  template <typename T = InputClientList>
  static T client_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BAB7Cu)); }
  static void set_client_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BAB7Cu, v.address); }
  static void set_client_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB7Cu, v); }
  static constexpr uint32_t client_list_address() { return 0x826BAB7Cu; }
  // 4 entries of 8 bytes: +0 a mapper (players 0-3), +4 a word the menus test
  template <typename T = ActionMapper>
  static T mappers(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826BAB80u)); }
  static void set_mappers(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826BAB80u, v.address); }
  static void set_mappers(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB80u, v); }
  static constexpr uint32_t mappers_address() { return 0x826BAB80u; }
  // 1 at boot
  static uint32_t settings_index(uint8_t* base) { return guest::LoadU32(base, 0x82600B84u); }
  static void set_settings_index(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82600B84u, v); }
  static constexpr uint32_t settings_index_address() { return 0x82600B84u; }
  // ActionSettings entries of 32 bytes, picked by the index
  static constexpr uint32_t settings_address() { return 0x82600B88u; }
  // ClientList's one list (the lookups read it here)
  template <typename T = InputClientList>
  static T the_client_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82970C14u)); }
  static void set_the_client_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82970C14u, v.address); }
  static void set_the_client_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82970C14u, v); }
  static constexpr uint32_t the_client_list_address() { return 0x82970C14u; }
};

}  // namespace schema

#include "schema/input.h"
