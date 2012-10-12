// Config.h: interface for the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIG_H__2EE917D0_3B18_4580_BC7D_C675F4866D9A__INCLUDED_)
#define AFX_CONFIG_H__2EE917D0_3B18_4580_BC7D_C675F4866D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Define.h"

struct userInfo 
{
//	int id;
	CString user;
	CString pass;
};

class CConfig  
{
public:
	CConfig();
	virtual ~CConfig();
	void	SaveConfig();
	void	LoadConfig();
	void	LoadDefaultConfig();
	void	GetFullPathToFile(char  *pszFullPath,char * pszFilename);

public:
	bool	m_bAutorun;		//�Զ�����
	bool	m_bAutologon;		//�Զ���¼
	bool	m_bRememberPWD;	//��������
	bool	m_bShowBubble;		//��ʾ����

	int		m_iTimeout;		//��ʱʱ��

	bool m_bWebAuth;//�Ƿ������ҳ��֤
	CString m_csWebAuthUrl;//��ҳ��֤��ַ

	bool m_bWebLogout;//�Ƿ������ҳע��
	CString m_csWebLogoutUrl;//��ҳע����ַ

	bool m_bEnableWebAccount;//�Ƿ�������ҳ��֤�ʺ���Ϣ��������������֤�ʺ�һ��
	CString m_csWebUsername;//��ҳ��֤�û���
	CString m_csWebPassword;//��ҳ��֤����


	CMap<int,int,userInfo,userInfo>   m_UserInfo;	//������е��˺�������Ϣ

	CString m_csLastUser;	//�ϴ����õ��û���

	CString	m_csNetCard;	//�ϴ���ѡ�����������



/*==================�������ò�����δ������================
	bool m_bDHCP;			//�Ƿ�����DHCP

	bool m_bReauth;		//������֤
	int  m_iReauthTime;	//���Դ���
=========================================================*/
};

#endif // !defined(AFX_CONFIG_H__2EE917D0_3B18_4580_BC7D_C675F4866D9A__INCLUDED_)
