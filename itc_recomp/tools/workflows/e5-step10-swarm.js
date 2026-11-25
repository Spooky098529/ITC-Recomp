export const meta = {
  name: 'e5-step10-swarm',
  description: 'E5 step 10 as one swarm: one agent per unit of files fixes what the codemod dropping `base` left the compiler rejecting (edits only; the lead builds and checks after)',
  phases: [
    { title: 'Fix', detail: 'one agent per unit; units sharing files never run side by side' },
  ],
}

// args.units: [{ name, files, errors }] (docs/loops/e5-loop.md, "Step 10 as
// one swarm"): `files` the source files the unit owns (the scheduler's: two
// units sharing a file never run at once), `errors` the compiler's error
// lines for them, as the lead's build printed them.
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
- Never build (no cmake, no ninja, no compiler runs), never run the game, never run git commands that change anything (read-only git such as git show, git diff and git log is fine), never run tools/lift.py, tools/lift_originals.py, tools/schema_gen.py or tools/type_map.py.
- Edit only your unit's files (below). Never edit itc_recomp/src/guest.h, itc_recomp/src/guest_code.h, itc_recomp/src/memory/, itc_recomp/src/lifted/, itc_recomp/src/schema/, itc_recomp/src/image/, itc_recomp/tools/ or docs/functions/. If a fix needs a change outside your files (a header another unit owns, a shared declaration), describe it in for_lead instead.
- American English. Nothing the player can see may change: the same game, tick for tick, on the same input. Keep the same loads and stores in the same order.`

const HOW = `Read first: docs/loops/e5-loop.md, "Step 10 as one swarm"; the WIP commit (git show d0383928 --stat, and git show d0383928 -- <a file of yours> for what the codemod did to it); itc_recomp/src/guest.h (every function takes only the guest address now), itc_recomp/src/schema/view.h (a view is only its address: \`explicit View(uint32_t)\`), itc_recomp/src/lifted/lifted.h (lifted::Call(address, args), entries take only \`const Args&\`), itc_recomp/src/hooks/native_entry.h.
What happened: E5 step 10 removed \`base\` (the guest memory base pointer, \`uint8_t* base\`, \`memory::Base()\`) from the whole program by a codemod: parameters \`uint8_t* base\` dropped, the first argument \`base\` (or \`x.base\`, \`memory::Base()\`) dropped from calls, \`[base]\` dropped from lambda captures. Memory is reached only through guest.h's functions by guest address. What the codemod couldn't do is left to you, and the compiler's errors below say where. Typical cases: a member or local holding base (\`uint8_t* base_\`, a struct's \`base\` field, \`uint8_t* base = memory::Base();\`): delete it and its initializers; a function pointer type or a std::function with an unnamed \`uint8_t*\` parameter: drop that parameter; a call the codemod emptied wrongly (\`if ()\`, \`reinterpret_cast<...>()\`, \`static_cast<void*>()\`): read git show d0383928 for what it was and rewrite it without base (a check that base is set goes; a host pointer made from base must become guest::Pointer(address)); an \`int base\` or other unrelated variable named base the codemod dropped from a call: put it back exactly as it was (git show d0383928); a view built from a guest address now needs its explicit constructor (\`schema::T(address)\`); an overload that differed only by base now collides: keep one. Fix every error listed for your files, then read the rest of each file for the same patterns the compiler stopped before reaching (an error early in a file can hide later ones). Don't restyle, don't change anything else.`

const REPORT = {
  type: 'object',
  properties: {
    unit: { type: 'string' },
    fixed: { type: 'array', items: { type: 'object', properties: {
      file: { type: 'string' }, what: { type: 'string', description: 'the error and the fix, briefly' } },
      required: ['file', 'what'] } },
    restored: { type: 'array', description: 'places the codemod dropped something that wasn\'t the guest base (an unrelated variable named base), put back', items: { type: 'object', properties: {
      file: { type: 'string' }, line: { type: 'integer' }, what: { type: 'string' } }, required: ['file', 'line', 'what'] } },
    left: { type: 'array', items: { type: 'object', properties: {
      file: { type: 'string' }, line: { type: 'integer' }, why: { type: 'string' } }, required: ['file', 'line', 'why'] } },
    for_lead: { type: 'string', description: 'changes needed in files you may not edit (exact), risks, or "none"' },
  },
  required: ['unit', 'fixed', 'restored', 'left', 'for_lead'],
}

phase('Fix')
const results = await schedule(UNITS, unit => agent(
`You're one of several agents doing E5 step 10 (removing the guest memory base pointer) as one swarm, each for one unit of this game port's code. Your unit: \`${unit.name}\`.
- Your files (held for you while you run): ${unit.files.join(', ')}.
- The compiler's errors in them (the lead's build; line numbers as of the WIP tree):
${unit.errors.join('\n')}
${RULES}
${HOW}

Your work: fix every error in your files and the same patterns the compiler didn't reach, then report what you fixed, what you restored (unrelated variables named base), what's left, and what the lead must change elsewhere.`,
  { label: `fix:${unit.name}`, phase: 'Fix', schema: REPORT }))

const reports = results.filter(Boolean)
log(`${reports.length} of ${UNITS.length} units reported; ${reports.reduce((n, r) => n + r.fixed.length, 0)} fixes, ${reports.reduce((n, r) => n + r.restored.length, 0)} restored, ${reports.reduce((n, r) => n + r.left.length, 0)} left`)
return {
  units: reports.length,
  missing: UNITS.filter((u, k) => !results[k]).map(u => u.name),
  reports,
}
