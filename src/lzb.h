#ifndef _LZB_H_
#define _LZB_H_
#include<stdint.h>
#include"packagemerge.h"

typedef struct
{
    unsigned dist;
    unsigned len;
    unsigned char ch;
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

void Byte2Fb(const unsigned char* source,unsigned char* dest,unsigned numSource);
uint32_t matchSeq(const unsigned char* windowSP,const unsigned char* seqSP,const unsigned char* sourceSP,const unsigned char* enSP);
int lzbCompress(const unsigned char* source,unsigned numSource,LZSeq* lzseq);
void genLzbHufTree(LZSeq* lzseq,unsigned seqLen,unsigned char* litTree,unsigned char* distTree,unsigned char* codeLen);
void huffTree2Code(unsigned char* tree,unsigned numTree,unsigned numMax,unsigned char* code);
unsigned lzb2Seq(LZSeq* lzseq,unsigned numSeq,unsigned* seq);
unsigned char* combCode(unsigned char* code1,unsigned numCode1,unsigned char* code2,unsigned numCode2,unsigned char* uniCode);
unsigned tree2Sq(unsigned char* tree,unsigned numTree,unsigned char* sq);
int genBitstream(unsigned char* code,unsigned char* tree,unsigned char numCode,unsigned* seq,unsigned numSeq,unsigned char* bitstream,char bitSp);
#endif