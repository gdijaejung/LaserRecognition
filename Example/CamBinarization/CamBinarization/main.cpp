// 카메라 영상을 이진화해서 출력하는 예제.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvaux.h"

using namespace cv;

void main()
{
	int threshold = 128; // 임계값(Threshold) 설정
	IplImage* image = 0;
	IplImage* output = 0;
	IplImage* gray = 0;

	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("HUV05-camera", 0);
	cvNamedWindow("HUV05-output", 0);
	cvResizeWindow("HUV05-camera", 640, 480);
	cvResizeWindow("HUV05-output", 640, 480);
	cvCreateTrackbar("Threthold", "HUV05-camera", &threshold, 255, NULL); // "HUV05-camera" 윈도우에 bar 를 붙이고 

	cvMoveWindow("HUV05-camera", 0, 0);
	cvMoveWindow("HUV05-output", 660, 0);

	while (1) {
		cvGrabFrame(capture);
		image = cvRetrieveFrame(capture);
		if (!image)
			continue;

		cvShowImage("HUV05-camera", image);

		if (!output){
			gray = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
			output = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
		}

		cvCvtColor(image, gray, CV_RGB2GRAY); // 컬러를 흑백으로 변환

		if (cvWaitKey(10) >= 0)
			break;
		
		// 영상의 각 픽셀(x,y) 값이 threshold 값의 초과는 255 로, 그 이하는 0 으로 변환
		cvThreshold(gray, output, threshold, 255, CV_THRESH_BINARY);
		output->origin = image->origin; // 방향이 뒤집어 진것을 바로 잡아줌

		cvShowImage("HUV05-output", output);
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("HUV05-camera");
	cvDestroyWindow("HUV05-output");
}
