/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,邓豪
 *
 *  @File    : HIKCamera.h
 *  @Brief   : 海康相机的图像获取类，适用于大华等支持rtsp协议的网络监控相机
 *
 *  @Version : V1.0
 *  @Date    : 2018/1/23
 
 *	@History : 
 *    Author : Hughie  E_mail:denghao@mails.swust.edu.cn
 *    Descrip: Creat File
 *************************************************************************************************************/

#ifndef HIK_CAMERA_H_
#define HIK_CAMERA_H_

#include <iostream>
#include <string>

#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <opencv2/opencv.hpp>

#include "Tool.h"

namespace IPSG
{
	class CHIKCamera
	{
	public:
		/***********************************************
		 *  @Name    : CHIKCamera::CHIKCamera
		 *  @Descrip : 构造函数
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CHIKCamera(){};

		/***********************************************
		 *  @Name    : CHIKCamera::~CHIKCamera
		 *  @Descrip : 析构函数
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CHIKCamera(){ CloseCamera(); };

		/***********************************************
		 *  @Name    : CHIKCamera::OpenCamera
		 *  @Descrip : 打开监控相机设备
		 *  @Para    : [in]		 src：相机的rtsp地址
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenCamera(const std::string src);

		/***********************************************
		 *  @Name    : CHIKCamera::CloseCamera
		 *  @Descrip : 关闭监控相机
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CloseCamera();

		/***********************************************
		 *  @Name    : CHIKCamera::ReadImage
		 *  @Descrip : 从监控相机获取一张图像
		 *	@Para    : [in/out]	Image：获取到的图像
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ReadImage(cv::Mat &Image);

	private:
		/***********************************************
		 *  @Name    : CHIKCamera::CaptureVideo
		 *  @Descrip : 监控相机图像捕获
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		void CaptureVideo();

	private:
		cv::VideoCapture	m_Vcap;		//视频捕获
		cv::Mat				m_Image;	//获取到的图像数据

		boost::mutex		m_IOMutex;	//线程锁，图像异步读写的竞态控制
	};

	/***********************************************
	 *  @Name    : HIKCameraTest
	 *  @Descrip : 单元测试
	 *  @Para    : [in]		void
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool HIKCameraTest(void);
}

#endif