/* Construct ContinentalReplicationData (133F290) and run its payload reader (vtable+0x28). */
#define NATIVE_INVENTORY_NO_MAIN
#include "native_inventory_check.c"
static double MS fx_atof(const char *p){return atof(p);}
int main(int argc,char **argv){
 if(argc!=5)return 2; unsigned kind=strtoul(argv[3],0,0); unsigned dump=strtoul(argv[4],0,16);
 struct sigaction sa={.sa_sigaction=inventory_crash,.sa_flags=SA_SIGINFO};sigemptyset(&sa.sa_mask);
 sigaction(SIGSEGV,&sa,NULL);sigaction(SIGILL,&sa,NULL);
 image=mmap((void*)BASE,0x6000000,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);
 if(image==MAP_FAILED)return 2;
 section(argv[1],"section-00-text.bin",0x1000);section(argv[1],"section-01-rdata.bin",0x32b8000);section(argv[1],"section-09-_RDATA.bin",0x48d0000);
 hook(0x8d4110,(void*)allocate,0);hook(0x8d34a0,(void*)release,1);hook(0x8d23e0,(void*)release,2);
 hook(0x19d7600,(void*)copy,3);hook(0x19d7b60,(void*)fill,4);hook(0x89ddb0,(void*)log_ignored,5);image[0xc0d3d]=0xc3;
 hook(0x1a2ab14,(void*)fx_atof,6);hook(0x8d8d30,(void*)log_ignored,7);hook(0x8d8ce0,(void*)log_ignored,8);
 FILE *f=fopen(argv[2],"rb");if(!f)return 2;fseek(f,0,SEEK_END);long size=ftell(f);rewind(f);
 size_t pg=((size+4095)/4096+1)*4096;unsigned char *pages=mmap(NULL,pg+4096,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
 mprotect(pages+pg,4096,PROT_NONE);unsigned char *pkt=pages+pg-size;input_end=pages+pg;
 if(fread(pkt,1,size,f)!=(size_t)size)return 2;fclose(f);
 unsigned char *obj=calloc(1,0x1000);
 snprintf(step,sizeof(step),"ctor 133F290");
 ((void*(MS*)(void*,uint32_t,uint64_t,uint64_t))(image+strtoul(getenv("CTOR"),0,16)))(obj,1,0,0);
 unsigned char *before=malloc(0x6f8);memcpy(before,obj,0x6f8);
 unsigned char reader[0x30];reader_init(reader,pkt,size);
 unsigned char *iface=obj+0x38; void **vt=(void**)(uintptr_t)u64(iface);
 snprintf(step,sizeof(step),"payload reader vt+0x28 (%llx)",(unsigned long long)((uintptr_t)vt[5]-BASE));
 ((void(MS*)(void*,void*,uint8_t))vt[5])(iface,reader,(uint8_t)kind);
 printf("{\"vtable\":\"%llx\",\"reader\":\"%llx\",\"parse_error\":%u,\"consumed\":%llu,\"record\":\"",(unsigned long long)((uintptr_t)vt-BASE),(unsigned long long)((uintptr_t)vt[5]-BASE),reader[0x20],(unsigned long long)(u64(reader+0x10)-(uintptr_t)pkt));
 for(unsigned i=0;i<dump;i++)printf("%02x",obj[i]);
 puts("\"}");return 0;
}
