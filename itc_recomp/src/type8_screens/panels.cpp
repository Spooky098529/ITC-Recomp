// The type 8 panel screen's panels and panel group
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code; the panels' bases are the screen constructors' plain C++
// (src/screen_constructors/) and the car list's (src/car_list/).

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "guest.h"
#include "screen_constructors/screen_constructors.h"

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

namespace sc = screen_constructors;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The words by the player: player 0's (the original's unsigned player < 1)
// or player 1's.
uint32_t By(uint32_t player, uint32_t first, uint32_t second) {
  return player < 1 ? first : second;
}

uint64_t ThroughCalls(void* user, uint32_t function, const uint64_t args[8], const double*,
                      uint32_t r1, uint32_t lr) {
  const Calls& calls = *static_cast<const Calls*>(user);
  return calls.call8(function, args, r1, lr);
}

}  // namespace

car_list::Calls CarListCalls(const Calls& calls) {
  return {const_cast<Calls*>(&calls), &ThroughCalls};
}

uint64_t PanelGroup8Construct(uint8_t* base, uint64_t r3, uint32_t movie, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239C10 PanelGroup8::PanelGroup8");
  const uint32_t group = uint32_t(r3);
  Frame(base, sp, 112);
  sc::PanelGroupBase(base, group);
  StoreU32(base, group + 4, movie);
  StoreU32(base, group + 12, 10);
  StoreU32(base, group + 0, kGroupVtable);
  StoreU32(base, group + 32, 0);
  return r3;
}

uint64_t Panel8FConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239C70 Panel8F::Panel8F");
  const uint32_t p = uint32_t(r3);
  Frame(base, sp, 112);
  sc::PanelFBase(base, p);
  StoreU32(base, p + 144, player);
  StoreU32(base, p + 4, group);
  StoreU32(base, p + 36, 11);
  StoreU32(base, p + 0, kListNodeVtable);
  StoreU32(base, p + 88, 1);
  StoreU32(base, p + 92, 0);
  StoreU32(base, p + 44, 5);
  StoreU32(base, p + 48, 6);
  StoreU32(base, p + 40, 0);
  StoreU32(base, p + 52, 7);
  StoreU32(base, p + 84, 4);
  StoreU32(base, p + 116, 369);
  StoreU32(base, p + 124, 354);
  StoreU32(base, p + 128, 358);
  StoreU32(base, p + 28, 5);
  StoreU32(base, p + 32, 4);
  StoreU32(base, p + 24, By(player, 6, 7));
  StoreU32(base, p + 96, By(player, 15, 186));
  StoreU32(base, p + 104, By(player, 26, 197));
  StoreU32(base, p + 100, By(player, 25, 196));
  StoreU32(base, p + 120, By(player, 12, 183));
  return r3;
}

uint64_t Panel8EConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239D58 Panel8E::Panel8E");
  const uint32_t p = uint32_t(r3);
  Frame(base, sp, 112);
  sc::PanelEBase(base, p);
  StoreU32(base, p + 160, player);
  StoreU32(base, p + 0, group);
  StoreU32(base, p + 20, 1);
  StoreU32(base, p + 4, 3);
  StoreU32(base, p + 28, 3);
  StoreU32(base, p + 24, 2);
  StoreU32(base, p + 60, 4);
  StoreU32(base, p + 120, 3);
  StoreU32(base, p + 124, 5);
  StoreU32(base, p + 128, 6);
  StoreU32(base, p + 132, 7);
  StoreU32(base, p + 16, 7);
  StoreU32(base, p + 144, 6);
  StoreU32(base, p + 12, By(player, 6, 7));
  StoreU32(base, p + 64, By(player, 15, 186));
  StoreU32(base, p + 68, By(player, 25, 196));
  StoreU32(base, p + 72, By(player, 26, 197));
  StoreU32(base, p + 76, By(player, 177, 348));
  StoreU32(base, p + 92, By(player, 65, 236));
  StoreU32(base, p + 108, By(player, 171, 342));
  StoreU32(base, p + 104, By(player, 170, 341));
  StoreU32(base, p + 100, By(player, 70, 241));
  StoreU32(base, p + 96, By(player, 69, 240));
  StoreU32(base, p + 148, 12);
  StoreU32(base, p + 88, By(player, 180, 351));
  return r3;
}

uint64_t Panel8CConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82239E78 Panel8C::Panel8C");
  const uint32_t p = uint32_t(r3);
  Frame(base, sp, 128);
  sc::PanelCBase(base, p);
  // The shared words, in the original's order.
  static constexpr uint32_t kWords[][2] = {
      {12, 0},    {320, 0},   {32, 11},   {172, 13},  {168, 16},  {180, 25},  {196, 11},
      {136, 10},  {0, 0},     {192, 14},  {204, 18},  {208, 23},  {216, 15},  {160, 10},
      {144, 5},   {212, 24},  {220, 12},  {224, 20},  {228, 26},  {232, 22},  {240, 21},
      {4, 0},     {140, 8},   {132, 6},   {176, 19},  {184, 9},   {200, 17},  {236, 28},
      {244, 27},  {272, 4},   {152, 0},   {56, 0xFFFFFFFFu},      {276, 123}, {280, 112},
      {20, 9},    {156, 8},
  };
  for (const auto& [at, value] : kWords) {
    uint32_t word = value;
    if (at == 12) word = group;
    if (at == 320) word = player;
    if (at == 0) word = kPanel8CVtable;
    StoreU32(base, p + at, word);
  }
  // The player's words.
  if (player < 1) {
    StoreU32(base, p + 36, 19);
    StoreU32(base, p + 16, 6);
    StoreU32(base, p + 24, 2);
    StoreU32(base, p + 40, 178);
    StoreU32(base, p + 44, 367);
    StoreU32(base, p + 68, 74);
    StoreU32(base, p + 48, 181);
    StoreU32(base, p + 52, 368);
    StoreU32(base, p + 72, 75);
    StoreU32(base, p + 108, 84);
    StoreU32(base, p + 112, 85);
    StoreU32(base, p + 60, 173);
    StoreU32(base, p + 120, 372);
    StoreU32(base, p + 64, 172);
    StoreU32(base, p + 116, 370);
  } else {
    StoreU32(base, p + 36, 17);
    StoreU32(base, p + 16, 7);
    StoreU32(base, p + 24, 3);
    StoreU32(base, p + 40, 349);
    StoreU32(base, p + 68, 245);
    StoreU32(base, p + 48, 352);
    StoreU32(base, p + 44, 365);
    StoreU32(base, p + 52, 366);
    StoreU32(base, p + 72, 246);
    StoreU32(base, p + 108, 255);
    StoreU32(base, p + 60, 344);
    StoreU32(base, p + 112, 256);
    StoreU32(base, p + 120, 373);
    StoreU32(base, p + 64, 343);
    StoreU32(base, p + 116, 371);
  }
  return r3;
}

uint64_t Panel8DConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223A100 Panel8D::Panel8D");
  const uint32_t p = uint32_t(r3);
  Frame(base, sp, 112);
  sc::PanelDBase(base, p);
  StoreU32(base, p + 92, player);
  StoreU32(base, p + 4, group);
  StoreU32(base, p + 68, 1);
  StoreU32(base, p + 64, 0);
  StoreU32(base, p + 80, 2);
  StoreU32(base, p + 76, 17);
  StoreU32(base, p + 0, kListNodeVtable);
  StoreU32(base, p + 12, 11);
  StoreU32(base, p + 60, 10);
  StoreU32(base, p + 72, 16);
  StoreU32(base, p + 8, By(player, 6, 7));
  StoreU32(base, p + 16, By(player, 179, 350));
  StoreU32(base, p + 24, By(player, 157, 328));
  StoreU32(base, p + 20, By(player, 182, 353));
  StoreU32(base, p + 56, By(player, 175, 346));
  StoreU32(base, p + 52, By(player, 174, 345));
  StoreU32(base, p + 40, By(player, 169, 340));
  StoreU32(base, p + 36, By(player, 168, 339));
  StoreU32(base, p + 32, By(player, 167, 338));
  StoreU32(base, p + 28, By(player, 166, 337));
  return r3;
}

uint64_t ConfirmPanel8Construct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                                uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223A208 ConfirmPanel8::ConfirmPanel8");
  const uint32_t p = uint32_t(r3);
  Frame(base, sp, 112);
  car_list::ConfirmPanelConstruct(base, r3);
  StoreU32(base, p + 40, player);
  StoreU32(base, p + 4, group);
  StoreU32(base, p + 24, 3);
  StoreU32(base, p + 0, kEmptyBaseVtable);
  StoreU32(base, p + 12, By(player, 0, 1));
  StoreU32(base, p + 16, By(player, 6, 7));
  StoreU32(base, p + 20, 2);
  StoreU32(base, p + 36, By(player, 2, 5));
  StoreU32(base, p + 32, By(player, 1, 4));
  StoreU32(base, p + 28, By(player, 374, 375));
  return r3;
}

uint64_t PanelGroup8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A2A8 PanelGroup8::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  car_list::GroupBaseDestruct(base, r3);
  if (flags & 1) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x8223A2D8);
  }
  return r3;
}

uint64_t Panel8CDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A348 Panel8C::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  car_list::CarPanelBaseDestruct(base, r3);
  if (flags & 1) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x8223A378);
  }
  return r3;
}

uint64_t Panel8CSetStats(uint8_t* base, uint64_t r3, uint32_t record, uint32_t sp,
                         const car_list::Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A090 Panel8C::SetStats");
  return car_list::Panel6CSetStats(base, uint32_t(r3), record, sp, calls);
}

uint64_t Panel8CShowArrows(uint8_t* base, uint64_t r3, uint64_t shown, uint32_t sp,
                           const car_list::Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223A098 Panel8C::ShowArrows");
  const uint32_t p = uint32_t(r3), frame = Frame(base, sp, 112);
  if (int32_t(LoadU32(base, p + 156)) < 0) {
    return r3;
  }
  const uint32_t m =
      uint32_t(type67_screens::PanelGroupMovie(base, LoadU32(base, p + 12), frame));
  const uint32_t child = LoadU32(base, p + 156);
  const uint32_t sub_scene = LoadU32(base, p + 16);
  if (uint32_t(shown) == 0) {
    return calls.Call(car_list::kHideNode, frame, 0x8223A0E4, m, sub_scene, child);
  }
  return calls.Call(car_list::kShowNode, frame, 0x8223A0DC, m, sub_scene, child);
}

}  // namespace type8_screens
