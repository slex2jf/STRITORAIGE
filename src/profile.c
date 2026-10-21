#include "profile.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAP 131072
#define NPC 18
extern uint64_t g_native_insn_count,g_interpreted_insn_count,g_recompiled_insn_count;
extern uint32_t g_miss_count_any;
extern uint32_t m68k_interp_hot_count(uint32_t);
uint64_t prof_diag_bytes,prof_diag_writes,prof_diag_opens,prof_diag_hash_bytes;
static uint64_t diag_base[4];
static const uint32_t pcs[NPC]={0x1050c,0x10510,0x1051e,0x10522,0x807e,0x8080,0x3b98,0x3b9e,0x3ba4,0x3ba8,0x8254,0x825e,0x18abc,0x18a22,0x1888a,0x1887a,0x1886a,0x1087a};
typedef struct { uint32_t frame,turbo,miss,hot[NPC]; uint64_t total,interp,recomp; uint64_t diag[4]; double us,phase[8],marks[8]; } Row;
static Row *rows,cur;
static uint64_t count,dropped,abandoned,start,last,freq,marks[8],base[3];
static uint32_t hot[NPC],miss;
static int active;
static double us(uint64_t ticks){return (double)ticks*1e6/(double)freq;}
static int cmp(const void*a,const void*b){double x=*(const double*)a,y=*(const double*)b;return(x>y)-(x<y);}
static int desc(const void*a,const void*b){const Row*x=a,*y=b;return(x->us<y->us)-(x->us>y->us);}
static void finish(void){
 if(!rows)return;
 const char *dir=getenv("SOR_PROFILE_DIR"); if(!dir||!*dir)dir=".";
 char path[4096]; snprintf(path,sizeof path,"%s/frames.csv",dir); FILE*f=fopen(path,"w");
 if(!f){perror("profile frames");return;}
 fprintf(f,"frame,turbo,total_us,simulation_us,upload_draw_us,pacing_us,present_us,rewind_us,work_us,total_insns,interpreted,recompiled,dispatch_misses,diag_write_bytes,diag_write_calls,diag_open_calls,diag_hash_bytes");
 for(int i=0;i<8;i++)fprintf(f,",phase%d_us",i);
 for(int i=0;i<NPC;i++)fprintf(f,",pc_%06X",pcs[i]); fputc('\n',f);
 double *times=malloc(count*sizeof(double));double sum=0;unsigned slow=0;
 for(uint64_t j=0;j<count;j++){Row*r=&rows[j];double wait=r->marks[4]-r->marks[3],present=r->marks[5]-r->marks[4];
 fprintf(f,"%u,%u,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%llu,%llu,%llu,%u",r->frame,r->turbo,r->us,r->marks[1]-r->marks[0],r->marks[3]-r->marks[2],wait,present,r->marks[7]-r->marks[6],r->us-wait-present,(unsigned long long)r->total,(unsigned long long)r->interp,(unsigned long long)r->recomp,r->miss);
 for(int i=0;i<4;i++)fprintf(f,",%llu",(unsigned long long)r->diag[i]);
 for(int i=0;i<8;i++)fprintf(f,",%.3f",r->phase[i]);for(int i=0;i<NPC;i++)fprintf(f,",%u",r->hot[i]);fputc('\n',f);
 if(times)times[j]=r->us;sum+=r->us;if(r->us>25000)slow++;
 }fclose(f);
 snprintf(path,sizeof path,"%s/summary.txt",dir);f=fopen(path,"w");if(!f)return;
 fprintf(f,"retained=%llu dropped=%llu abandoned_iterations=%llu threshold_us=25000 slow_frames=%u\n",(unsigned long long)count,(unsigned long long)dropped,(unsigned long long)abandoned,slow);
 if(count&&times){qsort(times,count,sizeof(double),cmp);fprintf(f,"mean_us=%.3f median_us=%.3f p95_us=%.3f p99_us=%.3f max_us=%.3f\n",sum/count,times[(count-1)/2],times[(count-1)*95/100],times[(count-1)*99/100],times[count-1]);}
 fprintf(f,"Top 64 retained frames by total duration (us); work excludes pacing and present. Full phases and watched-PC deltas: frames.csv\n");
 qsort(rows,count,sizeof(Row),desc);for(uint64_t i=0;i<count&&i<64;i++){Row*r=&rows[i];fprintf(f,"FRAME %u %.3fms sim=%.3fms pacing=%.3fms present=%.3fms interpreted=%llu recompiled=%llu misses=%u\n",r->frame,r->us/1000,(r->marks[1]-r->marks[0])/1000,(r->marks[4]-r->marks[3])/1000,(r->marks[5]-r->marks[4])/1000,(unsigned long long)r->interp,(unsigned long long)r->recomp,r->miss);}
 fclose(f);free(times);free(rows);rows=NULL;fprintf(stderr,"[profile] %s/summary.txt (%llu frames)\n",dir,(unsigned long long)count);
}
void prof_init(void){freq=SDL_GetPerformanceFrequency();rows=calloc(CAP,sizeof(Row));if(rows)atexit(finish);else fprintf(stderr,"[profile] allocation failed; disabled\n");}
void prof_begin(uint32_t frame,int turbo){if(!rows)return;if(active)abandoned++;memset(&cur,0,sizeof cur);memset(marks,0,sizeof marks);cur.frame=frame;cur.turbo=turbo;base[0]=g_native_insn_count;base[1]=g_interpreted_insn_count;base[2]=g_recompiled_insn_count;miss=g_miss_count_any;for(int i=0;i<NPC;i++)hot[i]=m68k_interp_hot_count(pcs[i]);diag_base[0]=prof_diag_bytes;diag_base[1]=prof_diag_writes;diag_base[2]=prof_diag_opens;diag_base[3]=prof_diag_hash_bytes;start=last=SDL_GetPerformanceCounter();active=1;}
void prof_phase(unsigned n){if(!active||n>=8)return;uint64_t now=SDL_GetPerformanceCounter();cur.phase[n]+=us(now-last);last=now;}
void prof_mark(unsigned n){if(active&&n<8)marks[n]=SDL_GetPerformanceCounter();}
void prof_end(void){if(!active)return;cur.diag[0]=prof_diag_bytes-diag_base[0];cur.diag[1]=prof_diag_writes-diag_base[1];cur.diag[2]=prof_diag_opens-diag_base[2];cur.diag[3]=prof_diag_hash_bytes-diag_base[3];cur.us=us(SDL_GetPerformanceCounter()-start);cur.total=g_native_insn_count-base[0];cur.interp=g_interpreted_insn_count-base[1];cur.recomp=g_recompiled_insn_count-base[2];cur.miss=g_miss_count_any-miss;for(int i=0;i<NPC;i++)cur.hot[i]=m68k_interp_hot_count(pcs[i])-hot[i];for(int i=0;i<8;i++)cur.marks[i]=marks[i]?us(marks[i]-start):0;if(count<CAP)rows[count++]=cur;else dropped++;active=0;}
