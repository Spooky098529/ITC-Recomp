export const meta = {
  name: 'e5-step6-swarm',
  description: 'E5 step 6 as one swarm: one agent per group of areas takes their hand ports off guest frames, kernel import calls and needless float mode switches (edits only; the lead builds and checks after)',
  phases: [
    { title: 'Move', detail: 'one agent per group of areas; units sharing files never run side by side' },
  ],
}

// args.units: [{ name, areas, files }] (docs/loops/e5-loop.md, "Step 6 as
// one swarm"): `areas` the src/ folders, `files` their hand-port files with
// guest frames, import calls or float mode switches (the scheduler's: two
// units sharing a file never run at once).
const UNITS = args.units

// At most 6 agents at once (the user's call), and never two units sharing a
// file: a unit starts when a slot is free and none of its files is held.
const CONCURRENCY = 6
async function schedule(items, fn) {
  const out = new Array(items.length).fill(null)
  const held = new Set()
  const waiting = items.map((_, k) => k)
  let running = 0
  return await new Promise(resolve => {
    function pump() {
      if (!waiting.length && !running) { resolve(out); return }
      for (let i = 0; i < waiting.length && running < CONCURRENCY; ) {
        const k = waiting[i]
        if (items[k].files.some(f => held.has(f))) { i++; continue }
        waiting.splice(i, 1)
        items[k].files.forEach(f => held.add(f))
        running++
        Promise.resolve().then(() => fn(items[k], k)).catch(() => null).then(r => {
          out[k] = r || null
          items[k].files.forEach(f => held.delete(f))
          running--
          pump()
        })
      }
    }
    pump()
  })
}

const RULES = `
The repo root is where CLAUDE.md is; branch e5-native-16-11-26 (stay on it). Several agents edit this working tree at once, one per unit, so these rules are strict:
- Never build (no cmake, no ninja), never run the game, never run git commands that change anything (read-only git such as git show and git log is fine), never run tools/lift.py, tools/lift_originals.py, tools/type_map.py or the full tools/schema_gen.py. Only: python3 -I itc_recomp/tools/schema_gen.py --area <area>, python3 -I itc_recomp/tools/schema_check.py <area>, python3 -I itc_recomp/tools/e5_movable.py <area>, and read-only commands.
- Never hand-edit itc_recomp/src/schema/ or itc_recomp/src/lifted/ (generated), itc_recomp/tools/, or docs/functions/ rows (propose row changes in your report: the user reviews rows before they're written). itc_recomp/src/harnesses/ isn't built any more: leave it alone.
- Your unit's files (below) are yours for this run: no other agent holds them. Another file may be edited by another agent at the same time: edit one only when your unit needs it, keep the edit small, and if the Edit tool says the file changed since you read it, read it again and redo the edit. Touch only your unit's types' schema entries.
- American English. Nothing the player can see may change: the same game, tick for tick, on the same input (the lead checks with the state digest, tools/tick_replay.sh). Keep the same loads and stores in the same order; where an object lives and how it's allocated may change (the user's call, docs/e5-native.md "Decided").`

const HOW = `Read first: docs/e5-native.md, "6. Control flow and threads"; docs/loops/e5-loop.md, "Step 6 as one swarm" (what's done: every thread's stack, thread object, per-thread block and TLS slots are blobs in PC memory; guest::Load*/Store* and guest::Pointer reach them, the calling thread's stack first); the groundwork commit (git show db5a8a0d). Background on the views and blobs: "Step 4 as one swarm" and "Step 5 as one swarm" in the same file.
The three things to do in your areas' hand ports (itc_recomp/src/<area>/ and itc_recomp/src/hooks/<area>.cpp):
1. Guest frames. Hand ports make frames on the guest stack (const uint32_t frame = stack - N, sp = stack - N, guest::StackFrame) to mirror the original's locals. A slot that only the port itself reads and writes (never handed to other code by address: not passed to a lifted or guest call, a calls.Call / std::function callback that reaches guest code, an import, or stored where other code finds it) becomes a C++ local (a value, an array, a host struct), with the same arithmetic in the same order (float math stays float or double as before; the vector library's host forms where they exist, else the same component math). A slot handed to other code by address stays on the stack (it's PC memory now): leave it, and keep the frame only as large as those slots need, at the same offsets (other code may rely on them). When the frame has no slots left, drop it, and drop the back-chain store (Store(base, frame, stack)) only when nothing calls guest code with that frame as r1 (a guest callee's frame is made below the r1 it's given: passing stack instead of frame is fine, as long as the callee doesn't read the caller's slots).
2. Kernel import calls in the hand ports (lifted::Import, lifted::ImportFromSdk, hooks:: wrappers of VirtualAlloc/VirtualFree/critical sections/events, imports::): where our answer is a plain host operation (itc_recomp/src/threads/, src/memory/memory_imports.cpp, src/imports/: read the answer), call it directly (the plain C++ function behind it, or a host primitive: a std::mutex, our event) instead of going through the import's lookup; keep the arguments and results the game sees. Don't edit src/threads/, src/imports/ or src/memory/ (the lead's); if a direct call needs a function they don't export, say so in your report.
3. Float modes: lifted::ScalarFloatMode / VectorFloatMode calls (the MXCSR switches the original made) in your hand ports: keep them where a result depends on flush-to-zero (vector math on denormals), drop them where the code that follows does no float math that could differ; when unsure, keep.
Rules for all three: the same game tick for tick (the lead checks with tick_replay.sh), the same loads and stores the game's other code sees, in the same order. Don't touch lifted code, the schema, the tools or another unit's files.`

const REPORT = {
  type: 'object',
  properties: {
    unit: { type: 'string' },
    moved: {
      type: 'array',
      items: {
        type: 'object',
        properties: {
          type: { type: 'string', description: 'the function (address and name)' },
          storage: { type: 'string', description: 'what changed: frame to locals, frame kept (why), import direct, float mode dropped or kept' },
          alloc_sites: { type: 'string', description: 'the slots or calls changed and how' },
          free_sites: { type: 'string', description: 'what stays on the guest stack and why, or "none"' },
          files: { type: 'array', items: { type: 'string' } },
        },
        required: ['type', 'storage', 'alloc_sites', 'free_sites', 'files'],
      },
    },
    skipped: {
      type: 'array',
      items: {
        type: 'object',
        properties: { type: { type: 'string' }, why: { type: 'string' } },
        required: ['type', 'why'],
      },
    },
    raw_left: { type: 'string', description: 'raw accesses to moved types you know remain (lifted functions, other files), which the runtime will log, or "none known"' },
    lifted_concerns: { type: 'string', description: 'lifted functions that reach a moved type in ways the tools may not handle (an address passed to an import, guest::Pointer, vector loads) and why you judged them safe, or "none"' },
    row_changes: { type: 'string', description: 'docs/functions rows you propose changing (status Host where every object a function touches is now host), in the table format, or "none"' },
    for_lead: { type: 'string', description: 'changes needed in files you left alone, risks, what to look at in the build or the replay (brief)' },
  },
  required: ['unit', 'moved', 'skipped', 'raw_left', 'lifted_concerns', 'row_changes', 'for_lead'],
}

phase('Move')
const results = await schedule(UNITS, unit => agent(
`You're one of several agents doing E5 step 6 (control flow and threads: the hand ports off guest frames, kernel import lookups and needless float mode switches) as one swarm, each for a group of areas of this game port. Your areas: ${unit.areas.join(', ')}.
- Your files (held for you while you run): ${unit.files.join(', ')}.
${RULES}
${HOW}

Your work: go through your files' functions with guest frames, import calls or float mode switches, read each (its row in docs/functions/ and the code), and change what can change. Then report: what changed per function (moved: one entry per function changed), what stays and why (skipped), the lifted code you judged (callees reading the caller's frame), proposed row changes (the rows' notes that describe frames, imports or float modes), and notes for the lead.`,
  { label: `move:${unit.name}`, phase: 'Move', schema: REPORT }))

const reports = results.filter(Boolean)
log(`${reports.length} of ${UNITS.length} units reported; ${reports.reduce((n, r) => n + r.moved.length, 0)} types moved`)
return {
  units: reports.length,
  missing: UNITS.filter((u, k) => !results[k]).map(u => u.name),
  reports,
}
