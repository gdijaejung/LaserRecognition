#pragma once


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvaux.h"
#include "opencv2/nonfree/nonfree.hpp"


using std::vector;


double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
void horizLines(cv::Mat &image, const int w, const int h, const int div);
void verticalLines(cv::Mat &image, const int w, const int h, const int div);


extern int g_screenThreshold;
extern cv::Rect g_screenRect;
extern vector<cv::Point> g_screenContour;

#include "maincamera.h"
extern cCapture g_camera;

