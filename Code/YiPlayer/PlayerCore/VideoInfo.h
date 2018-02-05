#pragma once

#include <string>

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

#include "windows.h"
using namespace std;
using namespace System;


public ref class VideoInfo
{
public:
	VideoInfo();
	~VideoInfo();

	bool Open(String^ filePath);
public:
	String^ FilePath = nullptr;
	int64_t Duration = 0;
};

