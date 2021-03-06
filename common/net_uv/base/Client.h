#pragma once

#include "Common.h"
#include "Runnable.h"
#include "SessionManager.h"

NS_NET_UV_BEGIN

class Client;
class Session;

using ClientConnectCall = std::function<void(Client* client, Session* session, int32_t status)>; // 0失败 1成功 2超时
using ClientDisconnectCall = std::function<void(Client* client, Session* session)>;
using ClientRecvCall = std::function<void(Client* client, Session* session, char* data, uint32_t len)>;
using ClientCloseCall = std::function<void(Client* client)>;
using ClientRemoveSessionCall = std::function<void(Client* client, Session* session)>;


enum CONNECTSTATE
{
	CONNECT,		//已连接
	CONNECTING,		//正在连接
	DISCONNECTING,	//正在断开
	DISCONNECT,		//已断开
};

//客户端所处阶段
enum class clientStage
{
	START,
	CLEAR_SESSION,//清理会话
	WAIT_EXIT,//即将退出
	STOP
};

class Client : public Runnable,  public SessionManager
{
public:
	Client();

	virtual ~Client();

	virtual void connect(const char* ip, uint32_t port, uint32_t sessionId) = 0;

	virtual void closeClient() = 0;

	virtual void updateFrame() = 0;

	virtual void removeSession(uint32_t sessionId) = 0;

	virtual bool isCloseFinish();

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

	clientStage m_clientStage;
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
