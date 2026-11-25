// XAudio's factory objects' register steps (src/xaudio/objects.h,
// docs/functions/xaudio-objects.md), shared by the adapters
// (src/hooks/xaudio_objects.cpp, behind --native_xaudio_objects) and the
// harness (without the flag). Each takes the original's registers and
// leaves r3 as the original does, in full.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void XAudioCodecSize(lifted::Regs& ctx);          // sub_82466B78
void XAudioMatrixSize(lifted::Regs& ctx);         // sub_82466840
void XAudioReverbSize(lifted::Regs& ctx);         // sub_82463B78
void XAudioMeterSize(lifted::Regs& ctx);          // sub_825A60E0
void XAudioCodecGetInfo(lifted::Regs& ctx);       // sub_82466E80
void XAudioMatrixGetInfo(lifted::Regs& ctx);      // sub_82465BE0
void XAudioMeterGetInfo(lifted::Regs& ctx);       // sub_825A5778
void XAudioCodecGetState(lifted::Regs& ctx);      // sub_8246A920
void XAudioMatrixSetOverwrite(lifted::Regs& ctx);  // sub_82465C08
void XAudioReverbSetStereo(lifted::Regs& ctx);    // sub_824679F8
void XAudioObjectRelease(lifted::Regs& ctx);      // sub_8246CE70

void XAudioCodecCreate(lifted::Regs& ctx);        // sub_82466BA0
void XAudioCodecBaseInit(lifted::Regs& ctx);      // sub_82469978 (slot +4)
void XAudioCodecDestruct(lifted::Regs& ctx);      // sub_82466EE8
void XAudioPcmCodecInnerDestroy(lifted::Regs& ctx);  // sub_8246CE68
void XAudioXmaCodecInnerDestroy(lifted::Regs& ctx);  // sub_8246A918
void XAudioMatrixInnerDestroy(lifted::Regs& ctx);    // sub_82465BD8
void XAudioMatrixCreate(lifted::Regs& ctx);       // sub_82466888
void XAudioMeterCreate(lifted::Regs& ctx);        // sub_825A6460
void XAudioMeterInit(lifted::Regs& ctx);          // sub_825A6138

// Functions only native callers reach, for the harness: their plain C++
// with the original's registers.
void XAudioPacketListNext(lifted::Regs& ctx);     // sub_82466EA0
void XAudioPacketListClear(lifted::Regs& ctx);    // sub_8246E060
void XAudioPacketListsAlloc(lifted::Regs& ctx);   // sub_8246E120
void XAudioPcmCodecConstruct(lifted::Regs& ctx);  // sub_8246D750
void XAudioXmaCodecConstruct(lifted::Regs& ctx);  // sub_8246C1D0

// The packet queue and the codecs' slots (under the voice lists' spin lock,
// taken by this thread as the game does).
void XAudioCodecSetFlags(lifted::Regs& ctx);            // sub_82467078
void XAudioCodecRetirePacket(lifted::Regs& ctx);        // sub_8246A430
void XAudioCodecCompletePacket(lifted::Regs& ctx);      // sub_824674D8
void XAudioCodecFlushPackets(lifted::Regs& ctx);        // sub_824699E0
void XAudioCodecFlush(lifted::Regs& ctx);               // sub_8246CEC8
void XAudioCodecBaseSetFormat(lifted::Regs& ctx);       // sub_82469E58
void XAudioPcmCodecSetFormat(lifted::Regs& ctx);        // sub_8246CFA8
void XAudioPcmCodecStart(lifted::Regs& ctx);            // sub_8246D410
void XAudioPcmCodecStop(lifted::Regs& ctx);             // sub_8246D500
void XAudioCodecEndOfStream(lifted::Regs& ctx);         // sub_8246D630
void XAudioPcmCodecSetVolume(lifted::Regs& ctx);        // sub_8246D008
void XAudioPcmCodecSetPitch(lifted::Regs& ctx);         // sub_8246D0C8
void XAudioPcmCodecSetFrequencyRatio(lifted::Regs& ctx);  // sub_8246D1E0
void XAudioPcmCodecBaseSetFrequencyRatio(lifted::Regs& ctx);  // sub_8246D320
void XAudioXmaCodecSetVolume(lifted::Regs& ctx);        // sub_8246AB48
void XAudioXmaCodecSetPitch(lifted::Regs& ctx);         // sub_8246AC40
void XAudioCodecSubmitPacket(lifted::Regs& ctx);        // sub_82469EF8
void XAudioCodecSubmitRegion(lifted::Regs& ctx);        // sub_8246A1C0
void XAudioPcmCodecSubmitPacket(lifted::Regs& ctx);     // sub_8246CEB8
void XAudioXmaCodecSubmitRegion(lifted::Regs& ctx);     // sub_8246A860
void XAudioCodecOnSubmit(lifted::Regs& ctx);            // sub_82469AF8 (harness)

// The output matrix and the volume meter.
void XAudioMatrixInit(lifted::Regs& ctx);       // sub_82465C30
void XAudioMatrixSetMatrix(lifted::Regs& ctx);  // sub_82465E50
void XAudioMatrixSetLevels(lifted::Regs& ctx);  // sub_82466188
void XAudioMatrixMix(lifted::Regs& ctx);        // sub_82466440
void XAudioMatrixMixRamp(lifted::Regs& ctx);    // sub_82466310 (harness)
void XAudioMeterProcess(lifted::Regs& ctx);     // sub_825A6210

// The render driver's set-up and the reverb's small parts.
void XAudioRenderDriverSetUp(lifted::Regs& ctx);   // sub_824657C8
void XAudioReverbInnerDestroy(lifted::Regs& ctx);  // sub_82465410
void XAudioReverbDestruct(lifted::Regs& ctx);      // sub_824656B8
void XAudioReverbDefaults(lifted::Regs& ctx);      // sub_82467538
void XAudioLargestPrimeBelow(lifted::Regs& ctx);   // sub_8246DFE8
void XAudioFilterCoefficient(lifted::Regs& ctx);   // sub_824675B8
void XAudioReverbSetParameters(lifted::Regs& ctx);  // sub_82462148
void XAudioI3dl2ToNative(lifted::Regs& ctx);        // sub_82469418
void XAudioPcmCodecProcess(lifted::Regs& ctx);      // sub_8246D860
void XAudioFillDelayLine2048(lifted::Regs& ctx);   // sub_82467720
void XAudioFillDelayLine64(lifted::Regs& ctx);     // sub_82467788
void XAudioFillDelayLine16(lifted::Regs& ctx);     // sub_824677F0
void XAudioFillDelayLine256(lifted::Regs& ctx);    // sub_82467858
void XAudioFillDelayLine32(lifted::Regs& ctx);     // sub_824678C0
void XAudioFillDelayLine128(lifted::Regs& ctx);    // sub_82467928
void XAudioFillDelayLine512(lifted::Regs& ctx);    // sub_82467990

// The resampler (src/xaudio/resampler.h).
void XAudioResamplerProcess(lifted::Regs& ctx);      // sub_8246F140
void XAudioResampleMono(lifted::Regs& ctx);          // sub_8247E188
void XAudioResampleStereo(lifted::Regs& ctx);        // sub_8247E260
void XAudioResampleMonoScalar(lifted::Regs& ctx);    // sub_82473E48
void XAudioResampleStereoScalar(lifted::Regs& ctx);  // sub_82474188
void XAudioCopyStereo(lifted::Regs& ctx);            // sub_8247F558
void XAudioCopyStereoScalar(lifted::Regs& ctx);      // sub_82478E60

// The reverb core's stages (src/xaudio/reverb_stages.h): the harness's register
// steps (no adapters: only the native ReverbCore::Process calls the stages).
void XAudioReverbLowPass(lifted::Regs& ctx);    // sub_82462BC8
void XAudioReverbDelay512(lifted::Regs& ctx);   // sub_82462CF8
void XAudioReverbDelay2048(lifted::Regs& ctx);  // sub_82463060
void XAudioReverbDelay1024(lifted::Regs& ctx);  // sub_82463578
void XAudioReverbAllPass128(lifted::Regs& ctx);    // sub_82462DC8
void XAudioReverbAllPass256(lifted::Regs& ctx);    // sub_82463130
void XAudioReverbAllPass512(lifted::Regs& ctx);    // sub_824632A0
void XAudioReverbAllPass512To(lifted::Regs& ctx);  // sub_82463410
void XAudioReverbCrossfadeDelay(lifted::Regs& ctx);     // sub_824624F8
void XAudioReverbCrossfadeDelay256(lifted::Regs& ctx);  // sub_82463620

// The reverb core (src/xaudio/reverb_core.h).
void XAudioReverbCoreProcess(lifted::Regs& ctx);  // sub_82463B90
void XAudioReverbCoreApply(lifted::Regs& ctx);    // sub_82467A18
void XAudioReverbCoreInit(lifted::Regs& ctx);     // sub_82468848

// The reverb's construction (src/xaudio/objects.h).
void XAudioI3dl2ParamsConstruct(lifted::Regs& ctx);  // sub_82469848
void XAudioReverbConstruct(lifted::Regs& ctx);       // sub_82465608
void XAudioReverbCreate(lifted::Regs& ctx);          // sub_82465710

// The reverb's pass (src/xaudio/objects.h; the spin lock taken by this
// thread as the game does).
void XAudioI3dl2SetParameter(lifted::Regs& ctx);  // sub_824696F0
void XAudioReverbRun(lifted::Regs& ctx);          // sub_82465258
void XAudioReverbProcess(lifted::Regs& ctx);      // sub_82465428

}  // namespace hooks
