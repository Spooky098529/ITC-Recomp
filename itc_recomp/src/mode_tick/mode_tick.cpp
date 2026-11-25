// The mode manager's tick services as plain C++
// (docs/functions/mode-tick.md); see mode_tick.h.

#include "mode_tick/mode_tick.h"

#include "base/log.h"
#include "guest.h"
#include "c_runtime/strings.h"
#include "command_list/command_list.h"
#include "helpers/compare_string.h"
#include "hooks/last_error.h"
#include "schema/command_list.h"
#include "schema/mode_tick.h"
#include "schema/race_states.h"
#include "schema/scene_target.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <climits>
#include <cmath>

namespace mode_tick {

using G = schema::ModeTickGlobals;

void BillboardsFlipRing() {
  ITC_NATIVE_RUNNING("sub_8209BD18 Billboards::FlipRing");
  const uint32_t half = G::ring_half();
  G::set_ring_count(0);
  G::set_ring_half(half == 0 ? 1 : 0);
}

void ScreenFadesInit() {
  ITC_NATIVE_RUNNING("sub_8209CDE8 ScreenFades::Init");
  for (uint32_t layer = 0; layer < 2; layer++) {
    for (uint32_t words : {G::fade_alpha_0_address(), G::fade_out_duration_0_address(),
                           G::fade_in_duration_0_address(), G::fade_out_count_0_address(),
                           G::fade_in_count_0_address()}) {
      guest::StoreU32(words + 4 * layer, 0);
    }
  }
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(G::fade_colors_address() + 4 * i, 0);
  }
}

void ScreenFadesFadeIn(uint32_t passes, uint32_t red, uint32_t green,
                       uint32_t blue, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_8209D038 ScreenFades::FadeIn");
  const uint32_t word = 4 * layer, color = G::fade_colors_address() + 12 * layer;
  guest::StoreU32(G::fade_in_duration_0_address() + word, passes);
  guest::StoreU32(G::fade_in_count_0_address() + word, passes);
  guest::StoreU32(G::fade_out_count_0_address() + word, 0);
  guest::StoreU32(G::fade_alpha_0_address() + word, 0);
  guest::StoreU32(color, red);
  guest::StoreU32(color + 4, green);
  guest::StoreU32(color + 8, blue);
  // The other layer: 1 for layer 0, else 0 (any other value).
  if (layer == 0) {
    G::set_fade_in_count_1(0);
    G::set_fade_out_count_1(0);
    G::set_fade_alpha_1(0);
  } else {
    G::set_fade_in_count_0(0);
    G::set_fade_alpha_0(0);
    G::set_fade_out_count_0(0);
  }
}

void ScreenFadesFadeOut(uint32_t passes, uint32_t red, uint32_t green,
                        uint32_t blue, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_8209D0C8 ScreenFades::FadeOut");
  const uint32_t word = 4 * layer, color = G::fade_colors_address() + 12 * layer;
  guest::StoreU32(color, red);
  guest::StoreU32(G::fade_out_count_0_address() + word, passes);
  guest::StoreU32(G::fade_out_duration_0_address() + word, passes);
  guest::StoreU32(G::fade_in_count_0_address() + word, 0);
  guest::StoreU32(G::fade_alpha_0_address() + word, 0);
  guest::StoreU32(color + 4, green);
  guest::StoreU32(color + 8, blue);
}

namespace {

// fctiwz's answer as the translated code computes it (a NaN 0x80000000, at
// or above INT_MAX INT_MAX, else truncated: below INT_MIN 0x80000000).
uint32_t ToInt(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT_MAX)) {
    return uint32_t(INT_MAX);
  }
  if (value <= double(INT_MIN) - 1.0) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(value));
}

// 255.0 / duration × count, as the original's fdivs and fmuls (single
// rounding each), truncated.
uint32_t FadeAlpha(int32_t duration, int32_t count) {
  const double scale = double(G::fade_scale());  // 255.0
  const double d = double(float(double(duration)));
  const double c = double(float(double(count)));
  return ToInt(double(float(double(float(scale / d)) * c)));
}

// A layer's counts stepped: the in count (the alpha down from 255), then
// the out count (the alpha up to 255, held there at 0).
void StepFade(uint32_t layer) {
  const uint32_t word = 4 * layer;
  const uint32_t in_count = G::fade_in_count_0_address() + word;
  const uint32_t out_count = G::fade_out_count_0_address() + word;
  const uint32_t alpha = G::fade_alpha_0_address() + word;
  int32_t count = int32_t(guest::LoadU32(in_count));
  if (count > 0) {
    const int32_t duration = int32_t(guest::LoadU32(G::fade_in_duration_0_address() + word));
    count--;
    guest::StoreU32(in_count, uint32_t(count));
    guest::StoreU32(alpha, FadeAlpha(duration, count));
  }
  count = int32_t(guest::LoadU32(out_count));
  if (count > 0) {
    count--;
    guest::StoreU32(out_count, uint32_t(count));
    if (count == 0) {
      guest::StoreU32(alpha, 255);
    } else {
      const int32_t duration =
          int32_t(guest::LoadU32(G::fade_out_duration_0_address() + word));
      guest::StoreU32(alpha,
                      FadeAlpha(duration, int32_t(uint32_t(duration) - uint32_t(count))));
    }
  }
}

// The layer's color rectangle at `rect`, in the original's store order (its
// matrix rows are vector stores, at 16-byte aligned addresses, over the
// 1.0s it stores at +12, +28, +44 and +60 first).
void BuildFadeRect(uint32_t rect_address, uint32_t layer) {
  const schema::ColorRect rect(rect_address);
  const uint32_t matrix = rect.matrix_address();
  const uint32_t one = G::float_one();  // 1.0
  for (uint32_t row = 0; row < 4; row++) {
    guest::StoreU32(matrix + 16 * row + 12, one);
  }
  const uint32_t color = G::fade_colors_address() + 12 * layer;
  const uint32_t red = guest::LoadU32(color);
  for (uint32_t row = 0; row < 4; row++) {
    const uint32_t at = (matrix + 16 * row) & ~0xFu;
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreU32(at + 4 * i, i == row ? 0x3F800000u : 0);
    }
  }
  const uint32_t green = guest::LoadU32(color + 4);
  const uint32_t blue = guest::LoadU32(color + 8);
  rect.set_kind(0);
  rect.set_unknown_68(0);
  rect.set_x(0);
  rect.set_y(0);
  rect.set_unknown_72(2);
  rect.set_red(red);
  rect.set_green(green);
  rect.set_blue(blue);
  rect.set_width(1280);
  rect.set_height(720);
  rect.set_alpha(int32_t(guest::LoadU32(G::fade_alpha_0_address() + 4 * layer)));
}

// A slot's three words (+0, +4, +8) in the order given.
struct SlotWrite {
  uint32_t offset, value;
};
void WriteSlot(uint32_t slot, std::initializer_list<SlotWrite> writes) {
  for (const SlotWrite& w : writes) {
    guest::StoreU32(slot + w.offset, w.value);
  }
}

// Whether `slots` more slots can be allocated on `list` without the
// original's messages: no chunk open (on it or another list), and room
// (the count not wrapping).
bool ChunkFits(uint32_t list, uint32_t slots) {
  const schema::CommandList commands(list);
  if (commands.chunk_open() != 0 || schema::CommandListGlobals::open_chunks() != 0) {
    return false;
  }
  const uint32_t used = commands.used();
  return used + slots >= used && used + slots < commands.capacity();
}

}  // namespace

uint32_t ScreenFadesUpdate(uint32_t frame,
                           const std::function<void(uint32_t key, uint32_t rect)>& draw) {
  ITC_NATIVE_RUNNING("sub_8209D130 ScreenFades::Update");
  const uint32_t rect = frame + kFadeRect;
  for (uint32_t layer = 0; layer < 2; layer++) {
    if (layer == 1 || (G::fade_in_count_1() == 0 && G::fade_out_count_1() == 0)) {
      StepFade(layer);
    }
    BuildFadeRect(rect, layer);
    if (int32_t(guest::LoadU32(G::fade_alpha_0_address() + 4 * layer)) > 0) {
      draw(guest::LoadU32(G::fade_keys_address() + 4 * layer), rect);
    }
  }
  if ((G::fade_out_count_0() | G::fade_in_count_0()) != 0) {
    return 1;
  }
  return (G::fade_out_count_1() | G::fade_in_count_1()) != 0
             ? 1
             : 0;
}

bool ScissorChunksFit(uint32_t list) { return ChunkFits(list, 8); }

uint32_t RecordScissorChunk(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209CE68 ScreenFades::RecordScissor");
  command_list::OpenChunk(list);
  auto slot = [&] { return command_list::AllocateSlots(list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(chunk + 2, 5);
  guest::StoreU32(chunk + 4, 0);
  WriteSlot(slot(), {{8, 0}, {0, 31}, {4, 1}});
  const uint32_t s = slot();
  guest::StoreU16(s + 0, 0);
  guest::StoreU16(s + 2, 0);
  guest::StoreU16(s + 4, 1280);
  guest::StoreU16(s + 6, 720);
  WriteSlot(slot(), {{4, 0}, {0, 36}, {8, 0}});
  command_list::CloseChunk(list);
  return chunk;
}

uint32_t RecordScissorEndChunk(uint32_t list) {
  command_list::OpenChunk(list);
  auto slot = [&] { return command_list::AllocateSlots(list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(chunk + 2, 5);
  guest::StoreU32(chunk + 4, 0);
  WriteSlot(slot(), {{0, 1}, {4, 1}, {8, 0}});
  WriteSlot(slot(), {{4, 0}, {0, 200}});
  WriteSlot(slot(), {{0, 36}, {4, 0}, {8, 0}});
  command_list::CloseChunk(list);
  return chunk;
}

std::optional<uint32_t> ColorRectRecordFlat(uint32_t rect_address) {
  ITC_NATIVE_RUNNING("sub_8217A370 ColorRect::RecordFlat");
  const schema::ColorRect rect(rect_address);
  const uint32_t list = G::color_rect_list();
  const bool blend = rect.alpha() > 0;
  // The header, the blend states (6 or 4), 13, 6, 5, the vertices (6), 15,
  // a slot left as it is and 36.
  if (!ChunkFits(list, blend ? 19 : 17)) {
    return std::nullopt;
  }
  command_list::OpenChunk(list);
  auto slot = [&] { return command_list::AllocateSlots(list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(chunk + 2, 5);
  guest::StoreU32(chunk + 4, 0);
  uint32_t last;
  if (blend) {
    WriteSlot(slot(), {{4, 5}, {0, 1}, {8, 0}});
    WriteSlot(slot(), {{0, 56}, {4, 6}});
    WriteSlot(slot(), {{4, 0}, {0, 96}});
    WriteSlot(slot(), {{4, 1}, {0, 60}});
    WriteSlot(slot(), {{4, 6}, {0, 72}});
    last = slot();
    WriteSlot(last, {{4, 7}, {0, 76}});
  } else {
    WriteSlot(slot(), {{8, 0}, {4, 3}, {0, 1}});
    WriteSlot(slot(), {{0, 56}, {4, 6}});
    WriteSlot(slot(), {{4, 0}, {0, 96}});
    last = slot();
    WriteSlot(last, {{4, 0}, {0, 60}});
  }
  WriteSlot(slot(), {{4, 1}, {0, 13}, {8, 0}});
  WriteSlot(slot(), {{0, 6}, {4, 1}, {8, 0}});
  WriteSlot(slot(), {{0, 5}, {4, 4}, {8, 0}});
  uint32_t vertex = command_list::AllocateVertices(list, 4, false);
  const uint32_t z = G::float_one();  // 1.0
  const int32_t x = rect.x();
  const int32_t y = rect.y();
  const int32_t right = int32_t(rect.width() + uint32_t(x));
  const int32_t bottom = int32_t(rect.height() + uint32_t(y));
  const float corners[4][2] = {{float(x), float(y)},
                               {float(right), float(y)},
                               {float(x), float(bottom)},
                               {float(right), float(bottom)}};
  for (const auto& corner : corners) {
    guest::StoreFloat(vertex + 0, corner[0]);
    guest::StoreFloat(vertex + 4, corner[1]);
    guest::StoreU32(vertex + 8, z);
    const uint32_t color = (uint32_t(rect.alpha()) << 24) |
                           (uint32_t(rect.red_byte()) << 16) |
                           (uint32_t(rect.green_byte()) << 8) | rect.blue_byte();
    guest::StoreU32(vertex + 12, color);
    vertex += 24;
  }
  WriteSlot(slot(), {{4, 1}, {0, 15}, {8, 0}});
  slot();
  WriteSlot(slot(), {{4, 0}, {0, 36}, {8, 0}});
  command_list::CloseChunk(list);
  return chunk;
}

MusicIds MusicPlay(uint32_t pcr, uint32_t frame, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_821063B0 Music::Play");
  // The names (bgm_tmp_01 to bgm_tmp_17 and BGM_SYSTEM, in the image) in the
  // order the original's table holds them, then its terminator.
  static constexpr uint32_t kNames[19] = {
      0x82006AD8, 0x82006ACC, 0x82006AC0, 0x82006AB4, 0x82006AA8, 0x82006A9C, 0x82006A90,
      0x82006A84, 0x82006A78, 0x82006AE4, 0x82006A6C, 0x82006A60, 0x82006A54, 0x82006A48,
      0x82006A3C, 0x82006A30, 0x82006A24, 0x82006A18, 0};
  // The table's stores as the original's: +132 first, then the rest.
  guest::StoreU32(frame + 132, kNames[9]);
  for (uint32_t i = 0; i < 19; i++) {
    if (i != 9) {
      guest::StoreU32(frame + 96 + 4 * i, kNames[i]);
    }
  }
  uint32_t index = 0;
  while (uint32_t(helpers::Lstrcmp(pcr, name, kNames[index])) != 0) {
    index++;
    if (guest::LoadU32(frame + 96 + 4 * index) == 0) {
      index = 99;
      break;
    }
  }
  if (index == 99) {
    guest::StoreU32(frame + 80, 22887);
    guest::StoreU32(frame + 84, 22888);
    return {frame + 84, frame + 80};
  }
  guest::StoreU32(frame + 84, 22972 + index);
  guest::StoreU32(frame + 80, 22990 + index);
  return {frame + 80, frame + 84};
}

int32_t PlayersSignedInSlot(uint32_t player) {
  ITC_NATIVE_RUNNING("sub_8209F658 Players::SignedInSlot");
  const uint32_t mask = G::signed_in_mask();
  int32_t slot = 0;
  for (uint32_t user = 0; user < 4; user++) {
    if ((mask & (1u << user)) == 0) {
      continue;
    }
    if (user == guest::LoadU32(G::player_users_address() + 4 * player)) {
      return slot;
    }
    slot++;
  }
  return -1;
}

void ActionMappersSetBlocked(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8209F318 ActionMappers::SetBlocked");
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(G::action_mapper_entries_address() + 8 * i + 4, value);
  }
}

namespace {

void StoreChecks(uint32_t value) {
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(G::checks_address() + 4 * i, value);
  }
}

// The checks +16, +20 and +24 set (the others cleared before).
void SetChecks16To24() {
  G::set_check_not_on_live(1);
  G::set_check_session(1);
  G::set_check_24(1);
}

// All seven set, in the order type 1's path stores them.
void SetAllChecks() {
  for (uint32_t offset : {16u, 20u, 24u, 0u, 4u, 8u, 12u}) {
    guest::StoreU32(G::checks_address() + offset, 1);
  }
}

}  // namespace

bool ModesSetChecks(uint32_t type, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82106868 Modes::SetChecks");
  switch (type) {
    case 1: {
      const uint32_t stage = frame + 80;
      if (!vehicle_accessors::StagesCopy(stage,
                                         G::race_data().stages_address(),
                                         0xFFFFFFFF)) {
        return false;
      }
      // The race data's kind (6: two human cars).
      const uint32_t humans = G::race_data().kind();
      if (humans == 0) {
        SetAllChecks();
      } else if (humans == 6) {
        StoreChecks(0);
        if (int32_t(guest::LoadU32(stage)) == 1) {
          G::set_check_session(1);
          G::set_check_24(1);
        }
      } else {
        StoreChecks(0);
        SetChecks16To24();
      }
      return true;
    }
    case 3:
      if (int32_t(G::boot_movie()) == 10) {
        SetAllChecks();
      } else {
        StoreChecks(0);
        SetChecks16To24();
      }
      return true;
    case 4:
      SetAllChecks();
      return true;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
      StoreChecks(0);
      SetChecks16To24();
      return true;
    default:  // 0, 11, above 12 and below 0
      StoreChecks(1);
      return true;
  }
}

void ModesSetSceneTargetMode(uint32_t type) {
  ITC_NATIVE_RUNNING("sub_82106740 Modes::SetSceneTargetMode");
  uint32_t mode = 0;
  switch (type) {
    case 1: mode = 1; break;
    case 3: mode = 3; break;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 12: mode = 4; break;
    default: break;  // 4, 9, 10, 11 and any other
  }
  G::scene_target().set_mode(mode);
}

bool PresenceSetForMode(uint32_t pcr, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_822934B8 Presence::SetForMode");
  if (type == 1) {
    return false;
  }
  last_error::Set(pcr, 0);
  return true;
}

bool SignInWatchCheckIdleTraps() {
  if (G::network_active() == 0) {
    return false;
  }
  const uint32_t begin = G::network_entries_begin();
  const uint32_t end = G::network_entries_end();
  return begin > end || (end - begin) % 384 != 0;
}

bool SignInWatchCheckIdle() {
  ITC_NATIVE_RUNNING("sub_8209ED58 SignInWatch::CheckIdle");
  auto prompt = [&](uint32_t reason) {
    G::set_watch_state(1);
    G::set_watch_reason(reason);
  };
  if (int32_t(G::other_user_signed_in()) == 1) {
    G::set_watch_reason(0);
    G::set_watch_state(1);
    return true;
  }
  if (int32_t(G::user_signed_out()) == 1) {
    G::set_watch_reason(1);
    G::set_watch_state(1);
    return true;
  }
  if (G::check_connection() == 0 && int32_t(G::connection_lost()) == 1) {
    prompt(2);
    return true;
  }
  // No network session: r8 0 in the original.
  bool session = true;
  if (G::network_active() != 0) {
    if (SignInWatchCheckIdleTraps()) {
      return false;
    }
    session = G::network_entries_begin() != G::network_entries_end();
  } else {
    session = G::network_idle() == 0;
  }
  if (G::check_session() == 0 && !session) {
    prompt(5);
  }
  if (G::check_not_on_live() != 0) {
    return true;
  }
  const uint32_t shift = G::player0_user() & 0x3F;
  const uint32_t bit = shift >= 32 ? 0 : 1u << shift;
  if ((bit & G::live_mask()) != 0) {
    return true;
  }
  prompt(4);
  return true;
}

namespace {

// The player's name (player 0's slot among the signed-in users) copied to
// `to` (wcscpy), or 32 bytes of zeros when not signed in (the frame's are
// four doubleword stores, the last name's a memset: the same bytes).
void CopyPlayerName(uint32_t to) {
  if (PlayersSignedInSlot(0) >= 0) {
    const uint32_t slot = uint32_t(PlayersSignedInSlot(0));
    c_runtime::Wcscpy(to, G::user_table_address() + 14 + (slot << 5));
  } else {
    for (uint32_t i = 0; i < 32; i += 4) {
      guest::StoreU32(to + i, 0);
    }
  }
}

}  // namespace

bool SignInWatchUpdate(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8209EEC8 SignInWatch::Update");
  const uint32_t state = G::watch_state();
  if ((state >= 1 && state <= 9) || (state == 0 && SignInWatchCheckIdleTraps())) {
    return false;
  }
  const uint32_t shift = G::player0_user() & 0x3F;
  const uint32_t user = shift >= 32 ? 0 : 1u << shift;
  const uint32_t name = frame + 80;
  CopyPlayerName(name);
  const uint32_t last_mask = G::last_mask();
  const bool was_in = (last_mask & user) == user;
  if (!was_in && last_mask != G::signed_in_mask()) {
    if (G::last_count() > G::signed_in_count()) {
      G::set_user_signed_out(1);
    } else {
      G::set_other_user_signed_in(1);
    }
  }
  if (was_in && ((user & G::signed_in_mask()) == 0 ||
                 c_runtime::Wcscmp(G::last_name_address(), name) != 0)) {
    G::set_user_signed_out(1);
  }
  if (int32_t(G::check_other_user()) == 1) {
    G::set_other_user_signed_in(0);
  }
  if (int32_t(G::check_signed_out()) == 1) {
    G::set_user_signed_out(0);
  }
  if (state == 0) {
    SignInWatchCheckIdle();
  }
  // States 3 to 9 run sub_822910F0 here: CheckIdle sets 1 at most, and a
  // state above 9 isn't among them.
  G::set_last_count(G::signed_in_count());
  G::set_last_mask(G::signed_in_mask());
  CopyPlayerName(G::last_name_address());
  return true;
}

int32_t SignInWatchStaticInit(const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9518 SignInWatch::StaticInit");
  G::set_watch_state(0);
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(G::checks_address() + 4 * i, 0);
  }
  G::set_remake_save_data(0);
  // memset (sub_825A71D0): the same bytes.
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU8(G::last_name_address() + i, 0);
  }
  return c_runtime::AtexitFrom(0x825DAF08, call);
}

}  // namespace mode_tick
