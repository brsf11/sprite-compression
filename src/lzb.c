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
    while((sp >= sourceSP)&&(seqSP-sp <= 256))
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
        sp-=2;
    }
    unsigned char temp = 0;
    unsigned char i;
    for(i=0;i<windowSP-sp;i+=2)
    {
        if(len[i]>len[temp])
        {
            temp = i;
        }
    }
    len[temp] -= len[temp]%2;
    return (((uint32_t)temp+2)<<16) + len[temp];
}

int lzbCompress(const char* source,unsigned numSource,LZSeq* lzseq)
{
    char* ComPointer = source+2;
    unsigned SeqPointer = 0;
    lzseq[SeqPointer].dist = 0;
    lzseq[SeqPointer].len  = 0;
    lzseq[SeqPointer].ch   = *source;
    lzseq[SeqPointer+1].dist = 0;
    lzseq[SeqPointer+1].len  = 0;
    lzseq[SeqPointer+1].ch   = *(source+1);

    while(ComPointer-source<numSource)
    {
        uint32_t matchRes = matchSeq(ComPointer-2,ComPointer,source,source+numSource-1);
        unsigned len  = matchRes & 0x001f;
        unsigned dist = matchRes >> 16;
        SeqPointer++;
        if(len>=6)
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

void genLzbHufTree(LZSeq* lzseq,unsigned seqLen,unsigned char* litTree,unsigned char* distTree)
{
    unsigned lit_hist[29],dist_hist[17],lit_hist_temp[28],dist_hist_temp[16];
    unsigned char litTree_temp[28],distTree_temp[16];
    unsigned litTree_idx[28],distTree_idx[16],numCodeLit,numCodeDist;

    int i,j;

    for(i=0;i<29;i++)
    {
        lit_hist[i] = 0;
    }
    for(i=0;i<17;i++)
    {
        dist_hist[i] = 0;
    }
    for(i=0;i<28;i++)
    {
        litTree[i] = 0;
    }
    for(i=0;i<16;i++)
    {
        distTree[i] = 0;
    }
    numCodeDist = 0;
    numCodeLit  = 0;

    for(i=0;i<16;i++)
    {
        int num = 0;
        for(j=0;j<seqLen;j++)
        {
            if(lzseq[j].dist>=lzbDistOffTab[i]) num++;
        }
        dist_hist[i] = num;
    }
    for(i=0;i<16;i++)
    {
        dist_hist[i] -=dist_hist[i+1];
        if(dist_hist[i]!=0)
        {
            dist_hist_temp[numCodeDist] = dist_hist[i];
            distTree_idx[numCodeDist]  = i;
            numCodeDist++;
        }
    }

    for(i=0;i<seqLen;i++)
    {
        if(lzseq[i].dist == 0)
        {
            lit_hist[lzseq[i].ch]++;
        }
    }

    for(i=0;i<16;i++)
    {
        if(lit_hist[i]!=0)
        {
            lit_hist_temp[numCodeLit] = lit_hist[i];
            litTree_idx[numCodeLit]  = i;
            numCodeLit++;
        }
    }

    for(i=0;i<12;i++)
    {
        int num = 0;
        for(j=0;j<seqLen;j++)
        {
            if(lzseq[j].len>=lzbLenOffTab[i]) num++;
        }
        lit_hist[i+16] = num;
    }
    for(i=0;i<12;i++)
    {
        lit_hist[i+16] -=lit_hist[i+16+1];
        if(lit_hist[i+16]!=0)
        {
            lit_hist_temp[numCodeLit] = lit_hist[i+16];
            litTree_idx[numCodeLit]  = i+16;
            numCodeLit++;
        }
    }

    int DistLen = packageMerge(8,numCodeDist,dist_hist_temp,distTree_temp);
    int LitLen  = packageMerge(8,numCodeLit,lit_hist_temp,litTree_temp);

    if((DistLen!=0)&&(LitLen!=0))
    {
        for(i=0;i<numCodeDist;i++)
        {
            distTree[distTree_idx[i]] = distTree_temp[i];
        }
        for(i=0;i<numCodeLit;i++)
        {
            litTree[litTree_idx[i]] = litTree_temp[i];
        }
    }
}
