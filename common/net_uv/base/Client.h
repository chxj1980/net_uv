#pragma once

#include "Common.h"
#include "Runnable.h"
#include "SessionManager.h"

NS_NET_UV_BEGIN

class Client;
class Session;

using ClientConnectCall = std::function<void(Client*, Session*, int)>; // 0ʧ�� 1�ɹ� 2��ʱ 3�ỰID�Ѵ��ڣ��������ڵ�IP���˿ڲ�һ��
using ClientDisconnectCall = std::function<void(Client*, Session*)>;
using ClientRecvCall = std::function<void(Client*, Session*, char*, unsigned int)>;
using ClientCloseCall = std::function<void(Client*)>;
using ClientRemoveSessionCall = std::function<void(Client*, Session*)>;

class NET_UV_EXTERN Client : public Runnable, public SessionManager
{
public:
	Client();

	virtual ~Client();

	virtual void connect(const char* ip, unsigned int port, unsigned int sessionId) = 0;

	virtual void disconnect(unsigned int sessionId) = 0;

	virtual void closeClient() = 0;

	virtual void updateFrame() = 0;

	virtual void send(unsigned int sessionId, char* data, unsigned int len) = 0;

	virtual void removeSession(unsigned int sessionId) = 0;

	inline void setConnectCallback(const ClientConnectCall& call);

	inline void setDisconnectCallback(const ClientDisconnectCall& call);

	inline void setRecvCallback(const ClientRecvCall& call);

	inline void setClientCloseCallback(const ClientCloseCall& call);
	
	inline void setRemoveSessionCallback(const ClientRemoveSessionCall& call);

protected:
	ClientConnectCall m_connectCall;
	ClientDisconnectCall m_disconnectCall;
	ClientRecvCall m_recvCall;
	ClientCloseCall m_clientCloseCall;
	ClientRemoveSessionCall m_removeSessionCall;
};

void Client::setConnectCallback(const ClientConnectCall& call)
{
	m_connectCall = std::move(call);
}

void Client::setDisconnectCallback(const ClientDisconnectCall& call)
{
	m_disconnectCall = std::move(call);
}

void Client::setRecvCallback(const ClientRecvCall& call)
{
	m_recvCall = std::move(call);
}

void Client::setClientCloseCallback(const ClientCloseCall& call)
{
	m_clientCloseCall = std::move(call);
}

void Client::setRemoveSessionCallback(const ClientRemoveSessionCall& call)
{
	m_removeSessionCall = std::move(call);
}

NS_NET_UV_END
