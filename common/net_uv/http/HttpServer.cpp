#include "HttpServer.h"
#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

NS_NET_UV_BEGIN

HttpServer::HttpServer()
{
	m_svr = (Pure_TCPServer*)fc_malloc(sizeof(Pure_TCPServer));
	new(m_svr) Pure_TCPServer();

	m_svr->setNewConnectCallback(std::bind(&HttpServer::onSvrNewConnectCallback, this, std::placeholders::_1, std::placeholders::_2));
	m_svr->setRecvCallback(std::bind(&HttpServer::onSvrRecvCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_svr->setDisconnectCallback(std::bind(&HttpServer::onSvrDisconnectCallback, this, std::placeholders::_1, std::placeholders::_2));
	m_svr->setCloseCallback([](Server*) {});

	m_svrCallback = [](const HttpRequest&, HttpResponse* resp) 
	{
		resp->setStatusCode(HttpResponse::k404NotFound);
		resp->setStatusMessage("Not Found");
		resp->setCloseConnection(true);
	};
}

HttpServer::~HttpServer()
{
	if (m_svr)
	{
		m_svr->~Pure_TCPServer();
		fc_free(m_svr);
		m_svr = NULL;
	}
}

bool HttpServer::startServer(const char* ip, uint32_t port, bool isIPV6, int32_t maxCount)
{
	return m_svr->startServer(ip, port, isIPV6, maxCount);
}

bool HttpServer::stopServer()
{
	return m_svr->stopServer();
}

void HttpServer::updateFrame()
{
	m_svr->updateFrame();
}

void HttpServer::onSvrNewConnectCallback(Server* svr, Session* session)
{
	HttpContext* context = (HttpContext*)fc_malloc(sizeof(HttpContext));
	new(context) HttpContext();
	m_contextMap[session] = context;
	printf("onSvrNewConnectCallback\n");
}

void HttpServer::onSvrRecvCallback(Server* svr, Session* session, char* data, uint32_t len)
{
	printf("onSvrRecvCallback\n");
	HttpContext* context = m_contextMap[session];


	printf("\n\n\n---------------------\n");
	printf("%s", std::string(data, len).c_str());
	printf("\n\n\n---------------------\n");

	if (!context->parseRequest(data, len))
	{
		printf("no parseRequest\n");
		static const char* badRequest = "HTTP/1.1 400 Bad Request\r\n\r\n";
		session->send((char*)badRequest, sizeof(badRequest));
	}

	if (context->gotAll())
	{
		printf("gotAll\n");
		const auto & req = context->request();

		const string& connection = req.getHeader("Connection");
		bool close = connection == "close" || (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
		
		HttpResponse response;
		response.setCloseConnection(close);
		
		m_svrCallback(req, &response);
		
		std::string res = response.toString();
		printf("%s\n", res.c_str());

		session->send((char*)res.c_str(), res.size());
		if (response.closeConnection())
		{
			session->disconnect();
		}

		context->reset();
	}
	else
	{
		printf("no gotAll\n");
	}
}

void HttpServer::onSvrDisconnectCallback(Server* svr, Session* session)
{
	printf("onSvrDisconnectCallback\n");
	HttpContext* context = m_contextMap[session];
	context->~HttpContext();
	fc_free(context);
	m_contextMap.erase(session);
}

NS_NET_UV_END