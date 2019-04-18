#include "CircleDetect.h"

bool IPSG::CCircleDetect::DetectContour(const cv::Mat InputImage, std::vector<std::vector<cv::Point2f>> &vecContours)
{
	FILE_LOG(logDEBUG) << "CCircleDetect::DetectContour() call";

	if (InputImage.empty())
	{
		FILE_LOG(logDEBUG) << "Image Error";
		return false;
	}

	if (InputImage.channels() != 1)
	{
		cv::cvtColor(InputImage, InputImage, CV_BGR2GRAY);
	}
	cv::imshow("GrayImage", InputImage);

	cv::Mat BinaryImage(InputImage.rows, InputImage.cols, CV_8UC1, cv::Scalar::all(0));
	cv::threshold(InputImage, BinaryImage, m_nBinaryThreshold, 255, CV_THRESH_BINARY);
	cv::Mat InvBinaryImage(InputImage.rows, InputImage.cols, CV_8UC1, cv::Scalar::all(0));

	int nXThresholdLeft  = static_cast<int>(InputImage.cols * m_fRateXLeft);
	int nXThresholdRight = static_cast<int>(InputImage.cols * m_fRateXRight);
	int nYThresholdTop = static_cast<int>(InputImage.rows * m_fRateYTop);
	int nYThresholdDown = static_cast<int>(InputImage.rows * m_fRateYDown);

	//�и��Ե���֣�������rectangle�������ڱ�����ʱ��ֱ���жϸ�ֵ��ʡ�ڴ�һЩ
	for (size_t i = 0; i < InputImage.rows; i++)
	{
		for (size_t j = 0; j < InputImage.cols; j++)
		{
			if (i > nYThresholdTop && i < nYThresholdDown 
				&& j > nXThresholdLeft && j < nXThresholdRight)
			{
				InvBinaryImage.at<uchar>(i, j) = 255 - BinaryImage.at<uchar>(i, j);
			}
			else InvBinaryImage.at<uchar>(i, j) = 0;
		}
	}
	cv::imshow("InvBinaryImage", InvBinaryImage);

	//��̬ѧ������
	cv::Mat ImageOpen(InputImage.rows, InputImage.cols, CV_8UC1, cv::Scalar::all(0));
	cv::Mat elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::morphologyEx(InvBinaryImage, ImageOpen, cv::MORPH_OPEN, elem);

	//��������
	cv::findContours(InvBinaryImage, vecContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	FILE_LOG(logDEBUG) << "CCircleDetect::DetectContour() return";
	return true;
}

bool IPSG::CCircleDetect::DetectCircle(const cv::Mat InputImage, const std::vector<std::vector<cv::Point2f>> vecContours, std::vector<cv::Point2f> &PointCenter)
{
	FILE_LOG(logDEBUG) << "CCircleDetect::DetectCircle() call";

	if (0 == vecContours.size())
	{
		FILE_LOG(logDEBUG) << "Cannot find contours from input image";
		return false;
	}

	//ɸѡ����������������Сɸѡ
	std::vector<std::vector<cv::Point2f>> vecContourTemp;
	for (size_t i = 0; i < vecContours.size(); i++)
	{
		if (vecContours.at(i).size() < m_nContourSizeHigh && vecContours.at(i).size() > m_nContourSizeLow)
		{
			vecContourTemp.push_back(vecContours.at(i));
		}
	}

	if (0 == vecContourTemp.size())
	{
		FILE_LOG(logINFO) << "Cannot find useful contours from image";
		return false;
	}

	//�ٴ�ɸѡ����������ȡ���ĵ�����
	std::vector<cv::RotatedRect> vecMinRect(vecContourTemp.size());
	std::vector<int> vecnLabel;
	//ǰ��Զ�ֵͼ�Ѿ������˴����˴��ɲ��ã���Ϊ�˸�һ���ų����ܵ����������趨һ����������Ч����
	int nXThresholdLeft = static_cast<int>(InputImage.cols * m_fRateXLeft);
	int nXThresholdRight = static_cast<int>(InputImage.cols * m_fRateXRight);
	int nYThresholdTop = static_cast<int>(InputImage.rows * m_fRateYTop);
	int nYThresholdDown = static_cast<int>(InputImage.rows * m_fRateYDown);
	for (size_t i = 0; i < vecContourTemp.size(); i++)
	{
		//��С��Ӿ���
		vecMinRect.at(i) = cv::minAreaRect(cv::Mat(vecContourTemp.at(i)));
		//��������Ӧ������С��Ӿ��εĻ���������Բ��ϲ���ȡԲ�ģ�����������������ֱ�����������
		//��Բ��Բ�ĵ���С��Ӿ��ε�ƫ����Ժ��Բ��ƣ���ֱ�ӻ�ȡ��С��Ӿ��ε��������꣬��Ϊ����������
		//������ ĳЩ���պ�������߽��ϵĲ��֣�����С��Ӿ�������Ϊ��ֵ����Ҫ�˳�
		if (vecMinRect.at(i).center.x > nXThresholdLeft && vecMinRect.at(i).center.x < nXThresholdRight
			&& vecMinRect.at(i).center.y > nYThresholdTop && vecMinRect.at(i).center.y < nYThresholdDown
			&& vecMinRect.at(i).center.x > 0 && vecMinRect.at(i).center.y > 0)
		{
			PointCenter.push_back(vecMinRect.at(i).center);
			vecnLabel.push_back(i);
		}
	}

	//��������Ч������������
	std::vector<std::vector<cv::Point2f>>vecContourFinal;
	for (size_t i = 0; i < vecnLabel.size();i++)
	{
		vecContourFinal.push_back(vecContourTemp.at(vecnLabel.at(i)));
	}
	cv::Mat DrawContour(InputImage.rows, InputImage.cols, CV_8UC1, cv::Scalar::all(0));
	drawContours(DrawContour, vecContourFinal, -1, cv::Scalar(255, 0, 0), 1, 8);
	cv::imshow("Final Contours", DrawContour);

	for (size_t i = 0; i < PointCenter.size(); i++)
	{
		cv::circle(DrawContour, PointCenter.at(i), 1, 255);
	}

	vecContourTemp.clear();
	vecContourFinal.clear();
	vecMinRect.clear();
	vecnLabel.clear();
	DrawContour.release();

	FILE_LOG(logDEBUG) << "CCircleDetect::DetectCircle() return";

	return true;
}

bool IPSG::CCircleDetect::DetectBall(const std::vector<cv::Point2f> PointCenter, cv::Point2f &BallCenter,
	cv::Point2f &BallNearestCenter, float &fNearestDistance)
{
	FILE_LOG(logDEBUG) << "CCircleDetect::DetectBall() call";

	if (0 == PointCenter.size())
	{
		FILE_LOG(logINFO) << "Cannot find point  from current image";
		return false;
	}

	//�����е�����֮���ŷʽ���룬������С��������ΪС���������ٽ���
	std::vector<SBall> vecBall;
	for (size_t i = 0; i < PointCenter.size(); i++)
	{
		for (size_t j = i + 1; j < PointCenter.size(); j++)
		{
			cv::Point2f PointTemp = PointCenter.at(i) - PointCenter.at(j);
			
			SBall BallTemp;
			BallTemp.m_fDistance = cv::norm(PointTemp);
			BallTemp.m_Point1 = PointCenter.at(i);
			BallTemp.m_Point2 = PointCenter.at(j);

			vecBall.push_back(BallTemp);
		}
	}

	//����С�����һ�������
	std::vector<float> vecfDistance;
	for (size_t i = 0; i < vecBall.size(); i++)
	{
		vecfDistance.push_back(vecBall.at(i).m_fDistance);
	}
	//����С����ĵ���vecBall�е��±�
	float fMinDistance = vecfDistance.at(0);
	int nLabel = 0;
	for (size_t i = 0; i < vecfDistance.size(); i++)
	{
		if (vecfDistance.at(i) < fMinDistance)
		{
			fMinDistance = vecfDistance.at(i);
			nLabel = i;
		}
	}
	SBall SelectBall = vecBall.at(nLabel);		//С�����������ĵ�

	SDoubleBall DoubleBall;
	DoubleBall.m_Point1 = SelectBall.m_Point1;	//��¼������������꣬�����Ҫ�����Ի�����������������������е��±�
	DoubleBall.m_Point2 = SelectBall.m_Point2;
	DoubleBall.m_nVote1 = 0;
	DoubleBall.m_nVote2 = 0;

	//ͨ��������������������ͶƱ���õ�С�������
	for (size_t i = 0; i < PointCenter.size(); i++)
	{
		if ((PointCenter.at(i).x - DoubleBall.m_Point1.x) < m_nDistanceThreshold
			|| (PointCenter.at(i).y - DoubleBall.m_Point1.y) < m_nDistanceThreshold)
		{
			DoubleBall.m_nVote1++;
		}

		if ((PointCenter.at(i).x - DoubleBall.m_Point2.x) < m_nDistanceThreshold
			|| (PointCenter.at(i).y - DoubleBall.m_Point2.y) < m_nDistanceThreshold)
		{
			DoubleBall.m_nVote2++;
		}
	}

	//��Ʊ���ٵ�ΪС�򣬵�Ʊ���Ϊ�ٽ���
	bool bAdvancedFlag = DoubleBall.m_nVote1 < DoubleBall.m_nVote2 ? true : false;
	if (bAdvancedFlag)
	{
		BallCenter = DoubleBall.m_Point1;
		BallNearestCenter = DoubleBall.m_Point2;
	}
	else
	{
		BallCenter = DoubleBall.m_Point2;
		BallNearestCenter = DoubleBall.m_Point1;
	}
	fNearestDistance = fMinDistance;

	vecBall.clear();
	vecfDistance.clear();

	FILE_LOG(logDEBUG) << "CCircleDetect::DetectBall() return";

	return true;
}

static cv::Point2f PointNear = cv::Point2f(0, 0);	//���ڴ洢�ٽ�֡���ٽ���
bool IPSG::CCircleDetect::ProcessFrame()
{
	FILE_LOG(logDEBUG) << "CCircleDetect::ProcessFrame() call";

#if defined(WIN32) || defined(_WIN32)
	IPSG::CSerialPort SerialPort;
	SerialPort.InitPort(3);
#else 
	int nFileDescriptor = 0;
	nFileDescriptor = SerialInit();
#endif

	cv::VideoCapture Video;
	Video.open(0);

	if ( !Video.isOpened())
	{
		std::cerr << "Failed to open the camera" << std::endl;
		return false;
	}

	bool bStartFlag = false;
	long lnFrameNO = 0;
	while (true)
	{
		cv::Mat InputImage;
		Video >> InputImage;
		if (InputImage.empty())
		{
			std::cerr << "Input image is empty" << std::endl;
			return false;
		}
		ScreenLog(InputImage, "Press 'S' or Enter to start");
		cv::imshow("Input image", InputImage);

		char cStartKey = cv::waitKey(2);
		if ('s' == cStartKey || 'S' == cStartKey || 13 == cStartKey) //13ΪEnter��
		{
			bStartFlag = true;
		}

		if (bStartFlag)
		{
			ScreenLog(InputImage, "Detecting...,Press 'Q' or ESC to exit");
			FILE_LOG(logINFO) << "Frame : #" << ++lnFrameNO << std::endl;

			std::vector<std::vector<cv::Point2f>> vecContours;
			std::vector<cv::Point2f> vecPointsCenter;

			cv::Point2f BallCenter;
			cv::Point2f BallNearestCenter;

			float fDistance;
			float fDistanceThreshold = 25;   //������ֵ��С�ڴ�ֵС����ٽ�������һ�𣬼�ⲻ��

			DetectContour(InputImage, vecContours);
			DetectCircle(InputImage, vecContours, vecPointsCenter);
			DetectBall(vecPointsCenter, BallCenter, BallNearestCenter, fDistance);

			if (fDistance > fDistanceThreshold && vecPointsCenter.size() == 10)	//�Ÿ����һ����
			{
				PointNear = BallNearestCenter;
			}
			else
			{
				if (lnFrameNO < m_nInitFrameNO) //��ʼ��û�����ʱ��
				{
					BallCenter = cv::Point2f(0, 0);
				}
				else
				{
					BallCenter = PointNear;		//�����һ����������Ϊ��ǰ�ٽ��������
				}
			}

			std::string strOutputData;
			PointsVector2String(vecPointsCenter,strOutputData);
			std::string strPointCenter;
			Point2String(BallCenter, strPointCenter);
			strOutputData += strPointCenter;
			strOutputData += "\r\n";			//��׺�����㵥Ƭ������λ��У������֡

#if defined(WIN32) || defined(_WIN32)
			SerialPort.WriteData(strOutputData);
#else 
			SerialTX(nFileDescriptor, strOutputData);
#endif

			char cStopKey = cv::waitKey(3);
			if (27 == cStopKey || 'q' == cStopKey || 'Q' == cStopKey)
			{
				FILE_LOG(logDEBUG) << "CCircleDetect::ProcessFrame() return";
				return true;
			}

			//��ǰ֡����������ڴ�
			vecContours.clear();
			vecPointsCenter.clear();
			InputImage.release();
		}
	}

#if !(defined(WIN32) || defined(_WIN32))
	close(nFileDescriptor);
#endif

	return true;
}

bool IPSG::CCircleDetect::ScreenLog(cv::Mat DrawImage, std::string strDrawText)
{
	FILE_LOG(logDEBUG) << "CCircleDetect::ScreenLog() call";
	int nFont = cv::FONT_HERSHEY_SIMPLEX;
	float fFontScale = 0.5;
	int nThickness = 1;
	int nBaseline;

	cv::Size TextSize = cv::getTextSize(strDrawText, nFont, fFontScale, nThickness, &nBaseline);

	cv::Point Text_bl = cv::Point(0, TextSize.height);
	cv::Point Rect_bl = Text_bl;
	Rect_bl.y += nBaseline;

	cv::Point Rect_tr = Rect_bl;
	Rect_tr.x += TextSize.width;
	Rect_tr.y -= TextSize.height + nBaseline;

	cv::rectangle(DrawImage, Rect_bl, Rect_tr, cv::Scalar(0, 0, 0), -1);
	cv::putText(DrawImage, strDrawText, Text_bl, nFont, fFontScale, cv::Scalar(255, 255, 255));

	FILE_LOG(logDEBUG) << "CCircleDetect::ScreenLog() return";
	return true;
}