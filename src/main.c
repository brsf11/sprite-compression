#include<stdio.h>
#include<stdbool.h>
#include"packagemerge.h"
#include"lzb.h"

int main()
{
    char source[]={'a','b','c','d','b','c','d','c','b','c','d','c','b','c','a','d'};
    char source_4b[32];
    unsigned char codeLen[2];
    unsigned char litTree[28],distTree[16];
    unsigned char litCode[28],distCode[16];
    LZSeq lzseq[32];

    Byte2Fb(source,source_4b,16);
    unsigned seqLen = lzbCompress(source_4b,32,lzseq);

    genLzbHufTree(lzseq,seqLen,litTree,distTree,codeLen);
    
    huffTree2Code(litTree,28,codeLen[0],litCode);
    huffTree2Code(distTree,16,codeLen[1],distCode);

    int i;
    for(i=0;i<16;i++)
    {
        printf("%2d ",distTree[i]);
    }
    printf("\n");
    for(i=0;i<16;i++)
    {
        printf("%2d ",distCode[i]);
    }
    printf("\n");
    for(i=0;i<28;i++)
    {
        printf("%2d ",litTree[i]);
    }
    printf("\n");
    for(i=0;i<28;i++)
    {
        printf("%2d ",litCode[i]);
    }
    printf("\n");

    return 0;
}