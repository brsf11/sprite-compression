#include<stdio.h>
#include"packagemerge.h"

int main()
{
    unsigned char maxLength;
    unsigned numCode;
    unsigned histogram[10] = {1,3,2,6,10,1,2,17,9,6};
    unsigned char codeLength[10];

    int i;

    maxLength = 5;
    numCode = 10;
    packageMerge(maxLength,numCode,histogram,codeLength);

    for(i=0;i<numCode;i++)
    {
        printf("%2d: %d\n",codeLength[i]);
    }

    return 0;
}