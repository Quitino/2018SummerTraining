#include "Server.h"

IPSG::CServer::CServer()
{
	m_hServerConnect = INVALID_HANDLE_VALUE;
	m_hDataReceive = INVALID_HANDLE_VALUE;

	InitializeCriticalSection(&m_csServer);
	m_bCruise = false;
	m_bShutDown = false;
}

IPSG::CServer::~CServer()
{
	closesocket(m_socClient);
	WSACleanup();

	CloseListenThread();
	CloseReceiveDataListenThread();
}

UINT IPSG::CServer::ServerAccept(void* pParam)
{
	CServer *pSclient = reinterpret_cast<CServer*>(pParam);	//��ȡ����ָ��
	sockaddr_in addrClient;
	int nAddrlen = sizeof(addrClient);
	while (pSclient->m_socClient != INVALID_SOCKET)
	{
		pSclient->m_socClient = accept(pSclient->m_socListen, (SOCKADDR *)&addrClient, &nAddrlen);
		
		//����ʧ��
		if (pSclient->m_socClient == SOCKET_ERROR)
		{
			FILE_LOG(logINFO) << WSAGetLastError()<<":�ͻ�������ʧ��";
			continue;
		}

		//���ӳɹ�
		FILE_LOG(logINFO) << "���ӳɹ����ͻ���IPΪ��" << inet_ntoa(addrClient.sin_addr);
		if ((pSclient->m_socClient != SOCKET_ERROR))
		{
			pSclient->OpenReceiveDataListenThread();
		}
	}

	return 0;
}

bool IPSG::CServer::SendData(const std::string strSendData)
{
	if (m_socClient == INVALID_SOCKET)
	{
		FILE_LOG(logINFO) << "�ͷ�������ʧ��";
		return false;
	}

	int nSendData = send(m_socClient, strSendData.c_str(), strSendData.size(), 0);
	if (nSendData == SOCKET_ERROR)
	{
		FILE_LOG(logINFO) << "���ݷ���ʧ��";
		return false;
	}
			
	FILE_LOG(logINFO) << "���ݷ��ͳɹ������͵������ǣ�" << strSendData;
	return true;

}

bool IPSG::CServer::ServerInit(int nPortNo, const std::string strIPAddress)
{
	EnterCriticalSection(&m_csServer);

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
		return false;

	//socket��������
	sockaddr_in	socket_in;
	socket_in.sin_family = AF_INET;
	socket_in.sin_port = htons(nPortNo);
	socket_in.sin_addr.S_un.S_addr = inet_addr(strIPAddress.c_str());
	m_socListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socListen == INVALID_SOCKET)
	{
		FILE_LOG(logINFO) << "���Ӽ����׽��ִ���ʧ��";
		return false;
	}

	if (bind(m_socListen, (LPSOCKADDR)&socket_in, sizeof(SOCKADDR_IN)))
	{
		FILE_LOG(logINFO) << "���Ӽ����׽��ֱ���IP��ʧ��";
		return false;
	}

	if (listen(m_socListen, 5) == SOCKET_ERROR)
	{
		FILE_LOG(logINFO) << "�ͻ���������Ŀ����";
		return false;
	}

	LeaveCriticalSection(&m_csServer);
	FILE_LOG(logINFO) << "�����ʼ���ɹ�";

	return true;
}

bool IPSG::CServer::OpenServerListenThread()
{
	if (m_hServerConnect != INVALID_HANDLE_VALUE)
		return false;

	UINT ThreadID;
	m_hServerConnect = (HANDLE)_beginthreadex(NULL, 0, ServerAccept, this, 0, &ThreadID);

	if (!m_hServerConnect)
		return false;

	if (!SetThreadPriority(m_hServerConnect, THREAD_PRIORITY_ABOVE_NORMAL))
		return false;

	return true;
}

bool IPSG::CServer::CloseListenThread()
{
	if (m_hServerConnect != INVALID_HANDLE_VALUE)
	{
		Sleep(5);
		CloseHandle(m_hServerConnect);
		m_hServerConnect = INVALID_HANDLE_VALUE;
	}

	return true;
}

UINT IPSG::CServer::ReceiveData(void* pParam)
{
	char c_ReceiveData[10];
	CServer *pReceiveData = reinterpret_cast<CServer*>(pParam);

	while (pReceiveData->m_socClient != SOCKET_ERROR)
	{
		memset(c_ReceiveData, 0, sizeof(c_ReceiveData));
		recv(pReceiveData->m_socClient, c_ReceiveData, sizeof(c_ReceiveData), 0);
		if (DataSendSuccess == c_ReceiveData[0])
		{
			std::cout << "���ճɹ���" << c_ReceiveData[0] << std::endl;
			pReceiveData->m_bCruise = true;
		}
		else if (SHUTDOWN == c_ReceiveData[0])
		{
			std::cout << "�����رռ����" << std::endl;
			pReceiveData->m_bShutDown = true;
		}
	}
	closesocket(pReceiveData->m_socClient);

	return 0;
}

bool IPSG::CServer::OpenReceiveDataListenThread()
{
	if (m_hDataReceive != INVALID_HANDLE_VALUE)
	{
		return false;
	}

	UINT ThreadID;
	m_hDataReceive = (HANDLE)_beginthreadex(NULL, 0, ReceiveData, this, 0, &ThreadID);
	if (!m_hDataReceive)
	{
		return false;
	}
	if (!SetThreadPriority(m_hDataReceive, THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}

	return true;
}

bool IPSG::CServer::CloseReceiveDataListenThread()
{
	if (m_hDataReceive != INVALID_HANDLE_VALUE)
	{
		Sleep(10);
		CloseHandle(m_hDataReceive);
		m_hDataReceive = INVALID_HANDLE_VALUE;
	}
	return true;
}