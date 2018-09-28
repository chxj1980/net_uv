#pragma once

#include "UDPCommon.h"

NS_NET_UV_BEGIN

//��Ϣ����
enum class UDPThreadMsgType
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


struct UDPThreadMsg_S
{
	UDPThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
};

struct UDPThreadMsg_C
{
	UDPThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
};

NS_NET_UV_END
