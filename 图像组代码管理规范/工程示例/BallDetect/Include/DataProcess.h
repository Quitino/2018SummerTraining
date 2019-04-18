/***********************************************
 *  Copyright Notice
 *  Copyright (c) 2017,Hughie
 *
 *  @File    : DataProcess.h
 *  @Brief   : 对串口发送的数据帧进行处理的工具函数集
 *
 *  @Version : V1.0
 *  @Data    : 2017/8/13
 
 *	@History :
 *    Author : Hughie
 *    Descrip: Creat File
 ***********************************************/

#ifndef DATA_PROCESS_H_
#define DATA_PROCESS_H_

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace IPSG
{
	/***********************************************
	 *  @Name    : PointsVector2String
	 *  @Descrip : 对输入的坐标点容器转化为字符串数据
	 *  @Para    : [in]		vecPoints：输入的坐标点容器
	 *	@Para    : [in/out]	strData：处理得到的字符串数据
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool PointsVector2String(const std::vector<cv::Point2f> vecPoints, std::string & strData);

	/***********************************************
	 *  @Name    : Point2String
	 *  @Descrip : 对单个坐标点数据处理成字符串数据
	 *  @Para    : [in]		Points：输入的坐标点
	 *	@Para    : [in/out]	strData：处理得到的字符串数据
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : 输入的是单个坐标点，但是变量名最好不要取Point，cv里面有这个数据结构，部分编译器要报错
	 ***********************************************/
	bool Point2String(const cv::Point2f Points, std::string &strData);

	/***********************************************
	 *  @Name    : IsThreeDigital
	 *  @Descrip : 判断输入的整数是否为三位数
	 *  @Para    : [in]		nNumber：输入的整形数据
	 *  @Return  : bool true:yes false:no
	 *  @Notes   : None
	 ***********************************************/
	bool IsThreeDigital(const int nNumber);

	/***********************************************
	 *  @Name    : IsTwoDigital
	 *  @Descrip : 判断输入的整数是否为两位数
	 *  @Para    : [in]		nNumber：输入的整形数据
	 *  @Return  : bool true:yes false:no
	 *  @Notes   : None
	 ***********************************************/
	bool IsTwoDigital(const int nNumber);

	/***********************************************
	 *  @Name    : AddZero
	 *  @Descrip : 某些数据帧发送时需要固定宽度，可以在末尾添加字符"0"
	 *  @Para    : [in]		nAddNO：需要添加的个数
	 *	@Para    : [in/out]	strData：处理得到的字符串数据
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool AddZero(const int nAddNO, std::string &strData);
}

#endif // DATA_PROCESS_H_
