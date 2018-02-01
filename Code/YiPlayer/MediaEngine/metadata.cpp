/*
 * Copyright (c) 2011 Reinhard Tartler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * Shows how the metadata API can be used in application programs.
 * @example metadata.c
 */

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
#include <tchar.h>
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


int main()
{
    AVFormatContext *fmt_ctx = NULL;
    AVDictionaryEntry *tag = NULL;
    int ret;

    av_register_all();
    if ((ret = avformat_open_input(&fmt_ctx, "C:\\Users\\Jason Yin\\Desktop\\MediaTest\\aaaaaaaa\\CG.mp4", NULL, NULL)))
        return ret;

    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
        printf("%s=%s\n", tag->key, tag->value);

    avformat_close_input(&fmt_ctx);

	
    return 0;
}
