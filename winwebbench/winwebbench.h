
// winwebbench.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CwinwebbenchApp: 
// �йش����ʵ�֣������ winwebbench.cpp
//

class CwinwebbenchApp : public CWinApp
{
public:
	CwinwebbenchApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CwinwebbenchApp theApp;