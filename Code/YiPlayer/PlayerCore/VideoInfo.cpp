#include "stdafx.h"
#include "VideoInfo.h"
#include <msclr\marshal_cppstd.h> 
using namespace msclr::interop;


VideoInfo::VideoInfo()
{
	av_register_all();
}


VideoInfo::~VideoInfo()
{
}

bool VideoInfo::Open(String^ filePath)
{
	FilePath = filePath; 
	AVFormatContext *pformat_ctx = NULL;
	marshal_context context;
	string strSavePath = context.marshal_as<std::string>(filePath);
	bool res = avformat_open_input(&pformat_ctx, context.marshal_as<std::string>(filePath).c_str(), NULL, NULL) == 0;
	if (res == false)
		return false;
	else
	{
		Duration = pformat_ctx->duration;
	}
}
