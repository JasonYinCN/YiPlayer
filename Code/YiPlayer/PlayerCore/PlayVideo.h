#pragma once

#include "WinDef.h"
#include "stdafx.h"
#include <stdio.h>
#include <string>

class PlayVideo
{
public:
	PlayVideo();
	~PlayVideo();

	ULONGLONG Open(string filePath);
public:
	string m_filePath;
private:
	
	AVDictionaryEntry *tag = NULL;
	AVCodec *pCodec = NULL;
	AVCodecContext *pCodec_C = NULL;
	AVPacket pk = { 0 };
	AVFormatContext *pformat_ctx = NULL;
};