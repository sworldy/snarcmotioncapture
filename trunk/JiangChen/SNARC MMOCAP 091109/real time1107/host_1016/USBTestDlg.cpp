// USBTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USBTest.h"
#include "USBTestDlg.h"
#include "SelectDialog.h"
#include "SiUSBXp.h"

#include "QKF.h"
#include "Displacement.h"
#include "Qinit.h"
#include <vector>

#include <fstream>
#include <iostream>
#include <afxmt.h>

using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SENSOR_NUM   16
#define SENSOR_DATA_SIZE 19
#define DOUBLE_DATA_NUM 9
#define PACKAGE_SIZE (SENSOR_NUM*SENSOR_DATA_SIZE)

#define ITEMS 10000
#define BUFFER_SIZE PACKAGE_SIZE*ITEMS
#define SPAN 20
#define PRE_PROCESS 200
#define PI 3.1415926535

//semaphores to control the access to the io_buffer
HANDLE h_readSemaphore;
HANDLE h_writeSemaphore;

//10 sensor named pipes
HANDLE PipeHandle[SENSOR_NUM+1];
//10 sensor data pipe names
char* PipeName[] = {"\\\\.\\Pipe\\sensor0","\\\\.\\Pipe\\sensor1","\\\\.\\Pipe\\sensor2","\\\\.\\Pipe\\sensor3",
"\\\\.\\Pipe\\sensor4","\\\\.\\Pipe\\sensor5","\\\\.\\Pipe\\sensor6","\\\\.\\Pipe\\sensor7",
"\\\\.\\Pipe\\sensor8","\\\\.\\Pipe\\sensor9","\\\\.\\Pipe\\sensor10","\\\\.\\Pipe\\sensor11",
"\\\\.\\Pipe\\sensor12","\\\\.\\Pipe\\sensor13","\\\\.\\Pipe\\sensor14","\\\\.\\Pipe\\sensor15",
"\\\\.\\Pipe\\sensor16"};
//10 Unscented Kalman Filter
QKF* ukf[SENSOR_NUM];

//10 acceleration of gravity
//gsl_vector *gravQuat[10];	
Displacement*	cur_displacemet;
Qinit*			quat_init[SENSOR_NUM];


// USB data buffer
Buffer      io_buffer[BUFFER_SIZE];
Buffer*		pReadIndex;
Buffer*		pWriteIndex;

locale loc = locale::global( locale(""));
ofstream files[17] = {	ofstream(_T("右大腿.txt")),ofstream(_T("右小腿.txt")),ofstream(_T("右脚.txt")),ofstream(_T("下腰.txt")),
								ofstream(_T("中腰.txt")),ofstream(_T("上腰.txt")),ofstream(_T("头.txt")),ofstream(_T("左大腿.txt")),
								ofstream(_T("左小腿.txt")),ofstream(_T("左脚.txt")),ofstream(_T("左上臂.txt")),ofstream(_T("左下臂.txt")),
								ofstream(_T("左手.txt")),ofstream(_T("右上臂.txt")),ofstream(_T("右下臂.txt")),ofstream(_T("右手.txt")),
								ofstream(_T("根节点位移和方向.txt"))
							};

//ofstream files[17] = {	ofstream(_T("1.txt")),ofstream(_T("2.txt")),ofstream(_T("3.txt")),ofstream(_T("4.txt")),
//ofstream(_T("5.txt")),ofstream(_T("6.txt")),ofstream(_T("7.txt")),ofstream(_T("8.txt")),
//ofstream(_T("9.txt")),ofstream(_T("10.txt")),ofstream(_T("11.txt")),ofstream(_T("12.txt")),
//ofstream(_T("13.txt")),ofstream(_T("14.txt")),ofstream(_T("15.txt")),ofstream(_T("16.txt")),
//ofstream(_T("17.txt"))
//};
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSBTestDlg dialog

CUSBTestDlg::CUSBTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUSBTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUSBTestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUSBTestDlg)
	DDX_Text(pDX, IDC_RX_FILE_NAME, m_sRXFileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUSBTestDlg, CDialog)
	//{{AFX_MSG_MAP(CUSBTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Button_Recieve, OnRecieve)
	ON_BN_CLICKED(Button_Browse, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSBTestDlg message handlers

BOOL CUSBTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bReadError = FALSE;
	m_bWriteError = FALSE;
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Custom Initialization follows:
	// Initialize handles
	m_hUSBDevice = INVALID_HANDLE_VALUE;


	if (!GetDeviceList())
	{	
		AfxMessageBox("Error finding USB device. Aborting application",MB_OK|MB_ICONEXCLAMATION);
		OnCancel();
		return TRUE;
	}

	m_nDeviceNum = -1;

	if (!SelectDevice())
		return TRUE;

	// Set read write timeouts
	//SI_SetTimeouts(1000, 1000);
	//SI_SetTimeouts(0, 0);
	// Open device handle
	SI_STATUS status = SI_Open((DWORD)m_nDeviceNum, &m_hUSBDevice);

	if (status != SI_SUCCESS)
	{
		CString sMessage;
		sMessage.Format("Error opening device: %s\n\nApplication is aborting.\nReset hardware and try again.", m_sDeviceName);
		if (AfxMessageBox(sMessage,MB_OK|MB_ICONEXCLAMATION))
		{
			OnCancel();
			return TRUE;
		}
	}
	//memset(&m_IObuffer, 0, sizeof(USB_iobuf));BUFFER_SIZE

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUSBTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CUSBTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUSBTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUSBTestDlg::OnOKExit() 
{
	SI_Close(m_hUSBDevice);
	int i;
	for( i=0; i<SENSOR_NUM; ++i)
		delete ukf[i];
	delete cur_displacemet;
	for( i=0; i<SENSOR_NUM; ++i)
		delete quat_init[i];
	CDialog::OnOK();

}

// Note: No cancel button exists on the dialog, but this function is used
// when dialog is terminated at customer request from message box input.
void CUSBTestDlg::OnCancel() 
{
	SI_Close(m_hUSBDevice); 
	int i;
	for( i=0; i<SENSOR_NUM; ++i)
		delete ukf[i];
	delete cur_displacemet;

	for( i=0; i<SENSOR_NUM; ++i)
		delete quat_init[i];
	CDialog::OnCancel();

}

BOOL CUSBTestDlg::GetDeviceList()
{
	DWORD	dwNumDevices = 0;
	SI_DEVICE_STRING	devStr;

	SI_STATUS status = SI_GetNumDevices(&dwNumDevices);

	if (status == SI_SUCCESS)
	{
		for (DWORD d = 0; d < dwNumDevices; d++)
		{
			status = SI_GetProductString(d, devStr, SI_RETURN_SERIAL_NUMBER);

			if (status == SI_SUCCESS)
			{
				CString str = devStr;
				m_DeviceList.push_back(str);
			}
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CUSBTestDlg::SelectDevice()
{
	// ask for selection
	CSelectDialog dlg;
	dlg.Initialize(&m_DeviceList);
	if (dlg.DoModal() != IDOK)
		return FALSE;
	m_nDeviceNum = dlg.m_deviceIndex;
	m_sDeviceName = dlg.m_deviceList[dlg.m_deviceIndex];

	return TRUE;
}

static void HextoDec(Buffer* buf, double* dest)
{
	int i;
	unsigned char* b = (buf->data);
	unsigned char d = (buf->id);
	short a[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	static double pre_mag[10][3];

	for ( i = 0; i < 18; i+=2) {
		short tmp1 = b[i];
		short tmp2 = b[i+1];
		a[i/2] = ((tmp1 & 0x00FF) << 8)|(tmp2 & 0x00FF);
	}


	for ( i = 0; i < 9; i++) 
	{
		a[i] =(a[i] & 0x3FFF);
		if((a[i] & 0x2000) == 0x2000)        {
			a[i] = ((~a[i]) + 1) & 0x3FFF;
			a[i] = -a[i];
		}
		if (i < 3)
			dest[i] = a[i]*0.00333*9.8;
		else if (i < 6)
			dest[i] = a[i]*0.00087266461;
		else
			dest[i] = a[i]*0.05;   
	}
}

static void Matrix_vector_Multiple(double *a,double *b){

	double temp1=a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
	double temp2=a[3]*b[0]+a[4]*b[1]+a[5]*b[2];
	double temp3=a[6]*b[0]+a[7]*b[1]+a[8]*b[2];
	
	b[0]=temp1;
	b[1]=temp2;
	b[2]=temp3;	

}
static UINT  WriteThreadFUNC(LPVOID pParam)
{
	DWORD dwBytesSucceed = 0;
	DWORD dwBytesReadRequest = PACKAGE_SIZE;
	SI_STATUS status = SI_SUCCESS;
	int i;
	char buffer1[PACKAGE_SIZE];

	//ignore the dirty shit
	for(i=0; i<215; ++i)
	{
		SI_Read( (HANDLE)pParam, buffer1, dwBytesReadRequest, &dwBytesSucceed);
	}
	SI_Read( (HANDLE)pParam, buffer1, 176, &dwBytesSucceed);
	//needs to be re-computed

	while(1)
	{
		WaitForSingleObject(h_writeSemaphore,INFINITE);
		if( pWriteIndex > &io_buffer[BUFFER_SIZE-1] )
			pWriteIndex = &io_buffer[0];
R:		status = SI_Read( (HANDLE)pParam, pWriteIndex, dwBytesReadRequest, &dwBytesSucceed);
		if(0 == dwBytesSucceed)
		{
			Sleep(10);
			goto R;
		}
		pWriteIndex+=SENSOR_NUM;
		ReleaseSemaphore(h_readSemaphore,1,NULL);
	}
	return 0;
}




static UINT  ReadThreadFUNC(LPVOID pParam)
{
	int i,j,k;
	//int count = 0;
	unsigned long BytesWritten;
	double averageGrav[SENSOR_NUM][DOUBLE_DATA_NUM];
	double buf[DOUBLE_DATA_NUM];
	double result[4];
	double axisangle[4];
	/////////////////////////////////////////////////////	
	double Quat_Init[SENSOR_NUM][6]={	{9.62703,0.032634,-0.130536,35.85,18.9,-6.2},{9.65966,-0.358974,0.228438,42.85,24.95,-5.7},
										{-0.097902,-0.16317,-10.0186,-21.6,-4.45,-42.9},{-9.82283,-0.522144,-0.358974,-41,-3.65,-25.2},
										{-9.82283,-0.522144,-0.358974,-41,-3.65,-25.2},{-9.92074,0.065268,0.032634,-41.1,8.7,-22.05},
										{-0.228438,0.032634,-9.82283,18.4,4.55,-40.15},{-9.85547,0.783216,0.587412,-37.05,26.35,6.35},
										{-9.8881,0.293706,-0.065268,-42,17.75,0.75},{0.065268,-0.097902,-9.75757,-19.2,-9.8,-44.25},
										{0.032634,-0.130536,-9.92074,5.4,-26.25,-44.9},{0.48951,-0.32634,-9.72493,1.8,-27.85,-47.5},
										{-0.16317,-0.261072,-10.0839,-4.05,-24.9,-42.45},{-0.391608,0.195804,-9.82283,-9.5,16.8,-40.1},
										{0.293706,-0.130536,-9.85547,-4.75,14.85,-38.25},{-0.097902,-0.130536,-9.95337,-6.15,15.95,-38.35}
										
	};
	double Quat_Init_Result[SENSOR_NUM][9];
	double QuartandDisplace [SENSOR_NUM][7];

	for (int i=0; i<SENSOR_NUM; i++ )    //初始化位移和四元数数组
	{
		for (int j=0; j<7; j++)
		{
			QuartandDisplace[i][j]=0.0;
		}
		QuartandDisplace[i][3]=1.0;
	}

	//////////////////////////////////////////////////////////
	for(i=0; i<SENSOR_NUM; ++i)
		for(j=0; j<DOUBLE_DATA_NUM; ++j)
			averageGrav[i][j] = 0;

	for( i=0; i<PRE_PROCESS;  ++i)
	{
		WaitForSingleObject(h_readSemaphore,INFINITE);
		if( pReadIndex > &io_buffer[BUFFER_SIZE-1] )
			pReadIndex = &io_buffer[0];
		for( j=0; j<SENSOR_NUM; ++j)
		{
			if(pReadIndex->id > SENSOR_NUM)
				AfxMessageBox("shit200");
			if(pReadIndex->id !=0 )
			{	
				HextoDec(pReadIndex, buf);
				for( k=0; k<DOUBLE_DATA_NUM; ++k)
					averageGrav[pReadIndex->id - 1][k]+=buf[k];
			}
			pReadIndex++;
		}
		ReleaseSemaphore(h_writeSemaphore,1,NULL);
	}

	for(i=0; i<SENSOR_NUM; ++i)
		for(j=0; j<DOUBLE_DATA_NUM; ++j)
			averageGrav[i][j]/=PRE_PROCESS;
	
	//create the UKF filters 
	for( i=0; i<SENSOR_NUM; ++i)
		ukf[i] = new QKF(&averageGrav[i][0],&averageGrav[i][6]);

	//create displacement object
	cur_displacemet = new Displacement();

	for( i=0; i<SENSOR_NUM; ++i)
	{
		quat_init[i] = new Qinit(&Quat_Init[i][0],&averageGrav[i][0],&averageGrav[i][6]);
		quat_init[i]->cmptQinit(&Quat_Init_Result[i][0]);
	}

	//for(i=0; i<25000; ++i)
	while(true)
	{
		WaitForSingleObject(h_readSemaphore,INFINITE);
		if( pReadIndex > &io_buffer[BUFFER_SIZE-1] )
			pReadIndex = &io_buffer[0];

		for( j=0; j<SENSOR_NUM; ++j)
		{
			if(pReadIndex->id > SENSOR_NUM)
				AfxMessageBox("shit");
			if(pReadIndex->id != 0 )
			{	
				HextoDec(pReadIndex, buf); 
		        //Matrix_vector_Multiple(&Quat_Init_Result[pReadIndex->id -1][0],&buf[0]);
				//Matrix_vector_Multiple(&Quat_Init_Result[pReadIndex->id -1][0],&buf[6]);
				ukf[ pReadIndex->id-1 ]->QKF_Filtering(buf);
				result[0] = ukf[ pReadIndex->id-1 ]->state_estimate[0];
				result[1] = ukf[ pReadIndex->id-1 ]->state_estimate[1];
				result[2] = ukf[ pReadIndex->id-1 ]->state_estimate[2];
				result[3] = ukf[ pReadIndex->id-1 ]->state_estimate[3];
				//Quaternion_Multiple(&result[0],&Quat_Init_Result[pReadIndex->id -1][0]);
				
				////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////
				/////四元数转换成欧拉角//////////////////////////////////////////////////////////////////
				if( pReadIndex->id==1 || pReadIndex->id==2 || pReadIndex->id==3 ||pReadIndex->id==4||
					pReadIndex->id==8|| pReadIndex->id==9|| pReadIndex->id==10)
				{
					double Quat_x, Quat_y, Quat_z, Quat_w;//输入四元数
					Quat_x = result[0];Quat_y =result[1]; Quat_z = result[2]; Quat_w = result[3];
					double roll,pitch,yaw;//算法1的欧拉角
					//其中Axx来源于论文 一种新的全角度四元数与欧拉角的转换算法
					double A11 = Quat_x*Quat_x - Quat_y*Quat_y - Quat_z*Quat_z + Quat_w*Quat_w;
					double A12 = 2*(Quat_y*Quat_x + Quat_z*Quat_w);
					double A33 = -Quat_x*Quat_x - Quat_y*Quat_y + Quat_z*Quat_z + Quat_w*Quat_w;
					double A23 = 2*(Quat_y*Quat_z + Quat_x*Quat_w);
					double A13 = 2*(Quat_x*Quat_z - Quat_y*Quat_w);
				////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////欧拉角转换算法1/////////////////////////////////////////////////////////
					if(A33 > 0)
						roll = atan( A23/A33 );
					else if( A23 >0)
						roll = PI + atan( A23/A33 );
					else 
						roll = -PI + atan( A23/A33 );

                    pitch =   asin(-A13);

					if(A11 > 0)
						yaw = atan( A12/A11);
					else if (A12 > 0)
						yaw = PI + atan( A12/A11);
					else 
						yaw = -PI + atan( A12/A11);

					
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////弧度转换成角度/////////////////////////////////////////////////////////////////////////  */ 				
					roll = roll * 180 / PI;
					pitch = pitch *180 /PI;
					yaw   = yaw *180 /PI;
                    //if ( abs(roll) < 3 ) roll = 0;
					//if ( abs(pitch) < 3 ) roll = 0;
					//if ( abs(yaw) < 3 ) roll = 0;
                                   
					QuartandDisplace[pReadIndex->id - 1][3] = roll;
					QuartandDisplace[pReadIndex->id - 1][4] = pitch;
					QuartandDisplace[pReadIndex->id - 1][5] = yaw;
					QuartandDisplace[pReadIndex->id - 1][6] = 0.0;
				}
			  
				else
				{
					double normal;
					double scale;
					if (result[3]>1.0)
					{
						normal = sqrt(result[0]*result[0]+result[1]*result[1]+result[2]*result[2]+result[3]*result[3]);
						axisangle[0] = result[0]/normal;
						axisangle[1] = result[1]/normal;
						axisangle[2] = result[2]/normal;
						axisangle[3] = result[3]/normal;

					}
					axisangle[3]  = 2 * acos(result[3]);
					scale = sqrt(1-result[3]*result[3]);
					if (scale > 0.01)
					{
						axisangle[0] = result[0]/scale;
						axisangle[1] = result[1]/scale;
						axisangle[2] = result[2]/scale;

					}
					else
					{
						axisangle[0] = result[0];
						axisangle[1] = result[1];
						axisangle[2] = result[2];
					}
					
					QuartandDisplace[pReadIndex->id - 1][3] = axisangle[3];
					QuartandDisplace[pReadIndex->id - 1][4] = axisangle[0];
					QuartandDisplace[pReadIndex->id - 1][5] = axisangle[1];
					QuartandDisplace[pReadIndex->id - 1][6] = axisangle[2];	
				}

				switch(pReadIndex->id)
				{
				case 4://腰部

					cur_displacemet->cur_quater_wist[0] = result[0];
					cur_displacemet->cur_quater_wist[1] = result[1];
					cur_displacemet->cur_quater_wist[2] = result[2];
					cur_displacemet->cur_quater_wist[3] = result[3];

					cur_displacemet->acc_wist[0] = buf[0];
					cur_displacemet->acc_wist[1] = buf[1];
					cur_displacemet->acc_wist[2] = buf[2];
					cur_displacemet->angular_wist[0] = buf[3];
					cur_displacemet->angular_wist[1] = buf[4];
					cur_displacemet->angular_wist[2] = buf[5];

					break;

				case 1://右大腿

					cur_displacemet->cur_quater_Rthigh[0] = result[0];
					cur_displacemet->cur_quater_Rthigh[1] = result[1];
					cur_displacemet->cur_quater_Rthigh[2] = result[2];
					cur_displacemet->cur_quater_Rthigh[3] = result[3];

					cur_displacemet->acc_Rthigh[0] = buf[0];
					cur_displacemet->acc_Rthigh[1] = buf[1];
					cur_displacemet->acc_Rthigh[2] = buf[2];
					cur_displacemet->angular_Rthigh[0] = buf[3];
					cur_displacemet->angular_Rthigh[1] = buf[4];
					cur_displacemet->angular_Rthigh[2] = buf[5];

					break;

				case 2://右小腿

					cur_displacemet->cur_quater_Rshank[0] = result[0];
					cur_displacemet->cur_quater_Rshank[1] = result[1];
					cur_displacemet->cur_quater_Rshank[2] = result[2];
					cur_displacemet->cur_quater_Rshank[3] = result[3];

					cur_displacemet->acc_Rshank[0] = buf[0];
					cur_displacemet->acc_Rshank[1] = buf[1];
					cur_displacemet->acc_Rshank[2] = buf[2];
					cur_displacemet->angular_Rshank[0] = buf[3];
					cur_displacemet->angular_Rshank[1] = buf[4];
					cur_displacemet->angular_Rshank[2] = buf[5];

					break;	

				case 3://右脚

					cur_displacemet->cur_quater_Rfeet[0] = result[0];
					cur_displacemet->cur_quater_Rfeet[1] = result[1];
					cur_displacemet->cur_quater_Rfeet[2] = result[2];
					cur_displacemet->cur_quater_Lfeet[3] = result[3];

					cur_displacemet->acc_Rfeet[0] = buf[0];
					cur_displacemet->acc_Rfeet[1] = buf[1];
					cur_displacemet->acc_Rfeet[2] = buf[2];
					cur_displacemet->angular_Rfeet[0] = buf[3];
					cur_displacemet->angular_Rfeet[1] = buf[4];
					cur_displacemet->angular_Rfeet[2] = buf[5];

					break;		


				case 8://左大腿

					cur_displacemet->cur_quater_Lthigh[0] = result[0];
					cur_displacemet->cur_quater_Lthigh[1] = result[1];
					cur_displacemet->cur_quater_Lthigh[2] = result[2];
					cur_displacemet->cur_quater_Lthigh[3] = result[3];	

					cur_displacemet->acc_Lthigh[0] = buf[0];
					cur_displacemet->acc_Lthigh[1] = buf[1];
					cur_displacemet->acc_Lthigh[2] = buf[2];
					cur_displacemet->angular_Lthigh[0] = buf[3];
					cur_displacemet->angular_Lthigh[1] = buf[4];
					cur_displacemet->angular_Lthigh[2] = buf[5];					  

					break;

				case 9://左小腿

					cur_displacemet->cur_quater_Lshank[0] = result[0];
					cur_displacemet->cur_quater_Lshank[1] = result[1];
					cur_displacemet->cur_quater_Lshank[2] = result[2];
					cur_displacemet->cur_quater_Lshank[3] = result[3];

					cur_displacemet->acc_Lshank[0] = buf[0];
					cur_displacemet->acc_Lshank[1] = buf[1];
					cur_displacemet->acc_Lshank[2] = buf[2];
					cur_displacemet->angular_Lshank[0] = buf[3];
					cur_displacemet->angular_Lshank[1] = buf[4];
					cur_displacemet->angular_Lshank[2] = buf[5];						  

					break;

				case 10://左脚

					cur_displacemet->cur_quater_Lfeet[0] = result[0];
					cur_displacemet->cur_quater_Lfeet[1] = result[1];
					cur_displacemet->cur_quater_Lfeet[2] = result[2];
					cur_displacemet->cur_quater_Lfeet[3] = result[3];

					cur_displacemet->acc_Lfeet[0] = buf[0];
					cur_displacemet->acc_Lfeet[1] = buf[1];
					cur_displacemet->acc_Lfeet[2] = buf[2];
					cur_displacemet->angular_Lfeet[0] = buf[3];
					cur_displacemet->angular_Lfeet[1] = buf[4];
					cur_displacemet->angular_Lfeet[2] = buf[5];

					break;	

				default:
					break;

				}

				////////////////////////////////////////////////////////////////////////////////////	

			/*for(k=0; k<9;k++)
					files[pReadIndex->id - 1]<<buf[k]<<"\t";
				for(k=0; k<4;k++)
					files[pReadIndex->id - 1]<<result[k]<<"\t";
				files[pReadIndex->id - 1]<<"\n";*/

				if(WriteFile(PipeHandle[ pReadIndex->id-1 ], &QuartandDisplace[pReadIndex->id - 1][3] , sizeof(double)*4 , &BytesWritten, NULL) == 0)
				{
					CString error;
					error.Format("%dWrite pipe %d failed with error %d\n", i,pReadIndex->id - 1 , GetLastError());
					AfxMessageBox(error);
				}	
			}		
			pReadIndex++;
		}	

		///////////////////////////////////////////////////////////////////////////////////////
		cur_displacemet->ComputeDisplacementbyQuaternion();//计算位移
		cur_displacemet->UpdateQuaternion();//保存本次四元数
		QuartandDisplace[16][0] = cur_displacemet->displacement[0];
		QuartandDisplace[16][1] = cur_displacemet->displacement[1];
		QuartandDisplace[16][2] = cur_displacemet->displacement[2];
		files[16]<<QuartandDisplace[16][0]<<"\t"<<QuartandDisplace[16][1]<<"\t"<<QuartandDisplace[16][2]<<"\t"<<"\n";
		if(WriteFile(PipeHandle[ 16 ], &QuartandDisplace[3][3]  , sizeof(double)*4 , &BytesWritten, NULL) == 0)
		{
			CString error;
			error.Format("%dWrite pipe %d failed with error %d\n", i,pReadIndex->id - 1 , GetLastError());
			AfxMessageBox(error);
		}
		
		if(WriteFile(PipeHandle[ 16 ], &(QuartandDisplace[16]) , sizeof(double)*3 , &BytesWritten, NULL) == 0)
		{
			CString error;
			error.Format("%dWrite pipe %d failed with error %d\n", i,pReadIndex->id - 1 , GetLastError());
			AfxMessageBox(error);
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		ReleaseSemaphore(h_writeSemaphore,1,NULL);
	}
	AfxMessageBox("Read finished");
	return 0;
}

void CUSBTestDlg::OnRecieve() 
{
	//create named pipes and wait
	int i;
	for( i=0; i<SENSOR_NUM; ++i)
	{
		if ((PipeHandle[i] = CreateNamedPipe(PipeName[i],
			PIPE_ACCESS_OUTBOUND, PIPE_READMODE_MESSAGE | PIPE_TYPE_MESSAGE, 1,
			40960, 0, 10000, NULL)) == INVALID_HANDLE_VALUE)
		{
			CString error;
			error.Format("CreateNamedPipe failed with error %d\n",GetLastError());
			AfxMessageBox(error);
			return;
		}
	}
	if ((PipeHandle[16] = CreateNamedPipe(PipeName[16],
			PIPE_ACCESS_OUTBOUND, PIPE_READMODE_MESSAGE | PIPE_TYPE_MESSAGE, 1,
			40960, 0, 10000, NULL)) == INVALID_HANDLE_VALUE)
		{
			CString error;
			error.Format("CreateNamedPipe failed with error %d\n",GetLastError());
			AfxMessageBox(error);
			return;
		}
	for( i=0; i<SENSOR_NUM; ++i)
	{
		if (ConnectNamedPipe(PipeHandle[i], NULL) == 0)
		{
			CString error;
			error.Format("ConnectNamed Pipe%d failed with error %d\n",i,GetLastError());
			AfxMessageBox(error);
			return;
		}	
	}
	if (ConnectNamedPipe(PipeHandle[16], NULL) == 0)
		{
			CString error;
			error.Format("ConnectNamed Pipe%d failed with error %d\n",i,GetLastError());
			AfxMessageBox(error);
			return;
	}

	h_readSemaphore    = CreateSemaphore(NULL, 0, ITEMS, "Buffer_Read_SEM");
	h_writeSemaphore   = CreateSemaphore(NULL, ITEMS, ITEMS, "Buffer_Write_SEM");
	pReadIndex		   = &io_buffer[0];
	pWriteIndex        = &io_buffer[0];
	AfxBeginThread( WriteThreadFUNC, m_hUSBDevice);
	AfxBeginThread( ReadThreadFUNC, NULL);
}

void CUSBTestDlg::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE);

	if (fileDlg.DoModal() == IDOK)
	{
		m_sRXFileName = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

