#pragma once

#include "../base/Macros.h"

NS_NET_UV_BEGIN

enum P2PMessageType : unsigned int
{
	P2P_MSG_ID_BEGIN,
	
	P2P_MSG_ID_C2S_REGISTER,		// P2PClient���������������ע������
	P2P_MSG_ID_S2C_REGISTER_RESULT,	// P2PClientע����

	P2P_MSG_ID_C2S_LOGOUT,			// P2PClient��������������͵ǳ�����
	P2P_MSG_ID_S2C_LOGOUT_RESULT,	// P2PClient�ǳ����

	P2P_MSG_ID_C2S_GET_USER_LIST,	// P2PClient����������������û��б�
	P2P_MSG_ID_S2C_USER_LIST,		// �����������P2PClient�����û��б�

	P2P_MSG_ID_C2S_WANT_TO_CONNECT, // P2PClient�����������������Ҫ���ӵ�ĳ����������
	P2P_MSG_ID_S2C_WANT_TO_CONNECT_RESULT, //���󷵻�
	P2P_MSG_ID_C2S_CONNECT_SUCCESS, // P2PClient��������������Ѿ����ӳɹ�

	P2P_MSG_ID_S2C_START_BURROW,	// �����������ĳ��P2PClient���Ϳ�ʼ��ָ��
	P2P_MSG_ID_S2C_STOP_BURROW,	// �����������ĳ��P2PClient����ֹͣ��ָ��

	P2P_MSG_ID_C2C_SONME_DATA,		// ������

	P2P_MSG_ID_END
};


struct P2PMessageBase
{
	unsigned int ID;
};

//////////////////////////////////////////////////////////////////////////
#define P2P_IP_MAX_LEN NET_UV_INET6_ADDRSTRLEN
#define P2P_CLIENT_USER_NAME_MAX_LEN 128

// �û���Ϣ
struct P2PClientUserInfo
{
	P2PClientUserInfo()
	{
		memset(szName, P2P_CLIENT_USER_NAME_MAX_LEN, 0);
	}
	// ID
	unsigned int userID;

	// ����
	char szName[P2P_CLIENT_USER_NAME_MAX_LEN];

	// �Ƿ���Ҫ����
	bool hasPassword;								
};


//////////////////////////////////////////////////////////////////////////

struct P2PMessage_C2S_Register
{
	P2PMessage_C2S_Register()
	{
		memset(szName, 0, P2P_CLIENT_USER_NAME_MAX_LEN);
	}
	// P2PClient �����˿�
	unsigned int port;	

	// �û���
	char szName[P2P_CLIENT_USER_NAME_MAX_LEN];

	// �û�ID��0��ʾ�ɷ��������䣬�������ʾ��������
	// ���������ȷ�ϸö���������Ч�򷵻ش�ID��������������·���һ���µ�ID
	unsigned int userID;

	// �������룬���Ϊ0���������Ҫ����
	int password;		 
};

struct P2PMessage_S2C_Register_Result
{
	// 0: ע��ɹ� 1: �����ظ� 2:���ƷǷ�
	int code;

	// �û�ID
	unsigned int userID;
};

struct P2PMessage_C2S_GetUserList
{
	// ����ҳ�±� 0���ʾ��������
	int pageIndex;	
};

struct P2PMessage_S2C_UserList
{
	// ��ǰҳ�±�
	int curPageIndex;	

	// һ���ж���ҳ
	int totalPageCount;	

	// ��ǰҳ���ж��ٸ��û���Ϣ
	int curUserCount;
	//P2PClientUserInfo* userInfoArr;
};

struct P2PMessage_C2S_WantToConnect
{
	// ��Ҫ���ӵ��û�ID
	unsigned int userID;

	// ��������
	int password;
};

struct P2PMessage_C2S_WantToConnectResult
{
	// 0:����ɹ� 1: �û�������  2:�Է�������
	int code;			

	// �Է��û�ID
	unsigned int userID;		

	// �Է������˿�
	unsigned int port;

	// �Է�IP��ַ
	char ip[P2P_IP_MAX_LEN];
};

struct P2PMessage_C2S_ConnectSuccess
{
	// ���ӳɹ����û�ID
	unsigned int userID;			
};

struct P2PMessage_S2C_StartBurrow
{
	// �򶴶˿�
	unsigned int port;

	// ��IP
	char ip[P2P_IP_MAX_LEN];

	// �Ƿ�ΪIPV6
	bool isIPV6;

	// �ỰID
	unsigned int sessionID;
};

struct P2PMessage_S2C_StopBurrow
{
	// �򶴶˿�
	unsigned int port;

	// ��IP
	char ip[P2P_IP_MAX_LEN];
};

NS_NET_UV_END
