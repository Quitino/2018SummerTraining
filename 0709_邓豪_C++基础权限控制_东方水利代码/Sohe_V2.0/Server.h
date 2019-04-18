#ifndef SOCKET_SERVER_H_
#define SOCKET_SERVER_H_

#pragma comment(lib,"ws2_32.lib") 
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <cstdlib>

#include "log.h"

#define  GainControl                   0
#define  ReleaseControl                1
#define  DataSendSuccess               50  //2��ASCIIֵ
#define  SHUTDOWN                      51

namespace IPSG
{
	class CServer
	{
	public:
		CServer();
		~CServer();

		/***********************************************
		*  @Name    : CServer::ServerInit
		*  @Descrip : ����ͨ�ų�ʼ��
		*  @Para    : portNo���˿ں�
		*  @Para    ��ipadress������IP��ַ
		*  @Return  : ����false���ʾ�����ʼ��ʧ�ܣ���ʱ��鱾��ip��ַ�Ƿ���д��ȷ���Ż�true���ʾ�����ʼ���ɹ�
		*  @Notes   : None
		***********************************************/
		bool ServerInit(int portNo, const std::string strIPAddress);
		/***********************************************
		*  @Name    : CServer::SendData
		*  @Descrip : �����ַ����ͷ���
		*  @Para    : data�����͸��ͻ��˵��ַ���
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool SendData(const std::string strSendData);
		/***********************************************
		*  @Name    : CServer::OpenServerListenThread
		*  @Descrip : �򿪷��������Ӽ�����������һֱ���ڵȴ�����״̬
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool OpenServerListenThread();
		/***********************************************
		*  @Name    : CServer::OpenReceiveDataListenThread
		*  @Descrip : �򿪽��������̣߳���֤��ʱ�ܽ��ܵ��ͷ��˵�����
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool OpenReceiveDataListenThread();

	private:
		/***********************************************
		*  @Name    : CServer::ServerAccept
		*  @Descrip : �������߳�
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		static UINT WINAPI ServerAccept(void* pParam);
		/***********************************************
		*  @Name    : CServer::CloseListenThread
		*  @Descrip : �رշ������߳�
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool CloseListenThread();
		/***********************************************
		*  @Name    : CServer::ReceiveData
		*  @Descrip : ���������߳�
		*  @Return  : None
		*  @Notes   : None
		***********************************************/

		static UINT WINAPI ReceiveData(void* pParam);
		/***********************************************
		*  @Name    : CServer::CloseReceiveDataListenThread
		*  @Descrip : �رս��������߳�
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool CloseReceiveDataListenThread();

	public:
		bool				m_bCruise;			//�ͻ���Ѳ�����ƣ�false���ȴ�Ѳ�� true����ʼѲ��
		bool				m_bShutDown;		//�ػ����ƣ�δʹ�ã�

	private:
		SOCKET				m_socClient;		//��ͻ���ͨ���׽���
		SOCKET				m_socListen;		//���Ӽ����׽���

		volatile HANDLE		m_hServerConnect;	//�������Ӽ����߳̾��
		volatile HANDLE		m_hDataReceive;		//���ݽ��ռ����߳̾��
		CRITICAL_SECTION	m_csServer;			//����ͨ���ٽ���
	};
}

#endif