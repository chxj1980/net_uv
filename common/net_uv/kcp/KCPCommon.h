#pragma once

#include "../base/Common.h"
#include "../base/Socket.h"
#include "../base/Runnable.h"
#include "../base/uv_func.h"
#include "../base/Session.h"
#include "../base/SessionManager.h"
#include "../base/Mutex.h"
#include "../base/Client.h"
#include "../base/Server.h"

NS_NET_UV_BEGIN


// �׽�����С���ջ����С
#define KCP_UV_SOCKET_RECV_BUF_LEN (1024 * 4)
// �׽�����С���ͻ����С
#define KCP_UV_SOCKET_SEND_BUF_LEN (1024 * 4)


NS_NET_UV_END
