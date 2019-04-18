#include "ARS.h"

bool IPSG::CARS::CutImage(const cv::Mat InputImage, std::vector<SRegionInfo> &vCellImage, int nX, int nY)
{
	FILE_LOG(logDEBUG) << "CARS::CutImage() call";

	if (InputImage.empty())
	{
		FILE_LOG(logINFO) << "The image for ARS is empty";
		return false;
	}
	cv::Mat ImageTemp = InputImage;

	if (nX <= 0 || nY <= 0 || nX > ImageTemp.cols || nY > ImageTemp.rows)
	{
		FILE_LOG(logINFO) << "The number for image cutting is wrong";
		return false;
	}
	int nCellHeight = ImageTemp.rows / nY;
	int	nCellWidth = ImageTemp.cols / nX;

	cv::Mat ImageRegion;
	vCellImage.clear();
	for (int i = 0; i < nX; i++)
	{
		for (int j = 0; j < nY; j++)
		{
			cv::Rect rect(i*nCellWidth, j*nCellHeight, nCellWidth, nCellHeight);
			ImageTemp(rect).copyTo(ImageRegion);

			SRegionInfo RegionInfoTemp;
			RegionInfoTemp.m_Region = ImageRegion;
			RegionInfoTemp.m_RegionRect = rect;
			RegionInfoTemp.m_RegionCenter = (rect.tl() + cv::Point(rect.width / 2, rect.height / 2));

			vCellImage.push_back(RegionInfoTemp);
			ImageRegion.release();		//һ��Ҫ�����ڴ�ռ�� ������ظ�
		}
	}

	ImageTemp.release();
	FILE_LOG(logDEBUG) << "CARS::CutImage() return";
	return true;
}

bool IPSG::CARS::GetGradientImg(const cv::Mat InputRegion, cv::Mat& GradientImg)
{
	FILE_LOG(logDEBUG) << "CARS::GetGradientImg() call";

	if (InputRegion.empty())
	{
		FILE_LOG(logINFO) << "The region for gradient calculating is empty";
		return false;
	}
	cv::Mat RegionTemp = InputRegion;

	if (1 != RegionTemp.channels())
		cvtColor(RegionTemp, RegionTemp, CV_BGR2GRAY);

	const int Height = RegionTemp.rows;
	const int Width = RegionTemp.cols;

	cv::Mat Gx(Height, Width, CV_32S);
	cv::Mat Gy(Height, Width, CV_32S);	//��Ϊ��������ݶ�ֵ���������и�����ֵҲ���ܻ�ܴ󣬹���������Ϊ����

	//ˮƽ�����ݶ�
	for (int i = 0; i < Height; i++)
	{
		Gx.at<int>(i, 0) = std::abs(RegionTemp.at<byte>(i, 1) - RegionTemp.at<byte>(i, 0)) * 2;
		for (int j = 1; j < Width - 1; j++)
			Gx.at<int>(i, j) = std::abs(RegionTemp.at<byte>(i, j + 1) - RegionTemp.at<byte>(i, j - 1));
		Gx.at<int>(i, Width - 1) = std::abs(RegionTemp.at<byte>(i, Width - 1) - RegionTemp.at<byte>(i, Width - 2)) * 2;
	}
	  
	//��ֱ�����ݶ�
	for (int i = 0; i < Width; i++)
	{
		Gy.at<int>(0, i) = std::abs(RegionTemp.at<byte>(1, i) - RegionTemp.at<byte>(0, i)) * 2;
		for (int j = 1; j < Height - 1; j++)
			Gy.at<int>(j, i) = std::abs(RegionTemp.at<byte>(j + 1, i) - RegionTemp.at<byte>(j - 1, i));
		Gy.at<int>(Height - 1, i) = std::abs(RegionTemp.at<byte>(Height - 1, i) - RegionTemp.at<byte>(Height - 2, i)) * 2;
	}
	
	cv::convertScaleAbs(((cv::min)(Gx + Gy, 255)), GradientImg);	//�ϳ��ݶ�ͼ���˴������X,Y�����ݶȾ���ֵ֮�ͣ�������ȡƽ���Ϳ���

	RegionTemp.release();

	FILE_LOG(logDEBUG) << "CARS::GetGradientImg() return";
	return true;
}

bool IPSG::CARS::GetEigenValue(const std::vector<SRegionInfo> vCellImage, std::vector<SARSRegion>& vARSRegion)
{
	FILE_LOG(logDEBUG) << "CARS::GetEigenValue() call";

	if (0 == vCellImage.size())
	{
		FILE_LOG(logINFO) << "Error:input image cells is empty";//"Error: Can't calculate the eigen value for input image cells";
		return false;
	}

	std::vector<SRegionInfo> vCellImageTemp(vCellImage);
	vARSRegion.resize(vCellImageTemp.size());
	for (size_t i = 0; i < vCellImageTemp.size(); i++)
	{
		int nXEigen = 0, nYEigen = 0, nGxEigen = 0, nGyEigen = 0;
		CalXYGEigenValue(vCellImageTemp.at(i).m_Region, nXEigen, nYEigen, nGxEigen, nGyEigen);

		SARSRegion ARSRegionTemp;
		ARSRegionTemp.m_RegionInfo = vCellImageTemp.at(i);
		ARSRegionTemp.m_nXEigenValue = nXEigen;
		ARSRegionTemp.m_nYEigenValue = nYEigen;
		ARSRegionTemp.m_nGxEigenValue = nGxEigen;
		ARSRegionTemp.m_nGyEigenValue = nGyEigen;
		ARSRegionTemp.m_nRegionLabel = i;

		vARSRegion.at(i) = ARSRegionTemp;
	}
	
	vCellImageTemp.clear();
	FILE_LOG(logDEBUG) << "CARS::GetEigenValue() return";
	return true;
}

bool IPSG::CARS::CalXYGEigenValue(const cv::Mat InputRegion, int& nXEigen, int& nYEigen, int& nGxEigen,int& nGyEigen)
{
	FILE_LOG(logDEBUG) << "CARS::CalXYGEigenValue() call";

	if (InputRegion.empty())
	{
		FILE_LOG(logINFO) << "The region for gradient calculating is empty";
		return false;
	}
	cv::Mat RegionTemp = InputRegion;
	
	if (1 != RegionTemp.channels())
		cvtColor(RegionTemp, RegionTemp, CV_BGR2GRAY);

	cv::Mat GradientImg;
	GetGradientImg(RegionTemp, GradientImg);	//ȡ�ûҶ�ͼ

	std::vector<int> v_nXRow;
	std::vector<int> v_nYCol;
	std::vector<int> v_nGxRow;
	std::vector<int> v_nGyCol;
	for (int i = 0; i < RegionTemp.rows; i++)
	{
		int nXRowSum = 0;
		int nGxRowSum = 0;
		for (int j = 0; j < RegionTemp.cols; j++)
		{
			nXRowSum += RegionTemp.at<uchar>(i, j);
			nGxRowSum += GradientImg.at<uchar>(i, j);
		}
		v_nXRow.push_back(nXRowSum / RegionTemp.cols);		//ͼ���ˮƽ����ƽ��ֵ�õ���������
		v_nGxRow.push_back(nGxRowSum / GradientImg.cols);	//�ݶ�ͼˮƽ����ƽ��ֵ�õ���������
	}

	for (int i = 0; i < RegionTemp.cols; i++)
	{
		int nYColSum = 0;
		int nGyColSum = 0;
		for (int j = 0; j < RegionTemp.rows; j++)
		{
			nYColSum += RegionTemp.at<uchar>(j, i);
			nGyColSum += GradientImg.at<uchar>(j, i);
		}
		v_nYCol.push_back(nYColSum / RegionTemp.rows);
		v_nGyCol.push_back(nGyColSum / GradientImg.rows);
	}

	CalMedian(v_nXRow, nXEigen);
	CalMedian(v_nYCol, nYEigen);
	CalMedian(v_nGxRow, nGxEigen);
	CalMedian(v_nGyCol, nGyEigen);

	v_nXRow.clear();
	v_nYCol.clear();
	v_nGxRow.clear();
	v_nGyCol.clear();

	GradientImg.release();
	RegionTemp.release();
	FILE_LOG(logDEBUG) << "CARS::CalXYGEigenValue() return";
	return true;
}

bool IPSG::CARS::RegionMerge(std::vector<SARSRegion> vARSRegion, const cv::Mat InputImage, cv::Mat& Label)
{
	FILE_LOG(logDEBUG) << "CARS::RegionMerge() call";

	if (0 == vARSRegion.size())
	{
		FILE_LOG(logINFO) << "Error: empty ARS region";
		return false;
	}

	int nRegionWidth = vARSRegion.at(0).m_RegionInfo.m_RegionRect.width;
	int nRegionHeight = vARSRegion.at(0).m_RegionInfo.m_RegionRect.height;
	int nDistanceThd = cvCeil(std::sqrt(nRegionWidth*nRegionWidth + nRegionHeight*nRegionHeight));	//�Խǳ��ȣ�����ȡ��

	for (size_t i = 0; i < vARSRegion.size(); i++)
	{
		for (size_t j = i + 1; j < vARSRegion.size(); j++)
		{
			float fDistance = 0.0;
			CalEucDistance4Points(vARSRegion.at(i).m_RegionInfo.m_RegionCenter, vARSRegion.at(j).m_RegionInfo.m_RegionCenter, fDistance);
			
			//����������������ж�
			if (fDistance <= nDistanceThd)
			{
				int nXSub = vARSRegion.at(i).m_nXEigenValue - vARSRegion.at(j).m_nXEigenValue;
				int nYSub = vARSRegion.at(i).m_nYEigenValue - vARSRegion.at(j).m_nYEigenValue;
				int nGxSub = vARSRegion.at(i).m_nGxEigenValue - vARSRegion.at(j).m_nGxEigenValue;
				int nGySub = vARSRegion.at(i).m_nGyEigenValue - vARSRegion.at(j).m_nGyEigenValue;

				int nEucDistance = cvFloor(std::sqrt(nXSub*nXSub + nYSub*nYSub + nGxSub*nGxSub + nGySub*nGySub));
				
				//�������
				if (nEucDistance <= m_nMergeThd)
					vARSRegion.at(j).m_nRegionLabel = vARSRegion.at(i).m_nRegionLabel;
			}
		}
	}

	cv::Mat mask = cv::Mat::zeros(InputImage.size(), CV_32S);
	for (size_t i = 0; i < vARSRegion.size(); i++)
	{
		mask(vARSRegion.at(i).m_RegionInfo.m_RegionRect).setTo(vARSRegion.at(i).m_nRegionLabel);	//���ɱ�ǩ����
	}
	Label = mask;

	mask.release();
	FILE_LOG(logDEBUG) << "CARS::RegionMerge() return";
	return true;
}

bool IPSG::CARS::CalMedian(std::vector<int> vAverage, int& nMedian)
{
	FILE_LOG(logDEBUG) << "CARS::CalMedian call";
	
	if (0 == vAverage.size())
	{
		FILE_LOG(logINFO) << "Empty vector for calculate median";
		return false;
	}

	std::nth_element(vAverage.begin(), vAverage.begin() + vAverage.size() / 2, vAverage.end());
	nMedian = vAverage.at(vAverage.size() / 2);
		
	FILE_LOG(logDEBUG) << "CARS::CalMedian call";
	return true;
}

bool IPSG::CARS::CalEucDistance4Points(const cv::Point Point1, const cv::Point Point2, float& fDistance)
{
	FILE_LOG(logDEBUG) << "CARS::CalEucDistance4Points call";
	
	fDistance = std::sqrt((Point1.x - Point2.x)*(Point1.x - Point2.x) + (Point1.y - Point2.y)*(Point1.y - Point2.y));
	
	FILE_LOG(logDEBUG) << "CARS::CalEucDistance4Points call";
	return true;
}

bool IPSG::CARS::ProcessFrame(const cv::Mat InputImage, std::vector<SRegionInfo>& SusFloater)
{
	FILE_LOG(logDEBUG) << "CARS::ProcessFrame call";
	
	if (InputImage.empty())
	{
		FILE_LOG(logINFO) << "Input image is empty";
		return false;
	}

	std::vector<SRegionInfo> vCellImage;	//Сͼ���
	if (!CutImage(InputImage, vCellImage, m_nCellX, m_nCellY))
	{
		FILE_LOG(logINFO) << "Error:ͼ��ֿ�ʧ��";
		return false;
	}
	
	std::vector<SARSRegion> vARSRegion;		
	if (!GetEigenValue(vCellImage, vARSRegion))
	{
		FILE_LOG(logINFO) << "Error:����ֵ��ȡʧ��";
		return false;
	}

	cv::Mat Label;
	if (! RegionMerge(vARSRegion, InputImage, Label))
	{
		FILE_LOG(logINFO) << "Error:����ϲ�ʧ��";
		return false;
	}

	if (!RegionSegment(InputImage, Label, SusFloater))
	{
		FILE_LOG(logINFO) << "Error:ǰ���ָ�ʧ��";
		return false;
	}

	FILE_LOG(logDEBUG) << "CARS::ProcessFrame call";
	return true;
}

bool IPSG::CARS::RegionSegment(const cv::Mat InputImage, const cv::Mat InputLabel, std::vector<SRegionInfo>& SusFloater)
{
	FILE_LOG(logDEBUG) << "CARS::RegionSegment call";
	
	std::vector<int> vLabel;
	vLabel.clear();

	if (InputLabel.empty())
	{
		FILE_LOG(logINFO) << "����ı�ǩ����Ϊ��";
		return false;
	}
	if (InputImage.empty())
	{
		FILE_LOG(logINFO) << "�����Դͼ��Ϊ��";
		return false;
	}
	cv::Mat ImageTemp = InputImage;
	
	std::vector<int>::iterator it;
	for (int i = 0; i < InputLabel.rows; i++)
	{
		for (int j = 0; j < InputLabel.cols; j++)
		{
			int nLabelTemp = InputLabel.at<int>(i, j);

			it = std::find(vLabel.begin(), vLabel.end(), nLabelTemp);	//�ڱ�ǩ������ȥ���ҵ�ǰ���ص�ı�ǩֵ�Ƿ����
			if (it == vLabel.end())
				vLabel.push_back(nLabelTemp);							//��������ڣ���ѹջ
		}
	}

	if (vLabel.size() > m_nLabelNOThd)
	{
		FILE_LOG(logINFO) << "Error:�ϲ������������࣬�����������Ŷȹ���";
		return false;
	}

	std::multimap<int, int, std::less<int>> mulmapLabel;
	mulmapLabel.clear();
	std::multimap<int, int, std::less<int>>::iterator itMapLabel = mulmapLabel.begin();
	for (int i = 0; i < vLabel.size(); i++)
	{
		mulmapLabel.insert(std::pair<int, int>(vLabel.at(i), i));
	}

	//�ؽ��հ�ͼ��
	std::vector<cv::Mat> vSusImage(vLabel.size(), cv::Mat::zeros(InputLabel.rows, InputLabel.cols, CV_8UC1));
	//�ع���ǩ����
	for (size_t i = 0; i < InputLabel.rows; i++)
	{
		for (size_t j = 0; j < InputLabel.cols; j++)
		{
			int nLabelTemp = InputLabel.at<int>(i, j);
			itMapLabel = mulmapLabel.find(nLabelTemp);
			if (mulmapLabel.end() == itMapLabel)
			{
				FILE_LOG(logINFO) << "Error:�ع�����ǩʧ��";
				return false;
			}

			//��ѯ��֮�󣬽���ѹջ����һ��֮���ǩ��Ӧλ��
			//vSusImage.at(itMapLabel->second).at<uchar>(i, j) = InputLabel.at<uchar>(i, j);
			vSusImage.at(itMapLabel->second).at<uchar>(i, j) = 255;	//ֱ�Ӷ�ֵ���������������Һ���ȡ������
		}
	}

	std::vector<cv::Rect> vRect;
	vRect.resize(vSusImage.size());
	SusFloater.resize(vSusImage.size());

	for (size_t i = 0; i < vSusImage.size(); i++)
	{
		cv::Mat BinaryImage = vSusImage.at(i);
		std::vector<std::vector<cv::Point>> vContours;
		vContours.clear();
		cv::findContours(BinaryImage, vContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);	//��������
		if (vContours.size() != 1)
		{
			FILE_LOG(logINFO) << "Error:��ֵ��ǩ������ȡ��������";
			return false;
		}

		cv::Rect rect = cv::boundingRect(vContours.at(0));
		cv::Mat ImageRegion;
		ImageTemp(rect).copyTo(ImageRegion);	//��ԭͼ�н�ͼ
		cv::Point RegionCenter = rect.tl() + cv::Point(rect.width / 2, rect.height / 2);

		SRegionInfo RegionInfoTemp;
		RegionInfoTemp.m_RegionRect = rect;
		RegionInfoTemp.m_Region = ImageRegion;
		RegionInfoTemp.m_RegionCenter = RegionCenter;

		SusFloater.push_back(RegionInfoTemp);
	}

	FILE_LOG(logDEBUG) << "CARS::RegionSegment return";
	return true;
}