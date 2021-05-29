#include<stdio.h>
#include<stdbool.h>
#include"packagemerge.h"
#include"lzb.h"

int main()
{
    unsigned char source[]={0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,
                            0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
                            0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff};
    unsigned char source_4b[76];
    
    unsigned char codeLen[2];
    unsigned char litTree[28],distTree[16],uniTree[44];
    unsigned char uniSq[44];
    unsigned char litCode[28],distCode[16],uniCode[44],CCL[10];
    unsigned char textBitstream[32];
    unsigned seq[76],sqHist[10];
    LZSeq lzseq[76];

    Byte2Fb(source,source_4b,38);
    unsigned lzbSeqLen = lzbCompress(source_4b,76,lzseq);

    genLzbHufTree(lzseq,lzbSeqLen,litTree,distTree,codeLen);
    
    huffTree2Code(litTree,28,codeLen[0],litCode);
    huffTree2Code(distTree,16,codeLen[1],distCode);

    unsigned seqLen = lzb2Seq(lzseq,lzbSeqLen,seq);

    combCode(litTree,28,distTree,16,uniTree);
    combCode(litCode,28,distCode,16,uniCode);

    unsigned numSq = tree2Sq(uniTree,44,uniSq);

    int i;

    for(i=0;i<10;i++)
    {
        sqHist[i] = 0;
    }

    for(i=0;i<numSq;i++)
    {
        sqHist[uniSq[i]]++;
    }
    packageMerge(7,10,sqHist,CCL);


    

    for(i=0;i<lzbSeqLen;i++)
    {
        if(lzseq[i].len != 0)
        {
            printf("%2d %2d|",lzseq[i].len,lzseq[i].dist);
        }
        else
        {
            printf("%2d|",lzseq[i].ch);
        }
    }
    printf("\n");

    for(i=0;i<44;i++)
    {
        printf("%d",uniTree[i]);
    }
    printf("\n");

    for(i=0;i<numSq;i++)
    {
        printf("%d",uniSq[i]);
    }
    printf("\n");

    for(i=0;i<10;i++)
    {
        printf("%d",sqHist[i]);
    }
    printf("\n");

    for(i=0;i<10;i++)
    {
        printf("%d",CCL[i]);
    }
    printf("\n");

    return 0;
}