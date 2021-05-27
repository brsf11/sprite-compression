#include<stdio.h>
#include<stdbool.h>
#include"packagemerge.h"
#include"lzb.h"

int main()
{
    char source[]={'a','b','c','d','b','c','d','c','b','c','d','c','b','c','a','d'};
    char source_4b[32];
    LZSeq lzseq[16];

    int seqLen = lzbCompress(source,16,lzseq);

    int i;
    for(i=0;i<seqLen;i++)
    {
        printf("%2d %2d %4X\n",lzseq[i].dist,lzseq[i].len,lzseq[i].ch);
    }

    return 0;
}