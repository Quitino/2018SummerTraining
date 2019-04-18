/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�˺�
 *
 *  @File    : Tool.h
 *  @Brief   : һЩ���ߺ�����ʵ��
 *
 *  @Version : V1.0
 *  @Date    : 2018/1/23
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/
#ifndef SCOLE_TOOL_H_
#define SCOLE_TOOL_H_

#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <TlHelp32.h>
#include <comdef.h>

#include <opencv2/opencv.hpp>

#include "log.h"

namespace IPSG
{
	/***********************************************
	 *  @Name    : GetCurrentSystemTime
	 *  @Descrip : �õ���ǰϵͳʱ�䣬��ʽ��5.352�룬��ȷ�����룬���嵽��
	 *  @Para    : [in]		none
	 *	@Para    : [in/out]	timestamp��ʱ���
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool GetCurrentSystemTime(double &timestamp);

	/***********************************************
	 *  @Name    : GetCurrentLocalTime
	 *  @Descrip : ���ո�����ʽ�����ǰϵͳʱ��
	 *  @Para    : [in]		std::string strFormat��������ʽ
	 *	@Para    : [in/out]	strLocalTime�������ʱ��
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool GetCurrentLocalTime(std::string &strLocalTime, const std::string strFormat = "%Y%m%d%H%M%S");

	/***********************************************
	 *  @Name    : GetCurrentSystemTimeForNameFile
	 *  @Descrip : �õ���ǰϵͳʱ�䣬�����ַ�����ʽ��������ԶԼ�ʱ�ļ���������
	 *  @Para    : [in]		bAppend���Ƿ���Ҫ�ļ���׺��
	 *  @Para    : [in]		strAppend����׺����ʽ
	 *	@Para    : [in/out]	strCurrentTime��������ַ�����ʽ��ʱ���
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool GetCurrentSystemTimeForNameFile(std::string &strCurrentTime, std::string strAppend, bool bAppend);

	/***********************************************
	 *  @Name    : GetProcessIdFromName
	 *  @Descrip : ��ȡ��Ҫ������ϵͳ���̵�ID��
	 *  @Para    : [in]		strProcessName����������
	 *  @Return  : ����ID
	 *  @Notes   : None
	 ***********************************************/
	DWORD GetProcessIdFromName(const std::string strProcessName);

	bool SaveImage(cv::Mat InputImage, std::string strSavePath);
}

#endif