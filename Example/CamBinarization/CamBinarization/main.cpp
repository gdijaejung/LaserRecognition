// ī�޶� ������ ����ȭ�ؼ� ����ϴ� ����.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvaux.h"

using namespace cv;

void main()
{
	int threshold = 128; // �Ӱ谪(Threshold) ����
	IplImage* image = 0;
	IplImage* output = 0;
	IplImage* gray = 0;

	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("HUV05-camera", 0);
	cvNamedWindow("HUV05-output", 0);
	cvResizeWindow("HUV05-camera", 640, 480);
	cvResizeWindow("HUV05-output", 640, 480);
	cvCreateTrackbar("Threthold", "HUV05-camera", &threshold, 255, NULL); // "HUV05-camera" �����쿡 bar �� ���̰� 

	cvMoveWindow("HUV05-camera", 0, 0);
	cvMoveWindow("HUV05-output", 660, 0);

	while (1) {
		cvGrabFrame(capture);
		image = cvRetrieveFrame(capture);
		if (!image)
			continue;

		cvShowImage("HUV05-camera", image);

		if (!output){
			gray = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // ��� �̹��� ����
			output = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // ��� �̹��� ����
		}

		cvCvtColor(image, gray, CV_RGB2GRAY); // �÷��� ������� ��ȯ

		if (cvWaitKey(10) >= 0)
			break;
		
		// ������ �� �ȼ�(x,y) ���� threshold ���� �ʰ��� 255 ��, �� ���ϴ� 0 ���� ��ȯ
		cvThreshold(gray, output, threshold, 255, CV_THRESH_BINARY);
		output->origin = image->origin; // ������ ������ ������ �ٷ� �����

		cvShowImage("HUV05-output", output);
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("HUV05-camera");
	cvDestroyWindow("HUV05-output");
}
