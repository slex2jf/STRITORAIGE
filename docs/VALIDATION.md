# Validation

Historical reference: Linux executable SHA-256 `6f713a24afdfb3adca6f871ff7f78e9aecdb8f55b97292753394e8acce494c0a`; source inputs preserved in this independent copy, with relative build paths.

Cases 1–4: 474 / 749 / 1129 / 742 CPU/WRAM comparisons, 4 passed, 0 failed (regression-capsule-final.log in original; summarized here). Private memory fixtures are not distributed. Strict 500, 1000, 3515 and 10000 PASS are evidenced by final-validation-matrix.json. These are historical evidence, separate from fresh copy tests reported below.

701 functions; total 117444667; recompiled 113957151; interpreted 3487516; 97.03050288% and 2.96949712%; misses 39150. These belong only to the measured 10k scenario, not absolute coverage of the entire game. Misses increased relative to the prior candidate. See final-backend-share.json.

Full-interpreter human testing reached Stage 7, player died. No complete-game validation. The final hybrid still awaits human testing, including the earlier Stage 2 hang scenario.

Reproduce important strict comparisons with scripts/validate.py and a matching user-supplied ROM. It rejects truncated runs, unequal row counts and differences in all listed state/timing fields. Original fixture Cases cannot be rerun without privately supplied snapshots.

## Fresh independent-copy verification

Linux clean build PASS. Strict 500/1000/3515/10000 PASS with SDL offscreen and physical controllers filtered. See COPY_TEST_RESULTS.json. Windows cross-build PASS, runtime NOT tested. Generator built locally; regeneration from externally read matching ROM produced the same 701-function C/H set after stripping comments (source path comments differ).
