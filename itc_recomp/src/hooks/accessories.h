// The interior accessories' register steps (src/accessories/,
// docs/functions/accessories.md), shared by the adapters
// (src/hooks/accessories.cpp, behind --native_accessories) and the harness
// (without the flag). Each falls back to the original where the plain C++
// can't run (the allocation layer or the archive's queue not native, a
// block the layer didn't give), and leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

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
std::function<void(uint32_t set, uint32_t frame)> AccessoriesSetDestructor(lifted::Regs& ctx);
// Whether the set in [slot] (if any) is the allocation layer's pages, so
// Free's FreeData runs natively.
bool AccessoriesSetFreeable(uint32_t slot);

// AccessoryFile::OnArchiveDone sub_820F0C20 (r3 the serial, r6 the file).
void AccessoryFileOnArchiveDone(lifted::Regs& ctx);
// Accessories::Create sub_820F1038 and SetScale sub_820F0F00 (r4 the
// scale).
void AccessoriesCreate(lifted::Regs& ctx);
void AccessoriesSetScale(lifted::Regs& ctx);
// Accessories::Destroy sub_820F1078.
void AccessoriesDestroy(lifted::Regs& ctx);
// Accessories::ReleaseAll sub_820F0AD8 (r3 the set).
void AccessoriesReleaseAll(lifted::Regs& ctx);
// Accessories::Release sub_820F0A90 (r3 the set, r4 the index).
void AccessoriesRelease(lifted::Regs& ctx);
// Accessories::~Accessories sub_820F0FE0 (r4 the flags), Accessory::SetTexture
// sub_820EF568 (r4 the index, r5 the shape or -1), Matrix::
// FromScaleRotationTranslation sub_820EF660 (r3 out, r4 the angles, r5 the
// position, r6 the scale).
void AccessoriesDestruct(lifted::Regs& ctx);
void AccessorySetTexture(lifted::Regs& ctx);
void MatrixFromScaleRotationTranslation(lifted::Regs& ctx);
// The texture provider's slot as Accessory::SetTexture's plain C++ calls it.
std::function<uint64_t(uint32_t provider, uint32_t frame)> AccessoryTextureSet(lifted::Regs& ctx);
// Whether Accessory::Finish runs natively: the loads, the models' lifetimes
// and memcpy native, and its files' resets (accessories::Finishable).
bool AccessoriesFinishNative(uint32_t accessory);
// Accessory::Finish sub_820EF368 (r4 release the model's files; only native
// code calls it, so no adapter: the harness's step) and
// AccessoryEntry::OnLoaded sub_820F0820 (r6 the entry).
void AccessoryFinish(lifted::Regs& ctx);
void AccessoryEntryOnLoaded(lifted::Regs& ctx);
// Whether Accessories::Acquire runs natively: the loads, the models'
// lifetimes and memcpy native, and a part model constructible.
bool AccessoriesAcquireNative();
// Accessories::Acquire sub_820F0EB8 (r4 the index, r5 the texture index).
void AccessoriesAcquire(lifted::Regs& ctx);

}  // namespace hooks
