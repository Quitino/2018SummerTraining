/************************************************************************************************************
 *  Copyright Notice
 *  Copyright (c) 2018,�˺�
 *
 *  @File    : HIKCamera.h
 *  @Brief   : ���������ͼ���ȡ�࣬�����ڴ󻪵�֧��rtspЭ������������
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
		 *  @Descrip : ���캯��
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		CHIKCamera(){};

		/***********************************************
		 *  @Name    : CHIKCamera::~CHIKCamera
		 *  @Descrip : ��������
		 *  @Return  : None
		 *  @Notes   : None
		 ***********************************************/
		~CHIKCamera(){ CloseCamera(); };

		/***********************************************
		 *  @Name    : CHIKCamera::OpenCamera
		 *  @Descrip : �򿪼������豸
		 *  @Para    : [in]		 src�������rtsp��ַ
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool OpenCamera(const std::string src);

		/***********************************************
		 *  @Name    : CHIKCamera::CloseCamera
		 *  @Descrip : �رռ�����
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool CloseCamera();

		/***********************************************
		 *  @Name    : CHIKCamera::ReadImage
		 *  @Descrip : �Ӽ�������ȡһ��ͼ��
		 *	@Para    : [in/out]	Image����ȡ����ͼ��
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		bool ReadImage(cv::Mat &Image);

	private:
		/***********************************************
		 *  @Name    : CHIKCamera::CaptureVideo
		 *  @Descrip : ������ͼ�񲶻�
		 *  @Return  : bool true:success false:failed
		 *  @Notes   : None
		 ***********************************************/
		void CaptureVideo();

	private:
		cv::VideoCapture	m_Vcap;		//��Ƶ����
		cv::Mat				m_Image;	//��ȡ����ͼ������

		boost::mutex		m_IOMutex;	//�߳�����ͼ���첽��д�ľ�̬����
	};

	/***********************************************
	 *  @Name    : HIKCameraTest
	 *  @Descrip : ��Ԫ����
	 *  @Para    : [in]		void
	 *  @Return  : bool true:success false:failed
	 *  @Notes   : None
	 ***********************************************/
	bool HIKCameraTest(void);
}

#endif