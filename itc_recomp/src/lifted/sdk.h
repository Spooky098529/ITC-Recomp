// The lift sweep's boundary with the SDK (docs/loops/lift-sweep.md, "The
// pieces", 3): the only lifted code that touches the SDK's context.
//
// Import runs an SDK import (a kernel or XAM function) with a call's
// arguments. Enter runs a lifted function's entry for the SDK, or for
// code that isn't native yet, from the SDK's context; while it runs, the
// thread's context is the one calls through the boundary use.

#pragma once

#include "lifted/imports.h"
#include "lifted/lifted.h"
#include "lifted/regs.h"

namespace lifted {

// An SDK import, as the generated code declares it.
using ImportFunction = void(PPCContext&, uint8_t*);

Result Import(uint8_t* base, ImportFunction* function, const Args& args);

// The SDK's import itself, past ours (src/kernel/native_imports.h): for our
// code falling back to it on objects it doesn't handle.
Result ImportFromSdk(uint8_t* base, ImportFunction* function, const Args& args);

// The import's name (written by itc_recomp/tools/import_entries.py into
// imports.cpp), for diagnostics.
const char* ImportName(ImportFunction* function);

// --log_imports (src/lifted/import_log.cpp, E1's measure in
// docs/rexglue-removal.md): notes one import call, its caller's return
// address and thread (r13).
void NoteImport(ImportFunction* function, const Args& args);
bool LoggingImports();

// Runs `entry` with the context's argument registers and puts its r3 and
// f1 back.
void Enter(PPCContext& ctx, uint8_t* base, Entry entry);

// A call by address from code that holds the SDK's context (a hand port's
// adapter or its `Calls`), with the context's registers: the entry in the
// dispatch table when there is one (as the function's own REX_FUNC entry
// does, through Enter), else the function the SDK has at the address (a
// hand port's override, not in the table yet) on the same context, as the
// generated code's indirect calls do. The only place outside the
// harnesses that reaches the SDK's function table (docs/rexglue-removal.md,
// "Before E1: the rewiring", stage 2).
void CallFromContext(PPCContext& ctx, uint8_t* base, uint32_t address);

// An SDK function (an import, or a recompiled original `__imp__sub_X`) with
// a register view's registers, on the thread's context (as Import); its r3
// and f1 back in them.
void Sdk(Regs& regs, uint8_t* base, ImportFunction* function);

// A register view of the context for code still entered with it (a
// harness, an adapter): made from the context's registers, the context the
// one calls through the boundary use while it lives, and its registers put
// back in the context at the end (the float mode synced).
class RegsScope {
 public:
  explicit RegsScope(PPCContext& ctx);
  ~RegsScope();
  RegsScope(const RegsScope&) = delete;
  RegsScope& operator=(const RegsScope&) = delete;

  Regs regs;

 private:
  PPCContext& ctx_;
  PPCContext* outer_;
};

}  // namespace lifted
