
// HelloOpenCV.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void main()
{
	IplImage* image = 0;
	CvCapture* capture = cvCaptureFromCAM(1);
	cvNamedWindow("T9-camera", 0);
	cvResizeWindow("T9-camera", 320, 240);

	while (1) {
		cvGrabFrame(capture);
		image = cvRetrieveFrame(capture);
		cvShowImage("T9-camera", image);

		if (cvWaitKey(10) >= 0)
			break;
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("T9-camera");
}
