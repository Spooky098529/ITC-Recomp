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

// AccessoryFile::Load sub_820F0C90 (r4 the id, r5 a buffer, r6 a callback,
// r7 its context) and OnArchiveDone sub_820F0C20 (r3 the serial, r6 the
// file): r3 the file.
void AccessoryFileLoad(PPCContext& ctx, uint8_t* base);
void AccessoryFileOnArchiveDone(PPCContext& ctx, uint8_t* base);
// Accessories::Create sub_820F1038 and SetScale sub_820F0F00 (r4 the
// scale).
void AccessoriesCreate(PPCContext& ctx, uint8_t* base);
void AccessoriesSetScale(PPCContext& ctx, uint8_t* base);
// Accessories::Destroy sub_820F1078.
void AccessoriesDestroy(PPCContext& ctx, uint8_t* base);
// AccessoryEntry::New sub_820F0920.
void AccessoryEntryNew(PPCContext& ctx, uint8_t* base);
// AccessoryEntry::Destroy sub_820F0988 (r3 the entry's slot).
void AccessoryEntryDestroy(PPCContext& ctx, uint8_t* base);
// Accessories::ReleaseAll sub_820F0AD8 (r3 the set).
void AccessoriesReleaseAll(PPCContext& ctx, uint8_t* base);
// Accessories::~Accessories sub_820F0FE0 (r4 the flags), Accessory::SetTexture
// sub_820EF568 (r4 the index, r5 the shape or -1), Matrix::
// FromScaleRotationTranslation sub_820EF660 (r3 out, r4 the angles, r5 the
// position, r6 the scale).
void AccessoriesDestruct(PPCContext& ctx, uint8_t* base);
void AccessorySetTexture(PPCContext& ctx, uint8_t* base);
void MatrixFromScaleRotationTranslation(PPCContext& ctx, uint8_t* base);
// The texture provider's slot as Accessory::SetTexture's plain C++ calls it.
std::function<uint64_t(uint32_t provider, uint32_t frame)> AccessoryTextureSet(PPCContext& ctx,
                                                                               uint8_t* base);
// Whether Accessory::Finish runs natively: the loads, the models' lifetimes
// and memcpy native, and its files' resets (accessories::Finishable).
bool AccessoriesFinishNative(uint8_t* base, uint32_t accessory);
// Accessory::Finish sub_820EF368 (r4 release the model's files; only native
// code calls it, so no adapter: the harness's step) and
// AccessoryEntry::OnLoaded sub_820F0820 (r6 the entry).
void AccessoryFinish(PPCContext& ctx, uint8_t* base);
void AccessoryEntryOnLoaded(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
