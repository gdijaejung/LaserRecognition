//
// ��ũ���� ���� �� �۾��� ó���Ѵ�.
// �ν��� ��ũ���� ũ���, ���� �������� ��µǴ� ��ũ���� ũ�� ������ �����Ѵ�.
// 
// ī�޶��� ���� Ư���� ���� ��ũ���� ������� �����ϴ� ������ �Ѵ�.
//
#pragma once

struct sCellMapping
{
	cv::Rect screenCell; // ȭ�鿡�� ����ϴ� cell ��ġ
	vector<cv::Point> cameraCell; // ī�޶󿡼� �ν��� cell ��ġ
};


class cScreen
{
public:
	cScreen();
	virtual ~cScreen();

	void Init(const cv::Rect &screenResolution, const vector<cv::Point> &screenContour, 
		const int increaseScreenSize = 20);

	void InitResolution(const cv::Rect &screenResolution);

	int GetWidth();
	int GetHeight();
	int GetRecogWidth();
	int GetRecogHeight();
	pair<float,float> GetResolutionRecognitionRate();

	const cv::Rect& GetScreenResolution() const;
	const vector<cv::Point>& GetScreenContour() const;

	void CreateCellMappingTable(const int cols, const int rows);
	void SetCellMappingTable(const int index, const cv::Rect &screenCell,
		const vector<cv::Point> &cameraCell);
	const vector<sCellMapping>& GetCellMappingTable() const;


protected:
	cv::Rect m_screenResolution; // {0,0,w,h}
	vector<cv::Point> m_screenContour;	// �νĵ� ��ũ���� ũ�� (�ε��� ����)
										// 0 ---------- 1
										// |            |
										// |            |
										// |            |
										// 2 ---------- 3
										//

	int m_increaseScreenLength; // �νĵ� ��ũ������ ����,���� ������ų ���� �����Ѵ�.


	int m_cellCols;
	int m_cellRows;
	vector<sCellMapping> m_cellMapping; // ��ũ���� ����ϴ� �������� ī�޶󿡼� �ν��ϴ� �������� �����ϰ� �����Ѵ�.
};


inline const cv::Rect& cScreen::GetScreenResolution() const { return m_screenResolution; }
inline const vector<cv::Point>& cScreen::GetScreenContour() const { return m_screenContour; }
inline const vector<sCellMapping>& cScreen::GetCellMappingTable() const { return m_cellMapping; }
