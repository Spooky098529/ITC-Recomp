#include "user/profile.h"

#include <algorithm>
#include <string_view>

namespace user {
namespace {

struct Setting {
  uint32_t id;
  uint8_t type;  // 0: unset, 1: int32, 4: string, 5: float
  uint32_t value;
};

// The settings the game asks for (its list at 82008EA0), in its order, with
// the SDK's values.
constexpr Setting kSettings[26] = {
    {0x10040013, 1, 0},     // achievements earned
    {0x10040006, 1, 0xFA},  // gamercard cred
    {0x402C0011, 4, 0},     // motto (empty: no data)
    {0x10040005, 1, 0},     // gamercard region (read by Network::Update)
    {0x5004000B, 5, 0},     // gamercard rep, 0.0f
    {0x10040012, 1, 1},     // titles played
    {0x10040004, 1, 0},     // gamercard zone
    {0x1004000C, 1, 0},     // voice muted
    {0x1004000D, 1, 0},     // voice through speakers
    {0x1004000E, 1, 0x64},  // voice volume
    {0x10040015, 1, 0},     // difficulty
    {0x10040022, 1, 1},     // auto aim
    {0x10040023, 1, 0},     // auto center
    {0x10040024, 1, 0},     // movement control
    {0x10040002, 1, 0},     // y axis inversion
    {0x10040026, 1, 0},     // race transmission
    {0x10040027, 1, 0},     // race camera location
    {0x10040028, 1, 0},     // race brake control
    {0x10040029, 1, 0},     // race accelerator control
    {0x10040039, 1, 0},     // title achievements earned
    {0x10040038, 1, 0},     // title cred earned
    {0x10040003, 1, 3},     // controller vibration
    {0x4064000F, 4, 0},     // picture key (the string below)
    {0x63E83FFF, 0, 0},     // title specific 1, unset
    {0x63E83FFE, 0, 0},     // title specific 2, unset
    {0x63E83FFD, 0, 0},     // title specific 3, unset
};
constexpr std::u16string_view kPictureKey = u"gamercard_picture_key";

void Store(std::vector<uint8_t>& buffer, uint32_t at, uint32_t value) {
  buffer[at] = uint8_t(value >> 24);
  buffer[at + 1] = uint8_t(value >> 16);
  buffer[at + 2] = uint8_t(value >> 8);
  buffer[at + 3] = uint8_t(value);
}

}  // namespace

void ProfileSettings(uint32_t user_index, uint32_t address, std::vector<uint8_t>& buffer) {
  const uint32_t data = 8 + 40 * 26;  // the picture key's string
  Store(buffer, 0, 26);
  Store(buffer, 4, address + 8);
  for (uint32_t n = 0; n < 26; n++) {
    const Setting& s = kSettings[n];
    const uint32_t at = 8 + 40 * n;
    std::fill(buffer.begin() + at, buffer.begin() + at + 40, 0);
    Store(buffer, at + 0, s.type ? 1 : 0);  // from: the profile, or unset
    Store(buffer, at + 8, user_index);
    Store(buffer, at + 16, s.id);
    buffer[at + 24] = s.type;
    Store(buffer, at + 32, s.value);
  }
  // The picture key: its size and address in its entry (22), then the string.
  const uint32_t key = 8 + 40 * 22, bytes = 2 * (uint32_t(kPictureKey.size()) + 1);
  Store(buffer, key + 32, bytes);
  Store(buffer, key + 36, address + data);
  for (uint32_t i = 0; i <= kPictureKey.size(); i++) {
    const char16_t c = i < kPictureKey.size() ? kPictureKey[i] : u'\0';
    buffer[data + 2 * i] = uint8_t(c >> 8);
    buffer[data + 2 * i + 1] = uint8_t(c);
  }
}

}  // namespace user
