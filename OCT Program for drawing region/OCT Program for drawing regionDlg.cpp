
// OCT Program for drawing regionDlg.cpp : ���� ����
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
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// COCTProgramfordrawingregionDlg ��ȭ ����



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


// COCTProgramfordrawingregionDlg �޽��� ó����

BOOL COCTProgramfordrawingregionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_bDrawingRect = FALSE;

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void COCTProgramfordrawingregionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
		CV_THRESH_BINARY);					// thresholding ���
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

	pixelWidth = 200 / (double)boundingRect(morphLargeContours[0]).width; // �ʺ�� 200um 
	pixelHeight = 200 / (double)boundingRect(morphLargeContours[0]).height; // ���̴� 200um
	pixelDepth = 200 / (double)boundingRect(morphSmallContours[0]).height; // �β��� 200um
	imgOut = imgOrg(rectContour);
	//	cv::rectangle(imgOriginal, rectContour, SCALAR_GREEN, 1);

	if (!find) { *key = 40; }
	return imgOut;
}


void COCTProgramfordrawingregionDlg::OnBnClickedBtnFindImage()
{
	CString strFileList;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("��� �̹��� ���� (*.jpg;*.bmp;*.png;*.tif)|*.jpg;*.bmp;*.png;*.tif||"));
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	if (dlg.DoModal() == IDOK) {
		int cnt = 0, key = 60; // �̹��� ������ ���� RPE Layer�� ����ũ �β��� �����ϴ� ����
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

// ���콺 �ݹ� �Լ�
// ����ڰ� ���콺 ���� ��ư�� ������ �簢�� �׸��⸦ �����Ѵ�.
// ���콺 ��ư�� ���� ä �巡���ϸ� �簢�� ũ�⸦ ��ȭ��Ų��.
// ���콺 ��ư�� ��������� ���������� ���� �簢���� �׸���.
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