
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


// ��ũ�� �ػ󵵿� ���󿡼� �νĵ� ��ũ���� ũ�⸦ �����Ѵ�.
void cScreen::Init(const cv::Rect &screenResolution, const vector<cv::Point> &screenContour,
	const int increaseScreenSize)
{
	m_screenResolution = screenResolution;
	m_screenContour = screenContour;
	m_increaseScreenLength = increaseScreenSize;

	// �νĵ� ��ũ������, ���Ʒ��� ũ�⸦ �ø���.
	m_screenContour[0].x -= increaseScreenSize;
	m_screenContour[0].y -= increaseScreenSize;
	m_screenContour[1].x += increaseScreenSize;
	m_screenContour[1].y -= increaseScreenSize;
	m_screenContour[2].x += increaseScreenSize;
	m_screenContour[2].y += increaseScreenSize;
	m_screenContour[3].x -= increaseScreenSize;
	m_screenContour[3].y += increaseScreenSize;
}


// ��ũ�� �ػ� ����.
void cScreen::InitResolution(const cv::Rect &screenResolution)
{
	m_screenResolution = screenResolution;
}


// ȭ�鿡 �ֻ��ϴ� ��ũ���� �ػ��� �ʺ� �����Ѵ�.
int cScreen::GetWidth()
{
	return m_screenResolution.width;
}


// ȭ�鿡 �ֻ��ϴ� ��ũ���� �ػ��� ���̸� �����Ѵ�.
int cScreen::GetHeight()
{
	return m_screenResolution.height;
}


// ���󿡼� �νĵ� ��ũ���� �ʺ� �����Ѵ�.
int cScreen::GetRecogWidth()
{
	return m_screenContour[1].x - m_screenContour[0].x - (m_increaseScreenLength * 2);
}


// ���󿡼� �νĵ� ��ũ���� ���̸� �����Ѵ�.
int cScreen::GetRecogHeight()
{
	return m_screenContour[3].y - m_screenContour[0].y - (m_increaseScreenLength * 2);
}


// ȭ�� �ػ� �� ���� �ν��� ������ ������ �����Ѵ�.
// width, height ������� �����Ѵ�.
pair<float, float> cScreen::GetResolutionRecognitionRate()
{
	return pair<float, float>(
		(float)GetRecogWidth() / (float)GetWidth(),
		(float)GetRecogHeight() / (float)GetHeight());
}


// ���� ���̺� ����
void cScreen::CreateCellMappingTable(const int cols, const int rows)
{
	m_cellCols = cols;
	m_cellRows = rows;
	m_cellMapping.resize(cols*rows);
}


// ���� ���̺� ����.
void cScreen::SetCellMappingTable(const int index, const cv::Rect &screenCell,
	const vector<cv::Point> &cameraCell)
{
	if (index >= (int)m_cellMapping.size())
		return;

	m_cellMapping[index].screenCell = screenCell;
	m_cellMapping[index].cameraCell = cameraCell;
}
