
// OCT Program for drawing regionDlg.h : 헤더 파일
//

#pragma once
#include "StaticGNUPlot.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// COCTProgramfordrawingregionDlg 대화 상자
class COCTProgramfordrawingregionDlg : public CDialogEx
{
// 생성입니다.
public:
	COCTProgramfordrawingregionDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OCTPROGRAMFORDRAWINGREGION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnFindImage();
	//afx_msg MouseCallback OnMouse(int event, int x, int y, int flags, void* userdata);
	DECLARE_MESSAGE_MAP()
public:
	CStaticGNUPlot m_plot;
	Mat CropImage(Mat imgInput, int* key);
};
