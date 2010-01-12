/*===================================================*\
|                                                     |
|  Uni-Sensor Demostration System                     |
|                                                     |
|  Copyright (c) 2009 - Guanhong Tao                  |
|      macrotao86@gmail.com                           |
|      tgh05@software.nju.edu.cn                      |
|                                                     |
|  Created:                                           |
|         2009/03/25                                  |
|  License:                                           |
|                                                     |
|  Copyright 2009 Guanhong Tao, All Rights Reserved.  |
|   For educational purposes only.                    |
|   Please do not republish in electronic or          |
|   print form without permission                     |
|                                                     |
|  Email me for the  docs and new versions            |
|                                                     |
\*===================================================*/

// UsbSelectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "UniSens.h"
#include "UsbSelectDialog.h"


// UsbSelectDialog dialog

IMPLEMENT_DYNAMIC(UsbSelectDialog, CDialog)

UsbSelectDialog::UsbSelectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(UsbSelectDialog::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_deviceIndex = -1;
}

void UsbSelectDialog::Initialize(DeviceList* devList) 
{
	//CDialog::OnInitDialog();
	m_deviceList = *devList;
}
BOOL UsbSelectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	int i;

	for ( i = 0; i < m_deviceList.size(); i++)
	{
		m_ctlDevicelist.AddString(m_deviceList[i]);
	}
	m_ctlDevicelist.SetCurSel(i - 1);

	return TRUE;
	
}
UsbSelectDialog::~UsbSelectDialog()
{
}

void UsbSelectDialog::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICELIST, m_ctlDevicelist);
	DDX_CBIndex(pDX, IDC_DEVICELIST, m_deviceIndex);
}


BEGIN_MESSAGE_MAP(UsbSelectDialog, CDialog)

END_MESSAGE_MAP()


// UsbSelectDialog message handlers
