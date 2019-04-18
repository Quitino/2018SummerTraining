#include "Saliency.h"

bool IPSG::CSaliency::ImageSaliency(const cv::Mat InputImage, std::vector<SSaliencyRegion> &vSaliencyRegion)
{
	FILE_LOG(logDEBUG) << "CSaliency::ImageSaliency call";
	
	if (InputImage.empty())
	{
		FILE_LOG(logINFO) << "Input image for saliency is empty";
		return false;
	}
	cv::Mat ImageTemp = InputImage;

	FILE_LOG(logINFO) << "Saliency target extract...";
	if (m_SaliencyAlgorithm == ASLIC)
	{
		SLIC Slic;
		Slic.SuperpixelTest(ImageTemp, 1000);

		if (0 == Slic.m_SOutSamples.m_vOutSamples.size())
		{
			FILE_LOG(logINFO) << "Warning: No suspicious floater after SLIC";
			return false;
		}
		vSaliencyRegion.resize(Slic.m_SOutSamples.m_vOutSamples.size());

		cv::Point ImageCenter = cv::Point(ImageTemp.cols / 2, ImageTemp.rows / 2);
		for (size_t i = 0; i < Slic.m_SOutSamples.m_vOutSamples.size(); i++)
		{
			SSaliencyRegion SaliencyRegionTemp;
			SaliencyRegionTemp.m_SaliencyRegion = Slic.m_SOutSamples.m_vOutSamples.at(i);
			SaliencyRegionTemp.m_SaliencyRect = Slic.m_SOutSamples.m_vOutRects.at(i);
			SaliencyRegionTemp.m_fAnglePerPixel = m_fCameraFramHoriAngle / ImageTemp.cols;

			cv::Rect rectTemp = Slic.m_SOutSamples.m_vOutRects.at(i);
			cv::Point RegionCenter = rectTemp.tl() + cv::Point(rectTemp.width / 2, rectTemp.height / 2);

			float fHoriAngle = (RegionCenter.x - ImageCenter.x)*SaliencyRegionTemp.m_fAnglePerPixel;
			float fVertAngle = (RegionCenter.y - ImageCenter.y)*SaliencyRegionTemp.m_fAnglePerPixel;
			SaliencyRegionTemp.m_fHoriAngle = fHoriAngle;
			SaliencyRegionTemp.m_fVertAngle = fVertAngle;

			vSaliencyRegion.push_back(SaliencyRegionTemp);
		}

		ImageTemp.release();
		return true;
	}
	else if (m_SaliencyAlgorithm == AARS)
	{
		int nHoriCutNO = ImageTemp.cols / 16;
		int nVertCutNO = ImageTemp.rows / 16;
		//CARS ARS(32, 32);	//图像切分的数目
		CARS ARS(nHoriCutNO, nVertCutNO);
		std::vector<SRegionInfo> vSusFloater;
		ARS.ProcessFrame(ImageTemp, vSusFloater);

		if (0 == vSusFloater.size())
		{
			FILE_LOG(logINFO) << "Warning: No suspicious floater after ARS";
			return false;
		}

		cv::Point ImageCenter = cv::Point(ImageTemp.cols / 2, ImageTemp.rows / 2);
		for (size_t i = 0; i < vSusFloater.size(); i++)
		{
			SSaliencyRegion SaliencyRegionTemp;
			SaliencyRegionTemp.m_SaliencyRegion = vSusFloater.at(i).m_Region;
			SaliencyRegionTemp.m_SaliencyRect = vSusFloater.at(i).m_RegionRect;
			SaliencyRegionTemp.m_fAnglePerPixel = m_fCameraFramHoriAngle / ImageTemp.cols;

			cv::Rect rectTemp = vSusFloater.at(i).m_RegionRect;
			cv::Point RegionCenter = rectTemp.tl() + cv::Point(rectTemp.width / 2, rectTemp.height / 2);

			float fHoriAngle = (RegionCenter.x - ImageCenter.x)*SaliencyRegionTemp.m_fAnglePerPixel;
			float fVertAngle = (RegionCenter.y - ImageCenter.y)*SaliencyRegionTemp.m_fAnglePerPixel;
			SaliencyRegionTemp.m_fHoriAngle = fHoriAngle;
			SaliencyRegionTemp.m_fVertAngle = fVertAngle;

			vSaliencyRegion.push_back(SaliencyRegionTemp);
		}

		ImageTemp.release();
		return true;
	}
	
	FILE_LOG(logDEBUG) << "CSaliency::ImageSaliency call";
	return false;
}

bool IPSG::SaliencyTest()
{

	//cv::Mat inputimage = cv::imread("src.jpg");
	//cv::imshow("src", inputimage);
	//cv::waitKey();
	
	cv::VideoCapture Video;
	Video.open("D://src.mp4");

	if (!Video.isOpened())
	{
		FILE_LOG(logINFO) << "Video source error";
		return false;
	}

	bool bStartFlag = false;
	int nImageCnt = 0;
	while (true)
	{
		cv::Mat inputImg;
		Video >> inputImg;

		if (inputImg.empty())
		{
			FILE_LOG(logINFO) << "Error:image empty";
			return false;
		}

		cv::imshow("Input Video", inputImg);

		char cStarKey = cv::waitKey(2);
		if (cStarKey == 's' || cStarKey == 'S')
			bStartFlag = true;

		if (bStartFlag)
		{
			FILE_LOG(logINFO) << "处理第" << ++nImageCnt << "张图片";

			CSaliency Saliency;
			std::vector<SSaliencyRegion> vSaliencyRegion;
			if (Saliency.ImageSaliency(inputImg, vSaliencyRegion))
			{
				for (size_t i = 0; i < vSaliencyRegion.size(); i++)
				{
					cv::rectangle(inputImg, vSaliencyRegion.at(i).m_SaliencyRect, cv::Scalar(0, 0, 255), 1);
				}

				cv::imshow("result", inputImg);
				char cExit = cv::waitKey(50);
				if (cExit == 27)
					break;
			}

		}
	}
	return true;
}