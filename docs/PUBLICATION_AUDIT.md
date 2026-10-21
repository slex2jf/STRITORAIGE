# Publication technical audit

```json
{
  "date": "2026-10-21 America/Sao_Paulo",
  "scope": "distribution payload; excludes .git and build/build-* artifacts",
  "files": 385,
  "bytes": 10898209,
  "ROM_PRESENT": "NO",
  "issues": [],
  "binary_files": [],
  "large_files_over_1MiB": [],
  "large_byte_arrays_found": [
    [
      "framework/runner/external/minicoro/minicoro.h",
      5491
    ]
  ],
  "reviewed_arrays": "minicoro host context switch/wrapper machine code; not game ROM; bundled license retained",
  "reference_sha": "dd44f120446654bb91c448762f3e0cd0d9b034f35d0e3266a4dc34402ada95c0",
  "checks": "Every payload file hashed; full ROM subsequence, exact ROM SHA, 524288 size, extensions/archives/dumps, header and aligned high-entropy 256-byte ROM chunks checked. Textual SHA/name allowed. Renamed raw ROM detected. No exhaustive proof against arbitrary encoding/compression/encryption.",
  "generated_C": "701 function declarations; translated instructions/immediates. Three const metadata arrays: dispatch, hook sites and instruction cycle costs. No byte asset arrays detected. No legal conclusion."
}
```

Counts include the prior version of this report if present; final independent payload inventory is recorded outside the distribution. Binary SDL/PE files, if present, are the documented runtime components, not ROM content. License clearance is separate and unresolved components remain identified in LICENSE-NOTICE.md.
