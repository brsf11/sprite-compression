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

typedef struct
{
      unsigned Header;
      unsigned char* source;
      unsigned numSource;
      unsigned char* source_4b;
      LZSeq* lzseq;
      unsigned lzbSeqLen;
      unsigned char* codeLen;
      unsigned char* litTree;
      unsigned char* distTree;
      unsigned char* litCode;
      unsigned char* distCode;
      unsigned seqLen;
      unsigned* seq;
      unsigned char* uniTree;
      unsigned char* uniCode;
      unsigned char* uniSq;
      unsigned numSq;
      unsigned char* CCL;
      unsigned char* sqCode;
      unsigned* sqHist;
      unsigned sqCodeLen;
      unsigned char* Bitstream;
}LZB;

static const unsigned char numLit  = 29;
static const unsigned char numDist = 16;

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

void           Byte2Fb(unsigned char* source,unsigned char* dest,unsigned numSource);
uint32_t       matchSeq(unsigned char* windowSP,unsigned char* seqSP,unsigned char* sourceSP,unsigned char* enSP);
int            lzbCompress(unsigned char* source,unsigned numSource,LZSeq* lzseq);
void           genLzbHufTree(LZSeq* lzseq,unsigned seqLen,unsigned char* litTree,unsigned char* distTree,unsigned char* codeLen);
void           huffTree2Code(unsigned char* tree,unsigned numTree,unsigned numMax,unsigned char* code);
unsigned       lzb2Seq(LZSeq* lzseq,unsigned numSeq,unsigned* seq);
unsigned char* combCode(unsigned char* code1,unsigned numCode1,unsigned char* code2,unsigned numCode2,unsigned char* uniCode);
unsigned       tree2Sq(unsigned char* tree,unsigned numTree,unsigned char* sq);
int            LZBPrepare(LZB* lzb);
unsigned       assembBits(unsigned char* Bitstream,unsigned* bias,unsigned char code,unsigned char codeWidth);
void           reverseBytes(unsigned char* Bitstream,unsigned numByte);
int            genBitstream(LZB* lzb);
#endif