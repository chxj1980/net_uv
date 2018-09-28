#pragma once

#include "UDPCommon.h"
#include "UDPSocket.h"
#include "KcpSession.h"
#include "UDPThreadMsg.h"

NS_NET_UV_BEGIN

class NET_UV_EXTERN UDPServer : public Server
{
	//�����������׶�
	enum class ServerStage
	{
		START,		//������
		RUN,		//������
		WAIT_CLOSE_SERVER_SOCKET,// �ȴ��������׽��ֹر�
		CLEAR,		//����Ự
		WAIT_SESSION_CLOSE,// �ȴ��Ự�ر�
		STOP		//�˳�
	};
	struct tcpSessionData
	{
		tcpSessionData()
		{
			isInvalid = false;
			session = NULL;
		}
		KcpSession* session;
		bool isInvalid;
	};
public:

	UDPServer();

	virtual ~UDPServer();

	///UDPServer
	virtual void startServer(const char* ip, int port, bool isIPV6)override;

	virtual bool stopServer()override;

	virtual void updateFrame()override;

	/// SessionManager
	virtual void send(Session* session, char* data, unsigned int len)override;

	virtual void disconnect(Session* session)override;

protected:
	/// Runnable
	virtual void run()override;

	/// SessionManager
	virtual void executeOperation()override;

	/// UDPServer
	void idleRun();

	void clearData();

	void pushThreadMsg(UDPThreadMsgType type, Session* session, char* data = NULL, unsigned int len = 0U);

	void onServerSocketClose(Socket* svr);

	void onServerSocketRead(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);

protected:

	static void uv_on_idle_run(uv_idle_t* handle);

protected:
	bool m_start;

	std::string m_ip;
	int m_port;
	bool m_isIPV6;

	UDPSocket* m_server;

	// �����������׶�
	ServerStage m_serverStage;

	// �߳���Ϣ
	Mutex m_msgMutex;
	std::queue<UDPThreadMsg_S> m_msgQue;
	std::queue<UDPThreadMsg_S> m_msgDispatchQue;

	// �Ự����
	std::map<unsigned int, tcpSessionData> m_allSession;

	uv_loop_t m_loop;
	uv_idle_t m_idle;
};


NS_NET_UV_END

