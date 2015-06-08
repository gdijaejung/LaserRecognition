
#include "global.h"
#include "screen.h"

using namespace cv;


cScreen::cScreen() :
	m_cellCols(0)
	, m_cellRows(0)
{
}

cScreen::~cScreen()
{
}


// 스크린 해상도와 영상에서 인식된 스크린의 크기를 설정한다.
void cScreen::Init(const cv::Rect &screenResolution, const vector<cv::Point> &screenContour,
	const int increaseScreenSize)
{
	m_screenResolution = screenResolution;
	m_screenContour = screenContour;
	m_increaseScreenLength = increaseScreenSize;

	// 인식된 스크린에서, 위아래로 크기를 늘린다.
	m_screenContour[0].x -= increaseScreenSize;
	m_screenContour[0].y -= increaseScreenSize;
	m_screenContour[1].x += increaseScreenSize;
	m_screenContour[1].y -= increaseScreenSize;
	m_screenContour[2].x += increaseScreenSize;
	m_screenContour[2].y += increaseScreenSize;
	m_screenContour[3].x -= increaseScreenSize;
	m_screenContour[3].y += increaseScreenSize;
}


// 스크린 해상도 설정.
void cScreen::InitResolution(const cv::Rect &screenResolution)
{
	m_screenResolution = screenResolution;
}


// 화면에 주사하는 스크린의 해상도의 너비를 리턴한다.
int cScreen::GetWidth()
{
	return m_screenResolution.width;
}


// 화면에 주사하는 스크린의 해상도의 높이를 리턴한다.
int cScreen::GetHeight()
{
	return m_screenResolution.height;
}


// 영상에서 인식된 스크린의 너비를 리턴한다.
int cScreen::GetRecogWidth()
{
	return m_screenContour[1].x - m_screenContour[0].x - (m_increaseScreenLength * 2);
}


// 영상에서 인식된 스크린의 높이를 리턴한다.
int cScreen::GetRecogHeight()
{
	return m_screenContour[3].y - m_screenContour[0].y - (m_increaseScreenLength * 2);
}


// 화면 해상도 와 실제 인식한 영상간의 비율을 리턴한다.
// width, height 순서대로 리턴한다.
pair<float, float> cScreen::GetResolutionRecognitionRate()
{
	const float rate = (float)(GetRecogWidth() * GetRecogHeight()) / (float)(GetWidth() * GetHeight());
	//return pair<float, float>(
	//	(float)GetRecogWidth() / (float)GetWidth(),
	//	(float)GetRecogHeight() / (float)GetHeight());
	return pair<float, float>(rate, rate);
}


// 매핑 테이블 생성
void cScreen::CreateCellMappingTable(const int cols, const int rows)
{
	m_cellCols = cols;
	m_cellRows = rows;
	m_cellMapping.resize(cols*rows);
}


// 매핑 테이블 설정.
void cScreen::SetCellMappingTable(const int index, const cv::Rect &screenCell,
	const vector<cv::Point> &cameraCell)
{
	if (index >= (int)m_cellMapping.size())
		return;

	m_cellMapping[index].screenCell = screenCell;
	m_cellMapping[index].cameraCell = cameraCell;
}


void cScreen::CalculateCellMapping()
{
	//for each (auto table in m_cellMapping)
	for (u_int i = 0; i < m_cellMapping.size(); ++i)
	{
		auto &table = m_cellMapping[i];

		vector<Point2f> cells(4);
		cells[0] = Point2f((float)table.screenCell.x, (float)table.screenCell.y);
		cells[1] = Point2f((float)table.screenCell.x + table.screenCell.width, (float)table.screenCell.y);
		cells[2] = Point2f((float)(table.screenCell.x + table.screenCell.width), (float)(table.screenCell.y + table.screenCell.height));
		cells[3] = Point2f((float)table.screenCell.x, (float)(table.screenCell.y + table.screenCell.height));

		vector<Point2f> cameraCell(4);
		cameraCell[0] = Point2f((float)table.cameraCell[0].x, (float)table.cameraCell[0].y);
		cameraCell[1] = Point2f((float)table.cameraCell[1].x, (float)table.cameraCell[1].y);
		cameraCell[2] = Point2f((float)table.cameraCell[2].x, (float)table.cameraCell[2].y);
		cameraCell[3] = Point2f((float)table.cameraCell[3].x, (float)table.cameraCell[3].y);

		table.tm = getPerspectiveTransform(cameraCell, cells);
	}

}


// 영상에서 얻은 포인터의 위치를 실제 스크린에서의 위치로 변환해서 리턴한다.
cv::Point cScreen::GetPointPos(const cv::KeyPoint &point)
{
	const int cameraW = m_screenContour[1].x - m_screenContour[0].x - (m_increaseScreenLength * 2);
	const int cameraH = m_screenContour[3].y - m_screenContour[0].y - (m_increaseScreenLength * 2);
	const int cellW = cameraW / m_cellCols;
	const int cellH = cameraH / m_cellRows;
	const float x = point.pt.x - m_increaseScreenLength;
	const float y = point.pt.y - m_increaseScreenLength;

	const int cellX = (int)(x / (float)cellW);
	const int cellY = (int)(y / (float)cellH);

	const int index = (cellY * m_cellCols) + cellX;
	if ((int)m_cellMapping.size() <= index)
		return Point(0, 0);

	vector<float> pos(3);
	pos[0] = point.pt.x;
	pos[1] = point.pt.y;
	pos[2] = 1;


	Mat src(3/*rows*/, 1 /* cols */, CV_64F);
	src.at<double>(0, 0) = point.pt.x;
	src.at<double>(1, 0) = point.pt.y;
	src.at<double>(2, 0) = 1.0f;
	Mat dst = m_cellMapping[index].tm * src; //USE MATRIX ALGEBRA 
	//return Point2f(dst.at<double>(0, 0), dst.at<double>(1, 0));


	//MatExpr expr = m_cellMapping[index].tm.mul(pos);
// 	Mat m = m_cellMapping[index].tm.mul(pos);
// 	getAffineTransform()
	//vector<float> ret(3);
	//cvMul(m_cellMapping[index].tm, pos, &ret);


	return Point( (int)dst.at<double>(0, 0), (int)dst.at<double>(1, 0));
}
