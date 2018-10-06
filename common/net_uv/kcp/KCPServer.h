#pragma once


#include "KCPSocket.h"
#include "KCPSession.h"

NS_NET_UV_BEGIN

class NET_UV_EXTERN KCPServer : public Server
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

	struct kcpSessionData
	{
		kcpSessionData()
		{
#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
			curHeartTime = 0;
			curHeartCount = 0;
#endif
			isInvalid = false;
			session = NULL;
		}
		KCPSession* session;
		bool isInvalid;
#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
		int curHeartTime;
		int curHeartCount;
#endif
	};

public:

	KCPServer();
	KCPServer(const KCPServer&) = delete;

	virtual ~KCPServer();

	/// Server
	virtual void startServer(const char* ip, int port, bool isIPV6)override;

	virtual bool stopServer()override;

	virtual void updateFrame()override;

	/// SessionManager
	virtual void send(Session* session, char* data, unsigned int len)override;

	virtual void disconnect(Session* session)override;

	/// KCPServer
	bool isCloseFinish();

protected:

	/// Runnable
	virtual void run()override;

	/// SessionManager
	virtual void executeOperation()override;

	/// KCPServer
	void onNewConnect(Socket* socket);

	void onServerSocketClose(Socket* svr);

	bool onServerSocketConnectFilter(const struct sockaddr* addr);

	void onServerIdleRun();

	void onSessionRecvData(Session* session, char* data, unsigned int len, NetMsgTag tag);
	
protected:

	void pushThreadMsg(NetThreadMsgType type, Session* session, char* data = NULL, unsigned int len = 0, const NetMsgTag& tag = NetMsgTag::MT_DEFAULT);

	void addNewSession(KCPSession* session);

	void onSessionClose(Session* session);

	void clearData();

#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
	void heartRun();
#endif

protected:
	bool m_start;

	std::string m_ip;
	int m_port;
	bool m_isIPV6;

	KCPSocket* m_server;

	uv_idle_t m_idle;
	uv_loop_t m_loop;

	// �����������׶�
	ServerStage m_serverStage;

#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
	uv_timer_t* m_heartTimer;
#endif

	// �Ự����
	std::map<unsigned int, kcpSessionData> m_allSession;

	unsigned int m_sessionID;
protected:

	static void uv_on_idle_run(uv_idle_t* handle);

#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
	static void uv_heart_timer_callback(uv_timer_t* handle);
#endif
};



NS_NET_UV_END
