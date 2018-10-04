#pragma once

#include "KCPCommon.h"

NS_NET_UV_BEGIN

//��Ϣ����
enum class KCPThreadMsgType
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


struct KCPThreadMsg_S
{
	KCPThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
};

struct KCPThreadMsg_C
{
	KCPThreadMsgType msgType;
	Session* pSession;
	char* data;
	unsigned int dataLen;
};

NS_NET_UV_END
