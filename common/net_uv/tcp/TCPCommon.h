#pragma once

#include "../base/Socket.h"
#include "../base/Server.h"
#include "../base/Client.h"
#include "../base/Runnable.h"
#include "../base/uv_func.h"
#include "../base/Mutex.h"
#include "../base/Session.h"
#include "../base/SessionManager.h"
#include "TCPConfig.h"

NS_NET_UV_BEGIN


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��Ϣ��ͷ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��Ϣ���ݱ��
enum TCPMsgTag : unsigned char
{
	MT_HEARTBEAT,	// ������Ϣ
	MT_DEFAULT
};

struct TCPMsgHead
{
	unsigned int len;// ��Ϣ���ȣ����������ṹ��
#if OPEN_UV_THREAD_HEARTBEAT == 1
	TCPMsgTag tag;// ��Ϣ���
#endif
};


NS_NET_UV_END