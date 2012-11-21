
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



// Luzj_ZTEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Luzj_ZTE.h"
#include "Luzj_ZTEDlg.h"
#include "AutoUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLuzj_ZTEDlg dialog
CLuzj_ZTEDlg::CLuzj_ZTEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLuzj_ZTEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLuzj_ZTEDlg)

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//��֤״̬Ϊ δ��֤
	m_bAuth=FALSE;
}

void CLuzj_ZTEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLuzj_ZTEDlg)
	DDX_Control(pDX, IDC_USERNAME, m_ccb_username);
	DDX_Control(pDX, IDC_NETCARD, m_ccbNetCard);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLuzj_ZTEDlg, CDialog)
	//{{AFX_MSG_MAP(CLuzj_ZTEDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_LOGOFF, OnLogoff)	
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(MENU_SHOW, OnTrayShow)
	ON_BN_CLICKED(MENU_EXIT, OnExit)
	ON_MESSAGE(WM_USER_TRAY_NOTIFICATION,OnTrayNotification)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SETTING, OnSetting)
	ON_BN_CLICKED(IDC_LOGSHOW, OnLogshow)
	ON_CBN_SELCHANGE(IDC_USERNAME, OnSelchangeUsername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CLuzj_ZTEDlg::CheckUpdate()
{
	int ret = AutoUpdate();
	if(ret == 0) {
		if(this->m_bAuth) {
			this->OnLogoff();
		}
		this->PostMessage(WM_QUIT);
	//	Sleep(1000);
		TerminateProcess(GetCurrentProcess(), 0);
	} else if(ret == 4) { return 2; }

	return 1;
}

char *CLuzj_ZTEDlg::GetOSVersion()
{
	static char ver[MAX_STRING];
	OSVERSIONINFO os;
	strncpy(ver, "unknown", MAX_STRING);
	ZeroMemory(&os, sizeof(OSVERSIONINFO));
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&os)) {
		_snprintf(ver, MAX_STRING, "%d.%d.%d.%d.%s", 
			os.dwMajorVersion, os.dwMinorVersion, os.dwBuildNumber, 
			os.dwPlatformId, os.szCSDVersion);
	}
	return ver;
}

char *	CLuzj_ZTEDlg::GetGUID(const char *name)
{
	static char GUID[MAX_STRING];

	if(name == NULL) return NULL;
	char *a = strstr(name, "{");
	if(a == NULL) return NULL;
	char *b = strstr(a, "}");
	if(b == NULL) return NULL;

	memcpy(GUID, a, b - a + 1);
	GUID[b - a + 1] = '\0';
	return GUID;
}

char *	CLuzj_ZTEDlg::ToTCPName(const char *GUID)
{
	static char NPFGUID[MAX_STRING] = "\\Device\\Tcpip_";
	strncpy(NPFGUID + 14, GUID, MAX_STRING - 14); 
	
	return NPFGUID;
}

char *	CLuzj_ZTEDlg::ToNPFName(const char *GUID)
{
	static char TCPGUID[MAX_STRING] =  "\\DEVICE\\NPF_";
	strncpy(TCPGUID + 12, GUID, MAX_STRING - 12); 
	return TCPGUID;
}



BOOL CLuzj_ZTEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ȡ���õ��ļ����ö�����ȥ
	Config.LoadConfig();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	SetWindowText(STR_AppName);
	status = INIT;
	m_bAuth = FALSE;
	m_DHCPThread = NULL;

	editLog = (CEdit*)GetDlgItem(IDC_EDIT_LOG);

	//��������ͼ��
	m_tray.Create(this, WM_USER_TRAY_NOTIFICATION,STR_AppName, m_hIcon, 0);

	//�޸��б�ؼ�
	GetWindowRect(&m_rc);
	m_rc.top=m_rc.bottom-5;    //����״̬���ľ�������
	m_StatusBar.Create(WS_CHILD |WS_VISIBLE|CBRS_BOTTOM,m_rc,this,20000);  
	int nParts[3]= {100,250,-1};      //�ָ�ߴ�
	m_StatusBar.SetParts(3, nParts);
	m_StatusBar.SetText("",0,0);
	m_StatusBar.SetText("",1,0);
	m_StatusBar.SetText("",2,0);
	
	//����״̬������ʾ
	SetTimer(1,1000,NULL);
	//��ʼ��ʱ���Ƚ���־������
	OnLogshow();

	Log(I_MSG, "APP version:%s", STR_Version);
	Log(I_INFO, "Winpcap version:%s", pcap_lib_version());
	Log(I_INFO, "OS version:%s", GetOSVersion());

	//////////////////////////////////////////////////////////////////////////	
	int i=0,k=0;
	CString str;
	
	//if(Config.m_bAutoUpdate) CheckUpdate();

	CheckDlgButton(IDC_REMEMBER,Config.m_bRememberPWD?BST_CHECKED:BST_UNCHECKED);
	//////////////////////////////////////////////////////////////////////////
	//�����˺���Ϣ
	CString user,pass;	
	POSITION p = Config.m_UserInfo.GetStartPosition();	
	while(p != NULL) {
		Config.m_UserInfo.GetNextAssoc(p, user, pass);		
		m_ccb_username.AddString(user);		
	}
	k = m_ccb_username.FindStringExact(-1, Config.m_csLastUser);
	if(k < 0) k = 0;
	if(!Config.m_UserInfo.IsEmpty()) {
		m_ccb_username.SetCurSel(k);
		m_ccb_username.GetWindowText(user);
		GetDlgItem(IDC_PWD)->SetWindowText(Config.m_UserInfo[user]);		
	}
	
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//����������Ϣ	
	char errorBuffer[ PCAP_ERRBUF_SIZE ];		//������Ϣ������
	pcap_if_t		* allAdapters;				//�������б�
	if(pcap_findalldevs(&allAdapters, errorBuffer) == -1 || allAdapters == NULL)
	{
		MessageBox("��ȡ������Ϣʧ�ܣ���ȷ���㰲װ��WinPcap!","����",MB_ICONERROR|MB_OK);
		pcap_freealldevs(allAdapters);
		CDialog::OnCancel();
	}

	k = 0; m_csAdapters.RemoveAll();
	pcap_if_t* adapter;//��ʱ���������
	char *szGuid = NULL;	
	Log(I_INFO, "LastNetcard:%s", Config.m_csNetCard);
    for(k = 0, adapter = allAdapters; adapter != NULL; adapter = adapter->next) {
		if(adapter->flags & PCAP_IF_LOOPBACK) continue;	

		szGuid = GetGUID(adapter->name);
		if(Config.m_bAutoFilter && TestAdapter(szGuid) != 0) continue;

		Log(I_INFO, "load netcard:(%d)%s(%s)", k, szGuid, adapter->description);
		m_csAdapters.Add(szGuid); m_ccbNetCard.AddString(adapter->description); 
		
		k++;
    }
	pcap_freealldevs(allAdapters);

	for(k = m_ccbNetCard.GetCount() - 1; k >= 0; k--) {
		if(stricmp(m_csAdapters[k], Config.m_csNetCard) == 0) break;		
	}
	
	if(k >= 0 && m_ccbNetCard.GetCount() > k) {		
		m_ccbNetCard.SetCurSel(k);
		Log(I_INFO, "select netcard:(%d)%s", k, m_csAdapters[k]);
	}
	
	this->Log(I_INFO, "�����������");
	//////////////////////////////////////////////////////////////////////////

	//ʹ�ÿ�ʼ��ť��Ч�����Ͽ���ť��Ч
	UpdateStatus(FALSE);
	
	if (k >= 0 && Config.m_bAutologon == TRUE)
	{
		ShowWindow(SW_HIDE);
		this->OnStart();
	}
	SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLuzj_ZTEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_MINIMIZE || (nID & 0xFFF0) == SC_CLOSE)
	{
		ShowWindow(SW_HIDE);
		SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
		SetBubble("��ʾ",STR_AppName" ���ں�̨�����š���",1);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

char *CLuzj_ZTEDlg::GetAdapterInfo(const char *name)
{
	static char info[MAX_STRING];
	char *adaptername = ToTCPName(name);
	char temp[MAX_STRING];
	
	if(adaptername == NULL) return NULL;

	_snprintf(temp, MAX_STRING, "adapter name:%s\r\n", adaptername); strncpy(info, temp, MAX_STRING);
	
	PIP_ADAPTER_INFO AdapterInfo = NULL;

	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
	DWORD dwStatus;
	AdapterInfo = (PIP_ADAPTER_INFO)malloc(dwBufLen);
	if(AdapterInfo == NULL) return "GetAdapterInfo:malloc1 failed";
	dwStatus = GetAdaptersInfo(AdapterInfo,&dwBufLen);	
	if(dwStatus == ERROR_BUFFER_OVERFLOW) {
		free(AdapterInfo); 
		AdapterInfo = (PIP_ADAPTER_INFO)malloc(dwBufLen);
		if(AdapterInfo == NULL) return "GetAdapterInfo:malloc2 failed";
		dwStatus = GetAdaptersInfo(AdapterInfo,&dwBufLen);			
	}
	if(dwStatus != NO_ERROR) {
		if(AdapterInfo != NULL) free(AdapterInfo);
		return "GetAdapterInfo:GetAdaptersInfo failed";
	}	
	
	if(name != NULL) {
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;		
		while(pAdapterInfo) {
			if (stricmp(name,pAdapterInfo->AdapterName) == 0) {
				_snprintf(temp, MAX_STRING, "Descript:%s\r\n", pAdapterInfo->Description); 
				strncpy(info, temp, MAX_STRING);

				_snprintf(temp, MAX_STRING, "\tDHCP Enabled:%s", pAdapterInfo->DhcpEnabled ? "YES" : "NO" ); 
				strcat(info, temp); strcat(info, "\r\n");

				_snprintf(temp, MAX_STRING, "\tIP:%s", pAdapterInfo->IpAddressList.IpAddress.String); 
				strcat(info, temp); strcat(info, "\r\n");

				_snprintf(temp, MAX_STRING, "\tMASK:%s", pAdapterInfo->IpAddressList.IpMask.String); 
				strcat(info, temp); strcat(info, "\r\n");

				_snprintf(temp, MAX_STRING, "\tGateWay:%s", pAdapterInfo->GatewayList.IpAddress.String); 
				strcat(info, temp); strcat(info, "\r\n");

				if(MIB_IF_TYPE_ETHERNET != pAdapterInfo->Type) {
					_snprintf(temp, MAX_STRING, "\tType:%d [warning]", pAdapterInfo->Type); 
					strcat(info, temp); strcat(info, "\r\n");
				}

				_snprintf(temp, MAX_STRING, "\tDHCP Server:%s", pAdapterInfo->DhcpServer.IpAddress.String); 
				strcat(info, temp);

				break;
			}
			pAdapterInfo = pAdapterInfo->Next;
		};
	} else {
		return "GetAdapterInfo:DescriptionToName failed";
	}
	
	free(AdapterInfo);
	return info;
}

void CLuzj_ZTEDlg::SetBubble(char * title,char * content,int timeout)
{
	if (Config.m_bShowBubble==TRUE)
	{
		m_tray.SetBubble(title,content,timeout);
	}
}

void CLuzj_ZTEDlg::OnTrayShow()
{   
	if (IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
		SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
	} 
	else
	{
		ShowWindow(SW_SHOW);
	}
}


LONG CLuzj_ZTEDlg::OnTrayNotification(WPARAM wparam, LPARAM lparam)
{   
	CMenu	m_menu;
	CPoint	m_point;
	switch (lparam)
	{
	case WM_RBUTTONDOWN:
		m_menu.CreatePopupMenu();
		if (IsWindowVisible())
		{
			m_menu.AppendMenu(MF_ENABLED, MENU_SHOW,_T("���ش���"));
		} 
		else
		{
			m_menu.AppendMenu(MF_ENABLED, MENU_SHOW,_T("��ʾ����"));
		}
		m_menu.AppendMenu(MF_ENABLED, MF_SEPARATOR);
		m_menu.AppendMenu(MF_ENABLED, MENU_EXIT,_T("�� ��"));
		GetCursorPos(&m_point);
		m_menu.TrackPopupMenu(TPM_LEFTALIGN,m_point.x,m_point.y,this);
		break;
	case WM_LBUTTONDOWN:
		if (IsWindowVisible())
		{
			ShowWindow(SW_HIDE);
			SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
		}
		else
		{
			ShowWindow(SW_SHOW);
			BringWindowToTop();
		}	
		break;
	}
	return 0;
}

void CLuzj_ZTEDlg::OnPaint() 
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
HCURSOR CLuzj_ZTEDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLuzj_ZTEDlg::OnStart() 
{
	UpdateStatus(TRUE);

	//////////////////////////////////////////////////////////////////////////
	//		�ȴ�����ò���
	CString strTemp;

	//ȡ���û�������
	GetDlgItem(IDC_USERNAME)->GetWindowText((char*)m_username, sizeof(m_username));
	m_usernameLen=strlen((char*)m_username);
	GetDlgItem(IDC_PWD)->GetWindowText((char*)m_password, sizeof(m_password));
	m_passwordLen=strlen((char*)m_password);

	if (m_usernameLen<1 || m_passwordLen<1)	{
		this->Log(I_ERR, "�û�����������̫��!"); UpdateStatus(FALSE); return;	
	}
	
	Config.m_csNetCard=m_csAdapters[((CComboBox*)GetDlgItem(IDC_NETCARD))->GetCurSel()];
	
	if (IsDlgButtonChecked(IDC_REMEMBER)) {		
		Config.m_bRememberPWD=TRUE;
	} else {		
		Config.m_bRememberPWD=FALSE;
	}

	Config.m_csLastUser=(char*)m_username;
	Config.m_UserInfo[(char*)m_username]=(char*)m_password;

	Config.SaveConfig();
	//////////////////////////////////////////////////////////////////////////
	SetBubble("��ʾ",STR_AppName" ��ʼ������֤����");
		
	char m_errorBuffer[PCAP_ERRBUF_SIZE];
	int retcode = 0;
		
	/////////////////////////////////////////////////////////////////////////
	//Ѱ����ѡ��������MAC	
	if (GetMacIP(Config.m_csNetCard, NULL, m_MacAdd) != 0)	{
		if(m_MacAdd[0] == 0 && m_MacAdd[1] == 0 && m_MacAdd[2] == 0 && 
			m_MacAdd[3] == 0 && m_MacAdd[4] == 0 && m_MacAdd[5] == 0) {
			Log(I_ERR, "GetMacIP:no mac address."); UpdateStatus(FALSE);
			return;
		}
	} else {
		Log(I_INFO, "MAC:%02X-%02X-%02X-%02X-%02X-%02X", m_MacAdd[0], m_MacAdd[1], m_MacAdd[2], 
			m_MacAdd[3], m_MacAdd[4], m_MacAdd[5]);
	}
	//////////////////////////////////////////////////////////////////////////
	// ��ָ��������
	m_adapterHandle=pcap_open_live(ToNPFName(Config.m_csNetCard),65536,1,Config.m_iTimeout,m_errorBuffer);
    if(m_adapterHandle == NULL) {	
		Log(I_INFO, "pcap_open_live:%s", m_errorBuffer); UpdateStatus(FALSE);
		return;
    }

	char	FilterStr[100];		//�������ַ���
	struct bpf_program	mfcode;	

	sprintf(FilterStr, "(ether proto 0x888e) and (ether dst host %02x:%02x:%02x:%02x:%02x:%02x)",
			m_MacAdd[0],m_MacAdd[1],m_MacAdd[2],m_MacAdd[3],m_MacAdd[4],m_MacAdd[5]);

    if((retcode=pcap_compile(m_adapterHandle, &mfcode, FilterStr, 1, 0xff))==-1
		||(retcode=pcap_setfilter(m_adapterHandle, &mfcode))==-1)
    {
		Log(I_INFO, "pcap_compile & pcap_setfilter:%s", pcap_strerror(retcode));
		pcap_close(m_adapterHandle); UpdateStatus(FALSE);
		return;
    }

	status = AUTHING;

	CPacket packet;	
	//////////////////////////////////////////////////////////////////////////
	///��ʼ��֤��
    if(!(retcode=packet.send_packet_start(m_adapterHandle,m_MacAdd))) {   
		Log(I_INFO, "send_packet_start:(%d)%s", retcode, pcap_geterr(m_adapterHandle));	 UpdateStatus(FALSE);	
		return;
    }
	Log(I_INFO, "Client:EAPOL_START...");

	m_AuthThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)eap_thread, this, 0, 0);
	if(m_AuthThread == NULL) Log(I_INFO, "CreateThread:(%d)", GetLastError());
}

DWORD WINAPI CLuzj_ZTEDlg::eap_thread(void *para)
{
	CLuzj_ZTEDlg *Dlg = (CLuzj_ZTEDlg*)para;	

	pcap_loop(Dlg->m_adapterHandle,-1,CLuzj_ZTEDlg::get_packet,(u_char*)Dlg);

	pcap_close(Dlg->m_adapterHandle);
	return 0;
}

DWORD WINAPI CLuzj_ZTEDlg::dhcp_thread(void *para)
{
	CLuzj_ZTEDlg *Dlg = (CLuzj_ZTEDlg*)para;
	
	Dlg->status = DHCPING;

	int retcode;

	Dlg->Log(I_INFO, "dhcp thread started.");

	retcode = Dlg->IpconfigRenew();
	
	if(retcode != 0) Dlg->Log(I_INFO, "Ipconfig/Renew return %d", retcode);
	else Dlg->status = DHCPED;	
	
	if(retcode == 0 && (Config.m_bWebAuth && Config.m_csWebAuthUrl.GetLength() > 0)){
		Dlg->status = HTTPING;
		char *msg; int i;
		for(i = 0; i < 3; i++) {
			if(Dlg->GetMacIP(Config.m_csNetCard, Dlg->m_ip, NULL) == 0) {
				msg = Dlg->HttpAuth(FALSE);
				if(strcmp(msg, "null") == 0) {							
					Dlg->Log(I_MSG, "��ҳ��֤�ɹ�.");	
					Dlg->status = HTTPED;
					if(Config.m_bAutoUpdate) Dlg->CheckUpdate();
					break;
				} else {		
					Dlg->Log(I_INFO, "HttpAuth:%s", msg);			
				}
			}
			Sleep(3000);
		}
		if(i >= 3) {
			Dlg->Log(I_WARN, "��ҳ��֤ʧ�ܣ����ֶ���֤");
		}
	}

	Dlg->Log(I_INFO, "dhcp thread exited.");

	Dlg->m_DHCPThread = NULL;
		
	return 0;
}


DWORD WINAPI CLuzj_ZTEDlg::GetMacIP(const char *adaptername, char *ip, unsigned char *mac)
{

	PIP_ADAPTER_INFO AdapterInfo = NULL;

	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
	DWORD dwStatus;
	AdapterInfo = (PIP_ADAPTER_INFO)malloc(dwBufLen);
	if(AdapterInfo == NULL) return -1;
	dwStatus = GetAdaptersInfo(AdapterInfo,&dwBufLen);	
	if(dwStatus == ERROR_BUFFER_OVERFLOW) {
		free(AdapterInfo); 
		AdapterInfo = (PIP_ADAPTER_INFO)malloc(dwBufLen);
		dwStatus = GetAdaptersInfo(AdapterInfo,&dwBufLen);	
		if(AdapterInfo == NULL) return -1;
	}
	if(dwStatus != NO_ERROR) {
		if(AdapterInfo != NULL) free(AdapterInfo);
		return -2;
	}

		
	if(ip) memset(ip, 0, 16);	
	if(mac) memset(mac, 0, 6);

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;	
	if(adaptername != NULL) {			
		while(pAdapterInfo) {
			if (stricmp(adaptername,pAdapterInfo->AdapterName) == 0) {
				if(mac) memcpy(mac, pAdapterInfo->Address, 6);
				if(ip) strncpy(ip, pAdapterInfo->IpAddressList.IpAddress.String, 16);
				break;
			}
			pAdapterInfo = pAdapterInfo->Next;
		};
	}
	
	free(AdapterInfo);
	if(pAdapterInfo == NULL) return -3;
	
	return 0;
}

#define MAX_DHCP_TIMES	3

DWORD WINAPI CLuzj_ZTEDlg::IpconfigRenew()
{

	int i;
	DWORD ret;
	char  adaptername[MAX_ADAPTER_NAME];
	int count = 0;

	DWORD dwOutBufLen = sizeof(IP_INTERFACE_INFO);
	PIP_INTERFACE_INFO pIfTable = (PIP_INTERFACE_INFO)malloc(dwOutBufLen);
	if(pIfTable == NULL) return -1;
	
	ret = GetInterfaceInfo(pIfTable, &dwOutBufLen);
	if(ret == ERROR_INSUFFICIENT_BUFFER) {
		free(pIfTable); pIfTable = (PIP_INTERFACE_INFO)malloc(dwOutBufLen);
		if(pIfTable == NULL) return -2;
		ret = GetInterfaceInfo(pIfTable, &dwOutBufLen);
	}

	if(ret != NO_ERROR) return -3;

	for(i = 0; i < pIfTable->NumAdapters; i++) {
		wcstombs(adaptername, pIfTable->Adapter[i].Name, MAX_ADAPTER_NAME);
		
		if(stricmp(adaptername, ToTCPName(Config.m_csNetCard)) == 0) {
			while(1) {
				if(count <= MAX_DHCP_TIMES) Log(I_INFO, "fetching IP address by DHCP...");	
				//IpReleaseAddress(&pIfTable->Adapter[i]);
				ret = IpRenewAddress(&pIfTable->Adapter[i]);
				if(ret == NO_ERROR) break;	
				if(count <= MAX_DHCP_TIMES) {
					if(count == MAX_DHCP_TIMES) {
						count++;
						Log(I_INFO, "dhcp keep quiet.");
					} else {
						Log(I_WARN, "fetch IP address failed. (%d)", ret); count++;
					}
				} 
				Sleep(10000);
			}
			
			char *info = GetAdapterInfo((LPCSTR)Config.m_csNetCard);
			if(info == NULL) Log(I_INFO, "GetAdapterInfo: NULL");
			else Log(I_INFO, info);		

			break;
		}	
	}

	free(pIfTable);
	return 0;
}

void CLuzj_ZTEDlg::Log (int level, const char *fmt, ...)
{

	if(level == I_INFO && !Config.m_bDebug) return;

    va_list args;
    char msg[MAX_STRING];
    va_start (args, fmt);
    vsnprintf (msg, MAX_STRING, fmt, args);
    va_end (args);

	time_t t=time(NULL);
	char szTime[MAX_STRING];
	strftime(szTime,MAX_STRING,"%H:%M:%S",localtime(&t));
	strcat(szTime, " "); strcat(szTime, msg); strcat(szTime, "\r\n");

	int nLength = editLog->SendMessage(WM_GETTEXTLENGTH);
    editLog->SetSel(nLength, nLength);
    editLog->ReplaceSel(szTime);
	editLog->SendMessage(WM_VSCROLL,SB_BOTTOM,0);

	if(level == I_WARN) SetBubble("����", szTime);
	if(level == I_ERR) AfxMessageBox(szTime, MB_ICONSTOP, 0);
}

char * CLuzj_ZTEDlg::HttpAuth(BOOL bForce = FALSE)
{
	static char *msg = NULL;
	if((Config.m_bWebAuth && Config.m_csWebAuthUrl.GetLength() > 0) || bForce) {
		if(Config.m_bEnableWebAccount) {
			msg = WebAuth(Config.m_csWebUsername, Config.m_csWebPassword, m_ip, Config.m_csWebAuthUrl);
		} else {
			msg = WebAuth((const char *)m_username, (const char *)m_password, m_ip, Config.m_csWebAuthUrl);
		}
	}
	return msg;
}

void CLuzj_ZTEDlg::UpdateStatus(bool bOnline)
{
	
	const char *m2[] = {"Offline", "Online"};
	m_bAuth = bOnline;
	int i = (bOnline ? 1 : 0);
	
	
	this->Log(I_INFO, "status:%s", m2[i]);

	if(m_bAuth) {
		m_startTime = time(NULL);
		ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_USERNAME)->EnableWindow(!bOnline);
	GetDlgItem(IDC_PWD)->EnableWindow(!bOnline);
	GetDlgItem(IDC_NETCARD)->EnableWindow(!bOnline);
	GetDlgItem(IDC_START)->EnableWindow(!bOnline);

	GetDlgItem(IDC_LOGOFF)->EnableWindow(bOnline);

}

void CLuzj_ZTEDlg::get_packet(u_char *args, const struct pcap_pkthdr *pcaket_header, const u_char *packet)
{
	int retcode;
	static int good = 0;
	CLuzj_ZTEDlg *Dlg = (CLuzj_ZTEDlg*)args;
	/* declare pointers to packet headers */
	CPacket P;
	if(packet[15] == 0x00) { //eap_packet
		if(packet[18] == 0x01) {//request
			if(packet[22] == 0x01) { //identity
				Dlg->Log(I_INFO, "Server:Request Identity...");
				if(!(retcode=P.send_packet_response_Identity(Dlg->m_adapterHandle,
					packet,Dlg->m_MacAdd,Dlg->m_username,Dlg->m_usernameLen)))	{
					Dlg->Log(I_INFO, "send_packet_response_Identity:(%d)%s", retcode, pcap_geterr(Dlg->m_adapterHandle));					
					Dlg->UpdateStatus(FALSE); return;					
				}
				Dlg->Log(I_INFO, "Client:Response Identity:%s", Dlg->m_username);
			} else if(packet[22] == 0x04) {//md5 challenge
				Dlg->Log(I_INFO, "Server:Request MD5 Chanllenge...");
				if(!(retcode=P.send_packet_response_MD5(Dlg->m_adapterHandle,packet,Dlg->m_MacAdd,
					Dlg->m_username,Dlg->m_usernameLen,Dlg->m_password,Dlg->m_passwordLen))) {
					Dlg->Log(I_INFO, "send_packet_response_MD5:(%d)%s", retcode, pcap_geterr(Dlg->m_adapterHandle));
					Dlg->UpdateStatus(FALSE); return;
				}
				Dlg->Log(I_INFO, "Client:Response MD5 Chanllenge");				
			} else {//unknown
				Dlg->Log(I_INFO, "Unknown EAPOL Type:0x%02X", packet[22]);
			}
		} else if(packet[18] == 0x03) {//successful
			Dlg->status = AUTHED;
			SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
			Dlg->UpdateStatus(TRUE);
			Dlg->Log(I_MSG, "��֤�ɹ�.");

			if(Dlg->m_DHCPThread == NULL) 
				Dlg->m_DHCPThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dhcp_thread, Dlg, 0, 0);			
			
		} else if(packet[18] == 0x04) {//fail
			Dlg->UpdateStatus(FALSE);				
			Dlg->Log(I_ERR, "��������֤ʧ��,��鿴��־��Ϣ!");
			Dlg->Log(I_INFO, (char*)packet+24);
			ExitThread(0);
			return;
		} else {//unknown
			Dlg->Log(I_INFO, "Unknown EAP Type:0x%02X", packet[18]);
		}
	} else if(packet[15] == 0x03) {//key		
		if(!(retcode=P.send_packet_key2(Dlg->m_adapterHandle,packet,Dlg->m_MacAdd))) {
			Dlg->Log(I_INFO, "send_packet_key2:(%d)%s", retcode, pcap_geterr(Dlg->m_adapterHandle));
			Dlg->UpdateStatus(FALSE);  return;
		}	
		if(good <= 3) Dlg->Log(I_INFO, good++ == 3 ? "��������." : "send_packet_key2...");	
		if(good >= 3) Dlg->status = ONLINE;

		Dlg->HttpAuth();
	} else {//unknown
		Dlg->Log(I_INFO, "Unknown Type:0x%02X", packet[15]);
	}
   
    return;
}

//========================OnLogoff======================================
void CLuzj_ZTEDlg::OnLogoff() 
{
	if(Config.m_bWebLogout && strlen(Config.m_csWebLogoutUrl) > 0) {
		char *msg = WebLogout(this->m_ip, Config.m_csWebLogoutUrl);
		if(msg == NULL) {
			this->Log(I_MSG, "��ҳ��֤������!");
		} else {
			this->Log(I_INFO, "WebLogout:%s", msg);			
		}
	}

	CPacket packet;
	packet.send_packet_logoff(m_adapterHandle,m_MacAdd);
	pcap_breakloop(m_adapterHandle);
	//pcap_close(m_adapterHandle);

	this->UpdateStatus(FALSE);

	//�ȴ������500ms��û���Զ��˳�����ǿ�ƽ���
	if (::WaitForSingleObject(this->m_AuthThread,500) == WAIT_TIMEOUT)	
	{
		::TerminateThread(this->m_AuthThread ,0);
	}

	if (::WaitForSingleObject(this->m_DHCPThread,500) == WAIT_TIMEOUT)	
	{
		::TerminateThread(this->m_AuthThread ,0);
	}

	Log(I_MSG, "������֤ע��.");
	this->status = OFFLINE;
}
//========================OnLogoff======================================


//////////////////////////////////////////////////////////////////////////
//��Ӧ�˳���ť������Ѿ���֤�����ȷ����˳���֤��
void CLuzj_ZTEDlg::OnExit() 
{
	ShowWindow(SW_HIDE);
	if (m_bAuth) {		
		this->OnLogoff();
	}
	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////


void CLuzj_ZTEDlg::OnTimer(UINT nIDEvent)
{
	static char *csStatus[] = {"��ʼ�����", "��֤��", "��֤�ɹ�", "���ڻ�ȡIP��ַ", "�ɹ���ȡIP��ַ",
		"��ҳ��֤��", "��ҳ��֤�ɹ�", "����", "����"};

	static STATUS lastStatus = INIT;
		
	time_t t =time(NULL);
	char szTime[MAX_STRING];
	strftime(szTime,MAX_STRING,"%H:%M:%S", localtime(&t));
	m_StatusBar.SetText(szTime,0,0);

	if(m_bAuth && Config.m_bReauth && stricmp(szTime, Config.m_csReauthTime) == 0) {
		OnLogoff();
		OnStart();
	}

	if (m_bAuth) {
		t -= m_startTime;
		strftime(szTime,MAX_STRING,"����ʱ��%H:%M:%S",gmtime(&t));
		m_StatusBar.SetText(szTime,1,0);
	} else {
		m_StatusBar.SetText("...",1,0);
	}

	m_StatusBar.SetText(csStatus[status],2,0);

	if(lastStatus != status) {
		lastStatus = status;
		SetBubble("״̬", csStatus[status]);
	}
		
	CDialog::OnTimer(nIDEvent);
}

void CLuzj_ZTEDlg::OnLogshow() 
{
	static bShow=TRUE;
	RECT rc1={0},rc2={0};
	if (bShow)
	{
		GetDlgItem(IDC_SPLIT)->GetWindowRect(&rc1);
		GetWindowRect(&m_rc);
		rc2=m_rc;
		rc2.right=rc1.left-2;
		MoveWindow(&rc2,TRUE);
		bShow=!bShow;
		GetDlgItem(IDC_LOGSHOW)->SetWindowText("��־>>");
	}
	else
	{
		GetWindowRect(&rc1);
		
		m_rc.right+=rc1.left-m_rc.left;
		m_rc.bottom+=rc1.top-m_rc.top;
		m_rc.left=rc1.left;
		m_rc.top=rc1.top;

		MoveWindow(&m_rc,TRUE);
		bShow=!bShow;
		GetDlgItem(IDC_LOGSHOW)->SetWindowText("��־<<");
	}
}

void CLuzj_ZTEDlg::OnSelchangeUsername() 
{
	CString str;
	m_ccb_username.GetLBText(m_ccb_username.GetCurSel(), str);	
	GetDlgItem(IDC_PWD)->SetWindowText(Config.m_UserInfo[str]);	
}

void CLuzj_ZTEDlg::OnSetting() 
{
	CSettingDlg dlg;
	if (dlg.DoModal())
	{
	}
}

int CLuzj_ZTEDlg::TestAdapter(const char *name)
{
	//Ѱ����ѡ��������MAC
	char errbuf[PCAP_ERRBUF_SIZE];   /* error buffer */
	u_char mac[6];	
	if (GetMacIP(name, NULL, mac) != 0) return -1;
	//////////////////////////////////////////////////////////////////////////
	// ��ָ��������
	pcap_t *handle=pcap_open_live(ToNPFName(name),65536,1,Config.m_iTimeout, errbuf);
    if(handle == NULL) {
		Log(I_INFO, "pcap_open_live:%s", errbuf);
		return -2;
	}

	char	FilterStr[100];		//�������ַ���
	struct bpf_program	mfcode;	

	sprintf(FilterStr, "(ether proto 0x888e) and (ether dst host %02x:%02x:%02x:%02x:%02x:%02x)",
			mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

	int retcode;
    if((retcode=pcap_compile(handle, &mfcode, FilterStr, 1, 0xff))==-1
		||(retcode=pcap_setfilter(handle, &mfcode))==-1) {		
		pcap_close(handle); return -3;
    }
	
	CPacket packet;	
	//////////////////////////////////////////////////////////////////////////
	///��ʼ��֤��
    if(!(retcode=packet.send_packet_start(handle,mac))) {
		pcap_close(handle); return -4;
	}
	const u_char *captured;
	struct pcap_pkthdr      *header;
	char m_errorBuffer[ PCAP_ERRBUF_SIZE ];		//������Ϣ������
	if((retcode=pcap_next_ex(handle, &header, &captured))!=1) {
		strncpy(m_errorBuffer, pcap_strerror(retcode), PCAP_ERRBUF_SIZE);
		pcap_close(handle); return -5;
	}
	pcap_close(handle); return 0;
}
