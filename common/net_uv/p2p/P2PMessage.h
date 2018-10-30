#pragma once

#include "P2PCommon.h"

NS_NET_UV_BEGIN

// ��ϢID
enum P2PMessageID
{
	P2P_MSG_ID_BEGIN = 1000,

	P2P_MSG_ID_HEART_SEND,
	P2P_MSG_ID_HEART_RESULT,
	
	P2P_MSG_ID_C2T_CLIENT_LOGIN,			// �ͻ��˵�¼
	P2P_MSG_ID_T2C_CLIENT_LOGIN_RESULT,		// �ͻ��˵�¼���

	P2P_MSG_ID_C2T_GET_LIST,				// �ͻ��������û��б�
	P2P_MSG_ID_T2C_GET_LIST_RESULT,			// �ͻ��������û��б���

	P2P_MSG_ID_C2T_WANT_TO_CONNECT,			// ��Ҫ���ӵ�ĳ���ͻ���

	P2P_MSG_ID_T2C_START_BURROW,			// ��ʼ��ָ��

	P2P_MSG_ID_C2C_HELLO,					// ����Ϣ

	P2P_MSG_RECV_MSG_RESULT__EX,			// �ظ��Ѿ��յ�ĳ����Ϣ�����߶Է���Ҫ�ڽ����ظ�����

	P2P_MSG_ID_END,
};

// ��Ϣ�ṹ
struct P2PMessage
{
	uint32_t msgID;		// ��ϢID
	uint32_t msgLen;	// ��Ϣ����(���������ṹ��)
	uint64_t uniqueID;  // ������ϢΨһID
};

//// ������ַ��Ϣ
//struct P2PIntranet_Address
//{
//	uint32_t addr;		// ��ַ
//	uint32_t mask;		// ����
//};

// ��ַ��Ϣ
union AddrInfo
{
	uint64_t key;		// key=ǰ���ֽ�ΪIP�����ֽ�Ϊ�˿����
	struct
	{
		uint32_t ip;	// IP
		uint32_t port;  // �˿�
	};
};

// ������ַ������
#define P2P_INTERFACE_ADDR_MAX 4

// P2P�ڵ���Ϣ
struct P2PNodeInfo
{
	// ������ַ��Ϣ
	AddrInfo addr;
};

NS_NET_UV_END
