# m68k-recomp-core

Shared clean-room Motorola 68000-family static-recompiler frontend used by
[segagenesisrecomp](https://github.com/mstan/segagenesisrecomp) and
[cdirecomp](https://github.com/mstan/cdirecomp).

The initial extraction deliberately preserves the two validated policy
profiles while centralizing the genuinely common instruction layer:

- `common/` contains the decoder, validator, and annotation reader.
- `profiles/genesis/` preserves the mature plain-68000 Genesis discovery and
  code-emission policy.
- `profiles/scc68070/` preserves CD-i's SCC68070 timing, exception, OS-9 trap,
  async-resume, and guest-stack behavior.

This profile split is a reconciliation boundary, not a claim that duplicated
emitter/finder code is permanently desirable. Changes should first be made in
shared code when semantics are CPU-family-wide. Profile code is reserved for a
demonstrated CPU, generated-runtime-ABI, or platform-policy difference.

Consumers include `cmake/M68kRecompCore.cmake`, select a profile with
`m68k_recomp_core_sources()`, and provide their own `rom_parser.h`,
`game_config.h`, timing/oracle dependencies, CLI, and generated runtime.

The repository contains no Clown68000, ClownMDEmu, CeDImu, ROM, BIOS, disc, or
generated game code. Optional third-party instruction oracles remain in the
consumer development repositories and are not dependencies of this source
package.

## Reconciliation direction

1. Keep common decoder/validator semantics covered by synthetic tests from
   both consumers.
2. Replace `GenesisRom` with a callback-backed neutral image interface.
3. Move timing and exception behavior behind explicit CPU profiles.
4. Reconcile function discovery, retaining Genesis evidence tracking and
   CD-i's full 24-bit address-space/OS-9 requirements.
5. Reconcile the emitter around a neutral generated-runtime ABI and extension
   hooks, then remove the duplicated profile implementations.
6. Extract the clean-room fallback interpreter once its runtime callbacks are
   similarly explicit.
