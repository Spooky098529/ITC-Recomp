#include "rexglue-sdk/include/rex/ppc/func.h"

// Custom override of the vertex buffer upload function for the cockpit needle.
// This function replaces the original implementation to add interpolation of the
// vertex data before it is copied to the GPU vertex buffer.
extern "C" REX_FUNC(__imp__sub_8258CAA0) {
    REX_FUNC_PROLOGUE();
    // TODO: Interpolate the needle vertex buffer before uploading.
    // The original body from the decompiled code follows.

    // mflr r12
    ctx.r12.u64 = ctx.lr;
    // stw r12,-8(r1)
    REX_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
    // stwu r1,-96(r1)
    uint32_t ea = -96 + ctx.r1.u32;
    REX_STORE_U32(ea, ctx.r1.u32);
    ctx.r1.u32 = ea;
    // mr r11,r3
    ctx.r11.u64 = ctx.r3.u64;
    // lwz r10,12(r11)
    ctx.r10.u64 = REX_LOAD_U32(ctx.r11.u32 + 12);
    // lwz r9,8(r11)
    ctx.r9.u64 = REX_LOAD_U32(ctx.r11.u32 + 8);
    // add r10,r4,r10
    ctx.r10.u64 = ctx.r4.u64 + ctx.r10.u64;
    // cmplw cr6,r10,r9
    ctx.cr6.compare<uint32_t>(ctx.r10.u32, ctx.r9.u32, ctx.xer);
    // bge cr6,0x8258cae8
    if (!ctx.cr6.lt) goto loc_8258CAE8;
    // lwz r3,44(r11)
    ctx.r3.u64 = REX_LOAD_U32(ctx.r11.u32 + 44);
    // rlwinm r9,r4,4,0,27
    ctx.r9.u64 = __builtin_rotateleft64(ctx.r4.u32 | (ctx.r4.u64 << 32), 4) & 0xFFFFFFF0;
    // stw r10,12(r11)
    REX_STORE_U32(ctx.r11.u32 + 12, ctx.r10.u32);
    // add r10,r9,r3
    ctx.r10.u64 = ctx.r9.u64 + ctx.r3.u64;
    // stw r10,44(r11)
    REX_STORE_U32(ctx.r11.u32 + 44, ctx.r10.u32);
    // addi r1,r1,96
    ctx.r1.s64 = ctx.r1.s64 + 96;
    // lwz r12,-8(r1)
    ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
    // mtlr r12
    ctx.lr = ctx.r12.u64;
    // blr
    return;

loc_8258CAE8:
    // lis r11,-32249
    ctx.r11.s64 = -2113470464;
    // addi r3,r11,-16216
    ctx.r3.s64 = ctx.r11.s64 + -16216;
    // bl 0x825a1318
    ctx.lr = 0x8258CAF4;
    sub_825A1318(ctx, base);
    // li r3,0
    ctx.r3.s64 = 0;
    // addi r1,r1,96
    ctx.r1.s64 = ctx.r1.s64 + 96;
    // lwz r12,-8(r1)
    ctx.r12.u64 = REX_LOAD_U32(ctx.r1.u32 + -8);
    // mtlr r12
    ctx.lr = ctx.r12.u64;
    // blr
    return;
}
