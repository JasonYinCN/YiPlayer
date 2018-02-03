#include "stdafx.h"
#include "VideoToPhotos.h"
#include "windows.h"
#include <wingdi.h>


VideoToPhotos::~VideoToPhotos()
{
	 if (m_pVideoInfo != NULL)
	 {
		 delete m_pVideoInfo;
		 m_pVideoInfo = NULL;
	 }
}

bool VideoToPhotos::Open(string filePath)
{
	if (m_pVideoInfo == NULL)
		m_pVideoInfo = new VideoInfo();
		
	return m_pVideoInfo->Open(filePath);
}

int VideoToPhotos::Start(ImageType _type, string _savePath)
{
	if (m_pVideoInfo == NULL)
		return -1;

	int videoStream = -1;
	AVCodecContext *pCodecCtx = NULL;
	AVFormatContext *pFormatCtx = NULL;
	AVCodec *pCodec = NULL;
	AVFrame *pFrame = NULL, *pFrameRGB = NULL;
	struct SwsContext *pSwsCtx = NULL;
	AVPacket packet;
	int frameFinished;
	int PictureSize;
	uint8_t *outBuff;
	AVPixelFormat avFormat = AVPixelFormat::AV_PIX_FMT_BGR24;
	bool doRotate = true;
	switch (_type)
	{
	case ImageType_BMP:
		break;
	case ImageType_JPEG:
		avFormat = AVPixelFormat::AV_PIX_FMT_YUV420P;
		doRotate = false;
		break;
	default:
		break;
	}

	//注册编解码器
	av_register_all();
	// 初始化网络模块
	avformat_network_init();
	// 分配AVFormatContext
	pFormatCtx = avformat_alloc_context();

	//打开视频文件
	if (avformat_open_input(&pFormatCtx, m_pVideoInfo->m_filePath.c_str(), NULL, NULL) != 0) {
		printf("av open input file failed!\n");
		exit(1);
	}

	//获取流信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		printf("av find stream info failed!\n");
		exit(1);
	}
	
	//获取视频流
	for (int i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1) {
		printf("find video stream failed!\n");
		exit(1);
	}

	// 寻找解码器
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		printf("avcode find decoder failed!\n");
		exit(1);
	}

	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("avcode open failed!\n");
		exit(1);
	}

	//为每帧图像分配内存
	pFrame = av_frame_alloc();
	pFrameRGB = av_frame_alloc();
	if ((pFrame == NULL) || (pFrameRGB == NULL)) {
		printf("avcodec alloc frame failed!\n");
		exit(1);
	}

	// 确定图片尺寸
	PictureSize = avpicture_get_size(avFormat, pCodecCtx->width, pCodecCtx->height);
	outBuff = (uint8_t*)av_malloc(PictureSize);
	if (outBuff == NULL) {
		printf("av malloc failed!\n");
		exit(1);
	}
	avpicture_fill((AVPicture *)pFrameRGB, outBuff, avFormat, pCodecCtx->width, pCodecCtx->height);

	//设置图像转换上下文
	pSwsCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, avFormat,
		SWS_BICUBIC, NULL, NULL, NULL);
	int imageCount = pFormatCtx->streams[videoStream]->nb_frames;
	int i = 0;
	while (av_read_frame(pFormatCtx, &packet) >= 0 && doContinue) {
		if (packet.stream_index == videoStream) {
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

			if (frameFinished) 
			{
				pFrame->width = pCodecCtx->width;
				pFrame->height = pCodecCtx->height;
				if (doRotate)
				{
					//反转图像 ，否则生成的图像是上下调到的
					pFrame->data[0] += pFrame->linesize[0] * (pCodecCtx->height - 1);
					pFrame->linesize[0] *= -1;
					pFrame->data[1] += pFrame->linesize[1] * (pCodecCtx->height / 2 - 1);
					pFrame->linesize[1] *= -1;
					pFrame->data[2] += pFrame->linesize[2] * (pCodecCtx->height / 2 - 1);
					pFrame->linesize[2] *= -1;
				}

				//转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
				sws_scale(pSwsCtx, pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				pFrameRGB->width = pFrame->width;
				pFrameRGB->height = pFrame->height;
				
				switch (_type)
				{
				case ImageType_BMP:
					SaveAsBMP(pFrameRGB, _savePath, i++, 24);
					break;
				case ImageType_JPEG:
					SaveAsJPEG(pFrameRGB, _savePath, i++);
					break;
				default:
					break;
				}
			}
		}
		else {
			int a = 2;
			int b = a;
		}
		m_progress =(i / (double)imageCount);//进度
		av_free_packet(&packet);
	}

	sws_freeContext(pSwsCtx);
	av_free(pFrame);
	av_free(pFrameRGB);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	printf("---------------- over ---------------！");
	return 0;
}

int VideoToPhotos::Stop()
{
	doContinue = false;
	return 0;
}

//保存BMP文件的函数
 void VideoToPhotos::SaveAsBMP(AVFrame *pFrameRGB, string _savePath, int index, int bpp)
 {
 	char buf[5] = { 0 };
 	BITMAPFILEHEADER bmpheader;
 	BITMAPINFOHEADER bmpinfo;
 	FILE *fp;
 
 	char *filename = new char[255];
	int width = pFrameRGB->width;
	int height = pFrameRGB->height;

 	//文件存放路径，根据自己的修改
 	sprintf_s(filename, 255, "%s%d.bmp", _savePath.c_str(), index);
 	if ((fp = fopen(filename, "wb+")) == NULL) {
 		printf("open file failed!\n");
 		return;
 	}
 
 	bmpheader.bfType = 0x4d42;
 	bmpheader.bfReserved1 = 0;
 	bmpheader.bfReserved2 = 0;
 	bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 	bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp / 8;
 
 	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
 	bmpinfo.biWidth = width;
 	bmpinfo.biHeight = height;
 	bmpinfo.biPlanes = 1;
 	bmpinfo.biBitCount = bpp;
 	bmpinfo.biCompression = BI_RGB;
 	bmpinfo.biSizeImage = (width*bpp + 31) / 32 * 4 * height;
 	bmpinfo.biXPelsPerMeter = 100;
 	bmpinfo.biYPelsPerMeter = 100;
 	bmpinfo.biClrUsed = 0;
 	bmpinfo.biClrImportant = 0;
 
 	fwrite(&bmpheader, sizeof(bmpheader), 1, fp);
 	fwrite(&bmpinfo, sizeof(bmpinfo), 1, fp);
 	fwrite(pFrameRGB->data[0], width*height*bpp / 8, 1, fp);
 
 	fclose(fp);
 }

 /**
 * 将AVFrame(YUV420格式)保存为JPEG格式的图片
 *
 * @param width YUV420的宽
 * @param height YUV42的高
 *
 */
 int VideoToPhotos::SaveAsJPEG(AVFrame* pFrame, string _savePath, int iIndex)
 {
	 // 输出文件路径  
	 char out_file[MAX_PATH] = { 0 };
	 sprintf_s(out_file, sizeof(out_file), "%s%d.jpg", _savePath.c_str(), iIndex);

	 // 分配AVFormatContext对象  
	 AVFormatContext* pFormatCtx = avformat_alloc_context();

	 // 设置输出文件格式  
	 pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);
	 // 创建并初始化一个和该url相关的AVIOContext  
	 if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
		 printf("Couldn't open output file.");
		 return -1;
	 }

	 // 构建一个新stream  
	 AVStream* pAVStream = avformat_new_stream(pFormatCtx, 0);
	 if (pAVStream == NULL) {
		 return -1;
	 }

	 // 设置该stream的信息  
	 AVCodecContext* pCodecCtx = pAVStream->codec;

	 pCodecCtx->codec_id = pFormatCtx->oformat->video_codec;
	 pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	 pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
	 pCodecCtx->width = pFrame->width;
	 pCodecCtx->height = pFrame->height;
	 pCodecCtx->time_base.num = 1;
	 pCodecCtx->time_base.den = 25;

	 // Begin Output some information  
	 av_dump_format(pFormatCtx, 0, out_file, 1);
	 // End Output some information  

	 // 查找解码器  
	 AVCodec* pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	 if (!pCodec) {
		 printf("Codec not found.");
		 return -1;
	 }
	 // 设置pCodecCtx的解码器为pCodec  
	 if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		 printf("Could not open codec.");
		 return -1;
	 }

	 //Write Header  
	 avformat_write_header(pFormatCtx, NULL);

	 int y_size = pCodecCtx->width * pCodecCtx->height;

	 //Encode  
	 // 给AVPacket分配足够大的空间  
	 AVPacket pkt;
	 av_new_packet(&pkt, y_size * 3);

	 //   
	 int got_picture = 0;
	 int ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);
	 if (ret < 0) {
		 printf("Encode Error.\n");
		 return -1;
	 }
	 if (got_picture == 1) {
		 //pkt.stream_index = pAVStream->index;  
		 ret = av_write_frame(pFormatCtx, &pkt);
	 }

	 av_free_packet(&pkt);

	 //Write Trailer  
	 av_write_trailer(pFormatCtx);
	 printf("Encode Successful.\n");

	 if (pAVStream) {
		 avcodec_close(pAVStream->codec);
	 }
	 avio_close(pFormatCtx->pb);
	 avformat_free_context(pFormatCtx);

	 return 0;
 }