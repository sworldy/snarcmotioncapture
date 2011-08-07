// MainFrm.h : interface of the CMainFrame class
//
#include "SerialPort.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <string>
#include <winsock.h>
#include <windows.h>
#include <math.h>
#include "Timer.h"
#include "DeviceBuffer.h"
#include "Fusion.h"
#include <vector>
#include <afxmt.h>
#pragma once

#define GROUP_DATA_SIZE 304
#define SENSOR_NUMBER 16
#define LENGTH_OF_NODE 19
#define LENGTH_OF_NODE 19
#define DATA_DIMENSION 9
#define SKELETON_NUMBER 23
#define PORTNUMBER 8889
#define WS_VERSION_REQUIRED 0x0101
#define DSCALE 175.987693025/1.7
#define NAN 19

using namespace std;
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnReceiveChar(UINT ch, LONG port);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConnect();
	void HextoDec(char* buf, double* dest);
	CSerialPort m_CSerialPort;
	int m_nRecBytes;
	char buffer[GROUP_DATA_SIZE];
	double QuadBUf[4];
	Timer* m_timer;
	CCriticalSection cs;

	char PakTmp[LENGTH_OF_NODE];
	double buf[DATA_DIMENSION];
	ofstream *Qtfiles;
	afx_msg void StartReading();
	afx_msg void OnEditUndo();
	int StartServer(int pPort);
	bool Initialize();
	bool Cleanup();
	void bzero(char *b, size_t length);
	nsTime GetNanoSeconds();
	void UpdateBuffer(sDataBuffer& mDataBuffer);
	sockaddr_in	lClientAddr;
	int		lSize;
	int		lSocket;
	int Soc;
	sDataBuffer mDataBuffer;
	double Quat_Result[18][4]; // DIM_QUAT为4, quaternion维度；
	double Dis_Result[3]; // DIM_VEC=3. 3维向量，存储位移结果
	double Dis_AResult[3];
	double lastDis_AResult[3];
	double Quat_ResAug[2][DIM_QUAT];
	double dtmp[18][4],dtmp2[18][4],dtmp3[4],dtmp4[4];
	double lastQuat[18][4];
	bool init;
	bool IsClose;
	bool IsRunning;
	Fusion Quat_Dis;
	//UINT __stdcall myThreadFunc(LPVOID lparam);
	//afx_msg void OnAppExit();
	afx_msg void OnShut();
//	virtual BOOL DestroyWindow();
	bool IsClosed;
	afx_msg void OnClose();
};


