// TuningScreen::Refresh's pieces (sub_821E3AD0, docs/functions/tuning-screen.md's
// "The refresh"), shared by its files: refresh.cpp (the header, states 3 and
// 4, the help line's tail and the dispatch for states 15 to 58),
// refresh_level.cpp (state 5), refresh_type.cpp (6) and refresh_choice.cpp
// (7 and 8), each defining its state's method below.
//
// The original keeps the screen in r31 (all 64 bits of r3 at entry), movie
// 2 in r30 and its 368-byte frame at r1; its callees' frames are below that
// frame. f1 is tracked as RefreshMenu's: `*f1` holds the value the original
// would have in f1 when the native code (or a native callee) set it last,
// nullopt when a guest call did (the adapter puts `*f1` in f1 before each
// later guest call and at the end).

#pragma once

#include "tuning_screen/tuning_screen.h"

#include <cstdint>
#include <initializer_list>
#include <optional>

namespace tuning_screen::refresh {

constexpr uint32_t kFrameBytes = 368;
constexpr uint32_t kAtEnd = 0x82001BE0;  // AnimationTime's answer for none playing
constexpr uint32_t kSetNodeText = 0x8211C7E8;       // UiMovie::SetNodeText(movie, node, id, ...)
constexpr uint32_t kSetNumberText = 0x8211C740;     // (movie, sub, child, value)
constexpr uint32_t kSetChildNameText = 0x8211D930;  // (movie, sub, child, name record)
constexpr uint32_t kAll = 0xFFFFFFFFu;              // sub-scene -1 for the node helpers
// The help line's tail (loc_821E68B0 and loc_821E68C0): the return
// addresses of its SetText and ScrollText.
constexpr uint32_t kHelpTextLr = 0x821E68C0;
constexpr uint32_t kHelpScrollLr = 0x821E68D4;

struct Context {
  uint8_t* base;
  const StandCalls& stand;
  uint64_t self;     // r31: r3 at entry, all 64 bits
  uint32_t screen;   // its low word
  uint32_t frame;    // r1: sp - 368
  uint32_t movie;    // r30: movie 2
  std::optional<double>* f1;

  uint32_t Word(uint32_t offset) const;
  uint32_t Byte(uint32_t offset) const;
  int32_t State() const { return int32_t(Word(kState)); }
  int32_t Category() const { return int32_t(Word(kCategory)); }
  uint32_t Item() const { return Byte(kItem); }

  // UiMovie::FindNode (sub_8211A578) and Child, the node.
  uint32_t Node(uint32_t sub_scene, uint32_t child) const;
  uint32_t ChildOf(uint32_t node, uint32_t i) const;
  // UiMovie::ShowNode (sub_8211AC00, `shown` 1) and HideNode (sub_8211AC48, 0).
  void Shown(uint32_t sub_scene, uint32_t child, uint32_t shown) const;
  // UiMovie::SetNodeTexture (sub_8211A708; the word r6's low word).
  void Texture(uint32_t sub_scene, uint32_t child, uint64_t texture) const;
  // UiMovie::SetNodeColor (sub_8211A5B8).
  void Color(uint32_t sub_scene, uint32_t child, uint32_t color) const;
  // UiMovie::AnimationTime (sub_8211BA00; f1 its time): whether it's the
  // float at `at` (fcmpu, so never for NaNs).
  bool TimeIs(uint32_t sub_scene, uint32_t index, uint32_t at) const;

  // A guest call from the frame with the original's return address; its r3.
  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> args, uint32_t lr) const;
  // UiMovie::SetText(movie, sub, child, id, r7, r8, r9, r10): `sub_scene` all
  // 64 bits (kAllScenes for li r4,-1); the arguments the original leaves
  // unset passed as 0 (the user's call, 2026-10-30).
  void Text(uint64_t sub_scene, uint32_t child, uint64_t id, uint64_t r7, uint32_t lr,
            uint64_t r10 = 0) const;
  // sub_8211C5F8 (ResetTextScroll) of the help line (10, 0), which every page
  // calls before its help line.
  void ResetHelp(uint32_t lr) const;
  // The shared tail (block 8): loc_821E68B0, the help line SetText(m, 10, 0,
  // id) then its scroll; loc_821E68C0, the scroll alone (ScrollText(m, 10, 0,
  // 1)).
  void Help(uint64_t id) const;
  void HelpScroll() const;
  // UiMovie::SetNodeText(movie, node, id, r6, r7, r8, r9, r10) from the
  // frame with the original's return address: the arguments it leaves unset
  // passed as 0 (the user's call, 2026-10-30), r6 and r10 as given.
  void NodeText(uint32_t node, uint64_t id, uint32_t lr, uint64_t r6 = 0,
                uint64_t r10 = 0) const;

  // The pieces.
  void Header();
  void State3();  // TUNING SHOP MAIN (block 2)
  void State4();  // the category's items (block 3)
  void State5();  // LEVEL_SELECT (block 4, refresh_level.cpp)
  void State6();  // TYPE_SELECT (block 5, refresh_type.cpp)
  // State 6's rear spoiler help string (629, 630 or 865; refresh_type.cpp).
  uint64_t SpoilerHelp() const;
  void State7();  // the bonnet's and colors' choice (block 6, refresh_choice.cpp)
  void State8();  // the option select (block 7, refresh_choice.cpp)
};

}  // namespace tuning_screen::refresh
