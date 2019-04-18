/***********************************************
 *  Copyright Notice
 *  Copyright (c) 2017,Hughie
 *
 *  @File    : DataProcess.h
 *  @Brief   : �Դ��ڷ��͵�����֡���д���Ĺ��ߺ�����
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
	 *  @Descrip : ����������������ת��Ϊ�ַ�������
	 *  @Para    : [in]		vecPoints����������������
	 *	@Para    : [in/out]	strData������õ����ַ�������
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool PointsVector2String(const std::vector<cv::Point2f> vecPoints, std::string & strData);

	/***********************************************
	 *  @Name    : Point2String
	 *  @Descrip : �Ե�����������ݴ�����ַ�������
	 *  @Para    : [in]		Points������������
	 *	@Para    : [in/out]	strData������õ����ַ�������
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : ������ǵ�������㣬���Ǳ�������ò�ҪȡPoint��cv������������ݽṹ�����ֱ�����Ҫ����
	 ***********************************************/
	bool Point2String(const cv::Point2f Points, std::string &strData);

	/***********************************************
	 *  @Name    : IsThreeDigital
	 *  @Descrip : �ж�����������Ƿ�Ϊ��λ��
	 *  @Para    : [in]		nNumber���������������
	 *  @Return  : bool true:yes false:no
	 *  @Notes   : None
	 ***********************************************/
	bool IsThreeDigital(const int nNumber);

	/***********************************************
	 *  @Name    : IsTwoDigital
	 *  @Descrip : �ж�����������Ƿ�Ϊ��λ��
	 *  @Para    : [in]		nNumber���������������
	 *  @Return  : bool true:yes false:no
	 *  @Notes   : None
	 ***********************************************/
	bool IsTwoDigital(const int nNumber);

	/***********************************************
	 *  @Name    : AddZero
	 *  @Descrip : ĳЩ����֡����ʱ��Ҫ�̶���ȣ�������ĩβ����ַ�"0"
	 *  @Para    : [in]		nAddNO����Ҫ��ӵĸ���
	 *	@Para    : [in/out]	strData������õ����ַ�������
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool AddZero(const int nAddNO, std::string &strData);
}

#endif // DATA_PROCESS_H_
