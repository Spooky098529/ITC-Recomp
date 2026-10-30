// The string table's and the font's adapters, for other code
// (src/hooks/strings_and_font.cpp).

#pragma once

#include "guest.h"
#include "strings_and_font/strings_and_font.h"

namespace hooks {

// For harnesses: while set, this runs instead of Text::Draw (sub_8210A758)
// from its adapter, so a harness sees what its callers hand it (the UI
// movies' texts: src/harnesses/ui_movies_game.cpp) without drawing.
using GuestStandIn = void (*)(PPCContext&, uint8_t*);
extern GuestStandIn g_text_draw_stand_in;

// Strings::FormatV (sub_8210A990)'s register steps without the flag (the
// adapter's and the harness's): r3 = the table, r4 = the output, r5 = the
// id, r6 = the va_list; r3 the output.
void StringsFormatVSteps(PPCContext& ctx, uint8_t* base);
// Strings::Format (sub_8210ABC0)'s: the same with r6 to r10 the varargs.
void StringsFormatSteps(PPCContext& ctx, uint8_t* base);

// Strings::FormatV's guest calls (swprintf's adapter and FormatGrouped's
// original where their plain C++ declines, the font's number) through `ctx`,
// for other native callers of StringsFormatV (UiMovie::SetText's adapter);
// lr, r1 and r3 to r5 as each call leaves them.
class FormatGuestCalls {
 public:
  FormatGuestCalls(PPCContext& ctx, uint8_t* base);
  const strings_and_font::FormatCalls& get() const { return calls_; }

 private:
  PPCContext& ctx_;
  uint8_t* base_;
  strings_and_font::FormatCalls calls_;
  friend uint32_t FormatGuestCall(void*, uint32_t, uint32_t, uint32_t, uint64_t, uint32_t,
                                  uint32_t);
};

// Text::Load (sub_8210ADF8) without its flag: the loads through the
// archive's queue (needs --native_heap and hooks::NativeArchiveQueue()).
void TextLoadSteps(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
