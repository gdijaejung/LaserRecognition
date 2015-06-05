
#include "global.h"
#include "pointdetectcalibration.h"


using namespace cv;


// ����Ʈ �˻縦 �ùķ��̼� �Ѵ�.
void DetectPointCalibration()
{
	const int w = g_screen.GetWidth();
	const int h = g_screen.GetHeight();
	const int rows = 5;
	const int cols = 5;
	const int cellW = w / cols;
	const int cellH = h / rows;

	int threshold = 33;
	IplImage *camera = 0;
	IplImage *binImage = 0;
	IplImage *binOutput = 0;

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

	std::vector<vector<Point> > contours(1);
	contours[0] = g_screen.GetScreenContour();

	
	
	Point2f pointPos(100,100);
	Point2f pointV(30, 30);
	Point recogPointPos(500, 300);
	const float circleW = 30;
	const float circleH = 30;
	
	int curT = timeGetTime();
	int oldT = curT;

	while (1)
	{
		// ������ ������.
		curT = timeGetTime();
		float incT = (float)(curT - oldT) / 1000.f;
		if (incT > 0.1f)
			incT = 0.1f;

		oldT = curT;
		
		Point2f pointCurPos = pointPos + pointV * incT;
		if (pointCurPos.x < 0)
			pointV.x = -pointV.x;
		if (pointCurPos.x > contours[0][1].x)
			pointV.x = -pointV.x;
		if (pointCurPos.y < 0)
			pointV.y = -pointV.y;
		if (pointCurPos.y > contours[0][3].y)
			pointV.y = -pointV.y;
		pointPos = pointCurPos;


		screen.setTo(Scalar(255, 255, 255));
		ellipse(screen, RotatedRect(recogPointPos, Size2f(circleW+5, circleH+5), 0), Scalar(200, 200, 255), CV_FILLED);
		ellipse(screen, RotatedRect(pointCurPos, Size2f(circleW, circleH), 0), Scalar(0, 0, 0), CV_FILLED);
		imshow("Screen", screen);

		camera = g_camera.GetCapture();


		// ��ũ�� ��ġ ���� ����ŷ�ؼ� �����´�.
		Mat src(camera);
		Mat mask = Mat::zeros(src.rows, src.cols, CV_8UC1);
		drawContours(mask, contours, 0, Scalar(255), CV_FILLED);
		Mat maskingScreen(src.rows, src.cols, CV_8UC3);
		maskingScreen.setTo(Scalar(0, 0, 0));
		src.copyTo(maskingScreen, mask);

		IplImage dummyScreen(maskingScreen); // ����ȭ ó���� ���� ���� ��ũ�� ����

		// ī�޶� ������ ����ȭ �Ѵ�.
		if (!binImage) {
			binImage = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // ��� �̹��� ����
			binOutput = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // ��� �̹��� ����
		}

		cvCvtColor(&dummyScreen, binImage, CV_RGB2GRAY); // �÷��� ������� ��ȯ
		// ������ �� �ȼ�(x,y) ���� threshold ���� �ʰ��� 255 ��, �� ���ϴ� 0 ���� ��ȯ
		cvThreshold(binImage, binOutput, threshold, 255, CV_THRESH_BINARY);
		binOutput->origin = dummyScreen.origin; // ������ ������ ������ �ٷ� ��� ��

		cvShowImage("HUV05-camera", &dummyScreen);
		cvShowImage("HUV05-binarization", binOutput);
		
		
		//--------------------------------------------------------------------------
		// ��ũ�� ���� Rect ���
		Mat dst(camera);
		vector<Point> rectLines = g_screen.GetScreenContour();
		line(dst, rectLines[0], rectLines[1], Scalar(255, 0, 0), 1);
		line(dst, rectLines[1], rectLines[2], Scalar(255, 0, 0), 1);
		line(dst, rectLines[2], rectLines[3], Scalar(255, 0, 0), 1);
		line(dst, rectLines[3], rectLines[0], Scalar(255, 0, 0), 1);
		//imshow("HUV05-screen", dst);


		//----------------------------------------------------------------------------------------------
		// ��ũ�� �̹������� �����͸� ã�´�.
		SimpleBlobDetector::Params params;

		// Change thresholds
		params.minThreshold = 1;
		params.maxThreshold = 20;

		// Filter by Area.
		params.filterByArea = true;
		//params.minArea = 1500;
		params.minArea = 10;
		params.maxArea = 200;

		// Filter by Circularity
		params.filterByCircularity = true;
		params.minCircularity = 0.1f;

		// Filter by Convexity
		params.filterByConvexity = true;
		params.minConvexity = 0.87f;

		// Filter by Inertia
		params.filterByInertia = true;
		params.minInertiaRatio = 0.01f;

		SimpleBlobDetector detector(params);

		// Detect blobs.
		std::vector<KeyPoint> keypoints;
		detector.detect(binOutput, keypoints);

		int posX = 0, posY = 0;
		if (!keypoints.empty())
		{
// 			const int screenW = approx[2].x - approx[0].x;
// 			const int screenH = approx[1].y - approx[0].y;
// 
// 			int newPosX = ((int)keypoints[0].pt.x - approx[0].x);
// 			int newPosY = ((int)keypoints[0].pt.y - approx[0].y);
// 
// 			newPosX = max(newPosX, 0);
// 			newPosX = min(screenW, newPosX);
// 			newPosY = max(newPosY, 0);
// 			newPosY = min(screenH, newPosY);
// 
// 			posX = newPosX;
// 			posY = newPosY;

			recogPointPos = g_screen.GetPointPos(keypoints[0]);
		}

		// Draw detected blobs as red circles.
		// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
		Mat im_with_keypoints;
		drawKeypoints(dst, keypoints, im_with_keypoints, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		imshow("HUV05-screen", im_with_keypoints);


		if (cvWaitKey(10) >= 0)
			break;
	}


	cvDestroyWindow("HUV05-camera");
	cvDestroyWindow("HUV05-binarization");
	cvDestroyWindow("HUV05-screen");
	cvDestroyWindow("Screen");
}
