// UiMovie::TrackProperty (sub_8259FE68) as plain C++: see ui_movies.h. The
// stores in the original's order of parts (the guard, the clear, atexit, the
// fill, the filled byte); within the fill each entry's words as the original
// leaves them.

#include "ui_movies/ui_movies.h"

#include "base/log.h"

namespace ui_movies {

namespace {

constexpr uint32_t kGuard = 0x82970F28;   // bit 0: the table cleared
constexpr uint32_t kFilled = 0x82970C20;  // byte: the table filled
constexpr uint32_t kCleanUp = 0x825DB3F0;

// The 48 descriptors the first call fills: the node offset, the type, and
// the set/add and indirect bytes (+8, +9).
struct Descriptor {
  uint32_t offset;
  uint32_t type;
  uint8_t add;
  uint8_t indirect;
};
constexpr Descriptor kDescriptors[] = {
    {12, 0, 0, 0},
    {16, 0, 0, 0},
    {20, 1, 0, 0},
    {24, 1, 0, 0},
    {28, 0, 1, 0},
    {32, 0, 1, 0},
    {36, 0, 0, 0},
    {40, 1, 0, 0},
    {44, 1, 0, 0},
    {48, 1, 0, 0},
    {52, 1, 0, 0},
    {56, 1, 1, 0},
    {60, 1, 1, 0},
    {64, 1, 1, 0},
    {68, 0, 1, 0},
    {80, 2, 1, 0},
    {72, 1, 0, 0},
    {76, 2, 0, 0},
    {76, 2, 0, 0},
    {0, 0, 0, 1},
    {4, 0, 0, 1},
    {8, 1, 1, 1},
    {12, 1, 1, 1},
    {16, 1, 1, 1},
    {20, 0, 1, 1},
    {24, 1, 0, 1},
    {28, 1, 0, 1},
    {100, 1, 1, 0},
    {104, 1, 1, 0},
    {108, 1, 1, 0},
    {100, 0, 0, 0},
    {104, 0, 0, 0},
    {108, 1, 1, 0},
    {112, 1, 1, 0},
    {116, 1, 0, 0},
    {120, 1, 0, 0},
    {124, 1, 0, 0},
    {128, 1, 0, 0},
    {132, 2, 0, 0},
    {136, 2, 0, 0},
    {100, 0, 0, 0},
    {104, 0, 0, 0},
    {116, 0, 0, 0},
    {120, 0, 0, 0},
    {124, 1, 1, 0},
    {128, 1, 1, 0},
    {132, 1, 1, 0},
    {136, 2, 1, 0},
};
static_assert(std::size(kDescriptors) == 48);

}  // namespace

uint64_t TrackProperty(uint8_t* base, uint64_t id, uint32_t frame,
                       const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_8259FE68 UiMovie::TrackProperty");
  // The +8 words' last two bytes: the original's frame's +90 and +91 (its
  // stack's leftovers, under the two bytes it stores at +88 and +89 and
  // loads back as a word), never written by it.
  const uint16_t leftover = guest::LoadU16(base, frame + 90);
  const uint32_t guard = guest::LoadU32(base, kGuard);
  if ((guard & 1) == 0) {
    guest::StoreU32(base, kGuard, guard | 1);
    for (uint32_t i = 0; i < 64; i++) {
      const uint32_t entry = kTrackProperties + 12 * i;
      guest::StoreU32(base, entry, 0);
      guest::StoreU32(base, entry + 4, 0);
      guest::StoreU8(base, entry + 8, 0);
      guest::StoreU8(base, entry + 9, 0);
    }
    c_runtime::AtexitFrom(base, kCleanUp, call);
  }
  if (guest::LoadU8(base, kFilled) == 0) {
    for (uint32_t i = 0; i < std::size(kDescriptors); i++) {
      const Descriptor& d = kDescriptors[i];
      const uint32_t entry = kTrackProperties + 12 * i;
      guest::StoreU32(base, entry, d.offset);
      guest::StoreU32(base, entry + 4, d.type);
      guest::StoreU32(base, entry + 8,
                      uint32_t(d.add) << 24 | uint32_t(d.indirect) << 16 | leftover);
    }
    guest::StoreU8(base, kFilled, 1);
  }
  // mulli and add on the full registers: the table's address sign-extended
  // (lis).
  return uint64_t(int64_t(id) * 12) + uint64_t(int64_t(int32_t(kTrackProperties)));
}

}  // namespace ui_movies
