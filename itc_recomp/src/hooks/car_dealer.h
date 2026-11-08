// The car dealer's register steps (src/car_dealer/,
// docs/functions/car-dealer.md), shared by the adapters
// (src/hooks/car_dealer.cpp, behind --native_car_dealer) and the harness
// (without the flag). Each takes the registers its original takes and
// leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The machine's set-up and handlers (r3 the machine, r4 the mode where the
// original reads it).
void CarDealerInitStates(PPCContext& ctx, uint8_t* base);
void CarDealerCreate(PPCContext& ctx, uint8_t* base);
void CarDealerOpenMovie(PPCContext& ctx, uint8_t* base);
void CarDealerWaitForLoad(PPCContext& ctx, uint8_t* base);
void CarDealerAfterDetail(PPCContext& ctx, uint8_t* base);
// The pages (r3 the mode).
void CarDealerPage4(PPCContext& ctx, uint8_t* base);
// The helpers.
void CarDealerCheckLimits(PPCContext& ctx, uint8_t* base);
void CarDealerReloadStandModel(PPCContext& ctx, uint8_t* base);
void CarDealerSetKindBar(PPCContext& ctx, uint8_t* base);
void CarDealerSetColorBar(PPCContext& ctx, uint8_t* base);
void CarDealerSetScrollArrows(PPCContext& ctx, uint8_t* base);
void CarDealerSetHeader(PPCContext& ctx, uint8_t* base);
void CarDealerSetPrompts(PPCContext& ctx, uint8_t* base);
void HelpLineSet(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
