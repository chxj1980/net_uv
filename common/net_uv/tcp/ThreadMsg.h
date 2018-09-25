#pragma once

#include "TCPCommon.h"

NS_NET_UV_BEGIN

//��Ϣ����
enum ThreadMsgType
{
	START_SERVER_SUC,	//�����������ɹ�
	START_SERVER_FAIL,//����������ʧ��
	CONNECT_FAIL,	//����ʧ��
	CONNECT_ING,	//��������
	CONNECT,		//���ӳɹ�
	NEW_CONNECT,	//������
	DIS_CONNECT,	//�Ͽ�����
	EXIT_LOOP,		//�˳�loop
	RECV_DATA		//�յ���Ϣ
};


struct ThreadMsg_S
{
public:
	ThreadMsgType msgType;
	Session* pSession;//TcpSessionָ��
	char* data;
	unsigned int dataLen;
	TCPMsgTag tag;
};

struct ThreadMsg_C
{
public:
	ThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
	TCPMsgTag tag;
};

NS_NET_UV_END
