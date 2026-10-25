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

// sub_8235A990 TextureHeader::Relocate: `base_address` added to the upper
// 20 bits of the header's +0x20 (the low 12 kept), and when the header has
// more than one mip level (sub_822A5B68: ((+44 >> 6) & 0xF) + 1, merged
// here) `mip_address` to +0x30's the same way. Returns the level count (the
// r3 the original leaves).
uint32_t RelocateTexture(uint8_t* base, uint32_t header, uint32_t base_address,
                         uint32_t mip_address);

// sub_8235A9F8 Resource::Relocate (probably): by the resource's type
// (Resource::GetType): 1 `address` added to +24 above its low 2 bits; 2 and 8
// added to +24; 3 and 17-20 TextureHeader::Relocate with `address` for both
// (the mip address 0 when +48's upper 20 bits are 0); the others nothing.
// Returns the r3 the original leaves: the level count after a relocation,
// else the type.
uint32_t RelocateResource(uint8_t* base, uint32_t resource, uint32_t address);

// sub_82596F68 XPR::Create: `file` an XPR2 bundle; anything else gets the
// original's debug message (logged) and 0x80004005. Else +4 and +12 the
// file's header and data sizes, +0 the header block (Memory::AllocVirtual)
// and +8 the data block (Memory::AllocPhysical with kXprDataAllocation)
// with the bytes after the file's 12 copied in (memcpy, recorded for the
// renderer: native::Buffers_Memcpy), +20 the record count (the header's +0)
// and +16 the records (its +4): each record's +12 made a pointer into the
// header block, and a texture's (`TX` in its +0's upper half) header (its +4,
// in the header block) relocated to the data block (TextureHeader::Relocate).
// Returns 0. nullopt, with nothing done, where the original must run:
// --native_memcpy off. Only over the allocation layer with the memory
// blocks' host lock (the adapter's condition).
constexpr uint32_t kXprDataAllocation = 0xBC7F0000;
std::optional<uint32_t> XprCreate(uint8_t* base, uint32_t xpr, uint32_t file);

}  // namespace models
