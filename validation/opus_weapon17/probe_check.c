/* Probe a single record reader on a zeroed record. Prints consumption and record bytes. */
#define NATIVE_INVENTORY_NO_MAIN
#include "native_inventory_check.c"
int main(int argc,char **argv){
 if(argc!=5)return 2;
 unsigned fn=strtoul(argv[2],0,16),rsize=strtoul(argv[3],0,16);
 struct sigaction sa={.sa_sigaction=inventory_crash,.sa_flags=SA_SIGINFO};sigemptyset(&sa.sa_mask);
 sigaction(SIGSEGV,&sa,NULL);sigaction(SIGILL,&sa,NULL);
 image=mmap((void*)BASE,0x6000000,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);
 if(image==MAP_FAILED)return 2;
 section(argv[1],"section-00-text.bin",0x1000);section(argv[1],"section-01-rdata.bin",0x32b8000);section(argv[1],"section-09-_RDATA.bin",0x48d0000);
 hook(0x8d4110,(void*)allocate,0);hook(0x8d34a0,(void*)release,1);hook(0x8d23e0,(void*)release,2);
 hook(0x19d7600,(void*)copy,3);hook(0x19d7b60,(void*)fill,4);hook(0x89ddb0,(void*)log_ignored,5);image[0xc0d3d]=0xc3;
 FILE *f=fopen(argv[4],"rb");if(!f)return 2;fseek(f,0,SEEK_END);long size=ftell(f);rewind(f);
 if(size<0||size>60000)return 2;
 size_t pg=((size+4095)/4096+1)*4096;
 unsigned char *pages=mmap(NULL,pg+4096,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
 if(pages==MAP_FAILED||mprotect(pages+pg,4096,PROT_NONE))return 2;
 unsigned char *packet=pages+pg-size;input_end=pages+pg;
 if(fread(packet,1,size,f)!=(size_t)size)return 2;fclose(f);
 unsigned char reader[0x30];reader_init(reader,packet,size);
 snprintf(step,sizeof(step),"probe %x",fn);
 unsigned char *record=calloc(1,rsize+64);
 if(getenv("VT")){unsigned vo=strtoul(getenv("VTOFF"),0,16);void *vt=image+strtoul(getenv("VT"),0,16);memcpy(record+vo,&vt,8);}
 if(strchr(argv[2],'r'))((void(MS*)(void*,void*))(image+fn))(reader,record);
 else ((void(MS*)(void*,void*))(image+fn))(record,reader);
 printf("{\"parse_error\":%u,\"consumed\":%llu,\"record\":\"",reader[0x20],(unsigned long long)(u64(reader+0x10)-(uintptr_t)packet));
 for(unsigned i=0;i<rsize;i++)printf("%02x",record[i]);
 puts("\"}");return 0;
}
