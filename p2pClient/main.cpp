#include "net_uv/net_uv.h"
#include <iostream>

NS_NET_UV_OPEN

#if OPEN_NET_UV_DEBUG == 1
#include "DbgHelp.h"
#pragma comment(lib, "DbgHelp.lib")
// ����Unhandled Exception�Ļص�����
LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException);
#endif

bool runLoop = true;

std::vector<P2PClientUserInfo> allUserInfoList;

P2PClient* p2pClient = NULL;

void runCMD();

void main()
{
#if OPEN_NET_UV_DEBUG == 1
	// ע���쳣������
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif

	p2pClient = new P2PClient();

	p2pClient->set_p2p_ConnectResultCallback([=](P2PClient* client, int status, unsigned int userId)
	{
		/// status: 0 ���ӳɹ� 1����ʧ�ܣ��û������� 2����ʧ�ܣ����������������ʧ�� 3����ʧ�ܣ���P2PClient�ı��ط���������ʧ��
		if (status == 0)
		{
			printf("����[%d] ���ӳɹ�\n", userId);
		}
		else if (status == 1)
		{
			printf("����[%d] ����ʧ�ܣ��û�������\n", userId);
		}
		else if (status == 2)
		{
			printf("����[%d] ����ʧ�ܣ����������������ʧ��\n", userId);
		}
		else if (status == 3)
		{
			printf("����[%d] ����ʧ�ܣ���P2PClient�ı��ط���������ʧ��\n", userId);
		}
	});
	p2pClient->set_p2p_RegisterResultCallback([](P2PClient* client, int status) {
		if (status == 0)
		{
			printf("ע�ᵽ����������ɹ�\n");
		}
		else if (status == 1)
		{
			printf("ע�ᵽ���������ʧ�ܣ������ظ�\n");
		}
	});
	p2pClient->set_p2p_UserListResultCallback([](P2PClient* client, P2PMessage_S2C_UserList* listInfo, const std::vector<P2PClientUserInfo>&userList) 
	{
		if (listInfo->curPageIndex == 1)
		{
			allUserInfoList.clear();
		}
		for (auto &it : userList)
		{
			allUserInfoList.push_back(it);
		}
		if (listInfo->curPageIndex == listInfo->totalPageCount)
		{
			printf("\n--------------------------------------\n");
			int index = 0;
			for (auto& it : allUserInfoList)
			{
				index++;
				printf("%02d userID:[%d]\t needPassword:[%s]\t name:[%s]\n", index, it.userID, it.hasPassword ? "true" : "false", it.szName);
			}
			printf("--------------------------------------\n");
		}
	});


	p2pClient->set_client_ConnectCallback([](Client* client, Session* session, int status) 
	{
		if (status == 0)
		{
			printf("session[%d] ����ʧ��\n", session->getSessionID());
		}
		else if (status == 1)
		{
			printf("session[%d] ���ӳɹ�\n", session->getSessionID());
		}
		else if (status == 2)
		{
			printf("session[%d] ���ӳ�ʱ\n", session->getSessionID());
		}
	});
	p2pClient->set_client_DisconnectCallback([](Client* client, Session* session) 
	{
		printf("session[%d] �Ͽ�����\n", session->getSessionID());
	});
	p2pClient->set_client_RecvCallback([](Client* client, Session* session, char* data, unsigned int len) 
	{
		printf("session[%d] �յ�%d���ֽ�\n", session->getSessionID(), len);
	});
	p2pClient->set_client_ClientCloseCallback([](Client* client) 
	{
		printf("�ͻ��˹ر�\n");
	});
	p2pClient->set_client_RemoveSessionCallback([](Client* client, Session* session) 
	{
		printf("ɾ��session[%d]\n", session->getSessionID());
	});


	p2pClient->set_server_StartCallback([](Server* svr, bool suc) 
	{
		printf("���ط���������%s\n", suc ? "�ɹ�" : "ʧ��");
	});
	p2pClient->set_server_CloseCallback([](Server* svr)
	{
		printf("���ط������ر�\n");
	});
	p2pClient->set_server_NewConnectCallback([](Server* svr, Session* session) 
	{
		printf("session[%d] ip:%s port:%d���������\n", session->getSessionID(), session->getIp().c_str(), session->getPort());
	});
	p2pClient->set_server_RecvCallback([](Server* svr, Session* session, char* data, unsigned int len) 
	{
		printf("�������յ� session[%d] %d���ֽ�\n", session->getSessionID(), len);
	});
	p2pClient->set_server_DisconnectCallback([](Server* svr, Session* session)
	{
		printf("session[%d] ip:%s port:%d�뿪������\n", session->getSessionID(), session->getIp().c_str(), session->getPort());
	});

	p2pClient->connectToCenterServer("www.kurumi.xin", 1003);
	//p2pClient->connectToCenterServer("127.0.0.1", 1003);

	while (runLoop)
	{
		runCMD();
		p2pClient->updateFrame();
		Sleep(1);
	}

	delete p2pClient;

	printMemInfo();

	system("pause");
}

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

std::vector<std::string> split(const std::string& str, const std::string& delim) {
	std::vector<std::string> res;
	if ("" == str) return res;
	//�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����
	char * strs = new char[str.length() + 1]; //��Ҫ����
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p) {
		std::string s = p; //�ָ�õ����ַ���ת��Ϊstring����
		res.push_back(s); //����������
		p = strtok(NULL, d);
	}

	return res;
}

bool isNumber(const std::string& str)
{
	for (int i = 0; i < str.size(); ++i)
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}


char szBuf[512];
int sessionID_Count = 0;
void runCMD()
{
	if (KEY_DOWN(VK_SPACE) && GetConsoleWindow() == GetForegroundWindow())
	{
		printf("����������:");
		std::cin.getline(szBuf, 512);
		std::vector<std::string> strlist = split(szBuf, " ");
		if (strlist.size() > 0)
		{
			if (strlist[0] == "getlist")
			{
				p2pClient->getUserList(0);
			}
			else if(strlist[0] == "newconnect" && strlist.size() > 1)
			{
				if (isNumber(strlist[1]))
				{
					unsigned int userID = atoi(strlist[1].c_str());
					for (auto &it : allUserInfoList)
					{
						if (userID == it.userID)
						{
							if (!it.hasPassword)
							{
								sessionID_Count++;
								if (!p2pClient->client_connect(userID, sessionID_Count, 0))
								{
									printf("��������ʧ��\n");
								}
								else
								{
									printf("�������ӳɹ�\n");
								}
								return;
							}
							while (true)
							{
								printf("��������������:\n");
								char szPassword[64];
								std::cin >> szPassword;
								if (isNumber(szPassword))
								{
									sessionID_Count++;
									if (!p2pClient->client_connect(userID, sessionID_Count, atoi(szPassword)))
									{
										printf("��������ʧ��\n");
									}
									else
									{
										printf("�������ӳɹ�\n");
									}
									return;
								}
								else
								{
									printf("����ֻ��Ϊ����\n");
								}
							}
						}
					}
					printf("�Ҳ���userID : %u\n", userID);
				}
				else
				{
					printf("�����ȷ�����������ʽΪ:newconnect [userID]");
				}
			}
			else if (strlist[0] == "connect" && strlist.size() > 2)
			{
				if (isNumber(strlist[1]) && isNumber(strlist[2]))
				{
					unsigned int userID = atoi(strlist[1].c_str());
					unsigned int sessionID = atoi(strlist[2].c_str());
					for (auto &it : allUserInfoList)
					{
						if (userID == it.userID)
						{
							if (!it.hasPassword)
							{
								sessionID_Count++;
								if (!p2pClient->client_connect(userID, sessionID_Count, 0))
								{
									printf("��������ʧ��\n");
								}
								else
								{
									printf("�������ӳɹ�\n");
								}
								return;
							}
							while (true)
							{
								printf("��������������:\n");
								char szPassword[64];
								std::cin >> szPassword;
								if (isNumber(szPassword))
								{
									if (!p2pClient->client_connect(userID, sessionID, atoi(szPassword)))
									{
										printf("��������ʧ��\n");
									}
									else
									{
										printf("�������ӳɹ�\n");
									}
									return;
								}
								else
								{
									printf("����ֻ��Ϊ����\n");
								}
							}
						}
					}
					printf("�Ҳ���userID : %u\n", userID);
				}
				else
				{
					printf("�����ȷ�����������ʽΪ:connect [userID] [sessionID]");
				}
			}
			else if (strlist[0] == "dis" && strlist.size() > 1)
			{
				if (isNumber(strlist[1]))
				{
					unsigned int sessionID = atoi(strlist[1].c_str());
					p2pClient->client_disconnect(sessionID);
				}
				else
				{
					printf("�����ȷ�����������ʽΪ:dis [sessionID]");
				}
			}
			else if (strlist[0] == "send" && strlist.size() > 2)
			{
				if (isNumber(strlist[1]))
				{
					unsigned int sessionID = atoi(strlist[1].c_str());
					p2pClient->client_send(sessionID, (char*)strlist[2].c_str(), strlist[2].size());
				}
				else
				{
					printf("�����ȷ�����������ʽΪ:send [sessionID] data");
				}
			}
			else if (strlist[0] == "startsvr" && strlist.size() > 2)
			{
				if (isNumber(strlist[2]))
				{
					std::string name = strlist[1];
					unsigned int password = atoi(strlist[2].c_str());
					if (!p2pClient->server_startServer(name.c_str(), password))
					{
						printf("����ʧ��\n");
					}
				}
				else
				{
					printf("�����ȷ�����������ʽΪ:startsvr name password");
				}
			}
			else if (strlist[0] == "stopsvr")
			{
				p2pClient->server_stopServer();
			}
			else
			{
				printf("��Ч����\n");
			}
		}
	}
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
	CreateDempFile(TEXT("p2pClient.dmp"), pException);
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif
