#pragma once
#include "VideoInfo.h"
#include "OperationBase.h"

enum ImageType
{
	ImageType_BMP = 0,
	ImageType_JPEG
};

class VideoToPhotos: public OperationBase
{
public:
	VideoToPhotos() {};
	~VideoToPhotos();

	bool Open(string filePath);
	int Start(ImageType _type, string _savePath);
	int Stop();
private:
	void SaveAsBMP(AVFrame *pFrameRGB, string _savePath, int index, int bpp);
	int SaveAsJPEG(AVFrame* pFrame, string _savePath, int iIndex);
	bool doContinue = true;
	
public:
	VideoInfo* m_pVideoInfo = NULL;
};

