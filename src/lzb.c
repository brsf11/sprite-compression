#include"lzb.h"
#include<stdint.h>
#include<stdio.h>

void Byte2Fb(const char* source,char* dest,unsigned numSource)
{
    int i;
    for(i=0;i<numSource;i++)
    {
        dest[2*i]   = source[i] >> 4;
        dest[2*i+1] = source[i] & 0x0f; 
    }
}

uint32_t matchSeq(const char* windowSP,const char* seqSP,const char* sourceSP,const char* enSP)
{
    unsigned len[256];
    char* sp;
    sp = windowSP;
    while((sp >= sourceSP)&&(windowSP-sp <= 256 - 1))
    {
        char* tempWinSP = sp;
        char* tempSeqSP = seqSP;
        unsigned tempLen = 0;
        while((*tempWinSP == *tempSeqSP)&&(tempSeqSP <= enSP)&&(tempLen<=256))
        {
            tempWinSP++;
            tempSeqSP++;
            tempLen++;
        }
        len[windowSP-sp] = tempLen;
        sp--;
    }
    unsigned char temp = 0;
    unsigned char i;
    for(i=0;i<windowSP-sp;i++)
    {
        if(len[i]>len[temp])
        {
            temp = i;
        }
    }
    return (((uint32_t)temp+1)<<16) + len[temp];
}

int lzbCompress(const char* source,unsigned numSource,LZSeq* lzseq)
{
    char* ComPointer = source+1;
    unsigned SeqPointer = 0;
    lzseq[SeqPointer].dist = 0;
    lzseq[SeqPointer].len  = 0;
    lzseq[SeqPointer].ch   = *source;

    while(ComPointer-source<numSource)
    {
        uint32_t matchRes = matchSeq(ComPointer-1,ComPointer,source,source+numSource-1);
        unsigned len  = matchRes & 0x001f;
        unsigned dist = matchRes >> 16;
        SeqPointer++;
        if(len>=3)
        {
            lzseq[SeqPointer].dist = dist;
            lzseq[SeqPointer].len  = len;
            lzseq[SeqPointer].ch   = 0;
            ComPointer += len;
        }
        else
        {
            lzseq[SeqPointer].dist = 0;
            lzseq[SeqPointer].len  = 0;
            lzseq[SeqPointer].ch   = *ComPointer;
            ComPointer++;
        }
    }
    return SeqPointer+1;
}
