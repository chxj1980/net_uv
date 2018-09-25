#pragma once

#include "UDPSocket.h"
#include "KcpSession.h"

NS_NET_UV_BEGIN

class UDPClient : Runnable
{
public:
	UDPClient();
	UDPClient(const UDPClient&) = delete;
	virtual ~UDPClient();

	//��ʼ����
	bool connect(const char* ip, int port, unsigned int key);

	void disconnect(unsigned int key);

	//�رտͻ��ˣ����ô˺���֮��ͻ��˽�׼���˳��̣߳���������������ͳͳ�޷�����
	void closeClient();

protected:

	virtual void run()override;

	void idle_run();

	void removeSessionBySocket(UDPSocket* socket);

protected:

	std::map<unsigned int, KcpSession*> m_allSession;
	uv_mutex_t m_sessionMutex;

	uv_loop_t m_loop;
	uv_idle_t m_idle;

protected:

	static void on_udp_socket_call(udp_socket_call_type type, UDPSocket* s, void* userdata);

	static void uv_on_idle_run(uv_idle_t* handle);
};

NS_NET_UV_END
