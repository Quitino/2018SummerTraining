/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,邓豪
 *
 *  @File    : Tool.h
 *  @Brief   : 一些工具函数的实现
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
	 *  @Descrip : 得到当前系统时间，格式如5.352秒，精确到毫秒，具体到秒
	 *  @Para    : [in]		none
	 *	@Para    : [in/out]	timestamp：时间戳
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool GetCurrentSystemTime(double &timestamp);

	/***********************************************
	 *  @Name    : GetCurrentLocalTime
	 *  @Descrip : 按照给定格式输出当前系统时间
	 *  @Para    : [in]		std::string strFormat：给定格式
	 *	@Para    : [in/out]	strLocalTime：输出的时间
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool GetCurrentLocalTime(std::string &strLocalTime, const std::string strFormat = "%Y%m%d%H%M%S");

	/***********************************************
	 *  @Name    : GetCurrentSystemTimeForNameFile
	 *  @Descrip : 得到当前系统时间，并以字符串格式输出，用以对即时文件进行命名
	 *  @Para    : [in]		bAppend：是否需要文件后缀名
	 *  @Para    : [in]		strAppend：后缀名格式
	 *	@Para    : [in/out]	strCurrentTime：输出的字符串格式的时间戳
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool GetCurrentSystemTimeForNameFile(std::string &strCurrentTime, std::string strAppend, bool bAppend);

	/***********************************************
	 *  @Name    : GetProcessIdFromName
	 *  @Descrip : 获取需要监听的系统进程的ID号
	 *  @Para    : [in]		strProcessName：进程名字
	 *  @Return  : 进程ID
	 *  @Notes   : None
	 ***********************************************/
	DWORD GetProcessIdFromName(const std::string strProcessName);

	bool SaveImage(cv::Mat InputImage, std::string strSavePath);
}

#endif