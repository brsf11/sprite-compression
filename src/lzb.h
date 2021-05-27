#ifndef _LZB_H_
#define _LZB_H_
#include<stdint.h>
#include"packagemerge.h"

typedef struct
{
    unsigned dist;
    unsigned len;
    char ch;
}LZSeq;

static const unsigned lzbLenOffTab[] = {
      6,   8,  10,  12,
     14,  18,  22,  26,
     34,  50,  64, 130 
};
static const unsigned lzbLenExBitTab[] = {
      0,   0,   0,   0,
      1,   1,   1,   2,
      3,   3,   5,   6
};
static const unsigned lzbDistOffTab[] = {
      2,   4,   6,   8,
     10,  14,  18,  26,
     34,  66, 130, 194,
    258, 322, 386, 450
};
static const unsigned lzbDistExBitTab[] = {
      0,   0,   0,   0,
      1,   1,   2,   2,
      4,   5,   5,   5,
      5,   5,   5,   5
};

void Byte2Fb(const char* source,char* dest,unsigned numSource);
uint32_t matchSeq(const char* windowSP,const char* seqSP,const char* sourceSP,const char* enSP);
int lzbCompress(const char* source,unsigned numSource,LZSeq* lzseq);
void genLzbHufTree(LZSeq* lzseq,unsigned seqLen,unsigned char* litTree,unsigned char* distTree);
#endif