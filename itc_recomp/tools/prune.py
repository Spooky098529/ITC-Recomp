"""Drop scanner hints implicated in 'Unresolved branch' sites of generated code."""
import re, sys, glob, bisect
hints_path, gen_dir = sys.argv[1], sys.argv[2]
text = open(hints_path).read()
hints = sorted(int(x, 16) for x in re.findall(r'^(0x[0-9A-F]+) = \{\}', text, re.M))
pairs = set()
for f in glob.glob(gen_dir + '/*.cpp'):
    for a, b in re.findall(r'Unresolved branch from 0x([0-9A-F]+) to 0x([0-9A-F]+)', open(f).read()):
        pairs.add((int(a, 16), int(b, 16)))
bad = set()
for a, b in pairs:
    lo, hi = min(a, b), max(a, b)
    i = bisect.bisect_right(hints, lo) - 1
    if i >= 0:
        bad.add(hints[i])                      # start of the function holding lo
    j = bisect.bisect_right(hints, hi)
    bad.update(hints[bisect.bisect_right(hints, lo):j])  # boundaries in between
print(f"{len(pairs)} unresolved branches -> removing {len(bad)} of {len(hints)} hints", file=sys.stderr)
out = [l for l in text.splitlines() if not (re.match(r'^0x[0-9A-F]+ = \{\}$', l) and int(l.split()[0], 16) in bad)]
open(hints_path, 'w').write('\n'.join(out) + '\n')
