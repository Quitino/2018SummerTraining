/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�˺�
 *
 *  @File    : SerialBase.h
 *  @Brief   : ���ڶ�д���ʵ��
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
		 *  @Descrip : ���캯��
		 *  @Para    : [in]		ucTail����֡β���ƽ�����ɵ�����֡β�ֽ�
		 *	@Para    : [in]		bTailCtrolEnd���Ƿ���֡β���ƽ������
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialBase(BYTE ucTail, bool bTailCtrolEnd = true);

		/***********************************************
		 *  @Name    : CSerialBase::CSerialBase
		 *  @Descrip : ���캯��
		 *  @Para    : [in]		nLength��������֡β�Ĵ������豸��ֻ��������֡���ȿ��ƽ�����ɣ����������Ϊ��������������֡����
		 *	@Para    : [in]		bTailCtrolEnd���Ƿ���֡β���ƽ������
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialBase(int nLength, bool bTailCtrolEnd = false);

		/***********************************************
		 *  @Name    : CSerialBase::~CSerialBase
		 *  @Descrip : ��������
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialBase();

		/***********************************************
		 *  @Name    : CSerialBase::InitPort
		 *  @Descrip : ���ڳ�ʼ������
		 *  @Para    : [in]		unPortNO�����ں�
		 *  @Para    : [in]		unBaud��������
		 *  @Para    : [in]		cParity����żУ��
		 *  @Para    : [in]		unDatabits������λ
		 *  @Para    : [in]		unStopsbits��ֹͣλ
		 *	@Para    : [in]		dwCommEvents����Ӧ�¼�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool InitPort(UINT  unPortNO, UINT  unBaud = CBR_9600, char  cParity = 'N', UINT  unDatabits = 8, UINT  unStopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
		
		/***********************************************
		 *  @Name    : CSerialBase::InitPort
		 *  @Descrip : ���ڳ�ʼ������
		 *  @Para    : [in]		unPortNO�����ں�
		 *	@Para    : [in]		plDCB������DCB�ṹ����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool InitPort(UINT  unPortNO, const LPDCB& plDCB);

		/***********************************************
		 *  @Name    : CSerialBase::OpenListenThread
		 *  @Descrip : �򿪴��ڼ����߳�
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenListenThread();

		/***********************************************
		 *  @Name    : CSerialBase::CloseListenTread
		 *  @Descrip : �رմ��ڼ����߳�
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CloseListenTread();

		/***********************************************
		 *  @Name    : CSerialBase::WriteData
		 *  @Descrip : �򴮿�д��һ�����������
		 *  @Para    : [in]		pData����д�������
		 *	@Para    : [in]		nLength����д������ݵĳ���
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool WriteData(unsigned char* pData, unsigned int nLength);

		/***********************************************
		 *  @Name    : CSerialBase::WriteString
		 *  @Descrip : �򴮿�д��һ���ַ���
		 *  @Para    : [in]		strSendData����д����ַ���
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool WriteString(std::string strSendData);

		/***********************************************
		 *  @Name    : CSerialBase::GetBytesInCOM
		 *  @Descrip : ��ȡ���ڻ�������ǰ���ݳ���
		 *  @Para    : None
		 *  @Return  : UINT ������
		 *  @Notes   : None
		 ***********************************************/
		UINT GetBytesInCOM();

		/***********************************************
		 *  @Name    : CSerialBase::ReadChar
		 *  @Descrip : �Ӵ��ڻ�������ȡһ���ַ�
		 *	@Para    : [in/out]	cRecved����ȡ�����ַ�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ReadChar(unsigned char &cRecved);

	private:
		/***********************************************
		 *  @Name    : CSerialBase::OpenPort
		 *  @Descrip : �򿪴���
		 *  @Para    : [in]		unPortNO�����ں�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenPort(UINT  unPortNO);

		/***********************************************
		 *  @Name    : CSerialBase::ClosePort
		 *  @Descrip : �رմ���
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		void ClosePort();

		/***********************************************
		 *  @Name    : CSerialBase::ListenThread
		 *  @Descrip : ���ڼ����߳�
		 *  @Para    : None	
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		static UINT WINAPI ListenThread(void* pParam);

		/***********************************************
		 *  @Name    : CSerialBase::CommunicationDeCode
		 *  @Descrip : ����ͨ������֡�����麯�����ɴ���ͨ���าд
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		virtual bool CommunicationDeCode();

		/***********************************************
		 *  @Name    : CSerialBase::TableDecode
		 *  @Descrip : ��̨��������֡�����麯�����ɴ���ͨ���าд
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		virtual bool TableDecode();

		/***********************************************
		 *  @Name    : CSerialBase::ECode
		 *  @Descrip : ���ڷ������ݱ����麯��
		 *  @Para    : None	
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		virtual bool ECode();

	public:
		std::vector<BYTE>			m_vucReceiveBuff;		//����֡���ջ�����
		bool                        m_bBusBusyFlag;         //�������߷�æ��־λ
	private:
		BYTE						m_ucTail;				//֡β
		int							m_nFrameLength;			//֡��
		bool						m_bTailCtrolEnd;		//��֡β��������֡�������

		HANDLE						m_hCOM;					//���ھ��
		static bool					m_sbExit;				//�߳��˳���־
		volatile HANDLE				m_hListenThread;		//�߳̾��
		CRITICAL_SECTION			m_csCommunicationSync;	//�ٽ���
	};
}

#endif // !SERIAL_BASE_H
