#pragma once

#include "TCPThreadMsg.h"
#include "TCPSocket.h"
#include "TCPSession.h"

NS_NET_UV_BEGIN

class NET_UV_EXTERN TCPClient : public Client
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

	TCPClient();
	TCPClient(const TCPClient&) = delete;
	virtual ~TCPClient();

	/// Client
	virtual void connect(const char* ip, unsigned int port, unsigned int sessionId)override;

	virtual void disconnect(unsigned int sessionId)override;

	virtual void closeClient()override;
	
	virtual void updateFrame()override;

	virtual void send(unsigned int sessionId, char* data, unsigned int len)override;

	/// SessionManager
	virtual void send(Session* session, char* data, unsigned int len)override;

	virtual void disconnect(Session* session)override;

	/// TCPClient
	//��ѯ�ͻ����Ƿ��Ѿ��ر����
	//�������true������Խ��и�����ڴ��ͷ�
	//������false�ͽ����ڴ��ͷ�ʱ�����ཫ�������߳���ȫ�˳�
	bool isCloseFinish();

	//�Ƿ�����TCP_NODELAY
	bool setSocketNoDelay(bool enable);

	//��������
	bool setSocketKeepAlive(int enable, unsigned int delay);

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

	/// TCPClient
	void onSocketConnect(Socket* socket, int status);

	void onSessionClose(Session* session);

#if OPEN_UV_THREAD_HEARTBEAT == 1
	void onSessionRecvData(TCPSession* session, char* data, unsigned int len, TCPMsgTag tag);
#else
	void onSessionRecvData(TCPSession* session, char* data, unsigned int len);
#endif

	void createNewConnect(void* data);

	void pushThreadMsg(TCPThreadMsgType type, Session* session, char* data = NULL, unsigned int len = 0U, TCPMsgTag tag = TCPMsgTag::MT_DEFAULT);

	void clearData();

	struct clientSessionData;
	TCPClient::clientSessionData* getClientSessionDataBySessionId(unsigned int sessionId);
	TCPClient::clientSessionData* getClientSessionDataBySession(Session* session);

#if OPEN_UV_THREAD_HEARTBEAT == 1
	void heartRun();
#endif

protected:
	uv_loop_t m_loop;
	uv_idle_t m_idle;
	uv_timer_t m_timer;

#if OPEN_UV_THREAD_HEARTBEAT == 1
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
		bool reconnect;
		float curtime;
		float totaltime;
		std::string ip;
		unsigned int port;
		TCPSession* session;
#if OPEN_UV_THREAD_HEARTBEAT == 1
		int curHeartTime;
		int curHeartCount;
#endif
	};

	// ���лỰ
	std::map<unsigned int, clientSessionData*> m_allSessionMap;

	//�߳���Ϣ����
	Mutex m_msgMutex;
	std::queue<TCPThreadMsg_C> m_msgQue;
	std::queue<TCPThreadMsg_C> m_msgDispatchQue;

	//�ͻ��������׶�
	enum class clientStage
	{
		START,
		CLEAR_SOCKET,//��������socket
		WAIT_EXIT,//�����˳�
		STOP
	};
	clientStage m_clientStage;
	bool m_isStop;
protected:

	static void uv_timer_run(uv_timer_t* handle);
	static void uv_on_idle_run(uv_idle_t* handle);
#if OPEN_UV_THREAD_HEARTBEAT == 1
	static void uv_heart_timer_callback(uv_timer_t* handle);
#endif
};
NS_NET_UV_END


