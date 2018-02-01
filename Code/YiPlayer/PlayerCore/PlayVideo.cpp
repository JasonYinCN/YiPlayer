
#include "PlayVideo.h"
 
PlayVideo::PlayVideo()
{
	av_register_all();
	pformat_ctx = avformat_alloc_context();
}

PlayVideo::~PlayVideo()
{
}

bool PlayVideo::Open(string filePath)
{
	m_filePath = filePath;
	return avformat_open_input(&pformat_ctx, m_filePath.c_str(), NULL, NULL) == 0;
}

ULONGLONG PlayVideo::GetVideoLength()
{
	if (pformat_ctx != NULL)
		return pformat_ctx->duration;
	return 0;
}