// The originals our C++ still called, lifted (lifted/originals.cpp):
// written by itc_recomp/tools/lift_originals.py.

#pragma once

#include "lifted/lifted.h"
#include "lifted/regs.h"
#include "lifted/sdk.h"

namespace lifted::original {

// sub_82091A60's original.
Result e82091A60(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82091A60).
void c82091A60(PPCContext& ctx, uint8_t* base);
inline void c82091A60(Regs& regs, uint8_t* base) { regs.Apply(e82091A60(base, regs.ToArgs())); }
// sub_820920D8's original.
Result e820920D8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820920D8).
void c820920D8(PPCContext& ctx, uint8_t* base);
inline void c820920D8(Regs& regs, uint8_t* base) { regs.Apply(e820920D8(base, regs.ToArgs())); }
// sub_82099B18's original.
Result e82099B18(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82099B18).
void c82099B18(PPCContext& ctx, uint8_t* base);
inline void c82099B18(Regs& regs, uint8_t* base) { regs.Apply(e82099B18(base, regs.ToArgs())); }
// sub_8209A080's original.
Result e8209A080(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8209A080).
void c8209A080(PPCContext& ctx, uint8_t* base);
inline void c8209A080(Regs& regs, uint8_t* base) { regs.Apply(e8209A080(base, regs.ToArgs())); }
// sub_8209BD50's original.
Result e8209BD50(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8209BD50).
void c8209BD50(PPCContext& ctx, uint8_t* base);
inline void c8209BD50(Regs& regs, uint8_t* base) { regs.Apply(e8209BD50(base, regs.ToArgs())); }
// sub_8209F738's original.
Result e8209F738(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8209F738).
void c8209F738(PPCContext& ctx, uint8_t* base);
inline void c8209F738(Regs& regs, uint8_t* base) { regs.Apply(e8209F738(base, regs.ToArgs())); }
// sub_820A2D30's original.
Result e820A2D30(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820A2D30).
void c820A2D30(PPCContext& ctx, uint8_t* base);
inline void c820A2D30(Regs& regs, uint8_t* base) { regs.Apply(e820A2D30(base, regs.ToArgs())); }
// sub_820A8F80's original.
Result e820A8F80(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820A8F80).
void c820A8F80(PPCContext& ctx, uint8_t* base);
inline void c820A8F80(Regs& regs, uint8_t* base) { regs.Apply(e820A8F80(base, regs.ToArgs())); }
// sub_820A9198's original.
Result e820A9198(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820A9198).
void c820A9198(PPCContext& ctx, uint8_t* base);
inline void c820A9198(Regs& regs, uint8_t* base) { regs.Apply(e820A9198(base, regs.ToArgs())); }
// sub_820AC7F0's original.
Result e820AC7F0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820AC7F0).
void c820AC7F0(PPCContext& ctx, uint8_t* base);
inline void c820AC7F0(Regs& regs, uint8_t* base) { regs.Apply(e820AC7F0(base, regs.ToArgs())); }
// sub_820AC8A0's original.
Result e820AC8A0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820AC8A0).
void c820AC8A0(PPCContext& ctx, uint8_t* base);
inline void c820AC8A0(Regs& regs, uint8_t* base) { regs.Apply(e820AC8A0(base, regs.ToArgs())); }
// sub_820B18B8's original.
Result e820B18B8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B18B8).
void c820B18B8(PPCContext& ctx, uint8_t* base);
inline void c820B18B8(Regs& regs, uint8_t* base) { regs.Apply(e820B18B8(base, regs.ToArgs())); }
// sub_820B2100's original.
Result e820B2100(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B2100).
void c820B2100(PPCContext& ctx, uint8_t* base);
inline void c820B2100(Regs& regs, uint8_t* base) { regs.Apply(e820B2100(base, regs.ToArgs())); }
// sub_820B5670's original.
Result e820B5670(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B5670).
void c820B5670(PPCContext& ctx, uint8_t* base);
inline void c820B5670(Regs& regs, uint8_t* base) { regs.Apply(e820B5670(base, regs.ToArgs())); }
// sub_820B5B08's original.
Result e820B5B08(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B5B08).
void c820B5B08(PPCContext& ctx, uint8_t* base);
inline void c820B5B08(Regs& regs, uint8_t* base) { regs.Apply(e820B5B08(base, regs.ToArgs())); }
// sub_820B5FD8's original.
Result e820B5FD8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B5FD8).
void c820B5FD8(PPCContext& ctx, uint8_t* base);
inline void c820B5FD8(Regs& regs, uint8_t* base) { regs.Apply(e820B5FD8(base, regs.ToArgs())); }
// sub_820B6498's original.
Result e820B6498(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B6498).
void c820B6498(PPCContext& ctx, uint8_t* base);
inline void c820B6498(Regs& regs, uint8_t* base) { regs.Apply(e820B6498(base, regs.ToArgs())); }
// sub_820B7998's original.
Result e820B7998(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B7998).
void c820B7998(PPCContext& ctx, uint8_t* base);
inline void c820B7998(Regs& regs, uint8_t* base) { regs.Apply(e820B7998(base, regs.ToArgs())); }
// sub_820B81C8's original.
Result e820B81C8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820B81C8).
void c820B81C8(PPCContext& ctx, uint8_t* base);
inline void c820B81C8(Regs& regs, uint8_t* base) { regs.Apply(e820B81C8(base, regs.ToArgs())); }
// sub_820BB758's original.
Result e820BB758(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820BB758).
void c820BB758(PPCContext& ctx, uint8_t* base);
inline void c820BB758(Regs& regs, uint8_t* base) { regs.Apply(e820BB758(base, regs.ToArgs())); }
// sub_820BBE30's original.
Result e820BBE30(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820BBE30).
void c820BBE30(PPCContext& ctx, uint8_t* base);
inline void c820BBE30(Regs& regs, uint8_t* base) { regs.Apply(e820BBE30(base, regs.ToArgs())); }
// sub_820BFD30's original.
Result e820BFD30(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820BFD30).
void c820BFD30(PPCContext& ctx, uint8_t* base);
inline void c820BFD30(Regs& regs, uint8_t* base) { regs.Apply(e820BFD30(base, regs.ToArgs())); }
// sub_820C0118's original.
Result e820C0118(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C0118).
void c820C0118(PPCContext& ctx, uint8_t* base);
inline void c820C0118(Regs& regs, uint8_t* base) { regs.Apply(e820C0118(base, regs.ToArgs())); }
// sub_820C1D08's original.
Result e820C1D08(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C1D08).
void c820C1D08(PPCContext& ctx, uint8_t* base);
inline void c820C1D08(Regs& regs, uint8_t* base) { regs.Apply(e820C1D08(base, regs.ToArgs())); }
// sub_820C20E0's original.
Result e820C20E0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C20E0).
void c820C20E0(PPCContext& ctx, uint8_t* base);
inline void c820C20E0(Regs& regs, uint8_t* base) { regs.Apply(e820C20E0(base, regs.ToArgs())); }
// sub_820C23E8's original.
Result e820C23E8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C23E8).
void c820C23E8(PPCContext& ctx, uint8_t* base);
inline void c820C23E8(Regs& regs, uint8_t* base) { regs.Apply(e820C23E8(base, regs.ToArgs())); }
// sub_820C27E8's original.
Result e820C27E8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C27E8).
void c820C27E8(PPCContext& ctx, uint8_t* base);
inline void c820C27E8(Regs& regs, uint8_t* base) { regs.Apply(e820C27E8(base, regs.ToArgs())); }
// sub_820C3608's original.
Result e820C3608(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C3608).
void c820C3608(PPCContext& ctx, uint8_t* base);
inline void c820C3608(Regs& regs, uint8_t* base) { regs.Apply(e820C3608(base, regs.ToArgs())); }
// sub_820C3B78's original.
Result e820C3B78(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C3B78).
void c820C3B78(PPCContext& ctx, uint8_t* base);
inline void c820C3B78(Regs& regs, uint8_t* base) { regs.Apply(e820C3B78(base, regs.ToArgs())); }
// sub_820C3BD0's original.
Result e820C3BD0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C3BD0).
void c820C3BD0(PPCContext& ctx, uint8_t* base);
inline void c820C3BD0(Regs& regs, uint8_t* base) { regs.Apply(e820C3BD0(base, regs.ToArgs())); }
// sub_820C5C88's original.
Result e820C5C88(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820C5C88).
void c820C5C88(PPCContext& ctx, uint8_t* base);
inline void c820C5C88(Regs& regs, uint8_t* base) { regs.Apply(e820C5C88(base, regs.ToArgs())); }
// sub_820CC470's original.
Result e820CC470(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820CC470).
void c820CC470(PPCContext& ctx, uint8_t* base);
inline void c820CC470(Regs& regs, uint8_t* base) { regs.Apply(e820CC470(base, regs.ToArgs())); }
// sub_820D09A0's original.
Result e820D09A0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D09A0).
void c820D09A0(PPCContext& ctx, uint8_t* base);
inline void c820D09A0(Regs& regs, uint8_t* base) { regs.Apply(e820D09A0(base, regs.ToArgs())); }
// sub_820D3318's original.
Result e820D3318(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D3318).
void c820D3318(PPCContext& ctx, uint8_t* base);
inline void c820D3318(Regs& regs, uint8_t* base) { regs.Apply(e820D3318(base, regs.ToArgs())); }
// sub_820D3688's original.
Result e820D3688(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D3688).
void c820D3688(PPCContext& ctx, uint8_t* base);
inline void c820D3688(Regs& regs, uint8_t* base) { regs.Apply(e820D3688(base, regs.ToArgs())); }
// sub_820D3740's original.
Result e820D3740(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D3740).
void c820D3740(PPCContext& ctx, uint8_t* base);
inline void c820D3740(Regs& regs, uint8_t* base) { regs.Apply(e820D3740(base, regs.ToArgs())); }
// sub_820D47A8's original.
Result e820D47A8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D47A8).
void c820D47A8(PPCContext& ctx, uint8_t* base);
inline void c820D47A8(Regs& regs, uint8_t* base) { regs.Apply(e820D47A8(base, regs.ToArgs())); }
// sub_820D4F80's original.
Result e820D4F80(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D4F80).
void c820D4F80(PPCContext& ctx, uint8_t* base);
inline void c820D4F80(Regs& regs, uint8_t* base) { regs.Apply(e820D4F80(base, regs.ToArgs())); }
// sub_820D5890's original.
Result e820D5890(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D5890).
void c820D5890(PPCContext& ctx, uint8_t* base);
inline void c820D5890(Regs& regs, uint8_t* base) { regs.Apply(e820D5890(base, regs.ToArgs())); }
// sub_820D6108's original.
Result e820D6108(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D6108).
void c820D6108(PPCContext& ctx, uint8_t* base);
inline void c820D6108(Regs& regs, uint8_t* base) { regs.Apply(e820D6108(base, regs.ToArgs())); }
// sub_820D63A0's original.
Result e820D63A0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D63A0).
void c820D63A0(PPCContext& ctx, uint8_t* base);
inline void c820D63A0(Regs& regs, uint8_t* base) { regs.Apply(e820D63A0(base, regs.ToArgs())); }
// sub_820D6A70's original.
Result e820D6A70(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D6A70).
void c820D6A70(PPCContext& ctx, uint8_t* base);
inline void c820D6A70(Regs& regs, uint8_t* base) { regs.Apply(e820D6A70(base, regs.ToArgs())); }
// sub_820D7428's original.
Result e820D7428(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D7428).
void c820D7428(PPCContext& ctx, uint8_t* base);
inline void c820D7428(Regs& regs, uint8_t* base) { regs.Apply(e820D7428(base, regs.ToArgs())); }
// sub_820D78B8's original.
Result e820D78B8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D78B8).
void c820D78B8(PPCContext& ctx, uint8_t* base);
inline void c820D78B8(Regs& regs, uint8_t* base) { regs.Apply(e820D78B8(base, regs.ToArgs())); }
// sub_820D7D38's original.
Result e820D7D38(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D7D38).
void c820D7D38(PPCContext& ctx, uint8_t* base);
inline void c820D7D38(Regs& regs, uint8_t* base) { regs.Apply(e820D7D38(base, regs.ToArgs())); }
// sub_820D7EE8's original.
Result e820D7EE8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D7EE8).
void c820D7EE8(PPCContext& ctx, uint8_t* base);
inline void c820D7EE8(Regs& regs, uint8_t* base) { regs.Apply(e820D7EE8(base, regs.ToArgs())); }
// sub_820D8908's original.
Result e820D8908(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D8908).
void c820D8908(PPCContext& ctx, uint8_t* base);
inline void c820D8908(Regs& regs, uint8_t* base) { regs.Apply(e820D8908(base, regs.ToArgs())); }
// sub_820D9570's original.
Result e820D9570(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820D9570).
void c820D9570(PPCContext& ctx, uint8_t* base);
inline void c820D9570(Regs& regs, uint8_t* base) { regs.Apply(e820D9570(base, regs.ToArgs())); }
// sub_820DB290's original.
Result e820DB290(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820DB290).
void c820DB290(PPCContext& ctx, uint8_t* base);
inline void c820DB290(Regs& regs, uint8_t* base) { regs.Apply(e820DB290(base, regs.ToArgs())); }
// sub_820F0070's original.
Result e820F0070(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820F0070).
void c820F0070(PPCContext& ctx, uint8_t* base);
inline void c820F0070(Regs& regs, uint8_t* base) { regs.Apply(e820F0070(base, regs.ToArgs())); }
// sub_820F4128's original.
Result e820F4128(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820F4128).
void c820F4128(PPCContext& ctx, uint8_t* base);
inline void c820F4128(Regs& regs, uint8_t* base) { regs.Apply(e820F4128(base, regs.ToArgs())); }
// sub_820F8618's original.
Result e820F8618(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820F8618).
void c820F8618(PPCContext& ctx, uint8_t* base);
inline void c820F8618(Regs& regs, uint8_t* base) { regs.Apply(e820F8618(base, regs.ToArgs())); }
// sub_820FEFB0's original.
Result e820FEFB0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820FEFB0).
void c820FEFB0(PPCContext& ctx, uint8_t* base);
inline void c820FEFB0(Regs& regs, uint8_t* base) { regs.Apply(e820FEFB0(base, regs.ToArgs())); }
// sub_820FFD88's original.
Result e820FFD88(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_820FFD88).
void c820FFD88(PPCContext& ctx, uint8_t* base);
inline void c820FFD88(Regs& regs, uint8_t* base) { regs.Apply(e820FFD88(base, regs.ToArgs())); }
// sub_8211AED0's original.
Result e8211AED0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8211AED0).
void c8211AED0(PPCContext& ctx, uint8_t* base);
inline void c8211AED0(Regs& regs, uint8_t* base) { regs.Apply(e8211AED0(base, regs.ToArgs())); }
// sub_8211BD10's original.
Result e8211BD10(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8211BD10).
void c8211BD10(PPCContext& ctx, uint8_t* base);
inline void c8211BD10(Regs& regs, uint8_t* base) { regs.Apply(e8211BD10(base, regs.ToArgs())); }
// sub_82128E70's original.
Result e82128E70(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82128E70).
void c82128E70(PPCContext& ctx, uint8_t* base);
inline void c82128E70(Regs& regs, uint8_t* base) { regs.Apply(e82128E70(base, regs.ToArgs())); }
// sub_8213B1E0's original.
Result e8213B1E0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8213B1E0).
void c8213B1E0(PPCContext& ctx, uint8_t* base);
inline void c8213B1E0(Regs& regs, uint8_t* base) { regs.Apply(e8213B1E0(base, regs.ToArgs())); }
// sub_8213B2B8's original.
Result e8213B2B8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8213B2B8).
void c8213B2B8(PPCContext& ctx, uint8_t* base);
inline void c8213B2B8(Regs& regs, uint8_t* base) { regs.Apply(e8213B2B8(base, regs.ToArgs())); }
// sub_82150350's original.
Result e82150350(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82150350).
void c82150350(PPCContext& ctx, uint8_t* base);
inline void c82150350(Regs& regs, uint8_t* base) { regs.Apply(e82150350(base, regs.ToArgs())); }
// sub_8216C2A0's original.
Result e8216C2A0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8216C2A0).
void c8216C2A0(PPCContext& ctx, uint8_t* base);
inline void c8216C2A0(Regs& regs, uint8_t* base) { regs.Apply(e8216C2A0(base, regs.ToArgs())); }
// sub_821715B8's original.
Result e821715B8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_821715B8).
void c821715B8(PPCContext& ctx, uint8_t* base);
inline void c821715B8(Regs& regs, uint8_t* base) { regs.Apply(e821715B8(base, regs.ToArgs())); }
// sub_8217C368's original.
Result e8217C368(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8217C368).
void c8217C368(PPCContext& ctx, uint8_t* base);
inline void c8217C368(Regs& regs, uint8_t* base) { regs.Apply(e8217C368(base, regs.ToArgs())); }
// sub_82294330's original.
Result e82294330(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82294330).
void c82294330(PPCContext& ctx, uint8_t* base);
inline void c82294330(Regs& regs, uint8_t* base) { regs.Apply(e82294330(base, regs.ToArgs())); }
// sub_82295330's original.
Result e82295330(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82295330).
void c82295330(PPCContext& ctx, uint8_t* base);
inline void c82295330(Regs& regs, uint8_t* base) { regs.Apply(e82295330(base, regs.ToArgs())); }
// sub_822A5B78's original.
Result e822A5B78(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_822A5B78).
void c822A5B78(PPCContext& ctx, uint8_t* base);
inline void c822A5B78(Regs& regs, uint8_t* base) { regs.Apply(e822A5B78(base, regs.ToArgs())); }
// sub_822A6008's original.
Result e822A6008(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_822A6008).
void c822A6008(PPCContext& ctx, uint8_t* base);
inline void c822A6008(Regs& regs, uint8_t* base) { regs.Apply(e822A6008(base, regs.ToArgs())); }
// sub_822A8740's original.
Result e822A8740(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_822A8740).
void c822A8740(PPCContext& ctx, uint8_t* base);
inline void c822A8740(Regs& regs, uint8_t* base) { regs.Apply(e822A8740(base, regs.ToArgs())); }
// sub_82594D10's original.
Result e82594D10(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82594D10).
void c82594D10(PPCContext& ctx, uint8_t* base);
inline void c82594D10(Regs& regs, uint8_t* base) { regs.Apply(e82594D10(base, regs.ToArgs())); }
// sub_82596920's original.
Result e82596920(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_82596920).
void c82596920(PPCContext& ctx, uint8_t* base);
inline void c82596920(Regs& regs, uint8_t* base) { regs.Apply(e82596920(base, regs.ToArgs())); }
// sub_825986B8's original.
Result e825986B8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_825986B8).
void c825986B8(PPCContext& ctx, uint8_t* base);
inline void c825986B8(Regs& regs, uint8_t* base) { regs.Apply(e825986B8(base, regs.ToArgs())); }
// sub_825998A0's original.
Result e825998A0(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_825998A0).
void c825998A0(PPCContext& ctx, uint8_t* base);
inline void c825998A0(Regs& regs, uint8_t* base) { regs.Apply(e825998A0(base, regs.ToArgs())); }
// sub_8259F770's original.
Result e8259F770(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8259F770).
void c8259F770(PPCContext& ctx, uint8_t* base);
inline void c8259F770(Regs& regs, uint8_t* base) { regs.Apply(e8259F770(base, regs.ToArgs())); }
// sub_8259FB30's original.
Result e8259FB30(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8259FB30).
void c8259FB30(PPCContext& ctx, uint8_t* base);
inline void c8259FB30(Regs& regs, uint8_t* base) { regs.Apply(e8259FB30(base, regs.ToArgs())); }
// sub_8259FC18's original.
Result e8259FC18(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_8259FC18).
void c8259FC18(PPCContext& ctx, uint8_t* base);
inline void c8259FC18(Regs& regs, uint8_t* base) { regs.Apply(e8259FC18(base, regs.ToArgs())); }
// sub_825A7068's original.
Result e825A7068(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_825A7068).
void c825A7068(PPCContext& ctx, uint8_t* base);
inline void c825A7068(Regs& regs, uint8_t* base) { regs.Apply(e825A7068(base, regs.ToArgs())); }
// sub_825A70E8's original.
Result e825A70E8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_825A70E8).
void c825A70E8(PPCContext& ctx, uint8_t* base);
inline void c825A70E8(Regs& regs, uint8_t* base) { regs.Apply(e825A70E8(base, regs.ToArgs())); }
// sub_825A7410's original.
Result e825A7410(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_825A7410).
void c825A7410(PPCContext& ctx, uint8_t* base);
inline void c825A7410(Regs& regs, uint8_t* base) { regs.Apply(e825A7410(base, regs.ToArgs())); }
// sub_825BE708's original.
Result e825BE708(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_825BE708).
void c825BE708(PPCContext& ctx, uint8_t* base);
inline void c825BE708(Regs& regs, uint8_t* base) { regs.Apply(e825BE708(base, regs.ToArgs())); }
// sub_825C31D8's original.
Result e825C31D8(uint8_t* base, const Args& a);
// The same for code still holding a context (in place of __imp__sub_825C31D8).
void c825C31D8(PPCContext& ctx, uint8_t* base);
inline void c825C31D8(Regs& regs, uint8_t* base) { regs.Apply(e825C31D8(base, regs.ToArgs())); }

}  // namespace lifted::original
