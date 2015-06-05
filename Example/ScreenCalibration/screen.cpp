
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
	return pair<float, float>(
		(float)GetRecogWidth() / (float)GetWidth(),
		(float)GetRecogHeight() / (float)GetHeight());
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
