// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SensorDevices.h"
#include "MainFrm.h"
#include "SelectPort.h"
#include <process.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
static double ServerStartedOn = 0.0;
static UINT __stdcall myThreadFunc(LPVOID lpParameter)
{
	CMainFrame*   pThis   =   (CMainFrame*)lpParameter;
	pThis->lSize = sizeof(pThis->lClientAddr);
	pThis->bzero((char *)&(pThis->lClientAddr), sizeof(pThis->lClientAddr));
	pThis->lSocket = accept(pThis->Soc, (struct sockaddr*)&(pThis->lClientAddr), &(pThis->lSize));
	if(pThis->lSocket >= 0)
	{
		sockaddr_in	lAddr;
		

		pThis->bzero((char *)&lAddr, sizeof(lAddr));
		if( getsockname(pThis->lSocket, (struct sockaddr*)&lAddr, &(pThis->lSize)) < 0 )
		{
			//return -1;
		}
		AfxMessageBox(_T("Connection established\n"));
		ServerStartedOn = (double)pThis->GetNanoSeconds();

		pThis->mDataBuffer.SetupBuffer();
	}
	return 0;
}
static int 	sensor2bone[31]  = {3,7,8,9,NAN,NAN,0,1,2,NAN,NAN,4,4,5,6,NAN,16,10,11,12,NAN,NAN,NAN,17,13,14,15,NAN,NAN,NAN};


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_CONNECT, &CMainFrame::OnConnect)
	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveChar)
	ON_COMMAND(ID_Menu, &CMainFrame::StartReading)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
	ON_WM_TIMER()
	//ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
	ON_COMMAND(ID_SHUT, &CMainFrame::OnShut)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	for (int i = 0;i <18;i++)
	{
		for (int j = 0;j<3;j++)
		{
			lastQuat[i][j] = 0;
		}
		lastQuat[i][3] = 1;
	}
	init = true;
	IsClose = false;
	IsRunning = false;
	IsClosed = true;
	//Fusion* Quat_Dis = new Fusion();
	//CMainFrame* s = new CMainFrame;
}

CMainFrame::~CMainFrame()
{
	cs.Lock();
	cs.Unlock();
	shutdown(lSocket, 2);
	closesocket( lSocket );
	if (Soc) 
	{
		closesocket( Soc );
	}
	Cleanup();
	
}

std::string Int2Str(int i)
{
		std::stringstream ss;
        ss << i;
        return ss.str();
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	string FileName;
	Qtfiles = new ofstream[16];
	for(int i=0; i<16; ++i)
	{
		FileName.clear();
		FileName = Int2Str(i) + ".txt";
		Qtfiles[i].open( FileName.c_str() );

		if(!Qtfiles[i])
		break;
	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnConnect()
{
	// TODO: Add your command handler code here
	//GetSystemMenu(FALSE)-> EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_GRAYED);
	SelectPort comdlg;
	if(comdlg.DoModal() != IDOK)
		return;
	int SelPortNO,SelBaudRate,SelDataBits,SelStopBits;
	char SelParity;
	CString temp;
	char str[32] = {0};

	UpdateData(true);
	temp=comdlg.m_strPortNO;
	temp.Delete(0,3);
	wcstombs(str, temp, temp.GetLength() );
	SelPortNO  = atoi(str);
	SelBaudRate= 115200;
	SelDataBits= 8;
	SelParity  ='N';
	SelStopBits= 1;


	if(m_CSerialPort.InitPort(this, SelPortNO, SelBaudRate,SelParity, SelDataBits, SelStopBits , EV_RXCHAR|EV_CTS, 512))		
	{
		m_nRecBytes = 0;
		m_CSerialPort.StartMonitoring();
		AfxMessageBox(_T("Connect Successfully!"));
	}
	else 
	{
		AfxMessageBox(_T("This serial port has been used!\nTry others!"));
		return ;
	}
}


void CMainFrame::HextoDec(char* buf, double* dest)
{
		unsigned char* b = (unsigned char*)buf;
		short a[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		int i;
		for ( i = 1; i < 19; i+=2) {
			short tmp1 = b[i];
			short tmp2 = b[i+1];
			a[i/2] = ((tmp1 & 0x00FF) << 8)|(tmp2 & 0x00FF);
		}
		for ( i = 0; i < 9; i++) 
		{
			if(a[i]&0x8000==0x8000)
			{
				a[i]	=	(~a[i]+1);
				a[i] = -a[i];
			}
			if (i < 3)
				dest[i] = a[i]*9.8/64.0;
			else if (i < 6)
				dest[i] = a[i]*2*3.1415926/360/14.375;
			else
				dest[i] = a[i]*0.059;  
		}
}

LRESULT  CMainFrame::OnReceiveChar(UINT ch, LONG port)
{
	double quadF[SENSOR_NUMBER][4];

	float QuadRec2Ani[SKELETON_NUMBER][4];
	double quadR[SKELETON_NUMBER][4];

	float Displace[3];
	double dis[3];

	double* pbuf;
	
	buffer[m_nRecBytes%GROUP_DATA_SIZE] = (char)ch;	
	for (int i = 0;i<4;i++)
	{
		QuadBUf[i] = 0;
	}
	if(m_nRecBytes>0 && (m_nRecBytes+1)%GROUP_DATA_SIZE == 0)
	{
		if (IsClose)
		{
			PostMessage(WM_QUIT,NULL,NULL);
		}
		cs.Lock();
		for (int i = 0;i<SENSOR_NUMBER;i++)
		{
			for (int j = 0; j<LENGTH_OF_NODE;j++)
			{
				PakTmp[j] = buffer[i*19+j];
			}
			HextoDec(PakTmp, buf);
			for(int k = 0; k<9; ++k)
				Qtfiles[i] << buf[k] << "\t";
			Qtfiles[i]<<"\n";
			
			double x = sqrt(buf[0]*buf[0] + buf[1]*buf[1] + buf[2]*buf[2]);
			if(x > 600.0)
			{
				AfxMessageBox( _T("Connection down!") );
				IsClosed = true;
				PostMessage(WM_QUIT,NULL,NULL);
			}
			Quat_Dis.Get_Measure(i,buf);
		}
		Quat_Dis.WholeBodyFusion(Quat_Result, Dis_Result);
		Quat_Dis.Quaternion_Augment2(Quat_Result); 

		
		m_nRecBytes = 0;

		Dis_AResult[2] = Dis_Result[0];
		Dis_AResult[0] = -1 * Dis_Result[1];
		Dis_AResult[1] = -1 * Dis_Result[2];
		Dis_AResult[1] += 0.80;
		// Send Data
		mDataBuffer.Simulate(GetNanoSeconds());

		if (send( lSocket, (char*)&mDataBuffer,sizeof(mDataBuffer), 0)==SOCKET_ERROR)
		{
			AfxMessageBox(_T("Send Error, Maybe Connection Closed"));
			IsClosed = true;
			PostMessage(WM_QUIT,NULL,NULL);
		}

		UpdateBuffer(mDataBuffer);
		cs.Unlock();
	}
	else
		m_nRecBytes++;
	return 1;
}
void CMainFrame::StartReading()
{
	// TODO: Add your command handler code here
	m_CSerialPort.WriteToPort("S",1);
	IsRunning = true;
	IsClosed = false;
}

void CMainFrame::OnEditUndo()
{
	Soc=0;
	if (!Soc) 
	{
		Soc = StartServer( PORTNUMBER );
		if (Soc) 
		{
			AfxMessageBox(_T("Listening Port 8889"));
			AfxBeginThread((AFX_THREADPROC)myThreadFunc,this);
		}
	} 
	else
	{
		AfxMessageBox(_T("Start Server Failed!"));
	}

}

int CMainFrame::StartServer(int pPort)
{
	int lSocket;
	struct protoent* lP;
	struct sockaddr_in  lSin;

	Initialize();

	lP = getprotobyname("tcp");

	lSocket = socket(AF_INET, SOCK_STREAM, lP->p_proto);

	if (lSocket)
	{
		bzero((char *)&lSin, sizeof(lSin));

		lSin.sin_family = AF_INET;
		lSin.sin_port = htons(pPort);
		lSin.sin_addr.s_addr = 0L;

		//Bind socket
		if( bind(lSocket, (struct sockaddr*)&lSin, sizeof(lSin)) < 0 )
		{
			return 0;
		}

		if( listen(lSocket, 5) < 0 )
		{
			return 0;
		}
	}

	return lSocket;
}

bool CMainFrame::Initialize()
{
	WSADATA wsadata;
	if (WSAStartup(WS_VERSION_REQUIRED, &wsadata)) 
	{
		//          GetErrorStr();
		Cleanup();
		return false;
	}
	return true;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnTimer(nIDEvent);
	//AfxMessageBox(_T("Entering OnTimer"));
	lSize = sizeof(lClientAddr);
	bzero((char *)&lClientAddr, sizeof(lClientAddr));
	lSocket = accept(Soc, (struct sockaddr*)&lClientAddr, &lSize);
	if(lSocket >= 0)
	{
		sockaddr_in	lAddr;

		bzero((char *)&lAddr, sizeof(lAddr));
		if( getsockname(lSocket, (struct sockaddr*)&lAddr, &lSize) < 0 )
		{
			//return -1;
		}
		AfxMessageBox(_T("Connection established\n"));
		ServerStartedOn = (double)GetNanoSeconds();

		mDataBuffer.SetupBuffer();
		KillTimer( 1 );
		if( m_timer )
		{
			delete m_timer;
			m_timer = NULL;
		}
	}
	
}

bool CMainFrame::Cleanup()
{
	if (WSACleanup()) 
	{
		//         GetErrorStr();
		WSACleanup();

		return false;
	}
	return true;
}

void CMainFrame::bzero(char *b, size_t length)
{
	memset( b,0,length );
}
nsTime CMainFrame::GetNanoSeconds()
{
	static double dmUnits = 0.0;

	LARGE_INTEGER	t;

	if(QueryPerformanceCounter(&t)) 
	{
		double	count;
		count = (double) t.QuadPart;
		if (!dmUnits) 
		{
			QueryPerformanceFrequency(&t);
			dmUnits = 1000000000.0 / (double) t.QuadPart;
		}
		return (unsigned __int64) (count * dmUnits);
	}
	return 0;
}








void CMainFrame::UpdateBuffer(sDataBuffer& mDataBuffer)
{
	std::vector<std::vector<float> >* ptmp;
	double dtmp3[4],dtmp4[4];

	for(int  i=0; i<18; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			mDataBuffer.rawdata[i][j] = Quat_Result[i][j];
		}
	}
	
	for (int j = 0; j < 3; j++)
	{
		mDataBuffer.displace[j] = Dis_AResult[j];
	}

	mDataBuffer.mCounter++;
}



void CMainFrame::OnShut()
{
	// TODO: 在此添加命令处理程序代码
	if (IsRunning)
	{
		IsClose = true;
	}
	else
	{
		PostMessage(WM_QUIT,NULL,NULL);
	}
	
}

//BOOL CMainFrame::DestroyWindow()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	if (IsRunning)
//	{
//		IsClose = true;
//	}
//	else
//	{
//		CFrameWnd::DestroyWindow();
//	}
//	return 0;
//}

void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsRunning)
	{
		IsClose = true;
	}
	else
	{
		PostMessage(WM_QUIT,NULL,NULL);
	}
}
