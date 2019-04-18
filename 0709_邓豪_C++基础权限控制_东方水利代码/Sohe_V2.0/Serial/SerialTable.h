/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,邓豪
 *
 *  @File    : SerialTable.h
 *  @Brief   : 云台控制类的具体实现
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
	//云台定位点的数据结构体
	typedef struct TableLocPoint
	{
		float	m_fHoriLocPoint;		//水平定位点
		float	m_fVertLocPoint;		//俯仰定位点

		//结构体构造函数
		TableLocPoint(float fHoriLocPoint = 125.0, float fVertLocPoint = 40.0) :
			m_fHoriLocPoint(fHoriLocPoint), m_fVertLocPoint(fVertLocPoint){};
	}STableLocPoint;

	//云台巡航参数
	typedef struct TableCruisePara
	{
		int		m_nTableHoriPointNO;	//云台水平方向定位点个数
		int		m_nTableVertPointNO;	//云台垂直方向定位点个数
		int		m_nTableHoriAngleRange;	//云台水平方向工作范围
		int		m_nTableVertAngleRange;	//云台垂直方向工作范围
		int		m_nTableHoriInitLoc;	//云台水平初始位置
		int		m_nTableVertInitLoc;	//云台俯仰初始位置
		int		m_nTableLocHoriStep;	//水平方向定位步长
		int		m_nTableLocVertStep;	//垂直方向定位步长

		//结构体构造函数
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
		float	m_fTableLeftThd;		//云台左限位
		float	m_fTableRightThd;		//云台右限位
		float	m_fTableTopThd;			//云台上仰限位
		float	m_fTableDownThd4Cruise;	//云台巡航时的下俯限位
		float	m_fTableDownThd4Track;	//云台跟随时的下俯限位

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
		 *  @Descrip : 构造函数
		 *  @Para    : None	
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CSerialTableControl() :CSerialBase(static_cast<int>(7)), m_bHorizonAboveVertical(true){};

		/***********************************************
		 *  @Name    : CSerialTableControl::~CSerialTableControl
		 *  @Descrip : 析构函数
		 *  @Para    : None
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CSerialTableControl();

		/***********************************************
		 *  @Name    : CSerialTableControl::OpenAuxN
		 *  @Descrip : 打开第N号辅助开关
		 *  @Para    : [in]		nAuxNO：辅助开关号数
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenAuxN(int nAuxNO);

		/***********************************************
		 *  @Name    : CSerialTableControl::CloseAuxN
		 *  @Descrip : 关闭第N号辅助开关
		 *  @Para    : [in]		nAuxNO：辅助开关号数
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CloseAuxN(int nAuxNO);

		/***********************************************
		 *  @Name    : CSerialTableControl::SetPresetN
		 *  @Descrip : 设置第N预置位
		 *  @Para    : [in]		nPresetNO：预置位号数
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool SetPresetN(int nPresetNO);

		/***********************************************
		*  @Name    : CSerialTableControl::ClearPresetN
		*  @Descrip : 删除第N预置位
		*  @Para    : [in]		nPresetNO：预置位号数
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool ClearPresetN(int nPresetNO);

		/***********************************************
		*  @Name    : CSerialTableControl::CallPresetN
		*  @Descrip : 调用第N预置位
		*  @Para    : [in]		nPresetNO：预置位号数
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool CallPresetN(int nPresetNO);

		/***********************************************
		 *  @Name    : CSerialTableControl::HoriLocate
		 *  @Descrip : 云台水平定位
		 *  @Para    : [in]		fLocAngle：水平定位角度
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : 范围：最左（95°）-最右（275°）
		 ***********************************************/
		bool HoriLocate(float fLocAngle);

		/***********************************************
		*  @Name    : CSerialTableControl::HoriLocate
		*  @Descrip : 云台俯仰定位
		*  @Para    : [in]		fLocAngle：俯仰定位角度
		*  @Return  : bool true:success false:failed
		*  @Notes   : 范围：最俯（60°）-水平向前（0°）
		***********************************************/
		bool VertLocate(float fLocAngle);

		/***********************************************
		 *  @Name    : CSerialTableControl::HVLocate
		 *  @Descrip : 水平、俯仰二维定位，即具体点定位
		 *  @Para    : [in]		fHLocAngle：水平定位点
		 *	@Para    : [in]		fVLocAngle：俯仰定位点
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool HVLocate(float fHLocAngle, float fVLocAngle);

		/***********************************************
		 *  @Name    : CSerialTableControl::SetLocSpeed
		 *  @Descrip : 设置定位速度
		 *  @Para    : [in]		nHSpeed：水平定位速度
		 *	@Para    : [in]		nVSpeed：俯仰定位速度
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : 范围：最小（0）-最大（63），量化后速度为：最小（0°/s）-最大（25°/s）
		 ***********************************************/
		bool SetLocSpeed(int nHSpeed, int nVSpeed);		

		/***********************************************
		 *  @Name    : CSerialTableControl::ResetTable
		 *  @Descrip : 将云台恢复出厂设置
		 *  @Para    : None
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ResetTable();

		/***********************************************
		 *  @Name    : CSerialTableControl::GetHoriAngle
		 *  @Descrip : 获取当前云台水平角度
		 *	@Para    : [in/out]	fHoriAngle：当前云台水平角度
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool GetHoriAngle(float &fHoriAngle);

		/***********************************************
		*  @Name    : CSerialTableControl::GetVertAngle
		*  @Descrip : 获取当前云台水平角度
		*  @Para    : [in/out]	fVertAngle：当前云台俯仰角度
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool GetVertAngle(float &fVertAngle);

		/***********************************************
		 *  @Name    : CSerialTableControl::GetLocSpeed
		 *  @Descrip : 获取当前云台定位速度
		 *  @Para    : [in/out]	nHSpeed：当前云台水平方向定位速度
		 *	@Para    : [in/out]	nVSpeed：当前云台俯仰方向定位速度
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : 范围：最小（0）-最大（63），量化后速度为：最小（0°/s）-最大（25°/s）
		 ***********************************************/
		bool GetLocSpeed(int &nHSpeed, int &nVSpeed);

		bool TableStop();

	private:
		/***********************************************
		 *  @Name    : CSerialTableControl::CalCheckSum
		 *  @Descrip : 计算校验码
		 *  @Para    : [in]		ucData：需要计算校验码的数据
		 *	@Para    : [in/out]	CheckSum：计算得到的校验码
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CalCheckSum(std::vector<BYTE> ucData, BYTE &CheckSum);

		/***********************************************
		 *  @Name    : CSerialTableControl::Int2UShort
		 *  @Descrip : 整形数据转化成两个字节的数据
		 *  @Para    : [in]		nIntNO：待转化的整形数据
		 *	@Para    : [in/out]	ucDataH：转化后的高位数据
		 *	@Para    : [in/out] ucDataL：转化后的低位数据
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool Int2UShort(const int nIntNO, BYTE &ucDataH, BYTE &ucDataL);

		/***********************************************
		 *  @Name    : CSerialTableControl::SerialWriteVector
		 *  @Descrip : 向串口写入一个vector
		 *  @Para    : [in]		v_ucSendData：待写入的vector数据
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool SerialWriteVector(std::vector<BYTE> v_ucSendData);

		/***********************************************
		 *  @Name    : CSerialTableControl::CalCurrentHoriAngle
		 *  @Descrip : 从云台返回的查询数据帧中计算当前云台水平角度
		 *  @Para    : [in]		ucData1：数据高位
		 *	@Para    : [in]		ucData2：数据低位
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CalCurrentHoriAngle(BYTE ucData1, BYTE ucData2);

		/***********************************************
		*  @Name    : CSerialTableControl::CalCurrentVertAngle
		*  @Descrip : 从云台返回的查询数据帧中计算当前云台俯仰角度
		*  @Para    : [in]		ucData1：数据高位
		*  @Para    : [in]		ucData2：数据低位
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool CalCurrentVertAngle(BYTE ucData1, BYTE ucData2);

		/***********************************************
		*  @Name    : CSerialTableControl::CalCurrentLocSpeed
		*  @Descrip : 从云台返回的查询数据帧中计算当前云台定位速度
		*  @Para    : [in]		ucData1：水平速度
		*  @Para    : [in]		ucData2：俯仰速度
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool CalCurrentLocSpeed(BYTE ucData1, BYTE ucData2);

		/***********************************************
		 *  @Name    : CSerialTableControl::ECode
		 *  @Descrip : 向云台发送查询数据帧的封装实现
		 *  @Para    : [in]		ucCommand2：查询内容
		 *  @Para    : [in]		ucData1：数据高位
		 *  @Para    : [in]		ucData2：数据低位
		 *	@Para    : [in/out]	v_ucEcodeData：封装后数据帧
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ECode(BYTE ucCommand2, BYTE ucData1, BYTE ucData2, std::vector<BYTE> &v_ucEcodeData);

		/***********************************************
		*  @Name    : CSerialTableControl::TableDecode
		*  @Descrip : 覆写基类中的云台返回的数据帧解码实现
		*  @Para    : None
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool TableDecode();

		/***********************************************
		 *  @Name    : CSerialTableControl::AskMessage
		 *  @Descrip : 发送查询数据帧
		 *  @Para    : [in]		ucCommand2：查询内容
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool AskMessage(const BYTE ucCommand2);

	private:
		bool	m_bHorizonAboveVertical;	//先水平后俯仰
		int		m_nInitHoriAngle;			//云台初始水平角度
		int		m_nInitVertAngle;			//初始云台俯仰角度
		
		float	m_fCurrentHoriAngle;		//当前云台水平角度
		float	m_fCurrentVertAngle;		//当前云台俯仰角度
		int		m_nCurrentHoriLocSpeed;		//当前水平方向定位速度
		int		m_nCurrentVertLocSpeed;		//当前俯仰方向定位速度

		//bool	m_bTableDataUpdated;		//云台回传数据更新状态位
	};

	/***********************************************
	 *  @Name    : TableTest
	 *  @Descrip : 单元测试
	 *  @Para    : None
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool TableTest();
	bool TableTestSimplePoint();
}

#endif // !SERIAL_TABLE_CONTRAL_H_
