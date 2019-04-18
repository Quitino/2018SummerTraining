/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�˺�
 *
 *  @File    : Scohe.h
 *  @Brief   : ����ˮ����Ŀ�����ֵļ���
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
	//Ư������Ϣ�ṹ��
	typedef struct FloaterMsg
	{
		SSaliencyRegion m_Floater;

		float			m_fHoriAngleViaRobot;	//Ư����ˮƽ������Դ������ߵ�ƽ�Ƽнǣ���Ӧ����ˮƽ����
		float			m_fVertAngleViaRobot;	//Ư���︩��������Դ������˵ĸ����нǣ���Ӧ���ĸ�������
		float			m_fDistanceViaRobot;	//Ư���︩��������������˰�װ���ľ���
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

		CSerialCommunication*		pCommunication;			//����ͨ��
		CSerialTableControl*		pTableControl;			//��̨����
		
		CSaliency					Saliency;				//��������ȡ
		//CClassifierMLP			Classifier;             //������
		CServer*                    pServer;				//����ͨ��

		STableCruisePara			m_TableCruisePara;		//��̨Ѳ������
		std::vector<STableLocPoint> m_vTableLocPoint;		//��̨�Ķ�λ��
		STableLocLimit				m_TableLimit;			//��̨��λ����

		float						m_fRobotHeightViaWater;	//�����ˮ��߶ȣ���λ��m
		float						m_fVertThd2CamVC;		//�������Ұ�غ�ʱ��̨�ĸ�����
	};
}

#endif // !SCOHE_MAIN_H_
