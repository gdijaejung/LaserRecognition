// HelloOpenCV.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

#pragma comment(lib, "opencv_core2411d.lib")
#pragma comment(lib, "opencv_highgui2411d.lib")
#pragma comment(lib, "opencv_imgproc2411d.lib")
#pragma comment(lib, "opencv_features2d2411d.lib")
#pragma comment(lib, "opencv_nonfree2411d.lib")
#pragma comment(lib, "winmm.lib")


void horizLines(Mat &image, const int w, const int h, const int div)
{
	const bool startBlank = true;
	const int h2 = h / (div * 2);
	int yOffset = 0;

	for (int k = 0; k < div; ++k)
	{
		yOffset = k * h2 * 2;
		if (startBlank)
			yOffset += h2;

		for (int i = 0; i < h2; ++i)
			line(image, Point(0, i + yOffset), Point(w, i + yOffset), Scalar(0, 0, 0));
	}
}


void verticalLines(Mat &image, const int w, const int h, const int div)
{
	const bool startBlank = true;
	const int w2 = w / (div * 2);
	int xOffset = 0;

	for (int k = 0; k < div; ++k)
	{
		xOffset = k * w2 * 2;
		if (startBlank)
			xOffset += w2;

		for (int i = 0; i < w2; ++i)
			line(image, Point(i + xOffset, 0), Point(i + xOffset, h), Scalar(0, 0, 0));
	}
}


int main(int argc, char* argv[])
{
	CvFont font;
	double hScale = 1.0;
	double vScale = 1.0;
	int    lineWidth = 1;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth);

	//IplImage* image = cvCreateImage({1920, 1080}, IPL_DEPTH_8U, 3); // 흑백 이미지 생성
	//Mat image(1080, 1920, CV_8UC(3));
	Mat image(768, 1024, CV_8UC(3));
	image.setTo(Scalar(255, 255, 255));
//	namedWindow("Hello");
 	namedWindow("Hello", CV_WINDOW_NORMAL);
 	cvSetWindowProperty("Hello", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	const bool startBlank = true;
	const int w = 1024;
	const int h = 768;
	const int div = 32;
	verticalLines(image, w, h, div);
	imshow("Hello", image);



	//-----------------------------------------------------------------------------------------
	// 카메라 영상 정보 출력.
	IplImage *camera = 0;
	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("HUV05-camera", 0);
	cvResizeWindow("HUV05-camera", 640, 480);
	cvMoveWindow("HUV05-camera", 0, 0);

	int fps = 0;
	int frameCnt = 0;
	DWORD oldT = timeGetTime();
	// FPS를 표시해 줄 Font 및 문자열 버퍼 초기화  

	while (1) {
		// check frame per seconds
		++frameCnt;
		DWORD curT = timeGetTime();
		if (curT - oldT > 1000)
		{
			//printf("fps = %d\n", fps);
			fps = frameCnt;
			frameCnt = 0;
			oldT = curT;
		}
		//

		// 		cvGrabFrame(capture);
		// 		image = cvRetrieveFrame(capture);
		camera = cvQueryFrame(capture);
		if (!camera)
			continue;

		if (cvWaitKey(10) >= 0)
			break;

		cvShowImage("HUV05-camera", camera);
	}


	cvReleaseCapture(&capture);
	cvDestroyWindow("HUV05-camera");
	cvDestroyWindow("Hello");
	return 0;
}
