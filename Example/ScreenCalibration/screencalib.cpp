
#include "global.h"
#include "screencalib.h"
#include "findrectcontour.h"


using namespace cv;


// ��ũ�� ���� ���� �����Ѵ�.
void ScreenCalibration( const vector<Point> &screenContour )
{
	const int w = 1024;
	const int h = 768;
	const int rows = 4;
	const int cols = 4;
	const int cellW = w / cols;
	const int cellH = h / rows;

	int threshold = 240;
	IplImage *camera = 0;
	IplImage *binImage = 0;
	IplImage *binOutput = 0;
	//CvCapture* capture = cvCaptureFromCAM(0);


	Mat screen(h, w, CV_8UC(3));
	namedWindow("Screen", CV_WINDOW_NORMAL);
	cvSetWindowProperty("Screen", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	cvNamedWindow("HUV05-camera", 0);
	cvNamedWindow("HUV05-binarization", 0);
	cvNamedWindow("HUV05-screen", 0);

	cvResizeWindow("HUV05-camera", 640, 480);
	cvResizeWindow("HUV05-binarization", 640, 480);
	cvResizeWindow("HUV05-screen", 640, 480);

	cvMoveWindow("HUV05-camera", -660, 0);
	cvMoveWindow("HUV05-binarization", -660, 520);
	cvMoveWindow("HUV05-screen", -660, 0);
	cvCreateTrackbar("Threshold", "HUV05-camera", &threshold, 255, NULL); // "HUV05-camera" �����쿡 bar �� ���δ�. 
	cvCreateTrackbar("Threshold", "HUV05-screen", &threshold, 255, NULL); // "HUV05-camera" �����쿡 bar �� ���δ�. 

	std::vector<std::vector<cv::Point> > contours(1);
	contours[0] = screenContour;

	cvWaitKey(300);


	int count = 0;
	while (count < (rows * cols))
	{
		const int r = count / rows;
		const int c = count % cols;

		screen.setTo(Scalar(0, 0, 0));
		rectangle(screen, Rect(c*cellW, r*cellH, cellW, cellH), Scalar(255, 255, 255), CV_FILLED);
		imshow("Screen", screen);


// 		camera = cvQueryFrame(capture);
// 		if (!camera)
// 			continue;
		camera = g_camera.GetCapture();


		// ��ũ�� ��ġ ���� �����´�.
		Mat src(camera);
		Mat mask = Mat::zeros(src.rows, src.cols, CV_8UC1);
		drawContours(mask, contours, 0, Scalar(255), CV_FILLED);
		Mat crop(src.rows, src.cols, CV_8UC3);
		crop.setTo(Scalar(0, 0, 0));
		src.copyTo(crop, mask);

		IplImage screen(crop);


		// ī�޶� ������ ����ȭ �Ѵ�.
		if (!binImage) {
			binImage = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // ��� �̹��� ����
			binOutput = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // ��� �̹��� ����
		}

		cvCvtColor(&screen, binImage, CV_RGB2GRAY); // �÷��� ������� ��ȯ
		// ������ �� �ȼ�(x,y) ���� threshold ���� �ʰ��� 255 ��, �� ���ϴ� 0 ���� ��ȯ
		cvThreshold(binImage, binOutput, threshold, 255, CV_THRESH_BINARY_INV);
		binOutput->origin = screen.origin; // ������ ������ ������ �ٷ� ��� ��

		cvShowImage("HUV05-camera", &screen);
		cvShowImage("HUV05-binarization", binOutput);


		Mat dst(&screen);
		vector<Point> rectLines;
		if (FindRectContour(&screen, binImage, (int)((cellH*cellW)*0.5f), (int)((cellH*cellW)*2.0f), rectLines))
		{
			setLabel(dst, "RECT", rectLines);
			line(dst, rectLines[0], rectLines[1], Scalar(255, 0, 0), 2);
			line(dst, rectLines[1], rectLines[2], Scalar(255, 0, 0), 2);
			line(dst, rectLines[2], rectLines[3], Scalar(255, 0, 0), 2);
			line(dst, rectLines[3], rectLines[0], Scalar(255, 0, 0), 2);
			imshow("HUV05-screen", dst);

			if (cvWaitKey(3000) >= 0)
				break;

			++count;
		}
		else
		{
			imshow("HUV05-screen", dst);
			if (cvWaitKey(1) >= 0)
				break;
		}
	}

// 		}
// 	}

	cvWaitKey(300);

//	cvReleaseCapture(&capture);
	cvDestroyWindow("HUV05-camera");
	cvDestroyWindow("HUV05-binarization");
	cvDestroyWindow("HUV05-screen");
	cvDestroyWindow("Screen");
}

