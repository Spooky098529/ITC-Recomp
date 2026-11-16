// The originals our C++ still called, lifted (lifted/originals.cpp):
// written by itc_recomp/tools/lift_originals.py.

#pragma once

#include "lifted/lifted.h"
#include "lifted/regs.h"
#include "lifted/sdk.h"

namespace lifted::original {

// sub_82090108's original.
Result e82090108(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090108).
inline void c82090108(Regs& regs, uint8_t* base) { regs.Apply(e82090108(base, regs.ToArgs())); }
// sub_820902B0's original.
Result e820902B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820902B0).
inline void c820902B0(Regs& regs, uint8_t* base) { regs.Apply(e820902B0(base, regs.ToArgs())); }
// sub_820902C8's original.
Result e820902C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820902C8).
inline void c820902C8(Regs& regs, uint8_t* base) { regs.Apply(e820902C8(base, regs.ToArgs())); }
// sub_82090760's original.
Result e82090760(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090760).
inline void c82090760(Regs& regs, uint8_t* base) { regs.Apply(e82090760(base, regs.ToArgs())); }
// sub_82090768's original.
Result e82090768(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090768).
inline void c82090768(Regs& regs, uint8_t* base) { regs.Apply(e82090768(base, regs.ToArgs())); }
// sub_82090770's original.
Result e82090770(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090770).
inline void c82090770(Regs& regs, uint8_t* base) { regs.Apply(e82090770(base, regs.ToArgs())); }
// sub_82090788's original.
Result e82090788(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090788).
inline void c82090788(Regs& regs, uint8_t* base) { regs.Apply(e82090788(base, regs.ToArgs())); }
// sub_82090828's original.
Result e82090828(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090828).
inline void c82090828(Regs& regs, uint8_t* base) { regs.Apply(e82090828(base, regs.ToArgs())); }
// sub_82090830's original.
Result e82090830(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090830).
inline void c82090830(Regs& regs, uint8_t* base) { regs.Apply(e82090830(base, regs.ToArgs())); }
// sub_82090838's original.
Result e82090838(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090838).
inline void c82090838(Regs& regs, uint8_t* base) { regs.Apply(e82090838(base, regs.ToArgs())); }
// sub_82090860's original.
Result e82090860(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090860).
inline void c82090860(Regs& regs, uint8_t* base) { regs.Apply(e82090860(base, regs.ToArgs())); }
// sub_82090868's original.
Result e82090868(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090868).
inline void c82090868(Regs& regs, uint8_t* base) { regs.Apply(e82090868(base, regs.ToArgs())); }
// sub_820908E0's original.
Result e820908E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820908E0).
inline void c820908E0(Regs& regs, uint8_t* base) { regs.Apply(e820908E0(base, regs.ToArgs())); }
// sub_820908E8's original.
Result e820908E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820908E8).
inline void c820908E8(Regs& regs, uint8_t* base) { regs.Apply(e820908E8(base, regs.ToArgs())); }
// sub_82090908's original.
Result e82090908(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090908).
inline void c82090908(Regs& regs, uint8_t* base) { regs.Apply(e82090908(base, regs.ToArgs())); }
// sub_820909B8's original.
Result e820909B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820909B8).
inline void c820909B8(Regs& regs, uint8_t* base) { regs.Apply(e820909B8(base, regs.ToArgs())); }
// sub_82090CC0's original.
Result e82090CC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82090CC0).
inline void c82090CC0(Regs& regs, uint8_t* base) { regs.Apply(e82090CC0(base, regs.ToArgs())); }
// sub_82091050's original.
Result e82091050(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091050).
inline void c82091050(Regs& regs, uint8_t* base) { regs.Apply(e82091050(base, regs.ToArgs())); }
// sub_82091160's original.
Result e82091160(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091160).
inline void c82091160(Regs& regs, uint8_t* base) { regs.Apply(e82091160(base, regs.ToArgs())); }
// sub_82091328's original.
Result e82091328(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091328).
inline void c82091328(Regs& regs, uint8_t* base) { regs.Apply(e82091328(base, regs.ToArgs())); }
// sub_82091720's original.
Result e82091720(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091720).
inline void c82091720(Regs& regs, uint8_t* base) { regs.Apply(e82091720(base, regs.ToArgs())); }
// sub_82091808's original.
Result e82091808(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091808).
inline void c82091808(Regs& regs, uint8_t* base) { regs.Apply(e82091808(base, regs.ToArgs())); }
// sub_82091A60's original.
Result e82091A60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091A60).
inline void c82091A60(Regs& regs, uint8_t* base) { regs.Apply(e82091A60(base, regs.ToArgs())); }
// sub_82091A98's original.
Result e82091A98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091A98).
inline void c82091A98(Regs& regs, uint8_t* base) { regs.Apply(e82091A98(base, regs.ToArgs())); }
// sub_82091E58's original.
Result e82091E58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091E58).
inline void c82091E58(Regs& regs, uint8_t* base) { regs.Apply(e82091E58(base, regs.ToArgs())); }
// sub_82091FF0's original.
Result e82091FF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82091FF0).
inline void c82091FF0(Regs& regs, uint8_t* base) { regs.Apply(e82091FF0(base, regs.ToArgs())); }
// sub_820920D8's original.
Result e820920D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820920D8).
inline void c820920D8(Regs& regs, uint8_t* base) { regs.Apply(e820920D8(base, regs.ToArgs())); }
// sub_820922B8's original.
Result e820922B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820922B8).
inline void c820922B8(Regs& regs, uint8_t* base) { regs.Apply(e820922B8(base, regs.ToArgs())); }
// sub_82092710's original.
Result e82092710(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82092710).
inline void c82092710(Regs& regs, uint8_t* base) { regs.Apply(e82092710(base, regs.ToArgs())); }
// sub_82092878's original.
Result e82092878(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82092878).
inline void c82092878(Regs& regs, uint8_t* base) { regs.Apply(e82092878(base, regs.ToArgs())); }
// sub_82092968's original.
Result e82092968(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82092968).
inline void c82092968(Regs& regs, uint8_t* base) { regs.Apply(e82092968(base, regs.ToArgs())); }
// sub_82092B78's original.
Result e82092B78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82092B78).
inline void c82092B78(Regs& regs, uint8_t* base) { regs.Apply(e82092B78(base, regs.ToArgs())); }
// sub_82092E70's original.
Result e82092E70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82092E70).
inline void c82092E70(Regs& regs, uint8_t* base) { regs.Apply(e82092E70(base, regs.ToArgs())); }
// sub_82092F98's original.
Result e82092F98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82092F98).
inline void c82092F98(Regs& regs, uint8_t* base) { regs.Apply(e82092F98(base, regs.ToArgs())); }
// sub_82093740's original.
Result e82093740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82093740).
inline void c82093740(Regs& regs, uint8_t* base) { regs.Apply(e82093740(base, regs.ToArgs())); }
// sub_820938C0's original.
Result e820938C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820938C0).
inline void c820938C0(Regs& regs, uint8_t* base) { regs.Apply(e820938C0(base, regs.ToArgs())); }
// sub_82093BE0's original.
Result e82093BE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82093BE0).
inline void c82093BE0(Regs& regs, uint8_t* base) { regs.Apply(e82093BE0(base, regs.ToArgs())); }
// sub_82094110's original.
Result e82094110(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82094110).
inline void c82094110(Regs& regs, uint8_t* base) { regs.Apply(e82094110(base, regs.ToArgs())); }
// sub_82094350's original.
Result e82094350(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82094350).
inline void c82094350(Regs& regs, uint8_t* base) { regs.Apply(e82094350(base, regs.ToArgs())); }
// sub_820943B0's original.
Result e820943B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820943B0).
inline void c820943B0(Regs& regs, uint8_t* base) { regs.Apply(e820943B0(base, regs.ToArgs())); }
// sub_820943C8's original.
Result e820943C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820943C8).
inline void c820943C8(Regs& regs, uint8_t* base) { regs.Apply(e820943C8(base, regs.ToArgs())); }
// sub_820944C8's original.
Result e820944C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820944C8).
inline void c820944C8(Regs& regs, uint8_t* base) { regs.Apply(e820944C8(base, regs.ToArgs())); }
// sub_820944E0's original.
Result e820944E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820944E0).
inline void c820944E0(Regs& regs, uint8_t* base) { regs.Apply(e820944E0(base, regs.ToArgs())); }
// sub_82094590's original.
Result e82094590(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82094590).
inline void c82094590(Regs& regs, uint8_t* base) { regs.Apply(e82094590(base, regs.ToArgs())); }
// sub_82094AB0's original.
Result e82094AB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82094AB0).
inline void c82094AB0(Regs& regs, uint8_t* base) { regs.Apply(e82094AB0(base, regs.ToArgs())); }
// sub_82094AB8's original.
Result e82094AB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82094AB8).
inline void c82094AB8(Regs& regs, uint8_t* base) { regs.Apply(e82094AB8(base, regs.ToArgs())); }
// sub_82094B10's original.
Result e82094B10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82094B10).
inline void c82094B10(Regs& regs, uint8_t* base) { regs.Apply(e82094B10(base, regs.ToArgs())); }
// sub_82095958's original.
Result e82095958(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82095958).
inline void c82095958(Regs& regs, uint8_t* base) { regs.Apply(e82095958(base, regs.ToArgs())); }
// sub_82096030's original.
Result e82096030(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82096030).
inline void c82096030(Regs& regs, uint8_t* base) { regs.Apply(e82096030(base, regs.ToArgs())); }
// sub_82096218's original.
Result e82096218(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82096218).
inline void c82096218(Regs& regs, uint8_t* base) { regs.Apply(e82096218(base, regs.ToArgs())); }
// sub_820962B8's original.
Result e820962B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820962B8).
inline void c820962B8(Regs& regs, uint8_t* base) { regs.Apply(e820962B8(base, regs.ToArgs())); }
// sub_82096330's original.
Result e82096330(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82096330).
inline void c82096330(Regs& regs, uint8_t* base) { regs.Apply(e82096330(base, regs.ToArgs())); }
// sub_82096340's original.
Result e82096340(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82096340).
inline void c82096340(Regs& regs, uint8_t* base) { regs.Apply(e82096340(base, regs.ToArgs())); }
// sub_82096350's original.
Result e82096350(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82096350).
inline void c82096350(Regs& regs, uint8_t* base) { regs.Apply(e82096350(base, regs.ToArgs())); }
// sub_82096360's original.
Result e82096360(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82096360).
inline void c82096360(Regs& regs, uint8_t* base) { regs.Apply(e82096360(base, regs.ToArgs())); }
// sub_82096418's original.
Result e82096418(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82096418).
inline void c82096418(Regs& regs, uint8_t* base) { regs.Apply(e82096418(base, regs.ToArgs())); }
// sub_82098630's original.
Result e82098630(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82098630).
inline void c82098630(Regs& regs, uint8_t* base) { regs.Apply(e82098630(base, regs.ToArgs())); }
// sub_820988B8's original.
Result e820988B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820988B8).
inline void c820988B8(Regs& regs, uint8_t* base) { regs.Apply(e820988B8(base, regs.ToArgs())); }
// sub_820988C8's original.
Result e820988C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820988C8).
inline void c820988C8(Regs& regs, uint8_t* base) { regs.Apply(e820988C8(base, regs.ToArgs())); }
// sub_82098940's original.
Result e82098940(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82098940).
inline void c82098940(Regs& regs, uint8_t* base) { regs.Apply(e82098940(base, regs.ToArgs())); }
// sub_82098A98's original.
Result e82098A98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82098A98).
inline void c82098A98(Regs& regs, uint8_t* base) { regs.Apply(e82098A98(base, regs.ToArgs())); }
// sub_82098CB8's original.
Result e82098CB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82098CB8).
inline void c82098CB8(Regs& regs, uint8_t* base) { regs.Apply(e82098CB8(base, regs.ToArgs())); }
// sub_82098CD0's original.
Result e82098CD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82098CD0).
inline void c82098CD0(Regs& regs, uint8_t* base) { regs.Apply(e82098CD0(base, regs.ToArgs())); }
// sub_82098E80's original.
Result e82098E80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82098E80).
inline void c82098E80(Regs& regs, uint8_t* base) { regs.Apply(e82098E80(base, regs.ToArgs())); }
// sub_82098FE0's original.
Result e82098FE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82098FE0).
inline void c82098FE0(Regs& regs, uint8_t* base) { regs.Apply(e82098FE0(base, regs.ToArgs())); }
// sub_820991E0's original.
Result e820991E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820991E0).
inline void c820991E0(Regs& regs, uint8_t* base) { regs.Apply(e820991E0(base, regs.ToArgs())); }
// sub_820993E8's original.
Result e820993E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820993E8).
inline void c820993E8(Regs& regs, uint8_t* base) { regs.Apply(e820993E8(base, regs.ToArgs())); }
// sub_82099430's original.
Result e82099430(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82099430).
inline void c82099430(Regs& regs, uint8_t* base) { regs.Apply(e82099430(base, regs.ToArgs())); }
// sub_82099A98's original.
Result e82099A98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82099A98).
inline void c82099A98(Regs& regs, uint8_t* base) { regs.Apply(e82099A98(base, regs.ToArgs())); }
// sub_82099B18's original.
Result e82099B18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82099B18).
inline void c82099B18(Regs& regs, uint8_t* base) { regs.Apply(e82099B18(base, regs.ToArgs())); }
// sub_82099C08's original.
Result e82099C08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82099C08).
inline void c82099C08(Regs& regs, uint8_t* base) { regs.Apply(e82099C08(base, regs.ToArgs())); }
// sub_82099FB0's original.
Result e82099FB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82099FB0).
inline void c82099FB0(Regs& regs, uint8_t* base) { regs.Apply(e82099FB0(base, regs.ToArgs())); }
// sub_8209A080's original.
Result e8209A080(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209A080).
inline void c8209A080(Regs& regs, uint8_t* base) { regs.Apply(e8209A080(base, regs.ToArgs())); }
// sub_8209A458's original.
Result e8209A458(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209A458).
inline void c8209A458(Regs& regs, uint8_t* base) { regs.Apply(e8209A458(base, regs.ToArgs())); }
// sub_8209A930's original.
Result e8209A930(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209A930).
inline void c8209A930(Regs& regs, uint8_t* base) { regs.Apply(e8209A930(base, regs.ToArgs())); }
// sub_8209A9F0's original.
Result e8209A9F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209A9F0).
inline void c8209A9F0(Regs& regs, uint8_t* base) { regs.Apply(e8209A9F0(base, regs.ToArgs())); }
// sub_8209AAA8's original.
Result e8209AAA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209AAA8).
inline void c8209AAA8(Regs& regs, uint8_t* base) { regs.Apply(e8209AAA8(base, regs.ToArgs())); }
// sub_8209AB48's original.
Result e8209AB48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209AB48).
inline void c8209AB48(Regs& regs, uint8_t* base) { regs.Apply(e8209AB48(base, regs.ToArgs())); }
// sub_8209B378's original.
Result e8209B378(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209B378).
inline void c8209B378(Regs& regs, uint8_t* base) { regs.Apply(e8209B378(base, regs.ToArgs())); }
// sub_8209B3D0's original.
Result e8209B3D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209B3D0).
inline void c8209B3D0(Regs& regs, uint8_t* base) { regs.Apply(e8209B3D0(base, regs.ToArgs())); }
// sub_8209BD50's original.
Result e8209BD50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209BD50).
inline void c8209BD50(Regs& regs, uint8_t* base) { regs.Apply(e8209BD50(base, regs.ToArgs())); }
// sub_8209BE88's original.
Result e8209BE88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209BE88).
inline void c8209BE88(Regs& regs, uint8_t* base) { regs.Apply(e8209BE88(base, regs.ToArgs())); }
// sub_8209C6E0's original.
Result e8209C6E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209C6E0).
inline void c8209C6E0(Regs& regs, uint8_t* base) { regs.Apply(e8209C6E0(base, regs.ToArgs())); }
// sub_8209CC38's original.
Result e8209CC38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209CC38).
inline void c8209CC38(Regs& regs, uint8_t* base) { regs.Apply(e8209CC38(base, regs.ToArgs())); }
// sub_8209CE68's original.
Result e8209CE68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209CE68).
inline void c8209CE68(Regs& regs, uint8_t* base) { regs.Apply(e8209CE68(base, regs.ToArgs())); }
// sub_8209D130's original.
Result e8209D130(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209D130).
inline void c8209D130(Regs& regs, uint8_t* base) { regs.Apply(e8209D130(base, regs.ToArgs())); }
// sub_8209D730's original.
Result e8209D730(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209D730).
inline void c8209D730(Regs& regs, uint8_t* base) { regs.Apply(e8209D730(base, regs.ToArgs())); }
// sub_8209D7B0's original.
Result e8209D7B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209D7B0).
inline void c8209D7B0(Regs& regs, uint8_t* base) { regs.Apply(e8209D7B0(base, regs.ToArgs())); }
// sub_8209D868's original.
Result e8209D868(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209D868).
inline void c8209D868(Regs& regs, uint8_t* base) { regs.Apply(e8209D868(base, regs.ToArgs())); }
// sub_8209DB20's original.
Result e8209DB20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209DB20).
inline void c8209DB20(Regs& regs, uint8_t* base) { regs.Apply(e8209DB20(base, regs.ToArgs())); }
// sub_8209DB90's original.
Result e8209DB90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209DB90).
inline void c8209DB90(Regs& regs, uint8_t* base) { regs.Apply(e8209DB90(base, regs.ToArgs())); }
// sub_8209DD80's original.
Result e8209DD80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209DD80).
inline void c8209DD80(Regs& regs, uint8_t* base) { regs.Apply(e8209DD80(base, regs.ToArgs())); }
// sub_8209DD98's original.
Result e8209DD98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209DD98).
inline void c8209DD98(Regs& regs, uint8_t* base) { regs.Apply(e8209DD98(base, regs.ToArgs())); }
// sub_8209DE48's original.
Result e8209DE48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209DE48).
inline void c8209DE48(Regs& regs, uint8_t* base) { regs.Apply(e8209DE48(base, regs.ToArgs())); }
// sub_8209DFA8's original.
Result e8209DFA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209DFA8).
inline void c8209DFA8(Regs& regs, uint8_t* base) { regs.Apply(e8209DFA8(base, regs.ToArgs())); }
// sub_8209EEC8's original.
Result e8209EEC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209EEC8).
inline void c8209EEC8(Regs& regs, uint8_t* base) { regs.Apply(e8209EEC8(base, regs.ToArgs())); }
// sub_8209F430's original.
Result e8209F430(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F430).
inline void c8209F430(Regs& regs, uint8_t* base) { regs.Apply(e8209F430(base, regs.ToArgs())); }
// sub_8209F5E0's original.
Result e8209F5E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F5E0).
inline void c8209F5E0(Regs& regs, uint8_t* base) { regs.Apply(e8209F5E0(base, regs.ToArgs())); }
// sub_8209F718's original.
Result e8209F718(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F718).
inline void c8209F718(Regs& regs, uint8_t* base) { regs.Apply(e8209F718(base, regs.ToArgs())); }
// sub_8209F738's original.
Result e8209F738(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F738).
inline void c8209F738(Regs& regs, uint8_t* base) { regs.Apply(e8209F738(base, regs.ToArgs())); }
// sub_8209F7A8's original.
Result e8209F7A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F7A8).
inline void c8209F7A8(Regs& regs, uint8_t* base) { regs.Apply(e8209F7A8(base, regs.ToArgs())); }
// sub_8209F8A0's original.
Result e8209F8A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F8A0).
inline void c8209F8A0(Regs& regs, uint8_t* base) { regs.Apply(e8209F8A0(base, regs.ToArgs())); }
// sub_8209F900's original.
Result e8209F900(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F900).
inline void c8209F900(Regs& regs, uint8_t* base) { regs.Apply(e8209F900(base, regs.ToArgs())); }
// sub_8209F9B8's original.
Result e8209F9B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209F9B8).
inline void c8209F9B8(Regs& regs, uint8_t* base) { regs.Apply(e8209F9B8(base, regs.ToArgs())); }
// sub_8209FA38's original.
Result e8209FA38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209FA38).
inline void c8209FA38(Regs& regs, uint8_t* base) { regs.Apply(e8209FA38(base, regs.ToArgs())); }
// sub_8209FBF0's original.
Result e8209FBF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209FBF0).
inline void c8209FBF0(Regs& regs, uint8_t* base) { regs.Apply(e8209FBF0(base, regs.ToArgs())); }
// sub_8209FFB8's original.
Result e8209FFB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8209FFB8).
inline void c8209FFB8(Regs& regs, uint8_t* base) { regs.Apply(e8209FFB8(base, regs.ToArgs())); }
// sub_820A0018's original.
Result e820A0018(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0018).
inline void c820A0018(Regs& regs, uint8_t* base) { regs.Apply(e820A0018(base, regs.ToArgs())); }
// sub_820A0208's original.
Result e820A0208(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0208).
inline void c820A0208(Regs& regs, uint8_t* base) { regs.Apply(e820A0208(base, regs.ToArgs())); }
// sub_820A02C8's original.
Result e820A02C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A02C8).
inline void c820A02C8(Regs& regs, uint8_t* base) { regs.Apply(e820A02C8(base, regs.ToArgs())); }
// sub_820A0320's original.
Result e820A0320(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0320).
inline void c820A0320(Regs& regs, uint8_t* base) { regs.Apply(e820A0320(base, regs.ToArgs())); }
// sub_820A0378's original.
Result e820A0378(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0378).
inline void c820A0378(Regs& regs, uint8_t* base) { regs.Apply(e820A0378(base, regs.ToArgs())); }
// sub_820A03D0's original.
Result e820A03D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A03D0).
inline void c820A03D0(Regs& regs, uint8_t* base) { regs.Apply(e820A03D0(base, regs.ToArgs())); }
// sub_820A0428's original.
Result e820A0428(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0428).
inline void c820A0428(Regs& regs, uint8_t* base) { regs.Apply(e820A0428(base, regs.ToArgs())); }
// sub_820A0490's original.
Result e820A0490(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0490).
inline void c820A0490(Regs& regs, uint8_t* base) { regs.Apply(e820A0490(base, regs.ToArgs())); }
// sub_820A0510's original.
Result e820A0510(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0510).
inline void c820A0510(Regs& regs, uint8_t* base) { regs.Apply(e820A0510(base, regs.ToArgs())); }
// sub_820A05D8's original.
Result e820A05D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A05D8).
inline void c820A05D8(Regs& regs, uint8_t* base) { regs.Apply(e820A05D8(base, regs.ToArgs())); }
// sub_820A0620's original.
Result e820A0620(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0620).
inline void c820A0620(Regs& regs, uint8_t* base) { regs.Apply(e820A0620(base, regs.ToArgs())); }
// sub_820A0678's original.
Result e820A0678(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0678).
inline void c820A0678(Regs& regs, uint8_t* base) { regs.Apply(e820A0678(base, regs.ToArgs())); }
// sub_820A06D0's original.
Result e820A06D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A06D0).
inline void c820A06D0(Regs& regs, uint8_t* base) { regs.Apply(e820A06D0(base, regs.ToArgs())); }
// sub_820A07F0's original.
Result e820A07F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A07F0).
inline void c820A07F0(Regs& regs, uint8_t* base) { regs.Apply(e820A07F0(base, regs.ToArgs())); }
// sub_820A08A0's original.
Result e820A08A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A08A0).
inline void c820A08A0(Regs& regs, uint8_t* base) { regs.Apply(e820A08A0(base, regs.ToArgs())); }
// sub_820A0958's original.
Result e820A0958(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0958).
inline void c820A0958(Regs& regs, uint8_t* base) { regs.Apply(e820A0958(base, regs.ToArgs())); }
// sub_820A0968's original.
Result e820A0968(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0968).
inline void c820A0968(Regs& regs, uint8_t* base) { regs.Apply(e820A0968(base, regs.ToArgs())); }
// sub_820A0978's original.
Result e820A0978(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0978).
inline void c820A0978(Regs& regs, uint8_t* base) { regs.Apply(e820A0978(base, regs.ToArgs())); }
// sub_820A0AC0's original.
Result e820A0AC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0AC0).
inline void c820A0AC0(Regs& regs, uint8_t* base) { regs.Apply(e820A0AC0(base, regs.ToArgs())); }
// sub_820A0C40's original.
Result e820A0C40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0C40).
inline void c820A0C40(Regs& regs, uint8_t* base) { regs.Apply(e820A0C40(base, regs.ToArgs())); }
// sub_820A0CC0's original.
Result e820A0CC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A0CC0).
inline void c820A0CC0(Regs& regs, uint8_t* base) { regs.Apply(e820A0CC0(base, regs.ToArgs())); }
// sub_820A1C30's original.
Result e820A1C30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A1C30).
inline void c820A1C30(Regs& regs, uint8_t* base) { regs.Apply(e820A1C30(base, regs.ToArgs())); }
// sub_820A2D30's original.
Result e820A2D30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A2D30).
inline void c820A2D30(Regs& regs, uint8_t* base) { regs.Apply(e820A2D30(base, regs.ToArgs())); }
// sub_820A2F40's original.
Result e820A2F40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A2F40).
inline void c820A2F40(Regs& regs, uint8_t* base) { regs.Apply(e820A2F40(base, regs.ToArgs())); }
// sub_820A3978's original.
Result e820A3978(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A3978).
inline void c820A3978(Regs& regs, uint8_t* base) { regs.Apply(e820A3978(base, regs.ToArgs())); }
// sub_820A3DB0's original.
Result e820A3DB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A3DB0).
inline void c820A3DB0(Regs& regs, uint8_t* base) { regs.Apply(e820A3DB0(base, regs.ToArgs())); }
// sub_820A4450's original.
Result e820A4450(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A4450).
inline void c820A4450(Regs& regs, uint8_t* base) { regs.Apply(e820A4450(base, regs.ToArgs())); }
// sub_820A4560's original.
Result e820A4560(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A4560).
inline void c820A4560(Regs& regs, uint8_t* base) { regs.Apply(e820A4560(base, regs.ToArgs())); }
// sub_820A52E0's original.
Result e820A52E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A52E0).
inline void c820A52E0(Regs& regs, uint8_t* base) { regs.Apply(e820A52E0(base, regs.ToArgs())); }
// sub_820A53E8's original.
Result e820A53E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A53E8).
inline void c820A53E8(Regs& regs, uint8_t* base) { regs.Apply(e820A53E8(base, regs.ToArgs())); }
// sub_820A55F0's original.
Result e820A55F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A55F0).
inline void c820A55F0(Regs& regs, uint8_t* base) { regs.Apply(e820A55F0(base, regs.ToArgs())); }
// sub_820A56E8's original.
Result e820A56E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A56E8).
inline void c820A56E8(Regs& regs, uint8_t* base) { regs.Apply(e820A56E8(base, regs.ToArgs())); }
// sub_820A6110's original.
Result e820A6110(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A6110).
inline void c820A6110(Regs& regs, uint8_t* base) { regs.Apply(e820A6110(base, regs.ToArgs())); }
// sub_820A6160's original.
Result e820A6160(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A6160).
inline void c820A6160(Regs& regs, uint8_t* base) { regs.Apply(e820A6160(base, regs.ToArgs())); }
// sub_820A63C8's original.
Result e820A63C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A63C8).
inline void c820A63C8(Regs& regs, uint8_t* base) { regs.Apply(e820A63C8(base, regs.ToArgs())); }
// sub_820A6AB8's original.
Result e820A6AB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A6AB8).
inline void c820A6AB8(Regs& regs, uint8_t* base) { regs.Apply(e820A6AB8(base, regs.ToArgs())); }
// sub_820A6BF0's original.
Result e820A6BF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A6BF0).
inline void c820A6BF0(Regs& regs, uint8_t* base) { regs.Apply(e820A6BF0(base, regs.ToArgs())); }
// sub_820A6F60's original.
Result e820A6F60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A6F60).
inline void c820A6F60(Regs& regs, uint8_t* base) { regs.Apply(e820A6F60(base, regs.ToArgs())); }
// sub_820A8B88's original.
Result e820A8B88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A8B88).
inline void c820A8B88(Regs& regs, uint8_t* base) { regs.Apply(e820A8B88(base, regs.ToArgs())); }
// sub_820A8F80's original.
Result e820A8F80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A8F80).
inline void c820A8F80(Regs& regs, uint8_t* base) { regs.Apply(e820A8F80(base, regs.ToArgs())); }
// sub_820A9198's original.
Result e820A9198(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A9198).
inline void c820A9198(Regs& regs, uint8_t* base) { regs.Apply(e820A9198(base, regs.ToArgs())); }
// sub_820A98C0's original.
Result e820A98C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820A98C0).
inline void c820A98C0(Regs& regs, uint8_t* base) { regs.Apply(e820A98C0(base, regs.ToArgs())); }
// sub_820AA1A8's original.
Result e820AA1A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AA1A8).
inline void c820AA1A8(Regs& regs, uint8_t* base) { regs.Apply(e820AA1A8(base, regs.ToArgs())); }
// sub_820AA2D0's original.
Result e820AA2D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AA2D0).
inline void c820AA2D0(Regs& regs, uint8_t* base) { regs.Apply(e820AA2D0(base, regs.ToArgs())); }
// sub_820AA368's original.
Result e820AA368(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AA368).
inline void c820AA368(Regs& regs, uint8_t* base) { regs.Apply(e820AA368(base, regs.ToArgs())); }
// sub_820AA430's original.
Result e820AA430(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AA430).
inline void c820AA430(Regs& regs, uint8_t* base) { regs.Apply(e820AA430(base, regs.ToArgs())); }
// sub_820AA4A8's original.
Result e820AA4A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AA4A8).
inline void c820AA4A8(Regs& regs, uint8_t* base) { regs.Apply(e820AA4A8(base, regs.ToArgs())); }
// sub_820AAE40's original.
Result e820AAE40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AAE40).
inline void c820AAE40(Regs& regs, uint8_t* base) { regs.Apply(e820AAE40(base, regs.ToArgs())); }
// sub_820AAE48's original.
Result e820AAE48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AAE48).
inline void c820AAE48(Regs& regs, uint8_t* base) { regs.Apply(e820AAE48(base, regs.ToArgs())); }
// sub_820AAE90's original.
Result e820AAE90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AAE90).
inline void c820AAE90(Regs& regs, uint8_t* base) { regs.Apply(e820AAE90(base, regs.ToArgs())); }
// sub_820AB358's original.
Result e820AB358(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AB358).
inline void c820AB358(Regs& regs, uint8_t* base) { regs.Apply(e820AB358(base, regs.ToArgs())); }
// sub_820AB560's original.
Result e820AB560(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AB560).
inline void c820AB560(Regs& regs, uint8_t* base) { regs.Apply(e820AB560(base, regs.ToArgs())); }
// sub_820AC248's original.
Result e820AC248(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AC248).
inline void c820AC248(Regs& regs, uint8_t* base) { regs.Apply(e820AC248(base, regs.ToArgs())); }
// sub_820AC7F0's original.
Result e820AC7F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AC7F0).
inline void c820AC7F0(Regs& regs, uint8_t* base) { regs.Apply(e820AC7F0(base, regs.ToArgs())); }
// sub_820AC848's original.
Result e820AC848(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AC848).
inline void c820AC848(Regs& regs, uint8_t* base) { regs.Apply(e820AC848(base, regs.ToArgs())); }
// sub_820AC8A0's original.
Result e820AC8A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AC8A0).
inline void c820AC8A0(Regs& regs, uint8_t* base) { regs.Apply(e820AC8A0(base, regs.ToArgs())); }
// sub_820AC8F8's original.
Result e820AC8F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AC8F8).
inline void c820AC8F8(Regs& regs, uint8_t* base) { regs.Apply(e820AC8F8(base, regs.ToArgs())); }
// sub_820ACA48's original.
Result e820ACA48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ACA48).
inline void c820ACA48(Regs& regs, uint8_t* base) { regs.Apply(e820ACA48(base, regs.ToArgs())); }
// sub_820ACF10's original.
Result e820ACF10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ACF10).
inline void c820ACF10(Regs& regs, uint8_t* base) { regs.Apply(e820ACF10(base, regs.ToArgs())); }
// sub_820ACF88's original.
Result e820ACF88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ACF88).
inline void c820ACF88(Regs& regs, uint8_t* base) { regs.Apply(e820ACF88(base, regs.ToArgs())); }
// sub_820ACFF8's original.
Result e820ACFF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ACFF8).
inline void c820ACFF8(Regs& regs, uint8_t* base) { regs.Apply(e820ACFF8(base, regs.ToArgs())); }
// sub_820AD058's original.
Result e820AD058(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AD058).
inline void c820AD058(Regs& regs, uint8_t* base) { regs.Apply(e820AD058(base, regs.ToArgs())); }
// sub_820AD0B8's original.
Result e820AD0B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AD0B8).
inline void c820AD0B8(Regs& regs, uint8_t* base) { regs.Apply(e820AD0B8(base, regs.ToArgs())); }
// sub_820AD118's original.
Result e820AD118(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AD118).
inline void c820AD118(Regs& regs, uint8_t* base) { regs.Apply(e820AD118(base, regs.ToArgs())); }
// sub_820AD768's original.
Result e820AD768(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AD768).
inline void c820AD768(Regs& regs, uint8_t* base) { regs.Apply(e820AD768(base, regs.ToArgs())); }
// sub_820ADA08's original.
Result e820ADA08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ADA08).
inline void c820ADA08(Regs& regs, uint8_t* base) { regs.Apply(e820ADA08(base, regs.ToArgs())); }
// sub_820ADAD0's original.
Result e820ADAD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ADAD0).
inline void c820ADAD0(Regs& regs, uint8_t* base) { regs.Apply(e820ADAD0(base, regs.ToArgs())); }
// sub_820ADBD8's original.
Result e820ADBD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ADBD8).
inline void c820ADBD8(Regs& regs, uint8_t* base) { regs.Apply(e820ADBD8(base, regs.ToArgs())); }
// sub_820ADC60's original.
Result e820ADC60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ADC60).
inline void c820ADC60(Regs& regs, uint8_t* base) { regs.Apply(e820ADC60(base, regs.ToArgs())); }
// sub_820ADE68's original.
Result e820ADE68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ADE68).
inline void c820ADE68(Regs& regs, uint8_t* base) { regs.Apply(e820ADE68(base, regs.ToArgs())); }
// sub_820AE130's original.
Result e820AE130(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AE130).
inline void c820AE130(Regs& regs, uint8_t* base) { regs.Apply(e820AE130(base, regs.ToArgs())); }
// sub_820AE1C8's original.
Result e820AE1C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AE1C8).
inline void c820AE1C8(Regs& regs, uint8_t* base) { regs.Apply(e820AE1C8(base, regs.ToArgs())); }
// sub_820AE348's original.
Result e820AE348(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AE348).
inline void c820AE348(Regs& regs, uint8_t* base) { regs.Apply(e820AE348(base, regs.ToArgs())); }
// sub_820AE498's original.
Result e820AE498(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AE498).
inline void c820AE498(Regs& regs, uint8_t* base) { regs.Apply(e820AE498(base, regs.ToArgs())); }
// sub_820AE958's original.
Result e820AE958(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AE958).
inline void c820AE958(Regs& regs, uint8_t* base) { regs.Apply(e820AE958(base, regs.ToArgs())); }
// sub_820AEA88's original.
Result e820AEA88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820AEA88).
inline void c820AEA88(Regs& regs, uint8_t* base) { regs.Apply(e820AEA88(base, regs.ToArgs())); }
// sub_820B0E70's original.
Result e820B0E70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B0E70).
inline void c820B0E70(Regs& regs, uint8_t* base) { regs.Apply(e820B0E70(base, regs.ToArgs())); }
// sub_820B18B8's original.
Result e820B18B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B18B8).
inline void c820B18B8(Regs& regs, uint8_t* base) { regs.Apply(e820B18B8(base, regs.ToArgs())); }
// sub_820B2100's original.
Result e820B2100(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B2100).
inline void c820B2100(Regs& regs, uint8_t* base) { regs.Apply(e820B2100(base, regs.ToArgs())); }
// sub_820B5670's original.
Result e820B5670(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B5670).
inline void c820B5670(Regs& regs, uint8_t* base) { regs.Apply(e820B5670(base, regs.ToArgs())); }
// sub_820B5B08's original.
Result e820B5B08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B5B08).
inline void c820B5B08(Regs& regs, uint8_t* base) { regs.Apply(e820B5B08(base, regs.ToArgs())); }
// sub_820B5FD8's original.
Result e820B5FD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B5FD8).
inline void c820B5FD8(Regs& regs, uint8_t* base) { regs.Apply(e820B5FD8(base, regs.ToArgs())); }
// sub_820B62E8's original.
Result e820B62E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B62E8).
inline void c820B62E8(Regs& regs, uint8_t* base) { regs.Apply(e820B62E8(base, regs.ToArgs())); }
// sub_820B6498's original.
Result e820B6498(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B6498).
inline void c820B6498(Regs& regs, uint8_t* base) { regs.Apply(e820B6498(base, regs.ToArgs())); }
// sub_820B72C8's original.
Result e820B72C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B72C8).
inline void c820B72C8(Regs& regs, uint8_t* base) { regs.Apply(e820B72C8(base, regs.ToArgs())); }
// sub_820B7998's original.
Result e820B7998(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B7998).
inline void c820B7998(Regs& regs, uint8_t* base) { regs.Apply(e820B7998(base, regs.ToArgs())); }
// sub_820B81C8's original.
Result e820B81C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B81C8).
inline void c820B81C8(Regs& regs, uint8_t* base) { regs.Apply(e820B81C8(base, regs.ToArgs())); }
// sub_820B8BA8's original.
Result e820B8BA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B8BA8).
inline void c820B8BA8(Regs& regs, uint8_t* base) { regs.Apply(e820B8BA8(base, regs.ToArgs())); }
// sub_820B9C40's original.
Result e820B9C40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820B9C40).
inline void c820B9C40(Regs& regs, uint8_t* base) { regs.Apply(e820B9C40(base, regs.ToArgs())); }
// sub_820BADF8's original.
Result e820BADF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BADF8).
inline void c820BADF8(Regs& regs, uint8_t* base) { regs.Apply(e820BADF8(base, regs.ToArgs())); }
// sub_820BAE38's original.
Result e820BAE38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BAE38).
inline void c820BAE38(Regs& regs, uint8_t* base) { regs.Apply(e820BAE38(base, regs.ToArgs())); }
// sub_820BB140's original.
Result e820BB140(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BB140).
inline void c820BB140(Regs& regs, uint8_t* base) { regs.Apply(e820BB140(base, regs.ToArgs())); }
// sub_820BB218's original.
Result e820BB218(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BB218).
inline void c820BB218(Regs& regs, uint8_t* base) { regs.Apply(e820BB218(base, regs.ToArgs())); }
// sub_820BB758's original.
Result e820BB758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BB758).
inline void c820BB758(Regs& regs, uint8_t* base) { regs.Apply(e820BB758(base, regs.ToArgs())); }
// sub_820BBE30's original.
Result e820BBE30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BBE30).
inline void c820BBE30(Regs& regs, uint8_t* base) { regs.Apply(e820BBE30(base, regs.ToArgs())); }
// sub_820BF038's original.
Result e820BF038(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BF038).
inline void c820BF038(Regs& regs, uint8_t* base) { regs.Apply(e820BF038(base, regs.ToArgs())); }
// sub_820BFB28's original.
Result e820BFB28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BFB28).
inline void c820BFB28(Regs& regs, uint8_t* base) { regs.Apply(e820BFB28(base, regs.ToArgs())); }
// sub_820BFD30's original.
Result e820BFD30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820BFD30).
inline void c820BFD30(Regs& regs, uint8_t* base) { regs.Apply(e820BFD30(base, regs.ToArgs())); }
// sub_820C0118's original.
Result e820C0118(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C0118).
inline void c820C0118(Regs& regs, uint8_t* base) { regs.Apply(e820C0118(base, regs.ToArgs())); }
// sub_820C1D08's original.
Result e820C1D08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C1D08).
inline void c820C1D08(Regs& regs, uint8_t* base) { regs.Apply(e820C1D08(base, regs.ToArgs())); }
// sub_820C20E0's original.
Result e820C20E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C20E0).
inline void c820C20E0(Regs& regs, uint8_t* base) { regs.Apply(e820C20E0(base, regs.ToArgs())); }
// sub_820C23E8's original.
Result e820C23E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C23E8).
inline void c820C23E8(Regs& regs, uint8_t* base) { regs.Apply(e820C23E8(base, regs.ToArgs())); }
// sub_820C27E8's original.
Result e820C27E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C27E8).
inline void c820C27E8(Regs& regs, uint8_t* base) { regs.Apply(e820C27E8(base, regs.ToArgs())); }
// sub_820C3608's original.
Result e820C3608(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3608).
inline void c820C3608(Regs& regs, uint8_t* base) { regs.Apply(e820C3608(base, regs.ToArgs())); }
// sub_820C3B78's original.
Result e820C3B78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3B78).
inline void c820C3B78(Regs& regs, uint8_t* base) { regs.Apply(e820C3B78(base, regs.ToArgs())); }
// sub_820C3BD0's original.
Result e820C3BD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3BD0).
inline void c820C3BD0(Regs& regs, uint8_t* base) { regs.Apply(e820C3BD0(base, regs.ToArgs())); }
// sub_820C3C40's original.
Result e820C3C40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3C40).
inline void c820C3C40(Regs& regs, uint8_t* base) { regs.Apply(e820C3C40(base, regs.ToArgs())); }
// sub_820C3CA0's original.
Result e820C3CA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3CA0).
inline void c820C3CA0(Regs& regs, uint8_t* base) { regs.Apply(e820C3CA0(base, regs.ToArgs())); }
// sub_820C3CF8's original.
Result e820C3CF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3CF8).
inline void c820C3CF8(Regs& regs, uint8_t* base) { regs.Apply(e820C3CF8(base, regs.ToArgs())); }
// sub_820C3DA0's original.
Result e820C3DA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3DA0).
inline void c820C3DA0(Regs& regs, uint8_t* base) { regs.Apply(e820C3DA0(base, regs.ToArgs())); }
// sub_820C3FD8's original.
Result e820C3FD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C3FD8).
inline void c820C3FD8(Regs& regs, uint8_t* base) { regs.Apply(e820C3FD8(base, regs.ToArgs())); }
// sub_820C4488's original.
Result e820C4488(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C4488).
inline void c820C4488(Regs& regs, uint8_t* base) { regs.Apply(e820C4488(base, regs.ToArgs())); }
// sub_820C4998's original.
Result e820C4998(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C4998).
inline void c820C4998(Regs& regs, uint8_t* base) { regs.Apply(e820C4998(base, regs.ToArgs())); }
// sub_820C4A70's original.
Result e820C4A70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C4A70).
inline void c820C4A70(Regs& regs, uint8_t* base) { regs.Apply(e820C4A70(base, regs.ToArgs())); }
// sub_820C4C80's original.
Result e820C4C80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C4C80).
inline void c820C4C80(Regs& regs, uint8_t* base) { regs.Apply(e820C4C80(base, regs.ToArgs())); }
// sub_820C4D88's original.
Result e820C4D88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C4D88).
inline void c820C4D88(Regs& regs, uint8_t* base) { regs.Apply(e820C4D88(base, regs.ToArgs())); }
// sub_820C4EB8's original.
Result e820C4EB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C4EB8).
inline void c820C4EB8(Regs& regs, uint8_t* base) { regs.Apply(e820C4EB8(base, regs.ToArgs())); }
// sub_820C4FB8's original.
Result e820C4FB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C4FB8).
inline void c820C4FB8(Regs& regs, uint8_t* base) { regs.Apply(e820C4FB8(base, regs.ToArgs())); }
// sub_820C5008's original.
Result e820C5008(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C5008).
inline void c820C5008(Regs& regs, uint8_t* base) { regs.Apply(e820C5008(base, regs.ToArgs())); }
// sub_820C5168's original.
Result e820C5168(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C5168).
inline void c820C5168(Regs& regs, uint8_t* base) { regs.Apply(e820C5168(base, regs.ToArgs())); }
// sub_820C52A0's original.
Result e820C52A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C52A0).
inline void c820C52A0(Regs& regs, uint8_t* base) { regs.Apply(e820C52A0(base, regs.ToArgs())); }
// sub_820C5A60's original.
Result e820C5A60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C5A60).
inline void c820C5A60(Regs& regs, uint8_t* base) { regs.Apply(e820C5A60(base, regs.ToArgs())); }
// sub_820C5C88's original.
Result e820C5C88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C5C88).
inline void c820C5C88(Regs& regs, uint8_t* base) { regs.Apply(e820C5C88(base, regs.ToArgs())); }
// sub_820C5DE8's original.
Result e820C5DE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C5DE8).
inline void c820C5DE8(Regs& regs, uint8_t* base) { regs.Apply(e820C5DE8(base, regs.ToArgs())); }
// sub_820C8480's original.
Result e820C8480(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C8480).
inline void c820C8480(Regs& regs, uint8_t* base) { regs.Apply(e820C8480(base, regs.ToArgs())); }
// sub_820C89A8's original.
Result e820C89A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C89A8).
inline void c820C89A8(Regs& regs, uint8_t* base) { regs.Apply(e820C89A8(base, regs.ToArgs())); }
// sub_820C8A50's original.
Result e820C8A50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C8A50).
inline void c820C8A50(Regs& regs, uint8_t* base) { regs.Apply(e820C8A50(base, regs.ToArgs())); }
// sub_820C8B40's original.
Result e820C8B40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C8B40).
inline void c820C8B40(Regs& regs, uint8_t* base) { regs.Apply(e820C8B40(base, regs.ToArgs())); }
// sub_820C8DE8's original.
Result e820C8DE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C8DE8).
inline void c820C8DE8(Regs& regs, uint8_t* base) { regs.Apply(e820C8DE8(base, regs.ToArgs())); }
// sub_820C9110's original.
Result e820C9110(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C9110).
inline void c820C9110(Regs& regs, uint8_t* base) { regs.Apply(e820C9110(base, regs.ToArgs())); }
// sub_820C9960's original.
Result e820C9960(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C9960).
inline void c820C9960(Regs& regs, uint8_t* base) { regs.Apply(e820C9960(base, regs.ToArgs())); }
// sub_820C9DD8's original.
Result e820C9DD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C9DD8).
inline void c820C9DD8(Regs& regs, uint8_t* base) { regs.Apply(e820C9DD8(base, regs.ToArgs())); }
// sub_820C9DF0's original.
Result e820C9DF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C9DF0).
inline void c820C9DF0(Regs& regs, uint8_t* base) { regs.Apply(e820C9DF0(base, regs.ToArgs())); }
// sub_820C9E00's original.
Result e820C9E00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C9E00).
inline void c820C9E00(Regs& regs, uint8_t* base) { regs.Apply(e820C9E00(base, regs.ToArgs())); }
// sub_820C9E98's original.
Result e820C9E98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C9E98).
inline void c820C9E98(Regs& regs, uint8_t* base) { regs.Apply(e820C9E98(base, regs.ToArgs())); }
// sub_820C9EE0's original.
Result e820C9EE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820C9EE0).
inline void c820C9EE0(Regs& regs, uint8_t* base) { regs.Apply(e820C9EE0(base, regs.ToArgs())); }
// sub_820CB9E0's original.
Result e820CB9E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CB9E0).
inline void c820CB9E0(Regs& regs, uint8_t* base) { regs.Apply(e820CB9E0(base, regs.ToArgs())); }
// sub_820CBC90's original.
Result e820CBC90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CBC90).
inline void c820CBC90(Regs& regs, uint8_t* base) { regs.Apply(e820CBC90(base, regs.ToArgs())); }
// sub_820CBD20's original.
Result e820CBD20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CBD20).
inline void c820CBD20(Regs& regs, uint8_t* base) { regs.Apply(e820CBD20(base, regs.ToArgs())); }
// sub_820CBD90's original.
Result e820CBD90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CBD90).
inline void c820CBD90(Regs& regs, uint8_t* base) { regs.Apply(e820CBD90(base, regs.ToArgs())); }
// sub_820CC0A8's original.
Result e820CC0A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CC0A8).
inline void c820CC0A8(Regs& regs, uint8_t* base) { regs.Apply(e820CC0A8(base, regs.ToArgs())); }
// sub_820CC248's original.
Result e820CC248(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CC248).
inline void c820CC248(Regs& regs, uint8_t* base) { regs.Apply(e820CC248(base, regs.ToArgs())); }
// sub_820CC2A8's original.
Result e820CC2A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CC2A8).
inline void c820CC2A8(Regs& regs, uint8_t* base) { regs.Apply(e820CC2A8(base, regs.ToArgs())); }
// sub_820CC470's original.
Result e820CC470(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CC470).
inline void c820CC470(Regs& regs, uint8_t* base) { regs.Apply(e820CC470(base, regs.ToArgs())); }
// sub_820CCDC0's original.
Result e820CCDC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CCDC0).
inline void c820CCDC0(Regs& regs, uint8_t* base) { regs.Apply(e820CCDC0(base, regs.ToArgs())); }
// sub_820CE928's original.
Result e820CE928(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CE928).
inline void c820CE928(Regs& regs, uint8_t* base) { regs.Apply(e820CE928(base, regs.ToArgs())); }
// sub_820CE990's original.
Result e820CE990(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CE990).
inline void c820CE990(Regs& regs, uint8_t* base) { regs.Apply(e820CE990(base, regs.ToArgs())); }
// sub_820CEA08's original.
Result e820CEA08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CEA08).
inline void c820CEA08(Regs& regs, uint8_t* base) { regs.Apply(e820CEA08(base, regs.ToArgs())); }
// sub_820CEB30's original.
Result e820CEB30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CEB30).
inline void c820CEB30(Regs& regs, uint8_t* base) { regs.Apply(e820CEB30(base, regs.ToArgs())); }
// sub_820CECA0's original.
Result e820CECA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820CECA0).
inline void c820CECA0(Regs& regs, uint8_t* base) { regs.Apply(e820CECA0(base, regs.ToArgs())); }
// sub_820D0270's original.
Result e820D0270(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D0270).
inline void c820D0270(Regs& regs, uint8_t* base) { regs.Apply(e820D0270(base, regs.ToArgs())); }
// sub_820D02D0's original.
Result e820D02D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D02D0).
inline void c820D02D0(Regs& regs, uint8_t* base) { regs.Apply(e820D02D0(base, regs.ToArgs())); }
// sub_820D06F0's original.
Result e820D06F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D06F0).
inline void c820D06F0(Regs& regs, uint8_t* base) { regs.Apply(e820D06F0(base, regs.ToArgs())); }
// sub_820D0758's original.
Result e820D0758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D0758).
inline void c820D0758(Regs& regs, uint8_t* base) { regs.Apply(e820D0758(base, regs.ToArgs())); }
// sub_820D07C8's original.
Result e820D07C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D07C8).
inline void c820D07C8(Regs& regs, uint8_t* base) { regs.Apply(e820D07C8(base, regs.ToArgs())); }
// sub_820D0960's original.
Result e820D0960(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D0960).
inline void c820D0960(Regs& regs, uint8_t* base) { regs.Apply(e820D0960(base, regs.ToArgs())); }
// sub_820D09A0's original.
Result e820D09A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D09A0).
inline void c820D09A0(Regs& regs, uint8_t* base) { regs.Apply(e820D09A0(base, regs.ToArgs())); }
// sub_820D1CB0's original.
Result e820D1CB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D1CB0).
inline void c820D1CB0(Regs& regs, uint8_t* base) { regs.Apply(e820D1CB0(base, regs.ToArgs())); }
// sub_820D1EB8's original.
Result e820D1EB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D1EB8).
inline void c820D1EB8(Regs& regs, uint8_t* base) { regs.Apply(e820D1EB8(base, regs.ToArgs())); }
// sub_820D3318's original.
Result e820D3318(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3318).
inline void c820D3318(Regs& regs, uint8_t* base) { regs.Apply(e820D3318(base, regs.ToArgs())); }
// sub_820D3688's original.
Result e820D3688(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3688).
inline void c820D3688(Regs& regs, uint8_t* base) { regs.Apply(e820D3688(base, regs.ToArgs())); }
// sub_820D3740's original.
Result e820D3740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3740).
inline void c820D3740(Regs& regs, uint8_t* base) { regs.Apply(e820D3740(base, regs.ToArgs())); }
// sub_820D3B58's original.
Result e820D3B58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3B58).
inline void c820D3B58(Regs& regs, uint8_t* base) { regs.Apply(e820D3B58(base, regs.ToArgs())); }
// sub_820D3C70's original.
Result e820D3C70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3C70).
inline void c820D3C70(Regs& regs, uint8_t* base) { regs.Apply(e820D3C70(base, regs.ToArgs())); }
// sub_820D3DE0's original.
Result e820D3DE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3DE0).
inline void c820D3DE0(Regs& regs, uint8_t* base) { regs.Apply(e820D3DE0(base, regs.ToArgs())); }
// sub_820D3E40's original.
Result e820D3E40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3E40).
inline void c820D3E40(Regs& regs, uint8_t* base) { regs.Apply(e820D3E40(base, regs.ToArgs())); }
// sub_820D3E98's original.
Result e820D3E98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D3E98).
inline void c820D3E98(Regs& regs, uint8_t* base) { regs.Apply(e820D3E98(base, regs.ToArgs())); }
// sub_820D4048's original.
Result e820D4048(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D4048).
inline void c820D4048(Regs& regs, uint8_t* base) { regs.Apply(e820D4048(base, regs.ToArgs())); }
// sub_820D4508's original.
Result e820D4508(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D4508).
inline void c820D4508(Regs& regs, uint8_t* base) { regs.Apply(e820D4508(base, regs.ToArgs())); }
// sub_820D4588's original.
Result e820D4588(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D4588).
inline void c820D4588(Regs& regs, uint8_t* base) { regs.Apply(e820D4588(base, regs.ToArgs())); }
// sub_820D4750's original.
Result e820D4750(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D4750).
inline void c820D4750(Regs& regs, uint8_t* base) { regs.Apply(e820D4750(base, regs.ToArgs())); }
// sub_820D47A8's original.
Result e820D47A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D47A8).
inline void c820D47A8(Regs& regs, uint8_t* base) { regs.Apply(e820D47A8(base, regs.ToArgs())); }
// sub_820D4F80's original.
Result e820D4F80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D4F80).
inline void c820D4F80(Regs& regs, uint8_t* base) { regs.Apply(e820D4F80(base, regs.ToArgs())); }
// sub_820D5750's original.
Result e820D5750(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D5750).
inline void c820D5750(Regs& regs, uint8_t* base) { regs.Apply(e820D5750(base, regs.ToArgs())); }
// sub_820D5890's original.
Result e820D5890(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D5890).
inline void c820D5890(Regs& regs, uint8_t* base) { regs.Apply(e820D5890(base, regs.ToArgs())); }
// sub_820D5988's original.
Result e820D5988(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D5988).
inline void c820D5988(Regs& regs, uint8_t* base) { regs.Apply(e820D5988(base, regs.ToArgs())); }
// sub_820D5D20's original.
Result e820D5D20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D5D20).
inline void c820D5D20(Regs& regs, uint8_t* base) { regs.Apply(e820D5D20(base, regs.ToArgs())); }
// sub_820D5EC8's original.
Result e820D5EC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D5EC8).
inline void c820D5EC8(Regs& regs, uint8_t* base) { regs.Apply(e820D5EC8(base, regs.ToArgs())); }
// sub_820D5FC8's original.
Result e820D5FC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D5FC8).
inline void c820D5FC8(Regs& regs, uint8_t* base) { regs.Apply(e820D5FC8(base, regs.ToArgs())); }
// sub_820D6010's original.
Result e820D6010(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6010).
inline void c820D6010(Regs& regs, uint8_t* base) { regs.Apply(e820D6010(base, regs.ToArgs())); }
// sub_820D6090's original.
Result e820D6090(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6090).
inline void c820D6090(Regs& regs, uint8_t* base) { regs.Apply(e820D6090(base, regs.ToArgs())); }
// sub_820D6108's original.
Result e820D6108(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6108).
inline void c820D6108(Regs& regs, uint8_t* base) { regs.Apply(e820D6108(base, regs.ToArgs())); }
// sub_820D6238's original.
Result e820D6238(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6238).
inline void c820D6238(Regs& regs, uint8_t* base) { regs.Apply(e820D6238(base, regs.ToArgs())); }
// sub_820D6328's original.
Result e820D6328(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6328).
inline void c820D6328(Regs& regs, uint8_t* base) { regs.Apply(e820D6328(base, regs.ToArgs())); }
// sub_820D63A0's original.
Result e820D63A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D63A0).
inline void c820D63A0(Regs& regs, uint8_t* base) { regs.Apply(e820D63A0(base, regs.ToArgs())); }
// sub_820D64D0's original.
Result e820D64D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D64D0).
inline void c820D64D0(Regs& regs, uint8_t* base) { regs.Apply(e820D64D0(base, regs.ToArgs())); }
// sub_820D6528's original.
Result e820D6528(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6528).
inline void c820D6528(Regs& regs, uint8_t* base) { regs.Apply(e820D6528(base, regs.ToArgs())); }
// sub_820D6798's original.
Result e820D6798(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6798).
inline void c820D6798(Regs& regs, uint8_t* base) { regs.Apply(e820D6798(base, regs.ToArgs())); }
// sub_820D67E8's original.
Result e820D67E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D67E8).
inline void c820D67E8(Regs& regs, uint8_t* base) { regs.Apply(e820D67E8(base, regs.ToArgs())); }
// sub_820D6948's original.
Result e820D6948(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6948).
inline void c820D6948(Regs& regs, uint8_t* base) { regs.Apply(e820D6948(base, regs.ToArgs())); }
// sub_820D6A20's original.
Result e820D6A20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6A20).
inline void c820D6A20(Regs& regs, uint8_t* base) { regs.Apply(e820D6A20(base, regs.ToArgs())); }
// sub_820D6A60's original.
Result e820D6A60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6A60).
inline void c820D6A60(Regs& regs, uint8_t* base) { regs.Apply(e820D6A60(base, regs.ToArgs())); }
// sub_820D6A70's original.
Result e820D6A70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6A70).
inline void c820D6A70(Regs& regs, uint8_t* base) { regs.Apply(e820D6A70(base, regs.ToArgs())); }
// sub_820D6AD0's original.
Result e820D6AD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6AD0).
inline void c820D6AD0(Regs& regs, uint8_t* base) { regs.Apply(e820D6AD0(base, regs.ToArgs())); }
// sub_820D6BC0's original.
Result e820D6BC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6BC0).
inline void c820D6BC0(Regs& regs, uint8_t* base) { regs.Apply(e820D6BC0(base, regs.ToArgs())); }
// sub_820D6D00's original.
Result e820D6D00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6D00).
inline void c820D6D00(Regs& regs, uint8_t* base) { regs.Apply(e820D6D00(base, regs.ToArgs())); }
// sub_820D6F20's original.
Result e820D6F20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6F20).
inline void c820D6F20(Regs& regs, uint8_t* base) { regs.Apply(e820D6F20(base, regs.ToArgs())); }
// sub_820D6F88's original.
Result e820D6F88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6F88).
inline void c820D6F88(Regs& regs, uint8_t* base) { regs.Apply(e820D6F88(base, regs.ToArgs())); }
// sub_820D6FE8's original.
Result e820D6FE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D6FE8).
inline void c820D6FE8(Regs& regs, uint8_t* base) { regs.Apply(e820D6FE8(base, regs.ToArgs())); }
// sub_820D7038's original.
Result e820D7038(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7038).
inline void c820D7038(Regs& regs, uint8_t* base) { regs.Apply(e820D7038(base, regs.ToArgs())); }
// sub_820D72D8's original.
Result e820D72D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D72D8).
inline void c820D72D8(Regs& regs, uint8_t* base) { regs.Apply(e820D72D8(base, regs.ToArgs())); }
// sub_820D7388's original.
Result e820D7388(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7388).
inline void c820D7388(Regs& regs, uint8_t* base) { regs.Apply(e820D7388(base, regs.ToArgs())); }
// sub_820D7428's original.
Result e820D7428(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7428).
inline void c820D7428(Regs& regs, uint8_t* base) { regs.Apply(e820D7428(base, regs.ToArgs())); }
// sub_820D74C0's original.
Result e820D74C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D74C0).
inline void c820D74C0(Regs& regs, uint8_t* base) { regs.Apply(e820D74C0(base, regs.ToArgs())); }
// sub_820D7578's original.
Result e820D7578(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7578).
inline void c820D7578(Regs& regs, uint8_t* base) { regs.Apply(e820D7578(base, regs.ToArgs())); }
// sub_820D75F8's original.
Result e820D75F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D75F8).
inline void c820D75F8(Regs& regs, uint8_t* base) { regs.Apply(e820D75F8(base, regs.ToArgs())); }
// sub_820D7690's original.
Result e820D7690(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7690).
inline void c820D7690(Regs& regs, uint8_t* base) { regs.Apply(e820D7690(base, regs.ToArgs())); }
// sub_820D76D8's original.
Result e820D76D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D76D8).
inline void c820D76D8(Regs& regs, uint8_t* base) { regs.Apply(e820D76D8(base, regs.ToArgs())); }
// sub_820D7840's original.
Result e820D7840(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7840).
inline void c820D7840(Regs& regs, uint8_t* base) { regs.Apply(e820D7840(base, regs.ToArgs())); }
// sub_820D78B8's original.
Result e820D78B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D78B8).
inline void c820D78B8(Regs& regs, uint8_t* base) { regs.Apply(e820D78B8(base, regs.ToArgs())); }
// sub_820D7960's original.
Result e820D7960(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7960).
inline void c820D7960(Regs& regs, uint8_t* base) { regs.Apply(e820D7960(base, regs.ToArgs())); }
// sub_820D79B0's original.
Result e820D79B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D79B0).
inline void c820D79B0(Regs& regs, uint8_t* base) { regs.Apply(e820D79B0(base, regs.ToArgs())); }
// sub_820D7AB0's original.
Result e820D7AB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7AB0).
inline void c820D7AB0(Regs& regs, uint8_t* base) { regs.Apply(e820D7AB0(base, regs.ToArgs())); }
// sub_820D7B90's original.
Result e820D7B90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7B90).
inline void c820D7B90(Regs& regs, uint8_t* base) { regs.Apply(e820D7B90(base, regs.ToArgs())); }
// sub_820D7BF8's original.
Result e820D7BF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7BF8).
inline void c820D7BF8(Regs& regs, uint8_t* base) { regs.Apply(e820D7BF8(base, regs.ToArgs())); }
// sub_820D7C40's original.
Result e820D7C40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7C40).
inline void c820D7C40(Regs& regs, uint8_t* base) { regs.Apply(e820D7C40(base, regs.ToArgs())); }
// sub_820D7CC0's original.
Result e820D7CC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7CC0).
inline void c820D7CC0(Regs& regs, uint8_t* base) { regs.Apply(e820D7CC0(base, regs.ToArgs())); }
// sub_820D7D38's original.
Result e820D7D38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7D38).
inline void c820D7D38(Regs& regs, uint8_t* base) { regs.Apply(e820D7D38(base, regs.ToArgs())); }
// sub_820D7E18's original.
Result e820D7E18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7E18).
inline void c820D7E18(Regs& regs, uint8_t* base) { regs.Apply(e820D7E18(base, regs.ToArgs())); }
// sub_820D7E90's original.
Result e820D7E90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7E90).
inline void c820D7E90(Regs& regs, uint8_t* base) { regs.Apply(e820D7E90(base, regs.ToArgs())); }
// sub_820D7EE8's original.
Result e820D7EE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D7EE8).
inline void c820D7EE8(Regs& regs, uint8_t* base) { regs.Apply(e820D7EE8(base, regs.ToArgs())); }
// sub_820D8430's original.
Result e820D8430(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8430).
inline void c820D8430(Regs& regs, uint8_t* base) { regs.Apply(e820D8430(base, regs.ToArgs())); }
// sub_820D8908's original.
Result e820D8908(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8908).
inline void c820D8908(Regs& regs, uint8_t* base) { regs.Apply(e820D8908(base, regs.ToArgs())); }
// sub_820D89B8's original.
Result e820D89B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D89B8).
inline void c820D89B8(Regs& regs, uint8_t* base) { regs.Apply(e820D89B8(base, regs.ToArgs())); }
// sub_820D8AA8's original.
Result e820D8AA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8AA8).
inline void c820D8AA8(Regs& regs, uint8_t* base) { regs.Apply(e820D8AA8(base, regs.ToArgs())); }
// sub_820D8B58's original.
Result e820D8B58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8B58).
inline void c820D8B58(Regs& regs, uint8_t* base) { regs.Apply(e820D8B58(base, regs.ToArgs())); }
// sub_820D8E68's original.
Result e820D8E68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8E68).
inline void c820D8E68(Regs& regs, uint8_t* base) { regs.Apply(e820D8E68(base, regs.ToArgs())); }
// sub_820D8EC0's original.
Result e820D8EC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8EC0).
inline void c820D8EC0(Regs& regs, uint8_t* base) { regs.Apply(e820D8EC0(base, regs.ToArgs())); }
// sub_820D8ED0's original.
Result e820D8ED0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8ED0).
inline void c820D8ED0(Regs& regs, uint8_t* base) { regs.Apply(e820D8ED0(base, regs.ToArgs())); }
// sub_820D8F60's original.
Result e820D8F60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D8F60).
inline void c820D8F60(Regs& regs, uint8_t* base) { regs.Apply(e820D8F60(base, regs.ToArgs())); }
// sub_820D9010's original.
Result e820D9010(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9010).
inline void c820D9010(Regs& regs, uint8_t* base) { regs.Apply(e820D9010(base, regs.ToArgs())); }
// sub_820D90B8's original.
Result e820D90B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D90B8).
inline void c820D90B8(Regs& regs, uint8_t* base) { regs.Apply(e820D90B8(base, regs.ToArgs())); }
// sub_820D9110's original.
Result e820D9110(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9110).
inline void c820D9110(Regs& regs, uint8_t* base) { regs.Apply(e820D9110(base, regs.ToArgs())); }
// sub_820D91E0's original.
Result e820D91E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D91E0).
inline void c820D91E0(Regs& regs, uint8_t* base) { regs.Apply(e820D91E0(base, regs.ToArgs())); }
// sub_820D92A0's original.
Result e820D92A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D92A0).
inline void c820D92A0(Regs& regs, uint8_t* base) { regs.Apply(e820D92A0(base, regs.ToArgs())); }
// sub_820D9570's original.
Result e820D9570(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9570).
inline void c820D9570(Regs& regs, uint8_t* base) { regs.Apply(e820D9570(base, regs.ToArgs())); }
// sub_820D95E8's original.
Result e820D95E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D95E8).
inline void c820D95E8(Regs& regs, uint8_t* base) { regs.Apply(e820D95E8(base, regs.ToArgs())); }
// sub_820D9638's original.
Result e820D9638(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9638).
inline void c820D9638(Regs& regs, uint8_t* base) { regs.Apply(e820D9638(base, regs.ToArgs())); }
// sub_820D9870's original.
Result e820D9870(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9870).
inline void c820D9870(Regs& regs, uint8_t* base) { regs.Apply(e820D9870(base, regs.ToArgs())); }
// sub_820D9B68's original.
Result e820D9B68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9B68).
inline void c820D9B68(Regs& regs, uint8_t* base) { regs.Apply(e820D9B68(base, regs.ToArgs())); }
// sub_820D9C50's original.
Result e820D9C50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9C50).
inline void c820D9C50(Regs& regs, uint8_t* base) { regs.Apply(e820D9C50(base, regs.ToArgs())); }
// sub_820D9D10's original.
Result e820D9D10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9D10).
inline void c820D9D10(Regs& regs, uint8_t* base) { regs.Apply(e820D9D10(base, regs.ToArgs())); }
// sub_820D9EA0's original.
Result e820D9EA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9EA0).
inline void c820D9EA0(Regs& regs, uint8_t* base) { regs.Apply(e820D9EA0(base, regs.ToArgs())); }
// sub_820D9F50's original.
Result e820D9F50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820D9F50).
inline void c820D9F50(Regs& regs, uint8_t* base) { regs.Apply(e820D9F50(base, regs.ToArgs())); }
// sub_820DA180's original.
Result e820DA180(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DA180).
inline void c820DA180(Regs& regs, uint8_t* base) { regs.Apply(e820DA180(base, regs.ToArgs())); }
// sub_820DA318's original.
Result e820DA318(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DA318).
inline void c820DA318(Regs& regs, uint8_t* base) { regs.Apply(e820DA318(base, regs.ToArgs())); }
// sub_820DA5D0's original.
Result e820DA5D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DA5D0).
inline void c820DA5D0(Regs& regs, uint8_t* base) { regs.Apply(e820DA5D0(base, regs.ToArgs())); }
// sub_820DA758's original.
Result e820DA758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DA758).
inline void c820DA758(Regs& regs, uint8_t* base) { regs.Apply(e820DA758(base, regs.ToArgs())); }
// sub_820DAA38's original.
Result e820DAA38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DAA38).
inline void c820DAA38(Regs& regs, uint8_t* base) { regs.Apply(e820DAA38(base, regs.ToArgs())); }
// sub_820DAAC8's original.
Result e820DAAC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DAAC8).
inline void c820DAAC8(Regs& regs, uint8_t* base) { regs.Apply(e820DAAC8(base, regs.ToArgs())); }
// sub_820DAB10's original.
Result e820DAB10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DAB10).
inline void c820DAB10(Regs& regs, uint8_t* base) { regs.Apply(e820DAB10(base, regs.ToArgs())); }
// sub_820DAB58's original.
Result e820DAB58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DAB58).
inline void c820DAB58(Regs& regs, uint8_t* base) { regs.Apply(e820DAB58(base, regs.ToArgs())); }
// sub_820DAC68's original.
Result e820DAC68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DAC68).
inline void c820DAC68(Regs& regs, uint8_t* base) { regs.Apply(e820DAC68(base, regs.ToArgs())); }
// sub_820DB290's original.
Result e820DB290(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB290).
inline void c820DB290(Regs& regs, uint8_t* base) { regs.Apply(e820DB290(base, regs.ToArgs())); }
// sub_820DB330's original.
Result e820DB330(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB330).
inline void c820DB330(Regs& regs, uint8_t* base) { regs.Apply(e820DB330(base, regs.ToArgs())); }
// sub_820DB400's original.
Result e820DB400(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB400).
inline void c820DB400(Regs& regs, uint8_t* base) { regs.Apply(e820DB400(base, regs.ToArgs())); }
// sub_820DB6C0's original.
Result e820DB6C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB6C0).
inline void c820DB6C0(Regs& regs, uint8_t* base) { regs.Apply(e820DB6C0(base, regs.ToArgs())); }
// sub_820DB720's original.
Result e820DB720(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB720).
inline void c820DB720(Regs& regs, uint8_t* base) { regs.Apply(e820DB720(base, regs.ToArgs())); }
// sub_820DB770's original.
Result e820DB770(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB770).
inline void c820DB770(Regs& regs, uint8_t* base) { regs.Apply(e820DB770(base, regs.ToArgs())); }
// sub_820DB7C8's original.
Result e820DB7C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB7C8).
inline void c820DB7C8(Regs& regs, uint8_t* base) { regs.Apply(e820DB7C8(base, regs.ToArgs())); }
// sub_820DB878's original.
Result e820DB878(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB878).
inline void c820DB878(Regs& regs, uint8_t* base) { regs.Apply(e820DB878(base, regs.ToArgs())); }
// sub_820DB930's original.
Result e820DB930(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB930).
inline void c820DB930(Regs& regs, uint8_t* base) { regs.Apply(e820DB930(base, regs.ToArgs())); }
// sub_820DB9E8's original.
Result e820DB9E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DB9E8).
inline void c820DB9E8(Regs& regs, uint8_t* base) { regs.Apply(e820DB9E8(base, regs.ToArgs())); }
// sub_820DBAA0's original.
Result e820DBAA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DBAA0).
inline void c820DBAA0(Regs& regs, uint8_t* base) { regs.Apply(e820DBAA0(base, regs.ToArgs())); }
// sub_820DBC10's original.
Result e820DBC10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DBC10).
inline void c820DBC10(Regs& regs, uint8_t* base) { regs.Apply(e820DBC10(base, regs.ToArgs())); }
// sub_820DBC88's original.
Result e820DBC88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DBC88).
inline void c820DBC88(Regs& regs, uint8_t* base) { regs.Apply(e820DBC88(base, regs.ToArgs())); }
// sub_820DBE20's original.
Result e820DBE20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DBE20).
inline void c820DBE20(Regs& regs, uint8_t* base) { regs.Apply(e820DBE20(base, regs.ToArgs())); }
// sub_820DBF88's original.
Result e820DBF88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DBF88).
inline void c820DBF88(Regs& regs, uint8_t* base) { regs.Apply(e820DBF88(base, regs.ToArgs())); }
// sub_820DBFF8's original.
Result e820DBFF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DBFF8).
inline void c820DBFF8(Regs& regs, uint8_t* base) { regs.Apply(e820DBFF8(base, regs.ToArgs())); }
// sub_820DC0A8's original.
Result e820DC0A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC0A8).
inline void c820DC0A8(Regs& regs, uint8_t* base) { regs.Apply(e820DC0A8(base, regs.ToArgs())); }
// sub_820DC0F0's original.
Result e820DC0F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC0F0).
inline void c820DC0F0(Regs& regs, uint8_t* base) { regs.Apply(e820DC0F0(base, regs.ToArgs())); }
// sub_820DC138's original.
Result e820DC138(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC138).
inline void c820DC138(Regs& regs, uint8_t* base) { regs.Apply(e820DC138(base, regs.ToArgs())); }
// sub_820DC190's original.
Result e820DC190(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC190).
inline void c820DC190(Regs& regs, uint8_t* base) { regs.Apply(e820DC190(base, regs.ToArgs())); }
// sub_820DC2F8's original.
Result e820DC2F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC2F8).
inline void c820DC2F8(Regs& regs, uint8_t* base) { regs.Apply(e820DC2F8(base, regs.ToArgs())); }
// sub_820DC3E8's original.
Result e820DC3E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC3E8).
inline void c820DC3E8(Regs& regs, uint8_t* base) { regs.Apply(e820DC3E8(base, regs.ToArgs())); }
// sub_820DC448's original.
Result e820DC448(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC448).
inline void c820DC448(Regs& regs, uint8_t* base) { regs.Apply(e820DC448(base, regs.ToArgs())); }
// sub_820DC498's original.
Result e820DC498(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC498).
inline void c820DC498(Regs& regs, uint8_t* base) { regs.Apply(e820DC498(base, regs.ToArgs())); }
// sub_820DC748's original.
Result e820DC748(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC748).
inline void c820DC748(Regs& regs, uint8_t* base) { regs.Apply(e820DC748(base, regs.ToArgs())); }
// sub_820DC7A0's original.
Result e820DC7A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC7A0).
inline void c820DC7A0(Regs& regs, uint8_t* base) { regs.Apply(e820DC7A0(base, regs.ToArgs())); }
// sub_820DC7F8's original.
Result e820DC7F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC7F8).
inline void c820DC7F8(Regs& regs, uint8_t* base) { regs.Apply(e820DC7F8(base, regs.ToArgs())); }
// sub_820DC8A8's original.
Result e820DC8A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC8A8).
inline void c820DC8A8(Regs& regs, uint8_t* base) { regs.Apply(e820DC8A8(base, regs.ToArgs())); }
// sub_820DC900's original.
Result e820DC900(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC900).
inline void c820DC900(Regs& regs, uint8_t* base) { regs.Apply(e820DC900(base, regs.ToArgs())); }
// sub_820DC958's original.
Result e820DC958(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DC958).
inline void c820DC958(Regs& regs, uint8_t* base) { regs.Apply(e820DC958(base, regs.ToArgs())); }
// sub_820DCA40's original.
Result e820DCA40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DCA40).
inline void c820DCA40(Regs& regs, uint8_t* base) { regs.Apply(e820DCA40(base, regs.ToArgs())); }
// sub_820DCB20's original.
Result e820DCB20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DCB20).
inline void c820DCB20(Regs& regs, uint8_t* base) { regs.Apply(e820DCB20(base, regs.ToArgs())); }
// sub_820DCB70's original.
Result e820DCB70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DCB70).
inline void c820DCB70(Regs& regs, uint8_t* base) { regs.Apply(e820DCB70(base, regs.ToArgs())); }
// sub_820DCBC0's original.
Result e820DCBC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DCBC0).
inline void c820DCBC0(Regs& regs, uint8_t* base) { regs.Apply(e820DCBC0(base, regs.ToArgs())); }
// sub_820DDA60's original.
Result e820DDA60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DDA60).
inline void c820DDA60(Regs& regs, uint8_t* base) { regs.Apply(e820DDA60(base, regs.ToArgs())); }
// sub_820DDCF8's original.
Result e820DDCF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DDCF8).
inline void c820DDCF8(Regs& regs, uint8_t* base) { regs.Apply(e820DDCF8(base, regs.ToArgs())); }
// sub_820DDE00's original.
Result e820DDE00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DDE00).
inline void c820DDE00(Regs& regs, uint8_t* base) { regs.Apply(e820DDE00(base, regs.ToArgs())); }
// sub_820DDE58's original.
Result e820DDE58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DDE58).
inline void c820DDE58(Regs& regs, uint8_t* base) { regs.Apply(e820DDE58(base, regs.ToArgs())); }
// sub_820DE030's original.
Result e820DE030(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE030).
inline void c820DE030(Regs& regs, uint8_t* base) { regs.Apply(e820DE030(base, regs.ToArgs())); }
// sub_820DE090's original.
Result e820DE090(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE090).
inline void c820DE090(Regs& regs, uint8_t* base) { regs.Apply(e820DE090(base, regs.ToArgs())); }
// sub_820DE0E0's original.
Result e820DE0E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE0E0).
inline void c820DE0E0(Regs& regs, uint8_t* base) { regs.Apply(e820DE0E0(base, regs.ToArgs())); }
// sub_820DE2C8's original.
Result e820DE2C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE2C8).
inline void c820DE2C8(Regs& regs, uint8_t* base) { regs.Apply(e820DE2C8(base, regs.ToArgs())); }
// sub_820DE498's original.
Result e820DE498(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE498).
inline void c820DE498(Regs& regs, uint8_t* base) { regs.Apply(e820DE498(base, regs.ToArgs())); }
// sub_820DE548's original.
Result e820DE548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE548).
inline void c820DE548(Regs& regs, uint8_t* base) { regs.Apply(e820DE548(base, regs.ToArgs())); }
// sub_820DE600's original.
Result e820DE600(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE600).
inline void c820DE600(Regs& regs, uint8_t* base) { regs.Apply(e820DE600(base, regs.ToArgs())); }
// sub_820DE7E8's original.
Result e820DE7E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE7E8).
inline void c820DE7E8(Regs& regs, uint8_t* base) { regs.Apply(e820DE7E8(base, regs.ToArgs())); }
// sub_820DE9D0's original.
Result e820DE9D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DE9D0).
inline void c820DE9D0(Regs& regs, uint8_t* base) { regs.Apply(e820DE9D0(base, regs.ToArgs())); }
// sub_820DEBF0's original.
Result e820DEBF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DEBF0).
inline void c820DEBF0(Regs& regs, uint8_t* base) { regs.Apply(e820DEBF0(base, regs.ToArgs())); }
// sub_820DECD0's original.
Result e820DECD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DECD0).
inline void c820DECD0(Regs& regs, uint8_t* base) { regs.Apply(e820DECD0(base, regs.ToArgs())); }
// sub_820DEE88's original.
Result e820DEE88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DEE88).
inline void c820DEE88(Regs& regs, uint8_t* base) { regs.Apply(e820DEE88(base, regs.ToArgs())); }
// sub_820DF020's original.
Result e820DF020(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF020).
inline void c820DF020(Regs& regs, uint8_t* base) { regs.Apply(e820DF020(base, regs.ToArgs())); }
// sub_820DF0C0's original.
Result e820DF0C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF0C0).
inline void c820DF0C0(Regs& regs, uint8_t* base) { regs.Apply(e820DF0C0(base, regs.ToArgs())); }
// sub_820DF128's original.
Result e820DF128(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF128).
inline void c820DF128(Regs& regs, uint8_t* base) { regs.Apply(e820DF128(base, regs.ToArgs())); }
// sub_820DF250's original.
Result e820DF250(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF250).
inline void c820DF250(Regs& regs, uint8_t* base) { regs.Apply(e820DF250(base, regs.ToArgs())); }
// sub_820DF410's original.
Result e820DF410(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF410).
inline void c820DF410(Regs& regs, uint8_t* base) { regs.Apply(e820DF410(base, regs.ToArgs())); }
// sub_820DF570's original.
Result e820DF570(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF570).
inline void c820DF570(Regs& regs, uint8_t* base) { regs.Apply(e820DF570(base, regs.ToArgs())); }
// sub_820DF668's original.
Result e820DF668(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF668).
inline void c820DF668(Regs& regs, uint8_t* base) { regs.Apply(e820DF668(base, regs.ToArgs())); }
// sub_820DF6B0's original.
Result e820DF6B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF6B0).
inline void c820DF6B0(Regs& regs, uint8_t* base) { regs.Apply(e820DF6B0(base, regs.ToArgs())); }
// sub_820DF6F8's original.
Result e820DF6F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF6F8).
inline void c820DF6F8(Regs& regs, uint8_t* base) { regs.Apply(e820DF6F8(base, regs.ToArgs())); }
// sub_820DF740's original.
Result e820DF740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820DF740).
inline void c820DF740(Regs& regs, uint8_t* base) { regs.Apply(e820DF740(base, regs.ToArgs())); }
// sub_820E00F0's original.
Result e820E00F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E00F0).
inline void c820E00F0(Regs& regs, uint8_t* base) { regs.Apply(e820E00F0(base, regs.ToArgs())); }
// sub_820E0258's original.
Result e820E0258(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E0258).
inline void c820E0258(Regs& regs, uint8_t* base) { regs.Apply(e820E0258(base, regs.ToArgs())); }
// sub_820E0510's original.
Result e820E0510(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E0510).
inline void c820E0510(Regs& regs, uint8_t* base) { regs.Apply(e820E0510(base, regs.ToArgs())); }
// sub_820E0590's original.
Result e820E0590(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E0590).
inline void c820E0590(Regs& regs, uint8_t* base) { regs.Apply(e820E0590(base, regs.ToArgs())); }
// sub_820E05E0's original.
Result e820E05E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E05E0).
inline void c820E05E0(Regs& regs, uint8_t* base) { regs.Apply(e820E05E0(base, regs.ToArgs())); }
// sub_820E0908's original.
Result e820E0908(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E0908).
inline void c820E0908(Regs& regs, uint8_t* base) { regs.Apply(e820E0908(base, regs.ToArgs())); }
// sub_820E2A10's original.
Result e820E2A10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E2A10).
inline void c820E2A10(Regs& regs, uint8_t* base) { regs.Apply(e820E2A10(base, regs.ToArgs())); }
// sub_820E2E10's original.
Result e820E2E10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E2E10).
inline void c820E2E10(Regs& regs, uint8_t* base) { regs.Apply(e820E2E10(base, regs.ToArgs())); }
// sub_820E2E68's original.
Result e820E2E68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E2E68).
inline void c820E2E68(Regs& regs, uint8_t* base) { regs.Apply(e820E2E68(base, regs.ToArgs())); }
// sub_820E2EE8's original.
Result e820E2EE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E2EE8).
inline void c820E2EE8(Regs& regs, uint8_t* base) { regs.Apply(e820E2EE8(base, regs.ToArgs())); }
// sub_820E2F18's original.
Result e820E2F18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E2F18).
inline void c820E2F18(Regs& regs, uint8_t* base) { regs.Apply(e820E2F18(base, regs.ToArgs())); }
// sub_820E2F78's original.
Result e820E2F78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E2F78).
inline void c820E2F78(Regs& regs, uint8_t* base) { regs.Apply(e820E2F78(base, regs.ToArgs())); }
// sub_820E2FC8's original.
Result e820E2FC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E2FC8).
inline void c820E2FC8(Regs& regs, uint8_t* base) { regs.Apply(e820E2FC8(base, regs.ToArgs())); }
// sub_820E3040's original.
Result e820E3040(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E3040).
inline void c820E3040(Regs& regs, uint8_t* base) { regs.Apply(e820E3040(base, regs.ToArgs())); }
// sub_820E3200's original.
Result e820E3200(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E3200).
inline void c820E3200(Regs& regs, uint8_t* base) { regs.Apply(e820E3200(base, regs.ToArgs())); }
// sub_820E32E8's original.
Result e820E32E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E32E8).
inline void c820E32E8(Regs& regs, uint8_t* base) { regs.Apply(e820E32E8(base, regs.ToArgs())); }
// sub_820E33F0's original.
Result e820E33F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E33F0).
inline void c820E33F0(Regs& regs, uint8_t* base) { regs.Apply(e820E33F0(base, regs.ToArgs())); }
// sub_820E3440's original.
Result e820E3440(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E3440).
inline void c820E3440(Regs& regs, uint8_t* base) { regs.Apply(e820E3440(base, regs.ToArgs())); }
// sub_820E34F8's original.
Result e820E34F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E34F8).
inline void c820E34F8(Regs& regs, uint8_t* base) { regs.Apply(e820E34F8(base, regs.ToArgs())); }
// sub_820E35D8's original.
Result e820E35D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E35D8).
inline void c820E35D8(Regs& regs, uint8_t* base) { regs.Apply(e820E35D8(base, regs.ToArgs())); }
// sub_820E3660's original.
Result e820E3660(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E3660).
inline void c820E3660(Regs& regs, uint8_t* base) { regs.Apply(e820E3660(base, regs.ToArgs())); }
// sub_820E3748's original.
Result e820E3748(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E3748).
inline void c820E3748(Regs& regs, uint8_t* base) { regs.Apply(e820E3748(base, regs.ToArgs())); }
// sub_820E3890's original.
Result e820E3890(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E3890).
inline void c820E3890(Regs& regs, uint8_t* base) { regs.Apply(e820E3890(base, regs.ToArgs())); }
// sub_820E3938's original.
Result e820E3938(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E3938).
inline void c820E3938(Regs& regs, uint8_t* base) { regs.Apply(e820E3938(base, regs.ToArgs())); }
// sub_820E39B0's original.
Result e820E39B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E39B0).
inline void c820E39B0(Regs& regs, uint8_t* base) { regs.Apply(e820E39B0(base, regs.ToArgs())); }
// sub_820E4140's original.
Result e820E4140(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E4140).
inline void c820E4140(Regs& regs, uint8_t* base) { regs.Apply(e820E4140(base, regs.ToArgs())); }
// sub_820E4250's original.
Result e820E4250(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E4250).
inline void c820E4250(Regs& regs, uint8_t* base) { regs.Apply(e820E4250(base, regs.ToArgs())); }
// sub_820E4FD8's original.
Result e820E4FD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E4FD8).
inline void c820E4FD8(Regs& regs, uint8_t* base) { regs.Apply(e820E4FD8(base, regs.ToArgs())); }
// sub_820E5178's original.
Result e820E5178(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E5178).
inline void c820E5178(Regs& regs, uint8_t* base) { regs.Apply(e820E5178(base, regs.ToArgs())); }
// sub_820E5340's original.
Result e820E5340(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E5340).
inline void c820E5340(Regs& regs, uint8_t* base) { regs.Apply(e820E5340(base, regs.ToArgs())); }
// sub_820E53A0's original.
Result e820E53A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E53A0).
inline void c820E53A0(Regs& regs, uint8_t* base) { regs.Apply(e820E53A0(base, regs.ToArgs())); }
// sub_820E53F0's original.
Result e820E53F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E53F0).
inline void c820E53F0(Regs& regs, uint8_t* base) { regs.Apply(e820E53F0(base, regs.ToArgs())); }
// sub_820E54E8's original.
Result e820E54E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E54E8).
inline void c820E54E8(Regs& regs, uint8_t* base) { regs.Apply(e820E54E8(base, regs.ToArgs())); }
// sub_820E56D0's original.
Result e820E56D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E56D0).
inline void c820E56D0(Regs& regs, uint8_t* base) { regs.Apply(e820E56D0(base, regs.ToArgs())); }
// sub_820E57A0's original.
Result e820E57A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E57A0).
inline void c820E57A0(Regs& regs, uint8_t* base) { regs.Apply(e820E57A0(base, regs.ToArgs())); }
// sub_820E5880's original.
Result e820E5880(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E5880).
inline void c820E5880(Regs& regs, uint8_t* base) { regs.Apply(e820E5880(base, regs.ToArgs())); }
// sub_820E58C8's original.
Result e820E58C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E58C8).
inline void c820E58C8(Regs& regs, uint8_t* base) { regs.Apply(e820E58C8(base, regs.ToArgs())); }
// sub_820E5930's original.
Result e820E5930(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E5930).
inline void c820E5930(Regs& regs, uint8_t* base) { regs.Apply(e820E5930(base, regs.ToArgs())); }
// sub_820E5A18's original.
Result e820E5A18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E5A18).
inline void c820E5A18(Regs& regs, uint8_t* base) { regs.Apply(e820E5A18(base, regs.ToArgs())); }
// sub_820E5A68's original.
Result e820E5A68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E5A68).
inline void c820E5A68(Regs& regs, uint8_t* base) { regs.Apply(e820E5A68(base, regs.ToArgs())); }
// sub_820E61D0's original.
Result e820E61D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E61D0).
inline void c820E61D0(Regs& regs, uint8_t* base) { regs.Apply(e820E61D0(base, regs.ToArgs())); }
// sub_820E68E8's original.
Result e820E68E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E68E8).
inline void c820E68E8(Regs& regs, uint8_t* base) { regs.Apply(e820E68E8(base, regs.ToArgs())); }
// sub_820E6A40's original.
Result e820E6A40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E6A40).
inline void c820E6A40(Regs& regs, uint8_t* base) { regs.Apply(e820E6A40(base, regs.ToArgs())); }
// sub_820E6E70's original.
Result e820E6E70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E6E70).
inline void c820E6E70(Regs& regs, uint8_t* base) { regs.Apply(e820E6E70(base, regs.ToArgs())); }
// sub_820E6ED8's original.
Result e820E6ED8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E6ED8).
inline void c820E6ED8(Regs& regs, uint8_t* base) { regs.Apply(e820E6ED8(base, regs.ToArgs())); }
// sub_820E7328's original.
Result e820E7328(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E7328).
inline void c820E7328(Regs& regs, uint8_t* base) { regs.Apply(e820E7328(base, regs.ToArgs())); }
// sub_820E74B8's original.
Result e820E74B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E74B8).
inline void c820E74B8(Regs& regs, uint8_t* base) { regs.Apply(e820E74B8(base, regs.ToArgs())); }
// sub_820E7918's original.
Result e820E7918(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E7918).
inline void c820E7918(Regs& regs, uint8_t* base) { regs.Apply(e820E7918(base, regs.ToArgs())); }
// sub_820E7C08's original.
Result e820E7C08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E7C08).
inline void c820E7C08(Regs& regs, uint8_t* base) { regs.Apply(e820E7C08(base, regs.ToArgs())); }
// sub_820E7F90's original.
Result e820E7F90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E7F90).
inline void c820E7F90(Regs& regs, uint8_t* base) { regs.Apply(e820E7F90(base, regs.ToArgs())); }
// sub_820E85C8's original.
Result e820E85C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E85C8).
inline void c820E85C8(Regs& regs, uint8_t* base) { regs.Apply(e820E85C8(base, regs.ToArgs())); }
// sub_820E8668's original.
Result e820E8668(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E8668).
inline void c820E8668(Regs& regs, uint8_t* base) { regs.Apply(e820E8668(base, regs.ToArgs())); }
// sub_820E88A8's original.
Result e820E88A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E88A8).
inline void c820E88A8(Regs& regs, uint8_t* base) { regs.Apply(e820E88A8(base, regs.ToArgs())); }
// sub_820E8908's original.
Result e820E8908(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E8908).
inline void c820E8908(Regs& regs, uint8_t* base) { regs.Apply(e820E8908(base, regs.ToArgs())); }
// sub_820E89B0's original.
Result e820E89B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E89B0).
inline void c820E89B0(Regs& regs, uint8_t* base) { regs.Apply(e820E89B0(base, regs.ToArgs())); }
// sub_820E8A00's original.
Result e820E8A00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E8A00).
inline void c820E8A00(Regs& regs, uint8_t* base) { regs.Apply(e820E8A00(base, regs.ToArgs())); }
// sub_820E8A80's original.
Result e820E8A80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E8A80).
inline void c820E8A80(Regs& regs, uint8_t* base) { regs.Apply(e820E8A80(base, regs.ToArgs())); }
// sub_820E99C0's original.
Result e820E99C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820E99C0).
inline void c820E99C0(Regs& regs, uint8_t* base) { regs.Apply(e820E99C0(base, regs.ToArgs())); }
// sub_820EA8E8's original.
Result e820EA8E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820EA8E8).
inline void c820EA8E8(Regs& regs, uint8_t* base) { regs.Apply(e820EA8E8(base, regs.ToArgs())); }
// sub_820EB320's original.
Result e820EB320(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820EB320).
inline void c820EB320(Regs& regs, uint8_t* base) { regs.Apply(e820EB320(base, regs.ToArgs())); }
// sub_820ECC48's original.
Result e820ECC48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ECC48).
inline void c820ECC48(Regs& regs, uint8_t* base) { regs.Apply(e820ECC48(base, regs.ToArgs())); }
// sub_820ECDD0's original.
Result e820ECDD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ECDD0).
inline void c820ECDD0(Regs& regs, uint8_t* base) { regs.Apply(e820ECDD0(base, regs.ToArgs())); }
// sub_820ECE28's original.
Result e820ECE28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ECE28).
inline void c820ECE28(Regs& regs, uint8_t* base) { regs.Apply(e820ECE28(base, regs.ToArgs())); }
// sub_820ECE40's original.
Result e820ECE40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ECE40).
inline void c820ECE40(Regs& regs, uint8_t* base) { regs.Apply(e820ECE40(base, regs.ToArgs())); }
// sub_820ECEB8's original.
Result e820ECEB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ECEB8).
inline void c820ECEB8(Regs& regs, uint8_t* base) { regs.Apply(e820ECEB8(base, regs.ToArgs())); }
// sub_820ECF08's original.
Result e820ECF08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ECF08).
inline void c820ECF08(Regs& regs, uint8_t* base) { regs.Apply(e820ECF08(base, regs.ToArgs())); }
// sub_820ECFC8's original.
Result e820ECFC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ECFC8).
inline void c820ECFC8(Regs& regs, uint8_t* base) { regs.Apply(e820ECFC8(base, regs.ToArgs())); }
// sub_820ED020's original.
Result e820ED020(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ED020).
inline void c820ED020(Regs& regs, uint8_t* base) { regs.Apply(e820ED020(base, regs.ToArgs())); }
// sub_820ED6D0's original.
Result e820ED6D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820ED6D0).
inline void c820ED6D0(Regs& regs, uint8_t* base) { regs.Apply(e820ED6D0(base, regs.ToArgs())); }
// sub_820EE638's original.
Result e820EE638(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820EE638).
inline void c820EE638(Regs& regs, uint8_t* base) { regs.Apply(e820EE638(base, regs.ToArgs())); }
// sub_820EF368's original.
Result e820EF368(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820EF368).
inline void c820EF368(Regs& regs, uint8_t* base) { regs.Apply(e820EF368(base, regs.ToArgs())); }
// sub_820EF660's original.
Result e820EF660(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820EF660).
inline void c820EF660(Regs& regs, uint8_t* base) { regs.Apply(e820EF660(base, regs.ToArgs())); }
// sub_820F0070's original.
Result e820F0070(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F0070).
inline void c820F0070(Regs& regs, uint8_t* base) { regs.Apply(e820F0070(base, regs.ToArgs())); }
// sub_820F0820's original.
Result e820F0820(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F0820).
inline void c820F0820(Regs& regs, uint8_t* base) { regs.Apply(e820F0820(base, regs.ToArgs())); }
// sub_820F0A90's original.
Result e820F0A90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F0A90).
inline void c820F0A90(Regs& regs, uint8_t* base) { regs.Apply(e820F0A90(base, regs.ToArgs())); }
// sub_820F0AD8's original.
Result e820F0AD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F0AD8).
inline void c820F0AD8(Regs& regs, uint8_t* base) { regs.Apply(e820F0AD8(base, regs.ToArgs())); }
// sub_820F0EB8's original.
Result e820F0EB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F0EB8).
inline void c820F0EB8(Regs& regs, uint8_t* base) { regs.Apply(e820F0EB8(base, regs.ToArgs())); }
// sub_820F0FE0's original.
Result e820F0FE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F0FE0).
inline void c820F0FE0(Regs& regs, uint8_t* base) { regs.Apply(e820F0FE0(base, regs.ToArgs())); }
// sub_820F1038's original.
Result e820F1038(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F1038).
inline void c820F1038(Regs& regs, uint8_t* base) { regs.Apply(e820F1038(base, regs.ToArgs())); }
// sub_820F1078's original.
Result e820F1078(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F1078).
inline void c820F1078(Regs& regs, uint8_t* base) { regs.Apply(e820F1078(base, regs.ToArgs())); }
// sub_820F1138's original.
Result e820F1138(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F1138).
inline void c820F1138(Regs& regs, uint8_t* base) { regs.Apply(e820F1138(base, regs.ToArgs())); }
// sub_820F11A8's original.
Result e820F11A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F11A8).
inline void c820F11A8(Regs& regs, uint8_t* base) { regs.Apply(e820F11A8(base, regs.ToArgs())); }
// sub_820F1208's original.
Result e820F1208(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F1208).
inline void c820F1208(Regs& regs, uint8_t* base) { regs.Apply(e820F1208(base, regs.ToArgs())); }
// sub_820F12F8's original.
Result e820F12F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F12F8).
inline void c820F12F8(Regs& regs, uint8_t* base) { regs.Apply(e820F12F8(base, regs.ToArgs())); }
// sub_820F1CE8's original.
Result e820F1CE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F1CE8).
inline void c820F1CE8(Regs& regs, uint8_t* base) { regs.Apply(e820F1CE8(base, regs.ToArgs())); }
// sub_820F22B8's original.
Result e820F22B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F22B8).
inline void c820F22B8(Regs& regs, uint8_t* base) { regs.Apply(e820F22B8(base, regs.ToArgs())); }
// sub_820F22F8's original.
Result e820F22F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F22F8).
inline void c820F22F8(Regs& regs, uint8_t* base) { regs.Apply(e820F22F8(base, regs.ToArgs())); }
// sub_820F23C0's original.
Result e820F23C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F23C0).
inline void c820F23C0(Regs& regs, uint8_t* base) { regs.Apply(e820F23C0(base, regs.ToArgs())); }
// sub_820F2520's original.
Result e820F2520(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F2520).
inline void c820F2520(Regs& regs, uint8_t* base) { regs.Apply(e820F2520(base, regs.ToArgs())); }
// sub_820F2638's original.
Result e820F2638(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F2638).
inline void c820F2638(Regs& regs, uint8_t* base) { regs.Apply(e820F2638(base, regs.ToArgs())); }
// sub_820F2AF8's original.
Result e820F2AF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F2AF8).
inline void c820F2AF8(Regs& regs, uint8_t* base) { regs.Apply(e820F2AF8(base, regs.ToArgs())); }
// sub_820F2B38's original.
Result e820F2B38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F2B38).
inline void c820F2B38(Regs& regs, uint8_t* base) { regs.Apply(e820F2B38(base, regs.ToArgs())); }
// sub_820F2D28's original.
Result e820F2D28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F2D28).
inline void c820F2D28(Regs& regs, uint8_t* base) { regs.Apply(e820F2D28(base, regs.ToArgs())); }
// sub_820F2EF8's original.
Result e820F2EF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F2EF8).
inline void c820F2EF8(Regs& regs, uint8_t* base) { regs.Apply(e820F2EF8(base, regs.ToArgs())); }
// sub_820F2F88's original.
Result e820F2F88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F2F88).
inline void c820F2F88(Regs& regs, uint8_t* base) { regs.Apply(e820F2F88(base, regs.ToArgs())); }
// sub_820F3020's original.
Result e820F3020(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F3020).
inline void c820F3020(Regs& regs, uint8_t* base) { regs.Apply(e820F3020(base, regs.ToArgs())); }
// sub_820F34C8's original.
Result e820F34C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F34C8).
inline void c820F34C8(Regs& regs, uint8_t* base) { regs.Apply(e820F34C8(base, regs.ToArgs())); }
// sub_820F4128's original.
Result e820F4128(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F4128).
inline void c820F4128(Regs& regs, uint8_t* base) { regs.Apply(e820F4128(base, regs.ToArgs())); }
// sub_820F51A8's original.
Result e820F51A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F51A8).
inline void c820F51A8(Regs& regs, uint8_t* base) { regs.Apply(e820F51A8(base, regs.ToArgs())); }
// sub_820F52A8's original.
Result e820F52A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F52A8).
inline void c820F52A8(Regs& regs, uint8_t* base) { regs.Apply(e820F52A8(base, regs.ToArgs())); }
// sub_820F5740's original.
Result e820F5740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F5740).
inline void c820F5740(Regs& regs, uint8_t* base) { regs.Apply(e820F5740(base, regs.ToArgs())); }
// sub_820F6068's original.
Result e820F6068(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F6068).
inline void c820F6068(Regs& regs, uint8_t* base) { regs.Apply(e820F6068(base, regs.ToArgs())); }
// sub_820F68E8's original.
Result e820F68E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F68E8).
inline void c820F68E8(Regs& regs, uint8_t* base) { regs.Apply(e820F68E8(base, regs.ToArgs())); }
// sub_820F6940's original.
Result e820F6940(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F6940).
inline void c820F6940(Regs& regs, uint8_t* base) { regs.Apply(e820F6940(base, regs.ToArgs())); }
// sub_820F8618's original.
Result e820F8618(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F8618).
inline void c820F8618(Regs& regs, uint8_t* base) { regs.Apply(e820F8618(base, regs.ToArgs())); }
// sub_820F9768's original.
Result e820F9768(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820F9768).
inline void c820F9768(Regs& regs, uint8_t* base) { regs.Apply(e820F9768(base, regs.ToArgs())); }
// sub_820FEBD0's original.
Result e820FEBD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820FEBD0).
inline void c820FEBD0(Regs& regs, uint8_t* base) { regs.Apply(e820FEBD0(base, regs.ToArgs())); }
// sub_820FEFB0's original.
Result e820FEFB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820FEFB0).
inline void c820FEFB0(Regs& regs, uint8_t* base) { regs.Apply(e820FEFB0(base, regs.ToArgs())); }
// sub_820FF1B0's original.
Result e820FF1B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820FF1B0).
inline void c820FF1B0(Regs& regs, uint8_t* base) { regs.Apply(e820FF1B0(base, regs.ToArgs())); }
// sub_820FF250's original.
Result e820FF250(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820FF250).
inline void c820FF250(Regs& regs, uint8_t* base) { regs.Apply(e820FF250(base, regs.ToArgs())); }
// sub_820FF860's original.
Result e820FF860(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820FF860).
inline void c820FF860(Regs& regs, uint8_t* base) { regs.Apply(e820FF860(base, regs.ToArgs())); }
// sub_820FFD88's original.
Result e820FFD88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_820FFD88).
inline void c820FFD88(Regs& regs, uint8_t* base) { regs.Apply(e820FFD88(base, regs.ToArgs())); }
// sub_821017C0's original.
Result e821017C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821017C0).
inline void c821017C0(Regs& regs, uint8_t* base) { regs.Apply(e821017C0(base, regs.ToArgs())); }
// sub_82101990's original.
Result e82101990(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82101990).
inline void c82101990(Regs& regs, uint8_t* base) { regs.Apply(e82101990(base, regs.ToArgs())); }
// sub_82101A10's original.
Result e82101A10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82101A10).
inline void c82101A10(Regs& regs, uint8_t* base) { regs.Apply(e82101A10(base, regs.ToArgs())); }
// sub_82101A98's original.
Result e82101A98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82101A98).
inline void c82101A98(Regs& regs, uint8_t* base) { regs.Apply(e82101A98(base, regs.ToArgs())); }
// sub_82101D38's original.
Result e82101D38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82101D38).
inline void c82101D38(Regs& regs, uint8_t* base) { regs.Apply(e82101D38(base, regs.ToArgs())); }
// sub_82101D50's original.
Result e82101D50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82101D50).
inline void c82101D50(Regs& regs, uint8_t* base) { regs.Apply(e82101D50(base, regs.ToArgs())); }
// sub_82101E90's original.
Result e82101E90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82101E90).
inline void c82101E90(Regs& regs, uint8_t* base) { regs.Apply(e82101E90(base, regs.ToArgs())); }
// sub_82101F68's original.
Result e82101F68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82101F68).
inline void c82101F68(Regs& regs, uint8_t* base) { regs.Apply(e82101F68(base, regs.ToArgs())); }
// sub_82102048's original.
Result e82102048(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82102048).
inline void c82102048(Regs& regs, uint8_t* base) { regs.Apply(e82102048(base, regs.ToArgs())); }
// sub_821020F0's original.
Result e821020F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821020F0).
inline void c821020F0(Regs& regs, uint8_t* base) { regs.Apply(e821020F0(base, regs.ToArgs())); }
// sub_821021A8's original.
Result e821021A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821021A8).
inline void c821021A8(Regs& regs, uint8_t* base) { regs.Apply(e821021A8(base, regs.ToArgs())); }
// sub_82102208's original.
Result e82102208(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82102208).
inline void c82102208(Regs& regs, uint8_t* base) { regs.Apply(e82102208(base, regs.ToArgs())); }
// sub_821046F8's original.
Result e821046F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821046F8).
inline void c821046F8(Regs& regs, uint8_t* base) { regs.Apply(e821046F8(base, regs.ToArgs())); }
// sub_82104898's original.
Result e82104898(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82104898).
inline void c82104898(Regs& regs, uint8_t* base) { regs.Apply(e82104898(base, regs.ToArgs())); }
// sub_82104D40's original.
Result e82104D40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82104D40).
inline void c82104D40(Regs& regs, uint8_t* base) { regs.Apply(e82104D40(base, regs.ToArgs())); }
// sub_82104E68's original.
Result e82104E68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82104E68).
inline void c82104E68(Regs& regs, uint8_t* base) { regs.Apply(e82104E68(base, regs.ToArgs())); }
// sub_82105508's original.
Result e82105508(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82105508).
inline void c82105508(Regs& regs, uint8_t* base) { regs.Apply(e82105508(base, regs.ToArgs())); }
// sub_82105560's original.
Result e82105560(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82105560).
inline void c82105560(Regs& regs, uint8_t* base) { regs.Apply(e82105560(base, regs.ToArgs())); }
// sub_821055D8's original.
Result e821055D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821055D8).
inline void c821055D8(Regs& regs, uint8_t* base) { regs.Apply(e821055D8(base, regs.ToArgs())); }
// sub_82105740's original.
Result e82105740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82105740).
inline void c82105740(Regs& regs, uint8_t* base) { regs.Apply(e82105740(base, regs.ToArgs())); }
// sub_821057C8's original.
Result e821057C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821057C8).
inline void c821057C8(Regs& regs, uint8_t* base) { regs.Apply(e821057C8(base, regs.ToArgs())); }
// sub_82105868's original.
Result e82105868(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82105868).
inline void c82105868(Regs& regs, uint8_t* base) { regs.Apply(e82105868(base, regs.ToArgs())); }
// sub_82106548's original.
Result e82106548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82106548).
inline void c82106548(Regs& regs, uint8_t* base) { regs.Apply(e82106548(base, regs.ToArgs())); }
// sub_82106618's original.
Result e82106618(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82106618).
inline void c82106618(Regs& regs, uint8_t* base) { regs.Apply(e82106618(base, regs.ToArgs())); }
// sub_82106868's original.
Result e82106868(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82106868).
inline void c82106868(Regs& regs, uint8_t* base) { regs.Apply(e82106868(base, regs.ToArgs())); }
// sub_82106AF0's original.
Result e82106AF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82106AF0).
inline void c82106AF0(Regs& regs, uint8_t* base) { regs.Apply(e82106AF0(base, regs.ToArgs())); }
// sub_821077A8's original.
Result e821077A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821077A8).
inline void c821077A8(Regs& regs, uint8_t* base) { regs.Apply(e821077A8(base, regs.ToArgs())); }
// sub_82107910's original.
Result e82107910(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82107910).
inline void c82107910(Regs& regs, uint8_t* base) { regs.Apply(e82107910(base, regs.ToArgs())); }
// sub_82107C58's original.
Result e82107C58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82107C58).
inline void c82107C58(Regs& regs, uint8_t* base) { regs.Apply(e82107C58(base, regs.ToArgs())); }
// sub_82108860's original.
Result e82108860(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82108860).
inline void c82108860(Regs& regs, uint8_t* base) { regs.Apply(e82108860(base, regs.ToArgs())); }
// sub_82109498's original.
Result e82109498(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82109498).
inline void c82109498(Regs& regs, uint8_t* base) { regs.Apply(e82109498(base, regs.ToArgs())); }
// sub_82109EF0's original.
Result e82109EF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82109EF0).
inline void c82109EF0(Regs& regs, uint8_t* base) { regs.Apply(e82109EF0(base, regs.ToArgs())); }
// sub_8210A400's original.
Result e8210A400(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A400).
inline void c8210A400(Regs& regs, uint8_t* base) { regs.Apply(e8210A400(base, regs.ToArgs())); }
// sub_8210A490's original.
Result e8210A490(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A490).
inline void c8210A490(Regs& regs, uint8_t* base) { regs.Apply(e8210A490(base, regs.ToArgs())); }
// sub_8210A538's original.
Result e8210A538(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A538).
inline void c8210A538(Regs& regs, uint8_t* base) { regs.Apply(e8210A538(base, regs.ToArgs())); }
// sub_8210A5E8's original.
Result e8210A5E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A5E8).
inline void c8210A5E8(Regs& regs, uint8_t* base) { regs.Apply(e8210A5E8(base, regs.ToArgs())); }
// sub_8210A758's original.
Result e8210A758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A758).
inline void c8210A758(Regs& regs, uint8_t* base) { regs.Apply(e8210A758(base, regs.ToArgs())); }
// sub_8210A850's original.
Result e8210A850(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A850).
inline void c8210A850(Regs& regs, uint8_t* base) { regs.Apply(e8210A850(base, regs.ToArgs())); }
// sub_8210A8D0's original.
Result e8210A8D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A8D0).
inline void c8210A8D0(Regs& regs, uint8_t* base) { regs.Apply(e8210A8D0(base, regs.ToArgs())); }
// sub_8210A990's original.
Result e8210A990(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210A990).
inline void c8210A990(Regs& regs, uint8_t* base) { regs.Apply(e8210A990(base, regs.ToArgs())); }
// sub_8210ABC0's original.
Result e8210ABC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210ABC0).
inline void c8210ABC0(Regs& regs, uint8_t* base) { regs.Apply(e8210ABC0(base, regs.ToArgs())); }
// sub_8210ADD0's original.
Result e8210ADD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210ADD0).
inline void c8210ADD0(Regs& regs, uint8_t* base) { regs.Apply(e8210ADD0(base, regs.ToArgs())); }
// sub_8210ADF8's original.
Result e8210ADF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210ADF8).
inline void c8210ADF8(Regs& regs, uint8_t* base) { regs.Apply(e8210ADF8(base, regs.ToArgs())); }
// sub_8210AF08's original.
Result e8210AF08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210AF08).
inline void c8210AF08(Regs& regs, uint8_t* base) { regs.Apply(e8210AF08(base, regs.ToArgs())); }
// sub_8210B018's original.
Result e8210B018(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210B018).
inline void c8210B018(Regs& regs, uint8_t* base) { regs.Apply(e8210B018(base, regs.ToArgs())); }
// sub_8210B270's original.
Result e8210B270(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210B270).
inline void c8210B270(Regs& regs, uint8_t* base) { regs.Apply(e8210B270(base, regs.ToArgs())); }
// sub_8210B568's original.
Result e8210B568(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210B568).
inline void c8210B568(Regs& regs, uint8_t* base) { regs.Apply(e8210B568(base, regs.ToArgs())); }
// sub_8210B5B8's original.
Result e8210B5B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210B5B8).
inline void c8210B5B8(Regs& regs, uint8_t* base) { regs.Apply(e8210B5B8(base, regs.ToArgs())); }
// sub_8210B698's original.
Result e8210B698(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210B698).
inline void c8210B698(Regs& regs, uint8_t* base) { regs.Apply(e8210B698(base, regs.ToArgs())); }
// sub_8210BA28's original.
Result e8210BA28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BA28).
inline void c8210BA28(Regs& regs, uint8_t* base) { regs.Apply(e8210BA28(base, regs.ToArgs())); }
// sub_8210BA68's original.
Result e8210BA68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BA68).
inline void c8210BA68(Regs& regs, uint8_t* base) { regs.Apply(e8210BA68(base, regs.ToArgs())); }
// sub_8210BDF0's original.
Result e8210BDF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BDF0).
inline void c8210BDF0(Regs& regs, uint8_t* base) { regs.Apply(e8210BDF0(base, regs.ToArgs())); }
// sub_8210BE30's original.
Result e8210BE30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BE30).
inline void c8210BE30(Regs& regs, uint8_t* base) { regs.Apply(e8210BE30(base, regs.ToArgs())); }
// sub_8210BE70's original.
Result e8210BE70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BE70).
inline void c8210BE70(Regs& regs, uint8_t* base) { regs.Apply(e8210BE70(base, regs.ToArgs())); }
// sub_8210BED8's original.
Result e8210BED8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BED8).
inline void c8210BED8(Regs& regs, uint8_t* base) { regs.Apply(e8210BED8(base, regs.ToArgs())); }
// sub_8210BF40's original.
Result e8210BF40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BF40).
inline void c8210BF40(Regs& regs, uint8_t* base) { regs.Apply(e8210BF40(base, regs.ToArgs())); }
// sub_8210BFA8's original.
Result e8210BFA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210BFA8).
inline void c8210BFA8(Regs& regs, uint8_t* base) { regs.Apply(e8210BFA8(base, regs.ToArgs())); }
// sub_8210C010's original.
Result e8210C010(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210C010).
inline void c8210C010(Regs& regs, uint8_t* base) { regs.Apply(e8210C010(base, regs.ToArgs())); }
// sub_8210C370's original.
Result e8210C370(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210C370).
inline void c8210C370(Regs& regs, uint8_t* base) { regs.Apply(e8210C370(base, regs.ToArgs())); }
// sub_8210C998's original.
Result e8210C998(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8210C998).
inline void c8210C998(Regs& regs, uint8_t* base) { regs.Apply(e8210C998(base, regs.ToArgs())); }
// sub_82110258's original.
Result e82110258(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82110258).
inline void c82110258(Regs& regs, uint8_t* base) { regs.Apply(e82110258(base, regs.ToArgs())); }
// sub_82110448's original.
Result e82110448(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82110448).
inline void c82110448(Regs& regs, uint8_t* base) { regs.Apply(e82110448(base, regs.ToArgs())); }
// sub_82111FC8's original.
Result e82111FC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82111FC8).
inline void c82111FC8(Regs& regs, uint8_t* base) { regs.Apply(e82111FC8(base, regs.ToArgs())); }
// sub_82112950's original.
Result e82112950(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82112950).
inline void c82112950(Regs& regs, uint8_t* base) { regs.Apply(e82112950(base, regs.ToArgs())); }
// sub_82112DB0's original.
Result e82112DB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82112DB0).
inline void c82112DB0(Regs& regs, uint8_t* base) { regs.Apply(e82112DB0(base, regs.ToArgs())); }
// sub_82113E60's original.
Result e82113E60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82113E60).
inline void c82113E60(Regs& regs, uint8_t* base) { regs.Apply(e82113E60(base, regs.ToArgs())); }
// sub_82114268's original.
Result e82114268(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82114268).
inline void c82114268(Regs& regs, uint8_t* base) { regs.Apply(e82114268(base, regs.ToArgs())); }
// sub_82115190's original.
Result e82115190(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82115190).
inline void c82115190(Regs& regs, uint8_t* base) { regs.Apply(e82115190(base, regs.ToArgs())); }
// sub_821159B0's original.
Result e821159B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821159B0).
inline void c821159B0(Regs& regs, uint8_t* base) { regs.Apply(e821159B0(base, regs.ToArgs())); }
// sub_82115D98's original.
Result e82115D98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82115D98).
inline void c82115D98(Regs& regs, uint8_t* base) { regs.Apply(e82115D98(base, regs.ToArgs())); }
// sub_821162B0's original.
Result e821162B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821162B0).
inline void c821162B0(Regs& regs, uint8_t* base) { regs.Apply(e821162B0(base, regs.ToArgs())); }
// sub_82116588's original.
Result e82116588(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82116588).
inline void c82116588(Regs& regs, uint8_t* base) { regs.Apply(e82116588(base, regs.ToArgs())); }
// sub_82116AF0's original.
Result e82116AF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82116AF0).
inline void c82116AF0(Regs& regs, uint8_t* base) { regs.Apply(e82116AF0(base, regs.ToArgs())); }
// sub_82116DA8's original.
Result e82116DA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82116DA8).
inline void c82116DA8(Regs& regs, uint8_t* base) { regs.Apply(e82116DA8(base, regs.ToArgs())); }
// sub_82116DB0's original.
Result e82116DB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82116DB0).
inline void c82116DB0(Regs& regs, uint8_t* base) { regs.Apply(e82116DB0(base, regs.ToArgs())); }
// sub_82116FF0's original.
Result e82116FF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82116FF0).
inline void c82116FF0(Regs& regs, uint8_t* base) { regs.Apply(e82116FF0(base, regs.ToArgs())); }
// sub_82117058's original.
Result e82117058(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117058).
inline void c82117058(Regs& regs, uint8_t* base) { regs.Apply(e82117058(base, regs.ToArgs())); }
// sub_821170C0's original.
Result e821170C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821170C0).
inline void c821170C0(Regs& regs, uint8_t* base) { regs.Apply(e821170C0(base, regs.ToArgs())); }
// sub_821170D8's original.
Result e821170D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821170D8).
inline void c821170D8(Regs& regs, uint8_t* base) { regs.Apply(e821170D8(base, regs.ToArgs())); }
// sub_821170F0's original.
Result e821170F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821170F0).
inline void c821170F0(Regs& regs, uint8_t* base) { regs.Apply(e821170F0(base, regs.ToArgs())); }
// sub_82117158's original.
Result e82117158(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117158).
inline void c82117158(Regs& regs, uint8_t* base) { regs.Apply(e82117158(base, regs.ToArgs())); }
// sub_82117178's original.
Result e82117178(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117178).
inline void c82117178(Regs& regs, uint8_t* base) { regs.Apply(e82117178(base, regs.ToArgs())); }
// sub_82117190's original.
Result e82117190(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117190).
inline void c82117190(Regs& regs, uint8_t* base) { regs.Apply(e82117190(base, regs.ToArgs())); }
// sub_821171B0's original.
Result e821171B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821171B0).
inline void c821171B0(Regs& regs, uint8_t* base) { regs.Apply(e821171B0(base, regs.ToArgs())); }
// sub_82117210's original.
Result e82117210(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117210).
inline void c82117210(Regs& regs, uint8_t* base) { regs.Apply(e82117210(base, regs.ToArgs())); }
// sub_82117228's original.
Result e82117228(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117228).
inline void c82117228(Regs& regs, uint8_t* base) { regs.Apply(e82117228(base, regs.ToArgs())); }
// sub_82117240's original.
Result e82117240(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117240).
inline void c82117240(Regs& regs, uint8_t* base) { regs.Apply(e82117240(base, regs.ToArgs())); }
// sub_821173F0's original.
Result e821173F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821173F0).
inline void c821173F0(Regs& regs, uint8_t* base) { regs.Apply(e821173F0(base, regs.ToArgs())); }
// sub_82117560's original.
Result e82117560(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117560).
inline void c82117560(Regs& regs, uint8_t* base) { regs.Apply(e82117560(base, regs.ToArgs())); }
// sub_82117568's original.
Result e82117568(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117568).
inline void c82117568(Regs& regs, uint8_t* base) { regs.Apply(e82117568(base, regs.ToArgs())); }
// sub_82117578's original.
Result e82117578(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117578).
inline void c82117578(Regs& regs, uint8_t* base) { regs.Apply(e82117578(base, regs.ToArgs())); }
// sub_821175B8's original.
Result e821175B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821175B8).
inline void c821175B8(Regs& regs, uint8_t* base) { regs.Apply(e821175B8(base, regs.ToArgs())); }
// sub_82117AD8's original.
Result e82117AD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117AD8).
inline void c82117AD8(Regs& regs, uint8_t* base) { regs.Apply(e82117AD8(base, regs.ToArgs())); }
// sub_82117B30's original.
Result e82117B30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117B30).
inline void c82117B30(Regs& regs, uint8_t* base) { regs.Apply(e82117B30(base, regs.ToArgs())); }
// sub_82117E08's original.
Result e82117E08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117E08).
inline void c82117E08(Regs& regs, uint8_t* base) { regs.Apply(e82117E08(base, regs.ToArgs())); }
// sub_82117E10's original.
Result e82117E10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117E10).
inline void c82117E10(Regs& regs, uint8_t* base) { regs.Apply(e82117E10(base, regs.ToArgs())); }
// sub_82117E18's original.
Result e82117E18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117E18).
inline void c82117E18(Regs& regs, uint8_t* base) { regs.Apply(e82117E18(base, regs.ToArgs())); }
// sub_82117EA8's original.
Result e82117EA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82117EA8).
inline void c82117EA8(Regs& regs, uint8_t* base) { regs.Apply(e82117EA8(base, regs.ToArgs())); }
// sub_82118190's original.
Result e82118190(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82118190).
inline void c82118190(Regs& regs, uint8_t* base) { regs.Apply(e82118190(base, regs.ToArgs())); }
// sub_82118200's original.
Result e82118200(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82118200).
inline void c82118200(Regs& regs, uint8_t* base) { regs.Apply(e82118200(base, regs.ToArgs())); }
// sub_821182E0's original.
Result e821182E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821182E0).
inline void c821182E0(Regs& regs, uint8_t* base) { regs.Apply(e821182E0(base, regs.ToArgs())); }
// sub_82118400's original.
Result e82118400(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82118400).
inline void c82118400(Regs& regs, uint8_t* base) { regs.Apply(e82118400(base, regs.ToArgs())); }
// sub_821184E8's original.
Result e821184E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821184E8).
inline void c821184E8(Regs& regs, uint8_t* base) { regs.Apply(e821184E8(base, regs.ToArgs())); }
// sub_82118720's original.
Result e82118720(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82118720).
inline void c82118720(Regs& regs, uint8_t* base) { regs.Apply(e82118720(base, regs.ToArgs())); }
// sub_82118B58's original.
Result e82118B58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82118B58).
inline void c82118B58(Regs& regs, uint8_t* base) { regs.Apply(e82118B58(base, regs.ToArgs())); }
// sub_82118C18's original.
Result e82118C18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82118C18).
inline void c82118C18(Regs& regs, uint8_t* base) { regs.Apply(e82118C18(base, regs.ToArgs())); }
// sub_82118FB0's original.
Result e82118FB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82118FB0).
inline void c82118FB0(Regs& regs, uint8_t* base) { regs.Apply(e82118FB0(base, regs.ToArgs())); }
// sub_821194A0's original.
Result e821194A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821194A0).
inline void c821194A0(Regs& regs, uint8_t* base) { regs.Apply(e821194A0(base, regs.ToArgs())); }
// sub_82119B38's original.
Result e82119B38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82119B38).
inline void c82119B38(Regs& regs, uint8_t* base) { regs.Apply(e82119B38(base, regs.ToArgs())); }
// sub_82119B80's original.
Result e82119B80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82119B80).
inline void c82119B80(Regs& regs, uint8_t* base) { regs.Apply(e82119B80(base, regs.ToArgs())); }
// sub_8211A1E0's original.
Result e8211A1E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A1E0).
inline void c8211A1E0(Regs& regs, uint8_t* base) { regs.Apply(e8211A1E0(base, regs.ToArgs())); }
// sub_8211A578's original.
Result e8211A578(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A578).
inline void c8211A578(Regs& regs, uint8_t* base) { regs.Apply(e8211A578(base, regs.ToArgs())); }
// sub_8211A5B8's original.
Result e8211A5B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A5B8).
inline void c8211A5B8(Regs& regs, uint8_t* base) { regs.Apply(e8211A5B8(base, regs.ToArgs())); }
// sub_8211A630's original.
Result e8211A630(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A630).
inline void c8211A630(Regs& regs, uint8_t* base) { regs.Apply(e8211A630(base, regs.ToArgs())); }
// sub_8211A6C0's original.
Result e8211A6C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A6C0).
inline void c8211A6C0(Regs& regs, uint8_t* base) { regs.Apply(e8211A6C0(base, regs.ToArgs())); }
// sub_8211A708's original.
Result e8211A708(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A708).
inline void c8211A708(Regs& regs, uint8_t* base) { regs.Apply(e8211A708(base, regs.ToArgs())); }
// sub_8211A818's original.
Result e8211A818(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A818).
inline void c8211A818(Regs& regs, uint8_t* base) { regs.Apply(e8211A818(base, regs.ToArgs())); }
// sub_8211A8C8's original.
Result e8211A8C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A8C8).
inline void c8211A8C8(Regs& regs, uint8_t* base) { regs.Apply(e8211A8C8(base, regs.ToArgs())); }
// sub_8211A948's original.
Result e8211A948(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A948).
inline void c8211A948(Regs& regs, uint8_t* base) { regs.Apply(e8211A948(base, regs.ToArgs())); }
// sub_8211A9E8's original.
Result e8211A9E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211A9E8).
inline void c8211A9E8(Regs& regs, uint8_t* base) { regs.Apply(e8211A9E8(base, regs.ToArgs())); }
// sub_8211AAA8's original.
Result e8211AAA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AAA8).
inline void c8211AAA8(Regs& regs, uint8_t* base) { regs.Apply(e8211AAA8(base, regs.ToArgs())); }
// sub_8211AB90's original.
Result e8211AB90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AB90).
inline void c8211AB90(Regs& regs, uint8_t* base) { regs.Apply(e8211AB90(base, regs.ToArgs())); }
// sub_8211ABA8's original.
Result e8211ABA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211ABA8).
inline void c8211ABA8(Regs& regs, uint8_t* base) { regs.Apply(e8211ABA8(base, regs.ToArgs())); }
// sub_8211AC00's original.
Result e8211AC00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AC00).
inline void c8211AC00(Regs& regs, uint8_t* base) { regs.Apply(e8211AC00(base, regs.ToArgs())); }
// sub_8211AC48's original.
Result e8211AC48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AC48).
inline void c8211AC48(Regs& regs, uint8_t* base) { regs.Apply(e8211AC48(base, regs.ToArgs())); }
// sub_8211AC90's original.
Result e8211AC90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AC90).
inline void c8211AC90(Regs& regs, uint8_t* base) { regs.Apply(e8211AC90(base, regs.ToArgs())); }
// sub_8211AD00's original.
Result e8211AD00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AD00).
inline void c8211AD00(Regs& regs, uint8_t* base) { regs.Apply(e8211AD00(base, regs.ToArgs())); }
// sub_8211AE68's original.
Result e8211AE68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AE68).
inline void c8211AE68(Regs& regs, uint8_t* base) { regs.Apply(e8211AE68(base, regs.ToArgs())); }
// sub_8211AED0's original.
Result e8211AED0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211AED0).
inline void c8211AED0(Regs& regs, uint8_t* base) { regs.Apply(e8211AED0(base, regs.ToArgs())); }
// sub_8211B080's original.
Result e8211B080(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B080).
inline void c8211B080(Regs& regs, uint8_t* base) { regs.Apply(e8211B080(base, regs.ToArgs())); }
// sub_8211B0E8's original.
Result e8211B0E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B0E8).
inline void c8211B0E8(Regs& regs, uint8_t* base) { regs.Apply(e8211B0E8(base, regs.ToArgs())); }
// sub_8211B310's original.
Result e8211B310(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B310).
inline void c8211B310(Regs& regs, uint8_t* base) { regs.Apply(e8211B310(base, regs.ToArgs())); }
// sub_8211B370's original.
Result e8211B370(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B370).
inline void c8211B370(Regs& regs, uint8_t* base) { regs.Apply(e8211B370(base, regs.ToArgs())); }
// sub_8211B3F0's original.
Result e8211B3F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B3F0).
inline void c8211B3F0(Regs& regs, uint8_t* base) { regs.Apply(e8211B3F0(base, regs.ToArgs())); }
// sub_8211B4D0's original.
Result e8211B4D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B4D0).
inline void c8211B4D0(Regs& regs, uint8_t* base) { regs.Apply(e8211B4D0(base, regs.ToArgs())); }
// sub_8211B660's original.
Result e8211B660(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B660).
inline void c8211B660(Regs& regs, uint8_t* base) { regs.Apply(e8211B660(base, regs.ToArgs())); }
// sub_8211B720's original.
Result e8211B720(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B720).
inline void c8211B720(Regs& regs, uint8_t* base) { regs.Apply(e8211B720(base, regs.ToArgs())); }
// sub_8211B770's original.
Result e8211B770(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211B770).
inline void c8211B770(Regs& regs, uint8_t* base) { regs.Apply(e8211B770(base, regs.ToArgs())); }
// sub_8211BA00's original.
Result e8211BA00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211BA00).
inline void c8211BA00(Regs& regs, uint8_t* base) { regs.Apply(e8211BA00(base, regs.ToArgs())); }
// sub_8211BB18's original.
Result e8211BB18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211BB18).
inline void c8211BB18(Regs& regs, uint8_t* base) { regs.Apply(e8211BB18(base, regs.ToArgs())); }
// sub_8211BD10's original.
Result e8211BD10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211BD10).
inline void c8211BD10(Regs& regs, uint8_t* base) { regs.Apply(e8211BD10(base, regs.ToArgs())); }
// sub_8211BE98's original.
Result e8211BE98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211BE98).
inline void c8211BE98(Regs& regs, uint8_t* base) { regs.Apply(e8211BE98(base, regs.ToArgs())); }
// sub_8211BEE8's original.
Result e8211BEE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211BEE8).
inline void c8211BEE8(Regs& regs, uint8_t* base) { regs.Apply(e8211BEE8(base, regs.ToArgs())); }
// sub_8211C168's original.
Result e8211C168(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C168).
inline void c8211C168(Regs& regs, uint8_t* base) { regs.Apply(e8211C168(base, regs.ToArgs())); }
// sub_8211C1F8's original.
Result e8211C1F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C1F8).
inline void c8211C1F8(Regs& regs, uint8_t* base) { regs.Apply(e8211C1F8(base, regs.ToArgs())); }
// sub_8211C370's original.
Result e8211C370(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C370).
inline void c8211C370(Regs& regs, uint8_t* base) { regs.Apply(e8211C370(base, regs.ToArgs())); }
// sub_8211C3F8's original.
Result e8211C3F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C3F8).
inline void c8211C3F8(Regs& regs, uint8_t* base) { regs.Apply(e8211C3F8(base, regs.ToArgs())); }
// sub_8211C470's original.
Result e8211C470(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C470).
inline void c8211C470(Regs& regs, uint8_t* base) { regs.Apply(e8211C470(base, regs.ToArgs())); }
// sub_8211C4F0's original.
Result e8211C4F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C4F0).
inline void c8211C4F0(Regs& regs, uint8_t* base) { regs.Apply(e8211C4F0(base, regs.ToArgs())); }
// sub_8211C570's original.
Result e8211C570(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C570).
inline void c8211C570(Regs& regs, uint8_t* base) { regs.Apply(e8211C570(base, regs.ToArgs())); }
// sub_8211C5F8's original.
Result e8211C5F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C5F8).
inline void c8211C5F8(Regs& regs, uint8_t* base) { regs.Apply(e8211C5F8(base, regs.ToArgs())); }
// sub_8211C680's original.
Result e8211C680(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C680).
inline void c8211C680(Regs& regs, uint8_t* base) { regs.Apply(e8211C680(base, regs.ToArgs())); }
// sub_8211C740's original.
Result e8211C740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C740).
inline void c8211C740(Regs& regs, uint8_t* base) { regs.Apply(e8211C740(base, regs.ToArgs())); }
// sub_8211C7E8's original.
Result e8211C7E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C7E8).
inline void c8211C7E8(Regs& regs, uint8_t* base) { regs.Apply(e8211C7E8(base, regs.ToArgs())); }
// sub_8211C8A0's original.
Result e8211C8A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211C8A0).
inline void c8211C8A0(Regs& regs, uint8_t* base) { regs.Apply(e8211C8A0(base, regs.ToArgs())); }
// sub_8211CA18's original.
Result e8211CA18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211CA18).
inline void c8211CA18(Regs& regs, uint8_t* base) { regs.Apply(e8211CA18(base, regs.ToArgs())); }
// sub_8211CBD8's original.
Result e8211CBD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211CBD8).
inline void c8211CBD8(Regs& regs, uint8_t* base) { regs.Apply(e8211CBD8(base, regs.ToArgs())); }
// sub_8211CC50's original.
Result e8211CC50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211CC50).
inline void c8211CC50(Regs& regs, uint8_t* base) { regs.Apply(e8211CC50(base, regs.ToArgs())); }
// sub_8211CCC0's original.
Result e8211CCC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211CCC0).
inline void c8211CCC0(Regs& regs, uint8_t* base) { regs.Apply(e8211CCC0(base, regs.ToArgs())); }
// sub_8211CD28's original.
Result e8211CD28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211CD28).
inline void c8211CD28(Regs& regs, uint8_t* base) { regs.Apply(e8211CD28(base, regs.ToArgs())); }
// sub_8211CD90's original.
Result e8211CD90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211CD90).
inline void c8211CD90(Regs& regs, uint8_t* base) { regs.Apply(e8211CD90(base, regs.ToArgs())); }
// sub_8211D280's original.
Result e8211D280(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211D280).
inline void c8211D280(Regs& regs, uint8_t* base) { regs.Apply(e8211D280(base, regs.ToArgs())); }
// sub_8211D2F8's original.
Result e8211D2F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211D2F8).
inline void c8211D2F8(Regs& regs, uint8_t* base) { regs.Apply(e8211D2F8(base, regs.ToArgs())); }
// sub_8211D880's original.
Result e8211D880(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211D880).
inline void c8211D880(Regs& regs, uint8_t* base) { regs.Apply(e8211D880(base, regs.ToArgs())); }
// sub_8211D8D8's original.
Result e8211D8D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211D8D8).
inline void c8211D8D8(Regs& regs, uint8_t* base) { regs.Apply(e8211D8D8(base, regs.ToArgs())); }
// sub_8211D930's original.
Result e8211D930(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211D930).
inline void c8211D930(Regs& regs, uint8_t* base) { regs.Apply(e8211D930(base, regs.ToArgs())); }
// sub_8211D978's original.
Result e8211D978(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211D978).
inline void c8211D978(Regs& regs, uint8_t* base) { regs.Apply(e8211D978(base, regs.ToArgs())); }
// sub_8211DAC8's original.
Result e8211DAC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DAC8).
inline void c8211DAC8(Regs& regs, uint8_t* base) { regs.Apply(e8211DAC8(base, regs.ToArgs())); }
// sub_8211DBE0's original.
Result e8211DBE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DBE0).
inline void c8211DBE0(Regs& regs, uint8_t* base) { regs.Apply(e8211DBE0(base, regs.ToArgs())); }
// sub_8211DDD8's original.
Result e8211DDD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DDD8).
inline void c8211DDD8(Regs& regs, uint8_t* base) { regs.Apply(e8211DDD8(base, regs.ToArgs())); }
// sub_8211DDE0's original.
Result e8211DDE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DDE0).
inline void c8211DDE0(Regs& regs, uint8_t* base) { regs.Apply(e8211DDE0(base, regs.ToArgs())); }
// sub_8211DDE8's original.
Result e8211DDE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DDE8).
inline void c8211DDE8(Regs& regs, uint8_t* base) { regs.Apply(e8211DDE8(base, regs.ToArgs())); }
// sub_8211DE28's original.
Result e8211DE28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DE28).
inline void c8211DE28(Regs& regs, uint8_t* base) { regs.Apply(e8211DE28(base, regs.ToArgs())); }
// sub_8211DE30's original.
Result e8211DE30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DE30).
inline void c8211DE30(Regs& regs, uint8_t* base) { regs.Apply(e8211DE30(base, regs.ToArgs())); }
// sub_8211DE38's original.
Result e8211DE38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DE38).
inline void c8211DE38(Regs& regs, uint8_t* base) { regs.Apply(e8211DE38(base, regs.ToArgs())); }
// sub_8211DF20's original.
Result e8211DF20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DF20).
inline void c8211DF20(Regs& regs, uint8_t* base) { regs.Apply(e8211DF20(base, regs.ToArgs())); }
// sub_8211DFF8's original.
Result e8211DFF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211DFF8).
inline void c8211DFF8(Regs& regs, uint8_t* base) { regs.Apply(e8211DFF8(base, regs.ToArgs())); }
// sub_8211E160's original.
Result e8211E160(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E160).
inline void c8211E160(Regs& regs, uint8_t* base) { regs.Apply(e8211E160(base, regs.ToArgs())); }
// sub_8211E1B8's original.
Result e8211E1B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E1B8).
inline void c8211E1B8(Regs& regs, uint8_t* base) { regs.Apply(e8211E1B8(base, regs.ToArgs())); }
// sub_8211E1C0's original.
Result e8211E1C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E1C0).
inline void c8211E1C0(Regs& regs, uint8_t* base) { regs.Apply(e8211E1C0(base, regs.ToArgs())); }
// sub_8211E258's original.
Result e8211E258(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E258).
inline void c8211E258(Regs& regs, uint8_t* base) { regs.Apply(e8211E258(base, regs.ToArgs())); }
// sub_8211E370's original.
Result e8211E370(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E370).
inline void c8211E370(Regs& regs, uint8_t* base) { regs.Apply(e8211E370(base, regs.ToArgs())); }
// sub_8211E378's original.
Result e8211E378(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E378).
inline void c8211E378(Regs& regs, uint8_t* base) { regs.Apply(e8211E378(base, regs.ToArgs())); }
// sub_8211E500's original.
Result e8211E500(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E500).
inline void c8211E500(Regs& regs, uint8_t* base) { regs.Apply(e8211E500(base, regs.ToArgs())); }
// sub_8211E798's original.
Result e8211E798(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E798).
inline void c8211E798(Regs& regs, uint8_t* base) { regs.Apply(e8211E798(base, regs.ToArgs())); }
// sub_8211E808's original.
Result e8211E808(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8211E808).
inline void c8211E808(Regs& regs, uint8_t* base) { regs.Apply(e8211E808(base, regs.ToArgs())); }
// sub_82124EE0's original.
Result e82124EE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82124EE0).
inline void c82124EE0(Regs& regs, uint8_t* base) { regs.Apply(e82124EE0(base, regs.ToArgs())); }
// sub_821252D0's original.
Result e821252D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821252D0).
inline void c821252D0(Regs& regs, uint8_t* base) { regs.Apply(e821252D0(base, regs.ToArgs())); }
// sub_82125B60's original.
Result e82125B60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82125B60).
inline void c82125B60(Regs& regs, uint8_t* base) { regs.Apply(e82125B60(base, regs.ToArgs())); }
// sub_821270E8's original.
Result e821270E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821270E8).
inline void c821270E8(Regs& regs, uint8_t* base) { regs.Apply(e821270E8(base, regs.ToArgs())); }
// sub_821281A8's original.
Result e821281A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821281A8).
inline void c821281A8(Regs& regs, uint8_t* base) { regs.Apply(e821281A8(base, regs.ToArgs())); }
// sub_82128750's original.
Result e82128750(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82128750).
inline void c82128750(Regs& regs, uint8_t* base) { regs.Apply(e82128750(base, regs.ToArgs())); }
// sub_82128E70's original.
Result e82128E70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82128E70).
inline void c82128E70(Regs& regs, uint8_t* base) { regs.Apply(e82128E70(base, regs.ToArgs())); }
// sub_82129280's original.
Result e82129280(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82129280).
inline void c82129280(Regs& regs, uint8_t* base) { regs.Apply(e82129280(base, regs.ToArgs())); }
// sub_821292E8's original.
Result e821292E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821292E8).
inline void c821292E8(Regs& regs, uint8_t* base) { regs.Apply(e821292E8(base, regs.ToArgs())); }
// sub_8212AA78's original.
Result e8212AA78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8212AA78).
inline void c8212AA78(Regs& regs, uint8_t* base) { regs.Apply(e8212AA78(base, regs.ToArgs())); }
// sub_8212B358's original.
Result e8212B358(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8212B358).
inline void c8212B358(Regs& regs, uint8_t* base) { regs.Apply(e8212B358(base, regs.ToArgs())); }
// sub_8212B748's original.
Result e8212B748(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8212B748).
inline void c8212B748(Regs& regs, uint8_t* base) { regs.Apply(e8212B748(base, regs.ToArgs())); }
// sub_8212C070's original.
Result e8212C070(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8212C070).
inline void c8212C070(Regs& regs, uint8_t* base) { regs.Apply(e8212C070(base, regs.ToArgs())); }
// sub_8212C2F0's original.
Result e8212C2F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8212C2F0).
inline void c8212C2F0(Regs& regs, uint8_t* base) { regs.Apply(e8212C2F0(base, regs.ToArgs())); }
// sub_8212D818's original.
Result e8212D818(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8212D818).
inline void c8212D818(Regs& regs, uint8_t* base) { regs.Apply(e8212D818(base, regs.ToArgs())); }
// sub_8212DDC8's original.
Result e8212DDC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8212DDC8).
inline void c8212DDC8(Regs& regs, uint8_t* base) { regs.Apply(e8212DDC8(base, regs.ToArgs())); }
// sub_82130198's original.
Result e82130198(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82130198).
inline void c82130198(Regs& regs, uint8_t* base) { regs.Apply(e82130198(base, regs.ToArgs())); }
// sub_82130978's original.
Result e82130978(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82130978).
inline void c82130978(Regs& regs, uint8_t* base) { regs.Apply(e82130978(base, regs.ToArgs())); }
// sub_821329B8's original.
Result e821329B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821329B8).
inline void c821329B8(Regs& regs, uint8_t* base) { regs.Apply(e821329B8(base, regs.ToArgs())); }
// sub_82132FC8's original.
Result e82132FC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82132FC8).
inline void c82132FC8(Regs& regs, uint8_t* base) { regs.Apply(e82132FC8(base, regs.ToArgs())); }
// sub_821332F0's original.
Result e821332F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821332F0).
inline void c821332F0(Regs& regs, uint8_t* base) { regs.Apply(e821332F0(base, regs.ToArgs())); }
// sub_82133340's original.
Result e82133340(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82133340).
inline void c82133340(Regs& regs, uint8_t* base) { regs.Apply(e82133340(base, regs.ToArgs())); }
// sub_82133608's original.
Result e82133608(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82133608).
inline void c82133608(Regs& regs, uint8_t* base) { regs.Apply(e82133608(base, regs.ToArgs())); }
// sub_82133790's original.
Result e82133790(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82133790).
inline void c82133790(Regs& regs, uint8_t* base) { regs.Apply(e82133790(base, regs.ToArgs())); }
// sub_82133C60's original.
Result e82133C60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82133C60).
inline void c82133C60(Regs& regs, uint8_t* base) { regs.Apply(e82133C60(base, regs.ToArgs())); }
// sub_82133D00's original.
Result e82133D00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82133D00).
inline void c82133D00(Regs& regs, uint8_t* base) { regs.Apply(e82133D00(base, regs.ToArgs())); }
// sub_82133DD8's original.
Result e82133DD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82133DD8).
inline void c82133DD8(Regs& regs, uint8_t* base) { regs.Apply(e82133DD8(base, regs.ToArgs())); }
// sub_82133F58's original.
Result e82133F58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82133F58).
inline void c82133F58(Regs& regs, uint8_t* base) { regs.Apply(e82133F58(base, regs.ToArgs())); }
// sub_821351F0's original.
Result e821351F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821351F0).
inline void c821351F0(Regs& regs, uint8_t* base) { regs.Apply(e821351F0(base, regs.ToArgs())); }
// sub_82135B50's original.
Result e82135B50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82135B50).
inline void c82135B50(Regs& regs, uint8_t* base) { regs.Apply(e82135B50(base, regs.ToArgs())); }
// sub_821363F0's original.
Result e821363F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821363F0).
inline void c821363F0(Regs& regs, uint8_t* base) { regs.Apply(e821363F0(base, regs.ToArgs())); }
// sub_821379D8's original.
Result e821379D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821379D8).
inline void c821379D8(Regs& regs, uint8_t* base) { regs.Apply(e821379D8(base, regs.ToArgs())); }
// sub_82138570's original.
Result e82138570(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82138570).
inline void c82138570(Regs& regs, uint8_t* base) { regs.Apply(e82138570(base, regs.ToArgs())); }
// sub_821385C0's original.
Result e821385C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821385C0).
inline void c821385C0(Regs& regs, uint8_t* base) { regs.Apply(e821385C0(base, regs.ToArgs())); }
// sub_821389C8's original.
Result e821389C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821389C8).
inline void c821389C8(Regs& regs, uint8_t* base) { regs.Apply(e821389C8(base, regs.ToArgs())); }
// sub_82138A18's original.
Result e82138A18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82138A18).
inline void c82138A18(Regs& regs, uint8_t* base) { regs.Apply(e82138A18(base, regs.ToArgs())); }
// sub_82138C38's original.
Result e82138C38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82138C38).
inline void c82138C38(Regs& regs, uint8_t* base) { regs.Apply(e82138C38(base, regs.ToArgs())); }
// sub_82139628's original.
Result e82139628(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82139628).
inline void c82139628(Regs& regs, uint8_t* base) { regs.Apply(e82139628(base, regs.ToArgs())); }
// sub_821399A8's original.
Result e821399A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821399A8).
inline void c821399A8(Regs& regs, uint8_t* base) { regs.Apply(e821399A8(base, regs.ToArgs())); }
// sub_8213ADF8's original.
Result e8213ADF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8213ADF8).
inline void c8213ADF8(Regs& regs, uint8_t* base) { regs.Apply(e8213ADF8(base, regs.ToArgs())); }
// sub_8213B1E0's original.
Result e8213B1E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8213B1E0).
inline void c8213B1E0(Regs& regs, uint8_t* base) { regs.Apply(e8213B1E0(base, regs.ToArgs())); }
// sub_8213B2B8's original.
Result e8213B2B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8213B2B8).
inline void c8213B2B8(Regs& regs, uint8_t* base) { regs.Apply(e8213B2B8(base, regs.ToArgs())); }
// sub_82144A50's original.
Result e82144A50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82144A50).
inline void c82144A50(Regs& regs, uint8_t* base) { regs.Apply(e82144A50(base, regs.ToArgs())); }
// sub_82144DF0's original.
Result e82144DF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82144DF0).
inline void c82144DF0(Regs& regs, uint8_t* base) { regs.Apply(e82144DF0(base, regs.ToArgs())); }
// sub_82144E58's original.
Result e82144E58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82144E58).
inline void c82144E58(Regs& regs, uint8_t* base) { regs.Apply(e82144E58(base, regs.ToArgs())); }
// sub_82146BD0's original.
Result e82146BD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82146BD0).
inline void c82146BD0(Regs& regs, uint8_t* base) { regs.Apply(e82146BD0(base, regs.ToArgs())); }
// sub_82147D88's original.
Result e82147D88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82147D88).
inline void c82147D88(Regs& regs, uint8_t* base) { regs.Apply(e82147D88(base, regs.ToArgs())); }
// sub_82148058's original.
Result e82148058(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82148058).
inline void c82148058(Regs& regs, uint8_t* base) { regs.Apply(e82148058(base, regs.ToArgs())); }
// sub_82148428's original.
Result e82148428(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82148428).
inline void c82148428(Regs& regs, uint8_t* base) { regs.Apply(e82148428(base, regs.ToArgs())); }
// sub_82148528's original.
Result e82148528(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82148528).
inline void c82148528(Regs& regs, uint8_t* base) { regs.Apply(e82148528(base, regs.ToArgs())); }
// sub_8214AA70's original.
Result e8214AA70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8214AA70).
inline void c8214AA70(Regs& regs, uint8_t* base) { regs.Apply(e8214AA70(base, regs.ToArgs())); }
// sub_8214AB48's original.
Result e8214AB48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8214AB48).
inline void c8214AB48(Regs& regs, uint8_t* base) { regs.Apply(e8214AB48(base, regs.ToArgs())); }
// sub_8214E190's original.
Result e8214E190(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8214E190).
inline void c8214E190(Regs& regs, uint8_t* base) { regs.Apply(e8214E190(base, regs.ToArgs())); }
// sub_82150350's original.
Result e82150350(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82150350).
inline void c82150350(Regs& regs, uint8_t* base) { regs.Apply(e82150350(base, regs.ToArgs())); }
// sub_821507D8's original.
Result e821507D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821507D8).
inline void c821507D8(Regs& regs, uint8_t* base) { regs.Apply(e821507D8(base, regs.ToArgs())); }
// sub_82150878's original.
Result e82150878(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82150878).
inline void c82150878(Regs& regs, uint8_t* base) { regs.Apply(e82150878(base, regs.ToArgs())); }
// sub_82150A10's original.
Result e82150A10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82150A10).
inline void c82150A10(Regs& regs, uint8_t* base) { regs.Apply(e82150A10(base, regs.ToArgs())); }
// sub_82150B10's original.
Result e82150B10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82150B10).
inline void c82150B10(Regs& regs, uint8_t* base) { regs.Apply(e82150B10(base, regs.ToArgs())); }
// sub_82152028's original.
Result e82152028(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82152028).
inline void c82152028(Regs& regs, uint8_t* base) { regs.Apply(e82152028(base, regs.ToArgs())); }
// sub_82152108's original.
Result e82152108(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82152108).
inline void c82152108(Regs& regs, uint8_t* base) { regs.Apply(e82152108(base, regs.ToArgs())); }
// sub_82152548's original.
Result e82152548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82152548).
inline void c82152548(Regs& regs, uint8_t* base) { regs.Apply(e82152548(base, regs.ToArgs())); }
// sub_82152698's original.
Result e82152698(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82152698).
inline void c82152698(Regs& regs, uint8_t* base) { regs.Apply(e82152698(base, regs.ToArgs())); }
// sub_82152D38's original.
Result e82152D38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82152D38).
inline void c82152D38(Regs& regs, uint8_t* base) { regs.Apply(e82152D38(base, regs.ToArgs())); }
// sub_82152E08's original.
Result e82152E08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82152E08).
inline void c82152E08(Regs& regs, uint8_t* base) { regs.Apply(e82152E08(base, regs.ToArgs())); }
// sub_82153290's original.
Result e82153290(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82153290).
inline void c82153290(Regs& regs, uint8_t* base) { regs.Apply(e82153290(base, regs.ToArgs())); }
// sub_82153478's original.
Result e82153478(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82153478).
inline void c82153478(Regs& regs, uint8_t* base) { regs.Apply(e82153478(base, regs.ToArgs())); }
// sub_82153640's original.
Result e82153640(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82153640).
inline void c82153640(Regs& regs, uint8_t* base) { regs.Apply(e82153640(base, regs.ToArgs())); }
// sub_82153A50's original.
Result e82153A50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82153A50).
inline void c82153A50(Regs& regs, uint8_t* base) { regs.Apply(e82153A50(base, regs.ToArgs())); }
// sub_82153E58's original.
Result e82153E58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82153E58).
inline void c82153E58(Regs& regs, uint8_t* base) { regs.Apply(e82153E58(base, regs.ToArgs())); }
// sub_82154548's original.
Result e82154548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82154548).
inline void c82154548(Regs& regs, uint8_t* base) { regs.Apply(e82154548(base, regs.ToArgs())); }
// sub_82154808's original.
Result e82154808(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82154808).
inline void c82154808(Regs& regs, uint8_t* base) { regs.Apply(e82154808(base, regs.ToArgs())); }
// sub_82154A48's original.
Result e82154A48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82154A48).
inline void c82154A48(Regs& regs, uint8_t* base) { regs.Apply(e82154A48(base, regs.ToArgs())); }
// sub_82154D38's original.
Result e82154D38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82154D38).
inline void c82154D38(Regs& regs, uint8_t* base) { regs.Apply(e82154D38(base, regs.ToArgs())); }
// sub_82155288's original.
Result e82155288(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82155288).
inline void c82155288(Regs& regs, uint8_t* base) { regs.Apply(e82155288(base, regs.ToArgs())); }
// sub_82155E58's original.
Result e82155E58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82155E58).
inline void c82155E58(Regs& regs, uint8_t* base) { regs.Apply(e82155E58(base, regs.ToArgs())); }
// sub_821562F8's original.
Result e821562F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821562F8).
inline void c821562F8(Regs& regs, uint8_t* base) { regs.Apply(e821562F8(base, regs.ToArgs())); }
// sub_82156CC0's original.
Result e82156CC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82156CC0).
inline void c82156CC0(Regs& regs, uint8_t* base) { regs.Apply(e82156CC0(base, regs.ToArgs())); }
// sub_82158F70's original.
Result e82158F70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82158F70).
inline void c82158F70(Regs& regs, uint8_t* base) { regs.Apply(e82158F70(base, regs.ToArgs())); }
// sub_821593D8's original.
Result e821593D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821593D8).
inline void c821593D8(Regs& regs, uint8_t* base) { regs.Apply(e821593D8(base, regs.ToArgs())); }
// sub_821594B8's original.
Result e821594B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821594B8).
inline void c821594B8(Regs& regs, uint8_t* base) { regs.Apply(e821594B8(base, regs.ToArgs())); }
// sub_821597F0's original.
Result e821597F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821597F0).
inline void c821597F0(Regs& regs, uint8_t* base) { regs.Apply(e821597F0(base, regs.ToArgs())); }
// sub_8215B4F8's original.
Result e8215B4F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215B4F8).
inline void c8215B4F8(Regs& regs, uint8_t* base) { regs.Apply(e8215B4F8(base, regs.ToArgs())); }
// sub_8215B790's original.
Result e8215B790(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215B790).
inline void c8215B790(Regs& regs, uint8_t* base) { regs.Apply(e8215B790(base, regs.ToArgs())); }
// sub_8215B878's original.
Result e8215B878(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215B878).
inline void c8215B878(Regs& regs, uint8_t* base) { regs.Apply(e8215B878(base, regs.ToArgs())); }
// sub_8215D790's original.
Result e8215D790(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215D790).
inline void c8215D790(Regs& regs, uint8_t* base) { regs.Apply(e8215D790(base, regs.ToArgs())); }
// sub_8215D808's original.
Result e8215D808(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215D808).
inline void c8215D808(Regs& regs, uint8_t* base) { regs.Apply(e8215D808(base, regs.ToArgs())); }
// sub_8215F5E8's original.
Result e8215F5E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215F5E8).
inline void c8215F5E8(Regs& regs, uint8_t* base) { regs.Apply(e8215F5E8(base, regs.ToArgs())); }
// sub_8215F678's original.
Result e8215F678(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215F678).
inline void c8215F678(Regs& regs, uint8_t* base) { regs.Apply(e8215F678(base, regs.ToArgs())); }
// sub_8215F728's original.
Result e8215F728(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8215F728).
inline void c8215F728(Regs& regs, uint8_t* base) { regs.Apply(e8215F728(base, regs.ToArgs())); }
// sub_82162330's original.
Result e82162330(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82162330).
inline void c82162330(Regs& regs, uint8_t* base) { regs.Apply(e82162330(base, regs.ToArgs())); }
// sub_821635D0's original.
Result e821635D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821635D0).
inline void c821635D0(Regs& regs, uint8_t* base) { regs.Apply(e821635D0(base, regs.ToArgs())); }
// sub_82163920's original.
Result e82163920(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82163920).
inline void c82163920(Regs& regs, uint8_t* base) { regs.Apply(e82163920(base, regs.ToArgs())); }
// sub_82164090's original.
Result e82164090(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82164090).
inline void c82164090(Regs& regs, uint8_t* base) { regs.Apply(e82164090(base, regs.ToArgs())); }
// sub_82164458's original.
Result e82164458(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82164458).
inline void c82164458(Regs& regs, uint8_t* base) { regs.Apply(e82164458(base, regs.ToArgs())); }
// sub_821648E0's original.
Result e821648E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821648E0).
inline void c821648E0(Regs& regs, uint8_t* base) { regs.Apply(e821648E0(base, regs.ToArgs())); }
// sub_82164DC0's original.
Result e82164DC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82164DC0).
inline void c82164DC0(Regs& regs, uint8_t* base) { regs.Apply(e82164DC0(base, regs.ToArgs())); }
// sub_82167610's original.
Result e82167610(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82167610).
inline void c82167610(Regs& regs, uint8_t* base) { regs.Apply(e82167610(base, regs.ToArgs())); }
// sub_821692C8's original.
Result e821692C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821692C8).
inline void c821692C8(Regs& regs, uint8_t* base) { regs.Apply(e821692C8(base, regs.ToArgs())); }
// sub_821698B0's original.
Result e821698B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821698B0).
inline void c821698B0(Regs& regs, uint8_t* base) { regs.Apply(e821698B0(base, regs.ToArgs())); }
// sub_82169920's original.
Result e82169920(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82169920).
inline void c82169920(Regs& regs, uint8_t* base) { regs.Apply(e82169920(base, regs.ToArgs())); }
// sub_8216C208's original.
Result e8216C208(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C208).
inline void c8216C208(Regs& regs, uint8_t* base) { regs.Apply(e8216C208(base, regs.ToArgs())); }
// sub_8216C210's original.
Result e8216C210(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C210).
inline void c8216C210(Regs& regs, uint8_t* base) { regs.Apply(e8216C210(base, regs.ToArgs())); }
// sub_8216C2A0's original.
Result e8216C2A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C2A0).
inline void c8216C2A0(Regs& regs, uint8_t* base) { regs.Apply(e8216C2A0(base, regs.ToArgs())); }
// sub_8216C330's original.
Result e8216C330(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C330).
inline void c8216C330(Regs& regs, uint8_t* base) { regs.Apply(e8216C330(base, regs.ToArgs())); }
// sub_8216C510's original.
Result e8216C510(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C510).
inline void c8216C510(Regs& regs, uint8_t* base) { regs.Apply(e8216C510(base, regs.ToArgs())); }
// sub_8216C520's original.
Result e8216C520(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C520).
inline void c8216C520(Regs& regs, uint8_t* base) { regs.Apply(e8216C520(base, regs.ToArgs())); }
// sub_8216C530's original.
Result e8216C530(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C530).
inline void c8216C530(Regs& regs, uint8_t* base) { regs.Apply(e8216C530(base, regs.ToArgs())); }
// sub_8216C5B8's original.
Result e8216C5B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C5B8).
inline void c8216C5B8(Regs& regs, uint8_t* base) { regs.Apply(e8216C5B8(base, regs.ToArgs())); }
// sub_8216C658's original.
Result e8216C658(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C658).
inline void c8216C658(Regs& regs, uint8_t* base) { regs.Apply(e8216C658(base, regs.ToArgs())); }
// sub_8216C6D8's original.
Result e8216C6D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216C6D8).
inline void c8216C6D8(Regs& regs, uint8_t* base) { regs.Apply(e8216C6D8(base, regs.ToArgs())); }
// sub_8216CA28's original.
Result e8216CA28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216CA28).
inline void c8216CA28(Regs& regs, uint8_t* base) { regs.Apply(e8216CA28(base, regs.ToArgs())); }
// sub_8216CB98's original.
Result e8216CB98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216CB98).
inline void c8216CB98(Regs& regs, uint8_t* base) { regs.Apply(e8216CB98(base, regs.ToArgs())); }
// sub_8216CC60's original.
Result e8216CC60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216CC60).
inline void c8216CC60(Regs& regs, uint8_t* base) { regs.Apply(e8216CC60(base, regs.ToArgs())); }
// sub_8216CE00's original.
Result e8216CE00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216CE00).
inline void c8216CE00(Regs& regs, uint8_t* base) { regs.Apply(e8216CE00(base, regs.ToArgs())); }
// sub_8216CF08's original.
Result e8216CF08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216CF08).
inline void c8216CF08(Regs& regs, uint8_t* base) { regs.Apply(e8216CF08(base, regs.ToArgs())); }
// sub_8216CFE0's original.
Result e8216CFE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216CFE0).
inline void c8216CFE0(Regs& regs, uint8_t* base) { regs.Apply(e8216CFE0(base, regs.ToArgs())); }
// sub_8216D198's original.
Result e8216D198(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216D198).
inline void c8216D198(Regs& regs, uint8_t* base) { regs.Apply(e8216D198(base, regs.ToArgs())); }
// sub_8216D450's original.
Result e8216D450(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216D450).
inline void c8216D450(Regs& regs, uint8_t* base) { regs.Apply(e8216D450(base, regs.ToArgs())); }
// sub_8216D600's original.
Result e8216D600(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216D600).
inline void c8216D600(Regs& regs, uint8_t* base) { regs.Apply(e8216D600(base, regs.ToArgs())); }
// sub_8216D6D0's original.
Result e8216D6D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216D6D0).
inline void c8216D6D0(Regs& regs, uint8_t* base) { regs.Apply(e8216D6D0(base, regs.ToArgs())); }
// sub_8216D8C8's original.
Result e8216D8C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216D8C8).
inline void c8216D8C8(Regs& regs, uint8_t* base) { regs.Apply(e8216D8C8(base, regs.ToArgs())); }
// sub_8216D918's original.
Result e8216D918(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216D918).
inline void c8216D918(Regs& regs, uint8_t* base) { regs.Apply(e8216D918(base, regs.ToArgs())); }
// sub_8216DA40's original.
Result e8216DA40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216DA40).
inline void c8216DA40(Regs& regs, uint8_t* base) { regs.Apply(e8216DA40(base, regs.ToArgs())); }
// sub_8216DB70's original.
Result e8216DB70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216DB70).
inline void c8216DB70(Regs& regs, uint8_t* base) { regs.Apply(e8216DB70(base, regs.ToArgs())); }
// sub_8216DD88's original.
Result e8216DD88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216DD88).
inline void c8216DD88(Regs& regs, uint8_t* base) { regs.Apply(e8216DD88(base, regs.ToArgs())); }
// sub_8216DFD0's original.
Result e8216DFD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216DFD0).
inline void c8216DFD0(Regs& regs, uint8_t* base) { regs.Apply(e8216DFD0(base, regs.ToArgs())); }
// sub_8216E150's original.
Result e8216E150(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216E150).
inline void c8216E150(Regs& regs, uint8_t* base) { regs.Apply(e8216E150(base, regs.ToArgs())); }
// sub_8216E5B0's original.
Result e8216E5B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8216E5B0).
inline void c8216E5B0(Regs& regs, uint8_t* base) { regs.Apply(e8216E5B0(base, regs.ToArgs())); }
// sub_82170668's original.
Result e82170668(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82170668).
inline void c82170668(Regs& regs, uint8_t* base) { regs.Apply(e82170668(base, regs.ToArgs())); }
// sub_821706E8's original.
Result e821706E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821706E8).
inline void c821706E8(Regs& regs, uint8_t* base) { regs.Apply(e821706E8(base, regs.ToArgs())); }
// sub_82171250's original.
Result e82171250(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82171250).
inline void c82171250(Regs& regs, uint8_t* base) { regs.Apply(e82171250(base, regs.ToArgs())); }
// sub_821715B8's original.
Result e821715B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821715B8).
inline void c821715B8(Regs& regs, uint8_t* base) { regs.Apply(e821715B8(base, regs.ToArgs())); }
// sub_82171718's original.
Result e82171718(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82171718).
inline void c82171718(Regs& regs, uint8_t* base) { regs.Apply(e82171718(base, regs.ToArgs())); }
// sub_82173A90's original.
Result e82173A90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82173A90).
inline void c82173A90(Regs& regs, uint8_t* base) { regs.Apply(e82173A90(base, regs.ToArgs())); }
// sub_821743A8's original.
Result e821743A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821743A8).
inline void c821743A8(Regs& regs, uint8_t* base) { regs.Apply(e821743A8(base, regs.ToArgs())); }
// sub_82178B40's original.
Result e82178B40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178B40).
inline void c82178B40(Regs& regs, uint8_t* base) { regs.Apply(e82178B40(base, regs.ToArgs())); }
// sub_82178B98's original.
Result e82178B98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178B98).
inline void c82178B98(Regs& regs, uint8_t* base) { regs.Apply(e82178B98(base, regs.ToArgs())); }
// sub_82178C40's original.
Result e82178C40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178C40).
inline void c82178C40(Regs& regs, uint8_t* base) { regs.Apply(e82178C40(base, regs.ToArgs())); }
// sub_82178DF8's original.
Result e82178DF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178DF8).
inline void c82178DF8(Regs& regs, uint8_t* base) { regs.Apply(e82178DF8(base, regs.ToArgs())); }
// sub_82178E60's original.
Result e82178E60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178E60).
inline void c82178E60(Regs& regs, uint8_t* base) { regs.Apply(e82178E60(base, regs.ToArgs())); }
// sub_82178EC0's original.
Result e82178EC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178EC0).
inline void c82178EC0(Regs& regs, uint8_t* base) { regs.Apply(e82178EC0(base, regs.ToArgs())); }
// sub_82178F00's original.
Result e82178F00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178F00).
inline void c82178F00(Regs& regs, uint8_t* base) { regs.Apply(e82178F00(base, regs.ToArgs())); }
// sub_82178FE8's original.
Result e82178FE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82178FE8).
inline void c82178FE8(Regs& regs, uint8_t* base) { regs.Apply(e82178FE8(base, regs.ToArgs())); }
// sub_82179050's original.
Result e82179050(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179050).
inline void c82179050(Regs& regs, uint8_t* base) { regs.Apply(e82179050(base, regs.ToArgs())); }
// sub_82179130's original.
Result e82179130(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179130).
inline void c82179130(Regs& regs, uint8_t* base) { regs.Apply(e82179130(base, regs.ToArgs())); }
// sub_821791C0's original.
Result e821791C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821791C0).
inline void c821791C0(Regs& regs, uint8_t* base) { regs.Apply(e821791C0(base, regs.ToArgs())); }
// sub_82179250's original.
Result e82179250(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179250).
inline void c82179250(Regs& regs, uint8_t* base) { regs.Apply(e82179250(base, regs.ToArgs())); }
// sub_82179310's original.
Result e82179310(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179310).
inline void c82179310(Regs& regs, uint8_t* base) { regs.Apply(e82179310(base, regs.ToArgs())); }
// sub_82179378's original.
Result e82179378(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179378).
inline void c82179378(Regs& regs, uint8_t* base) { regs.Apply(e82179378(base, regs.ToArgs())); }
// sub_82179420's original.
Result e82179420(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179420).
inline void c82179420(Regs& regs, uint8_t* base) { regs.Apply(e82179420(base, regs.ToArgs())); }
// sub_821794A8's original.
Result e821794A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821794A8).
inline void c821794A8(Regs& regs, uint8_t* base) { regs.Apply(e821794A8(base, regs.ToArgs())); }
// sub_821794F8's original.
Result e821794F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821794F8).
inline void c821794F8(Regs& regs, uint8_t* base) { regs.Apply(e821794F8(base, regs.ToArgs())); }
// sub_82179548's original.
Result e82179548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179548).
inline void c82179548(Regs& regs, uint8_t* base) { regs.Apply(e82179548(base, regs.ToArgs())); }
// sub_82179598's original.
Result e82179598(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179598).
inline void c82179598(Regs& regs, uint8_t* base) { regs.Apply(e82179598(base, regs.ToArgs())); }
// sub_821795E0's original.
Result e821795E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821795E0).
inline void c821795E0(Regs& regs, uint8_t* base) { regs.Apply(e821795E0(base, regs.ToArgs())); }
// sub_82179678's original.
Result e82179678(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179678).
inline void c82179678(Regs& regs, uint8_t* base) { regs.Apply(e82179678(base, regs.ToArgs())); }
// sub_821797A8's original.
Result e821797A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821797A8).
inline void c821797A8(Regs& regs, uint8_t* base) { regs.Apply(e821797A8(base, regs.ToArgs())); }
// sub_82179818's original.
Result e82179818(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179818).
inline void c82179818(Regs& regs, uint8_t* base) { regs.Apply(e82179818(base, regs.ToArgs())); }
// sub_821798D8's original.
Result e821798D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821798D8).
inline void c821798D8(Regs& regs, uint8_t* base) { regs.Apply(e821798D8(base, regs.ToArgs())); }
// sub_82179A18's original.
Result e82179A18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179A18).
inline void c82179A18(Regs& regs, uint8_t* base) { regs.Apply(e82179A18(base, regs.ToArgs())); }
// sub_82179A60's original.
Result e82179A60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179A60).
inline void c82179A60(Regs& regs, uint8_t* base) { regs.Apply(e82179A60(base, regs.ToArgs())); }
// sub_82179BE0's original.
Result e82179BE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179BE0).
inline void c82179BE0(Regs& regs, uint8_t* base) { regs.Apply(e82179BE0(base, regs.ToArgs())); }
// sub_82179DF0's original.
Result e82179DF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179DF0).
inline void c82179DF0(Regs& regs, uint8_t* base) { regs.Apply(e82179DF0(base, regs.ToArgs())); }
// sub_82179EA8's original.
Result e82179EA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82179EA8).
inline void c82179EA8(Regs& regs, uint8_t* base) { regs.Apply(e82179EA8(base, regs.ToArgs())); }
// sub_8217A370's original.
Result e8217A370(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217A370).
inline void c8217A370(Regs& regs, uint8_t* base) { regs.Apply(e8217A370(base, regs.ToArgs())); }
// sub_8217AEA8's original.
Result e8217AEA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217AEA8).
inline void c8217AEA8(Regs& regs, uint8_t* base) { regs.Apply(e8217AEA8(base, regs.ToArgs())); }
// sub_8217AFE8's original.
Result e8217AFE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217AFE8).
inline void c8217AFE8(Regs& regs, uint8_t* base) { regs.Apply(e8217AFE8(base, regs.ToArgs())); }
// sub_8217BA20's original.
Result e8217BA20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217BA20).
inline void c8217BA20(Regs& regs, uint8_t* base) { regs.Apply(e8217BA20(base, regs.ToArgs())); }
// sub_8217BE98's original.
Result e8217BE98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217BE98).
inline void c8217BE98(Regs& regs, uint8_t* base) { regs.Apply(e8217BE98(base, regs.ToArgs())); }
// sub_8217C368's original.
Result e8217C368(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217C368).
inline void c8217C368(Regs& regs, uint8_t* base) { regs.Apply(e8217C368(base, regs.ToArgs())); }
// sub_8217C370's original.
Result e8217C370(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217C370).
inline void c8217C370(Regs& regs, uint8_t* base) { regs.Apply(e8217C370(base, regs.ToArgs())); }
// sub_8217C708's original.
Result e8217C708(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217C708).
inline void c8217C708(Regs& regs, uint8_t* base) { regs.Apply(e8217C708(base, regs.ToArgs())); }
// sub_8217CAB8's original.
Result e8217CAB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217CAB8).
inline void c8217CAB8(Regs& regs, uint8_t* base) { regs.Apply(e8217CAB8(base, regs.ToArgs())); }
// sub_8217D120's original.
Result e8217D120(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217D120).
inline void c8217D120(Regs& regs, uint8_t* base) { regs.Apply(e8217D120(base, regs.ToArgs())); }
// sub_8217D2B0's original.
Result e8217D2B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217D2B0).
inline void c8217D2B0(Regs& regs, uint8_t* base) { regs.Apply(e8217D2B0(base, regs.ToArgs())); }
// sub_8217DBC0's original.
Result e8217DBC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217DBC0).
inline void c8217DBC0(Regs& regs, uint8_t* base) { regs.Apply(e8217DBC0(base, regs.ToArgs())); }
// sub_8217DD38's original.
Result e8217DD38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217DD38).
inline void c8217DD38(Regs& regs, uint8_t* base) { regs.Apply(e8217DD38(base, regs.ToArgs())); }
// sub_8217F880's original.
Result e8217F880(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217F880).
inline void c8217F880(Regs& regs, uint8_t* base) { regs.Apply(e8217F880(base, regs.ToArgs())); }
// sub_8217F9E0's original.
Result e8217F9E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217F9E0).
inline void c8217F9E0(Regs& regs, uint8_t* base) { regs.Apply(e8217F9E0(base, regs.ToArgs())); }
// sub_8217FBE8's original.
Result e8217FBE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217FBE8).
inline void c8217FBE8(Regs& regs, uint8_t* base) { regs.Apply(e8217FBE8(base, regs.ToArgs())); }
// sub_8217FC60's original.
Result e8217FC60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217FC60).
inline void c8217FC60(Regs& regs, uint8_t* base) { regs.Apply(e8217FC60(base, regs.ToArgs())); }
// sub_8217FCB0's original.
Result e8217FCB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217FCB0).
inline void c8217FCB0(Regs& regs, uint8_t* base) { regs.Apply(e8217FCB0(base, regs.ToArgs())); }
// sub_8217FD58's original.
Result e8217FD58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217FD58).
inline void c8217FD58(Regs& regs, uint8_t* base) { regs.Apply(e8217FD58(base, regs.ToArgs())); }
// sub_8217FDC8's original.
Result e8217FDC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217FDC8).
inline void c8217FDC8(Regs& regs, uint8_t* base) { regs.Apply(e8217FDC8(base, regs.ToArgs())); }
// sub_8217FE60's original.
Result e8217FE60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8217FE60).
inline void c8217FE60(Regs& regs, uint8_t* base) { regs.Apply(e8217FE60(base, regs.ToArgs())); }
// sub_82180108's original.
Result e82180108(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82180108).
inline void c82180108(Regs& regs, uint8_t* base) { regs.Apply(e82180108(base, regs.ToArgs())); }
// sub_82180738's original.
Result e82180738(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82180738).
inline void c82180738(Regs& regs, uint8_t* base) { regs.Apply(e82180738(base, regs.ToArgs())); }
// sub_82180B00's original.
Result e82180B00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82180B00).
inline void c82180B00(Regs& regs, uint8_t* base) { regs.Apply(e82180B00(base, regs.ToArgs())); }
// sub_82180CE8's original.
Result e82180CE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82180CE8).
inline void c82180CE8(Regs& regs, uint8_t* base) { regs.Apply(e82180CE8(base, regs.ToArgs())); }
// sub_82182C90's original.
Result e82182C90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82182C90).
inline void c82182C90(Regs& regs, uint8_t* base) { regs.Apply(e82182C90(base, regs.ToArgs())); }
// sub_82182D30's original.
Result e82182D30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82182D30).
inline void c82182D30(Regs& regs, uint8_t* base) { regs.Apply(e82182D30(base, regs.ToArgs())); }
// sub_82196EC0's original.
Result e82196EC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82196EC0).
inline void c82196EC0(Regs& regs, uint8_t* base) { regs.Apply(e82196EC0(base, regs.ToArgs())); }
// sub_8219ED10's original.
Result e8219ED10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8219ED10).
inline void c8219ED10(Regs& regs, uint8_t* base) { regs.Apply(e8219ED10(base, regs.ToArgs())); }
// sub_8219EDB0's original.
Result e8219EDB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8219EDB0).
inline void c8219EDB0(Regs& regs, uint8_t* base) { regs.Apply(e8219EDB0(base, regs.ToArgs())); }
// sub_821B9E70's original.
Result e821B9E70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821B9E70).
inline void c821B9E70(Regs& regs, uint8_t* base) { regs.Apply(e821B9E70(base, regs.ToArgs())); }
// sub_821BB120's original.
Result e821BB120(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821BB120).
inline void c821BB120(Regs& regs, uint8_t* base) { regs.Apply(e821BB120(base, regs.ToArgs())); }
// sub_821BB1F8's original.
Result e821BB1F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821BB1F8).
inline void c821BB1F8(Regs& regs, uint8_t* base) { regs.Apply(e821BB1F8(base, regs.ToArgs())); }
// sub_821BB9A8's original.
Result e821BB9A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821BB9A8).
inline void c821BB9A8(Regs& regs, uint8_t* base) { regs.Apply(e821BB9A8(base, regs.ToArgs())); }
// sub_821BE5A8's original.
Result e821BE5A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821BE5A8).
inline void c821BE5A8(Regs& regs, uint8_t* base) { regs.Apply(e821BE5A8(base, regs.ToArgs())); }
// sub_821CC890's original.
Result e821CC890(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821CC890).
inline void c821CC890(Regs& regs, uint8_t* base) { regs.Apply(e821CC890(base, regs.ToArgs())); }
// sub_821CCA38's original.
Result e821CCA38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821CCA38).
inline void c821CCA38(Regs& regs, uint8_t* base) { regs.Apply(e821CCA38(base, regs.ToArgs())); }
// sub_821DF2F8's original.
Result e821DF2F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821DF2F8).
inline void c821DF2F8(Regs& regs, uint8_t* base) { regs.Apply(e821DF2F8(base, regs.ToArgs())); }
// sub_821DFA78's original.
Result e821DFA78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821DFA78).
inline void c821DFA78(Regs& regs, uint8_t* base) { regs.Apply(e821DFA78(base, regs.ToArgs())); }
// sub_821E7368's original.
Result e821E7368(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821E7368).
inline void c821E7368(Regs& regs, uint8_t* base) { regs.Apply(e821E7368(base, regs.ToArgs())); }
// sub_821FE628's original.
Result e821FE628(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_821FE628).
inline void c821FE628(Regs& regs, uint8_t* base) { regs.Apply(e821FE628(base, regs.ToArgs())); }
// sub_82208738's original.
Result e82208738(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82208738).
inline void c82208738(Regs& regs, uint8_t* base) { regs.Apply(e82208738(base, regs.ToArgs())); }
// sub_8220BA68's original.
Result e8220BA68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220BA68).
inline void c8220BA68(Regs& regs, uint8_t* base) { regs.Apply(e8220BA68(base, regs.ToArgs())); }
// sub_8220BAC0's original.
Result e8220BAC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220BAC0).
inline void c8220BAC0(Regs& regs, uint8_t* base) { regs.Apply(e8220BAC0(base, regs.ToArgs())); }
// sub_8220BBB0's original.
Result e8220BBB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220BBB0).
inline void c8220BBB0(Regs& regs, uint8_t* base) { regs.Apply(e8220BBB0(base, regs.ToArgs())); }
// sub_8220BC28's original.
Result e8220BC28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220BC28).
inline void c8220BC28(Regs& regs, uint8_t* base) { regs.Apply(e8220BC28(base, regs.ToArgs())); }
// sub_8220C1E0's original.
Result e8220C1E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220C1E0).
inline void c8220C1E0(Regs& regs, uint8_t* base) { regs.Apply(e8220C1E0(base, regs.ToArgs())); }
// sub_8220C4C8's original.
Result e8220C4C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220C4C8).
inline void c8220C4C8(Regs& regs, uint8_t* base) { regs.Apply(e8220C4C8(base, regs.ToArgs())); }
// sub_8220D040's original.
Result e8220D040(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220D040).
inline void c8220D040(Regs& regs, uint8_t* base) { regs.Apply(e8220D040(base, regs.ToArgs())); }
// sub_8220D7E8's original.
Result e8220D7E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220D7E8).
inline void c8220D7E8(Regs& regs, uint8_t* base) { regs.Apply(e8220D7E8(base, regs.ToArgs())); }
// sub_8220FC98's original.
Result e8220FC98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8220FC98).
inline void c8220FC98(Regs& regs, uint8_t* base) { regs.Apply(e8220FC98(base, regs.ToArgs())); }
// sub_82210868's original.
Result e82210868(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82210868).
inline void c82210868(Regs& regs, uint8_t* base) { regs.Apply(e82210868(base, regs.ToArgs())); }
// sub_82211A28's original.
Result e82211A28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82211A28).
inline void c82211A28(Regs& regs, uint8_t* base) { regs.Apply(e82211A28(base, regs.ToArgs())); }
// sub_82211AB0's original.
Result e82211AB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82211AB0).
inline void c82211AB0(Regs& regs, uint8_t* base) { regs.Apply(e82211AB0(base, regs.ToArgs())); }
// sub_82211B98's original.
Result e82211B98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82211B98).
inline void c82211B98(Regs& regs, uint8_t* base) { regs.Apply(e82211B98(base, regs.ToArgs())); }
// sub_82211C90's original.
Result e82211C90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82211C90).
inline void c82211C90(Regs& regs, uint8_t* base) { regs.Apply(e82211C90(base, regs.ToArgs())); }
// sub_82211D18's original.
Result e82211D18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82211D18).
inline void c82211D18(Regs& regs, uint8_t* base) { regs.Apply(e82211D18(base, regs.ToArgs())); }
// sub_82211F40's original.
Result e82211F40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82211F40).
inline void c82211F40(Regs& regs, uint8_t* base) { regs.Apply(e82211F40(base, regs.ToArgs())); }
// sub_82216BA8's original.
Result e82216BA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82216BA8).
inline void c82216BA8(Regs& regs, uint8_t* base) { regs.Apply(e82216BA8(base, regs.ToArgs())); }
// sub_82217CD0's original.
Result e82217CD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82217CD0).
inline void c82217CD0(Regs& regs, uint8_t* base) { regs.Apply(e82217CD0(base, regs.ToArgs())); }
// sub_82217D70's original.
Result e82217D70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82217D70).
inline void c82217D70(Regs& regs, uint8_t* base) { regs.Apply(e82217D70(base, regs.ToArgs())); }
// sub_82218C10's original.
Result e82218C10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82218C10).
inline void c82218C10(Regs& regs, uint8_t* base) { regs.Apply(e82218C10(base, regs.ToArgs())); }
// sub_82219138's original.
Result e82219138(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82219138).
inline void c82219138(Regs& regs, uint8_t* base) { regs.Apply(e82219138(base, regs.ToArgs())); }
// sub_82219170's original.
Result e82219170(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82219170).
inline void c82219170(Regs& regs, uint8_t* base) { regs.Apply(e82219170(base, regs.ToArgs())); }
// sub_8221A3E8's original.
Result e8221A3E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221A3E8).
inline void c8221A3E8(Regs& regs, uint8_t* base) { regs.Apply(e8221A3E8(base, regs.ToArgs())); }
// sub_8221A498's original.
Result e8221A498(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221A498).
inline void c8221A498(Regs& regs, uint8_t* base) { regs.Apply(e8221A498(base, regs.ToArgs())); }
// sub_8221B1F0's original.
Result e8221B1F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221B1F0).
inline void c8221B1F0(Regs& regs, uint8_t* base) { regs.Apply(e8221B1F0(base, regs.ToArgs())); }
// sub_8221BDD0's original.
Result e8221BDD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221BDD0).
inline void c8221BDD0(Regs& regs, uint8_t* base) { regs.Apply(e8221BDD0(base, regs.ToArgs())); }
// sub_8221BE30's original.
Result e8221BE30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221BE30).
inline void c8221BE30(Regs& regs, uint8_t* base) { regs.Apply(e8221BE30(base, regs.ToArgs())); }
// sub_8221F390's original.
Result e8221F390(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221F390).
inline void c8221F390(Regs& regs, uint8_t* base) { regs.Apply(e8221F390(base, regs.ToArgs())); }
// sub_8221F3C8's original.
Result e8221F3C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221F3C8).
inline void c8221F3C8(Regs& regs, uint8_t* base) { regs.Apply(e8221F3C8(base, regs.ToArgs())); }
// sub_8221FA30's original.
Result e8221FA30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221FA30).
inline void c8221FA30(Regs& regs, uint8_t* base) { regs.Apply(e8221FA30(base, regs.ToArgs())); }
// sub_8221FA68's original.
Result e8221FA68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8221FA68).
inline void c8221FA68(Regs& regs, uint8_t* base) { regs.Apply(e8221FA68(base, regs.ToArgs())); }
// sub_82221D00's original.
Result e82221D00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82221D00).
inline void c82221D00(Regs& regs, uint8_t* base) { regs.Apply(e82221D00(base, regs.ToArgs())); }
// sub_822221A0's original.
Result e822221A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822221A0).
inline void c822221A0(Regs& regs, uint8_t* base) { regs.Apply(e822221A0(base, regs.ToArgs())); }
// sub_82224AB8's original.
Result e82224AB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82224AB8).
inline void c82224AB8(Regs& regs, uint8_t* base) { regs.Apply(e82224AB8(base, regs.ToArgs())); }
// sub_822271F8's original.
Result e822271F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822271F8).
inline void c822271F8(Regs& regs, uint8_t* base) { regs.Apply(e822271F8(base, regs.ToArgs())); }
// sub_82227708's original.
Result e82227708(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82227708).
inline void c82227708(Regs& regs, uint8_t* base) { regs.Apply(e82227708(base, regs.ToArgs())); }
// sub_82227818's original.
Result e82227818(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82227818).
inline void c82227818(Regs& regs, uint8_t* base) { regs.Apply(e82227818(base, regs.ToArgs())); }
// sub_822279A8's original.
Result e822279A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822279A8).
inline void c822279A8(Regs& regs, uint8_t* base) { regs.Apply(e822279A8(base, regs.ToArgs())); }
// sub_82228588's original.
Result e82228588(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82228588).
inline void c82228588(Regs& regs, uint8_t* base) { regs.Apply(e82228588(base, regs.ToArgs())); }
// sub_82229238's original.
Result e82229238(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82229238).
inline void c82229238(Regs& regs, uint8_t* base) { regs.Apply(e82229238(base, regs.ToArgs())); }
// sub_8222A470's original.
Result e8222A470(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222A470).
inline void c8222A470(Regs& regs, uint8_t* base) { regs.Apply(e8222A470(base, regs.ToArgs())); }
// sub_8222A718's original.
Result e8222A718(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222A718).
inline void c8222A718(Regs& regs, uint8_t* base) { regs.Apply(e8222A718(base, regs.ToArgs())); }
// sub_8222A978's original.
Result e8222A978(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222A978).
inline void c8222A978(Regs& regs, uint8_t* base) { regs.Apply(e8222A978(base, regs.ToArgs())); }
// sub_8222AE00's original.
Result e8222AE00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222AE00).
inline void c8222AE00(Regs& regs, uint8_t* base) { regs.Apply(e8222AE00(base, regs.ToArgs())); }
// sub_8222B048's original.
Result e8222B048(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222B048).
inline void c8222B048(Regs& regs, uint8_t* base) { regs.Apply(e8222B048(base, regs.ToArgs())); }
// sub_8222B438's original.
Result e8222B438(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222B438).
inline void c8222B438(Regs& regs, uint8_t* base) { regs.Apply(e8222B438(base, regs.ToArgs())); }
// sub_8222B8A8's original.
Result e8222B8A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222B8A8).
inline void c8222B8A8(Regs& regs, uint8_t* base) { regs.Apply(e8222B8A8(base, regs.ToArgs())); }
// sub_8222BBD0's original.
Result e8222BBD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222BBD0).
inline void c8222BBD0(Regs& regs, uint8_t* base) { regs.Apply(e8222BBD0(base, regs.ToArgs())); }
// sub_8222C188's original.
Result e8222C188(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222C188).
inline void c8222C188(Regs& regs, uint8_t* base) { regs.Apply(e8222C188(base, regs.ToArgs())); }
// sub_8222CD60's original.
Result e8222CD60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222CD60).
inline void c8222CD60(Regs& regs, uint8_t* base) { regs.Apply(e8222CD60(base, regs.ToArgs())); }
// sub_8222CFF0's original.
Result e8222CFF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222CFF0).
inline void c8222CFF0(Regs& regs, uint8_t* base) { regs.Apply(e8222CFF0(base, regs.ToArgs())); }
// sub_8222D108's original.
Result e8222D108(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222D108).
inline void c8222D108(Regs& regs, uint8_t* base) { regs.Apply(e8222D108(base, regs.ToArgs())); }
// sub_8222D1A8's original.
Result e8222D1A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222D1A8).
inline void c8222D1A8(Regs& regs, uint8_t* base) { regs.Apply(e8222D1A8(base, regs.ToArgs())); }
// sub_8222D2E8's original.
Result e8222D2E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222D2E8).
inline void c8222D2E8(Regs& regs, uint8_t* base) { regs.Apply(e8222D2E8(base, regs.ToArgs())); }
// sub_8222D610's original.
Result e8222D610(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222D610).
inline void c8222D610(Regs& regs, uint8_t* base) { regs.Apply(e8222D610(base, regs.ToArgs())); }
// sub_8222D828's original.
Result e8222D828(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222D828).
inline void c8222D828(Regs& regs, uint8_t* base) { regs.Apply(e8222D828(base, regs.ToArgs())); }
// sub_8222DA10's original.
Result e8222DA10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222DA10).
inline void c8222DA10(Regs& regs, uint8_t* base) { regs.Apply(e8222DA10(base, regs.ToArgs())); }
// sub_8222DC08's original.
Result e8222DC08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222DC08).
inline void c8222DC08(Regs& regs, uint8_t* base) { regs.Apply(e8222DC08(base, regs.ToArgs())); }
// sub_8222DEE0's original.
Result e8222DEE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222DEE0).
inline void c8222DEE0(Regs& regs, uint8_t* base) { regs.Apply(e8222DEE0(base, regs.ToArgs())); }
// sub_8222E6E0's original.
Result e8222E6E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222E6E0).
inline void c8222E6E0(Regs& regs, uint8_t* base) { regs.Apply(e8222E6E0(base, regs.ToArgs())); }
// sub_8222E8D8's original.
Result e8222E8D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222E8D8).
inline void c8222E8D8(Regs& regs, uint8_t* base) { regs.Apply(e8222E8D8(base, regs.ToArgs())); }
// sub_8222EAD0's original.
Result e8222EAD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222EAD0).
inline void c8222EAD0(Regs& regs, uint8_t* base) { regs.Apply(e8222EAD0(base, regs.ToArgs())); }
// sub_8222F0B0's original.
Result e8222F0B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222F0B0).
inline void c8222F0B0(Regs& regs, uint8_t* base) { regs.Apply(e8222F0B0(base, regs.ToArgs())); }
// sub_8222FB90's original.
Result e8222FB90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8222FB90).
inline void c8222FB90(Regs& regs, uint8_t* base) { regs.Apply(e8222FB90(base, regs.ToArgs())); }
// sub_82233870's original.
Result e82233870(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82233870).
inline void c82233870(Regs& regs, uint8_t* base) { regs.Apply(e82233870(base, regs.ToArgs())); }
// sub_822339F0's original.
Result e822339F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822339F0).
inline void c822339F0(Regs& regs, uint8_t* base) { regs.Apply(e822339F0(base, regs.ToArgs())); }
// sub_82233C80's original.
Result e82233C80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82233C80).
inline void c82233C80(Regs& regs, uint8_t* base) { regs.Apply(e82233C80(base, regs.ToArgs())); }
// sub_822340B0's original.
Result e822340B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822340B0).
inline void c822340B0(Regs& regs, uint8_t* base) { regs.Apply(e822340B0(base, regs.ToArgs())); }
// sub_82234C20's original.
Result e82234C20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82234C20).
inline void c82234C20(Regs& regs, uint8_t* base) { regs.Apply(e82234C20(base, regs.ToArgs())); }
// sub_82234D50's original.
Result e82234D50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82234D50).
inline void c82234D50(Regs& regs, uint8_t* base) { regs.Apply(e82234D50(base, regs.ToArgs())); }
// sub_82234E38's original.
Result e82234E38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82234E38).
inline void c82234E38(Regs& regs, uint8_t* base) { regs.Apply(e82234E38(base, regs.ToArgs())); }
// sub_822366F0's original.
Result e822366F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822366F0).
inline void c822366F0(Regs& regs, uint8_t* base) { regs.Apply(e822366F0(base, regs.ToArgs())); }
// sub_82236880's original.
Result e82236880(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82236880).
inline void c82236880(Regs& regs, uint8_t* base) { regs.Apply(e82236880(base, regs.ToArgs())); }
// sub_82243E08's original.
Result e82243E08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82243E08).
inline void c82243E08(Regs& regs, uint8_t* base) { regs.Apply(e82243E08(base, regs.ToArgs())); }
// sub_82243F58's original.
Result e82243F58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82243F58).
inline void c82243F58(Regs& regs, uint8_t* base) { regs.Apply(e82243F58(base, regs.ToArgs())); }
// sub_822441D8's original.
Result e822441D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822441D8).
inline void c822441D8(Regs& regs, uint8_t* base) { regs.Apply(e822441D8(base, regs.ToArgs())); }
// sub_82244E70's original.
Result e82244E70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82244E70).
inline void c82244E70(Regs& regs, uint8_t* base) { regs.Apply(e82244E70(base, regs.ToArgs())); }
// sub_82244FC0's original.
Result e82244FC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82244FC0).
inline void c82244FC0(Regs& regs, uint8_t* base) { regs.Apply(e82244FC0(base, regs.ToArgs())); }
// sub_822450A8's original.
Result e822450A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822450A8).
inline void c822450A8(Regs& regs, uint8_t* base) { regs.Apply(e822450A8(base, regs.ToArgs())); }
// sub_82246038's original.
Result e82246038(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82246038).
inline void c82246038(Regs& regs, uint8_t* base) { regs.Apply(e82246038(base, regs.ToArgs())); }
// sub_82246B28's original.
Result e82246B28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82246B28).
inline void c82246B28(Regs& regs, uint8_t* base) { regs.Apply(e82246B28(base, regs.ToArgs())); }
// sub_82246C90's original.
Result e82246C90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82246C90).
inline void c82246C90(Regs& regs, uint8_t* base) { regs.Apply(e82246C90(base, regs.ToArgs())); }
// sub_82247AA0's original.
Result e82247AA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82247AA0).
inline void c82247AA0(Regs& regs, uint8_t* base) { regs.Apply(e82247AA0(base, regs.ToArgs())); }
// sub_82247B98's original.
Result e82247B98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82247B98).
inline void c82247B98(Regs& regs, uint8_t* base) { regs.Apply(e82247B98(base, regs.ToArgs())); }
// sub_8224C450's original.
Result e8224C450(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8224C450).
inline void c8224C450(Regs& regs, uint8_t* base) { regs.Apply(e8224C450(base, regs.ToArgs())); }
// sub_8224CF18's original.
Result e8224CF18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8224CF18).
inline void c8224CF18(Regs& regs, uint8_t* base) { regs.Apply(e8224CF18(base, regs.ToArgs())); }
// sub_8224CF20's original.
Result e8224CF20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8224CF20).
inline void c8224CF20(Regs& regs, uint8_t* base) { regs.Apply(e8224CF20(base, regs.ToArgs())); }
// sub_8224FB10's original.
Result e8224FB10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8224FB10).
inline void c8224FB10(Regs& regs, uint8_t* base) { regs.Apply(e8224FB10(base, regs.ToArgs())); }
// sub_82250670's original.
Result e82250670(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82250670).
inline void c82250670(Regs& regs, uint8_t* base) { regs.Apply(e82250670(base, regs.ToArgs())); }
// sub_82250700's original.
Result e82250700(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82250700).
inline void c82250700(Regs& regs, uint8_t* base) { regs.Apply(e82250700(base, regs.ToArgs())); }
// sub_8225D590's original.
Result e8225D590(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8225D590).
inline void c8225D590(Regs& regs, uint8_t* base) { regs.Apply(e8225D590(base, regs.ToArgs())); }
// sub_82277DB0's original.
Result e82277DB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82277DB0).
inline void c82277DB0(Regs& regs, uint8_t* base) { regs.Apply(e82277DB0(base, regs.ToArgs())); }
// sub_82288898's original.
Result e82288898(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82288898).
inline void c82288898(Regs& regs, uint8_t* base) { regs.Apply(e82288898(base, regs.ToArgs())); }
// sub_8228B500's original.
Result e8228B500(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228B500).
inline void c8228B500(Regs& regs, uint8_t* base) { regs.Apply(e8228B500(base, regs.ToArgs())); }
// sub_8228E788's original.
Result e8228E788(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228E788).
inline void c8228E788(Regs& regs, uint8_t* base) { regs.Apply(e8228E788(base, regs.ToArgs())); }
// sub_8228F5C0's original.
Result e8228F5C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F5C0).
inline void c8228F5C0(Regs& regs, uint8_t* base) { regs.Apply(e8228F5C0(base, regs.ToArgs())); }
// sub_8228F6B0's original.
Result e8228F6B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F6B0).
inline void c8228F6B0(Regs& regs, uint8_t* base) { regs.Apply(e8228F6B0(base, regs.ToArgs())); }
// sub_8228F780's original.
Result e8228F780(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F780).
inline void c8228F780(Regs& regs, uint8_t* base) { regs.Apply(e8228F780(base, regs.ToArgs())); }
// sub_8228F7E8's original.
Result e8228F7E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F7E8).
inline void c8228F7E8(Regs& regs, uint8_t* base) { regs.Apply(e8228F7E8(base, regs.ToArgs())); }
// sub_8228F840's original.
Result e8228F840(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F840).
inline void c8228F840(Regs& regs, uint8_t* base) { regs.Apply(e8228F840(base, regs.ToArgs())); }
// sub_8228F8F8's original.
Result e8228F8F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F8F8).
inline void c8228F8F8(Regs& regs, uint8_t* base) { regs.Apply(e8228F8F8(base, regs.ToArgs())); }
// sub_8228F9B0's original.
Result e8228F9B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F9B0).
inline void c8228F9B0(Regs& regs, uint8_t* base) { regs.Apply(e8228F9B0(base, regs.ToArgs())); }
// sub_8228F9E8's original.
Result e8228F9E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228F9E8).
inline void c8228F9E8(Regs& regs, uint8_t* base) { regs.Apply(e8228F9E8(base, regs.ToArgs())); }
// sub_8228FA90's original.
Result e8228FA90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228FA90).
inline void c8228FA90(Regs& regs, uint8_t* base) { regs.Apply(e8228FA90(base, regs.ToArgs())); }
// sub_8228FAC8's original.
Result e8228FAC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228FAC8).
inline void c8228FAC8(Regs& regs, uint8_t* base) { regs.Apply(e8228FAC8(base, regs.ToArgs())); }
// sub_8228FB00's original.
Result e8228FB00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228FB00).
inline void c8228FB00(Regs& regs, uint8_t* base) { regs.Apply(e8228FB00(base, regs.ToArgs())); }
// sub_8228FBC0's original.
Result e8228FBC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228FBC0).
inline void c8228FBC0(Regs& regs, uint8_t* base) { regs.Apply(e8228FBC0(base, regs.ToArgs())); }
// sub_8228FC50's original.
Result e8228FC50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8228FC50).
inline void c8228FC50(Regs& regs, uint8_t* base) { regs.Apply(e8228FC50(base, regs.ToArgs())); }
// sub_822900E0's original.
Result e822900E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822900E0).
inline void c822900E0(Regs& regs, uint8_t* base) { regs.Apply(e822900E0(base, regs.ToArgs())); }
// sub_822902A0's original.
Result e822902A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822902A0).
inline void c822902A0(Regs& regs, uint8_t* base) { regs.Apply(e822902A0(base, regs.ToArgs())); }
// sub_82290378's original.
Result e82290378(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82290378).
inline void c82290378(Regs& regs, uint8_t* base) { regs.Apply(e82290378(base, regs.ToArgs())); }
// sub_822912A0's original.
Result e822912A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822912A0).
inline void c822912A0(Regs& regs, uint8_t* base) { regs.Apply(e822912A0(base, regs.ToArgs())); }
// sub_82291328's original.
Result e82291328(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82291328).
inline void c82291328(Regs& regs, uint8_t* base) { regs.Apply(e82291328(base, regs.ToArgs())); }
// sub_82294330's original.
Result e82294330(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82294330).
inline void c82294330(Regs& regs, uint8_t* base) { regs.Apply(e82294330(base, regs.ToArgs())); }
// sub_82294610's original.
Result e82294610(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82294610).
inline void c82294610(Regs& regs, uint8_t* base) { regs.Apply(e82294610(base, regs.ToArgs())); }
// sub_82294E58's original.
Result e82294E58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82294E58).
inline void c82294E58(Regs& regs, uint8_t* base) { regs.Apply(e82294E58(base, regs.ToArgs())); }
// sub_82295330's original.
Result e82295330(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82295330).
inline void c82295330(Regs& regs, uint8_t* base) { regs.Apply(e82295330(base, regs.ToArgs())); }
// sub_82295398's original.
Result e82295398(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82295398).
inline void c82295398(Regs& regs, uint8_t* base) { regs.Apply(e82295398(base, regs.ToArgs())); }
// sub_82295508's original.
Result e82295508(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82295508).
inline void c82295508(Regs& regs, uint8_t* base) { regs.Apply(e82295508(base, regs.ToArgs())); }
// sub_82295890's original.
Result e82295890(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82295890).
inline void c82295890(Regs& regs, uint8_t* base) { regs.Apply(e82295890(base, regs.ToArgs())); }
// sub_822964C8's original.
Result e822964C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822964C8).
inline void c822964C8(Regs& regs, uint8_t* base) { regs.Apply(e822964C8(base, regs.ToArgs())); }
// sub_82296550's original.
Result e82296550(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82296550).
inline void c82296550(Regs& regs, uint8_t* base) { regs.Apply(e82296550(base, regs.ToArgs())); }
// sub_822965C8's original.
Result e822965C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822965C8).
inline void c822965C8(Regs& regs, uint8_t* base) { regs.Apply(e822965C8(base, regs.ToArgs())); }
// sub_822976F8's original.
Result e822976F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822976F8).
inline void c822976F8(Regs& regs, uint8_t* base) { regs.Apply(e822976F8(base, regs.ToArgs())); }
// sub_82297790's original.
Result e82297790(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82297790).
inline void c82297790(Regs& regs, uint8_t* base) { regs.Apply(e82297790(base, regs.ToArgs())); }
// sub_82298FF8's original.
Result e82298FF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82298FF8).
inline void c82298FF8(Regs& regs, uint8_t* base) { regs.Apply(e82298FF8(base, regs.ToArgs())); }
// sub_8229E308's original.
Result e8229E308(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229E308).
inline void c8229E308(Regs& regs, uint8_t* base) { regs.Apply(e8229E308(base, regs.ToArgs())); }
// sub_8229EB10's original.
Result e8229EB10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229EB10).
inline void c8229EB10(Regs& regs, uint8_t* base) { regs.Apply(e8229EB10(base, regs.ToArgs())); }
// sub_8229EC00's original.
Result e8229EC00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229EC00).
inline void c8229EC00(Regs& regs, uint8_t* base) { regs.Apply(e8229EC00(base, regs.ToArgs())); }
// sub_8229EF48's original.
Result e8229EF48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229EF48).
inline void c8229EF48(Regs& regs, uint8_t* base) { regs.Apply(e8229EF48(base, regs.ToArgs())); }
// sub_8229F0E0's original.
Result e8229F0E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229F0E0).
inline void c8229F0E0(Regs& regs, uint8_t* base) { regs.Apply(e8229F0E0(base, regs.ToArgs())); }
// sub_8229F148's original.
Result e8229F148(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229F148).
inline void c8229F148(Regs& regs, uint8_t* base) { regs.Apply(e8229F148(base, regs.ToArgs())); }
// sub_8229F1B0's original.
Result e8229F1B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229F1B0).
inline void c8229F1B0(Regs& regs, uint8_t* base) { regs.Apply(e8229F1B0(base, regs.ToArgs())); }
// sub_8229F228's original.
Result e8229F228(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8229F228).
inline void c8229F228(Regs& regs, uint8_t* base) { regs.Apply(e8229F228(base, regs.ToArgs())); }
// sub_822A0F80's original.
Result e822A0F80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A0F80).
inline void c822A0F80(Regs& regs, uint8_t* base) { regs.Apply(e822A0F80(base, regs.ToArgs())); }
// sub_822A3740's original.
Result e822A3740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A3740).
inline void c822A3740(Regs& regs, uint8_t* base) { regs.Apply(e822A3740(base, regs.ToArgs())); }
// sub_822A3758's original.
Result e822A3758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A3758).
inline void c822A3758(Regs& regs, uint8_t* base) { regs.Apply(e822A3758(base, regs.ToArgs())); }
// sub_822A3908's original.
Result e822A3908(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A3908).
inline void c822A3908(Regs& regs, uint8_t* base) { regs.Apply(e822A3908(base, regs.ToArgs())); }
// sub_822A4FF0's original.
Result e822A4FF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A4FF0).
inline void c822A4FF0(Regs& regs, uint8_t* base) { regs.Apply(e822A4FF0(base, regs.ToArgs())); }
// sub_822A5B78's original.
Result e822A5B78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A5B78).
inline void c822A5B78(Regs& regs, uint8_t* base) { regs.Apply(e822A5B78(base, regs.ToArgs())); }
// sub_822A5B80's original.
Result e822A5B80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A5B80).
inline void c822A5B80(Regs& regs, uint8_t* base) { regs.Apply(e822A5B80(base, regs.ToArgs())); }
// sub_822A5C38's original.
Result e822A5C38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A5C38).
inline void c822A5C38(Regs& regs, uint8_t* base) { regs.Apply(e822A5C38(base, regs.ToArgs())); }
// sub_822A5D08's original.
Result e822A5D08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A5D08).
inline void c822A5D08(Regs& regs, uint8_t* base) { regs.Apply(e822A5D08(base, regs.ToArgs())); }
// sub_822A5E28's original.
Result e822A5E28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A5E28).
inline void c822A5E28(Regs& regs, uint8_t* base) { regs.Apply(e822A5E28(base, regs.ToArgs())); }
// sub_822A6008's original.
Result e822A6008(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A6008).
inline void c822A6008(Regs& regs, uint8_t* base) { regs.Apply(e822A6008(base, regs.ToArgs())); }
// sub_822A6210's original.
Result e822A6210(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A6210).
inline void c822A6210(Regs& regs, uint8_t* base) { regs.Apply(e822A6210(base, regs.ToArgs())); }
// sub_822A6738's original.
Result e822A6738(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A6738).
inline void c822A6738(Regs& regs, uint8_t* base) { regs.Apply(e822A6738(base, regs.ToArgs())); }
// sub_822A67B0's original.
Result e822A67B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A67B0).
inline void c822A67B0(Regs& regs, uint8_t* base) { regs.Apply(e822A67B0(base, regs.ToArgs())); }
// sub_822A6830's original.
Result e822A6830(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A6830).
inline void c822A6830(Regs& regs, uint8_t* base) { regs.Apply(e822A6830(base, regs.ToArgs())); }
// sub_822A6968's original.
Result e822A6968(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A6968).
inline void c822A6968(Regs& regs, uint8_t* base) { regs.Apply(e822A6968(base, regs.ToArgs())); }
// sub_822A8740's original.
Result e822A8740(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A8740).
inline void c822A8740(Regs& regs, uint8_t* base) { regs.Apply(e822A8740(base, regs.ToArgs())); }
// sub_822A8808's original.
Result e822A8808(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A8808).
inline void c822A8808(Regs& regs, uint8_t* base) { regs.Apply(e822A8808(base, regs.ToArgs())); }
// sub_822A8980's original.
Result e822A8980(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A8980).
inline void c822A8980(Regs& regs, uint8_t* base) { regs.Apply(e822A8980(base, regs.ToArgs())); }
// sub_822A8A60's original.
Result e822A8A60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A8A60).
inline void c822A8A60(Regs& regs, uint8_t* base) { regs.Apply(e822A8A60(base, regs.ToArgs())); }
// sub_822A9EE8's original.
Result e822A9EE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822A9EE8).
inline void c822A9EE8(Regs& regs, uint8_t* base) { regs.Apply(e822A9EE8(base, regs.ToArgs())); }
// sub_822AA018's original.
Result e822AA018(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822AA018).
inline void c822AA018(Regs& regs, uint8_t* base) { regs.Apply(e822AA018(base, regs.ToArgs())); }
// sub_822AC470's original.
Result e822AC470(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822AC470).
inline void c822AC470(Regs& regs, uint8_t* base) { regs.Apply(e822AC470(base, regs.ToArgs())); }
// sub_822ADC08's original.
Result e822ADC08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822ADC08).
inline void c822ADC08(Regs& regs, uint8_t* base) { regs.Apply(e822ADC08(base, regs.ToArgs())); }
// sub_822ADE30's original.
Result e822ADE30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822ADE30).
inline void c822ADE30(Regs& regs, uint8_t* base) { regs.Apply(e822ADE30(base, regs.ToArgs())); }
// sub_822ADEF0's original.
Result e822ADEF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822ADEF0).
inline void c822ADEF0(Regs& regs, uint8_t* base) { regs.Apply(e822ADEF0(base, regs.ToArgs())); }
// sub_822AE070's original.
Result e822AE070(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822AE070).
inline void c822AE070(Regs& regs, uint8_t* base) { regs.Apply(e822AE070(base, regs.ToArgs())); }
// sub_822AE1D0's original.
Result e822AE1D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822AE1D0).
inline void c822AE1D0(Regs& regs, uint8_t* base) { regs.Apply(e822AE1D0(base, regs.ToArgs())); }
// sub_822AE340's original.
Result e822AE340(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822AE340).
inline void c822AE340(Regs& regs, uint8_t* base) { regs.Apply(e822AE340(base, regs.ToArgs())); }
// sub_822AFA58's original.
Result e822AFA58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822AFA58).
inline void c822AFA58(Regs& regs, uint8_t* base) { regs.Apply(e822AFA58(base, regs.ToArgs())); }
// sub_822B6780's original.
Result e822B6780(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822B6780).
inline void c822B6780(Regs& regs, uint8_t* base) { regs.Apply(e822B6780(base, regs.ToArgs())); }
// sub_822B69A8's original.
Result e822B69A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822B69A8).
inline void c822B69A8(Regs& regs, uint8_t* base) { regs.Apply(e822B69A8(base, regs.ToArgs())); }
// sub_822B6A78's original.
Result e822B6A78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822B6A78).
inline void c822B6A78(Regs& regs, uint8_t* base) { regs.Apply(e822B6A78(base, regs.ToArgs())); }
// sub_822B6AD8's original.
Result e822B6AD8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822B6AD8).
inline void c822B6AD8(Regs& regs, uint8_t* base) { regs.Apply(e822B6AD8(base, regs.ToArgs())); }
// sub_822B6BF8's original.
Result e822B6BF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822B6BF8).
inline void c822B6BF8(Regs& regs, uint8_t* base) { regs.Apply(e822B6BF8(base, regs.ToArgs())); }
// sub_822B80D8's original.
Result e822B80D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822B80D8).
inline void c822B80D8(Regs& regs, uint8_t* base) { regs.Apply(e822B80D8(base, regs.ToArgs())); }
// sub_822B8F10's original.
Result e822B8F10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_822B8F10).
inline void c822B8F10(Regs& regs, uint8_t* base) { regs.Apply(e822B8F10(base, regs.ToArgs())); }
// sub_8235A920's original.
Result e8235A920(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8235A920).
inline void c8235A920(Regs& regs, uint8_t* base) { regs.Apply(e8235A920(base, regs.ToArgs())); }
// sub_82379D50's original.
Result e82379D50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82379D50).
inline void c82379D50(Regs& regs, uint8_t* base) { regs.Apply(e82379D50(base, regs.ToArgs())); }
// sub_823F2620's original.
Result e823F2620(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_823F2620).
inline void c823F2620(Regs& regs, uint8_t* base) { regs.Apply(e823F2620(base, regs.ToArgs())); }
// sub_823F6298's original.
Result e823F6298(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_823F6298).
inline void c823F6298(Regs& regs, uint8_t* base) { regs.Apply(e823F6298(base, regs.ToArgs())); }
// sub_823F6B70's original.
Result e823F6B70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_823F6B70).
inline void c823F6B70(Regs& regs, uint8_t* base) { regs.Apply(e823F6B70(base, regs.ToArgs())); }
// sub_823F6BC0's original.
Result e823F6BC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_823F6BC0).
inline void c823F6BC0(Regs& regs, uint8_t* base) { regs.Apply(e823F6BC0(base, regs.ToArgs())); }
// sub_8245BE08's original.
Result e8245BE08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245BE08).
inline void c8245BE08(Regs& regs, uint8_t* base) { regs.Apply(e8245BE08(base, regs.ToArgs())); }
// sub_8245BED0's original.
Result e8245BED0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245BED0).
inline void c8245BED0(Regs& regs, uint8_t* base) { regs.Apply(e8245BED0(base, regs.ToArgs())); }
// sub_8245DCF0's original.
Result e8245DCF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245DCF0).
inline void c8245DCF0(Regs& regs, uint8_t* base) { regs.Apply(e8245DCF0(base, regs.ToArgs())); }
// sub_8245EF80's original.
Result e8245EF80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245EF80).
inline void c8245EF80(Regs& regs, uint8_t* base) { regs.Apply(e8245EF80(base, regs.ToArgs())); }
// sub_8245EF90's original.
Result e8245EF90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245EF90).
inline void c8245EF90(Regs& regs, uint8_t* base) { regs.Apply(e8245EF90(base, regs.ToArgs())); }
// sub_8245F750's original.
Result e8245F750(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245F750).
inline void c8245F750(Regs& regs, uint8_t* base) { regs.Apply(e8245F750(base, regs.ToArgs())); }
// sub_8245F8E8's original.
Result e8245F8E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245F8E8).
inline void c8245F8E8(Regs& regs, uint8_t* base) { regs.Apply(e8245F8E8(base, regs.ToArgs())); }
// sub_8245FA10's original.
Result e8245FA10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245FA10).
inline void c8245FA10(Regs& regs, uint8_t* base) { regs.Apply(e8245FA10(base, regs.ToArgs())); }
// sub_8245FA28's original.
Result e8245FA28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8245FA28).
inline void c8245FA28(Regs& regs, uint8_t* base) { regs.Apply(e8245FA28(base, regs.ToArgs())); }
// sub_824605E8's original.
Result e824605E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_824605E8).
inline void c824605E8(Regs& regs, uint8_t* base) { regs.Apply(e824605E8(base, regs.ToArgs())); }
// sub_82460688's original.
Result e82460688(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82460688).
inline void c82460688(Regs& regs, uint8_t* base) { regs.Apply(e82460688(base, regs.ToArgs())); }
// sub_824657E0's original.
Result e824657E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_824657E0).
inline void c824657E0(Regs& regs, uint8_t* base) { regs.Apply(e824657E0(base, regs.ToArgs())); }
// sub_82465870's original.
Result e82465870(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82465870).
inline void c82465870(Regs& regs, uint8_t* base) { regs.Apply(e82465870(base, regs.ToArgs())); }
// sub_824658C8's original.
Result e824658C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_824658C8).
inline void c824658C8(Regs& regs, uint8_t* base) { regs.Apply(e824658C8(base, regs.ToArgs())); }
// sub_82469EF8's original.
Result e82469EF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82469EF8).
inline void c82469EF8(Regs& regs, uint8_t* base) { regs.Apply(e82469EF8(base, regs.ToArgs())); }
// sub_8246A1C0's original.
Result e8246A1C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8246A1C0).
inline void c8246A1C0(Regs& regs, uint8_t* base) { regs.Apply(e8246A1C0(base, regs.ToArgs())); }
// sub_8246A860's original.
Result e8246A860(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8246A860).
inline void c8246A860(Regs& regs, uint8_t* base) { regs.Apply(e8246A860(base, regs.ToArgs())); }
// sub_8246CEB8's original.
Result e8246CEB8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8246CEB8).
inline void c8246CEB8(Regs& regs, uint8_t* base) { regs.Apply(e8246CEB8(base, regs.ToArgs())); }
// sub_82480710's original.
Result e82480710(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82480710).
inline void c82480710(Regs& regs, uint8_t* base) { regs.Apply(e82480710(base, regs.ToArgs())); }
// sub_82481880's original.
Result e82481880(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82481880).
inline void c82481880(Regs& regs, uint8_t* base) { regs.Apply(e82481880(base, regs.ToArgs())); }
// sub_82487C48's original.
Result e82487C48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82487C48).
inline void c82487C48(Regs& regs, uint8_t* base) { regs.Apply(e82487C48(base, regs.ToArgs())); }
// sub_82487F68's original.
Result e82487F68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82487F68).
inline void c82487F68(Regs& regs, uint8_t* base) { regs.Apply(e82487F68(base, regs.ToArgs())); }
// sub_824880D8's original.
Result e824880D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_824880D8).
inline void c824880D8(Regs& regs, uint8_t* base) { regs.Apply(e824880D8(base, regs.ToArgs())); }
// sub_82488598's original.
Result e82488598(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82488598).
inline void c82488598(Regs& regs, uint8_t* base) { regs.Apply(e82488598(base, regs.ToArgs())); }
// sub_82488A10's original.
Result e82488A10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82488A10).
inline void c82488A10(Regs& regs, uint8_t* base) { regs.Apply(e82488A10(base, regs.ToArgs())); }
// sub_8248F030's original.
Result e8248F030(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8248F030).
inline void c8248F030(Regs& regs, uint8_t* base) { regs.Apply(e8248F030(base, regs.ToArgs())); }
// sub_824960E0's original.
Result e824960E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_824960E0).
inline void c824960E0(Regs& regs, uint8_t* base) { regs.Apply(e824960E0(base, regs.ToArgs())); }
// sub_82589990's original.
Result e82589990(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82589990).
inline void c82589990(Regs& regs, uint8_t* base) { regs.Apply(e82589990(base, regs.ToArgs())); }
// sub_8258BD28's original.
Result e8258BD28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258BD28).
inline void c8258BD28(Regs& regs, uint8_t* base) { regs.Apply(e8258BD28(base, regs.ToArgs())); }
// sub_8258C688's original.
Result e8258C688(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258C688).
inline void c8258C688(Regs& regs, uint8_t* base) { regs.Apply(e8258C688(base, regs.ToArgs())); }
// sub_8258C718's original.
Result e8258C718(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258C718).
inline void c8258C718(Regs& regs, uint8_t* base) { regs.Apply(e8258C718(base, regs.ToArgs())); }
// sub_8258C768's original.
Result e8258C768(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258C768).
inline void c8258C768(Regs& regs, uint8_t* base) { regs.Apply(e8258C768(base, regs.ToArgs())); }
// sub_8258C820's original.
Result e8258C820(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258C820).
inline void c8258C820(Regs& regs, uint8_t* base) { regs.Apply(e8258C820(base, regs.ToArgs())); }
// sub_8258C9D0's original.
Result e8258C9D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258C9D0).
inline void c8258C9D0(Regs& regs, uint8_t* base) { regs.Apply(e8258C9D0(base, regs.ToArgs())); }
// sub_8258CAA0's original.
Result e8258CAA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258CAA0).
inline void c8258CAA0(Regs& regs, uint8_t* base) { regs.Apply(e8258CAA0(base, regs.ToArgs())); }
// sub_8258CB08's original.
Result e8258CB08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258CB08).
inline void c8258CB08(Regs& regs, uint8_t* base) { regs.Apply(e8258CB08(base, regs.ToArgs())); }
// sub_8258CB98's original.
Result e8258CB98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258CB98).
inline void c8258CB98(Regs& regs, uint8_t* base) { regs.Apply(e8258CB98(base, regs.ToArgs())); }
// sub_8258CC30's original.
Result e8258CC30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258CC30).
inline void c8258CC30(Regs& regs, uint8_t* base) { regs.Apply(e8258CC30(base, regs.ToArgs())); }
// sub_8258CEB0's original.
Result e8258CEB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258CEB0).
inline void c8258CEB0(Regs& regs, uint8_t* base) { regs.Apply(e8258CEB0(base, regs.ToArgs())); }
// sub_8258CEE0's original.
Result e8258CEE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258CEE0).
inline void c8258CEE0(Regs& regs, uint8_t* base) { regs.Apply(e8258CEE0(base, regs.ToArgs())); }
// sub_8258CF10's original.
Result e8258CF10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258CF10).
inline void c8258CF10(Regs& regs, uint8_t* base) { regs.Apply(e8258CF10(base, regs.ToArgs())); }
// sub_8258D998's original.
Result e8258D998(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258D998).
inline void c8258D998(Regs& regs, uint8_t* base) { regs.Apply(e8258D998(base, regs.ToArgs())); }
// sub_8258D9F8's original.
Result e8258D9F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258D9F8).
inline void c8258D9F8(Regs& regs, uint8_t* base) { regs.Apply(e8258D9F8(base, regs.ToArgs())); }
// sub_8258DE88's original.
Result e8258DE88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258DE88).
inline void c8258DE88(Regs& regs, uint8_t* base) { regs.Apply(e8258DE88(base, regs.ToArgs())); }
// sub_8258DF18's original.
Result e8258DF18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258DF18).
inline void c8258DF18(Regs& regs, uint8_t* base) { regs.Apply(e8258DF18(base, regs.ToArgs())); }
// sub_8258DFF0's original.
Result e8258DFF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258DFF0).
inline void c8258DFF0(Regs& regs, uint8_t* base) { regs.Apply(e8258DFF0(base, regs.ToArgs())); }
// sub_8258E098's original.
Result e8258E098(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258E098).
inline void c8258E098(Regs& regs, uint8_t* base) { regs.Apply(e8258E098(base, regs.ToArgs())); }
// sub_8258E1D8's original.
Result e8258E1D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258E1D8).
inline void c8258E1D8(Regs& regs, uint8_t* base) { regs.Apply(e8258E1D8(base, regs.ToArgs())); }
// sub_8258E580's original.
Result e8258E580(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258E580).
inline void c8258E580(Regs& regs, uint8_t* base) { regs.Apply(e8258E580(base, regs.ToArgs())); }
// sub_8258E5B8's original.
Result e8258E5B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258E5B8).
inline void c8258E5B8(Regs& regs, uint8_t* base) { regs.Apply(e8258E5B8(base, regs.ToArgs())); }
// sub_8258E5E0's original.
Result e8258E5E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258E5E0).
inline void c8258E5E0(Regs& regs, uint8_t* base) { regs.Apply(e8258E5E0(base, regs.ToArgs())); }
// sub_8258E9F8's original.
Result e8258E9F8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258E9F8).
inline void c8258E9F8(Regs& regs, uint8_t* base) { regs.Apply(e8258E9F8(base, regs.ToArgs())); }
// sub_8258EA20's original.
Result e8258EA20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258EA20).
inline void c8258EA20(Regs& regs, uint8_t* base) { regs.Apply(e8258EA20(base, regs.ToArgs())); }
// sub_8258EA58's original.
Result e8258EA58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258EA58).
inline void c8258EA58(Regs& regs, uint8_t* base) { regs.Apply(e8258EA58(base, regs.ToArgs())); }
// sub_8258EAD0's original.
Result e8258EAD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258EAD0).
inline void c8258EAD0(Regs& regs, uint8_t* base) { regs.Apply(e8258EAD0(base, regs.ToArgs())); }
// sub_8258EAE0's original.
Result e8258EAE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258EAE0).
inline void c8258EAE0(Regs& regs, uint8_t* base) { regs.Apply(e8258EAE0(base, regs.ToArgs())); }
// sub_8258ED78's original.
Result e8258ED78(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258ED78).
inline void c8258ED78(Regs& regs, uint8_t* base) { regs.Apply(e8258ED78(base, regs.ToArgs())); }
// sub_8258EDC8's original.
Result e8258EDC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258EDC8).
inline void c8258EDC8(Regs& regs, uint8_t* base) { regs.Apply(e8258EDC8(base, regs.ToArgs())); }
// sub_8258F5C0's original.
Result e8258F5C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258F5C0).
inline void c8258F5C0(Regs& regs, uint8_t* base) { regs.Apply(e8258F5C0(base, regs.ToArgs())); }
// sub_8258FBD0's original.
Result e8258FBD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FBD0).
inline void c8258FBD0(Regs& regs, uint8_t* base) { regs.Apply(e8258FBD0(base, regs.ToArgs())); }
// sub_8258FC30's original.
Result e8258FC30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FC30).
inline void c8258FC30(Regs& regs, uint8_t* base) { regs.Apply(e8258FC30(base, regs.ToArgs())); }
// sub_8258FC90's original.
Result e8258FC90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FC90).
inline void c8258FC90(Regs& regs, uint8_t* base) { regs.Apply(e8258FC90(base, regs.ToArgs())); }
// sub_8258FCE0's original.
Result e8258FCE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FCE0).
inline void c8258FCE0(Regs& regs, uint8_t* base) { regs.Apply(e8258FCE0(base, regs.ToArgs())); }
// sub_8258FD48's original.
Result e8258FD48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FD48).
inline void c8258FD48(Regs& regs, uint8_t* base) { regs.Apply(e8258FD48(base, regs.ToArgs())); }
// sub_8258FF50's original.
Result e8258FF50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FF50).
inline void c8258FF50(Regs& regs, uint8_t* base) { regs.Apply(e8258FF50(base, regs.ToArgs())); }
// sub_8258FFA0's original.
Result e8258FFA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FFA0).
inline void c8258FFA0(Regs& regs, uint8_t* base) { regs.Apply(e8258FFA0(base, regs.ToArgs())); }
// sub_8258FFF0's original.
Result e8258FFF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8258FFF0).
inline void c8258FFF0(Regs& regs, uint8_t* base) { regs.Apply(e8258FFF0(base, regs.ToArgs())); }
// sub_82590040's original.
Result e82590040(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590040).
inline void c82590040(Regs& regs, uint8_t* base) { regs.Apply(e82590040(base, regs.ToArgs())); }
// sub_825900B8's original.
Result e825900B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825900B8).
inline void c825900B8(Regs& regs, uint8_t* base) { regs.Apply(e825900B8(base, regs.ToArgs())); }
// sub_82590108's original.
Result e82590108(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590108).
inline void c82590108(Regs& regs, uint8_t* base) { regs.Apply(e82590108(base, regs.ToArgs())); }
// sub_825901B0's original.
Result e825901B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825901B0).
inline void c825901B0(Regs& regs, uint8_t* base) { regs.Apply(e825901B0(base, regs.ToArgs())); }
// sub_82590268's original.
Result e82590268(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590268).
inline void c82590268(Regs& regs, uint8_t* base) { regs.Apply(e82590268(base, regs.ToArgs())); }
// sub_825902E0's original.
Result e825902E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825902E0).
inline void c825902E0(Regs& regs, uint8_t* base) { regs.Apply(e825902E0(base, regs.ToArgs())); }
// sub_82590360's original.
Result e82590360(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590360).
inline void c82590360(Regs& regs, uint8_t* base) { regs.Apply(e82590360(base, regs.ToArgs())); }
// sub_825903D8's original.
Result e825903D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825903D8).
inline void c825903D8(Regs& regs, uint8_t* base) { regs.Apply(e825903D8(base, regs.ToArgs())); }
// sub_82590428's original.
Result e82590428(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590428).
inline void c82590428(Regs& regs, uint8_t* base) { regs.Apply(e82590428(base, regs.ToArgs())); }
// sub_82590478's original.
Result e82590478(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590478).
inline void c82590478(Regs& regs, uint8_t* base) { regs.Apply(e82590478(base, regs.ToArgs())); }
// sub_825904C8's original.
Result e825904C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825904C8).
inline void c825904C8(Regs& regs, uint8_t* base) { regs.Apply(e825904C8(base, regs.ToArgs())); }
// sub_82590620's original.
Result e82590620(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590620).
inline void c82590620(Regs& regs, uint8_t* base) { regs.Apply(e82590620(base, regs.ToArgs())); }
// sub_82590698's original.
Result e82590698(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590698).
inline void c82590698(Regs& regs, uint8_t* base) { regs.Apply(e82590698(base, regs.ToArgs())); }
// sub_82590768's original.
Result e82590768(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590768).
inline void c82590768(Regs& regs, uint8_t* base) { regs.Apply(e82590768(base, regs.ToArgs())); }
// sub_82590860's original.
Result e82590860(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590860).
inline void c82590860(Regs& regs, uint8_t* base) { regs.Apply(e82590860(base, regs.ToArgs())); }
// sub_825908E0's original.
Result e825908E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825908E0).
inline void c825908E0(Regs& regs, uint8_t* base) { regs.Apply(e825908E0(base, regs.ToArgs())); }
// sub_82590968's original.
Result e82590968(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590968).
inline void c82590968(Regs& regs, uint8_t* base) { regs.Apply(e82590968(base, regs.ToArgs())); }
// sub_825909E8's original.
Result e825909E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825909E8).
inline void c825909E8(Regs& regs, uint8_t* base) { regs.Apply(e825909E8(base, regs.ToArgs())); }
// sub_82590A68's original.
Result e82590A68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590A68).
inline void c82590A68(Regs& regs, uint8_t* base) { regs.Apply(e82590A68(base, regs.ToArgs())); }
// sub_82590AD0's original.
Result e82590AD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590AD0).
inline void c82590AD0(Regs& regs, uint8_t* base) { regs.Apply(e82590AD0(base, regs.ToArgs())); }
// sub_82590EF0's original.
Result e82590EF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82590EF0).
inline void c82590EF0(Regs& regs, uint8_t* base) { regs.Apply(e82590EF0(base, regs.ToArgs())); }
// sub_82591540's original.
Result e82591540(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82591540).
inline void c82591540(Regs& regs, uint8_t* base) { regs.Apply(e82591540(base, regs.ToArgs())); }
// sub_825915E0's original.
Result e825915E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825915E0).
inline void c825915E0(Regs& regs, uint8_t* base) { regs.Apply(e825915E0(base, regs.ToArgs())); }
// sub_82591798's original.
Result e82591798(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82591798).
inline void c82591798(Regs& regs, uint8_t* base) { regs.Apply(e82591798(base, regs.ToArgs())); }
// sub_82591B30's original.
Result e82591B30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82591B30).
inline void c82591B30(Regs& regs, uint8_t* base) { regs.Apply(e82591B30(base, regs.ToArgs())); }
// sub_82592188's original.
Result e82592188(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82592188).
inline void c82592188(Regs& regs, uint8_t* base) { regs.Apply(e82592188(base, regs.ToArgs())); }
// sub_825924E0's original.
Result e825924E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825924E0).
inline void c825924E0(Regs& regs, uint8_t* base) { regs.Apply(e825924E0(base, regs.ToArgs())); }
// sub_82592670's original.
Result e82592670(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82592670).
inline void c82592670(Regs& regs, uint8_t* base) { regs.Apply(e82592670(base, regs.ToArgs())); }
// sub_825926E8's original.
Result e825926E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825926E8).
inline void c825926E8(Regs& regs, uint8_t* base) { regs.Apply(e825926E8(base, regs.ToArgs())); }
// sub_82593040's original.
Result e82593040(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593040).
inline void c82593040(Regs& regs, uint8_t* base) { regs.Apply(e82593040(base, regs.ToArgs())); }
// sub_82593088's original.
Result e82593088(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593088).
inline void c82593088(Regs& regs, uint8_t* base) { regs.Apply(e82593088(base, regs.ToArgs())); }
// sub_825931D0's original.
Result e825931D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825931D0).
inline void c825931D0(Regs& regs, uint8_t* base) { regs.Apply(e825931D0(base, regs.ToArgs())); }
// sub_82593248's original.
Result e82593248(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593248).
inline void c82593248(Regs& regs, uint8_t* base) { regs.Apply(e82593248(base, regs.ToArgs())); }
// sub_82593278's original.
Result e82593278(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593278).
inline void c82593278(Regs& regs, uint8_t* base) { regs.Apply(e82593278(base, regs.ToArgs())); }
// sub_82593500's original.
Result e82593500(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593500).
inline void c82593500(Regs& regs, uint8_t* base) { regs.Apply(e82593500(base, regs.ToArgs())); }
// sub_82593568's original.
Result e82593568(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593568).
inline void c82593568(Regs& regs, uint8_t* base) { regs.Apply(e82593568(base, regs.ToArgs())); }
// sub_82593908's original.
Result e82593908(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593908).
inline void c82593908(Regs& regs, uint8_t* base) { regs.Apply(e82593908(base, regs.ToArgs())); }
// sub_82593DC0's original.
Result e82593DC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593DC0).
inline void c82593DC0(Regs& regs, uint8_t* base) { regs.Apply(e82593DC0(base, regs.ToArgs())); }
// sub_82593FC0's original.
Result e82593FC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82593FC0).
inline void c82593FC0(Regs& regs, uint8_t* base) { regs.Apply(e82593FC0(base, regs.ToArgs())); }
// sub_825943E0's original.
Result e825943E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825943E0).
inline void c825943E0(Regs& regs, uint8_t* base) { regs.Apply(e825943E0(base, regs.ToArgs())); }
// sub_82594718's original.
Result e82594718(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82594718).
inline void c82594718(Regs& regs, uint8_t* base) { regs.Apply(e82594718(base, regs.ToArgs())); }
// sub_82594850's original.
Result e82594850(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82594850).
inline void c82594850(Regs& regs, uint8_t* base) { regs.Apply(e82594850(base, regs.ToArgs())); }
// sub_825948C0's original.
Result e825948C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825948C0).
inline void c825948C0(Regs& regs, uint8_t* base) { regs.Apply(e825948C0(base, regs.ToArgs())); }
// sub_82594970's original.
Result e82594970(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82594970).
inline void c82594970(Regs& regs, uint8_t* base) { regs.Apply(e82594970(base, regs.ToArgs())); }
// sub_82594D10's original.
Result e82594D10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82594D10).
inline void c82594D10(Regs& regs, uint8_t* base) { regs.Apply(e82594D10(base, regs.ToArgs())); }
// sub_82594F58's original.
Result e82594F58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82594F58).
inline void c82594F58(Regs& regs, uint8_t* base) { regs.Apply(e82594F58(base, regs.ToArgs())); }
// sub_825953D0's original.
Result e825953D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825953D0).
inline void c825953D0(Regs& regs, uint8_t* base) { regs.Apply(e825953D0(base, regs.ToArgs())); }
// sub_82595490's original.
Result e82595490(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82595490).
inline void c82595490(Regs& regs, uint8_t* base) { regs.Apply(e82595490(base, regs.ToArgs())); }
// sub_82595550's original.
Result e82595550(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82595550).
inline void c82595550(Regs& regs, uint8_t* base) { regs.Apply(e82595550(base, regs.ToArgs())); }
// sub_825960D0's original.
Result e825960D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825960D0).
inline void c825960D0(Regs& regs, uint8_t* base) { regs.Apply(e825960D0(base, regs.ToArgs())); }
// sub_82596920's original.
Result e82596920(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82596920).
inline void c82596920(Regs& regs, uint8_t* base) { regs.Apply(e82596920(base, regs.ToArgs())); }
// sub_82596B18's original.
Result e82596B18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82596B18).
inline void c82596B18(Regs& regs, uint8_t* base) { regs.Apply(e82596B18(base, regs.ToArgs())); }
// sub_82596D48's original.
Result e82596D48(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82596D48).
inline void c82596D48(Regs& regs, uint8_t* base) { regs.Apply(e82596D48(base, regs.ToArgs())); }
// sub_82596E68's original.
Result e82596E68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82596E68).
inline void c82596E68(Regs& regs, uint8_t* base) { regs.Apply(e82596E68(base, regs.ToArgs())); }
// sub_82596F68's original.
Result e82596F68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82596F68).
inline void c82596F68(Regs& regs, uint8_t* base) { regs.Apply(e82596F68(base, regs.ToArgs())); }
// sub_825970A8's original.
Result e825970A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825970A8).
inline void c825970A8(Regs& regs, uint8_t* base) { regs.Apply(e825970A8(base, regs.ToArgs())); }
// sub_82597118's original.
Result e82597118(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82597118).
inline void c82597118(Regs& regs, uint8_t* base) { regs.Apply(e82597118(base, regs.ToArgs())); }
// sub_82597148's original.
Result e82597148(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82597148).
inline void c82597148(Regs& regs, uint8_t* base) { regs.Apply(e82597148(base, regs.ToArgs())); }
// sub_825972A8's original.
Result e825972A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825972A8).
inline void c825972A8(Regs& regs, uint8_t* base) { regs.Apply(e825972A8(base, regs.ToArgs())); }
// sub_82597518's original.
Result e82597518(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82597518).
inline void c82597518(Regs& regs, uint8_t* base) { regs.Apply(e82597518(base, regs.ToArgs())); }
// sub_82597610's original.
Result e82597610(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82597610).
inline void c82597610(Regs& regs, uint8_t* base) { regs.Apply(e82597610(base, regs.ToArgs())); }
// sub_82598158's original.
Result e82598158(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82598158).
inline void c82598158(Regs& regs, uint8_t* base) { regs.Apply(e82598158(base, regs.ToArgs())); }
// sub_82598288's original.
Result e82598288(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82598288).
inline void c82598288(Regs& regs, uint8_t* base) { regs.Apply(e82598288(base, regs.ToArgs())); }
// sub_82598648's original.
Result e82598648(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82598648).
inline void c82598648(Regs& regs, uint8_t* base) { regs.Apply(e82598648(base, regs.ToArgs())); }
// sub_825986B8's original.
Result e825986B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825986B8).
inline void c825986B8(Regs& regs, uint8_t* base) { regs.Apply(e825986B8(base, regs.ToArgs())); }
// sub_82598768's original.
Result e82598768(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82598768).
inline void c82598768(Regs& regs, uint8_t* base) { regs.Apply(e82598768(base, regs.ToArgs())); }
// sub_82599758's original.
Result e82599758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82599758).
inline void c82599758(Regs& regs, uint8_t* base) { regs.Apply(e82599758(base, regs.ToArgs())); }
// sub_825997A0's original.
Result e825997A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825997A0).
inline void c825997A0(Regs& regs, uint8_t* base) { regs.Apply(e825997A0(base, regs.ToArgs())); }
// sub_825997E0's original.
Result e825997E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825997E0).
inline void c825997E0(Regs& regs, uint8_t* base) { regs.Apply(e825997E0(base, regs.ToArgs())); }
// sub_82599880's original.
Result e82599880(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82599880).
inline void c82599880(Regs& regs, uint8_t* base) { regs.Apply(e82599880(base, regs.ToArgs())); }
// sub_825998A0's original.
Result e825998A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825998A0).
inline void c825998A0(Regs& regs, uint8_t* base) { regs.Apply(e825998A0(base, regs.ToArgs())); }
// sub_825998B8's original.
Result e825998B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825998B8).
inline void c825998B8(Regs& regs, uint8_t* base) { regs.Apply(e825998B8(base, regs.ToArgs())); }
// sub_825999C8's original.
Result e825999C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825999C8).
inline void c825999C8(Regs& regs, uint8_t* base) { regs.Apply(e825999C8(base, regs.ToArgs())); }
// sub_82599A20's original.
Result e82599A20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_82599A20).
inline void c82599A20(Regs& regs, uint8_t* base) { regs.Apply(e82599A20(base, regs.ToArgs())); }
// sub_8259A140's original.
Result e8259A140(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259A140).
inline void c8259A140(Regs& regs, uint8_t* base) { regs.Apply(e8259A140(base, regs.ToArgs())); }
// sub_8259A228's original.
Result e8259A228(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259A228).
inline void c8259A228(Regs& regs, uint8_t* base) { regs.Apply(e8259A228(base, regs.ToArgs())); }
// sub_8259A9D0's original.
Result e8259A9D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259A9D0).
inline void c8259A9D0(Regs& regs, uint8_t* base) { regs.Apply(e8259A9D0(base, regs.ToArgs())); }
// sub_8259A9E0's original.
Result e8259A9E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259A9E0).
inline void c8259A9E0(Regs& regs, uint8_t* base) { regs.Apply(e8259A9E0(base, regs.ToArgs())); }
// sub_8259C9B0's original.
Result e8259C9B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259C9B0).
inline void c8259C9B0(Regs& regs, uint8_t* base) { regs.Apply(e8259C9B0(base, regs.ToArgs())); }
// sub_8259CE10's original.
Result e8259CE10(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259CE10).
inline void c8259CE10(Regs& regs, uint8_t* base) { regs.Apply(e8259CE10(base, regs.ToArgs())); }
// sub_8259D1A8's original.
Result e8259D1A8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259D1A8).
inline void c8259D1A8(Regs& regs, uint8_t* base) { regs.Apply(e8259D1A8(base, regs.ToArgs())); }
// sub_8259D3D8's original.
Result e8259D3D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259D3D8).
inline void c8259D3D8(Regs& regs, uint8_t* base) { regs.Apply(e8259D3D8(base, regs.ToArgs())); }
// sub_8259D418's original.
Result e8259D418(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259D418).
inline void c8259D418(Regs& regs, uint8_t* base) { regs.Apply(e8259D418(base, regs.ToArgs())); }
// sub_8259D478's original.
Result e8259D478(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259D478).
inline void c8259D478(Regs& regs, uint8_t* base) { regs.Apply(e8259D478(base, regs.ToArgs())); }
// sub_8259D7B0's original.
Result e8259D7B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259D7B0).
inline void c8259D7B0(Regs& regs, uint8_t* base) { regs.Apply(e8259D7B0(base, regs.ToArgs())); }
// sub_8259DAC0's original.
Result e8259DAC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259DAC0).
inline void c8259DAC0(Regs& regs, uint8_t* base) { regs.Apply(e8259DAC0(base, regs.ToArgs())); }
// sub_8259DAD0's original.
Result e8259DAD0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259DAD0).
inline void c8259DAD0(Regs& regs, uint8_t* base) { regs.Apply(e8259DAD0(base, regs.ToArgs())); }
// sub_8259E098's original.
Result e8259E098(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259E098).
inline void c8259E098(Regs& regs, uint8_t* base) { regs.Apply(e8259E098(base, regs.ToArgs())); }
// sub_8259EBE8's original.
Result e8259EBE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259EBE8).
inline void c8259EBE8(Regs& regs, uint8_t* base) { regs.Apply(e8259EBE8(base, regs.ToArgs())); }
// sub_8259ED40's original.
Result e8259ED40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259ED40).
inline void c8259ED40(Regs& regs, uint8_t* base) { regs.Apply(e8259ED40(base, regs.ToArgs())); }
// sub_8259F348's original.
Result e8259F348(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259F348).
inline void c8259F348(Regs& regs, uint8_t* base) { regs.Apply(e8259F348(base, regs.ToArgs())); }
// sub_8259F770's original.
Result e8259F770(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259F770).
inline void c8259F770(Regs& regs, uint8_t* base) { regs.Apply(e8259F770(base, regs.ToArgs())); }
// sub_8259FA00's original.
Result e8259FA00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259FA00).
inline void c8259FA00(Regs& regs, uint8_t* base) { regs.Apply(e8259FA00(base, regs.ToArgs())); }
// sub_8259FB30's original.
Result e8259FB30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259FB30).
inline void c8259FB30(Regs& regs, uint8_t* base) { regs.Apply(e8259FB30(base, regs.ToArgs())); }
// sub_8259FC18's original.
Result e8259FC18(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_8259FC18).
inline void c8259FC18(Regs& regs, uint8_t* base) { regs.Apply(e8259FC18(base, regs.ToArgs())); }
// sub_825A0548's original.
Result e825A0548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A0548).
inline void c825A0548(Regs& regs, uint8_t* base) { regs.Apply(e825A0548(base, regs.ToArgs())); }
// sub_825A08E0's original.
Result e825A08E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A08E0).
inline void c825A08E0(Regs& regs, uint8_t* base) { regs.Apply(e825A08E0(base, regs.ToArgs())); }
// sub_825A11D0's original.
Result e825A11D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A11D0).
inline void c825A11D0(Regs& regs, uint8_t* base) { regs.Apply(e825A11D0(base, regs.ToArgs())); }
// sub_825A1218's original.
Result e825A1218(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A1218).
inline void c825A1218(Regs& regs, uint8_t* base) { regs.Apply(e825A1218(base, regs.ToArgs())); }
// sub_825A1378's original.
Result e825A1378(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A1378).
inline void c825A1378(Regs& regs, uint8_t* base) { regs.Apply(e825A1378(base, regs.ToArgs())); }
// sub_825A18D8's original.
Result e825A18D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A18D8).
inline void c825A18D8(Regs& regs, uint8_t* base) { regs.Apply(e825A18D8(base, regs.ToArgs())); }
// sub_825A1A38's original.
Result e825A1A38(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A1A38).
inline void c825A1A38(Regs& regs, uint8_t* base) { regs.Apply(e825A1A38(base, regs.ToArgs())); }
// sub_825A1B70's original.
Result e825A1B70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A1B70).
inline void c825A1B70(Regs& regs, uint8_t* base) { regs.Apply(e825A1B70(base, regs.ToArgs())); }
// sub_825A1BA8's original.
Result e825A1BA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A1BA8).
inline void c825A1BA8(Regs& regs, uint8_t* base) { regs.Apply(e825A1BA8(base, regs.ToArgs())); }
// sub_825A1BE8's original.
Result e825A1BE8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A1BE8).
inline void c825A1BE8(Regs& regs, uint8_t* base) { regs.Apply(e825A1BE8(base, regs.ToArgs())); }
// sub_825A1E08's original.
Result e825A1E08(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A1E08).
inline void c825A1E08(Regs& regs, uint8_t* base) { regs.Apply(e825A1E08(base, regs.ToArgs())); }
// sub_825A2068's original.
Result e825A2068(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A2068).
inline void c825A2068(Regs& regs, uint8_t* base) { regs.Apply(e825A2068(base, regs.ToArgs())); }
// sub_825A6880's original.
Result e825A6880(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A6880).
inline void c825A6880(Regs& regs, uint8_t* base) { regs.Apply(e825A6880(base, regs.ToArgs())); }
// sub_825A6940's original.
Result e825A6940(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A6940).
inline void c825A6940(Regs& regs, uint8_t* base) { regs.Apply(e825A6940(base, regs.ToArgs())); }
// sub_825A6F00's original.
Result e825A6F00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A6F00).
inline void c825A6F00(Regs& regs, uint8_t* base) { regs.Apply(e825A6F00(base, regs.ToArgs())); }
// sub_825A7068's original.
Result e825A7068(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A7068).
inline void c825A7068(Regs& regs, uint8_t* base) { regs.Apply(e825A7068(base, regs.ToArgs())); }
// sub_825A70E8's original.
Result e825A70E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A70E8).
inline void c825A70E8(Regs& regs, uint8_t* base) { regs.Apply(e825A70E8(base, regs.ToArgs())); }
// sub_825A71B0's original.
Result e825A71B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A71B0).
inline void c825A71B0(Regs& regs, uint8_t* base) { regs.Apply(e825A71B0(base, regs.ToArgs())); }
// sub_825A71C0's original.
Result e825A71C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A71C0).
inline void c825A71C0(Regs& regs, uint8_t* base) { regs.Apply(e825A71C0(base, regs.ToArgs())); }
// sub_825A7270's original.
Result e825A7270(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A7270).
inline void c825A7270(Regs& regs, uint8_t* base) { regs.Apply(e825A7270(base, regs.ToArgs())); }
// sub_825A7348's original.
Result e825A7348(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A7348).
inline void c825A7348(Regs& regs, uint8_t* base) { regs.Apply(e825A7348(base, regs.ToArgs())); }
// sub_825A7410's original.
Result e825A7410(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A7410).
inline void c825A7410(Regs& regs, uint8_t* base) { regs.Apply(e825A7410(base, regs.ToArgs())); }
// sub_825A85C0's original.
Result e825A85C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A85C0).
inline void c825A85C0(Regs& regs, uint8_t* base) { regs.Apply(e825A85C0(base, regs.ToArgs())); }
// sub_825A85E0's original.
Result e825A85E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A85E0).
inline void c825A85E0(Regs& regs, uint8_t* base) { regs.Apply(e825A85E0(base, regs.ToArgs())); }
// sub_825A8A50's original.
Result e825A8A50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A8A50).
inline void c825A8A50(Regs& regs, uint8_t* base) { regs.Apply(e825A8A50(base, regs.ToArgs())); }
// sub_825A93D0's original.
Result e825A93D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A93D0).
inline void c825A93D0(Regs& regs, uint8_t* base) { regs.Apply(e825A93D0(base, regs.ToArgs())); }
// sub_825A9A90's original.
Result e825A9A90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A9A90).
inline void c825A9A90(Regs& regs, uint8_t* base) { regs.Apply(e825A9A90(base, regs.ToArgs())); }
// sub_825A9B58's original.
Result e825A9B58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825A9B58).
inline void c825A9B58(Regs& regs, uint8_t* base) { regs.Apply(e825A9B58(base, regs.ToArgs())); }
// sub_825AA840's original.
Result e825AA840(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AA840).
inline void c825AA840(Regs& regs, uint8_t* base) { regs.Apply(e825AA840(base, regs.ToArgs())); }
// sub_825AAD20's original.
Result e825AAD20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AAD20).
inline void c825AAD20(Regs& regs, uint8_t* base) { regs.Apply(e825AAD20(base, regs.ToArgs())); }
// sub_825AB010's original.
Result e825AB010(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AB010).
inline void c825AB010(Regs& regs, uint8_t* base) { regs.Apply(e825AB010(base, regs.ToArgs())); }
// sub_825AB1C0's original.
Result e825AB1C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AB1C0).
inline void c825AB1C0(Regs& regs, uint8_t* base) { regs.Apply(e825AB1C0(base, regs.ToArgs())); }
// sub_825AC320's original.
Result e825AC320(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AC320).
inline void c825AC320(Regs& regs, uint8_t* base) { regs.Apply(e825AC320(base, regs.ToArgs())); }
// sub_825AC710's original.
Result e825AC710(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AC710).
inline void c825AC710(Regs& regs, uint8_t* base) { regs.Apply(e825AC710(base, regs.ToArgs())); }
// sub_825ADEB0's original.
Result e825ADEB0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825ADEB0).
inline void c825ADEB0(Regs& regs, uint8_t* base) { regs.Apply(e825ADEB0(base, regs.ToArgs())); }
// sub_825AE8E0's original.
Result e825AE8E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AE8E0).
inline void c825AE8E0(Regs& regs, uint8_t* base) { regs.Apply(e825AE8E0(base, regs.ToArgs())); }
// sub_825AEBC8's original.
Result e825AEBC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AEBC8).
inline void c825AEBC8(Regs& regs, uint8_t* base) { regs.Apply(e825AEBC8(base, regs.ToArgs())); }
// sub_825AED30's original.
Result e825AED30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AED30).
inline void c825AED30(Regs& regs, uint8_t* base) { regs.Apply(e825AED30(base, regs.ToArgs())); }
// sub_825AF000's original.
Result e825AF000(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AF000).
inline void c825AF000(Regs& regs, uint8_t* base) { regs.Apply(e825AF000(base, regs.ToArgs())); }
// sub_825AF098's original.
Result e825AF098(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825AF098).
inline void c825AF098(Regs& regs, uint8_t* base) { regs.Apply(e825AF098(base, regs.ToArgs())); }
// sub_825B32D0's original.
Result e825B32D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825B32D0).
inline void c825B32D0(Regs& regs, uint8_t* base) { regs.Apply(e825B32D0(base, regs.ToArgs())); }
// sub_825B3430's original.
Result e825B3430(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825B3430).
inline void c825B3430(Regs& regs, uint8_t* base) { regs.Apply(e825B3430(base, regs.ToArgs())); }
// sub_825B6098's original.
Result e825B6098(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825B6098).
inline void c825B6098(Regs& regs, uint8_t* base) { regs.Apply(e825B6098(base, regs.ToArgs())); }
// sub_825B6140's original.
Result e825B6140(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825B6140).
inline void c825B6140(Regs& regs, uint8_t* base) { regs.Apply(e825B6140(base, regs.ToArgs())); }
// sub_825BA558's original.
Result e825BA558(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BA558).
inline void c825BA558(Regs& regs, uint8_t* base) { regs.Apply(e825BA558(base, regs.ToArgs())); }
// sub_825BD8F0's original.
Result e825BD8F0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BD8F0).
inline void c825BD8F0(Regs& regs, uint8_t* base) { regs.Apply(e825BD8F0(base, regs.ToArgs())); }
// sub_825BDDA0's original.
Result e825BDDA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BDDA0).
inline void c825BDDA0(Regs& regs, uint8_t* base) { regs.Apply(e825BDDA0(base, regs.ToArgs())); }
// sub_825BDDC0's original.
Result e825BDDC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BDDC0).
inline void c825BDDC0(Regs& regs, uint8_t* base) { regs.Apply(e825BDDC0(base, regs.ToArgs())); }
// sub_825BE300's original.
Result e825BE300(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BE300).
inline void c825BE300(Regs& regs, uint8_t* base) { regs.Apply(e825BE300(base, regs.ToArgs())); }
// sub_825BE708's original.
Result e825BE708(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BE708).
inline void c825BE708(Regs& regs, uint8_t* base) { regs.Apply(e825BE708(base, regs.ToArgs())); }
// sub_825BEB80's original.
Result e825BEB80(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BEB80).
inline void c825BEB80(Regs& regs, uint8_t* base) { regs.Apply(e825BEB80(base, regs.ToArgs())); }
// sub_825BFC50's original.
Result e825BFC50(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BFC50).
inline void c825BFC50(Regs& regs, uint8_t* base) { regs.Apply(e825BFC50(base, regs.ToArgs())); }
// sub_825BFE28's original.
Result e825BFE28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825BFE28).
inline void c825BFE28(Regs& regs, uint8_t* base) { regs.Apply(e825BFE28(base, regs.ToArgs())); }
// sub_825C0408's original.
Result e825C0408(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C0408).
inline void c825C0408(Regs& regs, uint8_t* base) { regs.Apply(e825C0408(base, regs.ToArgs())); }
// sub_825C0598's original.
Result e825C0598(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C0598).
inline void c825C0598(Regs& regs, uint8_t* base) { regs.Apply(e825C0598(base, regs.ToArgs())); }
// sub_825C05B0's original.
Result e825C05B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C05B0).
inline void c825C05B0(Regs& regs, uint8_t* base) { regs.Apply(e825C05B0(base, regs.ToArgs())); }
// sub_825C1518's original.
Result e825C1518(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C1518).
inline void c825C1518(Regs& regs, uint8_t* base) { regs.Apply(e825C1518(base, regs.ToArgs())); }
// sub_825C1F30's original.
Result e825C1F30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C1F30).
inline void c825C1F30(Regs& regs, uint8_t* base) { regs.Apply(e825C1F30(base, regs.ToArgs())); }
// sub_825C2060's original.
Result e825C2060(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2060).
inline void c825C2060(Regs& regs, uint8_t* base) { regs.Apply(e825C2060(base, regs.ToArgs())); }
// sub_825C21B0's original.
Result e825C21B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C21B0).
inline void c825C21B0(Regs& regs, uint8_t* base) { regs.Apply(e825C21B0(base, regs.ToArgs())); }
// sub_825C2200's original.
Result e825C2200(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2200).
inline void c825C2200(Regs& regs, uint8_t* base) { regs.Apply(e825C2200(base, regs.ToArgs())); }
// sub_825C2208's original.
Result e825C2208(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2208).
inline void c825C2208(Regs& regs, uint8_t* base) { regs.Apply(e825C2208(base, regs.ToArgs())); }
// sub_825C2268's original.
Result e825C2268(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2268).
inline void c825C2268(Regs& regs, uint8_t* base) { regs.Apply(e825C2268(base, regs.ToArgs())); }
// sub_825C22C8's original.
Result e825C22C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C22C8).
inline void c825C22C8(Regs& regs, uint8_t* base) { regs.Apply(e825C22C8(base, regs.ToArgs())); }
// sub_825C23C8's original.
Result e825C23C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C23C8).
inline void c825C23C8(Regs& regs, uint8_t* base) { regs.Apply(e825C23C8(base, regs.ToArgs())); }
// sub_825C2428's original.
Result e825C2428(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2428).
inline void c825C2428(Regs& regs, uint8_t* base) { regs.Apply(e825C2428(base, regs.ToArgs())); }
// sub_825C2480's original.
Result e825C2480(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2480).
inline void c825C2480(Regs& regs, uint8_t* base) { regs.Apply(e825C2480(base, regs.ToArgs())); }
// sub_825C24B0's original.
Result e825C24B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C24B0).
inline void c825C24B0(Regs& regs, uint8_t* base) { regs.Apply(e825C24B0(base, regs.ToArgs())); }
// sub_825C24E8's original.
Result e825C24E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C24E8).
inline void c825C24E8(Regs& regs, uint8_t* base) { regs.Apply(e825C24E8(base, regs.ToArgs())); }
// sub_825C2518's original.
Result e825C2518(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2518).
inline void c825C2518(Regs& regs, uint8_t* base) { regs.Apply(e825C2518(base, regs.ToArgs())); }
// sub_825C2548's original.
Result e825C2548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2548).
inline void c825C2548(Regs& regs, uint8_t* base) { regs.Apply(e825C2548(base, regs.ToArgs())); }
// sub_825C2618's original.
Result e825C2618(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2618).
inline void c825C2618(Regs& regs, uint8_t* base) { regs.Apply(e825C2618(base, regs.ToArgs())); }
// sub_825C26E0's original.
Result e825C26E0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C26E0).
inline void c825C26E0(Regs& regs, uint8_t* base) { regs.Apply(e825C26E0(base, regs.ToArgs())); }
// sub_825C27C0's original.
Result e825C27C0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C27C0).
inline void c825C27C0(Regs& regs, uint8_t* base) { regs.Apply(e825C27C0(base, regs.ToArgs())); }
// sub_825C2910's original.
Result e825C2910(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2910).
inline void c825C2910(Regs& regs, uint8_t* base) { regs.Apply(e825C2910(base, regs.ToArgs())); }
// sub_825C2C60's original.
Result e825C2C60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2C60).
inline void c825C2C60(Regs& regs, uint8_t* base) { regs.Apply(e825C2C60(base, regs.ToArgs())); }
// sub_825C2CC8's original.
Result e825C2CC8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C2CC8).
inline void c825C2CC8(Regs& regs, uint8_t* base) { regs.Apply(e825C2CC8(base, regs.ToArgs())); }
// sub_825C31D8's original.
Result e825C31D8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C31D8).
inline void c825C31D8(Regs& regs, uint8_t* base) { regs.Apply(e825C31D8(base, regs.ToArgs())); }
// sub_825C3548's original.
Result e825C3548(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C3548).
inline void c825C3548(Regs& regs, uint8_t* base) { regs.Apply(e825C3548(base, regs.ToArgs())); }
// sub_825C3EE0's original.
Result e825C3EE0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C3EE0).
inline void c825C3EE0(Regs& regs, uint8_t* base) { regs.Apply(e825C3EE0(base, regs.ToArgs())); }
// sub_825C4070's original.
Result e825C4070(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C4070).
inline void c825C4070(Regs& regs, uint8_t* base) { regs.Apply(e825C4070(base, regs.ToArgs())); }
// sub_825C4D90's original.
Result e825C4D90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C4D90).
inline void c825C4D90(Regs& regs, uint8_t* base) { regs.Apply(e825C4D90(base, regs.ToArgs())); }
// sub_825C4FC0's original.
Result e825C4FC0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C4FC0).
inline void c825C4FC0(Regs& regs, uint8_t* base) { regs.Apply(e825C4FC0(base, regs.ToArgs())); }
// sub_825C5758's original.
Result e825C5758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C5758).
inline void c825C5758(Regs& regs, uint8_t* base) { regs.Apply(e825C5758(base, regs.ToArgs())); }
// sub_825C5E28's original.
Result e825C5E28(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C5E28).
inline void c825C5E28(Regs& regs, uint8_t* base) { regs.Apply(e825C5E28(base, regs.ToArgs())); }
// sub_825C6040's original.
Result e825C6040(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C6040).
inline void c825C6040(Regs& regs, uint8_t* base) { regs.Apply(e825C6040(base, regs.ToArgs())); }
// sub_825C6140's original.
Result e825C6140(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C6140).
inline void c825C6140(Regs& regs, uint8_t* base) { regs.Apply(e825C6140(base, regs.ToArgs())); }
// sub_825C67E8's original.
Result e825C67E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C67E8).
inline void c825C67E8(Regs& regs, uint8_t* base) { regs.Apply(e825C67E8(base, regs.ToArgs())); }
// sub_825C6928's original.
Result e825C6928(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C6928).
inline void c825C6928(Regs& regs, uint8_t* base) { regs.Apply(e825C6928(base, regs.ToArgs())); }
// sub_825C6A20's original.
Result e825C6A20(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C6A20).
inline void c825C6A20(Regs& regs, uint8_t* base) { regs.Apply(e825C6A20(base, regs.ToArgs())); }
// sub_825C7218's original.
Result e825C7218(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7218).
inline void c825C7218(Regs& regs, uint8_t* base) { regs.Apply(e825C7218(base, regs.ToArgs())); }
// sub_825C7758's original.
Result e825C7758(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7758).
inline void c825C7758(Regs& regs, uint8_t* base) { regs.Apply(e825C7758(base, regs.ToArgs())); }
// sub_825C7798's original.
Result e825C7798(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7798).
inline void c825C7798(Regs& regs, uint8_t* base) { regs.Apply(e825C7798(base, regs.ToArgs())); }
// sub_825C7858's original.
Result e825C7858(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7858).
inline void c825C7858(Regs& regs, uint8_t* base) { regs.Apply(e825C7858(base, regs.ToArgs())); }
// sub_825C79B8's original.
Result e825C79B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C79B8).
inline void c825C79B8(Regs& regs, uint8_t* base) { regs.Apply(e825C79B8(base, regs.ToArgs())); }
// sub_825C7AA8's original.
Result e825C7AA8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7AA8).
inline void c825C7AA8(Regs& regs, uint8_t* base) { regs.Apply(e825C7AA8(base, regs.ToArgs())); }
// sub_825C7B70's original.
Result e825C7B70(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7B70).
inline void c825C7B70(Regs& regs, uint8_t* base) { regs.Apply(e825C7B70(base, regs.ToArgs())); }
// sub_825C7C90's original.
Result e825C7C90(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7C90).
inline void c825C7C90(Regs& regs, uint8_t* base) { regs.Apply(e825C7C90(base, regs.ToArgs())); }
// sub_825C7E40's original.
Result e825C7E40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7E40).
inline void c825C7E40(Regs& regs, uint8_t* base) { regs.Apply(e825C7E40(base, regs.ToArgs())); }
// sub_825C7F30's original.
Result e825C7F30(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7F30).
inline void c825C7F30(Regs& regs, uint8_t* base) { regs.Apply(e825C7F30(base, regs.ToArgs())); }
// sub_825C7F98's original.
Result e825C7F98(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C7F98).
inline void c825C7F98(Regs& regs, uint8_t* base) { regs.Apply(e825C7F98(base, regs.ToArgs())); }
// sub_825C8230's original.
Result e825C8230(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C8230).
inline void c825C8230(Regs& regs, uint8_t* base) { regs.Apply(e825C8230(base, regs.ToArgs())); }
// sub_825C8310's original.
Result e825C8310(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C8310).
inline void c825C8310(Regs& regs, uint8_t* base) { regs.Apply(e825C8310(base, regs.ToArgs())); }
// sub_825C8398's original.
Result e825C8398(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C8398).
inline void c825C8398(Regs& regs, uint8_t* base) { regs.Apply(e825C8398(base, regs.ToArgs())); }
// sub_825C8440's original.
Result e825C8440(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C8440).
inline void c825C8440(Regs& regs, uint8_t* base) { regs.Apply(e825C8440(base, regs.ToArgs())); }
// sub_825C8540's original.
Result e825C8540(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C8540).
inline void c825C8540(Regs& regs, uint8_t* base) { regs.Apply(e825C8540(base, regs.ToArgs())); }
// sub_825C8580's original.
Result e825C8580(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C8580).
inline void c825C8580(Regs& regs, uint8_t* base) { regs.Apply(e825C8580(base, regs.ToArgs())); }
// sub_825C85B8's original.
Result e825C85B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C85B8).
inline void c825C85B8(Regs& regs, uint8_t* base) { regs.Apply(e825C85B8(base, regs.ToArgs())); }
// sub_825C85E8's original.
Result e825C85E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C85E8).
inline void c825C85E8(Regs& regs, uint8_t* base) { regs.Apply(e825C85E8(base, regs.ToArgs())); }
// sub_825C8690's original.
Result e825C8690(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C8690).
inline void c825C8690(Regs& regs, uint8_t* base) { regs.Apply(e825C8690(base, regs.ToArgs())); }
// sub_825C9C40's original.
Result e825C9C40(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825C9C40).
inline void c825C9C40(Regs& regs, uint8_t* base) { regs.Apply(e825C9C40(base, regs.ToArgs())); }
// sub_825CA448's original.
Result e825CA448(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CA448).
inline void c825CA448(Regs& regs, uint8_t* base) { regs.Apply(e825CA448(base, regs.ToArgs())); }
// sub_825CA980's original.
Result e825CA980(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CA980).
inline void c825CA980(Regs& regs, uint8_t* base) { regs.Apply(e825CA980(base, regs.ToArgs())); }
// sub_825CB470's original.
Result e825CB470(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CB470).
inline void c825CB470(Regs& regs, uint8_t* base) { regs.Apply(e825CB470(base, regs.ToArgs())); }
// sub_825CB5E8's original.
Result e825CB5E8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CB5E8).
inline void c825CB5E8(Regs& regs, uint8_t* base) { regs.Apply(e825CB5E8(base, regs.ToArgs())); }
// sub_825CC0A0's original.
Result e825CC0A0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CC0A0).
inline void c825CC0A0(Regs& regs, uint8_t* base) { regs.Apply(e825CC0A0(base, regs.ToArgs())); }
// sub_825CC0D0's original.
Result e825CC0D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CC0D0).
inline void c825CC0D0(Regs& regs, uint8_t* base) { regs.Apply(e825CC0D0(base, regs.ToArgs())); }
// sub_825CC1D0's original.
Result e825CC1D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CC1D0).
inline void c825CC1D0(Regs& regs, uint8_t* base) { regs.Apply(e825CC1D0(base, regs.ToArgs())); }
// sub_825CC320's original.
Result e825CC320(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CC320).
inline void c825CC320(Regs& regs, uint8_t* base) { regs.Apply(e825CC320(base, regs.ToArgs())); }
// sub_825CC380's original.
Result e825CC380(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CC380).
inline void c825CC380(Regs& regs, uint8_t* base) { regs.Apply(e825CC380(base, regs.ToArgs())); }
// sub_825CC9C8's original.
Result e825CC9C8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825CC9C8).
inline void c825CC9C8(Regs& regs, uint8_t* base) { regs.Apply(e825CC9C8(base, regs.ToArgs())); }
// sub_825D0268's original.
Result e825D0268(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D0268).
inline void c825D0268(Regs& regs, uint8_t* base) { regs.Apply(e825D0268(base, regs.ToArgs())); }
// sub_825D0428's original.
Result e825D0428(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D0428).
inline void c825D0428(Regs& regs, uint8_t* base) { regs.Apply(e825D0428(base, regs.ToArgs())); }
// sub_825D06B8's original.
Result e825D06B8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D06B8).
inline void c825D06B8(Regs& regs, uint8_t* base) { regs.Apply(e825D06B8(base, regs.ToArgs())); }
// sub_825D0800's original.
Result e825D0800(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D0800).
inline void c825D0800(Regs& regs, uint8_t* base) { regs.Apply(e825D0800(base, regs.ToArgs())); }
// sub_825D0978's original.
Result e825D0978(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D0978).
inline void c825D0978(Regs& regs, uint8_t* base) { regs.Apply(e825D0978(base, regs.ToArgs())); }
// sub_825D09D0's original.
Result e825D09D0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D09D0).
inline void c825D09D0(Regs& regs, uint8_t* base) { regs.Apply(e825D09D0(base, regs.ToArgs())); }
// sub_825D0AA0's original.
Result e825D0AA0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D0AA0).
inline void c825D0AA0(Regs& regs, uint8_t* base) { regs.Apply(e825D0AA0(base, regs.ToArgs())); }
// sub_825D0BF8's original.
Result e825D0BF8(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D0BF8).
inline void c825D0BF8(Regs& regs, uint8_t* base) { regs.Apply(e825D0BF8(base, regs.ToArgs())); }
// sub_825D0F00's original.
Result e825D0F00(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D0F00).
inline void c825D0F00(Regs& regs, uint8_t* base) { regs.Apply(e825D0F00(base, regs.ToArgs())); }
// sub_825D2170's original.
Result e825D2170(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D2170).
inline void c825D2170(Regs& regs, uint8_t* base) { regs.Apply(e825D2170(base, regs.ToArgs())); }
// sub_825D3820's original.
Result e825D3820(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D3820).
inline void c825D3820(Regs& regs, uint8_t* base) { regs.Apply(e825D3820(base, regs.ToArgs())); }
// sub_825D5E58's original.
Result e825D5E58(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D5E58).
inline void c825D5E58(Regs& regs, uint8_t* base) { regs.Apply(e825D5E58(base, regs.ToArgs())); }
// sub_825D8660's original.
Result e825D8660(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D8660).
inline void c825D8660(Regs& regs, uint8_t* base) { regs.Apply(e825D8660(base, regs.ToArgs())); }
// sub_825D8F88's original.
Result e825D8F88(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D8F88).
inline void c825D8F88(Regs& regs, uint8_t* base) { regs.Apply(e825D8F88(base, regs.ToArgs())); }
// sub_825D9060's original.
Result e825D9060(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D9060).
inline void c825D9060(Regs& regs, uint8_t* base) { regs.Apply(e825D9060(base, regs.ToArgs())); }
// sub_825D9160's original.
Result e825D9160(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D9160).
inline void c825D9160(Regs& regs, uint8_t* base) { regs.Apply(e825D9160(base, regs.ToArgs())); }
// sub_825D91B0's original.
Result e825D91B0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D91B0).
inline void c825D91B0(Regs& regs, uint8_t* base) { regs.Apply(e825D91B0(base, regs.ToArgs())); }
// sub_825D9518's original.
Result e825D9518(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D9518).
inline void c825D9518(Regs& regs, uint8_t* base) { regs.Apply(e825D9518(base, regs.ToArgs())); }
// sub_825D9590's original.
Result e825D9590(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D9590).
inline void c825D9590(Regs& regs, uint8_t* base) { regs.Apply(e825D9590(base, regs.ToArgs())); }
// sub_825D9E60's original.
Result e825D9E60(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D9E60).
inline void c825D9E60(Regs& regs, uint8_t* base) { regs.Apply(e825D9E60(base, regs.ToArgs())); }
// sub_825D9EF0's original.
Result e825D9EF0(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D9EF0).
inline void c825D9EF0(Regs& regs, uint8_t* base) { regs.Apply(e825D9EF0(base, regs.ToArgs())); }
// sub_825D9F68's original.
Result e825D9F68(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825D9F68).
inline void c825D9F68(Regs& regs, uint8_t* base) { regs.Apply(e825D9F68(base, regs.ToArgs())); }
// sub_825DA540's original.
Result e825DA540(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825DA540).
inline void c825DA540(Regs& regs, uint8_t* base) { regs.Apply(e825DA540(base, regs.ToArgs())); }
// sub_825DA580's original.
Result e825DA580(uint8_t* base, const Args& a);
// The same on a register view (in place of __imp__sub_825DA580).
inline void c825DA580(Regs& regs, uint8_t* base) { regs.Apply(e825DA580(base, regs.ToArgs())); }

}  // namespace lifted::original
