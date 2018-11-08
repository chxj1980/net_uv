#pragma once

#include "P2PCommon.h"

NS_NET_UV_BEGIN

// ��ϢID
enum P2PMessageID
{
	P2P_MSG_ID_BEGIN = 1000,

	P2P_MSG_ID_JSON_BEGIN,

	P2P_MSG_ID_PING,
	P2P_MSG_ID_PONG,

	P2P_MSG_ID_CREATE_KCP,					// KCP����
	P2P_MSG_ID_CREATE_KCP_RESULT,			// KCP�������

	P2P_MSG_ID_C2C_DISCONNECT,				// �Ͽ�
	
	P2P_MSG_ID_C2T_CLIENT_LOGIN,			// �ͻ��˵�¼
	P2P_MSG_ID_T2C_CLIENT_LOGIN_RESULT,		// �ͻ��˵�¼���

	P2P_MSG_ID_C2T_WANT_TO_CONNECT,			// ��Ҫ���ӵ�ĳ���ͻ���

	P2P_MSG_ID_T2C_START_BURROW,			// ��ʼ��ָ��

	P2P_MSG_ID_C2C_HELLO,					// ����Ϣ
	
	P2P_MSG_ID_JSON_END,

	P2P_MSG_ID_KCP,							// KCP��Ϣ

	P2P_MSG_ID_END,
};

// ��Ϣ�ṹ
struct P2PMessage
{
	uint32_t msgID;		// ��ϢID
	uint32_t msgLen;	// ��Ϣ����(���������ṹ��)
	uint64_t uniqueID;  // ������ϢΨһID
};

// ��ַ��Ϣ
union AddrInfo
{
	uint64_t key;		// key : ǰ���ֽ�ΪIP,�����ֽ�Ϊ�˿�
	struct
	{
		uint32_t ip;	// IP
		uint32_t port;  // �˿�
	};
};

// P2P�ڵ���Ϣ
struct P2PNodeInfo
{
	// ������ַ��Ϣ
	AddrInfo addr;
};

static const char* P2P_NULL_JSON = "{}";
static uint32_t P2P_NULL_JSON_LEN = 3;

NS_NET_UV_END
