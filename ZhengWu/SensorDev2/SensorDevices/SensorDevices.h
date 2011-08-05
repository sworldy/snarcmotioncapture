// SensorDevices.h : main header file for the SensorDevices application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSensorDevicesApp:
// See SensorDevices.cpp for the implementation of this class
//

class CSensorDevicesApp : public CWinApp
{
public:
	CSensorDevicesApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSensorDevicesApp theApp;