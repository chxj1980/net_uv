#include "../TestCommon.h"

bool autosend = true;
bool autoDisconnect = true;
uint32_t keyIndex = 0;
char szWriteBuf[1024] = {0};

// �������
bool cmdResolve(char* cmd, uint32_t key);

TCPClient* client = new TCPClient();

void main()
{
	REGISTER_EXCEPTION("tcpClient.dmp");

	// �����Զ�����
	client->setAutoReconnect(true);

	client->setClientCloseCallback([](Client*) 
	{
		printf("�ͻ����ѹر�\n");
	});

	client->setConnectCallback([=](Client*, Session* session, int32_t status) 
	{
		if (status == 0)
		{
			printf("[%d]����ʧ��\n", session->getSessionID());
		}
		else if(status == 1)
		{
			printf("[%d]���ӳɹ�\n", session->getSessionID());
		}
		else if (status == 2)
		{
			printf("[%d]���ӳ�ʱ\n", session->getSessionID());
		}
	});

	client->setDisconnectCallback([=](Client*, Session* session) {
		printf("[%d]�Ͽ�����\n", session->getSessionID());
		//client->removeSession(session->getSessionID());
	});

	client->setRemoveSessionCallback([](Client*, Session* session) {
		printf("[%d]ɾ������\n", session->getSessionID());
	});

	client->setRecvCallback([](Client*, Session* session, char* data, uint32_t len)
	{
		char* msg = (char*)fc_malloc(len + 1);
		memcpy(msg, data, len);
		msg[len] = '\0';

		if (!cmdResolve(msg, session->getSessionID()))
		{
			if (len < 100)
			{
				sprintf(szWriteBuf, "this is %d send data...", session->getSessionID());
				if (strcmp(szWriteBuf, msg) != 0)
				{
					printf("�յ��Ƿ���Ϣ:[%s]\n", msg);
				}
			}
			else
			{
				printf("[%d]���յ���Ϣ:%d���ֽ�\n", session->getSessionID(), len);
			}
		}
		fc_free(msg);
		if (autoDisconnect && rand() % 100 == 0)
		{
			session->disconnect();
		}
	});

	for (int32_t i = 0; i < 10; ++i)
	{
		client->connect(TCP_CONNECT_IP, TCP_CONNECT_PORT, keyIndex++);
	}

	int32_t curCount = 0;
	while (!client->isCloseFinish())
	{
		client->updateFrame();
		
		//�Զ�����
		if (autosend)
		{
			curCount++;
			if (curCount > 40)
			{
				for (int32_t i = 0; i < keyIndex; ++i)
				{
					sprintf(szWriteBuf, "this is %d send data...", i);
					client->send(i, szWriteBuf, (uint32_t)strlen(szWriteBuf));
				}
				curCount = 0;
			}
		}

		ThreadSleep(1);
	}

	delete client;

	printMemInfo();

	system("pause");
}


#define CMD_STRCMP(v) (strcmp(cmd, v) == 0)

bool cmdResolve(char* cmd, uint32_t key)
{
	if (CMD_STRCMP("print"))
	{
		//��ӡ�ڴ���Ϣ
		printMemInfo();
	}
	else if (CMD_STRCMP("dis"))
	{
		//�Ͽ�����
		for (int32_t i = 0; i < keyIndex; ++i)
		{
			client->disconnect(i);
		}
	}
	else if (CMD_STRCMP("add"))
	{
		//���������
		client->connect(TCP_CONNECT_IP, TCP_CONNECT_PORT, keyIndex++);
	}
	else if (CMD_STRCMP("closea"))
	{
		client->setAutoReconnect(false);
	}
	else if (CMD_STRCMP("opena"))
	{
		client->setAutoReconnect(true);
	}
	else if (CMD_STRCMP("close"))
	{
		client->closeClient();
	}
	else if (CMD_STRCMP("sendb"))
	{
		autosend = true;
	}
	else if (CMD_STRCMP("sende"))
	{
		autosend = false;
	}
	else if (CMD_STRCMP("closeclient"))
	{
		client->closeClient();
	}
	else if (CMD_STRCMP("autodisb"))
	{
		autoDisconnect = true;
	}
	else if (CMD_STRCMP("autodise"))
	{
		autoDisconnect = false;
	}
	else if (CMD_STRCMP("big"))
	{
		int32_t msgLen = TCP_WRITE_MAX_LEN * 100;
		char* szMsg = (char*)fc_malloc(msgLen);
		for (int32_t i = 0; i < msgLen; ++i)
		{
			szMsg[i] = 'A';
		}
		szMsg[msgLen - 1] = '\0';
		client->send(key, szMsg, (uint32_t)strlen(szMsg));
		fc_free(szMsg);
	}
	else
	{
		return false;
	}
	return true;
}
