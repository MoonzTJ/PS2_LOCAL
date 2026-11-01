#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
static uint32_t h(const char*s){int32_t x=0;for(;*s;s++){x+=(signed char)*s;x=x*0x401;x^=x>>6;}x=x*9;x^=x>>11;return (uint32_t)(x*0x8001);}
int main(int c,char**v){uint32_t T=strtoul(v[1],0,16);int lim2=atoi(v[2]),lim3=atoi(v[3]);
 static char W[8000][64];int n=0;FILE*f=fopen("words.txt","r");while(n<8000&&fgets(W[n],64,f)){W[n][strcspn(W[n],"\n")]=0;n++;}
 const char*sep[]={"",".","_"," ",":","::","/","-"};char b[256];
 const char*wrap[][2]={{"",""},{"<",">"},{"StatId.",""},{"worldvar ",""}};
 for(int i=0;i<n;i++)for(int w=0;w<4;w++){snprintf(b,256,"%s%s%s",wrap[w][0],W[i],wrap[w][1]);if(h(b)==T)printf("HIT %s\n",b);}
 for(int i=0;i<lim2&&i<n;i++)for(int j=0;j<lim2&&j<n;j++)for(int s=0;s<8;s++){snprintf(b,256,"%s%s%s",W[i],sep[s],W[j]);if(h(b)==T)printf("HIT %s\n",b);}
 for(int i=0;i<lim3&&i<n;i++)for(int j=0;j<lim3&&j<n;j++)for(int k=0;k<lim3&&k<n;k++)for(int s=0;s<3;s++){snprintf(b,256,"%s%s%s%s%s",W[i],sep[s],W[j],sep[s],W[k]);if(h(b)==T)printf("HIT %s\n",b);}
 return 0;}
