export const meta = {
  name: 'e5-step9-swarm',
  description: 'E5 step 9 as one swarm: one agent per unit of hand ports finds code that assumes the old flat guest mapping and converts it to guest.h calls (edits only; the lead builds and checks after)',
  phases: [
    { title: 'Audit', detail: 'one agent per unit; units sharing files never run side by side' },
  ],
}

// args.units: [{ name, files }] (docs/loops/e5-loop.md, "Step 9 as one
// swarm"): `files` the directories and files of src/ the unit reads (the
// scheduler's: two units sharing a file never run at once).
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
- Never build (no cmake, no ninja), never run the game, never run git commands that change anything (read-only git such as git show and git log is fine), never run tools/lift.py, tools/lift_originals.py, tools/type_map.py or tools/schema_gen.py.
- Edit only your unit's files (below). Never edit itc_recomp/src/guest.h, itc_recomp/src/guest_code.h, itc_recomp/src/memory/, itc_recomp/src/lifted/, itc_recomp/src/schema/, itc_recomp/src/image/, itc_recomp/tools/, or docs/functions/ rows (propose row changes in your report: the user reviews rows before they're written). itc_recomp/src/harnesses/ isn't built: leave it alone. If a fix needs a change outside your files, describe it in for_lead instead.
- American English. Nothing the player can see may change: the same game, tick for tick, on the same input (the lead checks with tools/tick_replay.sh). Keep the same loads and stores in the same order, the same bytes.`

const HOW = `Read first: docs/loops/e5-loop.md, "Step 9 as one swarm" (and "Step 7 as one swarm" before it); the groundwork commit (git show 95c5f19f): itc_recomp/src/guest.h (memory::BlockPointer, guest::Pointer, the Load/Store functions, CopyU32s, StoreU32s, ForEachHostPart) and itc_recomp/src/memory/mapping.cpp.
What changed: until now, guest memory was one flat 4 GB mapping at \`base\`, so host address = base + guest address held for every address. Now it doesn't: the heap (guest 0x10000 + 384 MiB), the image (0x82000000 + 0x9B0000) and the physical memory (0x7F000000, 0xA0000000 and up) are separate host blocks; the stacks, blobs and host objects are elsewhere behind handles (0x60000000-0x7EFFFFFF). Only guest.h's functions translate. \`base\` (and memory::Base()) is now a range with nothing mapped: anything computing a host address from it faults.
What to find, in your unit's code (read it: grep finds the obvious forms, but the base may be held under another name, passed into a helper, or offset on another line):
1. A host address made from base or memory::Base() plus a guest address: base + x, base[x], &base[x], reinterpret_cast<T*>(base + x), memcpy/memset/memcmp/std::copy on base + x, a pointer to base kept in a member or a global (mem, membase, guest_base, m_base, ...) and offset later.
2. A guest address recovered from a host pointer: p - base, uintptr_t arithmetic against base.
3. A host pointer from guest::Pointer(base, a) (or a blob's or handle's bytes) used to reach a different guest address that may be in another block or object: guest::Pointer(base, 0) + x, guest::Pointer(base, a) + (b - a) for an unrelated b, a loop walking a host pointer across what were separate objects or across a block's end.
4. One guest::Pointer span covering a range that leaves its block (for example a scan from 0x80000000 on, or a copy spanning the heap and the image).
5. Address checks that assume the old layout and then make host pointers from it (the 0x90000000 alias of the image, "below 0x40000000 is the 4 KB heap" and the like). Plain comparisons of guest addresses that never become host pointers are fine.
What to do with each: convert it to guest.h's calls (guest::Pointer per address, guest::LoadU32/StoreU32 and the like, CopyU32s, StoreU32s, ForEachHostPart), the same bytes in the same order. guest::Pointer(base, a) is fine for a span inside one object or block. Don't change anything that already goes through guest.h, and don't restyle: only sites that would fault or read the wrong memory now. Comments and strings that merely mention base + address need no change, unless they describe what the code does and are now wrong. If your unit has no such site, say so: that's a fine result.`

const REPORT = {
  type: 'object',
  properties: {
    unit: { type: 'string' },
    changed: {
      type: 'array',
      items: {
        type: 'object',
        properties: {
          file: { type: 'string' },
          line: { type: 'integer' },
          function: { type: 'string', description: 'the function (game name or sub_X) the site is in' },
          before: { type: 'string', description: 'the code before (short)' },
          after: { type: 'string', description: 'the code after (short)' },
          reached_in_play: { type: 'string', description: 'whether the attract race or the pad replay would reach it, if you can tell, or "unknown"' },
        },
        required: ['file', 'line', 'function', 'before', 'after', 'reached_in_play'],
      },
    },
    left: {
      type: 'array',
      description: 'sites you found but couldn\'t fix inside your files, or are unsure of',
      items: {
        type: 'object',
        properties: { file: { type: 'string' }, line: { type: 'integer' }, why: { type: 'string' } },
        required: ['file', 'line', 'why'],
      },
    },
    how_searched: { type: 'string', description: 'what you grepped for and what you read (brief), so the lead can judge coverage' },
    row_changes: { type: 'string', description: 'docs/functions rows you propose changing, in the table format (four cells, exactly as written), or "none"' },
    for_lead: { type: 'string', description: 'changes needed in files you left alone, risks, what to look at in the build or the replay (brief), or "none"' },
  },
  required: ['unit', 'changed', 'left', 'how_searched', 'row_changes', 'for_lead'],
}

phase('Audit')
const results = await schedule(UNITS, unit => agent(
`You're one of several agents doing E5 step 9 (the heap and the image out of the guest mapping) as one swarm, each for one unit of this game port's hand-written code. Your unit: \`${unit.name}\`.
- Your files (directories are all their .cpp and .h files; held for you while you run): ${unit.files.join(', ')}.
${RULES}
${HOW}

Your work: read your unit's code for every site of the five kinds above and convert each. Then report: each site changed (file, line, function, before and after), sites left and why, how you searched, proposed row changes, and notes for the lead.`,
  { label: `audit:${unit.name}`, phase: 'Audit', schema: REPORT }))

const reports = results.filter(Boolean)
log(`${reports.length} of ${UNITS.length} units reported; ${reports.reduce((n, r) => n + r.changed.length, 0)} sites changed, ${reports.reduce((n, r) => n + r.left.length, 0)} left`)
return {
  units: reports.length,
  missing: UNITS.filter((u, k) => !results[k]).map(u => u.name),
  reports,
}
