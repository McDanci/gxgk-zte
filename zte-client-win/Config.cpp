
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


// Config.cpp: implementation of the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Luzj_ZTE.h"
#include "Config.h"
#include "aes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConfig Config;





CConfig::CConfig():m_UserInfo(16)
{

}

CConfig::~CConfig()
{

}

//ȡ��·��
void CConfig::GetFullPathToFile(char  *pszFullPath,char * pszFilename)
{
	GetModuleFileName(GetModuleHandle(NULL), pszFullPath, MAX_STRING);
	strcpy(strrchr(pszFullPath, '\\') + 1, pszFilename);
}


void CConfig::SaveConfig()
{
	//�������õ��ļ���ȥ
	char pszFullPath[MAX_STRING];
	char pszFilename[MAX_STRING]=CONFIGNAME;
	char szTemp[MAX_STRING];
	GetFullPathToFile(pszFullPath,pszFilename);
	
	sprintf(szTemp,"%d",m_iTimeout);
	WritePrivateProfileString("config","Timeout",szTemp,pszFullPath);

	//�����Ƿ��Զ���������
	WritePrivateProfileString("config","RememberPWD",m_bRememberPWD==TRUE?"1":"0",pszFullPath);

	//�����Ƿ���ҳ��֤
	WritePrivateProfileString("config","WebAuth",m_bWebAuth==TRUE?"1":"0",pszFullPath);
	
	//�����Ƿ���ҳע��
	WritePrivateProfileString("config","WebLogout",m_bWebLogout==TRUE?"1":"0",pszFullPath);
	
	//�����Ƿ񿪻��Զ�����
	WritePrivateProfileString("config","Autorun",m_bAutorun==TRUE?"1":"0",pszFullPath);
	
	//�����Ƿ��Զ���¼	
	WritePrivateProfileString("config","Autologon",m_bAutologon==TRUE?"1":"0",pszFullPath);
	//������ʾ
	WritePrivateProfileString("config","ShowBubble",m_bShowBubble==TRUE?"1":"0",pszFullPath);
	//������ҳ��֤�ʺ�
	WritePrivateProfileString("config","EnableWebAccount",m_bEnableWebAccount==TRUE?"1":"0",pszFullPath);
	//����������֤ʱ��
	WritePrivateProfileString("config","Reauth",m_bReauth ==TRUE?"1":"0",pszFullPath);
	//�����Զ�����
	WritePrivateProfileString("config","AutoUpdate",m_bAutoUpdate==TRUE?"1":"0",pszFullPath);

	HKEY hRun;
	LONG kResult = ::RegOpenKeyEx(	HKEY_CURRENT_USER ,
									"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
									NULL ,KEY_ALL_ACCESS ,&hRun);
	if (m_bAutorun == TRUE)		//���ÿ����Զ�����
	{
		char szTemp[MAX_STRING],pjPath[MAX_STRING];

		DWORD regsz=REG_SZ;
		DWORD iPathLen;

		strcpy(pszFilename,AfxGetApp()->m_pszExeName);
		strcat(pszFilename,".exe");
		GetFullPathToFile(szTemp,pszFilename);
		sprintf(pjPath,"\"%s\"",szTemp);

	
		iPathLen = (strlen(szTemp) +1) *sizeof(char);
		kResult =::RegQueryValueEx(hRun ,"zte",NULL ,&regsz ,(BYTE *)szTemp ,&iPathLen);
		if (kResult!=ERROR_SUCCESS || strcmp(szTemp,pjPath)!=0)
		{
			iPathLen = (strlen(pjPath) +1) *sizeof(char);
			kResult =::RegSetValueEx(hRun ,"zte",NULL ,REG_SZ ,(BYTE *)pjPath ,iPathLen);
		}

	}
	else
	{
		RegDeleteValue(hRun,"zte");
	}
	::RegCloseKey(hRun);


	//������֤ʱ��
	WritePrivateProfileString("config","ReauthTime",m_csReauthTime,pszFullPath);

	//������ѡ������
	WritePrivateProfileString("config","netcard",m_csNetCard,pszFullPath);

	//������ҳ��֤��ַ
	WritePrivateProfileString("config","WebAuthUrl",m_csWebAuthUrl,pszFullPath);

	//������ҳע����ַ
	WritePrivateProfileString("config","WebLogoutUrl",m_csWebLogoutUrl,pszFullPath);
	
	//��ȡ�����˺��������	
	CString user,pass;	
	POSITION p = Config.m_UserInfo.GetStartPosition();
	while(p != NULL) {		
		Config.m_UserInfo.GetNextAssoc(p, user, pass);		
		if(user.GetLength() > 0)
			WritePrivateProfileString("users", user, aes_encrypt((LPCTSTR)pass), pszFullPath);		
	}	

	WritePrivateProfileString("config","LastUser",m_csLastUser,pszFullPath);

	WritePrivateProfileString("config","WebUsername",m_csWebUsername,pszFullPath);
	WritePrivateProfileString("config","WebPassword",aes_encrypt((LPCTSTR)m_csWebPassword),pszFullPath);

}
void CConfig::LoadConfig()
{

	//�������ļ��ж�ȡ����
	char pszFullPath[MAX_STRING];
	char pszFilename[MAX_STRING]=CONFIGNAME;
	GetFullPathToFile(pszFullPath,pszFilename);
	

	int retCode;
	m_iTimeout=GetPrivateProfileInt("config","Timeout",180,pszFullPath);

	//��ȡ�Ƿ��Զ���������,Ĭ��Ϊ��
	retCode=GetPrivateProfileInt("config","RememberPWD",1,pszFullPath);
	m_bRememberPWD=(retCode==1?TRUE:FALSE);

	//��ȡ�Ƿ������ҳ��֤,Ĭ��Ϊ��
	retCode=GetPrivateProfileInt("config","WebAuth",1,pszFullPath);
	m_bWebAuth=(retCode==1?TRUE:FALSE);

	//��ȡ�Ƿ������ҳע��,Ĭ��Ϊ��
	retCode=GetPrivateProfileInt("config","WebLogout",0,pszFullPath);
	m_bWebLogout=(retCode==1?TRUE:FALSE);

	//��ȡ�Ƿ񿪻��Զ�����,Ĭ��Ϊ��
	retCode=GetPrivateProfileInt("config","Autorun",0,pszFullPath);
	m_bAutorun=(retCode==1?TRUE:FALSE);

	//��ȡ�Ƿ��Զ���¼
	retCode=GetPrivateProfileInt("config","Autologon",0,pszFullPath);
	m_bAutologon=(retCode==1?TRUE:FALSE);


	retCode=GetPrivateProfileInt("config","ShowBubble",1,pszFullPath);
	m_bShowBubble=(retCode==1?TRUE:FALSE);

	retCode=GetPrivateProfileInt("config","EnableWebAccount",0,pszFullPath);
	m_bEnableWebAccount=(retCode==1?TRUE:FALSE);
	
	retCode=GetPrivateProfileInt("config","Reauth",0,pszFullPath);
	m_bReauth=(retCode==1?TRUE:FALSE);

	retCode=GetPrivateProfileInt("config","AutoUpdate",0,pszFullPath);
	m_bAutoUpdate=(retCode==1?TRUE:FALSE);



	//��ȡ�����˺��������
	char szTemp[MAX_STRING];
	Config.m_UserInfo.RemoveAll();
	GetPrivateProfileString("users",NULL, NULL, szTemp, MAX_STRING, pszFullPath);
	char user[MAX_STRING], pass[MAX_STRING];	
	char *p = szTemp;
	if(*p == '\0') p++;
	while(*p) {
		strncpy(user,p, MAX_STRING);
		GetPrivateProfileString("users", user, "", pass, MAX_STRING,pszFullPath);
		Config.m_UserInfo[user] = aes_decrypt(pass);
		while(*p++);
	}

	//��ȡ�ϴ���ѡ����������
	GetPrivateProfileString("config","LastUser","",szTemp,MAX_STRING,pszFullPath);
	m_csLastUser=szTemp;

	//��ȡ�ϴ���ѡ����������
	GetPrivateProfileString("config","netcard","",szTemp,MAX_STRING,pszFullPath);
	m_csNetCard=szTemp;

	//��ȡ��ҳ��֤��ַ
	GetPrivateProfileString("config","WebAuthUrl","http://125.88.59.131:10001/login.do?edubas=113.98.13.29",szTemp,MAX_STRING,pszFullPath);
	m_csWebAuthUrl=szTemp;

	//��ȡ��ҳע����ַ
	GetPrivateProfileString("config","WebLogoutUrl","http://125.88.59.131:10001/Logout.do?edubas=113.98.13.29",szTemp,MAX_STRING,pszFullPath);
	m_csWebLogoutUrl=szTemp;

	GetPrivateProfileString("config","WebUsername","",szTemp,MAX_STRING,pszFullPath);
	m_csWebUsername=szTemp;

	GetPrivateProfileString("config","WebPassword","",szTemp,MAX_STRING,pszFullPath);
	m_csWebPassword=aes_decrypt(szTemp);

	GetPrivateProfileString("config","ReauthTime","",szTemp,MAX_STRING,pszFullPath);
	m_csReauthTime=szTemp;

}
void CConfig::LoadDefaultConfig()
{

}
