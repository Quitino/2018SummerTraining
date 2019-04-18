/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�˺�
 *
 *  @File    : SerialTable.h
 *  @Brief   : ��̨������ľ���ʵ��
 *
 *  @Version : V1.0
 *  @Date    : 2018/1/23
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/
#ifndef SERIAL_TABLE_CONTROL_H_
#define SERIAL_TABLE_CONTROL_H_

#include "SerialBase.h"
#include "SerialTableCommand.h"

namespace IPSG
{
	//��̨��λ������ݽṹ��
	typedef struct TableLocPoint
	{
		float	m_fHoriLocPoint;		//ˮƽ��λ��
		float	m_fVertLocPoint;		//������λ��

		//�ṹ�幹�캯��
		TableLocPoint(float fHoriLocPoint = 125.0, float fVertLocPoint = 40.0) :
			m_fHoriLocPoint(fHoriLocPoint), m_fVertLocPoint(fVertLocPoint){};
	}STableLocPoint;

	//��̨Ѳ������
	typedef struct TableCruisePara
	{
		int		m_nTableHoriPointNO;	//��̨ˮƽ����λ�����
		int		m_nTableVertPointNO;	//��̨��ֱ����λ�����
		int		m_nTableHoriAngleRange;	//��̨ˮƽ��������Χ
		int		m_nTableVertAngleRange;	//��̨��ֱ��������Χ
		int		m_nTableHoriInitLoc;	//��̨ˮƽ��ʼλ��
		int		m_nTableVertInitLoc;	//��̨������ʼλ��
		int		m_nTableLocHoriStep;	//ˮƽ����λ����
		int		m_nTableLocVertStep;	//��ֱ����λ����

		//�ṹ�幹�캯��
		TableCruisePara(int nTableHoriPointNO = 5, int nTableVertPointNO = 3,
			int nTableHoriAngleRange = 120, int nTableVertAngleRange = 20,
			int nTableHoriInitLoc = 126, int nTableVertInitLoc = 41,
			int nTableLocHoriStep = 30, int nTableLocVertStep = 10) :
			m_nTableHoriPointNO(nTableHoriPointNO), m_nTableVertPointNO(nTableVertPointNO),
			m_nTableHoriAngleRange(nTableHoriAngleRange), m_nTableVertAngleRange(nTableVertAngleRange),
			m_nTableHoriInitLoc(nTableHoriInitLoc), m_nTableVertInitLoc(nTableVertInitLoc),
			m_nTableLocHoriStep(nTableLocHoriStep), m_nTableLocVertStep(nTableLocVertStep){};
	}STableCruisePara;

	typedef struct TableLocLimit
	{
		float	m_fTableLeftThd;		//��̨����λ
		float	m_fTableRightThd;		//��̨����λ
		float	m_fTableTopThd;			//��̨������λ
		float	m_fTableDownThd4Cruise;	//��̨Ѳ��ʱ���¸���λ
		float	m_fTableDownThd4Track;	//��̨����ʱ���¸���λ

		TableLocLimit(float fLeftThd = 120.0, float fRightThd = 250.0, float fTopThd = 10.0,
			float fDownThd4Cruise = 40.0, float fDownThd4Track = 48.0) :
			m_fTableLeftThd(fLeftThd), m_fTableRightThd(fRightThd), m_fTableTopThd(fTopThd),
			m_fTableDownThd4Cruise(fDownThd4Cruise), m_fTableDownThd4Track(fDownThd4Track){};
	}STableLocLimit;

	class CSerialTableControl :public CSerialBase
	{
	public:
		/***********************************************
		 *  @Name    : CSerialTableControl::CSerialTableControl
		 *  @Descrip : ���캯��
		 *  @Para    : None	
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialTableControl() :CSerialBase(static_cast<int>(7)), m_bHorizonAboveVertical(true){};

		/***********************************************
		 *  @Name    : CSerialTableControl::~CSerialTableControl
		 *  @Descrip : ��������
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialTableControl();

		/***********************************************
		 *  @Name    : CSerialTableControl::OpenAuxN
		 *  @Descrip : �򿪵�N�Ÿ�������
		 *  @Para    : [in]		nAuxNO���������غ���
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenAuxN(int nAuxNO);

		/***********************************************
		 *  @Name    : CSerialTableControl::CloseAuxN
		 *  @Descrip : �رյ�N�Ÿ�������
		 *  @Para    : [in]		nAuxNO���������غ���
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CloseAuxN(int nAuxNO);

		/***********************************************
		 *  @Name    : CSerialTableControl::SetPresetN
		 *  @Descrip : ���õ�NԤ��λ
		 *  @Para    : [in]		nPresetNO��Ԥ��λ����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool SetPresetN(int nPresetNO);

		/***********************************************
		*  @Name    : CSerialTableControl::ClearPresetN
		*  @Descrip : ɾ����NԤ��λ
		*  @Para    : [in]		nPresetNO��Ԥ��λ����
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool ClearPresetN(int nPresetNO);

		/***********************************************
		*  @Name    : CSerialTableControl::CallPresetN
		*  @Descrip : ���õ�NԤ��λ
		*  @Para    : [in]		nPresetNO��Ԥ��λ����
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool CallPresetN(int nPresetNO);

		/***********************************************
		 *  @Name    : CSerialTableControl::HoriLocate
		 *  @Descrip : ��̨ˮƽ��λ
		 *  @Para    : [in]		fLocAngle��ˮƽ��λ�Ƕ�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : ��Χ������95�㣩-���ң�275�㣩
		 ***********************************************/
		bool HoriLocate(float fLocAngle);

		/***********************************************
		*  @Name    : CSerialTableControl::HoriLocate
		*  @Descrip : ��̨������λ
		*  @Para    : [in]		fLocAngle��������λ�Ƕ�
		*  @Return  : bool true:success false:failed
		*  @Notes   : ��Χ�����60�㣩-ˮƽ��ǰ��0�㣩
		***********************************************/
		bool VertLocate(float fLocAngle);

		/***********************************************
		 *  @Name    : CSerialTableControl::HVLocate
		 *  @Descrip : ˮƽ��������ά��λ��������㶨λ
		 *  @Para    : [in]		fHLocAngle��ˮƽ��λ��
		 *	@Para    : [in]		fVLocAngle��������λ��
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool HVLocate(float fHLocAngle, float fVLocAngle);

		/***********************************************
		 *  @Name    : CSerialTableControl::SetLocSpeed
		 *  @Descrip : ���ö�λ�ٶ�
		 *  @Para    : [in]		nHSpeed��ˮƽ��λ�ٶ�
		 *	@Para    : [in]		nVSpeed��������λ�ٶ�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : ��Χ����С��0��-���63�����������ٶ�Ϊ����С��0��/s��-���25��/s��
		 ***********************************************/
		bool SetLocSpeed(int nHSpeed, int nVSpeed);		

		/***********************************************
		 *  @Name    : CSerialTableControl::ResetTable
		 *  @Descrip : ����̨�ָ���������
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ResetTable();

		/***********************************************
		 *  @Name    : CSerialTableControl::GetHoriAngle
		 *  @Descrip : ��ȡ��ǰ��̨ˮƽ�Ƕ�
		 *	@Para    : [in/out]	fHoriAngle����ǰ��̨ˮƽ�Ƕ�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool GetHoriAngle(float &fHoriAngle);

		/***********************************************
		*  @Name    : CSerialTableControl::GetVertAngle
		*  @Descrip : ��ȡ��ǰ��̨ˮƽ�Ƕ�
		*  @Para    : [in/out]	fVertAngle����ǰ��̨�����Ƕ�
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool GetVertAngle(float &fVertAngle);

		/***********************************************
		 *  @Name    : CSerialTableControl::GetLocSpeed
		 *  @Descrip : ��ȡ��ǰ��̨��λ�ٶ�
		 *  @Para    : [in/out]	nHSpeed����ǰ��̨ˮƽ����λ�ٶ�
		 *	@Para    : [in/out]	nVSpeed����ǰ��̨��������λ�ٶ�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : ��Χ����С��0��-���63�����������ٶ�Ϊ����С��0��/s��-���25��/s��
		 ***********************************************/
		bool GetLocSpeed(int &nHSpeed, int &nVSpeed);

		bool TableStop();

	private:
		/***********************************************
		 *  @Name    : CSerialTableControl::CalCheckSum
		 *  @Descrip : ����У����
		 *  @Para    : [in]		ucData����Ҫ����У���������
		 *	@Para    : [in/out]	CheckSum������õ���У����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CalCheckSum(std::vector<BYTE> ucData, BYTE &CheckSum);

		/***********************************************
		 *  @Name    : CSerialTableControl::Int2UShort
		 *  @Descrip : ��������ת���������ֽڵ�����
		 *  @Para    : [in]		nIntNO����ת������������
		 *	@Para    : [in/out]	ucDataH��ת����ĸ�λ����
		 *	@Para    : [in/out] ucDataL��ת����ĵ�λ����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool Int2UShort(const int nIntNO, BYTE &ucDataH, BYTE &ucDataL);

		/***********************************************
		 *  @Name    : CSerialTableControl::SerialWriteVector
		 *  @Descrip : �򴮿�д��һ��vector
		 *  @Para    : [in]		v_ucSendData����д���vector����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool SerialWriteVector(std::vector<BYTE> v_ucSendData);

		/***********************************************
		 *  @Name    : CSerialTableControl::CalCurrentHoriAngle
		 *  @Descrip : ����̨���صĲ�ѯ����֡�м��㵱ǰ��̨ˮƽ�Ƕ�
		 *  @Para    : [in]		ucData1�����ݸ�λ
		 *	@Para    : [in]		ucData2�����ݵ�λ
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CalCurrentHoriAngle(BYTE ucData1, BYTE ucData2);

		/***********************************************
		*  @Name    : CSerialTableControl::CalCurrentVertAngle
		*  @Descrip : ����̨���صĲ�ѯ����֡�м��㵱ǰ��̨�����Ƕ�
		*  @Para    : [in]		ucData1�����ݸ�λ
		*  @Para    : [in]		ucData2�����ݵ�λ
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool CalCurrentVertAngle(BYTE ucData1, BYTE ucData2);

		/***********************************************
		*  @Name    : CSerialTableControl::CalCurrentLocSpeed
		*  @Descrip : ����̨���صĲ�ѯ����֡�м��㵱ǰ��̨��λ�ٶ�
		*  @Para    : [in]		ucData1��ˮƽ�ٶ�
		*  @Para    : [in]		ucData2�������ٶ�
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool CalCurrentLocSpeed(BYTE ucData1, BYTE ucData2);

		/***********************************************
		 *  @Name    : CSerialTableControl::ECode
		 *  @Descrip : ����̨���Ͳ�ѯ����֡�ķ�װʵ��
		 *  @Para    : [in]		ucCommand2����ѯ����
		 *  @Para    : [in]		ucData1�����ݸ�λ
		 *  @Para    : [in]		ucData2�����ݵ�λ
		 *	@Para    : [in/out]	v_ucEcodeData����װ������֡
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ECode(BYTE ucCommand2, BYTE ucData1, BYTE ucData2, std::vector<BYTE> &v_ucEcodeData);

		/***********************************************
		*  @Name    : CSerialTableControl::TableDecode
		*  @Descrip : ��д�����е���̨���ص�����֡����ʵ��
		*  @Para    : None
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool TableDecode();

		/***********************************************
		 *  @Name    : CSerialTableControl::AskMessage
		 *  @Descrip : ���Ͳ�ѯ����֡
		 *  @Para    : [in]		ucCommand2����ѯ����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool AskMessage(const BYTE ucCommand2);

	private:
		bool	m_bHorizonAboveVertical;	//��ˮƽ����
		int		m_nInitHoriAngle;			//��̨��ʼˮƽ�Ƕ�
		int		m_nInitVertAngle;			//��ʼ��̨�����Ƕ�
		
		float	m_fCurrentHoriAngle;		//��ǰ��̨ˮƽ�Ƕ�
		float	m_fCurrentVertAngle;		//��ǰ��̨�����Ƕ�
		int		m_nCurrentHoriLocSpeed;		//��ǰˮƽ����λ�ٶ�
		int		m_nCurrentVertLocSpeed;		//��ǰ��������λ�ٶ�

		//bool	m_bTableDataUpdated;		//��̨�ش����ݸ���״̬λ
	};

	/***********************************************
	 *  @Name    : TableTest
	 *  @Descrip : ��Ԫ����
	 *  @Para    : None
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool TableTest();
	bool TableTestSimplePoint();
}

#endif // !SERIAL_TABLE_CONTRAL_H_
