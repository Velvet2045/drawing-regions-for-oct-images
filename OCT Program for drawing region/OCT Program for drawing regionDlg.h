
// OCT Program for drawing regionDlg.h : ��� ����
//

#pragma once


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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
