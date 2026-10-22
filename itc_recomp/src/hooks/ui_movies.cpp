// The UI movie library's functions natively: adapters in front of
// src/ui_movies/ (docs/functions/ui-movies.md).

#include "guest.h"

#include "base/flags.h"
#include "ui_movies/ui_movies.h"

ITC_FLAG_BOOL(native_ui_movies, true, "ITC",
              "Run the UI movie library's functions (the movies the HUD and the menus are made "
              "of: the accessors sub_825A6508, sub_825A6518, sub_825A6528, sub_825A6540, "
              "sub_825A6558, sub_8259D600, sub_8259D618, sub_8259D628, sub_8259D648, "
              "UiMovie::Child sub_8259D658 and sub_8259D670) natively instead of through the "
              "translated code (same results; checked by --run_harnesses=ui_movies)");

// An accessor: r3 = the object, r4 = i; returns the address (or the pointer)
// `native` gives.
#define UI_MOVIE_ACCESSOR(address, expression)    \
  extern "C" REX_FUNC(sub_##address) {            \
    REX_FUNC_PROLOGUE();                          \
    if (!ITC_FLAG(native_ui_movies)) {            \
      __imp__sub_##address(ctx, base);            \
      return;                                     \
    }                                             \
    const uint32_t object = ctx.r3.u32;           \
    const uint32_t i = ctx.r4.u32;                \
    (void)base;                                   \
    ctx.r3.u64 = (expression);                    \
  }

UI_MOVIE_ACCESSOR(825A6508, ui_movies::NodeTrack(object, i))
UI_MOVIE_ACCESSOR(825A6518, ui_movies::Key(object, i))
UI_MOVIE_ACCESSOR(825A6528, ui_movies::NodeTrackList(base, object, i))
UI_MOVIE_ACCESSOR(825A6540, ui_movies::KeyList(base, object, i))
UI_MOVIE_ACCESSOR(825A6558, ui_movies::SubAnimation(base, object, i))
UI_MOVIE_ACCESSOR(8259D600, ui_movies::VertexOffset(object, i))
UI_MOVIE_ACCESSOR(8259D618, ui_movies::PrimitiveTexture(object, i))
UI_MOVIE_ACCESSOR(8259D628, ui_movies::PrimitiveVertex(base, object, i))
UI_MOVIE_ACCESSOR(8259D648, ui_movies::PictureTexture(object, i))
UI_MOVIE_ACCESSOR(8259D658, ui_movies::Child(base, object, i))
UI_MOVIE_ACCESSOR(8259D670, ui_movies::SceneAnimation(base, object, i))
#undef UI_MOVIE_ACCESSOR
