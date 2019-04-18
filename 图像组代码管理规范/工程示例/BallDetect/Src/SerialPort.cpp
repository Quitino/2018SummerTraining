#include "SerialPort.h"

//������������ʱ������һ�β�ѯ�ĵȴ�ʱ�䣬��λ��s
const UINT SLEEP_TIME_INTERVAL = 5;

IPSG::CSerialPort::CSerialPort()
{
	m_hCom = INVALID_HANDLE_VALUE;
	m_bExit = false;
	m_hListenThread = INVALID_HANDLE_VALUE;
	InitializeCriticalSection(&m_csComunicationSync);
}

IPSG::CSerialPort::~CSerialPort()
{
	CloseListenThread();
	ClosePort();
	DeleteCriticalSection(&m_csComunicationSync);
}

bool IPSG::CSerialPort::OpenPort(UINT uPortNO)
{
	//�����ٽ��
	EnterCriticalSection(&m_csComunicationSync);

	//�Ѵ��ڵı��ת��Ϊ�豸��
	char cSzPort[50];
	sprintf_s(cSzPort, "COM%d", uPortNO);

	m_hCom = CreateFileA(cSzPort,		//���ں�
		GENERIC_READ | GENERIC_WRITE,	//�����д
		0,								//����ģʽ��������
		NULL,
		OPEN_EXISTING,					//������־
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	//�첽IO,ͬ��IO�����������߳�
		NULL);

	if (m_hCom == INVALID_HANDLE_VALUE)	//��ʧ�ܣ��ͷ���Դ
	{
		LeaveCriticalSection(&m_csComunicationSync);
		return false;
	}

	LeaveCriticalSection(&m_csComunicationSync);
	return true;
}

bool IPSG::CSerialPort::ClosePort()
{
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
		m_hCom = INVALID_HANDLE_VALUE;
	}
}

bool IPSG::CSerialPort::InitPort(UINT uPortNo /* = 1 */, UINT uBaud /* = CBR_115200 */, char cParity /* = 'N' */,
	UINT uDataBits /* = 8 */, UINT uStopBits /* = 1 */, DWORD dwComEvents /* = EV_RXCHAR */)
{
	char cSzDCBParam[50];
	sprintf_s(cSzDCBParam, "baud=%d parity=%c data=%d stop=%d", uBaud, cParity, uDataBits, uStopBits);

	if (!OpenPort(uPortNo))
	{
		std::cerr << "Cannot open COM" << uPortNo << std::endl;
		return false;
	}

	EnterCriticalSection(&m_csComunicationSync);

	bool bSuccess = true;
	//���ó�ʱʱ��
	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 0;

	if (bSuccess)
	{
		bSuccess = SetCommTimeouts(m_hCom, &CommTimeouts);
	}

	DCB Dcb;
	if (bSuccess)
	{
		//��ANSI�ַ���ת��ΪUNICODE�ַ���
		DWORD dwNO = MultiByteToWideChar(CP_ACP, 0, cSzDCBParam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNO];
		if ( !MultiByteToWideChar(CP_ACP,0,cSzDCBParam,-1,pwText,dwNO))
		{
			bSuccess = true;
		}

		//ֱ�����ô��ڲ������ڸ��ӣ������Ȼ�ȡ��ǰ�������ò������ٹ��촮��DCB����
		bSuccess = GetCommState(m_hCom, &Dcb) && BuildCommDCB(pwText, &Dcb);

		//TRS FLOW ����
		Dcb.fRtsControl = RTS_CONTROL_ENABLE;

		delete[] pwText;
	}

	if (bSuccess)
	{
		//ʹ��DCB�������ô���״̬
		bSuccess = SetCommState(m_hCom, &Dcb);
	}

	//��ջ�����
	PurgeComm(m_hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	//�뿪�ٽ��
	LeaveCriticalSection(&m_csComunicationSync);

	return bSuccess == true;
}

bool IPSG::CSerialPort::OpenListenThread()
{
	//�߳��ѿ���
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		//log error
		return false;
	}

	m_bExit = false;
	UINT ThreadID;	//�߳�ID

	//���������߳�
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &ThreadID);
	if ( !m_hListenThread)
	{
		return false;
	}

	//���߳����ȼ�����Ϊ����ͨ�̸߳���
	if ( !SetThreadPriority(m_hListenThread,THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}
	return true;
}

UINT IPSG::CSerialPort::GetBytesInCom()
{
	DWORD dwError = 0;	//������
	COMSTAT comstat;	//�豸״̬��Ϣ
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT uBytesInQue = 0;

	//�����ǰ�����Ĵ����־
	if (ClearCommError(m_hCom,&dwError,&comstat))
	{
		uBytesInQue = comstat.cbInQue;
	}

	return uBytesInQue;
}

bool IPSG::CSerialPort::ReadChar(char &cReceivedData)
{
	bool bResult = true;
	DWORD dwBytesRead = 0;
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		std::cerr << "COM state error" << std::endl;
		return false;
	}

	EnterCriticalSection(&m_csComunicationSync);

	//���ٽ�����ȡһ���ֽ�
	bResult = ReadFile(m_hCom, &cReceivedData, 1, &dwBytesRead, NULL);

	if ( !bResult)
	{
		//δ�ɹ���ȡ���ʶ�ȡ�����룬�鿴ԭ��
		DWORD dwError = GetLastError();

		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_RXCLEAR);
		LeaveCriticalSection(&m_csComunicationSync);

		return false;
	}

	LeaveCriticalSection(&m_csComunicationSync);
	
	return 1 == dwBytesRead;
}

UINT WINAPI IPSG::CSerialPort::ListenThread(void *pParam)
{
	//��ȡ����ָ��
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

	while ( !pSerialPort->m_bExit)
	{
		UINT uBytesInQue = pSerialPort->GetBytesInCom();
		if (0 == uBytesInQue)   //������
		{
			Sleep(SLEEP_TIME_INTERVAL);
			continue;
		}

		char cReceivedData = 0X00;
		do 
		{
			cReceivedData = 0X00;
			if (pSerialPort->ReadChar(cReceivedData))
			{
				std::cout << cReceivedData;			//ֱ�Ӵ�ӡ��Ҳ����ת��
				continue;
			}
		} while (--uBytesInQue);
	}
	return 0;
}

bool IPSG::CSerialPort::CloseListenThread()
{
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		m_bExit = true;
		Sleep(10);

		CloseHandle(m_hListenThread);
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

bool IPSG::CSerialPort::WriteData(std::string strWriteData)
{
	bool bResult = true;
	DWORD dwBytesSend = 0;

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		std::cerr << "COM is not opened" << std::endl;
		return false;
	}

	EnterCriticalSection(&m_csComunicationSync);

	bResult = WriteFile(m_hCom, strWriteData.c_str(), strWriteData.size(), &dwBytesSend, NULL);

	if (bResult)
	{
		DWORD dwError = GetLastError();
		PurgeComm(m_hCom, PURGE_TXABORT | PURGE_TXCLEAR);
		LeaveCriticalSection(&m_csComunicationSync);
	}
	LeaveCriticalSection(&m_csComunicationSync);

	return bResult == true;
}