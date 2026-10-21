# Provenance

This repository was seeded on 2026-07-18 from
[`smsggrecomp`](https://github.com/mstan/smsggrecomp) feature commit `6d93016`.
That branch added the flat single-instruction output used by the Genesis sound
CPU experiment while retaining the same packed state and verified semantic
helpers used by SMS and Game Gear recompilations.

The initial shared files are:

- `include/sms_runtime.h`: author-owned state layout and generated host ABI,
  extracted without behavioral changes.
- `include/z80_ops.h`: semantic helpers derived from the MIT-licensed
  SuperZazu Z80 core by Nicolas Allemand. The original MIT notice is included
  at `LICENSES/SUPERZAZU-MIT.txt`.

The first consumers are `smsggrecomp:feature/genesis-z80-step` and
`segagenesisrecomp:feature/z80-static-apu`. Platform memory maps, schedulers,
interrupt controllers, interpreters, ROM captures, and generated game code
remain in the consumer repositories.

