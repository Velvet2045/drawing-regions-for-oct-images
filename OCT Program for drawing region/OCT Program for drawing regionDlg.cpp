
// OCT Program for drawing regionDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "OCT Program for drawing region.h"
#include "OCT Program for drawing regionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;
// global variables ///////////////////////////////////////////////////////////////////////////////

const int c_cMaxFiles = 130;
const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
const Scalar SCALAR_BLACK = Scalar(0.0, 0.0, 0.0);
const Scalar SCALAR_WHITE = Scalar(255.0, 255.0, 255.0);
const Scalar SCALAR_GREEN = Scalar(0.0, 255.0, 0.0);
static double pixelWidth = 0, pixelHeight = 0, pixelDepth = 0;

Rect m_roi;
BOOL m_bDrawingRect = FALSE;
CString m_strFile, m_strFolder;

string CS2S(CString cstr);
void OnMouse(int event, int x, int y, int flags, void* userdata);
void WriteText(CString path, CString contents);
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COCTProgramfordrawingregionDlg 대화 상자



COCTProgramfordrawingregionDlg::COCTProgramfordrawingregionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OCTPROGRAMFORDRAWINGREGION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COCTProgramfordrawingregionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COCTProgramfordrawingregionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FIND_IMAGE, &COCTProgramfordrawingregionDlg::OnBnClickedBtnFindImage)
END_MESSAGE_MAP()


// COCTProgramfordrawingregionDlg 메시지 처리기

BOOL COCTProgramfordrawingregionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_bDrawingRect = FALSE;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COCTProgramfordrawingregionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COCTProgramfordrawingregionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COCTProgramfordrawingregionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

Mat COCTProgramfordrawingregionDlg::CropImage(Mat imgInput, int* key)
{
	if (imgInput.empty()) return imgInput;

	Mat imgOrg, imgGray, imgBin, imgOut;
	imgOrg = imgInput.clone();
	bool find = false;
	cvtColor(imgOrg,				// input image
		imgGray,                       // output image
		CV_BGR2GRAY);						// change image from BGR to GRAY scale

	threshold(imgGray,				// input image
		imgBin,						// output image
		180,									// thresholding value
		255,								// maximum value
		CV_THRESH_BINARY);					// thresholding 방식
											//	cv::imshow("OTSU", imgBinarized);


	vector<vector<Point>> morphContours, morphLargeContours, morphSmallContours;							// make contour variables
	//Mat imgMorphContours(imgBinarized.size(), CV_8UC1, Scalar(0));							// makes contours image
	//imshow("binary", imgBin);
	//waitKey(0);
	findContours(imgBin, morphContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);		// find contours

	for (unsigned int i = 0; i < morphContours.size(); i++) {
		if (contourArea(morphContours[i]) > 62 && contourArea(morphContours[i]) < 67 &&
			((float)boundingRect(morphContours[i]).width / (float)boundingRect(morphContours[i]).height) < 0.5 &&
			(float)boundingRect(morphContours[i]).width / (float)boundingRect(morphContours[i]).height > 0.3) {
			morphLargeContours.push_back(morphContours[i]);
			find = true;
		}
	}

	if (find) {
		for (unsigned int i = 0; i < morphContours.size(); i++) {
			if (boundingRect(morphContours[i]).y + boundingRect(morphContours[i]).height == boundingRect(morphLargeContours[0]).y + boundingRect(morphLargeContours[0]).height) {
				morphSmallContours.push_back(morphContours[i]);
			}
		}
	}
	else if (!find) {
		for (unsigned int i = 0; i < morphContours.size(); i++) {
			if (contourArea(morphContours[i]) > 25 && cv::contourArea(morphContours[i]) < 30 &&
				((float)boundingRect(morphContours[i]).width / (float)boundingRect(morphContours[i]).height) < 1.1 &&
				(float)boundingRect(morphContours[i]).width / (float)boundingRect(morphContours[i]).height > 0.9) {
				morphLargeContours.push_back(morphContours[i]);
			}
		}
		for (unsigned int i = 0; i < morphContours.size(); i++) {
			if (boundingRect(morphContours[i]).y + boundingRect(morphContours[i]).height == boundingRect(morphLargeContours[0]).y + boundingRect(morphLargeContours[0]).height) {
				morphSmallContours.push_back(morphContours[i]);
			}
		}
	}
	//	cv::drawContours(imgMorphContours, morphLargeContours, -1, SCALAR_WHITE, -1);						// draw contours
	//	cv::drawContours(imgMorphContours, morphSmallContours, -1, SCALAR_WHITE, -1);						// draw contours

	cv::Rect rectContour(boundingRect(morphLargeContours[0]).x, 0,
		imgOrg.cols - (boundingRect(morphLargeContours[0]).x), boundingRect(morphLargeContours[0]).y + boundingRect(morphLargeContours[0]).height + 10);

	pixelWidth = 200 / (double)boundingRect(morphLargeContours[0]).width; // 너비당 200um 
	pixelHeight = 200 / (double)boundingRect(morphLargeContours[0]).height; // 높이당 200um
	pixelDepth = 200 / (double)boundingRect(morphSmallContours[0]).height; // 두께당 200um
	imgOut = imgOrg(rectContour);
	//	cv::rectangle(imgOriginal, rectContour, SCALAR_GREEN, 1);

	if (!find) { *key = 40; }
	return imgOut;
}


void COCTProgramfordrawingregionDlg::OnBnClickedBtnFindImage()
{
	CString strFileList;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("모든 이미지 파일 (*.jpg;*.bmp;*.png;*.tif)|*.jpg;*.bmp;*.png;*.tif||"));
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	if (dlg.DoModal() == IDOK) {
		int cnt = 0, key = 60; // 이미지 유형에 따라 RPE Layer의 마스크 두께를 결정하는 변수
		m_strFolder.Format(_T("%s\\result"), dlg.GetFolderPath());
		CreateDirectory(m_strFolder, NULL);

		POSITION pos = dlg.GetStartPosition();
#pragma omp parallel for
		while (pos != NULL) {
			Mat imgOrg = imread(CS2S(dlg.GetNextPathName(pos)));
			if (imgOrg.empty()) {
				AfxMessageBox(_T("error: image not read from file\n\n"));
				break;
			}

			Mat imgRes = CropImage(imgOrg, &key);

			Rect botRect(0, (int)(imgRes.rows * 0.7), imgRes.cols, (int)(imgRes.rows * 0.3));
			rectangle(imgRes, botRect, SCALAR_BLACK, -1);

			m_strFile.Format(_T("region_%d.jpg"), cnt);
			CString strFile = m_strFolder + _T("\\") + m_strFile;
			imshow("region", imgRes);
			imwrite(CS2S(strFile), imgRes);
			setMouseCallback("region", OnMouse, (void*)&imgRes);
			waitKey();
			cnt++;
		}
	}
}

string CS2S(CString cstr)
{
	CT2CA caCstr(cstr);
	std::string str(caCstr);
	return str;
}

// 마우스 콜백 함수
// 사용자가 마우스 왼쪽 버튼을 누르면 사각형 그리기를 시작한다.
// 마우스 버튼을 누른 채 드래그하면 사각형 크기를 변화시킨다.
// 마우스 버튼을 떼어놓으면 최종적으로 영상에 사각형을 그린다.
void OnMouse(int event, int x, int y, int flags, void* userdata) {
	Mat* pMat = (Mat*)userdata;
	Mat img = Mat(*pMat);

	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		if (m_bDrawingRect) {
			m_roi.width = x - m_roi.x;
			m_roi.height = y - m_roi.y;
		}
		break;
	case CV_EVENT_LBUTTONDOWN: 
		m_bDrawingRect = TRUE;
		m_roi = Rect(x, y, 0, 0);
		break;

	case CV_EVENT_LBUTTONUP: 
		m_bDrawingRect = FALSE;
		if (m_roi.width < 0) {
			m_roi.x += m_roi.width;
			m_roi.width *= -1;
		}
		if (m_roi.height < 0) {
			m_roi.y += m_roi.height;
			m_roi.height *= -1;
		}
		rectangle(img, m_roi, SCALAR_GREEN);
		imshow("region", img);
		CString strLabel, strFile = m_strFolder + _T("\\label.txt");
		strLabel.Format(_T("%s,%d,%d,%d,%d,particle"), m_strFile, m_roi.x, m_roi.y, m_roi.x + m_roi.width, m_roi.y + m_roi.height);

		WriteText(strFile, strLabel);
		break;
	}
}

void WriteText(CString path, CString contents)
{
	CStdioFile file;

	if (file.Open(path, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite)) {
		file.SeekToEnd();
		file.WriteString(contents + _T("\n"));
		file.Close();
	}
}