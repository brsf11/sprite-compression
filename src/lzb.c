#include"lzb.h"
#include<stdint.h>
#include<stdio.h>

void Byte2Fb(const unsigned char* source,unsigned char* dest,unsigned numSource)
{
    int i;
    for(i=0;i<numSource;i++)
    {
        dest[2*i]   = source[i] >> 4;
        dest[2*i+1] = source[i] & 0x0f; 
    }
}

uint32_t matchSeq(const unsigned char* windowSP,const unsigned char* seqSP,const unsigned char* sourceSP,const unsigned char* enSP)
{
    unsigned len[256];
    unsigned char* sp;
    sp = windowSP;
    while((sp >= sourceSP)&&(seqSP-sp <= 256))
    {
        unsigned char* tempWinSP = sp;
        unsigned char* tempSeqSP = seqSP;
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

int lzbCompress(const unsigned char* source,unsigned numSource,LZSeq* lzseq)
{
    unsigned char* ComPointer = source+2;
    unsigned SeqPointer = 0;
    lzseq[SeqPointer].dist = 0;
    lzseq[SeqPointer].len  = 0;
    lzseq[SeqPointer].ch   = *source;
    lzseq[SeqPointer+1].dist = 0;
    lzseq[SeqPointer+1].len  = 0;
    lzseq[SeqPointer+1].ch   = *(source+1);
    SeqPointer+=2;

    while(ComPointer-source<numSource)
    {
        uint32_t matchRes = matchSeq(ComPointer-2,ComPointer,source,source+numSource-1);
        unsigned len  = matchRes & 0x001f;
        unsigned dist = matchRes >> 16;
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
        SeqPointer++;
    }
    return SeqPointer;
}

void genLzbHufTree(LZSeq* lzseq,unsigned seqLen,unsigned char* litTree,unsigned char* distTree,unsigned char* codeLen)
{
    unsigned lit_hist[29],dist_hist[17];

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
    }

    for(i=0;i<seqLen;i++)
    {
        if(lzseq[i].dist == 0)
        {
            lit_hist[lzseq[i].ch]++;
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
    }

    codeLen[1] = packageMerge(8,numDist,dist_hist,distTree);
    codeLen[0] = packageMerge(8,numLit,lit_hist,litTree);

}

unsigned lzb2Seq(LZSeq* lzseq,unsigned numSeq,unsigned* seq)
{
    int i;
    unsigned sp = 0;
    for(i=0;i<numSeq;i++)
    {
        if(lzseq[i].len==0)
        {
            seq[sp] = lzseq[i].ch;
            sp++;
        }
        else
        {
            int j;
            for(j=0;j<12;j++)
            {
                if(lzseq[i].len >= lzbLenOffTab[j])
                {
                    seq[sp] = j + 16;
                }
                else
                {
                    break;
                }
            }
            sp++;
            for(j=0;j<12;j++)
            {
                if(lzseq[i].dist >= lzbDistOffTab[j])
                {
                    seq[sp] = j + 28;
                }
                else
                {
                    break;
                }
            }
            sp++;
        }
    }
    return sp;
}

unsigned char* combCode(unsigned char* code1,unsigned numCode1,unsigned char* code2,unsigned numCode2,unsigned char* uniCode)
{
    unsigned i;
    for(i=0;i<numCode1;i++)
    {
        uniCode[i] = code1[i];
    }
    for(i=0;i<numCode2;i++)
    {
        uniCode[i+numCode1] = code2[i];
    }
    return uniCode;
}

void huffTree2Code(unsigned char* tree,unsigned numTree,unsigned numMax,unsigned char* code)
{
    unsigned char len;
    int prefix = 0;
    int i;

    for(i=0;i<numTree;i++)
    {
        if(tree[i] == 0)
        {
            code[i] = 0;
        }
    }

    for(len=1;len<=numMax;len++)
    {
        for(i=0;i<numTree;i++)
        {
            if(tree[i] == len)
            {
                code[i] = prefix;
                prefix++;
            }
        }
        prefix <<= 1;
    }
}

unsigned tree2Sq(unsigned char* tree,unsigned numTree,unsigned char* sq)
{
    int i;
    unsigned numZero,flagZero;
    unsigned sqSP = 0;
    numZero  = 0;
    flagZero = 0;
    for(i=0;i<numTree;i++)
    {
        if(tree[i] == 0)
        {
            if(flagZero)
            {
                numZero++;
            }
            else
            {
                flagZero = 1;
                numZero++;
            }
            if(numZero == 10)
            {
                sq[sqSP]   = 9;
                sq[sqSP+1] = 7;
                flagZero   = 0;
                numZero    = 0;
                sqSP      += 2;
            }
        }
        else
        {
            if(flagZero)
            {
                if(numZero < 3)
                {
                    int j;
                    for(j=0;j<numZero;j++)
                    {
                        sq[sqSP] = 0;
                        sqSP++;
                    }
                    flagZero = 0;
                    numZero  = 0;
                }
                else
                {
                    sq[sqSP]   = 9;
                    sq[sqSP+1] = numZero-3;
                    flagZero   = 0;
                    numZero    = 0;
                    sqSP      += 2;
                }
            }
            sq[sqSP] = tree[i];
            sqSP++;
        }
    }
    if(flagZero)
    {
        if(numZero < 3)
        {
            int j;
            for(j=0;j<numZero;j++)
            {
                sq[sqSP] = 0;
                sqSP++;
            }
            flagZero = 0;
            numZero  = 0;
        }
        else
        {
            sq[sqSP]   = 9;
            sq[sqSP+1] = numZero-3;
            flagZero   = 0;
            numZero    = 0;
            sqSP      += 2;
        }
    }
    return sqSP;
}

int LZBPrepare(LZB* lzb)
{
    lzb->Header = 5;

    Byte2Fb(lzb->source,lzb->source_4b,lzb->numSource);
    lzb->lzbSeqLen = lzbCompress(lzb->source_4b,lzb->numSource*2,lzb->lzseq);

    genLzbHufTree(lzb->lzseq,lzb->lzbSeqLen,lzb->litTree,lzb->distTree,lzb->codeLen);
    
    huffTree2Code(lzb->litTree,numLit,lzb->codeLen[0],lzb->litCode);
    huffTree2Code(lzb->distTree,numDist,lzb->codeLen[1],lzb->distCode);

    lzb->seqLen = lzb2Seq(lzb->lzseq,lzb->lzbSeqLen,lzb->seq);

    combCode(lzb->litTree,numLit,lzb->distTree,numDist,lzb->uniTree);
    combCode(lzb->litCode,numLit,lzb->distCode,numDist,lzb->uniCode);

    lzb->numSq = tree2Sq(lzb->uniTree,numLit+numDist,lzb->uniSq);

    int i;

    for(i=0;i<10;i++)
    {
        lzb->sqHist[i] = 0;
    }

    for(i=0;i<lzb->numSq;i++)
    {
        lzb->sqHist[lzb->uniSq[i]]++;
    }
    lzb->sqCodeLen = packageMerge(7,10,lzb->sqHist,lzb->CCL);

    huffTree2Code(lzb->CCL,10,lzb->sqCodeLen,lzb->sqCode);
    return 1;
}

unsigned assembBits(unsigned char* Bitstream)
{

}

int genBitstream(LZB* lzb)
{

}
