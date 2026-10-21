#include "genesis_runtime.h"
#include "genesis_machine.h"
#include "cosim.h"
#include <stdio.h>
#include <stdlib.h>
extern uint64_t g_native_insn_count,g_recompiled_insn_count,g_interpreted_insn_count;
extern uint32_t g_cycle_accumulator,g_miss_count_any;
static FILE*frames;
static uint64_t bytes(const void*p,size_t n){return cosim_fnv_bytes(cosim_fnv_init(),p,n);}
static void close_hashes(void){if(frames)fclose(frames);}
static uint64_t hash_vdp(void) {
    const GVDP *v = &g_machine.vdp;
    uint64_t h = cosim_fnv_init();
    h = cosim_fnv_bytes(h, v->vram, sizeof v->vram);
    for (int i = 0; i < GVDP_CRAM_ENTRIES;  i++) h = cosim_fnv_u16(h, v->cram[i]);
    for (int i = 0; i < GVDP_VSRAM_ENTRIES; i++) h = cosim_fnv_u16(h, v->vsram[i]);
    h = cosim_fnv_bytes(h, v->reg, sizeof v->reg);
    h = cosim_fnv_u8(h, v->control_pending);
    h = cosim_fnv_u8(h, v->code);
    h = cosim_fnv_u16(h, v->address);
    h = cosim_fnv_u8(h, v->address_hi);
    h = cosim_fnv_u8(h, v->in_vblank);
    h = cosim_fnv_u8(h, v->in_hblank);
    h = cosim_fnv_u8(h, v->dma_active);
    h = cosim_fnv_u8(h, v->sprite_overflow);
    h = cosim_fnv_u8(h, v->sprite_collision);
    h = cosim_fnv_u8(h, v->vint_pending);
    h = cosim_fnv_u16(h, v->hint_counter);
    h = cosim_fnv_u16(h, v->scanline);
    h = cosim_fnv_u8(h, v->dma_fill_pending);
    /* funcptrs (bus_read/colour_updated) + their user ptrs EXCLUDED */
    return h;
}void state_hash_frame(unsigned f){static int init,enabled;if(!init){init=1;const char*e=getenv("SOR_STATE_HASHES");enabled=e&&e[0]=='1';if(enabled){frames=fopen("state-frames.csv","w");if(frames){fprintf(frames,"frame,cpu,pc,ram,vram,cram,vsram,regs,vdp,master,cycles,total,recompiled,interpreted,misses\n");atexit(close_hashes);}}}if(!frames||f>=10000)return;
uint64_t h=cosim_fnv_init();for(int i=0;i<8;i++)h=cosim_fnv_u32(h,g_cpu.D[i]);for(int i=0;i<8;i++)h=cosim_fnv_u32(h,g_cpu.A[i]);h=cosim_fnv_u16(h,g_cpu.SR);h=cosim_fnv_u32(h,g_cpu.USP);
fprintf(frames,"%u,%016llx,%06x,%016llx,%016llx,%016llx,%016llx,%016llx,%016llx,%u,%u,%llu,%llu,%llu,%u\n",f,(unsigned long long)h,g_cpu.PC,(unsigned long long)bytes(g_ram,65536),(unsigned long long)bytes(g_machine.vdp.vram,65536),(unsigned long long)bytes(g_machine.vdp.cram,sizeof g_machine.vdp.cram),(unsigned long long)bytes(g_machine.vdp.vsram,sizeof g_machine.vdp.vsram),(unsigned long long)bytes(g_machine.vdp.reg,sizeof g_machine.vdp.reg),(unsigned long long)hash_vdp(),g_machine.master_cycle,g_cycle_accumulator,(unsigned long long)g_native_insn_count,(unsigned long long)g_recompiled_insn_count,(unsigned long long)g_interpreted_insn_count,g_miss_count_any);
}
