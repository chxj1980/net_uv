#pragma once

#include "TCPCommon.h"

NS_NET_UV_BEGIN

//����
NET_UV_EXTERN char* tcp_uv_encode(const char* data, unsigned int len, unsigned int &outLen);
//����
NET_UV_EXTERN char* tcp_uv_decode(const char* data, unsigned int len, unsigned int &outLen);
// �������
#if TCP_OPEN_UV_THREAD_HEARTBEAT == 1
NET_UV_EXTERN uv_buf_t* tcp_packageData(char* data, unsigned int len, int* bufCount, NET_HEART_TYPE msgTag);
#else
NET_UV_EXTERN uv_buf_t* tcp_packageData(char* data, unsigned int len, int* bufCount);
#endif
// ���������Ϣ
#if TCP_OPEN_UV_THREAD_HEARTBEAT == 1
NET_UV_EXTERN char* tcp_packageHeartMsgData(char msg, unsigned int* outBufSize);
#endif
NS_NET_UV_END