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
#define  DataSendSuccess               50  //2的ASCII值
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
		*  @Descrip : 网络通信初始化
		*  @Para    : portNo：端口号
		*  @Para    ：ipadress：本机IP地址
		*  @Return  : 返回false则表示网络初始化失败，此时检查本机ip地址是否填写正确，放回true则表示网络初始化成功
		*  @Notes   : None
		***********************************************/
		bool ServerInit(int portNo, const std::string strIPAddress);
		/***********************************************
		*  @Name    : CServer::SendData
		*  @Descrip : 发送字符给客服端
		*  @Para    : data：发送给客户端的字符串
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool SendData(const std::string strSendData);
		/***********************************************
		*  @Name    : CServer::OpenServerListenThread
		*  @Descrip : 打开服务器连接监听，服务器一直处于等待连接状态
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool OpenServerListenThread();
		/***********************************************
		*  @Name    : CServer::OpenReceiveDataListenThread
		*  @Descrip : 打开接受数据线程，保证随时能接受到客服端的数据
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool OpenReceiveDataListenThread();

	private:
		/***********************************************
		*  @Name    : CServer::ServerAccept
		*  @Descrip : 服务器线程
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		static UINT WINAPI ServerAccept(void* pParam);
		/***********************************************
		*  @Name    : CServer::CloseListenThread
		*  @Descrip : 关闭服务器线程
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool CloseListenThread();
		/***********************************************
		*  @Name    : CServer::ReceiveData
		*  @Descrip : 接受数据线程
		*  @Return  : None
		*  @Notes   : None
		***********************************************/

		static UINT WINAPI ReceiveData(void* pParam);
		/***********************************************
		*  @Name    : CServer::CloseReceiveDataListenThread
		*  @Descrip : 关闭接受数据线程
		*  @Return  : None
		*  @Notes   : None
		***********************************************/
		bool CloseReceiveDataListenThread();

	public:
		bool				m_bCruise;			//客户端巡航控制，false：等待巡航 true：开始巡航
		bool				m_bShutDown;		//关机控制（未使用）

	private:
		SOCKET				m_socClient;		//与客户端通信套接字
		SOCKET				m_socListen;		//连接监听套接字

		volatile HANDLE		m_hServerConnect;	//网络连接监听线程句柄
		volatile HANDLE		m_hDataReceive;		//数据接收监听线程句柄
		CRITICAL_SECTION	m_csServer;			//网络通信临界区
	};
}

#endif