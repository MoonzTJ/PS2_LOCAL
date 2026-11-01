/* Run the real 0xD1 name/value reader (9BB1C0 -> 97B9D0) into a zeroed table,
 * then the real lookup 96A520 for each requested name hash. */
#define NATIVE_INVENTORY_NO_MAIN
#include "native_inventory_check.c"
static uint32_t oaat_signed(const char *s){int32_t h=0;for(;*s;s++){h+=(signed char)*s;h=h*0x401;h^=h>>6;}h=h*9;h^=h>>11;return (uint32_t)(h*0x8001);}
static unsigned atof_calls;
static double MS fx_atof(const char *p){atof_calls++;return atof(p);}
static const char *MS fx_strchr(const char *p,int c){return strchr(p,c);}
int main(int argc,char **argv){
 if(argc<3)return 2;
 struct sigaction sa={.sa_sigaction=inventory_crash,.sa_flags=SA_SIGINFO};sigemptyset(&sa.sa_mask);
 sigaction(SIGSEGV,&sa,NULL);sigaction(SIGILL,&sa,NULL);
 image=mmap((void*)BASE,0x6000000,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);
 if(image==MAP_FAILED)return 2;
 section(argv[1],"section-00-text.bin",0x1000);section(argv[1],"section-01-rdata.bin",0x32b8000);section(argv[1],"section-09-_RDATA.bin",0x48d0000);
 hook(0x8d4110,(void*)allocate,0);hook(0x8d34a0,(void*)release,1);hook(0x8d23e0,(void*)release,2);
 hook(0x19d7600,(void*)copy,3);hook(0x19d7b60,(void*)fill,4);hook(0x89ddb0,(void*)log_ignored,5);image[0xc0d3d]=0xc3;hook(0x1a2ab14,(void*)fx_atof,6);hook(0x19da834,(void*)fx_strchr,7);
 FILE *f=fopen(argv[2],"rb");if(!f)return 2;fseek(f,0,SEEK_END);long size=ftell(f);rewind(f);
 unsigned char *pkt=malloc(size);if(fread(pkt,1,size,f)!=(size_t)size)return 2;fclose(f);
 unsigned char *table=calloc(1,0x400),handler[0x40]={0};void *vt=image+0x33c60c0;memcpy(handler,&vt,8);memcpy(handler+0x10,&table,8);
 snprintf(step,sizeof(step),"0xD1 reader");
 uint8_t ok=((uint8_t(MS*)(void*,void*,int32_t,uint8_t))(image+0x9bb1c0))(handler,pkt,(int32_t)size,1);
 printf("{\"reader_ok\":%u,\"opcode_byte\":%u,\"count\":%llu,\"lookups\":{",ok,handler[8],(unsigned long long)u64(table+0x70));
 snprintf(step,sizeof(step),"lookup");
 for(int i=3;i<argc;i++){uint32_t h=oaat_signed(argv[i]);
  int32_t v=((int32_t(MS*)(void*,uint32_t*,int32_t))(image+0x96a520))(table,&h,-999);
  printf("%s\"%s\":{\"hash\":\"%08x\",\"value\":%d}",i>3?",":"",argv[i],h,v);}
 printf("},\"crt_atof_calls\":%u}\n",atof_calls);return 0;
}
