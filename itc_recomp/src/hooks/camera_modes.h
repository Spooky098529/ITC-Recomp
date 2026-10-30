// The camera modes' register steps (src/hooks/camera_modes.cpp): what each
// adapter does with --native_camera_modes on, shared with the harness
// (src/harnesses/camera_modes.cpp), so they stay when an adapter goes.

#pragma once

#include "guest.h"

namespace hooks {

void CameraModesStep82152400(PPCContext& ctx, uint8_t* base);
void CameraModesStep821524B8(PPCContext& ctx, uint8_t* base);
void CameraModesStep821524D8(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153160(PPCContext& ctx, uint8_t* base);
void CameraModesStep821524F0(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152548(PPCContext& ctx, uint8_t* base);
void CameraModesStep821525F8(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152698(PPCContext& ctx, uint8_t* base);
void CameraModesStep82090000(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153228(PPCContext& ctx, uint8_t* base);
void CameraModesStep821544E0(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154458(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154378(PPCContext& ctx, uint8_t* base);
void CameraModesStep821543E8(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154300(PPCContext& ctx, uint8_t* base);
void CameraModesStep82150078(PPCContext& ctx, uint8_t* base);
void CameraModesStep82150300(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152108(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152290(PPCContext& ctx, uint8_t* base);
void CameraModesStep82150B10(PPCContext& ctx, uint8_t* base);
void CameraModesStep82150878(PPCContext& ctx, uint8_t* base);
void CameraModesStep821507D8(PPCContext& ctx, uint8_t* base);
void CameraModesStep82150960(PPCContext& ctx, uint8_t* base);
void CameraModesStep82150A98(PPCContext& ctx, uint8_t* base);
void CameraModesStep82150A10(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152E08(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152078(PPCContext& ctx, uint8_t* base);
void CameraModesStep82211848(PPCContext& ctx, uint8_t* base);
void CameraModesStep82211AB0(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153290(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152CD8(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152788(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152E78(PPCContext& ctx, uint8_t* base);
void CameraModesStep82158F70(PPCContext& ctx, uint8_t* base);
void CameraModesStep82152338(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153098(PPCContext& ctx, uint8_t* base);
void CameraModesStep82156B68(PPCContext& ctx, uint8_t* base);
void CameraModesStep82155800(PPCContext& ctx, uint8_t* base);
void CameraModesStep82155A28(PPCContext& ctx, uint8_t* base);
void CameraModesStep821561F8(PPCContext& ctx, uint8_t* base);
void CameraModesStep820FE830(PPCContext& ctx, uint8_t* base);
void CameraModesStep820F9B48(PPCContext& ctx, uint8_t* base);
void CameraModesStep82156980(PPCContext& ctx, uint8_t* base);
void CameraModesStep820AC248(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153478(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153640(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154548(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154808(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154A48(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153A50(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154D38(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153E58(PPCContext& ctx, uint8_t* base);
void CameraModesStep82155E58(PPCContext& ctx, uint8_t* base);
void CameraModesStep822118E0(PPCContext& ctx, uint8_t* base);
void CameraModesStep82211980(PPCContext& ctx, uint8_t* base);
void CameraModesStep82155288(PPCContext& ctx, uint8_t* base);
void CameraModesStep821562F8(PPCContext& ctx, uint8_t* base);
void CameraModesStep821593D8(PPCContext& ctx, uint8_t* base);
void CameraModesStep8217BE98(PPCContext& ctx, uint8_t* base);
void CameraModesStep82159950(PPCContext& ctx, uint8_t* base);
void CameraModesStep821591F0(PPCContext& ctx, uint8_t* base);
void CameraModesStep821596B0(PPCContext& ctx, uint8_t* base);
void CameraModesStep821597F0(PPCContext& ctx, uint8_t* base);
void CameraModesStep821594B8(PPCContext& ctx, uint8_t* base);
// CameraDirector::Update (sub_821594B8) as frame interpolation's wrapper
// runs it on logic passes: the step with --native_camera_modes, else the
// original.
void CameraModesUpdate(PPCContext& ctx, uint8_t* base);

// The views' camera constructors' stand-in for the SetMode harness: while
// set, the six constructors' adapters (and SetMode's native calls) answer
// with it (the function, r3, r4, r5, r1 and lr; its answer r3). nullptr
// puts them back.
using CameraConstructStandIn = uint32_t (*)(uint32_t function, uint32_t r3, uint32_t r4,
                                            uint32_t r5, uint32_t r1, uint32_t lr);
void SetCameraConstructStandIn(CameraConstructStandIn stand_in);
void CameraModesStep82156CC0(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
