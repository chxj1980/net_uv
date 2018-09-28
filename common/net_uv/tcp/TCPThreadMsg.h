#pragma once

#include "TCPCommon.h"

NS_NET_UV_BEGIN

//��Ϣ����
enum class TCPThreadMsgType
{
	START_SERVER_SUC,	//�����������ɹ�
	START_SERVER_FAIL,	//����������ʧ��
	CONNECT_FAIL,		//����ʧ��
	CONNECT_ING,		//��������
	CONNECT,			//���ӳɹ�
	NEW_CONNECT,		//������
	DIS_CONNECT,		//�Ͽ�����
	EXIT_LOOP,			//�˳�loop
	RECV_DATA			//�յ���Ϣ
};


struct TCPThreadMsg_S
{
	TCPThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
	TCPMsgTag tag;
};

struct TCPThreadMsg_C
{
	TCPThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
	TCPMsgTag tag;
};

NS_NET_UV_END
