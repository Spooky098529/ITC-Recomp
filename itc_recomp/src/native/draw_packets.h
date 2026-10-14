#pragma once

#include "guest.h"

namespace native {

// The library's DrawIndexedVertices (sub_825BE708), without its GPU packets
// in exclusive mode; see draw_packets.cpp.
void DrawPackets_IndexedDraw(PPCContext& ctx, uint8_t* base);

}  // namespace native
