/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�˺�
 *
 *  @File    : SerialComunication.h
 *  @Brief   : ����ͨ�ŵľ���ʵ��
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
		 *  @Descrip : ���캯��
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : ��Ҫ��֡βǿ��ת��ΪBYTE�ṹ����Ȼ���ʼ��������
		 ***********************************************/
		CSerialCommunication() :CSerialBase(static_cast<BYTE>(0xDD)), m_bSendFlag(false){};

		/***********************************************
		 *  @Name    : CSerialCommunication::~CSerialCommunication
		 *  @Descrip : ��������
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialCommunication(){};

		/***********************************************
		 *  @Name    : CSerialCommunication::ECode
		 *  @Descrip : ������鷢������֡�ķ�װʵ��
		 *  @Para    : [in]		nAngel���ǶȻ���Ҫ���͵ĵ�һ������
		 *  @Para    : [in]		nDistance���������Ҫ���͵ĵڶ�������
		 *	@Para    : [in/out]	v_ucData����װ�õ�����֡
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ECode(int nAngel, int nDistance, std::vector<unsigned char> &v_ucData);

		/***********************************************
		 *  @Name    : CSerialCommunication::ECode4Brush
		 *  @Descrip : ������ˢ�ӵĿ������ݷ�֡ʵ��
		 *  @Para    : [in]		Switch������ 80Ϊ�� 0Ϊ��
		 *	@Para    : [in/out]	v_ucData����װ�õ�����֡
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ECode4Brush(const int Switch, std::vector<unsigned char> &v_ucData);
	private:
		/***********************************************
		 *  @Name    : CSerialCommunication::CountCRC
		 *  @Descrip : 16λѭ��У��CRC�ľ���ʵ��
		 *  @Para    : [in]		v_ucData������CRC������
		 *  @Para    : [in]		nLength�����������ǰ�����λ���ڼ���CRC
		 *	@Para    : [in/out]	usCRC���õ���CRCУ����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CountCRC(std::vector<unsigned char>v_ucData, int nLength, unsigned short &usCRC);
		
		/***********************************************
		 *  @Name    : CSerialCommunication::CommunicationDeCode
		 *  @Descrip : ��д�����еĴ���ͨ������֡����ʵ��
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CommunicationDeCode();


	public:
		bool                        m_bSendFlag;		    //��ǰ״̬�Ƿ���Է���������������
	};

	/***********************************************
	 *  @Name    : SerialCommunicationTest
	 *  @Descrip : ��Ԫ����
	 *  @Para    : None
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool SerialCommunicationTest();
}

#endif // !SERIAL_COMMUNICATION_H_
