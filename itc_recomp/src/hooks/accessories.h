// The interior accessories' register steps (src/accessories/,
// docs/functions/accessories.md), shared by the adapters
// (src/hooks/accessories.cpp, behind --native_accessories) and the harness
// (without the flag). Each falls back to the original where the plain C++
// can't run (the allocation layer or the archive's queue not native, a
// block the layer didn't give), and leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// Whether the accessories' functions that allocate, free or queue loads can
// run natively: VirtualAlloc and VirtualFree through the allocation layer,
// and Archive::Load's queue native.
bool AccessoriesNativeLoads();

// AccessoryFile::AccessoryFile sub_820F0C50, SetData sub_820F0BF0 (r4 the
// data, r5 owned), Load sub_820F0C90 (r4 the id, r5 a buffer, r6 a
// callback, r7 its context), OnArchiveDone sub_820F0C20 (r3 the serial, r6
// the file) and FreeData sub_820F08D0 (r3 the word): r3 the file.
void AccessoryFileConstruct(PPCContext& ctx, uint8_t* base);
void AccessoryFileSetData(PPCContext& ctx, uint8_t* base);
void AccessoryFileLoad(PPCContext& ctx, uint8_t* base);
void AccessoryFileOnArchiveDone(PPCContext& ctx, uint8_t* base);
void AccessoryFileFreeData(PPCContext& ctx, uint8_t* base);
// AccessoriesBase::AccessoriesBase sub_820F07B8, Accessories::Accessories
// sub_820F0E60, New sub_820F0F48, Create sub_820F1038 and SetScale
// sub_820F0F00 (r4 the scale).
void AccessoriesBaseConstruct(PPCContext& ctx, uint8_t* base);
void AccessoriesConstruct(PPCContext& ctx, uint8_t* base);
void AccessoriesNew(PPCContext& ctx, uint8_t* base);
void AccessoriesCreate(PPCContext& ctx, uint8_t* base);
void AccessoriesSetScale(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
