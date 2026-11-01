#define NATIVE_INVENTORY_NO_MAIN
#include "native_inventory_check.c"
static unsigned node_allocs;
static void *MS fixture_node(void *self,size_t a,size_t b){(void)self;(void)a;(void)b;node_allocs++;return calloc(1,0x2000);}
static void MS fixture_noop(void *self,void *p){(void)self;(void)p;}
static void *fake_vt[16];
int main(int argc,char **argv){
 if(argc!=3)return 2;
 struct sigaction sa={.sa_sigaction=inventory_crash,.sa_flags=SA_SIGINFO};sigemptyset(&sa.sa_mask);
 sigaction(SIGSEGV,&sa,NULL);sigaction(SIGILL,&sa,NULL);
 image=mmap((void*)BASE,0x6000000,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);
 if(image==MAP_FAILED)return 2;
 section(argv[1],"section-00-text.bin",0x1000);section(argv[1],"section-01-rdata.bin",0x32b8000);section(argv[1],"section-09-_RDATA.bin",0x48d0000);
 hook(0x8d4110,(void*)allocate,0);hook(0x8d34a0,(void*)release,1);hook(0x8d23e0,(void*)release,2);
 hook(0x19d7600,(void*)copy,3);hook(0x19d7b60,(void*)fill,4);hook(0x89ddb0,(void*)log_ignored,5);image[0xc0d3d]=0xc3;
 FILE *f=fopen(argv[2],"rb");if(!f)return 2;fseek(f,0,SEEK_END);long size=ftell(f);rewind(f);
 size_t pg=((size+4095)/4096+1)*4096;
 unsigned char *pages=mmap(NULL,pg+4096,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
 if(pages==MAP_FAILED||mprotect(pages+pg,4096,PROT_NONE))return 2;
 unsigned char *packet=pages+pg-size;input_end=pages+pg;
 if(fread(packet,1,size,f)!=(size_t)size)return 2;fclose(f);
 unsigned char reader[0x30];reader_init(reader,packet,size);
 static unsigned char manager[0x400];unsigned offsets[]={0x48,0x60,0x78,0x90,0xc8,0x100};
 unsigned functions[]={0x9b2e30,0x9b2880,0x9b2cf0,0x9aab20,0x9b2ba0,0x9aa5c0};
 for(int i=0;i<16;i++)fake_vt[i]=(void*)fixture_noop;fake_vt[2]=(void*)fixture_node;
 for(unsigned i=0;i<6;i++){void *v=fake_vt;memcpy(manager+offsets[i],&v,8);}
 printf("{\"steps\":[");
 for(unsigned i=0;i<6;i++){
  snprintf(step,sizeof(step),"collection %u",i);
  ((void(MS*)(void*,void*))(image+functions[i]))(reader,manager+offsets[i]);
  printf("%s{\"i\":%u,\"err\":%u,\"consumed\":%llu}",i?",":"",i,reader[0x20],(unsigned long long)(u64(reader+0x10)-(uintptr_t)packet));
 }
 printf("],\"node_allocs\":%u,\"remaining\":%llu}\n",node_allocs,(unsigned long long)(u64(reader+0x18)-u64(reader+0x10)));
 FILE *m=fopen("manager.bin","wb");fwrite(manager,1,sizeof manager,m);fclose(m);
 return 0;
}
