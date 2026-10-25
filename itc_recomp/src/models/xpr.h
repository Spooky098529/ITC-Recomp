// The XPR texture bundles' object (24 bytes: +0 the header block, +8 the
// data block, both memory blocks; docs/functions/models.md, "XPR::") as
// plain C++: its constructor, release and destructor. The blocks are freed
// through the put-off frees (models/memory_blocks.h), so the release runs
// natively only while --native_heap routes the game's heap to the
// allocation layer (its adapter's condition). Adapters: src/hooks/models.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace models {

constexpr uint32_t kXprBytes = 24;
constexpr uint32_t kXprHeader = 0;
constexpr uint32_t kXprData = 8;
// The attributes the data block is freed with (XMemFree's, probably those
// it was allocated with).
constexpr uint32_t kXprDataAttributes = 0x807F0000;

// sub_82596E68 XPR::XPR: the six words = 0.
void XprConstruct(uint8_t* base, uint32_t xpr);

// sub_825970A8 XPR::Release: the header block put off (Memory::FreeLater)
// when there is one, +0 and +4 = 0, the data block put off
// (FreePhysicalLater with kXprDataAttributes) when there is one, +8 to +20
// = 0. Returns the original's r3 (FreePhysicalLater's, or 0 with no data
// block); nullopt, with nothing done, where the original must run (the
// pending frees can't take both blocks natively).
std::optional<uint32_t> XprRelease(uint8_t* base, uint32_t xpr);

// sub_82597148 XPR::~XPR: a jump to XPR::Release.
std::optional<uint32_t> XprDestruct(uint8_t* base, uint32_t xpr);

}  // namespace models
