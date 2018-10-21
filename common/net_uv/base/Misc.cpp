#include "Misc.h"
#include <time.h>

NS_NET_UV_BEGIN


std::string net_getUVError(int errcode)
{
	if (0 == errcode)
	{
		return "";
	}
	std::string err;
	auto tmpChar = uv_err_name(errcode);
	if (tmpChar)
	{
		err = tmpChar;
		err += ":";
	}
	else
	{
		char szCode[16];
		sprintf(szCode, "%d:", errcode);
		err = "unknown system errcode ";
		err.append(szCode);
	}
	tmpChar = uv_strerror(errcode);
	if (tmpChar)
	{
		err += tmpChar;
	}
	return std::move(err);
}

std::string net_getTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

void net_alloc_buffer(uv_handle_t* handle, size_t  size, uv_buf_t* buf)
{
	buf->base = (char*)fc_malloc(size);
	buf->len = size;
}

void net_closehandle_defaultcallback(uv_handle_t* handle)
{
	fc_free(handle);
}

void net_closeHandle(uv_handle_t* handle, uv_close_cb closecb)
{
	if (handle && !uv_is_closing(handle))
	{
		uv_close(handle, closecb);
	}
}

// 调整socket缓冲区大小
void net_adjustBuffSize(uv_handle_t* handle, int minRecvBufSize, int minSendBufSize)
{
	int len = 0;
	int r = uv_recv_buffer_size(handle, &len);
	CHECK_UV_ASSERT(r);

	if (len < minRecvBufSize)
	{
		len = minRecvBufSize;
		r = uv_recv_buffer_size(handle, &len);
		CHECK_UV_ASSERT(r);
	}

	len = 0;
	r = uv_send_buffer_size(handle, &len);
	CHECK_UV_ASSERT(r);

	if (len < minSendBufSize)
	{
		len = minSendBufSize;
		r = uv_send_buffer_size(handle, &len);
		CHECK_UV_ASSERT(r);
	}
}

// hash
unsigned int net_getBufHash(const void *buf, unsigned int len)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	unsigned int i = 0;
	char *str = (char *)buf;
	while (i < len)
	{
		hash = hash * seed + (*str++);
		++i;
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int net_getsockAddrIPAndPort(const struct sockaddr* addr, std::string& outIP, unsigned int& outPort)
{
	if (addr == NULL)
		return 0;

	std::string strip;
	unsigned int addrlen = 0;
	unsigned int port = 0;

	if (addr->sa_family == AF_INET6)
	{
		addrlen = sizeof(struct sockaddr_in6);

		const struct sockaddr_in6* addr_in = (const struct sockaddr_in6*) addr;

		char szIp[NET_UV_INET6_ADDRSTRLEN + 1] = { 0 };
		int r = uv_ip6_name(addr_in, szIp, NET_UV_INET6_ADDRSTRLEN);
		if (r != 0)
		{
			return 0;
		}

		strip = szIp;
		port = ntohs(addr_in->sin6_port);
	}
	else
	{
		addrlen = sizeof(struct sockaddr_in);

		const struct sockaddr_in* addr_in = (const struct sockaddr_in*) addr;

		char szIp[NET_UV_INET_ADDRSTRLEN + 1] = { 0 };
		int r = uv_ip4_name(addr_in, szIp, NET_UV_INET_ADDRSTRLEN);
		if (r != 0)
		{
			return 0;
		}

		strip = szIp;
		port = ntohs(addr_in->sin_port);
	}

	outIP = strip;
	outPort = port;

	return addrlen;
}

struct sockaddr* net_getsocketAddr(const char* ip, unsigned int port, unsigned int* outAddrLen)
{
	struct addrinfo hints;
	struct addrinfo* ainfo;
	struct addrinfo* rp;
	struct sockaddr_in* addr4 = NULL;
	struct sockaddr_in6* addr6 = NULL;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;

	int ret = getaddrinfo(ip, NULL, &hints, &ainfo);

	if (ret == 0)
	{
		for (rp = ainfo; rp; rp = rp->ai_next)
		{
			if (rp->ai_family == AF_INET)
			{
				addr4 = (struct sockaddr_in*)rp->ai_addr;
				addr4->sin_port = htons(port);
				if (outAddrLen != NULL)
				{
					*outAddrLen = sizeof(struct sockaddr_in);
				}
				break;

			}
			else if (rp->ai_family == AF_INET6)
			{
				addr6 = (struct sockaddr_in6*)rp->ai_addr;
				addr6->sin6_port = htons(port);
				if (outAddrLen != NULL)
				{
					*outAddrLen = sizeof(struct sockaddr_in6);
				}
				break;
			}
			else
			{
				continue;
			}
		}

		struct sockaddr* addr = NULL;
		if (addr4)
		{
			addr = (struct sockaddr*)fc_malloc(sizeof(struct sockaddr_in));
			memcpy(addr, addr4, sizeof(struct sockaddr_in));
		}
		if (addr6)
		{
			addr = (struct sockaddr*)fc_malloc(sizeof(struct sockaddr_in6));
			memcpy(addr, addr6, sizeof(struct sockaddr_in6));
		}
		return addr;
	}
	return NULL;
}

struct sockaddr* net_getsocketAddr_no(const char* ip, unsigned int port, bool isIPV6, unsigned int* outAddrLen)
{
	if (outAddrLen)
	{
		*outAddrLen = 0;
	}

	if (isIPV6)
	{
		struct sockaddr_in6* send_addr = (struct sockaddr_in6*)fc_malloc(sizeof(struct sockaddr_in6));
		int r = uv_ip6_addr(ip, port, send_addr);

		if (r != 0)
		{
			fc_free(send_addr);
			return NULL;
		}

		if (outAddrLen)
		{
			*outAddrLen = sizeof(struct sockaddr_in6);
		}

		return (struct sockaddr*)send_addr;
	}
	struct sockaddr_in* send_addr = (struct sockaddr_in*)fc_malloc(sizeof(struct sockaddr_in));
	int r = uv_ip4_addr(ip, port, send_addr);

	if (r != 0)
	{
		fc_free(send_addr);
		return NULL;
	}

	if (outAddrLen)
	{
		*outAddrLen = sizeof(struct sockaddr_in6);
	}

	return (struct sockaddr*)send_addr;
}

unsigned int net_getsockAddrPort(const struct sockaddr* addr)
{
	if (addr->sa_family == AF_INET6)
	{
		const struct sockaddr_in6* addr_in = (const struct sockaddr_in6*) addr;
		return ntohs(addr_in->sin6_port);
	}
	const struct sockaddr_in* addr_in = (const struct sockaddr_in*) addr;
	return ntohs(addr_in->sin_port);
}

struct sockaddr* net_tcp_getAddr(const uv_tcp_t* handle)
{

	//有的机子调用uv_tcp_getpeername报错
	//sockaddr_in client_addr;改为 sockaddr_in client_addr[2];
	//https://blog.csdn.net/readyisme/article/details/28249883
	//http://msdn.microsoft.com/en-us/library/ms737524(VS.85).aspx
	//
	//The buffer size for the local and remote address must be 16 bytes more than the size of the sockaddr structure for 
	//the transport protocol in use because the addresses are written in an internal format. For example, the size of a 
	//sockaddr_in (the address structure for TCP/IP) is 16 bytes. Therefore, a buffer size of at least 32 bytes must be 
	//specified for the local and remote addresses.

	int client_addr_length = sizeof(sockaddr_in6) * 2;
	struct sockaddr* client_addr = (struct sockaddr*)fc_malloc(client_addr_length);
	memset(client_addr, 0, client_addr_length);

	int r = uv_tcp_getpeername(handle, client_addr, &client_addr_length);

	if (r != 0)
	{
		fc_free(client_addr);
		return NULL;
	}
	return client_addr;
}

unsigned int net_getAddrPort(const struct sockaddr* addr)
{
	unsigned int outPort = 0;
	if (addr->sa_family == AF_INET6)
	{
		const struct sockaddr_in6* addr_in = (const struct sockaddr_in6*) addr;
		outPort = ntohs(addr_in->sin6_port);
	}
	else
	{
		const struct sockaddr_in* addr_in = (const struct sockaddr_in*) addr;
		outPort = ntohs(addr_in->sin_port);
	}
	return outPort;
}

unsigned int net_udp_getPort(uv_udp_t* handle)
{
	int client_addr_length = sizeof(sockaddr_in6) * 2;
	struct sockaddr* client_addr = (struct sockaddr*)fc_malloc(client_addr_length);
	memset(client_addr, 0, client_addr_length);

	int r = uv_udp_getsockname(handle, client_addr, &client_addr_length);
	CHECK_UV_ASSERT(r);

	unsigned int outPort = 0;

	if (r == 0)
	{
		if (client_addr->sa_family == AF_INET6)
		{
			const struct sockaddr_in6* addr_in = (const struct sockaddr_in6*) client_addr;
			outPort = ntohs(addr_in->sin6_port);
		}
		else
		{
			const struct sockaddr_in* addr_in = (const struct sockaddr_in*) client_addr;
			outPort = ntohs(addr_in->sin_port);
		}
	}
	fc_free(client_addr);

	return outPort;
}

NS_NET_UV_END
