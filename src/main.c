#include<stdio.h>
#include"packagemerge.h"
#include"lzb.h"

int main()
{
    LZB lzb;
    unsigned char source[]={0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,
                            0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
                            0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff};
    unsigned char source_4b[76];
    
    unsigned char codeLen[2];
    unsigned char litTree[28],distTree[16],uniTree[44];
    unsigned char uniSq[44];
    unsigned char litCode[28],distCode[16],uniCode[44],CCL[10],sqCode[10];
    unsigned char Bitstream[38];
    unsigned seq[76],sqHist[10];
    LZSeq lzseq[76];

    lzb.source    = source;
    lzb.numSource = 38;
    
    lzb.source_4b = source_4b;
    lzb.lzseq     = lzseq;
    lzb.codeLen   = codeLen;
    lzb.litTree   = litTree;
    lzb.distTree  = distTree;
    lzb.litCode   = litCode;
    lzb.distCode  = distCode;
    lzb.seq       = seq;
    lzb.uniTree   = uniTree;
    lzb.uniCode   = uniCode;
    lzb.uniSq     = uniSq;
    lzb.CCL       = CCL;
    lzb.sqHist    = sqHist;
    lzb.sqCode    = sqCode;
    lzb.Bitstream = Bitstream;

    LZBPrepare(&lzb);
    
    int i;

    for(i=0;i<lzb.lzbSeqLen;i++)
    {
        if(lzb.lzseq[i].len != 0)
        {
            printf("%2d %2d|",lzb.lzseq[i].len,lzb.lzseq[i].dist);
        }
        else
        {
            printf("%2d|",lzb.lzseq[i].ch);
        }
    }
    printf("\n");

    for(i=0;i<44;i++)
    {
        printf("%d",lzb.uniTree[i]);
    }
    printf("\n");

    for(i=0;i<lzb.numSq;i++)
    {
        printf("%d",lzb.uniSq[i]);
    }
    printf("\n");

    for(i=0;i<10;i++)
    {
        printf("%d",lzb.sqHist[i]);
    }
    printf("\n");

    for(i=0;i<10;i++)
    {
        printf("%d",lzb.CCL[i]);
    }
    printf("\n");

    for(i=0;i<10;i++)
    {
        printf("%2d",lzb.sqCode[i]);
    }
    printf("\n");


    return 0;
}