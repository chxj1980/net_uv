#pragma once

#include "../base/Macros.h"

NS_NET_UV_BEGIN

typedef unsigned int NET_HEART_TYPE;

#define NET_HEARTBEAT_MSG_C2S	  (0)		// �ͻ���->����������̽����Ϣ
#define NET_HEARTBEAT_MSG_S2C	  (1)		// ������->�ͻ�������̽����Ϣ
#define NET_HEARTBEAT_RET_MSG_C2S (2)		// �ͻ���->�����������ظ���Ϣ
#define NET_HEARTBEAT_RET_MSG_S2C (3)		// ������->�ͻ��������ظ���Ϣ
#define NET_HEARTBEAT_MSG_SIZE	  sizeof(NET_HEART_TYPE)// ������Ϣ��С

NS_NET_UV_END
