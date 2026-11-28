export const meta = {
  name: 'missing-functions-shims',
  description: 'Write ITC_DIRECT_ENTRY shims for ~380 hand ports the fallback originals call by address, then review each unit',
  phases: [
    { title: 'Write', detail: 'one agent per unit of areas writes the shims' },
    { title: 'Review', detail: 'one agent per unit checks each shim against the port and the original' },
  ],
}

const BRIEF = `You are one unit of a swarm on branch missing-functions (repo: the current one; work from itc_recomp/).
Read docs/missing-functions.md first (the problem and the mechanism).

The task: hand ports (plain C++ in src/<area>/) lost their dispatch entries when their adapters were deleted, and
the fallback originals (src/lifted/originals.cpp, generated, ~442,000 lines) still call them by constant address.
For each target address you're given, write a shim with the macro ITC_DIRECT_ENTRY(address) (src/hooks/native_entry.h).
Its body runs on the register view lifted::Regs named ctx (r1, r3..r10, r13 and f1..f13, as Temp with .u32/.u64/.s32/.s64/.f64;
caller_r31), calls the hand port, and writes the result to ctx.r3.u64 (integers/bools/pointers, zero- or sign-extended as the
original leaves r3) or ctx.f1.f64 (float/double results). Done examples, with an existing register step:
  src/hooks/car_dealer.cpp: ITC_DIRECT_ENTRY(821824A8) { hooks::CarDealerSetSpecTexts(ctx); }
and steps like ITC_REGS_STEP(CarDealerSetSpecTexts) there show how an adapter maps registers and builds the area's Calls.
The macro defines lifted::entry::e<address>; it's NOT registered in the dispatch table (on purpose).

How to get each shim right:
- The port: find it by ITC_NATIVE_RUNNING("sub_<address> ...") (port_file, port_name, signature given; verify).
- If a register step (ITC_REGS_STEP) already calls this port for this address, the shim just calls the step. The
  candidate_steps list is a name match and can be wrong (shared names like Init, SetSpeed): check the step really wraps
  this port.
- Otherwise map the registers yourself, from: the function's row in docs/functions/*.md (grep sub_<address>; "r3 = ...,
  returns ..."), the original in itc_recomp/generated/default/*.cpp (grep "DEFINE_REX_FUNC(sub_<address>)": which argument
  registers it reads, what it leaves in r3/f1; read only, never edit generated/), the deleted adapter in git history
  (git log -S "<address>" -- itc_recomp/src/hooks, then git show), and how native callers call the port.
- An sp/stack/frame parameter is ctx.r1.u32. A parameter like caller_r24 (a caller's nonvolatile register the original
  only saves) has no source in Regs: pass 0 and say so in the note.
- A Calls struct (the area's callees, usually a function pointer plus user data doing guest calls): build it the way the
  area's remaining adapters/steps do (often a GuestCalls class over ctx in the area's hooks file, or calls through
  lifted::Call / lifted::CallFromContext / the callee's lifted::entry::e or lifted::f functions). Reuse what exists.
- A port that can decline (returns std::optional, or a bool meaning "not handled"): on decline call
  lifted::original::c<address>(ctx) and list the address in needs_original (the lead lifts those originals afterwards;
  until then that name won't exist, so the syntax check of that line fails: that's expected, say so).
- A port that's static or in an unnamed namespace: declare it in its area's header (or move it out of the unnamed
  namespace) with the smallest change. Don't change any port's behavior.
- Multiple-register results, out-parameters through memory, etc.: follow the original exactly.

This round (2026-11-28, the stale originals): every target here ALREADY has a lifted original registered as a fallback
(lifted::original::c<address> exists, so decline fallbacks compile). The lift.py batches call these hand ports by address,
and that runs the stale original instead of the port. E5 moved some objects' data to host memory, so an original can read
the wrong place (a camera stop: lifted CameraAnim::EvalCamera reading blocks by guest pointer arithmetic). The shim makes
lifted code call the PORT. Map registers exactly as before; there's no needs_original work in this round.

Where to write:
- In the hooks file of one of YOUR areas (src/hooks/<area>.cpp, the area's own adapters) when the shim needs something
  private there (a step, a GuestCalls class). Put shims after the area's existing entries with a one-line comment
  "For the fallback originals (docs/missing-functions.md)".
- Otherwise in src/hooks/direct/unit_<N>.cpp (already in the build; add the includes you need).
- Edit only: your areas' hooks files, your areas' own src/<area>/ headers and sources (only to expose a port), and your
  unit file. Nothing else: not originals.cpp, entries.h, names.h, CMakeLists, generated/, other areas. Other agents are
  editing other files at the same time.
- No cmake/ninja builds (the build folder is shared). Syntax-check each .cpp you changed with
  "tools/syntax_check.sh src/hooks/<file>.cpp" (from itc_recomp/; ignore the "-c unused" warning). Fix all errors except
  expected lifted::original::c<address> ones.
- No commits, no git checkout/stash/reset. American English in comments. No absolute /home paths in files.
- A target you truly can't do (no port found, unclear mapping): skip it, status "skipped" with the reason.`

const WRITE_SCHEMA = {
  type: 'object',
  properties: {
    results: { type: 'array', items: { type: 'object', properties: {
      addr: { type: 'string' }, status: { type: 'string', enum: ['done', 'skipped'] },
      file: { type: 'string' }, note: { type: 'string' } }, required: ['addr', 'status', 'note'] } },
    needs_original: { type: 'array', items: { type: 'string' } },
    files_changed: { type: 'array', items: { type: 'string' } },
    syntax_ok: { type: 'boolean' },
    summary: { type: 'string' },
  },
  required: ['results', 'needs_original', 'files_changed', 'syntax_ok', 'summary'],
}

const REVIEW_SCHEMA = {
  type: 'object',
  properties: {
    fixed: { type: 'array', items: { type: 'object', properties: {
      addr: { type: 'string' }, problem: { type: 'string' } }, required: ['addr', 'problem'] } },
    still_doubtful: { type: 'array', items: { type: 'object', properties: {
      addr: { type: 'string' }, reason: { type: 'string' } }, required: ['addr', 'reason'] } },
    needs_original: { type: 'array', items: { type: 'string' } },
    files_changed: { type: 'array', items: { type: 'string' } },
    syntax_ok: { type: 'boolean' },
    summary: { type: 'string' },
  },
  required: ['fixed', 'still_doubtful', 'needs_original', 'files_changed', 'syntax_ok', 'summary'],
}

const results = await pipeline(
  args,
  u => agent(`${BRIEF}

You are unit ${u.unit}. Your areas: ${u.areas.join(', ')}. Your unit file: src/hooks/direct/unit_${u.unit}.cpp.
Existing hooks files of your areas: ${u.hooks_files_existing.join(', ') || '(none with the same name; look for where the area\'s adapters live, and use your unit file if that file isn\'t clearly your area\'s alone)'}.
Your ${u.count} targets: the entry with "unit": ${u.unit} in out/swarm/units.json (its "targets" list; read it first).

Return one result per target.`, { label: `write:unit${u.unit}`, phase: 'Write', schema: WRITE_SCHEMA }),
  (w, u) => agent(`${BRIEF}

You are the REVIEWER of unit ${u.unit} (areas: ${u.areas.join(', ')}). Another agent just wrote the shims; its report:
${JSON.stringify(w, null, 1)}

Check every ITC_DIRECT_ENTRY shim it wrote (in ${(w && w.files_changed || []).join(', ') || 'its files'}) against the
port's signature, the docs/functions row and the original in generated/: each argument from the right register with the
right width and signedness, floats from f registers, the frame from r1, the result in r3 or f1 as the original leaves it,
the Calls built correctly, a decline falling back to lifted::original::c<address>. Also check the skipped targets: do
the ones you can. Fix what's wrong directly (same file rules), then syntax-check every changed file.
Targets of this unit: the entry with "unit": ${u.unit} in out/swarm/units.json.`,
    { label: `review:unit${u.unit}`, phase: 'Review', schema: REVIEW_SCHEMA })
    .then(r => ({ unit: u.unit, write: w, review: r })),
)
return results
