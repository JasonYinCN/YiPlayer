#include "stdafx.h"
#include "VideoToPhotos.h"
#include <string.h>  
#include <wingdi.h>
#include <msclr\marshal_cppstd.h> 
using namespace msclr::interop;
using namespace Video2Photos;

VideoToPhotos::~VideoToPhotos()
{

}

bool VideoToPhotos::Open(String^ filePath)
{
	if (m_videoInfo == nullptr)
		m_videoInfo = gcnew VideoInfo();
	if (m_pCodecCtx != NULL)
		avcodec_close(m_pCodecCtx);
	
	return m_videoInfo->Open(filePath);
}

int VideoToPhotos::Start(ImageType _type, String^ _savePath)
{
	if (m_videoInfo == nullptr)
		return -1;

	marshal_context context;
	string strSavePath = context.marshal_as<std::string>(_savePath);
	string strFilePath = context.marshal_as<std::string>(m_videoInfo->FilePath);
	int videoStream = -1;
	int frameFinished;
	int PictureSize;
	uint8_t *outBuff;
	AVPixelFormat avFormat = AVPixelFormat::AV_PIX_FMT_BGR24;
	bool doRotate = true;
	switch (_type)
	{
	case ImageType::ImageType_BMP:
		break;
	case ImageType::ImageType_JPEG:
		avFormat = AVPixelFormat::AV_PIX_FMT_YUV420P;
		doRotate = false;
		break;
	default:
		break;
	}

	//ע��������
	av_register_all();
	// ��ʼ������ģ��
	avformat_network_init();
	// ����AVFormatContext
	m_pFormatCtx = avformat_alloc_context();
	//����Ƶ�ļ�
	if (avformat_open_input(&m_pFormatCtx, strFilePath.c_str(), NULL, NULL) != 0) {
		std::printf("av open input file failed!\n");
		exit(1);
	}

	//��ȡ����Ϣ
	if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0) 
	{
		std::printf("av find stream info failed!\n");
		exit(1);
	}
	
	//��ȡ��Ƶ��
	for (int i = 0; i < m_pFormatCtx->nb_streams; i++) 
	{
		if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1) {
		std::printf("find video stream failed!\n");
		exit(1);
	}

	// Ѱ�ҽ�����
	m_pCodecCtx = m_pFormatCtx->streams[videoStream]->codec;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
	if (m_pCodec == NULL) {
		std::printf("avcode find decoder failed!\n");
		exit(1);
	}

	//�򿪽�����
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0) {
		std::printf("avcode open failed!\n");
		exit(1);
	}

	//Ϊÿ֡ͼ������ڴ�
	m_pFrame = av_frame_alloc();
	m_pFrameRGB = av_frame_alloc();
	if ((m_pFrame == NULL) || (m_pFrameRGB == NULL)) {
		std::printf("avcodec alloc frame failed!\n");
		exit(1);
	}

	// ȷ��ͼƬ�ߴ�
	PictureSize = avpicture_get_size(avFormat, m_pCodecCtx->width, m_pCodecCtx->height);
	outBuff = (uint8_t*)av_malloc(PictureSize);
	if (outBuff == NULL) {
		std::printf("av malloc failed!\n");
		exit(1);
	}
	avpicture_fill((AVPicture *)m_pFrameRGB, outBuff, avFormat, m_pCodecCtx->width, m_pCodecCtx->height);

	//����ͼ��ת��������
	pSwsCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
		m_pCodecCtx->width, m_pCodecCtx->height, avFormat,
		SWS_BICUBIC, NULL, NULL, NULL);
	int imageCount = m_pFormatCtx->streams[videoStream]->nb_frames;
	int i = 0;
	while (av_read_frame(m_pFormatCtx, m_pPacket) >= 0 && doContinue)
	{
		if (m_pPacket->stream_index == videoStream)
		{
			avcodec_decode_video2(m_pCodecCtx, m_pFrame, &frameFinished, m_pPacket);
			if (frameFinished) 
			{
				m_pFrame->width = m_pCodecCtx->width;
				m_pFrame->height = m_pCodecCtx->height;
				if (doRotate)
				{
					//��תͼ�� ���������ɵ�ͼ�������µ�����
					m_pFrame->data[0] += m_pFrame->linesize[0] * (m_pCodecCtx->height - 1);
					m_pFrame->linesize[0] *= -1;
					m_pFrame->data[1] += m_pFrame->linesize[1] * (m_pCodecCtx->height / 2 - 1);
					m_pFrame->linesize[1] *= -1;
					m_pFrame->data[2] += m_pFrame->linesize[2] * (m_pCodecCtx->height / 2 - 1);
					m_pFrame->linesize[2] *= -1;
				}

				//ת��ͼ���ʽ������ѹ������YUV420P��ͼ��ת��ΪBRG24��ͼ��
				sws_scale(pSwsCtx, m_pFrame->data,
					m_pFrame->linesize, 0, m_pCodecCtx->height,
					m_pFrameRGB->data, m_pFrameRGB->linesize);

				m_pFrameRGB->width = m_pFrame->width;
				m_pFrameRGB->height = m_pFrame->height;
				
				switch (_type)
				{
				case ImageType::ImageType_BMP:
					SaveAsBMP(m_pFrameRGB, strSavePath.c_str(), i++, 24);
					break;
				case ImageType::ImageType_JPEG:
					SaveAsJPEG(m_pFrameRGB, strSavePath.c_str(), i++);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			//do nothing
		}
		m_progress =(i / (double)imageCount);//����
		av_free_packet(m_pPacket);

	}

	sws_freeContext(pSwsCtx);
	av_free(m_pFrame);
	av_free(m_pFrameRGB);
	avcodec_close(m_pCodecCtx);
	avformat_close_input(&m_pFormatCtx);

	printf("---------------- over ---------------��");
	return 0;
}

//����BMP�ļ��ĺ���
 void VideoToPhotos::SaveAsBMP(AVFrame *pFrameRGB, string _savePath, int index, int bpp)
 {
 	char buf[5] = { 0 };
 	BITMAPFILEHEADER bmpheader;
 	BITMAPINFOHEADER bmpinfo;
 	FILE *fp;
 
 	char *filename = new char[255];
	int width = pFrameRGB->width;
	int height = pFrameRGB->height;

 	//�ļ����·���������Լ����޸�
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
 * ��AVFrame(YUV420��ʽ)����ΪJPEG��ʽ��ͼƬ
 *
 * @param width YUV420�Ŀ�
 * @param height YUV42�ĸ�
 *
 */
 int VideoToPhotos::SaveAsJPEG(AVFrame* pFrame, string _savePath, int iIndex)
 {
	 // ����ļ�·��  
	 char out_file[MAX_PATH] = { 0 };
	 sprintf_s(out_file, sizeof(out_file), "%s%d.jpg", _savePath.c_str(), iIndex);

	 // ����AVFormatContext����  
	 AVFormatContext* pFormatCtx = avformat_alloc_context();

	 // ��������ļ���ʽ  
	 pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);
	 // ��������ʼ��һ���͸�url��ص�AVIOContext  
	 if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
		 printf("Couldn't open output file.");
		 return -1;
	 }

	 // ����һ����stream  
	 AVStream* pAVStream = avformat_new_stream(pFormatCtx, 0);
	 if (pAVStream == NULL) {
		 return -1;
	 }

	 // ���ø�stream����Ϣ  
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

	 // ���ҽ�����  
	 AVCodec* pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	 if (!pCodec) {
		 printf("Codec not found.");
		 return -1;
	 }
	 // ����pCodecCtx�Ľ�����ΪpCodec  
	 if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		 printf("Could not open codec.");
		 return -1;
	 }

	 //Write Header  
	 avformat_write_header(pFormatCtx, NULL);

	 int y_size = pCodecCtx->width * pCodecCtx->height;

	 //Encode  
	 // ��AVPacket�����㹻��Ŀռ�  
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