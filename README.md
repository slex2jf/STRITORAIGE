# Streets of Rage 1 — Experimental Native/Hybrid Recompilation

Experimental, unofficial recompilation of Motorola 68000 game logic for Mega Drive / Genesis. The current hybrid candidate combines 701 translated functions with an interpreted fallback and a native SDL audio/video/input runtime. It is not 100% native and does not distribute the commercial game.

The historical strict 500/1000/3515/10000 frame comparisons PASS; Cases 1–4 PASS (474 / 749 / 1129 / 742). In the measured 10,000-frame scenario, 113,957,151 of 117,444,667 instructions were recompiled (97.03050288%), and 3,487,516 were interpreted (2.96949712%); misses 39,150. Misses increased relative to the earlier candidate. The demonstrated improvement is reduced interpreted volume. These percentages do not describe the whole game.

The full interpreter was human-tested smoothly through Stage 7, where the player died. The final hybrid candidate awaits fresh human validation; a prior hybrid hung in Stage 2. Neither resolution of that human issue nor completion of the game has been established.

## Linux build and run

Requires CMake >=3.19, a C11/C++17 GCC-compatible toolchain, pkg-config, Python 3 and SDL2 development files. No ROM is needed to compile the audited translations.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
python3 scripts/run.py
```

Place your own legally obtained ROM at `rom/Streets of Rage.bin` (524288 bytes). Expected SHA-256: `dd44f120446654bb91c448762f3e0cd0d9b034f35d0e3266a4dc34402ada95c0`. The launcher aborts on missing or mismatching ROM. No download is provided.

## Structure and validation

`src/` contains the frontend/game specification; `runtime/` the candidate hybrid bridge/interpreter and ABI override; `generated/` the 701-function instruction translations and dispatch/cycle metadata; `framework/` the runner and supporting libraries; `config/` the generator game settings; `tests/smoke.input` the measured input sequence; `scripts/validate.py` compares frame hashes of interpreter and hybrid; `docs/` records evidence and audit limits. Essential files are independent regular copies.

```sh
python3 scripts/validate.py
# For supported Mesa/SDL installations, use SDL_VIDEODRIVER=offscreen. Physical controllers are filtered by the validation script.
```

Validation outputs stay in ignored `build-validation/`. The script checks exact requested row counts and CPU/RAM/VRAM/CRAM/VSRAM/register/VDP/master/cycle hashes. Cases 1–4 are recorded historical results; private RAM/VRAM fixtures are deliberately excluded and those four tests cannot be rerun from this public tree alone. See docs/VALIDATION.md, docs/TECHNICAL_NOTES.md and docs/PUBLICATION_AUDIT.md.

## Limitations and roadmap

Timing equivalence is against the present generator/reference policy, not proof of physical hardware cycle accuracy. Translation metadata and literal instruction immediates remain ROM-derived logic; the audit is technical, not legal clearance. Perform human hybrid gameplay validation, investigate remaining fallback coverage, and test the Windows binary on real Windows 10/11 x64.

Streets of Rage / Bare Knuckle, Sega and related marks belong to their respective owners. The project is not affiliated with, sponsored by or approved by Sega. No ROM, commercial assets or complete game copy is supplied. Framework components from segagenesisrecomp are subject to PolyForm Noncommercial License 1.0.0; this distribution is noncommercial. Third-party components retain their respective licenses. See [LICENSE.md](LICENSE.md), [LICENSE-NOTICE.md](LICENSE-NOTICE.md), [THIRD-PARTY-LICENSES.md](THIRD-PARTY-LICENSES.md), and [SOR1-CONTRIBUTIONS.md](SOR1-CONTRIBUTIONS.md) for the limited declaration covering original local contributions under PolyForm Noncommercial License 1.0.0. These notices grant no rights to Sega game code, ROM contents, graphics, music, or other assets.

## Local regeneration

```sh
cmake -S framework/recompiler -B build-generator -DCMAKE_BUILD_TYPE=Release
cmake --build build-generator -j4
build-generator/GenesisRecomp "rom/Streets of Rage.bin" --game config/game.toml --output-dir build-regenerated
```

These commands were evaluated on Linux using the matching ROM read from its private external path. Outputs reproduce all 701 translated functions (the discovery log reports 537 initial functions before further codegen discovery). Generated path comments may differ. Never stage ROMs or private generated memory snapshots.

---

# Download and Play

Prebuilt experimental versions of STRITORAIGE are available for **Linux x86-64** and **Windows x86-64** on the GitHub Releases page.

The original Streets of Rage ROM is **not included**. You must provide your own legally obtained copy.

Required ROM SHA-256: `dd44f120446654bb91c448762f3e0cd0d9b034f35d0e3266a4dc34402ada95c0`

## Linux - How to run

Download `StreetsOfRage1-Recomp-Linux.zip` from the GitHub Releases page and extract it completely.

The main files are organized like this:

- `bin/STRITORAIGE` - Linux x86-64 game executable.
- `rom/` - folder where you must place your ROM.
- `run.sh` - recommended launcher used to start STRITORAIGE.

### ROM location

Open the `rom` folder and place your legally obtained ROM there with the exact filename:

`Streets of Rage.bin`

The complete ROM path must be:

`StreetsOfRage1-Recomp-Linux-Portable/rom/Streets of Rage.bin`

### Starting the game

Return to the main `StreetsOfRage1-Recomp-Linux-Portable` folder and open a terminal there.

Run:

`./run.sh`

The actual Linux executable is located at:

`bin/STRITORAIGE`

However, using `run.sh` is recommended because the launcher prepares and starts STRITORAIGE correctly.

If Linux reports a permission error, run:

`chmod +x run.sh`

`chmod +x bin/STRITORAIGE`

Then start the game again with:

`./run.sh`

## Windows - How to run

Download `StreetsOfRage1-Recomp-Windows.zip` from the GitHub Releases page and extract it completely.

The main files are organized like this:

- `STRITORAIGE.exe` - Windows x86-64 game executable.
- `SDL2.dll` - required runtime library. Keep it in the same folder as `STRITORAIGE.exe`.
- `rom/` - folder where you must place your ROM.
- `run.bat` - recommended launcher used to start STRITORAIGE.

### ROM location

Open the `rom` folder and place your legally obtained ROM there with the exact filename:

`Streets of Rage.bin`

The complete ROM path must be:

`StreetsOfRage1-Recomp-Windows-Portable/rom/Streets of Rage.bin`

### Starting the game

Return to the main `StreetsOfRage1-Recomp-Windows-Portable` folder.

Double-click `run.bat` to start the game.

The actual Windows executable is located in the main folder:

`STRITORAIGE.exe`

Using `run.bat` is recommended because it verifies the ROM before launching `STRITORAIGE.exe`.

Do not remove or move `SDL2.dll` from the STRITORAIGE folder.

## Quick reference

Linux:
- ROM: `rom/Streets of Rage.bin`
- Executable: `bin/STRITORAIGE`
- Start with: `./run.sh`

Windows:
- ROM: `rom/Streets of Rage.bin`
- Executable: `STRITORAIGE.exe`
- Start with: `run.bat`

The original game ROM is not distributed with STRITORAIGE.
