\page kwk_finalization_roadmap Finalization Roadmap

# KIWAKU Finalization Roadmap

This page tracks where KIWAKU stands and what remains to reach "done". The authoritative, up-to-date
task list is the `plan.md` / `report.html` pair in the repository root; this page is a prose summary
of the same picture for readers landing here from the architecture docs.

The library is **built and green**: `plain` (release-style) and `Debug` builds compile with
`-Werror`, the Doxygen job is warning-free, and the unit suite (28 tests) passes. The architecture
described on these pages is the **current** one — `element` → `scheme` → `blob` → `layout` →
`accessor` → `options`/`builder` → `collection`/`table`/`view` are all implemented, tested and
wired together. What remains is feature depth, not scaffolding.

## Where the library is

| layer | status |
|---|---|
| `blob` (stack / heap / shallow storage, `as_storage` backend trait) | done |
| `element` (per-point kind + byte offset) and `resolve` helper | done |
| `scheme` (per-point offset map over a blob, a kumi record) | done |
| `shape` / `stride` (hybrid static/dynamic, `cosize`, `to_stride`) | done |
| `storage_order` (row-major / column-major / arbitrary generator) and the real-order printer | done |
| `accessor` (coordinate → boundary → interpolator pipeline, `down`/`up`) | done |
| `options` bag and `builder` (defaulting, deduction, storage synthesis, SoA offsets) | done |
| `collection` / `table` / `view` (scalar, slicer, tiler, SoA `get<I>`, interpolating) | done |
| boundaries (`clamp` / `tile` / `mirror`) and interpolators (`none` / `nearest` / `linear` / `floor` / `ceil` / `round`) | done |
| phantom strategies (`morton` / `hilbert` space-filling curves) | reserved (empty tags) |

## What is still open

1. **Per-field geometry (the *sheaf* regime) — Phase K, the active work.** Today a product (record)
   `kind` shares **one** shape across all fields: structure-of-arrays with a single uniform layout
   (the *fiber-bundle* regime). The remaining milestone lets each field carry its **own** shape and
   extents — jagged / Iliffe-orientation storage for heterogeneous fields — while still collapsing to
   the byte-identical SoA fast path when shapes happen to coincide. This is the genuine sheaf side of
   the fiber-bundle / sheaf split described in `foundations` and `scheme`.
2. **Real per-field `congruent` check.** The builder's congruence logic is currently shape-wide; with
   per-field geometry it becomes a real, per-field compatibility check (this replaces the former
   `congruent` placeholder).
3. **Space-filling coordinates.** `morton_t` / `hilbert_t` are kept as zero-cost strategy tags but not
   yet implemented; `to_stride`'s arbitrary `storage_order` hook is the integration point.
4. **Expansion of the test suite** alongside Phase K (per-field shapes, jagged access, sheaf `size()`
   per the container maths).

## Landing order

```
Phases A–J (build, core, accessor, SoA, audit)  ..... DONE
   └─ Phase K — per-field / sheaf geometry        ..... NEXT
        ├─ per-field extents + builder offsets
        ├─ get<I> per-field shape/stride
        ├─ jagged size()/shape()
        ├─ real per-field congruent
        └─ tests + docs
```

Everything lands in small increments; after each increment the project compiles and the suite passes.
The `plan.md` checklist (items ticked `[x]`) and `report.html` (one row per landed PR, with the
per-commit diff/header under `notes/pr/`) are the ground truth for what is done.
