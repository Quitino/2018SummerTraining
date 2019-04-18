/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,邓豪
 *
 *  @File    : Scohe.h
 *  @Brief   : 东方水利项目各部分的集成
 *
 *  @Version : V2.0
 *  @Date    : 2018/4/30
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/
#ifndef SCOHE_MAIN_H_
#define SCOHE_MAIN_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <functional>

#include "Server.h"
#include "HIKCamera.h"
#include "Serial/SerialComunication.h"
#include "Serial/SerialTable.h"
#include "Saliency.h"
#include "Classifier.h"
#include "ControlStrategy.h"
#include "Tool.h"

#define     PI			3.14159265
#define		BRUSH_OPEN	80
#define		BRUSH_CLOSE	0

namespace IPSG
{
	//漂浮物信息结构体
	typedef struct FloaterMsg
	{
		SSaliencyRegion m_Floater;

		float			m_fHoriAngleViaRobot;	//漂浮物水平方向相对船中心线的平移夹角，对应船的水平方向
		float			m_fVertAngleViaRobot;	//漂浮物俯仰方向相对船升降杆的俯仰夹角，对应船的俯仰方向
		float			m_fDistanceViaRobot;	//漂浮物俯仰方向相对升降杆安装处的距离
	}SFloaterMsg;

	class CScohe
	{
	public:
		CScohe() :m_fRobotHeightViaWater(2.8), m_fVertThd2CamVC(47.0)
		{
			pServer = new CServer;
		};
		~CScohe(){};

		bool ModelInital();
		bool Process();

	private:
		bool TableCruise();
		bool TableFollow(SFloaterMsg FollowFloater);

		bool CamTopImgProc(const float fCurrentHoriPoint, const float fCurrentVertPoint);

		bool SaveFloaterMsg(std::vector<SSaliencyRegion> vSaliencyRegion, float fCurrentHoriPoint, float fCurrentVertPoint, std::vector<SFloaterMsg>& vFloater);
		bool CamCertralAlign(SFloaterMsg FollowFloater, float fTableRotPara = 0.65);
		
		bool CtrlSgyDataSend(int nAngle4Send, int nDistance4Send);
		bool CtrlDataSend4Brush(int nSwitch);
	private:
		CHIKCamera					HIKCameraTop;
		//CHIKCamera				HIKCameraDown;

		CSerialCommunication*		pCommunication;			//串口通信
		CSerialTableControl*		pTableControl;			//云台控制
		
		CSaliency					Saliency;				//显著性提取
		//CClassifierMLP			Classifier;             //分类器
		CServer*                    pServer;				//网络通信

		STableCruisePara			m_TableCruisePara;		//云台巡航参数
		std::vector<STableLocPoint> m_vTableLocPoint;		//云台的定位点
		STableLocLimit				m_TableLimit;			//云台限位参数

		float						m_fRobotHeightViaWater;	//相机离水面高度，单位：m
		float						m_fVertThd2CamVC;		//两相机视野重合时云台的俯仰角
	};
}

#endif // !SCOHE_MAIN_H_
