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

#pragma comment(lib, "avcodec.lib")  
#pragma comment(lib, "avformat.lib")  
#pragma comment(lib, "avdevice.lib")  
#pragma comment(lib, "avfilter.lib")  
#pragma comment(lib, "avutil.lib")  
#pragma comment(lib, "postproc.lib")  
#pragma comment(lib, "swresample.lib")  
#pragma comment(lib, "swscale.lib")  

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

	AVDictionaryEntry *tag = NULL;
	AVCodec *pCodec = NULL;
	AVCodecContext *pCodec_C = NULL;
	AVPacket pk = { 0 };
	AVFormatContext *pformat_ctx = NULL;
};

