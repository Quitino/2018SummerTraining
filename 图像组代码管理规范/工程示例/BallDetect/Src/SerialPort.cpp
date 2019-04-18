#include "SerialPort.h"

//当串口无数据时，到下一次查询的等待时间，单位：s
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
	//进入临界段
	EnterCriticalSection(&m_csComunicationSync);

	//把串口的编号转换为设备名
	char cSzPort[50];
	sprintf_s(cSzPort, "COM%d", uPortNO);

	m_hCom = CreateFileA(cSzPort,		//串口号
		GENERIC_READ | GENERIC_WRITE,	//允许读写
		0,								//共享模式，不共享
		NULL,
		OPEN_EXISTING,					//创建标志
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	//异步IO,同步IO会阻塞监听线程
		NULL);

	if (m_hCom == INVALID_HANDLE_VALUE)	//打开失败，释放资源
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
	//设置超时时间
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
		//将ANSI字符串转化为UNICODE字符串
		DWORD dwNO = MultiByteToWideChar(CP_ACP, 0, cSzDCBParam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNO];
		if ( !MultiByteToWideChar(CP_ACP,0,cSzDCBParam,-1,pwText,dwNO))
		{
			bSuccess = true;
		}

		//直接设置串口参数过于复杂，所以先获取当前串口配置参数，再构造串口DCB参数
		bSuccess = GetCommState(m_hCom, &Dcb) && BuildCommDCB(pwText, &Dcb);

		//TRS FLOW 控制
		Dcb.fRtsControl = RTS_CONTROL_ENABLE;

		delete[] pwText;
	}

	if (bSuccess)
	{
		//使用DCB参数配置串口状态
		bSuccess = SetCommState(m_hCom, &Dcb);
	}

	//清空缓存区
	PurgeComm(m_hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	//离开临界段
	LeaveCriticalSection(&m_csComunicationSync);

	return bSuccess == true;
}

bool IPSG::CSerialPort::OpenListenThread()
{
	//线程已开启
	if (m_hListenThread != INVALID_HANDLE_VALUE)
	{
		//log error
		return false;
	}

	m_bExit = false;
	UINT ThreadID;	//线程ID

	//创建监听线程
	m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &ThreadID);
	if ( !m_hListenThread)
	{
		return false;
	}

	//将线程优先级设置为比普通线程更高
	if ( !SetThreadPriority(m_hListenThread,THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return false;
	}
	return true;
}

UINT IPSG::CSerialPort::GetBytesInCom()
{
	DWORD dwError = 0;	//错误码
	COMSTAT comstat;	//设备状态信息
	memset(&comstat, 0, sizeof(COMSTAT));

	UINT uBytesInQue = 0;

	//清除此前遗留的错误标志
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

	//从临界区读取一个字节
	bResult = ReadFile(m_hCom, &cReceivedData, 1, &dwBytesRead, NULL);

	if ( !bResult)
	{
		//未成功读取，故读取错误码，查看原因
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
	//获取本类指针
	CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

	while ( !pSerialPort->m_bExit)
	{
		UINT uBytesInQue = pSerialPort->GetBytesInCom();
		if (0 == uBytesInQue)   //无数据
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
				std::cout << cReceivedData;			//直接打印，也可以转存
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