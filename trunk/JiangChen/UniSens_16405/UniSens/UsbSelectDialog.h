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

#pragma once

#include "MainFrm.h"
#include "resource.h"

// UsbSelectDialog dialog

class UsbSelectDialog : public CDialog
{
	DECLARE_DYNAMIC(UsbSelectDialog)

public:
	UsbSelectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~UsbSelectDialog();

// Dialog Data
	enum { IDD = IDD_SELECT };
	CComboBox	m_ctlDevicelist;
	int		m_deviceIndex;

	DeviceList m_deviceList;
	void Initialize(DeviceList* devList);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
