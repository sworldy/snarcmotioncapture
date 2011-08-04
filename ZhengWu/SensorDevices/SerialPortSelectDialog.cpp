// SerialPortSelectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Cal3DViewer.h"
#include "SerialPortSelectDialog.h"

#define MaxSerialPortNum 70

// SerialPortSelectDialog dialog

IMPLEMENT_DYNAMIC(SerialPortSelectDialog, CDialog)

SerialPortSelectDialog::SerialPortSelectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SerialPortSelectDialog::IDD, pParent)
{

}

SerialPortSelectDialog::~SerialPortSelectDialog()
{
}
BOOL SerialPortSelectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString temp;
	//Set Serial Port numbers
	for(int i=1;i<=MaxSerialPortNum-1;i++)
	{
			temp.Format(_T("COM%d"),i);		
			m_PortNo.AddString((LPCTSTR)temp);
	}
	m_PortNo.SetCurSel(0);
	return TRUE;
	
}
void SerialPortSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_COM, m_strPortNO);
	DDX_Control(pDX, IDC_COMBO_COM, m_PortNo);
}


BEGIN_MESSAGE_MAP(SerialPortSelectDialog, CDialog)
END_MESSAGE_MAP()

