/***********************************************
 *  Copyright Notice
 *  Copyright (c) 2017,Hughie
 *
 *  @File    : CircleDetect.h
 *  @Brief   :
 *
 *  @Version : V1.0
 *  @Data    : 2017/8/13
 
 *	@History :
 *    Author : Hughie
 *    Descrip: Creat File
 ***********************************************/

#ifndef CIRCLE_DETECT_H_
#define CIRCLE_DETECT_H_

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "logging/log.h"
#include "DataProcess.h"

#if defined(WIN32) || defined(_WIN32)
#include "SerialPort.h"
#else 
#include "ManiFold_UART1.hpp"
#endif

namespace IPSG
{
	//������ȡ���е���������Ľṹ��
	typedef struct BallPoint
	{
		float		m_fDistance;
		cv::Point2f	m_Point1;
		cv::Point2f m_Point2;
	}SBall;

	//���ڼ������е�ͶƱ�Ľṹ��
	typedef struct DoubleBall
	{
		cv::Point2f m_Point1;
		cv::Point2f m_Point2;

		int			m_nVote1;
		int			m_nVote2;
	}SDoubleBall;

	class CCircleDetect
	{
	public:

		/***********************************************
		 *  @Name    : CCircleDetect::CCircleDetect
		 *  @Descrip : ��CCircleDetect��Ա�������г�ʼ��
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CCircleDetect() :m_nBinaryThreshold(80), m_nContourSizeLow(13), m_nContourSizeHigh(60), m_nDistanceThreshold(10),
			m_fRateXLeft(0.12), m_fRateXRight(0.81), m_fRateYTop(0.0), m_fRateYDown(0.87), m_nInitFrameNO(15){};
		
		/***********************************************
		 *  @Name    : CCircleDetect::DetectContour
		 *  @Descrip : ��ȡ����ͼ�������������ͼ�ο���Ϊ��ͨ����ɫ���ߵ�ͨ���Ҷ�ͼ
		 *  @Para	 : [in]		InputImage������ͼ��
		 *	@Para    : [in/out]	vecContours���õ��Ĺ�������ͼ�����������
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool DetectContour(const cv::Mat InputImage, std::vector<std::vector<cv::Point2f>> &vecContours);
		
		/***********************************************
		 *  @Name    : CCircleDetect::DetectCircle
		 *  @Descrip : �������������ȡԲ������
		 *  @Para    : [in]		InputImage������ͼ������ȷ��ͼ����ƵȵĴ�С����ȵ�����
		 *  @Para    : [in]		vecContours���������������
		 *	@Para    : [in/out]	PointCenter���õ���Բ����������
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool DetectCircle(const cv::Mat InputImage, const std::vector<std::vector<cv::Point2f>> vecContours, std::vector<cv::Point2f> &PointCenter);
		
		/***********************************************
		 *  @Name    : CCircleDetect::DetectBall
		 *  @Descrip : ��ȡС������겢��ȡ��֮����ĵ�����꣬�Լ�����֮���ŷ�Ͼ���
		 *  @Para    : [in]		PointCenter�������Բ����������
		 *	@Para    : [in/out]	BallCenter��С�����������
		 *	@Para    : [in/out] BallNearestCenter����С����������������
		 *	@Para    : [in/out] fNearestDistance��С���������֮���ŷ�Ͼ��루�����ĵ�֮�䣩
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool DetectBall(const std::vector<cv::Point2f> PointCenter, cv::Point2f &BallCenter, cv::Point2f &BallNearestCenter, float &fNearestDistance);
		
		/***********************************************
		 *  @Name    : CCircleDetect::ProcessFrame
		 *  @Descrip : ��ͼ��ɼ�����������������̣������ʹ��������ȡ���̣�����ֱ��ʹ�øú���
		 *  @Para    : [in/out] strOutputData������õ�����Ҫ���͵����ڵ�����		
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ProcessFrame();

	private:
		/***********************************************
		 *  @Name    : CCircleDetect::ScreenLog
		 *  @Descrip : ����ʾͼ���ϴ�ӡ�����ʾ��Ϣ
		 *  @Para    : [in]		DrawImage��Ҫ��ӡ��ʾ��Ϣ����ʾͼ��
		 *  @Para	 : [in]		strText��Ҫ��ӡ���ı�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ScreenLog(cv::Mat DrawImage, std::string strText);
	private:
		int		m_nBinaryThreshold;		//ͼ���ֵ������ֵ
		int		m_nContourSizeLow;		//������С����ֵ
		int		m_nContourSizeHigh;		//������С����ֵ
		int		m_nDistanceThreshold;	//������ֵ������ͶƱʱ�ж����������ԣ���ֵΪ����������ֵ
		float	m_fRateXLeft;			//ͼ������ȡ�ٷֱ�
		float	m_fRateXRight;			//ͼ���Ҳ��ȡ�ٷֱ�
		float	m_fRateYTop;			//ͼ���Ϸ���ȡ�ٷֱ�
		float	m_fRateYDown;			//ͼ���·���ȡ�ٷֱ�
		int		m_nInitFrameNO;			//ǰ����֡ͼ�����ڳ�ʼ��
	};
}

#endif // !CIRCLE_DETECT_H_
