#include "../TestCommon.h"

void main()
{
	P2PTurn* turn = new P2PTurn();
	turn->start("0.0.0.0", 1234);

	while (true)
	{
		Sleep(1);
	}
}





//auto instance = new KCPClient();
//
//char cmdBuf[1024] = {0};
//char szWriteBuf[1024] = {0};
//bool exitloop = false;
//uint32_t control_key = 0;
//
//void myPrintLog(int32_t level, const char* log)
//{
//	if (level > NET_UV_L_HEART)
//	{
//		printf("[LOG]:%s\n", log);
//	}
//}
//
//void main()
//{
//	REGISTER_EXCEPTION("kcpControl.dmp");
//
//	setNetUVLogPrintFunc(myPrintLog);
//
//	//�����Զ�����
//	instance->setAutoReconnect(true);
//	//��������ʱ��
//	instance->setAutoReconnectTime(3.0f);
//
//	memset(cmdBuf, 0, 1024);
//	memset(szWriteBuf, 0, 1024);
//
//
//	instance->setClientCloseCallback([](Client*) {
//		printf("�ͻ����ѹر�\n");
//		exitloop = true;
//	});
//	instance->setConnectCallback([=](Client*, Session* session, int32_t status) {
//		if (status == 0)
//		{
//			printf("[%d]����ʧ��\n", session->getSessionID());
//		}
//		else if (status == 1)
//		{
//			printf("[%d]���ӳɹ�\n", session->getSessionID());
//		}
//		else if (status == 2)
//		{
//			printf("[%d]���ӳ�ʱ\n", session->getSessionID());
//		}
//	});
//	instance->setDisconnectCallback([](Client*, Session* session) {
//		printf("[%d]�Ͽ�����\n", session->getSessionID());
//	});
//	instance->setRecvCallback([](Client*, Session* session, char* data, uint32_t len)
//	{
//		char* msg = (char*)fc_malloc(len + 1);
//		memcpy(msg, data, len);
//		msg[len] = '\0';
//		printf("[%d]���յ���Ϣ:%s\n", session->getSessionID(), msg);
//		fc_free(msg);
//	});
//
//	instance->connect(KCP_CONNECT_IP, KCP_CONNECT_PORT, control_key);
//
//	while (!exitloop)
//	{
//		if (KEY_DOWN(VK_SPACE))
//		{
//			std::cin >> cmdBuf;
//			if (CMD_STR_STR("send"))
//			{
//				printf("������Ҫ���͵�����:");
//				std::cin >> szWriteBuf;
//				instance->send(control_key, szWriteBuf, strlen(szWriteBuf));
//			}
//			else if (CMD_STR_STR("print"))
//			{
//				printMemInfo();
//			}
//			else if (CMD_STR_STR("close"))
//			{
//				instance->closeClient();
//			}
//		}
//		instance->updateFrame();
//		ThreadSleep(1);
//	}
//	delete instance;
//
//	printMemInfo();
//
//	system("pause");
//}
//
//
