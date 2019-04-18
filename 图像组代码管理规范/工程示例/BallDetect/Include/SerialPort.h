/***********************************************
 *  Copyright Notice
 *  Copyright (c) 2017,Hughie
 *
 *  @File    : SerialPort.h
 *  @Brief   : Windows下串口读写的封装
 *
 *  @Version : V1.0
 *  @Data    : 2017/8/13
 
 *	@History :
 *    Author : Hughie
 *    Descrip: Creat File
 ***********************************************/
#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <iostream>
#include <Windows.h>
#include <process.h>

namespace IPSG
{
	class CSerialPort
	{
	public:
		/***********************************************
		 *  @Name    : CSerialPort::CSerialPort
		 *  @Descrip : 构造函数，初始化成员变量
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialPort();

		/***********************************************
		 *  @Name    : CSerialPort::~CSerialPort
		 *  @Descrip : 析构函数，关闭串口，关闭监听线程，清楚临界区占用资源
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialPort();

		/***********************************************
		 *  @Name    : CSerialPort::InitPort
		 *  @Descrip : 根据给定参数初始化串口
		 *  @Para    : [in]	uPortNo：串口号	
		 *  @Para    : [in] uBaud：波特率
		 *  @Para    : [in] cParity：奇偶校验
		 *  @Para    : [in] uDataBits：数据位
		 *  @Para    : [in] uStopBits：停止位
		 *  @Para    : [in]	dwComEvents 响应触发
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool InitPort(UINT uPortNo = 1, UINT uBaud = CBR_115200, char cParity = 'N', UINT uDataBits = 8, UINT uStopBits = 1,
			DWORD dwComEvents = EV_RXCHAR);

		/***********************************************
		 *  @Name    : CSerialPort::OpenListenThread
		 *  @Descrip : 开启监听线程，收到字符后打印显示
		 *  @Return  : bool true:success false:error
		 *  @Notes   : None
		 ***********************************************/
		bool OpenListenThread();

		/***********************************************
		 *  @Name    : CSerialPort::CloseListenThread
		 *  @Descrip : 关闭监听线程
		 *  @Return  : bool true:success false:error
		 *  @Notes   : None
		 ***********************************************/
		bool CloseListenThread();

		/***********************************************
		 *  @Name    : CSerialPort::WriteData
		 *  @Descrip : 上位机向串口写入数据
		 *  @Para    : [in]		strWriteData：待写入的数据
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool WriteData(std::string strWriteData);

		/***********************************************
		 *  @Name    : CSerialPort::GetBytesInCom
		 *  @Descrip : 获取缓存区的数据长度
		 *  @Return  : UINT 表示读缓存区存在待读取的字节数
		 *  @Notes   : None
		 ***********************************************/
		UINT GetBytesInCom();

		/***********************************************
		 *  @Name    : CSerialPort::ReadChar
		 *  @Descrip : 从串口读取一个字符
		 *	@Para    : [in/out]	cReceivedData：从串口读取到的数据
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ReadChar(char &cReceivedData);
	private:
		/***********************************************
		 *  @Name    : CSerialPort::OpenPort
		 *  @Descrip : 打开端口，设定格式
		 *  @Para    : [in]		uPortNO：串口号
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenPort(UINT uPortNO);

		/***********************************************
		 *  @Name    : CSerialPort::ClosePort
		 *  @Descrip : 关闭已打开的串口
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ClosePort();

		/***********************************************
		 *  @Name    : CSerialPort::ListenThread
		 *  @Descrip : 监听串口的响应函数
		 *  @Return  : 0
		 *  @Notes   : None
		 ***********************************************/
		static UINT WINAPI ListenThread(void *pParam);
	private:
		HANDLE				m_hCom;					//串口句柄
		static bool			m_bExit;				//线程退出标志位
		volatile HANDLE		m_hListenThread;		//线程句柄
		CRITICAL_SECTION	m_csComunicationSync;	//临界区保护，同步互斥操作串口
	};
}

#endif // !SERIAL_PORT_H_
