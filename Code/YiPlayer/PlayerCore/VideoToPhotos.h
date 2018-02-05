#pragma once
#include "VideoInfo.h"
#include "OperationBase.h"
using namespace System;
using namespace std;

namespace Video2Photos
{
	public enum class ImageType
	{
		ImageType_BMP = 0,
		ImageType_JPEG
	};

	public ref class VideoToPhotos
	{
	//º¯Êý
	public:
		VideoToPhotos() {};
		~VideoToPhotos();
	
		bool Open(String^ filePath);
		int Start(ImageType _type, String^ _savePath);
	private:
		void SaveAsBMP(AVFrame *pFrameRGB, string _savePath, int index, int bpp);
		int SaveAsJPEG(AVFrame *pFrame, string _savePath, int iIndex);
		bool doContinue = true;
		double m_progress = 0;
	
	//±äÁ¿
	public:
		VideoInfo^ m_videoInfo = nullptr;
	private:
		AVCodecContext *m_pCodecCtx = NULL;
		AVFormatContext *m_pFormatCtx = NULL;
		AVCodec *m_pCodec = NULL;
		AVFrame *m_pFrame = NULL, *m_pFrameRGB = NULL;
		struct SwsContext *pSwsCtx = NULL;
		AVPacket *m_pPacket = NULL;
	};
}


