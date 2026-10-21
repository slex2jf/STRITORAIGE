# Technical notes

The historical +184 timing divergence came from estimator policy differences: 38 × 2 + 9 × 12 = 184. General corrections cover ADD/SUB/AND/OR, ADDA/SUBA, immediate operations, EOR and NEG/NEGX/NOT. MUL follows the generator's average/static policy; DIVS/DIVU use its static policy. This establishes equivalence to the current generator/reference policy, not physical hardware cycle-exact behavior.

Nested RTS ownership prevents a split/callee return from being consumed by the wrong frame. Capsule terminal RTS ownership assigns the terminal stack-pop/return boundary to the owning capsule rather than duplicating return handling. General Dn -> An discovery expanded 414 -> 701 functions (+287) without adding seeds. ABCD/SBCD distinguish register from predecrement-memory timing. Callback discovery includes 008254, 00825E, 00826A, 008274. No changes have been propagated to any global framework.

The large byte arrays in vendored minicoro are host context-switch/wrapper machine code (_mco_switch_code and _mco_wrap_main_code), not guest ROM bytes. They remain under the bundled minicoro license.
