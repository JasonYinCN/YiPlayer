#include <stdio.h>

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL) 
#endif

extern "C"
{
#include "include\libavcodec\avcodec.h"  
#include "include\libavformat\avformat.h"  
#include "include\libavutil\channel_layout.h"  
#include "include\libavutil\common.h"  
#include "include\libavutil\imgutils.h"  
#include "include\libswscale\swscale.h"   
#include "include\libavutil\imgutils.h"      
#include "include\libavutil\opt.h"         
#include "include\libavutil\mathematics.h"      
#include "include\libavutil\samplefmt.h" 
};//----´úÂëÇø--- #ifdef __cplusplus  } #endif 

#pragma comment(lib, "avcodec.lib")  
#pragma comment(lib, "avformat.lib")  
#pragma comment(lib, "avdevice.lib")  
#pragma comment(lib, "avfilter.lib")  
#pragma comment(lib, "avutil.lib")  
#pragma comment(lib, "postproc.lib")  
#pragma comment(lib, "swresample.lib")  
#pragma comment(lib, "swscale.lib")  

  /*Include ffmpeg header file*/