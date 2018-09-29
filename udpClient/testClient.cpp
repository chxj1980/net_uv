#include "net_uv/udp/UDPServer.h"
#include "net_uv/udp/UDPClient.h"
#include "net_uv/udp/KcpSession.h"

NS_NET_UV_OPEN

#if OPEN_NET_UV_DEBUG == 1
#include "DbgHelp.h"
#pragma comment(lib, "DbgHelp.lib")
// ����Unhandled Exception�Ļص�����
LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException);
#endif

//#define CONNECT_IP "127.0.0.1"
#define CONNECT_IP "www.kurumi.xin"
#define CONNECT_PORT 1234

bool autosend = false;
unsigned int keyIndex = 0;
char *szWriteBuf = new char[1024];

bool clientClose = false;


// �������
bool cmdResolve(char* cmd, unsigned int key);

UDPClient* client = new UDPClient();

void main()
{
#if OPEN_NET_UV_DEBUG == 1
	// ע���쳣������
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif

	client->setClientCloseCallback([](Client*)
	{
		clientClose = true;
		printf("�ͻ����ѹر�\n");
	});

	client->setConnectCallback([=](Client*, Session* session, int status)
	{
		if (status == 0)
		{
			printf("[%d]����ʧ��\n", session->getSessionID());
		}
		else if (status == 1)
		{
			printf("[%d]���ӳɹ�\n", session->getSessionID());
			autosend = true;
		}
	});

	client->setDisconnectCallback([=](Client*, Session* session) {
		printf("[%d]�Ͽ�����\n", session->getSessionID());
		//client->removeSession(session->getSessionID());
	});

	client->setRemoveSessionCallback([](Client*, Session* session) {
		printf("[%d]ɾ������\n", session->getSessionID());
	});

	client->setRecvCallback([](Client*, Session* session, char* data, unsigned int len)
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
				NET_UV_LOG(NET_UV_L_INFO, "[%d]���յ���Ϣ:%d���ֽ�", session->getSessionID(), len);
			}
		}
		fc_free(msg);
		if (rand() % 100 == 0)
		{
			session->disconnect();
		}
	});

	for (int i = 0; i < 1; ++i)
	{
		client->connect(CONNECT_IP, CONNECT_PORT, keyIndex++);
	}

	int curCount = 0;
	while (!clientClose)
	{
		client->updateFrame();

		//�Զ�����
		if (autosend)
		{
			autosend = false;
			//curCount++;
			//if (curCount > 40)
			//{
				//for (int i = 0; i < keyIndex; ++i)
				//{
				//	sprintf(szWriteBuf, "this is %d send data...", i);
				//	client->send(i, szWriteBuf, (unsigned int)strlen(szWriteBuf));
				//}
				int msgLen = 1024 * 10;
				char* szMsg = (char*)fc_malloc(msgLen);
				for (int i = 0; i < msgLen; ++i)
				{
					szMsg[i] = 'A';
				}
				szMsg[msgLen - 1] = '\0';
				for (int i = 0; i < keyIndex; ++i)
				{
					client->send(i, szMsg, (unsigned int)strlen(szMsg));
				}
				fc_free(szMsg);
				//curCount = 0;
			//}
		}

		ThreadSleep(1);
	}

	delete client;

	printMemInfo();

	system("pause");
}


#define CMD_STRCMP(v) (strcmp(cmd, v) == 0)

bool cmdResolve(char* cmd, unsigned int key)
{
	if (CMD_STRCMP("print"))
	{
		//��ӡ�ڴ���Ϣ
		printMemInfo();
	}
	else if (CMD_STRCMP("dis"))
	{
		//�Ͽ�����
		for (int i = 0; i < keyIndex; ++i)
		{
			client->disconnect(i);
		}
	}
	else if (CMD_STRCMP("add"))
	{
		//���������
		client->connect(CONNECT_IP, CONNECT_PORT, keyIndex++);
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
	else if (CMD_STRCMP("big"))
	{
		//int msgLen = TCP_WRITE_MAX_LEN * 100;
		//char* szMsg = (char*)fc_malloc(msgLen);
		//for (int i = 0; i < msgLen; ++i)
		//{
		//	szMsg[i] = 'A';
		//}
		//szMsg[msgLen - 1] = '\0';
		//client->send(key, szMsg, (unsigned int)strlen(szMsg));
		//fc_free(szMsg);
	}
	else
	{
		return false;
	}
	return true;
}


#if OPEN_NET_UV_DEBUG == 1
// ����dump�ļ�
void CreateDempFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException)
{
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//dump��Ϣ
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	// д��dump�ļ�����
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
	CreateDempFile(TEXT("udpClient.dmp"), pException);
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif