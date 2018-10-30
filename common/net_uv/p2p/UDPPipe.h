#pragma once

#include "P2PCommon.h"
#include "P2PMessage.h"

NS_NET_UV_BEGIN

class UDPPipe : public Runnable
{
public:

	UDPPipe();

	UDPPipe(const UDPPipe&) = delete;

	virtual ~UDPPipe();

	void stop();

	// ������Ϣ
	// ����һֱ���ڶԷ������յ�������Ϣ֮ǰ������ʱ����ͣ�ķ���
	// ÿ����һ��timeoutCount��һ������0ʱ������Ϣ��Ϊ����ʧ��
	// ���ڽ��м������ͣ���ô����Ŀ���Ǿ�����֤��Ϣ���ͳɹ�...
	void sendMsg(P2PMessageID msgID, char* data, uint32_t len, uint32_t toIP, uint32_t toPort, uint32_t timeoutCount = 30);

protected:

	bool startBind(const char* bindIP, uint32_t bindPort);

	void startLoop();

	void startTimer(uint64_t time);

	void stopTimer();

protected:

	virtual void startFailureLogic();

	virtual void on_udp_read(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);

	virtual void on_recv_msg(P2PMessageID msgID, rapidjson::Document& document, uint64_t key, const struct sockaddr* addr) = 0;

	virtual void onIdleRun();

	virtual void onTimerRun();

	virtual void onStop();

	virtual void onSessionRemove(uint64_t sessionID);

	virtual void onNewSession(uint64_t sessionID);


	void setIsClient(bool isClient);

	uint64_t newUniqueID();

	void clearData();

	bool addMsgToRecv(uint64_t sessionID, uint64_t uniqueID, const struct sockaddr* addr);

	// ���ڼ��
	void overdueDetection();

protected:
	/// Runnable
	virtual void run()override;

	static void uv_on_idle_run(uv_idle_t* handle);

	static void uv_on_timer_run(uv_timer_t* handle);

protected:

	UDPSocket* m_socket;
	uv_loop_t m_loop;
	uv_idle_t m_idle;
	uv_timer_t m_timer;

	uint64_t m_curUniqueID;
	uint64_t m_uniqueIDBegin;
	uint64_t m_uniqueIDEnd;

	enum RunState
	{
		RUN,
		WILL_STOP,
		STOP
	};

	RunState m_state;

	struct SessionData
	{
		// �ѽ��յ�������
		// key: ��ϢuniqueID
		// value : ����ʱ��
		std::map<uint64_t, uint64_t> recvDataMap;

		// ��������Ӧ����
		uint8_t noResponseCount;

		// ��ַ
		sockaddr_in send_addr;

		bool recvData(uint64_t uniqueID)
		{
			noResponseCount = 0;

			if (uniqueID == 0)
				return false;

			auto it = recvDataMap.find(uniqueID);
			if (it == recvDataMap.end())
			{
				recvDataMap.insert(std::make_pair(uniqueID, time(NULL) + 300));
				return true;
			}
			return false;
		}
	};
	std::map<uint64_t, SessionData> m_allSessionDataMap;

	Mutex m_sendLock;

	// ���ڼ����
	uint32_t m_overdueDetectionSpace;

	// ���ڷ��͵�����	
	struct SendDataInfo
	{
		P2PMessage* msg;
		uint16_t sendCount;
		sockaddr_in send_addr;
	};
	// key : sessionID(IP�Ͷ˿���϶���)
	// value : 
	std::map<uint64_t, std::map<uint64_t, SendDataInfo> > m_sendMsgMap;
	// ��Ч��������Map�����
	uint16_t m_invalidSendMsgDetectionSpace;
};

NS_NET_UV_END
