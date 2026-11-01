/* Probe a single record reader on a zeroed record. Prints consumption and record bytes. */
#define NATIVE_INVENTORY_NO_MAIN
#include "native_inventory_check.c"
static uint64_t MS fx_zero(void){return 0;}
static double MS fx_atof(const char *p){return atof(p);}
int main(int argc,char **argv){
 if(argc!=5)return 2;
 unsigned fn=strtoul(argv[2],0,16),rsize=strtoul(argv[3],0,16);
 struct sigaction sa={.sa_sigaction=inventory_crash,.sa_flags=SA_SIGINFO};sigemptyset(&sa.sa_mask);
 sigaction(SIGSEGV,&sa,NULL);sigaction(SIGILL,&sa,NULL);
 image=mmap((void*)BASE,0x6000000,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);
 if(image==MAP_FAILED)return 2;
 section(argv[1],"section-00-text.bin",0x1000);section(argv[1],"section-01-rdata.bin",0x32b8000);section(argv[1],"section-09-_RDATA.bin",0x48d0000);
 hook(0x8d4110,(void*)allocate,0);hook(0x8d34a0,(void*)release,1);hook(0x8d23e0,(void*)release,2);
 hook(0x19d7600,(void*)copy,3);hook(0x19d7b60,(void*)fill,4);hook(0x89ddb0,(void*)log_ignored,5);image[0xc0d3d]=0xc3;hook(0x1a2ab14,(void*)fx_atof,6);hook(0x8d8d30,(void*)fx_zero,7);hook(0x8d8ce0,(void*)fx_zero,8);{void *z=(void*)fx_zero;const char *e=getenv("IAT");unsigned slots[64];int n=0;if(e){char *p=(char*)e;while(*p&&n<64){slots[n++]=strtoul(p,&p,16);if(*p==',')p++;}}for(int i=0;i<n;i++)memcpy(image+slots[i],&z,8);}

 FILE *f=fopen(argv[4],"rb");if(!f)return 2;fseek(f,0,SEEK_END);long size=ftell(f);rewind(f);
 unsigned char *pkt=malloc(size+16);if(fread(pkt,1,size,f)!=(size_t)size)return 2;fclose(f);
 unsigned char *st=calloc(1,0x200),*rec=calloc(1,0x200);
 void *vt=image+0x33C78F8; memcpy(rec+8,&vt,8); float q=0.125f; memcpy(rec+0x48,&q,4);
 memcpy(st+0x28,&rec,8);
 snprintf(step,sizeof(step),"C52130");
 unsigned ok=((unsigned char(MS*)(void*,int,void*))(image+0xC52130))(pkt,(int)size,st);
 printf("{\"ok\":%u,\"hdr\":\"",ok); for(int i=0;i<0x30;i++)printf("%02x",st[i]);
 printf("\",\"rec\":\""); for(unsigned i=0;i<rsize;i++)printf("%02x",rec[i]); puts("\"}"); return 0;
}
