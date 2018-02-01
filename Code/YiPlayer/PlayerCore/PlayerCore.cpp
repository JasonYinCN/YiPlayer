// PlayerCore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include "PlayVideo.h"
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL) 
#endif

int main()
{
	PlayVideo* playVideoObj = new PlayVideo();
	playVideoObj->Open("C:\\Users\\Jason Yin\\Desktop\\MediaTest\\aaaaaaaa\\PopCap.mp4");
	ULONGLONG videoLength = playVideoObj->GetVideoLength();

    return 0;
}

