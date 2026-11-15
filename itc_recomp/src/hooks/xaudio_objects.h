// XAudio's factory objects' register steps (src/xaudio/objects.h,
// docs/functions/xaudio-objects.md), shared by the adapters
// (src/hooks/xaudio_objects.cpp, behind --native_xaudio_objects) and the
// harness (without the flag). Each takes the original's registers and
// leaves r3 as the original does, in full.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void XAudioCodecSize(PPCContext& ctx, uint8_t* base);
void XAudioCodecSize(lifted::Regs& ctx, uint8_t* base);          // sub_82466B78
void XAudioMatrixSize(PPCContext& ctx, uint8_t* base);
void XAudioMatrixSize(lifted::Regs& ctx, uint8_t* base);         // sub_82466840
void XAudioReverbSize(PPCContext& ctx, uint8_t* base);
void XAudioReverbSize(lifted::Regs& ctx, uint8_t* base);         // sub_82463B78
void XAudioMeterSize(PPCContext& ctx, uint8_t* base);
void XAudioMeterSize(lifted::Regs& ctx, uint8_t* base);          // sub_825A60E0
void XAudioCodecGetInfo(PPCContext& ctx, uint8_t* base);
void XAudioCodecGetInfo(lifted::Regs& ctx, uint8_t* base);       // sub_82466E80
void XAudioMatrixGetInfo(PPCContext& ctx, uint8_t* base);
void XAudioMatrixGetInfo(lifted::Regs& ctx, uint8_t* base);      // sub_82465BE0
void XAudioMeterGetInfo(PPCContext& ctx, uint8_t* base);
void XAudioMeterGetInfo(lifted::Regs& ctx, uint8_t* base);       // sub_825A5778
void XAudioCodecGetState(PPCContext& ctx, uint8_t* base);
void XAudioCodecGetState(lifted::Regs& ctx, uint8_t* base);      // sub_8246A920
void XAudioMatrixSetOverwrite(PPCContext& ctx, uint8_t* base);
void XAudioMatrixSetOverwrite(lifted::Regs& ctx, uint8_t* base);  // sub_82465C08
void XAudioReverbSetStereo(PPCContext& ctx, uint8_t* base);
void XAudioReverbSetStereo(lifted::Regs& ctx, uint8_t* base);    // sub_824679F8
void XAudioObjectRelease(PPCContext& ctx, uint8_t* base);
void XAudioObjectRelease(lifted::Regs& ctx, uint8_t* base);      // sub_8246CE70

void XAudioCodecCreate(PPCContext& ctx, uint8_t* base);
void XAudioCodecCreate(lifted::Regs& ctx, uint8_t* base);        // sub_82466BA0
void XAudioCodecBaseInit(PPCContext& ctx, uint8_t* base);
void XAudioCodecBaseInit(lifted::Regs& ctx, uint8_t* base);      // sub_82469978 (slot +4)
void XAudioCodecDestruct(PPCContext& ctx, uint8_t* base);
void XAudioCodecDestruct(lifted::Regs& ctx, uint8_t* base);      // sub_82466EE8
void XAudioPcmCodecInnerDestroy(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecInnerDestroy(lifted::Regs& ctx, uint8_t* base);  // sub_8246CE68
void XAudioXmaCodecInnerDestroy(PPCContext& ctx, uint8_t* base);
void XAudioXmaCodecInnerDestroy(lifted::Regs& ctx, uint8_t* base);  // sub_8246A918
void XAudioMatrixInnerDestroy(PPCContext& ctx, uint8_t* base);
void XAudioMatrixInnerDestroy(lifted::Regs& ctx, uint8_t* base);    // sub_82465BD8
void XAudioMatrixCreate(PPCContext& ctx, uint8_t* base);
void XAudioMatrixCreate(lifted::Regs& ctx, uint8_t* base);       // sub_82466888
void XAudioMeterCreate(PPCContext& ctx, uint8_t* base);
void XAudioMeterCreate(lifted::Regs& ctx, uint8_t* base);        // sub_825A6460
void XAudioMeterInit(PPCContext& ctx, uint8_t* base);
void XAudioMeterInit(lifted::Regs& ctx, uint8_t* base);          // sub_825A6138

// Functions only native callers reach, for the harness: their plain C++
// with the original's registers.
void XAudioPacketListNext(PPCContext& ctx, uint8_t* base);
void XAudioPacketListNext(lifted::Regs& ctx, uint8_t* base);     // sub_82466EA0
void XAudioPacketListClear(PPCContext& ctx, uint8_t* base);
void XAudioPacketListClear(lifted::Regs& ctx, uint8_t* base);    // sub_8246E060
void XAudioPacketListsAlloc(PPCContext& ctx, uint8_t* base);
void XAudioPacketListsAlloc(lifted::Regs& ctx, uint8_t* base);   // sub_8246E120
void XAudioPcmCodecConstruct(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecConstruct(lifted::Regs& ctx, uint8_t* base);  // sub_8246D750
void XAudioXmaCodecConstruct(PPCContext& ctx, uint8_t* base);
void XAudioXmaCodecConstruct(lifted::Regs& ctx, uint8_t* base);  // sub_8246C1D0

// The packet queue and the codecs' slots (under the voice lists' spin lock,
// taken by this thread as the game does).
void XAudioCodecSetFlags(PPCContext& ctx, uint8_t* base);
void XAudioCodecSetFlags(lifted::Regs& ctx, uint8_t* base);            // sub_82467078
void XAudioCodecRetirePacket(PPCContext& ctx, uint8_t* base);
void XAudioCodecRetirePacket(lifted::Regs& ctx, uint8_t* base);        // sub_8246A430
void XAudioCodecCompletePacket(PPCContext& ctx, uint8_t* base);
void XAudioCodecCompletePacket(lifted::Regs& ctx, uint8_t* base);      // sub_824674D8
void XAudioCodecFlushPackets(PPCContext& ctx, uint8_t* base);
void XAudioCodecFlushPackets(lifted::Regs& ctx, uint8_t* base);        // sub_824699E0
void XAudioCodecFlush(PPCContext& ctx, uint8_t* base);
void XAudioCodecFlush(lifted::Regs& ctx, uint8_t* base);               // sub_8246CEC8
void XAudioCodecBaseSetFormat(PPCContext& ctx, uint8_t* base);
void XAudioCodecBaseSetFormat(lifted::Regs& ctx, uint8_t* base);       // sub_82469E58
void XAudioPcmCodecSetFormat(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecSetFormat(lifted::Regs& ctx, uint8_t* base);        // sub_8246CFA8
void XAudioPcmCodecStart(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecStart(lifted::Regs& ctx, uint8_t* base);            // sub_8246D410
void XAudioPcmCodecStop(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecStop(lifted::Regs& ctx, uint8_t* base);             // sub_8246D500
void XAudioCodecEndOfStream(PPCContext& ctx, uint8_t* base);
void XAudioCodecEndOfStream(lifted::Regs& ctx, uint8_t* base);         // sub_8246D630
void XAudioPcmCodecSetVolume(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecSetVolume(lifted::Regs& ctx, uint8_t* base);        // sub_8246D008
void XAudioPcmCodecSetPitch(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecSetPitch(lifted::Regs& ctx, uint8_t* base);         // sub_8246D0C8
void XAudioPcmCodecSetFrequencyRatio(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecSetFrequencyRatio(lifted::Regs& ctx, uint8_t* base);  // sub_8246D1E0
void XAudioPcmCodecBaseSetFrequencyRatio(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecBaseSetFrequencyRatio(lifted::Regs& ctx, uint8_t* base);  // sub_8246D320
void XAudioXmaCodecSetVolume(PPCContext& ctx, uint8_t* base);
void XAudioXmaCodecSetVolume(lifted::Regs& ctx, uint8_t* base);        // sub_8246AB48
void XAudioXmaCodecSetPitch(PPCContext& ctx, uint8_t* base);
void XAudioXmaCodecSetPitch(lifted::Regs& ctx, uint8_t* base);         // sub_8246AC40
void XAudioCodecSubmitPacket(PPCContext& ctx, uint8_t* base);
void XAudioCodecSubmitPacket(lifted::Regs& ctx, uint8_t* base);        // sub_82469EF8
void XAudioCodecSubmitRegion(PPCContext& ctx, uint8_t* base);
void XAudioCodecSubmitRegion(lifted::Regs& ctx, uint8_t* base);        // sub_8246A1C0
void XAudioPcmCodecSubmitPacket(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecSubmitPacket(lifted::Regs& ctx, uint8_t* base);     // sub_8246CEB8
void XAudioXmaCodecSubmitRegion(PPCContext& ctx, uint8_t* base);
void XAudioXmaCodecSubmitRegion(lifted::Regs& ctx, uint8_t* base);     // sub_8246A860
void XAudioCodecOnSubmit(PPCContext& ctx, uint8_t* base);
void XAudioCodecOnSubmit(lifted::Regs& ctx, uint8_t* base);            // sub_82469AF8 (harness)

// The output matrix and the volume meter.
void XAudioMatrixInit(PPCContext& ctx, uint8_t* base);
void XAudioMatrixInit(lifted::Regs& ctx, uint8_t* base);       // sub_82465C30
void XAudioMatrixSetMatrix(PPCContext& ctx, uint8_t* base);
void XAudioMatrixSetMatrix(lifted::Regs& ctx, uint8_t* base);  // sub_82465E50
void XAudioMatrixSetLevels(PPCContext& ctx, uint8_t* base);
void XAudioMatrixSetLevels(lifted::Regs& ctx, uint8_t* base);  // sub_82466188
void XAudioMatrixMix(PPCContext& ctx, uint8_t* base);
void XAudioMatrixMix(lifted::Regs& ctx, uint8_t* base);        // sub_82466440
void XAudioMatrixMixRamp(PPCContext& ctx, uint8_t* base);
void XAudioMatrixMixRamp(lifted::Regs& ctx, uint8_t* base);    // sub_82466310 (harness)
void XAudioMeterProcess(PPCContext& ctx, uint8_t* base);
void XAudioMeterProcess(lifted::Regs& ctx, uint8_t* base);     // sub_825A6210

// The render driver's set-up and the reverb's small parts.
void XAudioRenderDriverSetUp(PPCContext& ctx, uint8_t* base);
void XAudioRenderDriverSetUp(lifted::Regs& ctx, uint8_t* base);   // sub_824657C8
void XAudioReverbInnerDestroy(PPCContext& ctx, uint8_t* base);
void XAudioReverbInnerDestroy(lifted::Regs& ctx, uint8_t* base);  // sub_82465410
void XAudioReverbDestruct(PPCContext& ctx, uint8_t* base);
void XAudioReverbDestruct(lifted::Regs& ctx, uint8_t* base);      // sub_824656B8
void XAudioReverbDefaults(PPCContext& ctx, uint8_t* base);
void XAudioReverbDefaults(lifted::Regs& ctx, uint8_t* base);      // sub_82467538
void XAudioLargestPrimeBelow(PPCContext& ctx, uint8_t* base);
void XAudioLargestPrimeBelow(lifted::Regs& ctx, uint8_t* base);   // sub_8246DFE8
void XAudioFilterCoefficient(PPCContext& ctx, uint8_t* base);
void XAudioFilterCoefficient(lifted::Regs& ctx, uint8_t* base);   // sub_824675B8
void XAudioReverbSetParameters(PPCContext& ctx, uint8_t* base);
void XAudioReverbSetParameters(lifted::Regs& ctx, uint8_t* base);  // sub_82462148
void XAudioI3dl2ToNative(PPCContext& ctx, uint8_t* base);
void XAudioI3dl2ToNative(lifted::Regs& ctx, uint8_t* base);        // sub_82469418
void XAudioPcmCodecProcess(PPCContext& ctx, uint8_t* base);
void XAudioPcmCodecProcess(lifted::Regs& ctx, uint8_t* base);      // sub_8246D860
void XAudioFillDelayLine2048(PPCContext& ctx, uint8_t* base);
void XAudioFillDelayLine2048(lifted::Regs& ctx, uint8_t* base);   // sub_82467720
void XAudioFillDelayLine64(PPCContext& ctx, uint8_t* base);
void XAudioFillDelayLine64(lifted::Regs& ctx, uint8_t* base);     // sub_82467788
void XAudioFillDelayLine16(PPCContext& ctx, uint8_t* base);
void XAudioFillDelayLine16(lifted::Regs& ctx, uint8_t* base);     // sub_824677F0
void XAudioFillDelayLine256(PPCContext& ctx, uint8_t* base);
void XAudioFillDelayLine256(lifted::Regs& ctx, uint8_t* base);    // sub_82467858
void XAudioFillDelayLine32(PPCContext& ctx, uint8_t* base);
void XAudioFillDelayLine32(lifted::Regs& ctx, uint8_t* base);     // sub_824678C0
void XAudioFillDelayLine128(PPCContext& ctx, uint8_t* base);
void XAudioFillDelayLine128(lifted::Regs& ctx, uint8_t* base);    // sub_82467928
void XAudioFillDelayLine512(PPCContext& ctx, uint8_t* base);
void XAudioFillDelayLine512(lifted::Regs& ctx, uint8_t* base);    // sub_82467990

// The resampler (src/xaudio/resampler.h).
void XAudioResamplerProcess(PPCContext& ctx, uint8_t* base);
void XAudioResamplerProcess(lifted::Regs& ctx, uint8_t* base);      // sub_8246F140
void XAudioResampleMono(PPCContext& ctx, uint8_t* base);
void XAudioResampleMono(lifted::Regs& ctx, uint8_t* base);          // sub_8247E188
void XAudioResampleStereo(PPCContext& ctx, uint8_t* base);
void XAudioResampleStereo(lifted::Regs& ctx, uint8_t* base);        // sub_8247E260
void XAudioResampleMonoScalar(PPCContext& ctx, uint8_t* base);
void XAudioResampleMonoScalar(lifted::Regs& ctx, uint8_t* base);    // sub_82473E48
void XAudioResampleStereoScalar(PPCContext& ctx, uint8_t* base);
void XAudioResampleStereoScalar(lifted::Regs& ctx, uint8_t* base);  // sub_82474188
void XAudioCopyStereo(PPCContext& ctx, uint8_t* base);
void XAudioCopyStereo(lifted::Regs& ctx, uint8_t* base);            // sub_8247F558
void XAudioCopyStereoScalar(PPCContext& ctx, uint8_t* base);
void XAudioCopyStereoScalar(lifted::Regs& ctx, uint8_t* base);      // sub_82478E60

// The reverb core's stages (src/xaudio/reverb_stages.h): the harness's register
// steps (no adapters: only the native ReverbCore::Process calls the stages).
void XAudioReverbLowPass(PPCContext& ctx, uint8_t* base);
void XAudioReverbLowPass(lifted::Regs& ctx, uint8_t* base);    // sub_82462BC8
void XAudioReverbDelay512(PPCContext& ctx, uint8_t* base);
void XAudioReverbDelay512(lifted::Regs& ctx, uint8_t* base);   // sub_82462CF8
void XAudioReverbDelay2048(PPCContext& ctx, uint8_t* base);
void XAudioReverbDelay2048(lifted::Regs& ctx, uint8_t* base);  // sub_82463060
void XAudioReverbDelay1024(PPCContext& ctx, uint8_t* base);
void XAudioReverbDelay1024(lifted::Regs& ctx, uint8_t* base);  // sub_82463578
void XAudioReverbAllPass128(PPCContext& ctx, uint8_t* base);
void XAudioReverbAllPass128(lifted::Regs& ctx, uint8_t* base);    // sub_82462DC8
void XAudioReverbAllPass256(PPCContext& ctx, uint8_t* base);
void XAudioReverbAllPass256(lifted::Regs& ctx, uint8_t* base);    // sub_82463130
void XAudioReverbAllPass512(PPCContext& ctx, uint8_t* base);
void XAudioReverbAllPass512(lifted::Regs& ctx, uint8_t* base);    // sub_824632A0
void XAudioReverbAllPass512To(PPCContext& ctx, uint8_t* base);
void XAudioReverbAllPass512To(lifted::Regs& ctx, uint8_t* base);  // sub_82463410
void XAudioReverbCrossfadeDelay(PPCContext& ctx, uint8_t* base);
void XAudioReverbCrossfadeDelay(lifted::Regs& ctx, uint8_t* base);     // sub_824624F8
void XAudioReverbCrossfadeDelay256(PPCContext& ctx, uint8_t* base);
void XAudioReverbCrossfadeDelay256(lifted::Regs& ctx, uint8_t* base);  // sub_82463620

// The reverb core (src/xaudio/reverb_core.h).
void XAudioReverbCoreProcess(PPCContext& ctx, uint8_t* base);
void XAudioReverbCoreProcess(lifted::Regs& ctx, uint8_t* base);  // sub_82463B90
void XAudioReverbCoreApply(PPCContext& ctx, uint8_t* base);
void XAudioReverbCoreApply(lifted::Regs& ctx, uint8_t* base);    // sub_82467A18
void XAudioReverbCoreInit(PPCContext& ctx, uint8_t* base);
void XAudioReverbCoreInit(lifted::Regs& ctx, uint8_t* base);     // sub_82468848

// The reverb's construction (src/xaudio/objects.h).
void XAudioI3dl2ParamsConstruct(PPCContext& ctx, uint8_t* base);
void XAudioI3dl2ParamsConstruct(lifted::Regs& ctx, uint8_t* base);  // sub_82469848
void XAudioReverbConstruct(PPCContext& ctx, uint8_t* base);
void XAudioReverbConstruct(lifted::Regs& ctx, uint8_t* base);       // sub_82465608
void XAudioReverbCreate(PPCContext& ctx, uint8_t* base);
void XAudioReverbCreate(lifted::Regs& ctx, uint8_t* base);          // sub_82465710

// The reverb's pass (src/xaudio/objects.h; the spin lock taken by this
// thread as the game does).
void XAudioI3dl2SetParameter(PPCContext& ctx, uint8_t* base);
void XAudioI3dl2SetParameter(lifted::Regs& ctx, uint8_t* base);  // sub_824696F0
void XAudioReverbRun(PPCContext& ctx, uint8_t* base);
void XAudioReverbRun(lifted::Regs& ctx, uint8_t* base);          // sub_82465258
void XAudioReverbProcess(PPCContext& ctx, uint8_t* base);
void XAudioReverbProcess(lifted::Regs& ctx, uint8_t* base);      // sub_82465428

}  // namespace hooks
