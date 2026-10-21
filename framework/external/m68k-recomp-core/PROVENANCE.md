# Provenance

The author-owned frontend originated in `segagenesisrecomp`. CD-i copied the
frontend from `segagenesisrecomp` commit `5aa0c4f` on 2026-05-28 and developed
SCC68070/OS-9 behavior independently afterward.

This repository was seeded on 2026-07-17 from:

- Genesis profile: `segagenesisrecomp` commit `cb1a686`.
- SCC68070 profile: `cdirecomp` commit `09136eb`.
- Common annotations: byte-identical in both consumers at extraction.
- Common decoder/validator: the CD-i superset, which adds validated MOVEC
  decoding required by SCC68070 while retaining the shared 68000 instruction
  set.

The clean-room sources are copyright Matthew Stan and licensed under the
included PolyForm Noncommercial License 1.0.0. No Clown68000/ClownMDEmu or
CeDImu source is included.

