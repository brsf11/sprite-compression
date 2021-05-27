#ifndef _LZB_H_
#define _LZB_H_
#include<stdint.h>

typedef struct
{
    unsigned dist;
    unsigned len;
    char ch;
}LZSeq;


void Byte2Fb(const char* source,char* dest,unsigned numSource);
uint32_t matchSeq(const char* windowSP,const char* seqSP,const char* sourceSP,const char* enSP);
int lzbCompress(const char* source,unsigned numSource,LZSeq* lzseq);
#endif