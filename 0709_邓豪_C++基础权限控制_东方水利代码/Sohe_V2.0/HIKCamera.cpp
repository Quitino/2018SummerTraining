#include "HIKCamera.h"

bool IPSG::CHIKCamera::OpenCamera(const std::string src)
{
	FILE_LOG(logDEBUG) << "CHIKCamera::OpenCamera() call";
	if ( !m_Vcap.open(src))
	{
		FILE_LOG(logINFO) << "Error in opening video stream";
		return false;
	}
	FILE_LOG(logINFO) << "Video stream address: " << src;

	boost::thread thd(boost::bind(&CHIKCamera::CaptureVideo, this));

	FILE_LOG(logDEBUG) << "CHIKCamera::OpenCamera() return";

	return true;
}

bool IPSG::CHIKCamera::CloseCamera()
{
	if (m_Vcap.isOpened())
		m_Vcap.release();

	if (!m_Image.empty())
		m_Image.release();

	return true;
}

void IPSG::CHIKCamera::CaptureVideo()
{
	FILE_LOG(logDEBUG) << "CHIKCamera::CaptureVideo() call";

	for (;;)
	{
		if (m_Vcap.grab())
		{
			boost::mutex::scoped_lock lck(m_IOMutex);
			m_Vcap.retrieve(m_Image);
		}
		else Sleep(30);	//µÈ´ý30ms
	}
	FILE_LOG(logDEBUG) << "CHIKCamera::CaptureVideo() return";
}

bool IPSG::CHIKCamera::ReadImage(cv::Mat &Image)
{
	FILE_LOG(logDEBUG) << "CHIKCamera::ReadImage() call";
	boost::mutex::scoped_lock lck(m_IOMutex);
	if (!m_Image.empty())
	{
		Image.release();
		cv::swap(m_Image, Image);
		FILE_LOG(logDEBUG) << "CHIKCamera::ReadImage() return";
		return true;
	}
	else return false;
}

bool IPSG::HIKCameraTest()
{
	FILE_LOG(logDEBUG) << "HIKCameraTest() call";

	CHIKCamera HIKCamera;
	if ( !HIKCamera.OpenCamera("rtsp://admin:Adminhik@192.168.1.64:554/mjpeg/ch1/sub/av_stream"))
	{
		FILE_LOG(logDEBUG) << "Cannot open camera" ;
		return false;
	}

	while (true)
	{
		cv::Mat img;
		if ( !HIKCamera.ReadImage(img))
			continue;

		if (img.empty())
		{
			FILE_LOG(logDEBUG) << "Current image empty";
			continue;
		}
		cv::imshow("video", img);

		char cKey = cv::waitKey(1);
		if (cKey == 'Q' || cKey == 'q')
			break;
	}

	FILE_LOG(logDEBUG) << "HIKCameraTest() return";
	return true;
}