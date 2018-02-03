#pragma once

#include <string>

using namespace std;
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

  /*Include ffmpeg header file*/

class VideoInfo
{
public:
	VideoInfo();
	~VideoInfo();

	bool Open(string filePath);
	int64_t GetVideoLength();
public:
	string m_filePath;

private:
	AVFormatContext *pformat_ctx = NULL;
};

