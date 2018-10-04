#pragma once

#include "../base/Common.h"
#include "../base/Session.h"

NS_NET_UV_BEGIN

//��Ϣ����
enum class NetThreadMsgType
{
	START_SERVER_SUC,	//�����������ɹ�
	START_SERVER_FAIL,	//����������ʧ��
	CONNECT_FAIL,		//����ʧ��
	CONNECT_ING,		//��������
	CONNECT,			//���ӳɹ�
	NEW_CONNECT,		//������
	DIS_CONNECT,		//�Ͽ�����
	EXIT_LOOP,			//�˳�loop
	RECV_DATA,			//�յ���Ϣ
	REMOVE_SESSION,		//�Ƴ��Ự
};


struct NetThreadMsg_S
{
	NetThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
};

struct NetThreadMsg_C
{
	NetThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
};

NS_NET_UV_END
