#include "stdafx.h"
#include "VideoInfo.h"


VideoInfo::VideoInfo()
{
	av_register_all();
}


VideoInfo::~VideoInfo()
{
}

bool VideoInfo::Open(string filePath)
{
	m_filePath = filePath;
	return avformat_open_input(&pformat_ctx, m_filePath.c_str(), NULL, NULL) == 0;
}

int64_t VideoInfo::GetVideoLength()
{
	if (pformat_ctx != NULL)
		return pformat_ctx->duration;
	return 0;
}
