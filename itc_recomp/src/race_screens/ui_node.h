// The UI movie nodes the in-race screens touch, through tuning-screen.toml's
// UiMovieNode view (docs/e5-native.md, step 2). A group's first child index
// (+100) and child count (+104) share the picture's width and height
// fields, read as words.

#pragma once

#include "guest.h"
#include "schema/tuning_screen.h"

#include <cstdint>

namespace race_screens {

using UiNode = schema::UiMovieNode;

// A group's first child index (+100, a word).
inline uint32_t FirstChild(uint8_t* base, uint32_t node) {
  return guest::LoadU32(base, UiNode(base, node).width_address());
}

// A group's child count (+104, a word).
inline uint32_t ChildCount(uint8_t* base, uint32_t node) {
  return guest::LoadU32(base, UiNode(base, node).height_address());
}

}  // namespace race_screens
