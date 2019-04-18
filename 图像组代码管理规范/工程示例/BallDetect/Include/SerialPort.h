/***********************************************
 *  Copyright Notice
 *  Copyright (c) 2017,Hughie
 *
 *  @File    : SerialPort.h
 *  @Brief   : Windows�´��ڶ�д�ķ�װ
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
		 *  @Descrip : ���캯������ʼ����Ա����
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialPort();

		/***********************************************
		 *  @Name    : CSerialPort::~CSerialPort
		 *  @Descrip : �����������رմ��ڣ��رռ����̣߳�����ٽ���ռ����Դ
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialPort();

		/***********************************************
		 *  @Name    : CSerialPort::InitPort
		 *  @Descrip : ���ݸ���������ʼ������
		 *  @Para    : [in]	uPortNo�����ں�	
		 *  @Para    : [in] uBaud��������
		 *  @Para    : [in] cParity����żУ��
		 *  @Para    : [in] uDataBits������λ
		 *  @Para    : [in] uStopBits��ֹͣλ
		 *  @Para    : [in]	dwComEvents ��Ӧ����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool InitPort(UINT uPortNo = 1, UINT uBaud = CBR_115200, char cParity = 'N', UINT uDataBits = 8, UINT uStopBits = 1,
			DWORD dwComEvents = EV_RXCHAR);

		/***********************************************
		 *  @Name    : CSerialPort::OpenListenThread
		 *  @Descrip : ���������̣߳��յ��ַ����ӡ��ʾ
		 *  @Return  : bool true:success false:error
		 *  @Notes   : None
		 ***********************************************/
		bool OpenListenThread();

		/***********************************************
		 *  @Name    : CSerialPort::CloseListenThread
		 *  @Descrip : �رռ����߳�
		 *  @Return  : bool true:success false:error
		 *  @Notes   : None
		 ***********************************************/
		bool CloseListenThread();

		/***********************************************
		 *  @Name    : CSerialPort::WriteData
		 *  @Descrip : ��λ���򴮿�д������
		 *  @Para    : [in]		strWriteData����д�������
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool WriteData(std::string strWriteData);

		/***********************************************
		 *  @Name    : CSerialPort::GetBytesInCom
		 *  @Descrip : ��ȡ�����������ݳ���
		 *  @Return  : UINT ��ʾ�����������ڴ���ȡ���ֽ���
		 *  @Notes   : None
		 ***********************************************/
		UINT GetBytesInCom();

		/***********************************************
		 *  @Name    : CSerialPort::ReadChar
		 *  @Descrip : �Ӵ��ڶ�ȡһ���ַ�
		 *	@Para    : [in/out]	cReceivedData���Ӵ��ڶ�ȡ��������
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ReadChar(char &cReceivedData);
	private:
		/***********************************************
		 *  @Name    : CSerialPort::OpenPort
		 *  @Descrip : �򿪶˿ڣ��趨��ʽ
		 *  @Para    : [in]		uPortNO�����ں�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenPort(UINT uPortNO);

		/***********************************************
		 *  @Name    : CSerialPort::ClosePort
		 *  @Descrip : �ر��Ѵ򿪵Ĵ���
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ClosePort();

		/***********************************************
		 *  @Name    : CSerialPort::ListenThread
		 *  @Descrip : �������ڵ���Ӧ����
		 *  @Return  : 0
		 *  @Notes   : None
		 ***********************************************/
		static UINT WINAPI ListenThread(void *pParam);
	private:
		HANDLE				m_hCom;					//���ھ��
		static bool			m_bExit;				//�߳��˳���־λ
		volatile HANDLE		m_hListenThread;		//�߳̾��
		CRITICAL_SECTION	m_csComunicationSync;	//�ٽ���������ͬ�������������
	};
}

#endif // !SERIAL_PORT_H_
