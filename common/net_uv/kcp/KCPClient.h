#pragma once

#include "KCPSession.h"
#include "KCPThreadMsg.h"

NS_NET_UV_BEGIN

class NET_UV_EXTERN KCPClient : public Client
{
public:
	enum CONNECTSTATE
	{
		CONNECT,		//������
		DISCONNECTING,	//���ڶϿ�
		DISCONNECT,		//�ѶϿ�
	};
public:
	KCPClient();
	KCPClient(const KCPClient&) = delete;
	virtual ~KCPClient();

	/// Client
	virtual void connect(const char* ip, unsigned int port, unsigned int sessionId)override;

	virtual void disconnect(unsigned int sessionId)override;

	virtual void closeClient()override;

	virtual void updateFrame()override;

	virtual void removeSession(unsigned int sessionId)override;

	/// SessionManager
	virtual void send(Session* session, char* data, unsigned int len)override;

	virtual void disconnect(Session* session)override;

	/// KCPClient
	void send(unsigned int sessionId, char* data, unsigned int len);

protected:
	/// Runnable
	virtual void run()override;

	/// SessionManager
	virtual void executeOperation()override;

	/// KCPClient
	void idle_run();

	void clearData();

	void pushThreadMsg(KCPThreadMsgType type, Session* session, char* data = NULL, unsigned int len = 0U);

	struct clientSessionData;
	KCPClient::clientSessionData* getClientSessionDataBySessionId(unsigned int sessionId);

	void createNewConnect(void* data);

	void onSessionClose(Session* session);

	void onSocketRead(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);

protected:
	struct clientSessionData
	{
		clientSessionData() {}
		~clientSessionData() {}
		CONNECTSTATE connectState;
		KCPSession* session;
		bool removeTag;
	};
	std::map<unsigned int, clientSessionData> m_allSessionMap;

	uv_loop_t m_loop;
	uv_idle_t m_idle;
	bool m_isStop;

	//�ͻ��������׶�
	enum class clientStage
	{
		START,
		CLEAR_SESSION,
		WAIT_EXIT,
		STOP
	};
	clientStage m_clientStage;

	//�߳���Ϣ����
	Mutex m_msgMutex;
	std::queue<UDPThreadMsg_C> m_msgQue;
	std::queue<UDPThreadMsg_C> m_msgDispatchQue;

protected:

	static void uv_on_idle_run(uv_idle_t* handle);
};

NS_NET_UV_END
