
#include "diag.h"
#include "genesis_runtime.h"
#include "genesis_machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
/* Observational counters only: existing diagnostic behavior is retained. */
extern uint64_t prof_diag_bytes,prof_diag_writes,prof_diag_opens,prof_diag_hash_bytes;
static size_t prof_fwrite(const void*p,size_t size,size_t n,FILE*f){size_t r=fwrite(p,size,n,f);prof_diag_bytes+=r*size;prof_diag_writes++;return r;}
static FILE*prof_fopen(const char*p,const char*m){prof_diag_opens++;return fopen(p,m);}
#define fwrite prof_fwrite
#define fopen prof_fopen
extern uint64_t g_frame_count;
extern uint32_t g_cycle_accumulator,g_audio_cycle_counter;
extern uint32_t m68k_interp_hot_count(uint32_t pc);
extern uint64_t m68k_interp_move_predec_account_count(void);
extern uint64_t m68k_interp_move_predec_saved_cycles(void);
extern uint64_t g_native_insn_count;
extern uint64_t g_recompiled_insn_count;
extern uint64_t g_interpreted_insn_count;
#pragma pack(push,1)
typedef struct {uint32_t pc,kind,metadata_pc,sr,usp,D[8],A[8],cycles,audio,master,scanline;uint64_t frame,ramhash,vramhash;} Record;
typedef struct {uint32_t seq,pc,kind,addr,width,value,postbyte;uint64_t frame;} Access;
#pragma pack(pop)
static int heavy_enabled;
static FILE *tr,*events,*watch;static uint32_t active,count,lastpc,lastkind;static int initialized;static uint32_t limit=500000;static FILE*control;static FILE*mailbox;
static uint64_t hash(const uint8_t*p,size_t n){prof_diag_hash_bytes+=n;uint64_t h=14695981039346656037ull;while(n--){h^=*p++;h*=1099511628211ull;}return h;}
static void closefiles(void){if(tr)fclose(tr);if(events)fclose(events);if(watch)fclose(watch);if(control)fclose(control);if(mailbox)fclose(mailbox);}
static void initialize(void){if(initialized)return;initialized=1;const char*heavy=getenv("SOR_HEAVY_DIAG");heavy_enabled=(heavy&&strcmp(heavy,"1")==0);mailbox=fopen("watch-fa00.bin","wb");const char*cap=getenv("SOR_DIAG_LIMIT");if(cap){unsigned long v=strtoul(cap,0,10);if(v>0&&v<=2000000)limit=(uint32_t)v;}watch=fopen("watch-ffb801.bin","wb");atexit(closefiles);}
void diag_before(uint32_t pc,uint32_t kind){
 initialize();lastpc=pc;lastkind=kind;

 if(active && (pc==0x010798u || pc==0x0003b2u)){
  char path[80];snprintf(path,sizeof path,"endpoint-%06X-%u.bin",pc,count);
  FILE*f=fopen(path,"wb");fwrite(&g_cpu,1,sizeof g_cpu,f);fwrite(g_ram,1,65536,f);fwrite(g_machine.vdp.vram,1,65536,f);fclose(f);
 }
 if(active && pc==0x010798u){
  uint32_t sp=g_cpu.A[7]&0x00FFFFFFu;
  uint32_t off=sp&0xFFFFu;
  uint32_t ret=((uint32_t)g_ram[off]<<24) |
               ((uint32_t)g_ram[(off+1)&0xFFFFu]<<16) |
               ((uint32_t)g_ram[(off+2)&0xFFFFu]<<8) |
               ((uint32_t)g_ram[(off+3)&0xFFFFu]);
  fprintf(stderr,
          "[RTS10798] SP=$%06X RAM_LONG=$%08X cycles=%u frame=%llu\\n",
          sp,ret,g_cycle_accumulator,
          (unsigned long long)g_frame_count);
 }
 if(heavy_enabled && !active && pc==0x106ea){
 static const uint32_t move_pcs[]={
  0x000266,0x003E38,0x0095E0,0x0095FA,
  0x00ADA4,0x00AE18,0x0195F6,0x019ED4,
  0x01CABE,0x01ECFA,0x01ED00,0x026856,
  0x072DCE,0x072EA8
 };
 fprintf(stderr,"[MOVE-ANCHOR] PC=$0106EA cycles=%u\\n",g_cycle_accumulator);
 fprintf(stderr,
         "[MOVE-ACCOUNT] predec_count=%llu saved_cycles=%llu\\n",
         (unsigned long long)m68k_interp_move_predec_account_count(),
         (unsigned long long)m68k_interp_move_predec_saved_cycles());
 fprintf(stderr,
         "[BACKEND-ANCHOR] native=%llu recompiled=%llu interpreted=%llu\\n",
         (unsigned long long)g_native_insn_count,
         (unsigned long long)g_recompiled_insn_count,
         (unsigned long long)g_interpreted_insn_count);
 for(unsigned i=0;i<sizeof(move_pcs)/sizeof(move_pcs[0]);i++)
  fprintf(stderr,"[MOVE-ANCHOR] target=$%06X count=%u\\n",
          move_pcs[i],m68k_interp_hot_count(move_pcs[i]));
 active=1;control=fopen("control.bin","wb");diag_save_hidden();tr=fopen("trace.bin","wb");events=fopen("access.bin","wb");
 FILE*vdp=fopen("anchor.vdp-hardware","wb");fwrite(&g_machine.vdp,1,offsetof(GVDP,bus_read),vdp);fclose(vdp);FILE*f=fopen("anchor.ram","wb");fwrite(g_ram,1,65536,f);fclose(f);f=fopen("anchor.vram","wb");fwrite(g_machine.vdp.vram,1,65536,f);fclose(f);}
 if(!active||count>=limit)return;
 if(count<8){char path[80];FILE*f;
 snprintf(path,sizeof path,"step-%08u.ram",count);f=fopen(path,"wb");fwrite(g_ram,1,65536,f);fclose(f);
 snprintf(path,sizeof path,"step-%08u.vram",count);f=fopen(path,"wb");fwrite(g_machine.vdp.vram,1,65536,f);fclose(f);
 snprintf(path,sizeof path,"step-%08u.vdp",count);f=fopen(path,"wb");fwrite(&g_machine.vdp,1,offsetof(GVDP,bus_read),f);fclose(f);}
 Record r={0};r.pc=pc;r.kind=kind;r.metadata_pc=g_cpu.PC;r.sr=g_cpu.SR;r.usp=g_cpu.USP;memcpy(r.D,g_cpu.D,sizeof r.D);memcpy(r.A,g_cpu.A,sizeof r.A);
 r.cycles=g_cycle_accumulator;r.audio=g_audio_cycle_counter;r.master=g_machine.master_cycle;r.scanline=g_machine.vdp.scanline;r.frame=g_frame_count;r.ramhash=(count<25000)?hash(g_ram,65536):UINT64_MAX;r.vramhash=(count<25000)?hash(g_machine.vdp.vram,65536):UINT64_MAX;
 fwrite(&r,sizeof r,1,tr);count++;
}
static void note(uint32_t mode,uint32_t a,uint32_t width,uint32_t value,uint32_t origin){
 initialize();a&=0xffffff;Access e={count,origin,mode,a,width,value,g_ram[0xb801],g_frame_count};
 if(active&&count<limit)fwrite(&e,sizeof e,1,events);
 if(mode==1&&a<=0xfffa00&&a+width>0xfffa00)fwrite(&e,sizeof e,1,mailbox);
 if(mode==1&&a<=0xffb801&&a+width>0xffb801)fwrite(&e,sizeof e,1,watch);
}
#define READ(bits,type) extern type diag_original_read##bits(uint32_t); type m68k_read##bits(uint32_t a){uint32_t origin=lastpc;type v=diag_original_read##bits(a);note(0,a,bits/8,v,origin);return v;}
#define WRITE(bits,type) extern void diag_original_write##bits(uint32_t,type);void m68k_write##bits(uint32_t a,type v){uint32_t origin=lastpc;diag_original_write##bits(a,v);note(1,a,bits/8,v,origin);}
READ(8,uint8_t) READ(16,uint16_t) READ(32,uint32_t)
WRITE(8,uint8_t) WRITE(16,uint16_t) WRITE(32,uint32_t)

void diag_control(uint32_t event,uint32_t argument){
 if(!active||(event==1&&count>=limit))return;
 #pragma pack(push,1)
 struct {uint32_t event,argument,seq,pc,sr,cycles,audio,master,scanline,vdp_flags;uint64_t frame;} r;
 #pragma pack(pop)
 r.event=event;r.argument=argument;r.seq=count;r.pc=lastpc;r.sr=g_cpu.SR;r.cycles=g_cycle_accumulator;r.audio=g_audio_cycle_counter;r.master=g_machine.master_cycle;r.scanline=g_machine.vdp.scanline;r.vdp_flags=g_machine.vdp.vint_pending|(g_machine.vdp.in_vblank<<8);r.frame=g_frame_count;
 fwrite(&r,sizeof r,1,control);
}
