

/**************************************************************************************
			The Luzj's Zte Project
			//////////////////////
			Copyleft ? 2009 Luzj
		Author:Luzj		QQ:86829232
		http://blog.csdn.net/luzjqq
		Email: luzjcn@gmail.com
	///////////////////////////////////
����Luzj's Zte��֤�˵�������

1������������漰��������֤�Ĺ��ܵ�ʵ�־���ͨ���ںз�����������δͨ���κβ�����������á�

2������������о�ѧϰ֮�ã�����ʹ�ñ���������˹�˾��ҵ���档

3����������������κ���ҵ�ͷǷ���;�����������Ը���

4��������ڷ���ǰ��ͨ��һ����Ӧ�ò��ԣ�������֤�κ�����¶Ի����޺���
����δ֪��ʹ�û����򲻵���ʹ�öԼ������ɵ��𺦣�������ʹ����ȫ���е���

5.�������Ȩû�У���ӡ����������Э���Ľ�����Ʒ��

6.���������Դ����������Ҫ�޸ı����Դ���Խ��ж��η�����Ҳ���������Դ���롣

�����κβ�����������������ľ��ף����뱾���޹أ��粻ͬ��������벻Ҫʹ�ø������лл������
**************************************************************************************/
// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Luzj_ZTE.h"
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog


CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Control(pDX, IDC_WEBURL, m_url);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_BN_CLICKED(IDC_CHK_WEB_AUTH, OnChkWebAuth)
	ON_BN_CLICKED(IDC_CHK_WEB_LOGOUT, OnChkWebLogout)
	ON_BN_CLICKED(IDC_CHK_ENABLE_WEBACCOUNT, OnChkEnableWebaccount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CheckDlgButton(IDC_CHK_AUTOLOGON,Config.m_bAutologon?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_CHK_AUTORUN,Config.m_bAutorun?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_CHK_BUBBLE,Config.m_bShowBubble?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_CHK_WEB_AUTH,Config.m_bWebAuth?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_CHK_WEB_LOGOUT,Config.m_bWebLogout?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_CHK_ENABLE_WEBACCOUNT,Config.m_bEnableWebAccount?BST_CHECKED:BST_UNCHECKED);


	char szTemp[MAX_STRING];
	sprintf(szTemp,"%d",Config.m_iTimeout);
	GetDlgItem(IDC_TIMEOUT)->SetWindowText(szTemp);

	GetDlgItem(IDC_WEB_AUTH_URL)->SetWindowText(Config.m_csWebAuthUrl);
	GetDlgItem(IDC_WEB_LOGOUT_URL)->SetWindowText(Config.m_csWebLogoutUrl);
	GetDlgItem(IDC_WEB_USERNAME)->SetWindowText(Config.m_csWebUsername);
	GetDlgItem(IDC_WEB_PASSWORD)->SetWindowText(Config.m_csWebPassword);
	
	m_url.SetURL(STR_WebUrl);
	GetDlgItem(IDC_VERSION)->SetWindowText(STR_AppName"       "STR_Version);

	OnChkWebAuth();
	OnChkWebLogout();
	OnChkEnableWebaccount();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::OnOK()
{
	char szTemp[MAX_STRING];
	Config.m_bAutologon = (bool)(IsDlgButtonChecked(IDC_CHK_AUTOLOGON));
	Config.m_bWebAuth = (bool)(IsDlgButtonChecked(IDC_CHK_WEB_AUTH));
	Config.m_bAutorun = (bool)(IsDlgButtonChecked(IDC_CHK_AUTORUN));
	Config.m_bShowBubble = (bool)(IsDlgButtonChecked(IDC_CHK_BUBBLE));
	Config.m_bWebLogout = (bool)(IsDlgButtonChecked(IDC_CHK_WEB_LOGOUT));
	Config.m_bEnableWebAccount = (bool)(IsDlgButtonChecked(IDC_CHK_ENABLE_WEBACCOUNT));


	GetDlgItem(IDC_TIMEOUT)->GetWindowText(szTemp,MAX_STRING);
	Config.m_iTimeout=atoi(szTemp);

	GetDlgItem(IDC_WEB_AUTH_URL)->GetWindowText(szTemp,MAX_STRING);
	Config.m_csWebAuthUrl = szTemp;

	GetDlgItem(IDC_WEB_LOGOUT_URL)->GetWindowText(szTemp,MAX_STRING);
	Config.m_csWebLogoutUrl = szTemp;

	GetDlgItem(IDC_WEB_USERNAME)->GetWindowText(szTemp,MAX_STRING);
	Config.m_csWebUsername = szTemp;

	GetDlgItem(IDC_WEB_PASSWORD)->GetWindowText(szTemp,MAX_STRING);
	Config.m_csWebPassword = szTemp;

	Config.SaveConfig();
	CDialog::OnOK();
}

void CSettingDlg::OnChkWebAuth() 
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHK_WEB_AUTH))
	{
		GetDlgItem(IDC_WEB_AUTH_URL)->EnableWindow(TRUE);
	}else{
		GetDlgItem(IDC_WEB_AUTH_URL)->EnableWindow(FALSE);
	}
}

void CSettingDlg::OnChkWebLogout() 
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHK_WEB_LOGOUT))
	{
		GetDlgItem(IDC_WEB_LOGOUT_URL)->EnableWindow(TRUE);
	}else{
		GetDlgItem(IDC_WEB_LOGOUT_URL)->EnableWindow(FALSE);
	}
}

void CSettingDlg::OnChkEnableWebaccount() 
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHK_ENABLE_WEBACCOUNT))
	{
		GetDlgItem(IDC_WEB_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_WEB_PASSWORD)->EnableWindow(TRUE);
	}else{
		GetDlgItem(IDC_WEB_USERNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_WEB_PASSWORD)->EnableWindow(FALSE);
	}
}
