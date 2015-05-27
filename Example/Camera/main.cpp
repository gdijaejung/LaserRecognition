// 카메라 영상 출력 예제

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void main()
{
	IplImage* image = 0;
	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("HUV05-camera", 0);
	cvResizeWindow("HUV05-camera", 640, 480);

	while (1) {
		cvGrabFrame(capture);
		image = cvRetrieveFrame(capture);
		cvShowImage("HUV05-camera", image);

		if (cvWaitKey(10) >= 0)
			break;
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("HUV05-camera");
}
