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
	//用于求取所有点两两距离的结构体
	typedef struct BallPoint
	{
		float		m_fDistance;
		cv::Point2f	m_Point1;
		cv::Point2f m_Point2;
	}SBall;

	//用于计算所有点投票的结构体
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
		 *  @Descrip : 对CCircleDetect成员变量进行初始化
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CCircleDetect() :m_nBinaryThreshold(80), m_nContourSizeLow(13), m_nContourSizeHigh(60), m_nDistanceThreshold(10),
			m_fRateXLeft(0.12), m_fRateXRight(0.81), m_fRateYTop(0.0), m_fRateYDown(0.87), m_nInitFrameNO(15){};
		
		/***********************************************
		 *  @Name    : CCircleDetect::DetectContour
		 *  @Descrip : 提取输入图像的轮廓，输入图形可以为三通道彩色或者单通道灰度图
		 *  @Para	 : [in]		InputImage：输入图像
		 *	@Para    : [in/out]	vecContours：得到的关于输入图像的轮廓容器
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool DetectContour(const cv::Mat InputImage, std::vector<std::vector<cv::Point2f>> &vecContours);
		
		/***********************************************
		 *  @Name    : CCircleDetect::DetectCircle
		 *  @Descrip : 对输入的轮廓提取圆心坐标
		 *  @Para    : [in]		InputImage：输入图像，用于确定图像绘制等的大小及宽度等问题
		 *  @Para    : [in]		vecContours：输入的轮廓容器
		 *	@Para    : [in/out]	PointCenter：得到的圆心坐标容器
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool DetectCircle(const cv::Mat InputImage, const std::vector<std::vector<cv::Point2f>> vecContours, std::vector<cv::Point2f> &PointCenter);
		
		/***********************************************
		 *  @Name    : CCircleDetect::DetectBall
		 *  @Descrip : 提取小球的坐标并求取与之最近的点的坐标，以及他们之间的欧氏距离
		 *  @Para    : [in]		PointCenter：输入的圆心坐标容器
		 *	@Para    : [in/out]	BallCenter：小球的中心坐标
		 *	@Para    : [in/out] BallNearestCenter：离小球最近点的中心坐标
		 *	@Para    : [in/out] fNearestDistance：小球与最近点之间的欧氏距离（两中心点之间）
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool DetectBall(const std::vector<cv::Point2f> PointCenter, cv::Point2f &BallCenter, cv::Point2f &BallNearestCenter, float &fNearestDistance);
		
		/***********************************************
		 *  @Name    : CCircleDetect::ProcessFrame
		 *  @Descrip : 从图像采集到数据输出整个流程，如果不使用上述提取过程，可以直接使用该函数
		 *  @Para    : [in/out] strOutputData：处理得到的需要发送到串口的数据		
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ProcessFrame();

	private:
		/***********************************************
		 *  @Name    : CCircleDetect::ScreenLog
		 *  @Descrip : 在显示图像上打印相关提示信息
		 *  @Para    : [in]		DrawImage：要打印提示信息的显示图像
		 *  @Para	 : [in]		strText：要打印的文本
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ScreenLog(cv::Mat DrawImage, std::string strText);
	private:
		int		m_nBinaryThreshold;		//图像二值化的阈值
		int		m_nContourSizeLow;		//轮廓大小下限值
		int		m_nContourSizeHigh;		//轮廓大小上限值
		int		m_nDistanceThreshold;	//距离阈值，用于投票时判定两点相似性，其值为横或纵坐标差值
		float	m_fRateXLeft;			//图像左侧截取百分比
		float	m_fRateXRight;			//图像右侧截取百分比
		float	m_fRateYTop;			//图像上方截取百分比
		float	m_fRateYDown;			//图像下方截取百分比
		int		m_nInitFrameNO;			//前多少帧图像用于初始化
	};
}

#endif // !CIRCLE_DETECT_H_
