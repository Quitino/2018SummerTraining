/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,邓豪
 *
 *  @File    : SerialBase.h
 *  @Brief   : 串口读写类的实现
 *
 *  @Version : V1.0
 *  @Date    : 2018/1/23
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/
#ifndef SERIAL_BASE_H_
#define SERIAL_BASE_H_

#include <iostream>
#include <string>
#include <vector>
#include <process.h>
#include <Windows.h>
#include <memory>

#include "../log.h"

namespace IPSG
{
	class CSerialBase
	{
	public:
		CSerialBase();
		/***********************************************
		 *  @Name    : CSerialBase::CSerialBase
		 *  @Descrip : 构造函数
		 *  @Para    : [in]		ucTail：由帧尾控制接受完成的类其帧尾字节
		 *	@Para    : [in]		bTailCtrolEnd：是否由帧尾控制接受完成
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialBase(BYTE ucTail, bool bTailCtrolEnd = true);

		/***********************************************
		 *  @Name    : CSerialBase::CSerialBase
		 *  @Descrip : 构造函数
		 *  @Para    : [in]		nLength：对于无帧尾的传感器设备，只能由数据帧长度控制接收完成，此输入变量为传感器返回数据帧长度
		 *	@Para    : [in]		bTailCtrolEnd：是否由帧尾控制接受完成
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialBase(int nLength, bool bTailCtrolEnd = false);

		/***********************************************
		 *  @Name    : CSerialBase::~CSerialBase
		 *  @Descrip : 析构函数
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialBase();

		/***********************************************
		 *  @Name    : CSerialBase::InitPort
		 *  @Descrip : 串口初始化函数
		 *  @Para    : [in]		unPortNO：串口号
		 *  @Para    : [in]		unBaud：波特率
		 *  @Para    : [in]		cParity：奇偶校验
		 *  @Para    : [in]		unDatabits：数据位
		 *  @Para    : [in]		unStopsbits：停止位
		 *	@Para    : [in]		dwCommEvents：响应事件
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool InitPort(UINT  unPortNO, UINT  unBaud = CBR_9600, char  cParity = 'N', UINT  unDatabits = 8, UINT  unStopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
		
		/***********************************************
		 *  @Name    : CSerialBase::InitPort
		 *  @Descrip : 串口初始化函数
		 *  @Para    : [in]		unPortNO：串口号
		 *	@Para    : [in]		plDCB：串口DCB结构属性
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool InitPort(UINT  unPortNO, const LPDCB& plDCB);

		/***********************************************
		 *  @Name    : CSerialBase::OpenListenThread
		 *  @Descrip : 打开串口监听线程
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenListenThread();

		/***********************************************
		 *  @Name    : CSerialBase::CloseListenTread
		 *  @Descrip : 关闭串口监听线程
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CloseListenTread();

		/***********************************************
		 *  @Name    : CSerialBase::WriteData
		 *  @Descrip : 向串口写入一个数组的数据
		 *  @Para    : [in]		pData：待写入的数据
		 *	@Para    : [in]		nLength：待写入的数据的长度
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool WriteData(unsigned char* pData, unsigned int nLength);

		/***********************************************
		 *  @Name    : CSerialBase::WriteString
		 *  @Descrip : 向串口写入一个字符串
		 *  @Para    : [in]		strSendData：待写入的字符串
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool WriteString(std::string strSendData);

		/***********************************************
		 *  @Name    : CSerialBase::GetBytesInCOM
		 *  @Descrip : 获取串口缓存区当前数据长度
		 *  @Para    : None
		 *  @Return  : UINT 缓存区
		 *  @Notes   : None
		 ***********************************************/
		UINT GetBytesInCOM();

		/***********************************************
		 *  @Name    : CSerialBase::ReadChar
		 *  @Descrip : 从串口缓存区读取一个字符
		 *	@Para    : [in/out]	cRecved：读取到的字符
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ReadChar(unsigned char &cRecved);

	private:
		/***********************************************
		 *  @Name    : CSerialBase::OpenPort
		 *  @Descrip : 打开串口
		 *  @Para    : [in]		unPortNO：串口号
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenPort(UINT  unPortNO);

		/***********************************************
		 *  @Name    : CSerialBase::ClosePort
		 *  @Descrip : 关闭串口
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		void ClosePort();

		/***********************************************
		 *  @Name    : CSerialBase::ListenThread
		 *  @Descrip : 串口监听线程
		 *  @Para    : None	
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		static UINT WINAPI ListenThread(void* pParam);

		/***********************************************
		 *  @Name    : CSerialBase::CommunicationDeCode
		 *  @Descrip : 串口通信数据帧解码虚函数，由串口通信类覆写
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		virtual bool CommunicationDeCode();

		/***********************************************
		 *  @Name    : CSerialBase::TableDecode
		 *  @Descrip : 云台返回数据帧解码虚函数，由串口通信类覆写
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		virtual bool TableDecode();

		/***********************************************
		 *  @Name    : CSerialBase::ECode
		 *  @Descrip : 串口发送数据编码虚函数
		 *  @Para    : None	
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		virtual bool ECode();

	public:
		std::vector<BYTE>			m_vucReceiveBuff;		//数据帧接收缓存区
		bool                        m_bBusBusyFlag;         //串口总线繁忙标志位
	private:
		BYTE						m_ucTail;				//帧尾
		int							m_nFrameLength;			//帧长
		bool						m_bTailCtrolEnd;		//由帧尾控制数据帧接收完成

		HANDLE						m_hCOM;					//串口句柄
		static bool					m_sbExit;				//线程退出标志
		volatile HANDLE				m_hListenThread;		//线程句柄
		CRITICAL_SECTION			m_csCommunicationSync;	//临界区
	};
}

#endif // !SERIAL_BASE_H
