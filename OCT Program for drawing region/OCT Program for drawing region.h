
// OCT Program for drawing region.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// COCTProgramfordrawingregionApp:
// �� Ŭ������ ������ ���ؼ��� OCT Program for drawing region.cpp�� �����Ͻʽÿ�.
//

class COCTProgramfordrawingregionApp : public CWinApp
{
public:
	COCTProgramfordrawingregionApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern COCTProgramfordrawingregionApp theApp;
