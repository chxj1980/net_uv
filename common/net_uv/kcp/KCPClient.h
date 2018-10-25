#pragma once

#include "KCPSocket.h"
#include "KCPSession.h"

NS_NET_UV_BEGIN

// ����Ԥ��socket�ص�
using KCPClientCreatePreSocketCall = std::function<void(KCPClient* client, unsigned int sessionID, unsigned int bindPort)>;

class KCPClient : public Client
{
protected:

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
	};

public:

	KCPClient();
	KCPClient(const KCPClient&) = delete;
	virtual ~KCPClient();

	/// Client
	virtual void connect(const char* ip, unsigned int port, unsigned int sessionId)override;

	virtual void closeClient()override;

	virtual void updateFrame()override;

	virtual void removeSession(unsigned int sessionId)override;

	/// SessionManager
	virtual void send(unsigned int sessionId, char* data, unsigned int len)override;

	virtual void disconnect(unsigned int sessionId)override;

	//��������socket�Ƿ��Զ�����
	void setAutoReconnect(bool isAuto);

	//��������socket�Զ�����ʱ��(��λ��S)
	void setAutoReconnectTime(float time);

	//�Ƿ��Զ�����
	void setAutoReconnectBySessionID(unsigned int sessionID, bool isAuto);

	//�Զ�����ʱ��(��λ��S)
	void setAutoReconnectTimeBySessionID(unsigned int sessionID, float time);

	// ����Ԥ��Socket
	void createPrefabricationSocket(unsigned int sessionID);

	inline void setCreatePreSocketCallback(const KCPClientCreatePreSocketCall& call);

protected:

	/// Runnable
	virtual void run()override;

	/// SessionManager
	virtual void executeOperation()override;

	/// Client
	virtual void onIdleRun()override;

	virtual void onSessionUpdateRun()override;

	/// KCPClient
	void onSocketConnect(Socket* socket, int status);

	void onSessionClose(Session* session);

	void onSessionRecvData(Session* session, char* data, unsigned int len);

	void createNewConnect(void* data);

	void clearData();

	clientSessionData* getClientSessionDataBySessionId(unsigned int sessionId);

	clientSessionData* getClientSessionDataBySession(Session* session);

	void onClientUpdate();

protected:
	uv_timer_t m_clientUpdateTimer;

	bool m_reconnect;		// �Ƿ��Զ���������
	float m_totalTime;		// ��������ʱ��

	// ���лỰ
	std::map<unsigned int, clientSessionData*> m_allSessionMap;


	// Ԥ��socket����
	struct PrefabricationSocket
	{
		KCPSocket* socket;
		unsigned int bindPort;
	};
	std::map<unsigned int, PrefabricationSocket> m_allPrefabricationSocket;

	bool m_isStop;

	KCPClientCreatePreSocketCall m_createPreSocketCall;
protected:

	static void uv_client_update_timer_run(uv_timer_t* handle);
};

void KCPClient::setCreatePreSocketCallback(const KCPClientCreatePreSocketCall& call)
{
	m_createPreSocketCall = std::move(call);
}


NS_NET_UV_END
