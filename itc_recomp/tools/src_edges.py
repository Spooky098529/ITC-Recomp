#!/usr/bin/env python3
"""The guest functions our C++ in itc_recomp/src still reaches, per native
function: the outgoing edges static_map.py gives functions that are ours
(docs/coverage.md, "The static map"). Imported by static_map.py; run alone
it prints a summary and, for the functions given, their native edges.

usage: src_edges.py [sub_XXXXXXXX ...]

How src/ is read (no compiler: a small preprocessor and a brace parser):
  files      every .cpp, .h and .inc under itc_recomp/src except
             src/harnesses/ (harnesses call the originals on purpose)
  macros     #define'd macros are expanded (stringizing and pasting too),
             except those defined in src/base/ (logging, flags,
             ITC_NATIVE_RUNNING), so macro-made adapters and their
             __imp__sub_##address fallbacks are seen with their addresses
  defs       every function body at namespace or class level (a lambda
             belongs to the function it's in), every namespace-level
             variable with an initializer (constants, tables such as the
             InitStates' step tables), and every enumerator
  refs       an identifier in a body names the definitions of that name
             visible from the file: the same file, a header it includes
             (transitively), or a .cpp that includes a header both see
             declaring the name (`extern` variables included); a qualifier
             `a::b` must match a scope of the definition when any
             candidate's does, after namespace aliases (`namespace t67 =
             type67_modes;`, the file's, its headers' or the function's)
             are replaced by what they name; an unqualified name takes the
             candidates in the reference's own namespaces, innermost first,
             when there are any (C++'s lookup)
  targets    in a body, `sub_X` (a call through the function table: its
             override when it has one), `__imp__sub_X` (the original's
             body) and hexadecimal constants (a guest call by address, a
             function pointer or a vtable stored: static_map.py decides
             which land on a function or in data; a constant only compared,
             as a `case` label or beside `==` or `!=`, isn't a target)
  skipped    a statement guarded by a flag that's false by default:
             `if (ITC_FLAG(x))` with x off by default, `if (!ITC_FLAG(x))`
             with x on (the flag-off fallback), or the same through a
             function whose body is `return ITC_FLAG(x);`; the arguments of
             ITC_LOG_* and ITC_NATIVE_RUNNING; and the bodies of
             src/debug/'s functions other than overrides (diagnostics). A
             definition named only in such statements isn't host code.
  guarded    a function body's parts an address constant picks (`case
             0xA:` up to the next label, or the statement of `if (x ==
             0xA)`, `&&` with others allowed) are definitions of their own:
             when 0xA is a function's start, the part is that function's
             C++ (RunInit's 18 Inits, a slot dispatcher's methods, a guest
             call's trampoline, where `__imp__sub_A` is a real call to the
             original), reached where the address is and never from the
             dispatcher's callers; when it's a table or vtable, no one's
             (unless it holds a print); else (a plain constant) its parent's
  key tables KEY_TABLES: tables naming addresses only as lookup keys
             (`info.address == address`) give no edges
  entries    a native function's C++ is its override (`REX_FUNC(sub_X)`,
             written out or made by a macro), every definition holding
             ITC_NATIVE_RUNNING("sub_X ...") (a guarded part's with the
             code around it) and the parts its address picks; a definition
             several functions share by an argument (a door's side) gives
             its callers all of them
"""

import collections
import os
import re
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(TOOLS, "..", ".."))
SRC = os.path.join(ROOT, "itc_recomp", "src")

STRIP = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', re.S)
TOKEN = re.compile(r'(?P<str>"(?:\\.|[^"\\\n])*")|(?P<chr>\'(?:\\.|[^\'\\\n])*\')'
                   r"|(?P<num>0[xX][0-9A-Fa-f']+[uUlL]*|\d[\w.']*)|(?P<id>[A-Za-z_]\w*)"
                   r"|(?P<op>::|->|##|&&|\|\||[#{}()\[\];,<>=:.!&|?*+\-/%^~])")
SUB = re.compile(r"^sub_([0-9A-F]{8})$")
IMP = re.compile(r"^__imp__sub_([0-9A-F]{8})$")
FLAG_BOOL = re.compile(r"ITC_FLAG_BOOL\s*\(\s*(\w+)\s*,\s*(true|false)")
OPAQUE = re.compile(r"^ITC_(LOG_\w+|NATIVE_RUNNING)$")
RANK = {"plain": 0, "native": 1, "imp": 2, "table": 3}
KEYWORDS = {"if", "for", "while", "switch", "return", "sizeof", "alignof", "decltype",
            "static_assert", "catch", "new", "delete", "case", "do", "else", "throw"}
SPECIFIERS = {"static", "inline", "constexpr", "consteval", "constinit", "extern", "const",
              "volatile", "thread_local", "mutable", "virtual", "explicit", "friend",
              "typename", "unsigned", "signed", "[[", "noexcept", "override", "final"}


class Tok:
    __slots__ = ("k", "t")

    def __init__(self, k, t):
        self.k, self.t = k, t

    def __repr__(self):
        return self.t


def tokenize(text):
    out = []
    for m in TOKEN.finditer(text):
        k = m.lastgroup
        out.append(Tok(k, m.group(k)))
    return out


def read_sources():
    files = {}
    for d, dirs, names in os.walk(SRC):
        dirs[:] = [x for x in dirs if not (d == SRC and x == "harnesses")]
        for n in names:
            if n.endswith((".cpp", ".h", ".inc")):
                p = os.path.join(d, n)
                with open(p, errors="replace") as f:
                    files[os.path.relpath(p, SRC)] = f.read()
    return files


def preprocess(rel, text):
    """Comments out, continuations joined; (#includes, macros, code)."""
    text = STRIP.sub(lambda m: m.group(0) if m.group(0)[0] in "\"'" else " ", text)
    text = text.replace("\\\n", " ")
    includes, macros, code = [], {}, []
    for line in text.split("\n"):
        s = line.strip()
        if s.startswith("#"):
            m = re.match(r'#\s*include\s*"([^"]+)"', s)
            if m:
                includes.append(m.group(1))
            m = re.match(r"#\s*define\s+(\w+)(\(([^)]*)\))?\s*(.*)$", s)
            if m:
                params = None
                if m.group(2) is not None:
                    params = [p.strip() for p in m.group(3).split(",") if p.strip()]
                macros[m.group(1)] = (params, tokenize(m.group(4)))
            code.append("")
        else:
            code.append(line)
    return includes, macros, "\n".join(code)


def paste(a, b):
    t = tokenize(a.t + b.t)
    return t[0] if len(t) == 1 else Tok("id", a.t + b.t)


def expand(tokens, lookup, disabled=frozenset(), depth=0):
    out, i, n = [], 0, len(tokens)
    while i < n:
        t = tokens[i]
        mac = lookup(t.t) if t.k == "id" and t.t not in disabled and depth < 12 else None
        if mac is None:
            out.append(t)
            i += 1
            continue
        params, body = mac
        if params is None:
            out.extend(expand(body, lookup, disabled | {t.t}, depth + 1))
            i += 1
            continue
        if i + 1 >= n or tokens[i + 1].t != "(":
            out.append(t)
            i += 1
            continue
        args, cur, d, j = [], [], 0, i + 2
        while j < n:
            x = tokens[j]
            if x.t == "(":
                d += 1
            elif x.t == ")":
                if d == 0:
                    break
                d -= 1
            if x.t == "," and d == 0:
                args.append(cur)
                cur = []
            else:
                cur.append(x)
            j += 1
        args.append(cur)
        amap = {p: (args[k] if k < len(args) else []) for k, p in enumerate(params)}
        if params and params[-1] == "...":
            amap["__VA_ARGS__"] = [x for a in args[len(params) - 1:] for x in a + [Tok("op", ",")]][:-1]
        res, k = [], 0
        while k < len(body):
            b = body[k]
            if b.t == "#" and k + 1 < len(body) and body[k + 1].t in amap:
                res.append(Tok("str", '"' + " ".join(x.t for x in amap[body[k + 1].t]) + '"'))
                k += 2
                continue
            if b.t == "##" and res and k + 1 < len(body):
                nxt = body[k + 1]
                rep = amap.get(nxt.t, [nxt]) if nxt.k == "id" else [nxt]
                if rep:
                    res[-1] = paste(res[-1], rep[0])
                    res.extend(rep[1:])
                k += 2
                continue
            if b.k == "id" and b.t in amap:
                res.extend(amap[b.t])
            else:
                res.append(b)
            k += 1
        out.extend(expand(res, lookup, disabled | {t.t}, depth + 1))
        i = j + 1
    return out


def match(tokens, i, open_="{", close="}"):
    d = 0
    for j in range(i, len(tokens)):
        t = tokens[j].t
        if t == open_:
            d += 1
        elif t == close:
            d -= 1
            if d == 0:
                return j
    return len(tokens) - 1


class Def:
    __slots__ = ("id", "name", "scope", "file", "kind", "body", "adapter", "prints",
                 "refs", "subs", "imps", "hexes", "flag_alias", "debug", "guards", "parent",
                 "guarded", "skipped")

    def __init__(self, name, scope, file, kind, body):
        self.name, self.scope, self.file, self.kind, self.body = name, scope, file, kind, body
        self.adapter = None
        self.prints = set()
        self.refs, self.subs, self.imps, self.hexes = [], set(), set(), set()
        self.flag_alias = None
        self.debug = file.startswith("debug" + os.sep)
        self.guards = ()  # a guarded part's address constants
        self.parent = None
        self.guarded = []  # its guarded parts (Defs of their own)
        self.skipped = set()  # names in its statements under a flag off by default


def strip_template(stmt):
    """The statement without leading `template <...>` and specifiers."""
    i = 0
    while i < len(stmt):
        t = stmt[i].t
        if t == "template" and i + 1 < len(stmt) and stmt[i + 1].t == "<":
            d, j = 0, i + 1
            while j < len(stmt):
                if stmt[j].t == "<":
                    d += 1
                elif stmt[j].t == ">":
                    d -= 1
                    if d == 0:
                        break
                j += 1
            i = j + 1
            continue
        if t in SPECIFIERS or stmt[i].k == "str":
            i += 1
            continue
        if t == "[" and i + 1 < len(stmt) and stmt[i + 1].t == "[":
            j = i
            while j < len(stmt) and not (stmt[j].t == "]" and j + 1 < len(stmt) and stmt[j + 1].t == "]"):
                j += 1
            i = j + 2
            continue
        break
    return stmt[i:]


def top_index(stmt, what):
    d = 0
    for i, t in enumerate(stmt):
        if t.t in "([":
            if t.t == what and d == 0:
                return i
            d += 1
        elif t.t in ")]":
            d -= 1
        elif t.t == what and d == 0:
            return i
    return -1


def decl_name(head):
    """A variable declaration's name: its last identifier outside brackets."""
    d, name = 0, None
    for x in head:
        if x.t == "[":
            d += 1
        elif x.t == "]":
            d -= 1
        elif x.k == "id" and d == 0:
            name = x.t
    return name


def call_paren(stmt):
    """A declaration's parameter list: the first `(` outside parentheses,
    brackets and template arguments (a `<` after a name)."""
    d = a = 0
    for i, t in enumerate(stmt):
        if t.t == "<" and i and (stmt[i - 1].k == "id" or stmt[i - 1].t == "::"):
            a += 1
        elif t.t == ">" and a:
            a -= 1
        elif t.t in "([":
            if t.t == "(" and d == 0 and a == 0:
                return i
            d += 1
        elif t.t in ")]":
            d -= 1
    return top_index(stmt, "(")


def qualified_before(stmt, i):
    """The identifier ending at stmt[i] with its qualifiers."""
    parts = [stmt[i].t]
    j = i - 1
    while j >= 1 and stmt[j].t == "::" and stmt[j - 1].k == "id":
        parts.insert(0, stmt[j - 1].t)
        j -= 2
    return parts


def namespace_alias(stmt):
    """(alias, [namespaces]) for `namespace a = b::c` (no `;`), else None."""
    w = [x.t for x in stmt]
    if len(w) >= 4 and w[0] == "namespace" and stmt[1].k == "id" and w[2] == "=":
        parts = [x for x in w[3:] if x != "::"]
        if parts and all(re.match(r"^\w+$", x) for x in parts):
            return w[1], parts
    return None


class Parser:
    def __init__(self, rel):
        self.rel = rel
        self.defs = []
        self.decls = set()  # names declared (or defined) here
        self.ns_aliases = {}  # `namespace a = b::c;` at any level: a -> [b, c]

    def container(self, toks, i, end, scope):
        start = i
        while i < end:
            t = toks[i].t
            if t == ";":
                self.statement(toks[start:i], scope)
                i += 1
                start = i
            elif t == "(" or t == "[":
                i = match(toks, i, t, ")" if t == "(" else "]") + 1
            elif t == "{":
                j = match(toks, i)
                stmt = strip_template(toks[start:i])
                consumed = self.block(toks, stmt, i, j, scope)
                if consumed is None:  # a member initializer's braces
                    i = j + 1
                    continue
                i = consumed
                start = i
            else:
                i += 1
        if start < end:
            self.statement(toks[start:end], scope)

    def block(self, toks, stmt, i, j, scope):
        words = [x.t for x in stmt]
        if words[:1] == ["namespace"] or words[:2] == ["inline", "namespace"]:
            name = [w for w in words if w not in ("namespace", "inline", "::")]
            self.container(toks, i + 1, j, scope + name)
            return j + 1
        if not stmt or words == ["extern"]:
            self.container(toks, i + 1, j, scope)
            return j + 1
        eq = top_index(stmt, "=")
        paren = top_index(stmt, "(")
        if "operator" in words:
            eq = -1
            o = words.index("operator")
            k = o + 3 if words[o + 1:o + 3] == ["(", ")"] else o + 1
            while k < len(words) and words[k] != "(":
                k += 1
            paren = k if k < len(words) else -1
        elif paren >= 0:
            paren = call_paren(stmt)
        if words[0] in ("class", "struct", "union") and (paren < 0 or (eq >= 0 and eq < paren)):
            names = [x.t for x in stmt[1:] if x.k == "id" and x.t not in ("final", "alignas")]
            name = names[0] if names else "?"
            if eq < 0:
                self.decls.add(name)
                self.container(toks, i + 1, j, scope + [name])
                # Declarators after the class: up to the `;`.
                k = j + 1
                while k < len(toks) and toks[k].t != ";":
                    k += 1
                return k + 1
        if words[0] == "enum":
            self.enum(toks, i + 1, j, scope)
            k = j + 1
            while k < len(toks) and toks[k].t != ";":
                k += 1
            return k + 1
        if eq >= 0 and (paren < 0 or eq < paren):
            return self.variable(toks, stmt[:eq], i, scope)
        if paren >= 0:
            # A constructor's member initializer with braces: `x_{...}` after `) :`.
            close = match(stmt, paren, "(", ")")
            rest = stmt[close + 1:]
            if any(x.t == ":" for x in rest) and stmt[-1].k == "id":
                return None
            name_i = paren - 1
            while name_i >= 0 and stmt[name_i].k != "id":
                name_i -= 1
            if name_i < 0:
                return j + 1
            if stmt[name_i].t == "REX_FUNC" or (stmt[name_i].t in KEYWORDS):
                pass
            parts = qualified_before(stmt, name_i)
            d = Def(parts[-1], scope + parts[:-1], self.rel, "func", toks[i + 1:j])
            # Member initializer list arguments run too.
            d.body = rest + d.body
            if stmt[name_i].t == "REX_FUNC":
                arg = stmt[paren + 1] if paren + 1 < len(stmt) else None
                m = SUB.match(arg.t) if arg is not None else None
                if m:
                    d.adapter = arg.t
                    d.name = arg.t
                else:
                    return j + 1
            self.defs.append(d)
            self.decls.add(d.name)
            return j + 1
        # `Type name{...}` or `Type name[]{...}`.
        return self.variable(toks, stmt, i, scope)

    def variable(self, toks, head, i, scope):
        name = decl_name(head)
        k = i
        d = 0
        while k < len(toks):
            if toks[k].t in "({[":
                d += 1
            elif toks[k].t in ")}]":
                d -= 1
            elif toks[k].t == ";" and d == 0:
                break
            k += 1
        if name:
            v = Def(name, scope, self.rel, "var", toks[i:k])
            self.defs.append(v)
            self.decls.add(v.name)
        return k + 1

    def enum(self, toks, i, j, scope):
        k = i
        while k < j:
            if toks[k].k == "id":
                e = k + 1
                d = 0
                while e < j and not (toks[e].t == "," and d == 0):
                    if toks[e].t in "({[":
                        d += 1
                    elif toks[e].t in ")}]":
                        d -= 1
                    e += 1
                v = Def(toks[k].t, scope, self.rel, "var", toks[k + 1:e])
                self.defs.append(v)
                self.decls.add(v.name)
                k = e + 1
            else:
                k += 1

    def statement(self, stmt, scope):
        stmt = strip_template(stmt)
        if not stmt:
            return
        alias = namespace_alias(stmt)
        if alias:
            self.ns_aliases[alias[0]] = alias[1]
            return
        eq = top_index(stmt, "=")
        paren = top_index(stmt, "(")
        if any(x.t == "operator" for x in stmt):
            return
        if eq >= 0 and (paren < 0 or eq < paren) and stmt[0].t not in ("using", "typedef"):
            name = decl_name(stmt[:eq])
            if name:
                v = Def(name, scope, self.rel, "var", stmt[eq + 1:])
                self.defs.append(v)
                self.decls.add(v.name)
            return
        if paren > 0:
            k = paren - 1
            while k >= 0 and stmt[k].k != "id":
                k -= 1
            if k >= 0:
                self.decls.add(stmt[k].t)
        else:
            # `extern const Info kTable[kCount];` declares kTable.
            name = decl_name(stmt)
            if name:
                self.decls.add(name)


def analyze_body(d, flag_default, aliases, ns_aliases=None):
    """Fills d's refs, subs, imps, hexes and prints from its body, leaving
    out statements guarded by a flag off by default."""
    toks = d.body
    n = len(toks)
    i = 0
    ns = dict(ns_aliases or {})

    def cond_false(c):
        # c: the condition's tokens; false by default when a top-level
        # conjunct is.
        parts, cur, depth = [], [], 0
        for x in c:
            if x.t in "([":
                depth += 1
            elif x.t in ")]":
                depth -= 1
            if x.t == "&&" and depth == 0:
                parts.append(cur)
                cur = []
            else:
                cur.append(x)
        parts.append(cur)
        for p in parts:
            neg = False
            while p and p[0].t == "!":
                neg = not neg
                p = p[1:]
            while len(p) >= 2 and p[0].t == "(" and p[-1].t == ")":
                p = p[1:-1]
            w = [x.t for x in p]
            val = None
            if len(w) == 4 and w[0] == "ITC_FLAG" and w[1] == "(" and w[3] == ")":
                val = flag_default.get(w[2])
            elif len(w) >= 3 and w[-2:] == ["(", ")"]:
                val = aliases.get(w[-3])
            if val is not None and val == neg:
                return True
        return False

    while i < n:
        t = toks[i]
        if t.k == "id":
            if t.t == "namespace":  # a function-local namespace alias
                k = i
                while k < n and toks[k].t != ";":
                    k += 1
                alias = namespace_alias(toks[i:k])
                if alias:
                    ns[alias[0]] = alias[1]
                i = k + 1
                continue
            if t.t == "if" and i + 1 < n and toks[i + 1].t == "(":
                close = match(toks, i + 1, "(", ")")
                if cond_false(toks[i + 2:close]):
                    k = close + 1
                    if k < n and toks[k].t == "{":
                        i = match(toks, k) + 1
                    else:
                        while k < n and toks[k].t != ";":
                            if toks[k].t in "({[":
                                k = match(toks, k, toks[k].t, {"(": ")", "{": "}", "[": "]"}[toks[k].t])
                            k += 1
                        i = k + 1
                    d.skipped |= {x.t for x in toks[close + 1:i] if x.k == "id"}
                    continue
            if OPAQUE.match(t.t) and i + 1 < n and toks[i + 1].t == "(":
                close = match(toks, i + 1, "(", ")")
                if t.t == "ITC_NATIVE_RUNNING":
                    s = "".join(x.t[1:-1] for x in toks[i + 2:close] if x.k == "str")
                    m = re.match(r"\s*(sub_[0-9A-Fa-f]{8})", s)
                    if m:
                        d.prints.add("sub_" + m.group(1)[4:].upper())
                i = close + 1
                continue
            m = IMP.match(t.t)
            if m:
                d.imps.add("sub_" + m.group(1))
            elif SUB.match(t.t):
                d.subs.add(t.t)
            elif t.t not in KEYWORDS:
                prev = toks[i - 1].t if i else ""
                qual = None
                if prev == "::" and i >= 2 and toks[i - 2].k == "id":
                    parts = qualified_before(toks, i)[:-1]
                    if parts[0] in ns:  # a namespace alias: what it names
                        parts = ns[parts[0]] + parts[1:]
                    qual = tuple(parts)
                member = prev in (".", "->")
                d.refs.append((qual, t.t, member))
        elif t.k == "num" and t.t[:2] in ("0x", "0X"):
            # A case label or a comparison only tests an address.
            p1 = toks[i - 1].t if i else ""
            p2 = toks[i - 2].t if i >= 2 else ""
            nx = toks[i + 1].t if i + 1 < n else ""
            nx2 = toks[i + 2].t if i + 2 < n else ""
            if p1 == "case" or (p1 == "=" and p2 in ("=", "!")) or (nx in ("=", "!") and nx2 == "="):
                i += 1
                continue
            try:
                d.hexes.add(int(t.t.rstrip("uUlL").replace("'", ""), 16))
            except ValueError:
                pass
        i += 1
    # A flag alias: `return ITC_FLAG(x);` alone.
    w = [x.t for x in toks]
    if d.kind == "func" and len(w) == 6 and w[0] == "return" and w[1] == "ITC_FLAG":
        d.flag_alias = w[3]


def is_hex(tok):
    return tok.k == "num" and tok.t[:2] in ("0x", "0X")


def hex_value(tok):
    try:
        return int(tok.t.rstrip("uUlL").replace("'", ""), 16)
    except ValueError:
        return None


def guard_of(cond):
    """The address constant an `if` condition requires (a top-level `&&`
    conjunct `x == 0xA` or `0xA == x`), else None. Conditions naming a flag
    are left to analyze_body's flag rule."""
    if any(x.t == "ITC_FLAG" for x in cond):
        return None
    parts, cur, depth = [], [], 0
    for x in cond:
        if x.t in "([":
            depth += 1
        elif x.t in ")])":
            depth -= 1
        if depth == 0 and x.t == "||":
            return None
        if x.t == "&&" and depth == 0:
            parts.append(cur)
            cur = []
        else:
            cur.append(x)
    parts.append(cur)
    for p in parts:
        while len(p) >= 2 and p[0].t == "(" and p[-1].t == ")":
            p = p[1:-1]
        hx = [k for k, x in enumerate(p) if is_hex(x)]
        if len(hx) != 1:
            continue
        k = hx[0]
        w = [x.t for x in p]
        if (k == len(p) - 1 and w[k - 2:k] == ["=", "="] and "!" not in w[:k - 2][-1:]) or \
                (k == 0 and w[1:3] == ["=", "="]):
            return hex_value(p[k])
    return None


def split_guarded(toks, only_prints):
    """The body's parts guarded by an address constant (`case 0xA:` up to
    the next label, or `if (x == 0xA)`'s statement): (the rest, [(addresses,
    tokens)]). With only_prints, only parts holding ITC_NATIVE_RUNNING."""
    n = len(toks)
    keep, parts = [], []
    i, depth = 0, 0
    while i < n:
        t = toks[i]
        if t.t == "case":
            labels, k = [], i
            ok = True
            while k < n and toks[k].t == "case":
                e = k + 1
                while e < n and toks[e].t != ":":
                    e += 1
                lab = toks[k + 1:e]
                if len(lab) == 1 and is_hex(lab[0]) and hex_value(lab[0]) is not None:
                    labels.append(hex_value(lab[0]))
                else:
                    ok = False
                k = e + 1
            if ok and labels:
                e, d = k, 0
                while e < n:
                    x = toks[e].t
                    if x in ("case", "default") and d == 0:
                        break
                    if x == "{":
                        d += 1
                    elif x == "}":
                        if d == 0:
                            break
                        d -= 1
                    e += 1
                seg = toks[k:e]
                if not only_prints or any(x.t == "ITC_NATIVE_RUNNING" for x in seg):
                    parts.append((tuple(labels), seg))
                    keep.extend(toks[i:k])
                    i = e
                    continue
            keep.extend(toks[i:k])
            i = k
            continue
        if t.t == "if" and not only_prints and i + 1 < n and toks[i + 1].t == "(":
            close = match(toks, i + 1, "(", ")")
            a = guard_of(toks[i + 2:close])
            if a is not None:
                k = close + 1
                if k < n and toks[k].t == "{":
                    e = match(toks, k)
                    seg = toks[k + 1:e]
                else:
                    e = k
                    while e < n and toks[e].t != ";":
                        if toks[e].t in "({[":
                            e = match(toks, e, toks[e].t, {"(": ")", "{": "}", "[": "]"}[toks[e].t])
                        e += 1
                    seg = toks[k:e + 1]
                parts.append(((a,), seg))
                keep.extend(toks[i:close + 1])
                keep.append(Tok("op", ";"))
                i = e + 1
                continue
        if t.t == "{":
            depth += 1
        elif t.t == "}":
            depth -= 1
        keep.append(t)
        i += 1
    return keep, parts


# Tables whose addresses are only lookup keys (`info.address == address`,
# the entry's plain C++ then picked by `case 0xA:`): naming the table gives
# no edges; each address is its function's own, reached where its address
# is (a vtable, a mode's Init list).
KEY_TABLES = {
    ("type67_modes/inits.cpp", "kInits"),  # the type 6, 7 and 8 Inits
    ("save_screen/inits.cpp", "kInits"),  # the save screens' Inits
    ("options_screen/inits.cpp", "kInits"),  # the options screens' Inits
}

# Whether every address-guarded part is split off (False: only those
# holding ITC_NATIVE_RUNNING).
GUARD_ALL = True


def scope_has(scope, qual):
    """Whether the qualifier's names appear in the scope in a row."""
    n = len(qual)
    return any(tuple(scope[k:k + n]) == qual for k in range(len(scope) - n + 1))


def parse_all():
    files = read_sources()
    pre = {rel: preprocess(rel, text) for rel, text in files.items()}
    flag_default = {}
    for text in files.values():
        for m in FLAG_BOOL.finditer(text):
            flag_default[m.group(1)] = m.group(2) == "true"

    # Includes, resolved against the file's folder and src/.
    def resolve(rel, inc):
        for cand in (os.path.normpath(os.path.join(os.path.dirname(rel), inc)), os.path.normpath(inc)):
            if cand in pre:
                return cand
        return None

    direct = {rel: [r for r in (resolve(rel, i) for i in p[0]) if r] for rel, p in pre.items()}
    closure = {}

    def incl(rel):
        if rel in closure:
            return closure[rel]
        out, stack = set(), list(direct[rel])
        while stack:
            x = stack.pop()
            if x in out:
                continue
            out.add(x)
            stack.extend(direct[x])
        closure[rel] = frozenset(out)
        return closure[rel]

    macros_by_name = collections.defaultdict(list)
    for rel, (_, macros, _) in pre.items():
        if rel.startswith("base" + os.sep):
            continue
        for name, m in macros.items():
            macros_by_name[name].append((rel, m))

    parsers = {}
    for rel, (_, macros, code) in pre.items():
        vis = incl(rel)

        def lookup(name, rel=rel, vis=vis):
            c = macros_by_name.get(name)
            if not c:
                return None
            for r, m in c:
                if r == rel:
                    return m
            for r, m in c:
                if r in vis:
                    return m
            return None

        toks = expand(tokenize(code), lookup)
        p = Parser(rel)
        p.container(toks, 0, len(toks), [])
        parsers[rel] = p

    defs = []

    def add(d):
        d.id = len(defs)
        defs.append(d)
        if d.kind != "func":
            return
        keep, parts = split_guarded(d.body, not GUARD_ALL)
        if not parts:
            return
        d.body = keep
        for guards, seg in parts:
            c = Def(d.name + "@" + "/".join(f"{a:08X}" for a in guards), d.scope, d.file,
                    "func", seg)
            c.guards, c.parent = guards, d
            d.guarded.append(c)
            add(c)

    for rel in sorted(parsers):
        for d in parsers[rel].defs:
            add(d)
    file_ns = {}
    for rel in parsers:
        m = {}
        for h in incl(rel):
            m.update(parsers[h].ns_aliases)
        m.update(parsers[rel].ns_aliases)
        file_ns[rel] = m
    aliases = {}
    for d in defs:
        analyze_body(d, flag_default, {}, file_ns[d.file])
        if d.flag_alias and d.flag_alias in flag_default:
            aliases[d.name] = flag_default[d.flag_alias]
    if aliases:
        for d in defs:
            d.refs, d.subs, d.imps, d.hexes, d.prints = [], set(), set(), set(), set()
            d.skipped = set()
            analyze_body(d, flag_default, aliases, file_ns[d.file])
    for d in defs:
        if (d.file.replace(os.sep, "/"), d.name) in KEY_TABLES:
            d.hexes = set()

    # Resolution: visible definitions of a name.
    by_name = collections.defaultdict(list)
    for d in defs:
        by_name[d.name].append(d)
    declares = {rel: p.decls for rel, p in parsers.items()}
    vis_cache = {}

    def visible(g, f, name):
        if f == g:
            return True
        key = (g, f, name)
        if key in vis_cache:
            return vis_cache[key]
        ig, if_ = incl(g), incl(f)
        ok = f in ig or any(name in declares[h] for h in (ig & (if_ | {f})))
        vis_cache[key] = ok
        return ok

    edges = []
    unresolved_shared = 0
    for d in defs:
        out = set()
        for qual, name, member in d.refs:
            cands = by_name.get(name)
            if not cands:
                continue
            cs = [c for c in cands if c is not d and visible(d.file, c.file, name)]
            if qual and len(cs) > 1:
                q = [c for c in cs if scope_has(c.scope, qual)]
                if not q:
                    q = [c for c in cs if qual[-1] in c.scope]
                if q:
                    cs = q
            # A name defined in the same file wins over others.
            same = [c for c in cs if c.file == d.file]
            if same and not qual:
                cs = same
            # C++'s lookup: an unqualified name is looked for in the
            # reference's own namespaces, innermost first.
            if not qual and not member and len(cs) > 1:
                best, inner = 0, []
                for c in cs:
                    k = len(c.scope)
                    if 0 < k <= len(d.scope) and list(d.scope[:k]) == list(c.scope):
                        if k > best:
                            best, inner = k, [c]
                        elif k == best:
                            inner.append(c)
                if inner:
                    cs = inner
            if len(cs) > 1:
                unresolved_shared += 1
            out.update(c.id for c in cs)
        edges.append(out)
    return defs, edges, {"files": len(files), "defs": len(defs), "flags": len(flag_default),
                         "aliases": len(aliases), "ambiguous_refs": unresolved_shared,
                         "namespace_aliases": sum(len(p.ns_aliases) for p in parsers.values()),
                         "guarded_parts": sum(1 for d in defs if d.guards)}


def native_edges(is_function, classify_hex):
    """Per sub_X with C++ entries: [(callee, kind)], kind "native" (a call
    or address through the function table), "table" (a function in a table
    or vtable the code names) or "imp" (the original's body); and stats.

    is_function(sub) -> whether sub is a recompiled function's name.
    classify_hex(value) -> [(sub, kind)] for a constant ("native" when it's
    a function's start, "table" for the functions a data table there holds).
    """
    defs, edges, stats = parse_all()
    hex_cache = {}

    def classify(v):
        if v not in hex_cache:
            hex_cache[v] = classify_hex(v)
        return hex_cache[v]

    def guard_kind(d):
        """A guarded part's: "function" (one of its addresses is a
        function's start: the part is that function's C++), "address" (a
        table or vtable: no one's edges) or "plain" (a constant that's no
        address: the part is its parent's)."""
        kinds = set()
        for a in d.guards:
            c = classify(a)
            kinds.add("function" if any(k == "native" for _, k in c) else
                      "address" if c else "plain")
        return "plain" if "plain" in kinds or not kinds else \
            "function" if "function" in kinds else "address"

    entries = collections.defaultdict(set)
    for d in defs:
        if d.adapter:
            entries[d.adapter].add(d.id)
        for s in d.prints:
            entries[s].add(d.id)
        if d.guards and guard_kind(d) == "function":
            for a in d.guards:
                for s, k in classify(a):
                    if k == "native":
                        entries[s].add(d.id)
    owners = collections.defaultdict(set)
    for s, ds in entries.items():
        for x in ds:
            owners[x].add(s)

    def targets(d):
        out = []
        if d.debug and not d.adapter:
            return out
        for s in d.subs:
            if is_function(s):
                out.append((s, "native"))
        for s in d.imps:
            if is_function(s):
                out.append((s, "imp"))
        for v in d.hexes:
            out.extend(classify(v))
        return out

    result = {}
    visited_any = set()
    via = {}
    for sub, starts in entries.items():
        if not is_function(sub):
            continue
        # A part holding a function's print runs inside the code around it
        # (its parent's, outside the other guarded parts).
        starts = set(starts)
        for x in list(starts):
            if defs[x].prints and sub in defs[x].prints:
                p = defs[x].parent
                while p is not None:
                    starts.add(p.id)
                    p = p.parent
        out = {}
        seen = set()
        stack = list(starts)
        while stack:
            x = stack.pop()
            if x in seen:
                continue
            seen.add(x)
            d = defs[x]
            if x not in starts and owners[x] and sub not in owners[x]:
                # A definition several functions share by an argument (a
                # door's side, say) gives the caller all of them; where an
                # address picks which (`case 0xA:`), each part is a
                # definition of its own the caller doesn't walk into.
                for o in owners[x]:
                    if o != sub and is_function(o):
                        out[o] = ("plain", x)
                continue
            # Its guarded parts: a plain constant's are its own, and so is
            # a print a table or vtable address picks (`if (vtable == 0xA)
            # ITC_NATIVE_RUNNING(...)`: no function to reach it through).
            stack.extend(c.id for c in d.guarded if c.id not in seen and (
                guard_kind(c) == "plain" or (guard_kind(c) == "address" and c.prints)))
            if d.debug and not d.adapter:
                continue
            own = d.guards and int(sub[4:], 16) in d.guards
            for c, kind in targets(d):
                if c == sub and not (kind == "imp" and own):
                    # Its own original is the flag-off fallback, except in
                    # a part its address picks (a guest call's trampoline:
                    # `case 0xA: __imp__sub_A(ctx, base)` runs the original).
                    continue
                prev = out.get(c)
                if prev is None or RANK[kind] < RANK[prev[0]]:
                    out[c] = (kind, x)
            stack.extend(e for e in edges[x] if e not in seen)
        visited_any |= seen
        result[sub] = {c: k for c, (k, _) in out.items()}
        via[sub] = {c: defs[x] for c, (_, x) in out.items()}

    # Host code: definitions no native function's C++ reaches that still
    # name guest functions (threads, start-up, the app).
    host = {}
    # Names used only under a flag off by default (the multiplayer bypass's
    # calls, say): not host code.
    skipped = set().union(*(d.skipped for d in defs))
    used = {name for d in defs for _, name, _ in d.refs}
    for d in defs:
        if d.id in visited_any or (d.debug and not d.adapter) or d.adapter:
            continue
        if d.name in skipped and d.name not in used:
            continue
        if d.guards and guard_kind(d) != "plain":
            continue  # another function's C++, or no one's
        t = [(c, k) for c, k in targets(d) if not (k == "imp")]
        if t:
            host[d.id] = t
    # Only those not reached from another host definition: report them.
    stats["entries"] = sum(1 for s in entries if is_function(s))
    stats["host_defs"] = len(host)
    return result, via, host, defs, entries, stats


if __name__ == "__main__":
    defs, edges, stats = parse_all()
    print(stats)
    by = collections.defaultdict(list)
    for d in defs:
        if d.adapter:
            by[d.adapter].append(d)
        for s in d.prints:
            by[s].append(d)
    for a in sys.argv[1:]:
        for d in by.get(a, []):
            print(a, d.file, d.name, d.kind, "subs", sorted(d.subs), "imps", sorted(d.imps),
                  "hexes", [hex(h) for h in sorted(d.hexes) if h >= 0x82000000][:30],
                  "refs", len(edges[d.id]), [defs[e].name for e in list(edges[d.id])[:20]])
