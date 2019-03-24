
// OCT Program for drawing regionDlg.h : ��� ����
//

#pragma once
#include "StaticGNUPlot.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// COCTProgramfordrawingregionDlg ��ȭ ����
class COCTProgramfordrawingregionDlg : public CDialogEx
{
// �����Դϴ�.
public:
	COCTProgramfordrawingregionDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OCTPROGRAMFORDRAWINGREGION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
