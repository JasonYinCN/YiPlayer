// PlayerCore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL) 
#endif

int main()
{
	AVDictionaryEntry *tag = NULL;

	AVCodec *pCodec = NULL;
	AVCodecContext *pCodec_C = NULL;
	AVPacket pk = {0};
	AVFormatContext *pformat_ctx = NULL;
	int vindex = -1;
	pformat_ctx = avformat_alloc_context();
	if (pformat_ctx == NULL)
	{
		printf("pformat_ctx alloc error!");
		return 1;
	}
	string filename = "C:\\Users\\Jason Yin\\Desktop\\MediaTest\\aaaaaaaa\\PopCap.mp4";
	av_register_all();
 	if (avformat_open_input(&pformat_ctx, filename.c_str(), NULL, NULL) != 0)
 		printf("Couldn't open input stream.\n"); // 不能打开此文件

    return 0;
}

