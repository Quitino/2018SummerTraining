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
			ImageRegion.release();		//一定要消除内存占用 否则会重复
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
	cv::Mat Gy(Height, Width, CV_32S);	//因为计算出的梯度值可能有正有负，且值也可能会很大，故数据类型为整形

	//水平方向梯度
	for (int i = 0; i < Height; i++)
	{
		Gx.at<int>(i, 0) = std::abs(RegionTemp.at<byte>(i, 1) - RegionTemp.at<byte>(i, 0)) * 2;
		for (int j = 1; j < Width - 1; j++)
			Gx.at<int>(i, j) = std::abs(RegionTemp.at<byte>(i, j + 1) - RegionTemp.at<byte>(i, j - 1));
		Gx.at<int>(i, Width - 1) = std::abs(RegionTemp.at<byte>(i, Width - 1) - RegionTemp.at<byte>(i, Width - 2)) * 2;
	}
	  
	//垂直方向梯度
	for (int i = 0; i < Width; i++)
	{
		Gy.at<int>(0, i) = std::abs(RegionTemp.at<byte>(1, i) - RegionTemp.at<byte>(0, i)) * 2;
		for (int j = 1; j < Height - 1; j++)
			Gy.at<int>(j, i) = std::abs(RegionTemp.at<byte>(j + 1, i) - RegionTemp.at<byte>(j - 1, i));
		Gy.at<int>(Height - 1, i) = std::abs(RegionTemp.at<byte>(Height - 1, i) - RegionTemp.at<byte>(Height - 2, i)) * 2;
	}
	
	cv::convertScaleAbs(((cv::min)(Gx + Gy, 255)), GradientImg);	//合成梯度图，此处是求的X,Y方向梯度绝对值之和，可以求取平方和开根

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
	GetGradientImg(RegionTemp, GradientImg);	//取得灰度图

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
		v_nXRow.push_back(nXRowSum / RegionTemp.cols);		//图像块水平方向平均值得到的列向量
		v_nGxRow.push_back(nGxRowSum / GradientImg.cols);	//梯度图水平方向平均值得到的列向量
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
	int nDistanceThd = cvCeil(std::sqrt(nRegionWidth*nRegionWidth + nRegionHeight*nRegionHeight));	//对角长度，向上取整

	for (size_t i = 0; i < vARSRegion.size(); i++)
	{
		for (size_t j = i + 1; j < vARSRegion.size(); j++)
		{
			float fDistance = 0.0;
			CalEucDistance4Points(vARSRegion.at(i).m_RegionInfo.m_RegionCenter, vARSRegion.at(j).m_RegionInfo.m_RegionCenter, fDistance);
			
			//相邻区域进行相似判断
			if (fDistance <= nDistanceThd)
			{
				int nXSub = vARSRegion.at(i).m_nXEigenValue - vARSRegion.at(j).m_nXEigenValue;
				int nYSub = vARSRegion.at(i).m_nYEigenValue - vARSRegion.at(j).m_nYEigenValue;
				int nGxSub = vARSRegion.at(i).m_nGxEigenValue - vARSRegion.at(j).m_nGxEigenValue;
				int nGySub = vARSRegion.at(i).m_nGyEigenValue - vARSRegion.at(j).m_nGyEigenValue;

				int nEucDistance = cvFloor(std::sqrt(nXSub*nXSub + nYSub*nYSub + nGxSub*nGxSub + nGySub*nGySub));
				
				//如果相似
				if (nEucDistance <= m_nMergeThd)
					vARSRegion.at(j).m_nRegionLabel = vARSRegion.at(i).m_nRegionLabel;
			}
		}
	}

	cv::Mat mask = cv::Mat::zeros(InputImage.size(), CV_32S);
	for (size_t i = 0; i < vARSRegion.size(); i++)
	{
		mask(vARSRegion.at(i).m_RegionInfo.m_RegionRect).setTo(vARSRegion.at(i).m_nRegionLabel);	//生成标签矩阵
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

	std::vector<SRegionInfo> vCellImage;	//小图像块
	if (!CutImage(InputImage, vCellImage, m_nCellX, m_nCellY))
	{
		FILE_LOG(logINFO) << "Error:图像分块失败";
		return false;
	}
	
	std::vector<SARSRegion> vARSRegion;		
	if (!GetEigenValue(vCellImage, vARSRegion))
	{
		FILE_LOG(logINFO) << "Error:特征值求取失败";
		return false;
	}

	cv::Mat Label;
	if (! RegionMerge(vARSRegion, InputImage, Label))
	{
		FILE_LOG(logINFO) << "Error:区域合并失败";
		return false;
	}

	if (!RegionSegment(InputImage, Label, SusFloater))
	{
		FILE_LOG(logINFO) << "Error:前景分割失败";
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
		FILE_LOG(logINFO) << "输入的标签矩阵为空";
		return false;
	}
	if (InputImage.empty())
	{
		FILE_LOG(logINFO) << "输入的源图像为空";
		return false;
	}
	cv::Mat ImageTemp = InputImage;
	
	std::vector<int>::iterator it;
	for (int i = 0; i < InputLabel.rows; i++)
	{
		for (int j = 0; j < InputLabel.cols; j++)
		{
			int nLabelTemp = InputLabel.at<int>(i, j);

			it = std::find(vLabel.begin(), vLabel.end(), nLabelTemp);	//在标签矩阵中去查找当前像素点的标签值是否存在
			if (it == vLabel.end())
				vLabel.push_back(nLabelTemp);							//如果不存在，则压栈
		}
	}

	if (vLabel.size() > m_nLabelNOThd)
	{
		FILE_LOG(logINFO) << "Error:合并后区域类别过多，相邻相似置信度过低";
		return false;
	}

	std::multimap<int, int, std::less<int>> mulmapLabel;
	mulmapLabel.clear();
	std::multimap<int, int, std::less<int>>::iterator itMapLabel = mulmapLabel.begin();
	for (int i = 0; i < vLabel.size(); i++)
	{
		mulmapLabel.insert(std::pair<int, int>(vLabel.at(i), i));
	}

	//重建空白图像
	std::vector<cv::Mat> vSusImage(vLabel.size(), cv::Mat::zeros(InputLabel.rows, InputLabel.cols, CV_8UC1));
	//重构标签矩阵
	for (size_t i = 0; i < InputLabel.rows; i++)
	{
		for (size_t j = 0; j < InputLabel.cols; j++)
		{
			int nLabelTemp = InputLabel.at<int>(i, j);
			itMapLabel = mulmapLabel.find(nLabelTemp);
			if (mulmapLabel.end() == itMapLabel)
			{
				FILE_LOG(logINFO) << "Error:重构类别标签失败";
				return false;
			}

			//查询到之后，将其压栈至归一化之后标签对应位置
			//vSusImage.at(itMapLabel->second).at<uchar>(i, j) = InputLabel.at<uchar>(i, j);
			vSusImage.at(itMapLabel->second).at<uchar>(i, j) = 255;	//直接二值化，便于轮廓查找和求取外界矩形
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
		cv::findContours(BinaryImage, vContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);	//查找轮廓
		if (vContours.size() != 1)
		{
			FILE_LOG(logINFO) << "Error:二值标签矩阵求取轮廓错误";
			return false;
		}

		cv::Rect rect = cv::boundingRect(vContours.at(0));
		cv::Mat ImageRegion;
		ImageTemp(rect).copyTo(ImageRegion);	//从原图中截图
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