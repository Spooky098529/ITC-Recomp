// The lift runtime's imports (docs/loops/lift-sweep.md, "The pieces", 3):
// the game's calls to kernel and XAM imports, answered by our code
// (src/imports/native_imports.h) since E1 and E2. The SDK's context and
// its functions are gone since E4 (docs/rexglue-removal.md, "E4: the
// plan", step 3).

#pragma once

#include "lifted/imports.h"
#include "lifted/lifted.h"
#include "lifted/regs.h"

namespace lifted {

// Runs an import with a call's arguments: our answer. An import with none
// (none is reached in play) logs its name and the caller and stops the
// program, as a call to an address with no dispatch table entry does.
Result Import(ImportFunction* function, const Args& args);

// Where our code once fell back to the SDK's import on objects ours don't
// handle: logs the import's name and stops the program (none is reached in
// play).
[[noreturn]] Result ImportFromSdk(ImportFunction* function, const Args& args);

// The import's name, for diagnostics.
inline const char* ImportName(ImportFunction* function) { return function->name; }

// --log_imports (src/lifted/import_log.cpp, E1's measure in
// docs/rexglue-removal.md): notes one import call, its caller's return
// address and thread (r13).
void NoteImport(ImportFunction* function, const Args& args);
bool LoggingImports();

// An import's call site in lifted code (E5 step 6): our answer found at its
// first call, then called straight, with no lookup; with --log_imports on,
// through Import as before (the log notes each call).
using ImportAnswer = Result (*)(const Args& args);
ImportAnswer FindImportAnswer(ImportFunction* function);
template <ImportFunction* F>
inline Result ImportAt(const Args& args) {
  static const ImportAnswer answer = FindImportAnswer(F);
  if (answer && !LoggingImports()) [[likely]] {
    return answer(args);
  }
  return Import(F, args);
}

// An import with a register view's registers; its r3 and f1 back in them.
void Sdk(Regs& regs, ImportFunction* function);

// A function on a register view (a lifted original's Regs form,
// lifted/originals.h), run as the call sites that once went through the
// SDK's boundary did.
using RegsFunction = void(Regs& regs);
inline void Sdk(Regs& regs, RegsFunction* function) { function(regs); }

}  // namespace lifted
