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
void CameraModesStep820FE830(PPCContext& ctx, uint8_t* base);
void CameraModesStep820F9B48(PPCContext& ctx, uint8_t* base);
void CameraModesStep82156980(PPCContext& ctx, uint8_t* base);
void CameraModesStep820AC248(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153478(PPCContext& ctx, uint8_t* base);
void CameraModesStep82153640(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154548(PPCContext& ctx, uint8_t* base);
void CameraModesStep82154808(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
