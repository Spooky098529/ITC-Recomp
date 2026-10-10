"""Find likely function entry points in a dumped Xbox 360 image that the
ReXGlue analyzer misses (no RTTI, so its vtable scanner finds nothing).

Sources of candidates:
  - 32-bit words in .rdata/.data that point into .text (vtables, callbacks)
  - lis/addi (or lis/ori) address constants in .text (member-function pointers)
A candidate is kept only if it looks like a function entry, not a label inside
a function.
"""
import struct, sys, re

img_path, manifest_path = sys.argv[1], sys.argv[2]
BASE = 0x82000000
img = open(img_path, 'rb').read()
TEXT = (0x82090000, 0x82090000 + 0x54C064)
DATA_RANGES = [(0x82000600, 0x82000600 + 0x76DC4),   # .rdata
               (0x82600000, 0x82600000 + 0x37833C)]  # .data

def word(addr):
    o = addr - BASE
    return struct.unpack('>I', img[o:o + 4])[0]

def in_text(v):
    return TEXT[0] <= v < TEXT[1] and v % 4 == 0

BLR, BCTR = 0x4E800020, 0x4E800420
MFLR_R12 = 0x7D8802A6

def is_uncond_b(i):
    return (i >> 26) == 18 and (i & 3) == 0  # b, not bl/ba

def entry_shape(ins):
    # this-adjustor thunk (addi r3,r3,imm), standard prologue, or vcall thunk
    # (lwz r12,0(r3|r4)).
    return (ins >> 16) == 0x3863 or ins == MFLR_R12 or ins in (0x81830000, 0x81840000)

def plausible_start(t, loose_after_b):
    prev, ins = word(t - 4), word(t)
    if prev in (BLR, BCTR, 0):  # nothing can fall through into t
        return True
    if is_uncond_b(prev):        # could be a switch-case label instead
        return entry_shape(ins) or loose_after_b
    return False

def jump_table_like(addr):
    # Switch tables are runs of code pointers that all land close together.
    run = [addr]
    for step in (-4, 4):
        a = addr + step
        while in_text(word(a)) and len(run) < 64:
            run.append(a); a += step
    if len(run) < 3:
        return False
    vals = [word(a) for a in run]
    return max(vals) - min(vals) < 0x800

cands = {}
for lo, hi in DATA_RANGES:
    for a in range(lo, hi - 3, 4):
        v = word(a)
        if in_text(v) and plausible_start(v, loose_after_b=not jump_table_like(a)):
            cands.setdefault(v, 'data')

# lis rD,hi ; ... ; addi/ori rX,rD,lo within a short window
for a in range(TEXT[0], TEXT[1] - 4, 4):
    i = word(a)
    if (i >> 26) != 15 or ((i >> 16) & 31) != 0:  # lis = addis rD,0,imm
        continue
    rd, hi = (i >> 21) & 31, (i & 0xFFFF) << 16
    for b in range(a + 4, min(a + 4 * 12, TEXT[1]), 4):
        j = word(b)
        op, ra = j >> 26, (j >> 16) & 31
        if ra == rd and op in (14, 24):  # addi / ori
            lo = j & 0xFFFF
            v = (hi + (lo - 0x10000 if op == 14 and lo & 0x8000 else lo)) & 0xFFFFFFFF
            if in_text(v) and plausible_start(v, loose_after_b=False):
                cands.setdefault(v, 'code')
            break
        if ((j >> 21) & 31) == rd and op not in (36, 37, 38, 44, 45, 47):  # rD overwritten (not a store)
            break

hinted = set(int(a, 16) for a in re.findall(r'^(0x[0-9A-Fa-f]+) = \{\}', open(manifest_path).read(), re.M))
n_data = sum(1 for k in cands.values() if k == 'data')
print(f"candidates: {len(cands)} ({n_data} from data, {len(cands) - n_data} from code)", file=sys.stderr)
print(f"loop-found hints covered: {len(hinted & set(cands))}/{len(hinted)}; missed: "
      f"{[hex(h) for h in sorted(hinted - set(cands))]}", file=sys.stderr)
for c in sorted(cands):
    print(f"0x{c:08X}")
