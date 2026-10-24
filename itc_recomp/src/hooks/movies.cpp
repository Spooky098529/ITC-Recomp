// The game's movie class (vtable 0x82002910, docs/functions/movies.md) on our
// own FFmpeg player: adapters in front of src/movies/movie.h, behind
// --play_movies_with_ffmpeg. Slot 8 (Movie::Open, the library's player made
// on a worker) is no longer called; slots 9 and 10 (the tick handler that
// queues command 28, and the listener) stay recompiled. The callers read no
// r3 but State's: the rest leave r3 as it came. See docs/native-movies.md.

#include "guest.h"

#include "base/flags.h"
#include "hooks/title_screen.h"
#include "movies/movie.h"

ITC_FLAG_BOOL(play_movies_with_ffmpeg, true, "ITC",
              "Play the game's movies through our own FFmpeg and draw them with our renderer "
              "(the movie class's slots, docs/native-movies.md) instead of through the movie "
              "library linked into the game. Set at launch.");

namespace mv = movies::movie;

#define ITC_MOVIE(address, ...)                 \
  extern "C" REX_FUNC(sub_##address) {          \
    REX_FUNC_PROLOGUE();                        \
    if (!ITC_FLAG(play_movies_with_ffmpeg)) {   \
      __imp__sub_##address(ctx, base);          \
      return;                                   \
    }                                           \
    __VA_ARGS__;                                \
  }

ITC_MOVIE(820AEC10, mv::Play(base, ctx.r3.u32, ctx.r4.u32))
ITC_MOVIE(820AE4E0, mv::Start(base, ctx.r3.u32))
ITC_MOVIE(820AE828, mv::Stop(base, ctx.r3.u32))
ITC_MOVIE(820AE898, mv::Pause(base, ctx.r3.u32))
ITC_MOVIE(820AE8F8, mv::Resume(base, ctx.r3.u32))
ITC_MOVIE(820AEA80, ctx.r3.u64 = mv::GetState(base, ctx.r3.u32))
ITC_MOVIE(820AE528, mv::Release(base, ctx.r3.u32))

#undef ITC_MOVIE

// Movie::RenderFrame, on the render thread from command 28. --skip_intro
// waits for it to have returned once (hooks/title_screen.cpp).
extern "C" REX_FUNC(sub_820AE958) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(play_movies_with_ffmpeg)) {
    mv::RenderFrame(base, ctx.r3.u32);
  } else {
    __imp__sub_820AE958(ctx, base);
  }
  hooks::TitleScreen_MovieRendered();
}
