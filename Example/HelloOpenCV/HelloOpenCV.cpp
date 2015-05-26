// HelloOpenCV.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	
	Mat f = imread("C:/opencv/sources/samples/cpp/baboon.jpg");
	if (f.empty())
		return 0;

	namedWindow("Hello");
	imshow("Hello", f);
//	putText(f, "Hello, monkey", Point(50, 60), Font();
	waitKey();

	return 0;
}
