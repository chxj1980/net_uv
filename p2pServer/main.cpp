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
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

void main()
{
#if OPEN_NET_UV_DEBUG == 1
	// ע���쳣������
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif

	P2PServer* svr = new P2PServer();

	svr->set_server_StartCallback([](Server*, bool suc) 
	{
		if (suc)
		{
			printf("�����������ɹ�!!\n");
		}
		else
		{
			printf("����������ʧ��!!\n");
		}
	});
	svr->set_server_CloseCallback([](Server*) 
	{
		runLoop = false;
	});

	svr->startServer("0.0.0.0", 1003);

	char szCMDBuf[256] = {0};
	while (runLoop)
	{
		//if (KEY_DOWN(VK_SPACE) && GetConsoleWindow() == GetForegroundWindow())
		//{
		//	printf("����������:");
		//	std::cin >> szCMDBuf;
		//	if (strstr(szCMDBuf, "stop"))
		//	{
		//		svr->stopServer();
		//	}
		//}
		svr->updateFrame();
		Sleep(1);
	}
	delete svr;

	printMemInfo();

	system("pause");
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
	CreateDempFile(TEXT("p2pServer.dmp"), pException);
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif