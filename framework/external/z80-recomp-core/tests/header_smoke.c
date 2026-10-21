#include "z80_ops.h"

int main(void)
{
    Z80State s = {0};
    s.a = 0x7F;
    return z80_add8(&s, s.a, 1, false) == 0x80 &&
           (s.f & (Z80_FLAG_S | Z80_FLAG_H | Z80_FLAG_P)) ==
           (Z80_FLAG_S | Z80_FLAG_H | Z80_FLAG_P)
           ? 0 : 1;
}
