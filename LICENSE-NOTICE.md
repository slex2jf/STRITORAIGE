# License and attribution notice

No single repository-wide license is asserted. The upstream segagenesisrecomp framework, including its runner, Genesis-specific recompiler CLI/ROM parser/TOML configuration, and CMake integration, is licensed under PolyForm Noncommercial License 1.0.0, as explicitly stated in its README and root LICENSE.md. This distribution is noncommercial; use and redistribution of the PolyForm-covered components must comply with that license. The root [LICENSE.md](LICENSE.md) supplies the PolyForm terms for those upstream components and for original project contributions covered by [SOR1-CONTRIBUTIONS.md](SOR1-CONTRIBUTIONS.md), not automatically for every file in this repository. Generated Motorola 68000 instruction translations derive from analysis of the user's game; the technical audit does not establish publication rights.

| Component | Notice found locally |
| --- | --- |
| m68k-recomp-core, z80-recomp-core | PolyForm Noncommercial License 1.0.0, Matthew Stan, 2026; retained LICENSE files |
| rbengine, recomp-net | MIT, respective contributors; retained LICENSE files |
| ymfm | BSD 3-Clause, Aaron Giles; retained LICENSE |
| superzazu Z80 | MIT, Nicolas Allemand; retained LICENSE |
| minicoro | Alternative Unlicense or MIT No Attribution; retained LICENSE |
| SDL 2.28.5 (external on Linux, bundled Windows development distribution) | zlib license; Windows dependencies/SDL2/COPYING.txt |
| segagenesisrecomp framework (`framework/runner/`, `framework/recompiler/`, `framework/cmake/`) | PolyForm Noncommercial License 1.0.0 for upstream project code; root LICENSE.md and upstream README license declaration; third-party exceptions remain separate |
| clowncommon | ISC-style permissive license, Copyright (c) 2021-2023 Clownacy; framework/runner/external/clowncommon/LICENCE.txt retained |
| tomlc99 (`framework/recompiler/src/toml.c`, `toml.h`) | MIT, CK Tan; embedded license notices retained |
| ShadowVerifier and color-science core | MIT OR Apache-2.0, JRickey; adapted through gba/snes implementations; see retained headers and THIRD-PARTY-LICENSES.md |
| Musashi | No Musashi implementation is selected in this runtime; m68k_interp.c identifies itself as a clean-room interpreter mirroring the generator |
| GCC C++/compiler runtime in Windows EXE | GCC runtime linking, GCC Runtime Library Exception; local toolchain copyright notice retained in Windows copy |

The root [THIRD-PARTY-LICENSES.md](THIRD-PARTY-LICENSES.md) is an unchanged copy of the upstream framework inventory. Its paths are relative to the upstream framework root (normally `framework/` here); references to optional launcher/release files describe upstream packaging, not necessarily files included in this snapshot.

Original SOR1 project contributions are declared under PolyForm Noncommercial License 1.0.0 as specified in [SOR1-CONTRIBUTIONS.md](SOR1-CONTRIBUTIONS.md), using the provisional designation "Project contributor". This includes original local additions within modified framework, generator, runtime, and frontend files, as well as original project-specific integration, configuration, scripts, documentation, and tests. Upstream and third-party portions retain their own rights and licenses. The declaration grants only rights held in the original contributions; it does not independently prove authorship or ownership.

All existing copyright/license headers remain intact. This inventory is not a legal clearance or a blanket MIT license.

Streets of Rage / Bare Knuckle, Sega and related marks belong to their respective owners. This project is not affiliated with, sponsored by or approved by Sega. No ROM, commercial asset package or complete copy of the game is supplied. Users must provide their own legally obtained ROM.

Neither the framework license nor this audit grants rights to Streets of Rage game code, ROM contents, graphics, music, or other Sega assets.
