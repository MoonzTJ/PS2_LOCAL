/* Real WeaponDefinitions readers -> manager installed as the weapon catalog ->
 * real factory initialization of item 44 and its SendSelf weapon detail. */
#define NATIVE_WEAPON_INIT_NO_MAIN
#include "native_weapon_init_check.c"
static void *MS fx_node(void *self,size_t a,size_t b){(void)self;(void)a;(void)b;return calloc(1,0x2000);}
static void MS fx_noop(void *self,void *p){(void)self;(void)p;}
static void *fx_vt[16];
int main(int argc,char **argv){
 if(argc!=4)return 2;int with_detail=!strcmp(argv[2],"--detail");
 struct sigaction sa={.sa_sigaction=owned_crash,.sa_flags=SA_SIGINFO};sigemptyset(&sa.sa_mask);sigaction(SIGSEGV,&sa,NULL);sigaction(SIGILL,&sa,NULL);
 image=mmap((void*)BASE,0x6000000,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE,-1,0);if(image==MAP_FAILED)return 2;
 section(argv[1],"section-00-text.bin",0x1000);section(argv[1],"section-01-rdata.bin",0x32b8000);section(argv[1],"section-09-_RDATA.bin",0x48d0000);
 hook(0x8d4110,(void*)allocate,0);hook(0x8d34a0,(void*)release,1);hook(0x8d23e0,(void*)release,2);
 hook(0x19d7600,(void*)copy,3);hook(0x19d7b60,(void*)fill,4);hook(0x89ddb0,(void*)log_ignored,5);image[0xc0d3d]=0xc3;
 hook(0x41a74,(void*)missing_metadata_boundary,6);hook(0x9746a,(void*)missing_weapon_data_boundary,7);
 hook(0xe6c31,(void*)initialize_native,8);hook(0x69a7e,(void*)actor_setup_boundary,9);hook(0xcd85d,(void*)update_boundary,10);
 hook(0xcaa63,(void*)mode_update_boundary,12);hook(0x8dab70,(void*)fixture_pair_time,11);
 void *api_fn=(void*)fixture_frequency;memcpy(image+0x49b46f0,&api_fn,8);api_fn=(void*)fixture_counter;memcpy(image+0x49b46e8,&api_fn,8);
 FILE *f=fopen(argv[3],"rb");if(!f)return 2;fseek(f,0,SEEK_END);long size=ftell(f);rewind(f);if(size<8||size>60000)return 2;
 size_t pg=((size+4095)/4096+1)*4096;unsigned char *pages=mmap(NULL,pg+4096,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
 if(pages==MAP_FAILED||mprotect(pages+pg,4096,PROT_NONE))return 2;
 cursor=pages+pg-size;input_end=pages+pg;if(fread(cursor,1,size,f)!=(size_t)size)return 2;fclose(f);
 unsigned wire_len=init_word();unsigned char *wire=take_init(wire_len);
 unsigned char container[0x108]={0};owned_init(container,123);prepare_weapon_metadata(44,26,17,17);
 for(int i=0;i<16;i++)fx_vt[i]=(void*)fx_noop;fx_vt[2]=(void*)fx_node;
 unsigned char *manager=calloc(1,0x400);unsigned offsets[]={0x48,0x60,0x78,0x90,0xc8,0x100};
 unsigned functions[]={0x9b2e30,0x9b2880,0x9b2cf0,0x9aab20,0x9b2ba0,0x9aa5c0};
 unsigned char reader[0x30];reader_init(reader,wire,wire_len);
 for(unsigned i=0;i<6;i++){void *v=fx_vt;memcpy(manager+offsets[i],&v,8);}
 snprintf(step,sizeof(step),"weapon definitions wire");
 for(unsigned i=0;i<6;i++)((void(MS*)(void*,void*))(image+functions[i]))(reader,manager+offsets[i]);
 printf("{\"wire_error\":%u,\"wire_remaining\":%llu,",reader[0x20],(unsigned long long)(u64(reader+0x18)-u64(reader+0x10)));
 unsigned char *node=(void*)(uintptr_t)u64(manager+0x50);
 printf("\"weapon_node_key\":%u,",node?u32(node):0);
 definition=node?node+8:NULL;printf("\"weapon_record_key_18\":%u,",definition?u32(definition+0x18):0);
 catalog=manager;init_singleton(0x40402ec,0x417c810,catalog);
 void *player=calloc(1,0x90000);init_singleton(0x4040104,0x4169eb0,player);
 void *actor_context=calloc(1,0x2500);init_singleton(0x4040280,0x4178640,actor_context);
 snprintf(step,sizeof(step),"factory initialization");
 unsigned char r2[0x30]={0},base[0x48]={0};reader_init(r2,cursor,input_end-cursor);
 ((void(MS*)(void*,void*))(image+0x9928a0))(base,r2);
 void *item=((void*(MS*)(void*,void*))(image+0xb4de10))(factory_context,base);
 printf("\"initial\":");if(active_state)print_initialization();else printf("null");
 if(with_detail && active_state){snprintf(step,sizeof(step),"weapon detail");((void(MS*)(void*,void*))(image+0x144eca0))(item,r2);printf(",\"after_detail\":");print_initialization();}
 printf(",\"detail_error\":%u,\"detail_remaining\":%llu}\n",r2[0x20],(unsigned long long)(u64(r2+0x18)-u64(r2+0x10)));
 return 0;
}
