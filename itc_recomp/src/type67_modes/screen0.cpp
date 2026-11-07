// The type 6 and 7 first screens' helpers (docs/functions/type6-type7-modes.md):
// the confirms that fill the yes/no dialog (movie 16) and post its open
// (Type6Screen0::Confirm, ConfirmAgain, Type7Screen0::Confirm), and the type
// 6 menu's page pictures (Type6Screen0::ShowPage). UiMovies::Find,
// UiMovie::SetText (Strings::FormatV's guest calls through FormatCalls) and
// the node setters through their plain C++.

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <cstdlib>

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// The movie with id `id`: the id at the frame's +80, then UiMovies::Find.
uint32_t MovieAt(uint8_t* base, uint32_t frame, uint32_t id) {
  StoreU32(base, frame + 80, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

// UiMovie::SetText with r7 to r10 0 (the user's call for its unset varargs,
// 2026-10-30), from the frame at `frame`; a refusal after the adapters'
// checks is a logged abort.
uint32_t SetText(uint8_t* base, const strings_and_font::FormatCalls& calls, uint32_t frame,
                 uint32_t movie, uint32_t sub_scene, uint32_t child, uint32_t id) {
  static constexpr uint64_t kNoArguments[4] = {0, 0, 0, 0};
  const std::optional<uint32_t> r3 =
      ui_movies::SetText(base, calls, frame, movie, sub_scene, child, id, kNoArguments);
  if (!r3) {
    ITC_LOG_ERROR("type67_modes: UiMovie::SetText refused after the adapter's checks");
    std::abort();
  }
  return *r3;
}

// A message (`message`, 0, 0) queued in a dispatcher's ring (+204), as the
// handlers do it inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, message);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

void Show(uint8_t* base, uint32_t movie, uint32_t child, uint32_t value,
          uint32_t sub_scene = 3) {
  StoreU32(base, ui_movies::FindNode(base, movie, sub_scene, child) + 80, value);
}

uint32_t Texture(uint8_t* base, uint32_t movie, uint32_t child, uint32_t texture,
                 uint32_t sub_scene = 3) {
  return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, sub_scene, child),
                                   texture);
}

// UiMovie::HideNode's work: the node's +80 = 0; the node (its r3).
uint32_t Hide(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 80, 0);
  return node;
}

// A UI movie text helper that has no native path where GetText would run
// the original: a logged abort (the adapters run these only while the UI
// movies' text setters are native).
uint32_t Required(const std::optional<uint32_t>& r3, const char* what) {
  if (!r3) {
    ITC_LOG_ERROR("type67_modes: {} refused after the adapter's checks", what);
    std::abort();
  }
  return *r3;
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  return static_cast<const Calls*>(user)->call_set(function, args, set, r1, lr);
}

constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishAll = 0x8211DFF8;

}  // namespace

uint64_t Screen0Confirm(uint8_t* base, const ConfirmKind& kind, uint32_t dispatcher, uint32_t sp,
                        const strings_and_font::FormatCalls& calls) {
  // One print per function: the macro's flag is per call site.
  if (&kind == &kType6Confirm) {
    ITC_NATIVE_RUNNING("sub_82233FC8 Type6Screen0::Confirm");
  } else if (&kind == &kType6ConfirmAgain) {
    ITC_NATIVE_RUNNING("sub_822340B0 Type6Screen0::ConfirmAgain");
  } else {
    ITC_NATIVE_RUNNING("sub_82244528 Type7Screen0::Confirm");
  }
  const uint32_t frame = sp - 112;
  const uint32_t movie = MovieAt(base, frame, 16);
  SetText(base, calls, frame, movie, 0xFFFFFFFF, 0, kind.text);
  SetText(base, calls, frame, movie, 1, 3, 2);
  const uint32_t r3 = SetText(base, calls, frame, movie, 1, 4, 3);
  StoreU32(base, movie + 40960, kind.choice);
  Post(base, dispatcher, kind.message);
  return r3;
}

uint64_t Type6Screen0ShowPage(uint8_t* base, uint32_t page, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822344A8 Type6Screen0::ShowPage");
  const uint32_t movie = MovieAt(base, sp - 112, 22);
  Show(base, movie, 4, 1);
  Show(base, movie, 5, 1);
  for (const uint32_t child : {6u, 7u, 8u, 9u, 13u, 14u}) {
    Show(base, movie, child, 0);
  }
  switch (int32_t(page)) {
    case 0:
      Show(base, movie, 6, 1);
      Show(base, movie, 7, 1);
      Texture(base, movie, 4, 0);
      Texture(base, movie, 6, 6);
      Texture(base, movie, 5, 0);
      return Texture(base, movie, 7, 2);
    case 1:
      Texture(base, movie, 4, 6);
      return Texture(base, movie, 5, 1);
    case 2:
      Show(base, movie, 6, 1);
      Show(base, movie, 7, 1);
      Texture(base, movie, 4, 0);
      Texture(base, movie, 6, 1);
      Texture(base, movie, 5, 0);
      return Texture(base, movie, 7, 1);
    case 3:
      Texture(base, movie, 4, 0);
      return Texture(base, movie, 5, 0);
    default: {
      Show(base, movie, 4, 0);
      const uint32_t node = ui_movies::FindNode(base, movie, 3, 5);
      StoreU32(base, node + 80, 0);
      return node;
    }
  }
}

uint64_t Type7Screen0ShowPage(uint8_t* base, uint32_t page, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82244930 Type7Screen0::ShowPage");
  const uint32_t movie = MovieAt(base, sp - 112, 22);
  for (const uint32_t child : {4u, 5u, 6u, 7u}) {
    Show(base, movie, child, 1, 2);
  }
  Hide(base, movie, 2, 8);
  Hide(base, movie, 2, 9);
  switch (int32_t(page)) {
    case 0:
      Texture(base, movie, 4, 0, 2);
      Texture(base, movie, 6, 6, 2);
      Texture(base, movie, 5, 0, 2);
      return Texture(base, movie, 7, 2, 2);
    case 1:
      Texture(base, movie, 4, 6, 2);
      Texture(base, movie, 5, 1, 2);
      Hide(base, movie, 2, 6);
      return Hide(base, movie, 2, 7);
    default:
      Texture(base, movie, 4, 0, 2);
      Texture(base, movie, 6, 1, 2);
      Texture(base, movie, 5, 0, 2);
      return Texture(base, movie, 7, 1, 2);
  }
}

uint64_t Type7Screen0PlaceCursor(uint8_t* base, uint64_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822448B0 Type7Screen0::PlaceCursor");
  const uint32_t frame = sp - 112;
  const uint32_t movie = MovieAt(base, frame, 22);
  const uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFF, 3);
  // mulli and extsw, the doubleword through the frame's +80 (std, lfd,
  // fcfid, frsp).
  const int64_t value = int64_t(int32_t(uint32_t(item * 40)));
  StoreU32(base, frame + 80, uint32_t(uint64_t(value) >> 32));
  StoreU32(base, frame + 84, uint32_t(value));
  const uint32_t child = ui_movies::Child(base, node, 0);
  guest::StoreFloat(base, child + 16, float(double(value)));
  return child;
}

uint64_t Type7Screen0ShowHelp(uint8_t* base, uint32_t item, uint32_t sp,
                              const strings_and_font::FormatCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82244AB8 Type7Screen0::ShowHelp");
  const uint32_t frame = sp - 128;
  const uint32_t movie = MovieAt(base, frame, 22);
  uint32_t text = 919;
  switch (int32_t(item)) {
    case 1: text = 916; break;
    case 2: text = 917; break;
    case 3: text = 918; break;
    default: break;
  }
  Required(ui_movies::ResetTextScroll(base, movie, 1, 0), "sub_8211C5F8");
  SetText(base, calls, frame, movie, 1, 0, text);
  return Required(ui_movies::ScrollText(base, movie, 1, 0, 1), "UiMovie::ScrollText");
}

uint64_t Type7Screen0FadeOutStep(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82244760 Type7Screen0::FadeOutStep");
  const uint32_t frame = sp - 112;
  const uint32_t movie = MovieAt(base, frame, 22);
  const uint32_t page = LoadU32(base, screen + 9648);
  if (LoadU32(base, page + 24) == 0) {
    mode_tick::ScreenFadesFadeOut(base, LoadU32(base, 0x82001BF0), 0, 0, 0, 0);
    StoreU32(base, page + 24, 1);
  }
  calls.call(kRun, frame, 0x822447D4, movie, 0);
  const bool done = (LoadU32(base, mode_tick::kFadeOutCounts) |
                     LoadU32(base, mode_tick::kFadeInCounts)) == 0;
  if (done) {
    calls.call(kFinishAll, frame, 0x822447FC, movie, 0);
  }
  return done ? 1 : 0;
}

uint64_t Type7Screen0ChosenPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82244610 Type7Screen0::ChosenPage");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = sp - 112;
  const uint32_t item = LoadU32(base, LoadU32(base, screen + 9652));
  if (item == 3) {
    StoreU32(base, screen + 9608, 10);
    return r3;
  }
  if (item > 3) {
    return r3;
  }
  StoreU32(base, screen + 9608, 9);
  // r6 as the post leaves it (the ring's start plus its count, 64-bit),
  // passed on to the slot below.
  const uint64_t r6 = uint64_t(LoadU32(base, dispatcher + 204 + 604)) +
                      uint64_t(LoadU32(base, dispatcher + 204 + 600));
  Post(base, dispatcher, item == 0 ? 13 : item == 1 ? 15 : 23);
  if (item != 0) {
    return r3;
  }
  // The music: bank 1's cue 2 stopped (the sound manager's slot 13), bank
  // 1's cue 0 played (Sound::Play), the indexes in the frame.
  StoreU32(base, frame + 84, 1);
  StoreU32(base, frame + 80, 2);
  const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
  const uint64_t args[4] = {manager, frame + 84, frame + 80, r6};
  calls.call_set(LoadU32(base, LoadU32(base, manager) + 52), args, 4, frame, 0x82244730);
  StoreU32(base, frame + 92, 1);
  StoreU32(base, frame + 88, 0);
  return menu_sounds::PlaySound(base, frame + 92, frame + 88, frame,
                                {const_cast<Calls*>(&calls), &SoundThunk});
}

}  // namespace type67_modes
