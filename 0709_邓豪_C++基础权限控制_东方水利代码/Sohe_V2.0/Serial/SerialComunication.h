/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,邓豪
 *
 *  @File    : SerialComunication.h
 *  @Brief   : 串口通信的具体实现
 *
 *  @Version : V1.0
 *  @Date    : 2018/1/23
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/
#ifndef SERIAL_COMMUNICATION_H_
#define SERIAL_COMMUNICATION_H_

#include "SerialBase.h"

namespace IPSG
{
	class CSerialCommunication :public CSerialBase
	{
	public:
		/***********************************************
		 *  @Name    : CSerialCommunication::CSerialCommunication
		 *  @Descrip : 构造函数
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : 需要将帧尾强制转化为BYTE结构，不然会初始化成整形
		 ***********************************************/
		CSerialCommunication() :CSerialBase(static_cast<BYTE>(0xDD)), m_bSendFlag(false){};

		/***********************************************
		 *  @Name    : CSerialCommunication::~CSerialCommunication
		 *  @Descrip : 析构函数
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialCommunication(){};

		/***********************************************
		 *  @Name    : CSerialCommunication::ECode
		 *  @Descrip : 向控制组发送数据帧的封装实现
		 *  @Para    : [in]		nAngel：角度或需要发送的第一个数据
		 *  @Para    : [in]		nDistance：距离或需要发送的第二个数据
		 *	@Para    : [in/out]	v_ucData：封装好的数据帧
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ECode(int nAngel, int nDistance, std::vector<unsigned char> &v_ucData);

		/***********************************************
		 *  @Name    : CSerialCommunication::ECode4Brush
		 *  @Descrip : 拖链与刷子的控制数据封帧实现
		 *  @Para    : [in]		Switch：开关 80为开 0为关
		 *	@Para    : [in/out]	v_ucData：封装好的数据帧
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ECode4Brush(const int Switch, std::vector<unsigned char> &v_ucData);
	private:
		/***********************************************
		 *  @Name    : CSerialCommunication::CountCRC
		 *  @Descrip : 16位循环校验CRC的具体实现
		 *  @Para    : [in]		v_ucData：计算CRC的数组
		 *  @Para    : [in]		nLength：输入的数据前面多少位用于计算CRC
		 *	@Para    : [in/out]	usCRC：得到的CRC校验码
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CountCRC(std::vector<unsigned char>v_ucData, int nLength, unsigned short &usCRC);
		
		/***********************************************
		 *  @Name    : CSerialCommunication::CommunicationDeCode
		 *  @Descrip : 覆写基类中的串口通信数据帧解码实现
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CommunicationDeCode();


	public:
		bool                        m_bSendFlag;		    //当前状态是否可以发送数据至控制组
	};

	/***********************************************
	 *  @Name    : SerialCommunicationTest
	 *  @Descrip : 单元测试
	 *  @Para    : None
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool SerialCommunicationTest();
}

#endif // !SERIAL_COMMUNICATION_H_
