#include<stdio.h>
#include<stdbool.h>
#include"packagemerge.h"
#include"lzb.h"

int main()
{
    char source[]={'a','b','c','d','b','c','d','c','b','c','d','c','b','c','a','d'};
    char source_4b[32];
    LZSeq lzseq[32];

    Byte2Fb(source,source_4b,16);
    int seqLen = lzbCompress(source_4b,32,lzseq);

    int i;
    for(i=0;i<32;i++)
    {
        printf("%X ",source_4b[i]);
    }
    printf("\n");
    for(i=0;i<seqLen;i++)
    {
        printf("%2d %2d %4X\n",lzseq[i].dist,lzseq[i].len,lzseq[i].ch);
    }

    return 0;
}