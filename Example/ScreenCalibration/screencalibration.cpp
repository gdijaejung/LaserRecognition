//
// 화면 휘어짐을 보정하는 알고리즘.
//

#include "global.h"
#include "screendetect.h"

using namespace cv;


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

//void drawRectangle(Mat &image, const )

int main(int argc, char* argv[])
{
	std::vector<cv::Point> screenLines(4);
	Rect screenRect = ScreenDetect(screenLines);

	
	
/*
	CvFont font;
	double hScale = 1.0;
	double vScale = 1.0;
	int    lineWidth = 1;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth);

	//IplImage* image = cvCreateImage({1920, 1080}, IPL_DEPTH_8U, 3); // 흑백 이미지 생성
	//Mat image(1080, 1920, CV_8UC(3));
	Mat image(768, 1024, CV_8UC(3));
	//image.setTo(Scalar(255, 255, 255));
	image.setTo(Scalar(0, 0, 0));
//	namedWindow("Hello");
 	namedWindow("Hello", CV_WINDOW_NORMAL);
 	cvSetWindowProperty("Hello", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	const bool startBlank = true;
	const int w = 1024;
	const int h = 768;
	const int div = 32;
//	verticalLines(image, w, h, div);
	rectangle(image, Rect(0, 0, 100, 100), Scalar(255, 255,255), CV_FILLED);
	imshow("Hello", image);



	//-----------------------------------------------------------------------------------------
	// 카메라 영상 정보 출력.
	int threshold = 128; // 임계값(Threshold) 설정

	IplImage *camera = 0;
	IplImage *binImage = 0;
	IplImage *binOutput = 0;
	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("HUV05-camera", 0);
	cvNamedWindow("HUV05-binarization", 0);
	cvResizeWindow("HUV05-camera", 640, 480);
	cvResizeWindow("HUV05-binarization", 640, 480);
	cvMoveWindow("HUV05-camera", -660, 0);
	cvMoveWindow("HUV05-binarization", -660, 500);
	cvCreateTrackbar("Threshold", "HUV05-camera", &threshold, 255, NULL); // "HUV05-camera" 윈도우에 bar 를 붙인다. 


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

		camera = cvQueryFrame(capture);
		if (!camera)
			continue;

		if (cvWaitKey(10) >= 0)
			break;

		// 카메라 영상을 이진화 한다.
		if (!binImage) {
			binImage = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
			binOutput = cvCreateImage({ 640, 480 }, IPL_DEPTH_8U, 1); // 흑백 이미지 생성
		}
		cvCvtColor(camera, binImage, CV_RGB2GRAY); // 컬러를 흑백으로 변환
		// 영상의 각 픽셀(x,y) 값이 threshold 값의 초과는 255 로, 그 이하는 0 으로 변환
		cvThreshold(binImage, binOutput, threshold, 255, CV_THRESH_BINARY);
		binOutput->origin = camera->origin; // 방향이 뒤집어 진것을 바로 잡아줌

		cv::Mat bw;
		cv::Canny(Mat(binOutput), bw, 0, 50, 5);

		// Find contours
		std::vector<std::vector<cv::Point> > contours;
		cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		std::vector<cv::Point> approx;

		cv::Mat dst(camera);

		for (u_int i = 0; i < contours.size(); i++)
		{
			// Approximate contour with accuracy proportional
			// to the contour perimeter
			cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

// 			for (u_int k = 0; k < approx.size()-1; ++k)
// 				line(dst, approx[k], approx[k+1], Scalar(255, 0, 0), 2);
// 			if (!approx.empty())
// 				line(dst, approx[approx.size()-1], approx[0], Scalar(255, 0, 0), 2);
			// Skip small or non-convex objects 
// 			if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
// 				continue;
// 
// 			if (approx.size() >= 4)
// 			{
// 				// Number of vertices of polygonal curve
// 				int vtc = approx.size();
// 
// 				// Get the cosines of all corners
// 				std::vector<double> cos;
// 				for (int j = 2; j < vtc + 1; j++)
// 					cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));
// 
// 				// Sort ascending the cosine values
// 				std::sort(cos.begin(), cos.end());
// 
// 				// Get the lowest and the highest cosine
// 				double mincos = cos.front();
// 				double maxcos = cos.back();
// 
// 				// Use the degrees obtained above and the number of vertices
// 				// to determine the shape of the contour
// // 				if (vtc >= 4 && mincos >= -0.1 && maxcos <= 0.3)
// // 				{
// // 					setLabel(bw, "RECT", contours[i]);
// // 
// // 					line(bw, approx[0], approx[1], Scalar(255, 0, 0), 2);
// // 					line(bw, approx[1], approx[2], Scalar(255, 0, 0), 2);
// // 					line(bw, approx[2], approx[3], Scalar(255, 0, 0), 2);
// // 					line(bw, approx[3], approx[0], Scalar(255, 0, 0), 2);
// // 				}
// 
// 				line(dst, approx[0], approx[1], Scalar(255, 0, 0), 2);
// 					line(dst, approx[1], approx[2], Scalar(255, 0, 0), 2);
// 					line(dst, approx[2], approx[3], Scalar(255, 0, 0), 2);
// 					line(dst, approx[3], approx[0], Scalar(255, 0, 0), 2);
// 
// 			}
		}

		cvShowImage("HUV05-camera", camera);
		//cvShowImage("HUV05-binarization", binOutput);
		imshow("HUV05-binarization", bw);
		imshow("HUV05-rect", dst);
	}


	cvReleaseCapture(&capture);
	cvDestroyWindow("HUV05-camera");
	cvDestroyWindow("HUV05-binarization");
	cvDestroyWindow("Hello");
*/
	return 0;
}

