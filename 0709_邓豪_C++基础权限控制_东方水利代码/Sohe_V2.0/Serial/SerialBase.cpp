#include "SerialBase.h"

bool IPSG::CSerialBase::m_sbExit = false;
const UINT SLEEP_TIME_INTERVAL = 5;			//串口无数据等待时间，单位：ms

IPSG::CSerialBase::CSerialBase(BYTE ucTail, bool bTailCtrolEnd)
{
	m_ucTail = ucTail;
	m_bTailCtrolEnd = bTailCtrolEnd;
	m_bBusBusyFlag = false;

	m_hCOM = INVALID_HANDLE_VALUE;
	m_hListenThread = INVALID_HANDLE_VALUE;
	InitializeCriticalSection(&m_csCommunicationSync);
}

IPSG::CSerialBase::CSerialBase(int nLength, bool bTailCtrolEnd)
{
	m_nFrameLength = nLength;
	m_bTailCtrolEnd = bTailCtrolEnd;
	m_bBusBusyFlag = false;

	m_hCOM = INVALID_HANDLE_VALUE;
	m_hListenThread = INVALID_HANDLE_VALUE;
	InitializeCriticalSection(&m_csCommunicationSync);
}

IPSG::CSerialBase::~CSerialBase()
{
	CloseListenTread();
	ClosePort();
	DeleteCriticalSection(&m_csCommunicationSync);
	m_vucReceiveBuff.clear();
}

bool IPSG::CSerialBase::InitPort(UINT unPortNO, UINT unBaud, char cParity, UINT unDatabits, UINT unStopsbits, DWORD dwCommEvents)
{
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", unBaud, cParity, unDatabits, unStopsbits);

	if (!OpenPort(unPortNO)) 
		return false;

	EnterCriticalSection(&m_csCommunicationSync);
	bool bSuccess = true;

	COMMTIMEOUTS  CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 0;

	if (bSuccess)
		bSuccess = SetCommTimeouts(m_hCOM, &CommTimeouts);

	DCB  dcb;
	if (bSuccess)
	{
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
			bSuccess = true;

		bSuccess = GetCommState(m_hCOM, &dcb) && BuildCommDCB(pwText, &dcb);
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		
		delete[] pwText;
	}

	if (bSuccess)
		bSuccess = SetCommState(m_hCOM, &dcb);

	PurgeComm(m_hCOM, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	LeaveCriticalSection(&m_csCommunicationSync);

	return (bSuccess == true);
}

bool IPSG::CSerialBase::InitPort(UINT unPortNO, const LPDCB& plDCB)
{
	if (!OpenPort(unPortNO))
		return false;

	EnterCriticalSection(&m_csCommunicationSync);

	if (!SetCommState(m_hCOM, plDCB))
		return false;

	PurgeComm(m_hCOM, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

void IPSG::CSerialBase::ClosePort()
{
	if (m_hCOM != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCOM);
		m_hCOM = INVALID_HANDLE_VALUE;
	}
}

bool IPSG::CSerialBase::OpenPort(UINT unPortNO)
{
	EnterCriticalSection(&m_csCommunicationSync);

	char szPort[50];
	sprintf_s(szPort, "COM%d", unPortNO);

	m_hCOM = CreateFileA(szPort,		/** 设备名,COM1,COM2等 */
		GENERIC_READ | GENERIC_WRITE,	/** 访问模式,可同时读写 */
		0,								/** 共享模式,0表示不共享 */
		NULL,							/** 安全性设置,一般使用NULL */
		OPEN_EXISTING,					/** 该参数表示设备必须存在,否则创建失败 */
		0,
		0);

	if (m_hCOM == INVALID_HANDLE_VALUE)
	{
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}
	LeaveCriticalSection(&m_csCommunicationSync);
	return true;
}

bool IPSG::CSerialBase::OpenListenThread()
{
	if (m_hListenThread != INVALID_HANDLE_VALUE)
		return false;

	m_sbExit = false;
	UINT ThreadID;
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &ThreadID);
	if (!m_hListenThread)
		return false;

	if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))
		return false;

	return true;
}

bool IPSG::CSerialBase::CloseListenTread()
{
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		m_sbExit = true;
		Sleep(10);
		CloseHandle(m_hListenThread);
		m_hListenThread = INVALID_HANDLE_VALUE;
	}
	return true;
}

UINT IPSG::CSerialBase::GetBytesInCOM()
{
	DWORD dwError = 0;  // 错误码 
	COMSTAT  comstat;   // COMSTAT结构体,记录通信设备的状态信息
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT BytesInQue = 0;
	
	if (ClearCommError(m_hCOM, &dwError, &comstat))// 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 
		BytesInQue = comstat.cbInQue; // 获取在输入缓冲区中的字节数 

	return BytesInQue;
}

UINT IPSG::CSerialBase::ListenThread(void* pParam)
{
	CSerialBase *pSerialBase = reinterpret_cast<CSerialBase*>(pParam);	//得到本类指针

	while (!pSerialBase->m_sbExit)
	{
		UINT BytesInQue = pSerialBase->GetBytesInCOM();
		if (BytesInQue == 0)
		{
			Sleep(SLEEP_TIME_INTERVAL);
			continue;
		}
		pSerialBase->m_bBusBusyFlag = true;
		unsigned char ucRecved = 0X00;
		pSerialBase->m_vucReceiveBuff.clear();

		if (pSerialBase->m_bTailCtrolEnd)	//串口通信由帧尾控制接收完成
		{
			do
			{
				ucRecved = 0X00;
				if (pSerialBase->ReadChar(ucRecved) == true)
				{
					pSerialBase->m_vucReceiveBuff.push_back(ucRecved);
				}
				if ((pSerialBase->m_ucTail == ucRecved))
					break;
			} while ((--BytesInQue) || (ucRecved != pSerialBase->m_ucTail)); 
			pSerialBase->m_bBusBusyFlag = false;
			pSerialBase->CommunicationDeCode();
			pSerialBase->m_vucReceiveBuff.clear();
		}
		else
		{
			do
			{
				//unRecvByteCnt++;
				ucRecved = 0x00;
				if (BytesInQue == 0)
					BytesInQue = 1;
				if (pSerialBase->ReadChar(ucRecved) == true)
				{
					pSerialBase->m_vucReceiveBuff.push_back(ucRecved);
				}
			} while ((--BytesInQue));
			if (pSerialBase->m_vucReceiveBuff.at(0) != 0XFF)
			{
				pSerialBase->m_vucReceiveBuff.clear();
				continue;
			}
			pSerialBase->TableDecode();
			pSerialBase->m_vucReceiveBuff.clear();
		}

	}
	return 0;
}

bool IPSG::CSerialBase::ReadChar(unsigned char &cRecved)
{
	FILE_LOG(logDEBUG) << "ReadChar calls.";
	bool bResult = true;
	DWORD dwBytesRead = 0;

	if (m_hCOM == INVALID_HANDLE_VALUE)
		return false;

	EnterCriticalSection(&m_csCommunicationSync);
	
	bResult = ReadFile(m_hCOM, &cRecved, 1, &dwBytesRead, NULL);
	if ((!bResult)) //查看错误原因
	{
		DWORD dwError = GetLastError();
		PurgeComm(m_hCOM, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}
	LeaveCriticalSection(&m_csCommunicationSync);
	FILE_LOG(logDEBUG) << "readChar return.";
	return (dwBytesRead == 1);
}

bool IPSG::CSerialBase::WriteData(unsigned char* pData, unsigned int nLength)
{
	FILE_LOG(logDEBUG) << "WritData call.";
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;

	if (m_hCOM == INVALID_HANDLE_VALUE)
		return false;

	EnterCriticalSection(&m_csCommunicationSync);
	bResult = WriteFile(m_hCOM, pData, nLength, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		PurgeComm(m_hCOM, PURGE_TXCLEAR | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}
	LeaveCriticalSection(&m_csCommunicationSync);
	FILE_LOG(logDEBUG) << "WriteData return.";
	return true;
}

bool IPSG::CSerialBase::WriteString(std::string strSendData)
{
	bool bResult = true;
	DWORD dwBytesSend = 0;

	if (m_hCOM == INVALID_HANDLE_VALUE)
		return false;

	EnterCriticalSection(&m_csCommunicationSync);
	bResult = WriteFile(m_hCOM, strSendData.c_str(), strSendData.size(), &dwBytesSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		PurgeComm(m_hCOM, PURGE_RXCLEAR | PURGE_RXABORT);
		LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}
	LeaveCriticalSection(&m_csCommunicationSync);

	return true;
}

bool IPSG::CSerialBase::ECode()
{
	FILE_LOG(logINFO) << "ECode Virtual";
	return true;
}

bool IPSG::CSerialBase::CommunicationDeCode()
{
	FILE_LOG(logINFO) << "Communication decode Virtual";
	return true;
}

bool IPSG::CSerialBase::TableDecode()
{
	FILE_LOG(logINFO) << "Table decode Virtual";
	return true;
}