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
        while((*tempWinSP == *tempSeqSP)&&(tempSeqSP <= enSP))
        {
            tempWinSP++;
            tempSeqSP++;
            tempLen++;
        }
        len[windowSP-sp] = tempLen;
        printf("%c %c %d\n",*sp,*tempSeqSP,len[windowSP-sp]);
        sp--;
    }
    unsigned char temp = 0;
    unsigned char i;
    for(i=0;i<windowSP-sp-1;i++)
    {
        if(len[i]>len[temp])
        {
            temp = i;
        }
    }
    printf("%d %d\n",temp,len[temp]);
    return (((uint32_t)temp+1)<<16) + len[temp];
}
