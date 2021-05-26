#include<stdio.h>
#include<stdbool.h>
#include"packagemerge.h"

int main()
{
    unsigned char maxLength;
    unsigned numCode;
    unsigned histogram[10] = {1,3,2,6,10,1,2,17,9,6};
    unsigned char codeLength[10]= {0};

    int i;

    maxLength = 6;
    numCode = 10;
    int fi = packageMerge(maxLength,numCode,histogram,codeLength);

    if(fi)
    {
        printf("max code length: %d\n",fi);
        for(i=0;i<numCode;i++)
        {
            printf("%2d: %d\n",i,(unsigned)codeLength[i]);
        }
    }
    else
    {
        printf("failed\n");
    }

    return 0;
}