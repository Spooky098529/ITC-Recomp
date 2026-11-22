export const meta = {
  name: 'e5-step2-swarm',
  description: 'E5 step 2 for 130 areas at once: schema + hand ports per area, integrate, then cross-area switches (no builds; the lead validates after)',
  phases: [
    { title: 'Convert', detail: 'one agent per area: schema file, own types, hand ports' },
    { title: 'Integrate', detail: 'full schema_gen, clashes between areas fixed' },
    { title: 'Cross-area', detail: 'one agent per area: accesses to other areas\' types' },
  ],
}

// args.areas: the areas to convert. args.cross_areas (optional): the areas
// the Cross-area phase takes (every area with a schema file, when an earlier
// run converted some); by default the areas converted in this run.
const AREAS = args.areas
const CROSS = args.cross_areas || null

// At most 6 agents at once (the user's call): workers take the items in order.
const CONCURRENCY = 6
async function pool(items, fn) {
  const out = new Array(items.length).fill(null)
  let next = 0
  async function worker() {
    while (next < items.length) {
      const k = next++
      try { out[k] = await fn(items[k], k) } catch (e) { out[k] = null }
    }
  }
  await Promise.all(Array.from({ length: CONCURRENCY }, worker))
  return out
}

const RULES = `
The repo root (where CLAUDE.md is), branch e5-native-16-11-26 (stay on it). Many agents are editing this same working tree at the same time, one per area, so these rules are strict:
- Never build (no cmake, no ninja), never run the game, never run git commands that change anything (no add, commit, checkout, stash, reset), never run tools/lift.py or tools/lift_originals.py, never run the full tools/schema_gen.py (only: python3 -I itc_recomp/tools/schema_gen.py --area <your area>), never run tools/type_map.py (itc_recomp/out/type_map.json is already there).
- Edit only your own files: itc_recomp/schema/<area>.toml, the hand-port files of your area (its folder itc_recomp/src/<area with _>/ and itc_recomp/src/hooks/<area with _>.cpp/.h, or the files whose functions are your area's rows when the folder is named otherwise, e.g. src/memory/, src/c_runtime/: a file another area's functions mostly live in is not yours), and docs/functions/<area>.md. If a change is needed in a file that isn't yours, put it in your report instead. If the Edit tool says a file changed since you read it, read it again and redo the edit.
- Never hand-edit itc_recomp/src/schema/ or itc_recomp/src/lifted/ (generated).
- American English. Nothing the game sees may change: the same loads and stores, same widths, same order, same addresses.`

const DONE_EXAMPLES = `Examples already done: itc_recomp/schema/type2-dialog.toml (with src/type2_dialog/*.cpp switched), heap.toml (src/memory/c_runtime_heap.cpp, src/hooks/heap.cpp), c-runtime.toml (src/c_runtime/), menu-helpers.toml (FrontEndMenu, PageObject: the front-end screens' shared menu and page object), game-modes.toml (StateMachine), and xaudio-objects (in progress). The procedure is docs/loops/e5-loop.md, "Step 2, one area", steps 1 to 5 and 9 (not 6 to 8: the lead does the lifted code, the build, the checks and the commit). docs/e5-native.md step 2 explains why.`

const REPORT = {
  type: 'object',
  properties: {
    area: { type: 'string' },
    types: { type: 'integer', description: 'schema types in your file' },
    fields: { type: 'integer', description: 'named fields (not unknown_)' },
    unknown_fields: { type: 'integer' },
    files_switched: { type: 'array', items: { type: 'string' } },
    accesses_left: { type: 'string', description: 'what you left as offsets and why (brief)' },
    check: { type: 'string', description: "schema_check.py <area>'s last line" },
    for_lead: { type: 'string', description: 'changes needed in files not yours, measure faults, clashes, decisions (brief)' },
  },
  required: ['area', 'types', 'fields', 'unknown_fields', 'files_switched', 'accesses_left', 'check', 'for_lead'],
}

phase('Convert')
const converted = await pool(AREAS, area => agent(
`You're one of many agents doing E5 step 2 (the schema and named fields) at once, each for one area of this game port. Your area: \`${area}\` (docs/functions/${area}.md).
${RULES}
${DONE_EXAMPLES}

Your work:
1. Read docs/functions/${area}.md, docs/known-data.md's entries for its globals and objects, your hand ports' headers (they name most offsets), and your area's rows in itc_recomp/coverage/type_map.tsv.
2. If itc_recomp/schema/${area}.toml doesn't exist, seed it: python3 -I itc_recomp/tools/schema_seed.py ${area}. The seed holds only the types the measure says your area owns (the area touching them most): describe only those (other areas' agents describe theirs at the same time). Don't edit other areas' .toml files.
3. Name every field from the docs and the hand port's constants and comments; unknown_<offset> only where nothing says. Merge types the measure split (one object met through several paths or allocations) into one schema type with measures = [...]. Give each type a C++ name of its own, prefixed with something of your area when it's generic (two areas may both want "Record"): it must be unique schema-wide. Image words your area uses go in [globals].
4. python3 -I itc_recomp/tools/schema_gen.py --area ${area}, then python3 -I itc_recomp/tools/schema_check.py ${area} until it reports 0 accesses that fit no field (fix fields; report measure faults).
5. Switch your hand ports' accesses to the objects in YOUR schema file (and the done areas' types listed above) to the views (#include "schema/<area_>.h"; see src/type2_dialog/*.cpp): same loads and stores in the same order; delete offset constants that become unused. Accesses to types other areas own are left for a second pass (don't wait for them). Frames and stack words aren't objects: leave them. If your area has no hand ports (all lifted), only the schema is needed.
Then report.`,
  { label: `convert:${area}`, phase: 'Convert', schema: REPORT }))

const reports = converted.filter(Boolean)
log(`converted ${reports.length} of ${AREAS.length} areas`)

phase('Integrate')
const integration = await agent(
`You're the integrator after ${AREAS.length} agents each wrote one area's schema at once (E5 step 2; docs/loops/e5-loop.md; docs/e5-native.md step 2).
The repo root (where CLAUDE.md is), branch e5-native-16-11-26. Don't build, don't run the game, no git commands that change anything, don't run lift.py, lift_originals.py or type_map.py.
1. Run python3 -I itc_recomp/tools/schema_gen.py (full). Fix every error it reports by editing the itc_recomp/schema/*.toml files: TOML syntax errors, duplicate type names (rename one, prefixing its area's name, and fix every \`to = \` and every C++ use: grep -rn "schema::<Name>" itc_recomp/src), unknown pointee types (point to the right type, or make the field u32), two types claiming one measure name (keep it on the type its area owns most; if they are the same object, merge them into one type in the owner's file and point the other area's uses at it). Repeat until it runs clean.
2. Run python3 -I itc_recomp/tools/schema_check.py (all areas) and fix what fits no field (a missing field or a wrong width) in the owning file.
3. Here are the area agents' notes for the lead (this run's below; an earlier run's in docs/loops/e5-swarm-reports.md, "For the lead"); act on the ones about schema files (moving a type to its owner, a field another area needs added to a type you own now); leave notes about code files in your report:
${JSON.stringify(reports.map(r => ({ area: r.area, for_lead: r.for_lead })).filter(r => r.for_lead && r.for_lead.length > 3))}
Report: schema_gen's and schema_check's last lines, every rename (old -> new, files touched), and what you left.`,
  { label: 'integrate', phase: 'Integrate' })

phase('Cross-area')
// Areas converted in an earlier run have no report in this one: their first
// pass's notes are in docs/loops/e5-swarm-reports.md.
const crossItems = CROSS
  ? CROSS.map(area => reports.find(r => r.area === area) ||
      { area, accesses_left: 'see docs/loops/e5-swarm-reports.md, this area\'s "Left as offsets" (if it is listed there)' })
  : reports
const crossed = await pool(crossItems, r => agent(
`You're one of many agents doing the second pass of E5 step 2 at once. Your area: \`${r.area}\` (docs/functions/${r.area}.md). In the first pass you (another agent) wrote itc_recomp/schema/${r.area}.toml and switched your area's hand ports to your own types' views; every area's schema now exists and src/schema/ holds every view (the integrator ran the full generator).
${RULES}
Your work: in your area's hand-port files, switch the accesses to objects OTHER areas' schema files describe (grep -n "measure\\|^\\[" itc_recomp/schema/*.toml to find a type; its view is schema::<Type> in src/schema/<area_>.h, globals in schema::<Area>Globals) to their views, the same way (same loads and stores, same order). The first pass's note on what it left: ${JSON.stringify(r.accesses_left)}. If a field you need is missing from another area's type, don't edit that file: list it in your report. Also fix anything in your files that no longer matches a renamed type (the integrator renamed clashing types; grep your files for schema:: names and check they exist in src/schema/).
Then report (types and fields: your schema file's counts; check: schema_check.py ${r.area}'s last line).`,
  { label: `cross:${r.area}`, phase: 'Cross-area', schema: REPORT }))

const final = crossed.filter(Boolean)
return {
  converted: reports.length,
  crossed: final.length,
  integration,
  missing_convert: AREAS.filter(a => !reports.some(r => r.area === a)),
  for_lead: final.filter(r => r.for_lead && r.for_lead.length > 3).map(r => ({ area: r.area, note: r.for_lead })),
  totals: {
    types: final.reduce((n, r) => n + r.types, 0),
    fields: final.reduce((n, r) => n + r.fields, 0),
    unknown: final.reduce((n, r) => n + r.unknown_fields, 0),
  },
  left: final.map(r => ({ area: r.area, left: r.accesses_left })),
}
