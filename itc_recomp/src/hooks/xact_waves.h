// XACT's voice class and waves' register steps (src/xact/waves.h,
// docs/functions/xact-waves.md), shared by the adapters
// (src/hooks/xact_waves.cpp, behind --native_xact_waves) and the harness
// (without the flag). Each takes the original's registers and leaves r3 as
// the original does, in a frame of the original's size.

#pragma once

#include "guest.h"

namespace hooks {

void XactVoiceCreate(PPCContext& ctx, uint8_t* base);         // sub_825C9C40
void XactVoiceInit(PPCContext& ctx, uint8_t* base);           // sub_825C9B60
void XactVoiceGetState(PPCContext& ctx, uint8_t* base);       // sub_825C9DB8
void XactVoiceSetVolume(PPCContext& ctx, uint8_t* base);      // sub_825C9BE8
void XactVoiceSetPitch(PPCContext& ctx, uint8_t* base);       // sub_825C9BF0
void XactVoiceSubmitPacket(PPCContext& ctx, uint8_t* base);   // sub_825C9E08
void XactVoiceStart(PPCContext& ctx, uint8_t* base);          // sub_825C9BF8
void XactVoiceStop(PPCContext& ctx, uint8_t* base);           // sub_825C9C10
void XactVoiceFlushPackets(PPCContext& ctx, uint8_t* base);   // sub_825C9C28
void XactVoiceDeletingDestruct(PPCContext& ctx, uint8_t* base);  // sub_825CA448

}  // namespace hooks
