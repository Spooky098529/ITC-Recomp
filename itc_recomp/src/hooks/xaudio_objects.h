// XAudio's factory objects' register steps (src/xaudio/objects.h,
// docs/functions/xaudio-objects.md), shared by the adapters
// (src/hooks/xaudio_objects.cpp, behind --native_xaudio_objects) and the
// harness (without the flag). Each takes the original's registers and
// leaves r3 as the original does, in full.

#pragma once

#include "guest.h"

namespace hooks {

void XAudioCodecSize(PPCContext& ctx, uint8_t* base);          // sub_82466B78
void XAudioMatrixSize(PPCContext& ctx, uint8_t* base);         // sub_82466840
void XAudioReverbSize(PPCContext& ctx, uint8_t* base);         // sub_82463B78
void XAudioMeterSize(PPCContext& ctx, uint8_t* base);          // sub_825A60E0
void XAudioCodecGetInfo(PPCContext& ctx, uint8_t* base);       // sub_82466E80
void XAudioMatrixGetInfo(PPCContext& ctx, uint8_t* base);      // sub_82465BE0
void XAudioMeterGetInfo(PPCContext& ctx, uint8_t* base);       // sub_825A5778
void XAudioCodecGetState(PPCContext& ctx, uint8_t* base);      // sub_8246A920
void XAudioMatrixSetOverwrite(PPCContext& ctx, uint8_t* base);  // sub_82465C08
void XAudioReverbSetStereo(PPCContext& ctx, uint8_t* base);    // sub_824679F8
void XAudioObjectRelease(PPCContext& ctx, uint8_t* base);      // sub_8246CE70

void XAudioCodecCreate(PPCContext& ctx, uint8_t* base);        // sub_82466BA0
void XAudioCodecBaseInit(PPCContext& ctx, uint8_t* base);      // sub_82469978 (slot +4)
void XAudioCodecDestruct(PPCContext& ctx, uint8_t* base);      // sub_82466EE8
void XAudioPcmCodecInnerDestroy(PPCContext& ctx, uint8_t* base);  // sub_8246CE68
void XAudioXmaCodecInnerDestroy(PPCContext& ctx, uint8_t* base);  // sub_8246A918
void XAudioMatrixInnerDestroy(PPCContext& ctx, uint8_t* base);    // sub_82465BD8
void XAudioMatrixCreate(PPCContext& ctx, uint8_t* base);       // sub_82466888
void XAudioMeterCreate(PPCContext& ctx, uint8_t* base);        // sub_825A6460
void XAudioMeterInit(PPCContext& ctx, uint8_t* base);          // sub_825A6138

// Functions only native callers reach, for the harness: their plain C++
// with the original's registers.
void XAudioPacketListNext(PPCContext& ctx, uint8_t* base);     // sub_82466EA0
void XAudioPacketListClear(PPCContext& ctx, uint8_t* base);    // sub_8246E060
void XAudioPacketListsAlloc(PPCContext& ctx, uint8_t* base);   // sub_8246E120
void XAudioPcmCodecConstruct(PPCContext& ctx, uint8_t* base);  // sub_8246D750
void XAudioXmaCodecConstruct(PPCContext& ctx, uint8_t* base);  // sub_8246C1D0

}  // namespace hooks
