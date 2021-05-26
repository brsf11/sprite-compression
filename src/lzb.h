#ifndef _LZB_H_
#define _LZB_H_
#include<stdint.h>


void Byte2Fb(const char* source,char* dest,unsigned numSource);
uint32_t matchSeq(const char* windowSP,const char* seqSP,const char* sourceSP,const char* enSP);
#endif