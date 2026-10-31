// The interior accessories' register steps (src/accessories/,
// docs/functions/accessories.md), shared by the adapters
// (src/hooks/accessories.cpp, behind --native_accessories) and the harness
// (without the flag). Each falls back to the original where the plain C++
// can't run (the allocation layer or the archive's queue not native, a
// block the layer didn't give), and leaves r3 as the original does.

#pragma once

#include "guest.h"

#include <cstdint>
#include <functional>

namespace hooks {

// Whether the accessories' functions that allocate, free or queue loads can
// run natively: VirtualAlloc and VirtualFree through the allocation layer,
// and Archive::Load's queue native.
bool AccessoriesNativeLoads();

// The set's destructor without delete (its vtable slot 0, r4 = 0) as
// Accessories::Free calls it: a guest call from `frame` returning to
// 0x820F0FC4, through `ctx`.
std::function<void(uint32_t set, uint32_t frame)> AccessoriesSetDestructor(PPCContext& ctx,
                                                                           uint8_t* base);
// Whether the set in [slot] (if any) is the allocation layer's pages, so
// Free's FreeData runs natively.
bool AccessoriesSetFreeable(uint8_t* base, uint32_t slot);

// AccessoryFile::SetData sub_820F0BF0 (r4 the data, r5 owned), Load
// sub_820F0C90 (r4 the id, r5 a buffer, r6 a callback, r7 its context),
// OnArchiveDone sub_820F0C20 (r3 the serial, r6 the file) and FreeData
// sub_820F08D0 (r3 the word): r3 the file.
void AccessoryFileSetData(PPCContext& ctx, uint8_t* base);
void AccessoryFileLoad(PPCContext& ctx, uint8_t* base);
void AccessoryFileOnArchiveDone(PPCContext& ctx, uint8_t* base);
void AccessoryFileFreeData(PPCContext& ctx, uint8_t* base);
// Accessories::Create sub_820F1038 and SetScale sub_820F0F00 (r4 the
// scale).
void AccessoriesCreate(PPCContext& ctx, uint8_t* base);
void AccessoriesSetScale(PPCContext& ctx, uint8_t* base);
// AccessoryFile::Reset sub_820F0D30 (r3 the file) and Accessories::Destroy
// sub_820F1078.
void AccessoryFileReset(PPCContext& ctx, uint8_t* base);
void AccessoriesDestroy(PPCContext& ctx, uint8_t* base);
// AccessoryEntry::New sub_820F0920.
void AccessoryEntryNew(PPCContext& ctx, uint8_t* base);
// AccessoryFile::~AccessoryFile sub_820F0DB8, AccessorySkeleton::
// ~AccessorySkeleton sub_820EFFD8, Accessory::~Accessory sub_820EF310 (r3
// them) and AccessoryEntry::Destroy sub_820F0988 (r3 the entry's slot).
void AccessoryFileDestruct(PPCContext& ctx, uint8_t* base);
void AccessorySkeletonDestruct(PPCContext& ctx, uint8_t* base);
void AccessoryDestruct(PPCContext& ctx, uint8_t* base);
void AccessoryEntryDestroy(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
