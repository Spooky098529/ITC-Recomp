// Tags shader constant uploads with the draw that recorded them, so the same
// object's uploads can be found again in the next tick's command list.
//
// A draw recorder (sub_82595550) is called with what it draws: r3 = model,
// r4 = part index, r5 = flag, r6-r8 = per-draw values. Each upload recorded
// during a call is tagged with those, plus which occurrence of that tag it is
// this tick (a model drawn several times).
//
// Not effective yet: in a race this recorder runs once a tick, records 16
// uploads, and writes them to a list other than the one replayed. The
// ~2,000 per-object uploads in the replayed list come from a recorder not
// yet found, which is what should be tagged instead.

#pragma once

#include <cstdint>
#include <unordered_map>

namespace interpolation {

using DrawTag = uint64_t;

// Tags of the uploads in one recorded list, by payload address.
using DrawTags = std::unordered_map<uint32_t, DrawTag>;

// Render thread, after each tick: the tags of the list now being replayed
// (recorded on the previous tick). Valid until the next call.
const DrawTags& DrawTags_TakeReplayedTags();

}  // namespace interpolation
