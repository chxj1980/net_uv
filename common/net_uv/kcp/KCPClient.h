#pragma once

#include "KCPSocket.h"
#include "KCPSession.h"

NS_NET_UV_BEGIN

class NET_UV_EXTERN KCPClient : public Client
{
public:
	enum CONNECTSTATE
	{
		CONNECT,		//������
		CONNECTING,		//��������
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

	virtual void send(unsigned int sessionId, char* data, unsigned int len)override;

	virtual void removeSession(unsigned int sessionId)override;

	/// SessionManager
	virtual void send(Session* session, char* data, unsigned int len)override;

	virtual void disconnect(Session* session)override;

	/// KCPClient
	bool isCloseFinish();

	//��������socket�Ƿ��Զ�����
	void setAutoReconnect(bool isAuto);

	//��������socket�Զ�����ʱ��(��λ��S)
	void setAutoReconnectTime(float time);

	//�Ƿ��Զ�����
	void setAutoReconnectBySessionID(unsigned int sessionID, bool isAuto);

	//�Զ�����ʱ��(��λ��S)
	void setAutoReconnectTimeBySessionID(unsigned int sessionID, float time);

protected:

	/// Runnable
	virtual void run()override;

	/// SessionManager
	virtual void executeOperation()override;

	/// KCPClient
	void onSocketConnect(Socket* socket, int status);

	void onSessionClose(Session* session);

	void onSessionRecvData(Session* session, char* data, unsigned int len, NetMsgTag tag);

	void createNewConnect(void* data);

	void pushThreadMsg(NetThreadMsgType type, Session* session, char* data = NULL, unsigned int len = 0U, NetMsgTag tag = NetMsgTag::MT_DEFAULT);

	void clearData();

	struct clientSessionData;
	KCPClient::clientSessionData* getClientSessionDataBySessionId(unsigned int sessionId);
	KCPClient::clientSessionData* getClientSessionDataBySession(Session* session);

#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
	void heartRun();
#endif

protected:
	uv_loop_t m_loop;
	uv_idle_t m_idle;
	uv_timer_t m_timer;

#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
	uv_timer_t m_heartTimer;
#endif

	bool m_reconnect;		// �Ƿ��Զ���������
	float m_totalTime;		// ��������ʱ��
	bool m_enableNoDelay;
	int m_enableKeepAlive;
	int m_keepAliveDelay;

	struct clientSessionData
	{
		clientSessionData() {}
		~clientSessionData() {}
		CONNECTSTATE connectState;
		bool removeTag; // �Ƿ񱻱���Ƴ�
		bool reconnect;	// �Ƿ��������
		float curtime;
		float totaltime;
		std::string ip;
		unsigned int port;
		KCPSession* session;
#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
		int curHeartTime;
		int curHeartCount;
#endif
	};

	// ���лỰ
	std::map<unsigned int, clientSessionData*> m_allSessionMap;

	//�ͻ��������׶�
	enum class clientStage
	{
		START,
		CLEAR_SESSION,//����Ự
		WAIT_EXIT,//�����˳�
		STOP
	};
	clientStage m_clientStage;
	bool m_isStop;
protected:

	static void uv_timer_run(uv_timer_t* handle);
	static void uv_on_idle_run(uv_idle_t* handle);
#if KCP_OPEN_UV_THREAD_HEARTBEAT == 1
	static void uv_heart_timer_callback(uv_timer_t* handle);
#endif
};
NS_NET_UV_END


