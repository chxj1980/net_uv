#pragma once


#include "Macros.h"
#include "Config.h"
#include "md5.h"
#include <string>
#include <map>
#include <list>
#include <queue>
#include <functional>
#include <assert.h>
#include "uv.h"


NS_NET_UV_BEGIN

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if OPEN_NET_UV_DEBUG == 1

#define fc_malloc(len) fc_malloc_s(len, __FILE__, __LINE__)

NET_UV_EXTERN void* fc_malloc_s(unsigned int len, const char* file, int line);
NET_UV_EXTERN void fc_free(void* p);

//��ӡ�ڴ���Ϣ
NET_UV_EXTERN void printMemInfo();

#define NET_UV_LOG(level, format, ...) net_uvLog(level, format, ##__VA_ARGS__)

#else

#define fc_malloc malloc

#define fc_free free

#define printMemInfo() ((void) 0) 

#define NET_UV_LOG(level, format, ...) ((void) 0) 

#endif // !OPEN_NET_UV_DEBUG

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��־���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define CHECK_UV_ERROR(r) if(r) { getUVError(r); return false; }

#define CHECK_UV_ASSERT(r) if(r) { auto str = getUVError(r); NET_UV_LOG(NET_UV_L_ERROR, str.c_str()); assert(0); }

NET_UV_EXTERN std::string getUVError(int errcode);

//��־���
NET_UV_EXTERN void net_uvLog(int level, const char* format, ...);

//������־�������
NET_UV_EXTERN void setNetUVLogPrintFunc(void(*func)(int, const char*));

NS_NET_UV_END