# z80-recomp-core

Shared Zilog Z80 static-recompiler runtime contract and verified instruction
semantics used to support:

- [SMS/GG Recomp](https://github.com/mstan/smsggrecomp), where the Z80 is the
  console's main CPU.
- [Sega Genesis Recomp](https://github.com/mstan/segagenesisrecomp), where the
  Z80 runs the cartridge sound driver as the audio coprocessor.

The initial extraction deliberately preserves the validated generated-code ABI:

- `include/sms_runtime.h` defines the packed `Z80State`, flag layout, register
  accessors, and host callback/global surface used by generated code.
- `include/z80_ops.h` provides header-only ALU, flag, rotate, shift, bit, and
  miscellaneous instruction semantics.

The `sms_*` callback names are retained for generated-source compatibility;
they describe host-provided Z80 memory/I/O operations and are not a requirement
that the consumer emulate Master System hardware. Genesis supplies those hooks
with its Z80 RAM, YM2612, PSG, BUSREQ, reset, and interrupt scheduler.

Consumers add this repository as a submodule and either link its interface
target or add `include/` to the generated target:

```cmake
add_subdirectory(external/z80-recomp-core)
target_link_libraries(my_generated_z80 PRIVATE z80-recomp-core::core)
```

The decoder and C emitter currently remain in `smsggrecomp`; this repository is
the platform-neutral runtime/semantic layer their output consumes. It contains
no ROM, captured Z80 RAM, generated game source, emulator, or platform bus.

## Licensing and provenance

Author-owned extraction/glue is licensed under the included PolyForm
Noncommercial License 1.0.0. Instruction semantic helpers were derived from
Nicolas Allemand's MIT-licensed SuperZazu Z80 core; its notice is retained in
`LICENSES/SUPERZAZU-MIT.txt`. See `PROVENANCE.md` for extraction details.

