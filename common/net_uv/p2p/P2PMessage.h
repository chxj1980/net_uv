#pragma once

#include "../base/Macros.h"

NS_NET_UV_BEGIN

enum P2PMessageType : unsigned int
{
	P2P_MSG_ID_BEGIN,
	
	P2P_MSG_ID_C2S_REGISTER_SVR,			// ���������������ע�����������
	P2P_MSG_ID_S2C_REGISTER_SVR_RESULT,		// ע����

	P2P_MSG_ID_C2S_UN_REGISTER_SVR,			// ���������������ȡ��ע�����������
	P2P_MSG_ID_S2C_UN_REGISTER_SVR_RESULT,	// ȡ��ע����

	P2P_MSG_ID_C2S_LOGIN,					// ��������������͵�¼����
	P2P_MSG_ID_S2C_LOGIN_RESULT,			// ��¼���

	P2P_MSG_ID_C2S_LOGOUT,					// ��������������͵ǳ�����
	P2P_MSG_ID_S2C_LOGOUT_RESULT,			// �ǳ����

	P2P_MSG_ID_C2S_GET_SVR_LIST,			// ���������������������б�
	P2P_MSG_ID_S2C_SVR_LIST,				// ���ͷ������б�

	P2P_MSG_ID_C2S_WANT_TO_CONNECT,			// �����������������Ҫ���ӵ�ĳ������������
	P2P_MSG_ID_S2C_WANT_TO_CONNECT_RESULT,	// ���󷵻�
	P2P_MSG_ID_C2S_CONNECT_SUCCESS,			// ��������������Ѿ����ӳɹ�

	P2P_MSG_ID_S2C_START_BURROW,			// �����������ĳ�����������Ϳ�ʼ��ָ��
	P2P_MSG_ID_S2C_STOP_BURROW,				// �����������ĳ������������ֹͣ��ָ��

	P2P_MSG_ID_END
};


struct P2PMessageBase
{
	unsigned int ID;
};

//////////////////////////////////////////////////////////////////////////
#define P2P_IP_MAX_LEN NET_UV_INET6_ADDRSTRLEN
#define P2P_CLIENT_USER_NAME_MAX_LEN 32

// �û���Ϣ
struct P2PClientUserInfo
{
	// ID
	unsigned int userID;

	// �Ƿ���Ҫ����
	bool hasPassword;

	// �Ƿ���ͬһ��������
	bool sameLAN;

	// ����
	char szName[P2P_CLIENT_USER_NAME_MAX_LEN];
};

// ����IP��Ϣ
struct P2PMessageInterface_Address
{
	char szIP[P2P_IP_MAX_LEN];		// IP
	char szMask[P2P_IP_MAX_LEN];	// ��������
};

//////////////////////////////////////////////////////////////////////////

struct P2PMessage_C2S_Register
{
	// P2PClient �����˿�
	unsigned int port;	

	// �û�ID��0��ʾ�ɷ��������䣬�������ʾ��������
	// ���������ȷ�ϸö���������Ч�򷵻ش�ID��������������·���һ���µ�ID
	unsigned int userID;

	// �������룬���Ϊ0���������Ҫ����
	int password;

	// ����
	unsigned int token;

	// �û���
	char szName[P2P_CLIENT_USER_NAME_MAX_LEN];

	// ����IP����
	unsigned int intranet_IP_Count;

	// �Լ�����IP
	// P2PMessageInterface_Address[intranet_IP_Count]
};

struct P2PMessage_S2C_Register_Result
{
	// 0: ע��ɹ� 1: �����ظ� 2:���ƷǷ�
	int code;

	// �û�ID
	unsigned int userID;

	// ����
	unsigned int token;
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

	// �˿�
	unsigned int port;

	// ����IP����
	unsigned int intranet_IP_Count;

	// �Լ�����IP
	// P2PMessageInterface_Address[intranet_IP_Count]
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
