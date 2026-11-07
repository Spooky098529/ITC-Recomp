// The type 6 and 7 first screens' helpers (docs/functions/type6-type7-modes.md):
// the confirms that fill the yes/no dialog (movie 16) and post its open
// (Type6Screen0::Confirm, ConfirmAgain, Type7Screen0::Confirm), and the type
// 6 menu's page pictures (Type6Screen0::ShowPage). UiMovies::Find,
// UiMovie::SetText (Strings::FormatV's guest calls through FormatCalls) and
// the node setters through their plain C++.

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

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

void Show(uint8_t* base, uint32_t movie, uint32_t child, uint32_t value) {
  StoreU32(base, ui_movies::FindNode(base, movie, 3, child) + 80, value);
}

uint32_t Texture(uint8_t* base, uint32_t movie, uint32_t child, uint32_t texture) {
  return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 3, child), texture);
}

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

}  // namespace type67_modes
