// SelectPort.cpp : implementation file
//

#include "stdafx.h"
#include "SensorDevices.h"
#include "SelectPort.h"

#define MaxSerialPortNum 70
// SelectPort dialog

IMPLEMENT_DYNAMIC(SelectPort, CDialog)

SelectPort::SelectPort(CWnd* pParent /*=NULL*/)
	: CDialog(SelectPort::IDD, pParent)
{

}

SelectPort::~SelectPort()
{
}

BOOL SelectPort::OnInitDialog()
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

void SelectPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_COM, m_strPortNO);
	DDX_Control(pDX, IDC_COMBO_COM, m_PortNo);
}


BEGIN_MESSAGE_MAP(SelectPort, CDialog)
	ON_BN_CLICKED(IDOK, &SelectPort::OnBnClickedOk)
END_MESSAGE_MAP()


// SelectPort message handlers

void SelectPort::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
