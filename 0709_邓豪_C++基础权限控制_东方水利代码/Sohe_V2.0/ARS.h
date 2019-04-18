#ifndef ADJACENT_REGION_SIMILAR_H_
#define ADJACENT_REGION_SIMILAR_H_

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include <opencv2/opencv.hpp>

#include "log.h"

namespace IPSG
{
	typedef struct RegionInfo
	{
		cv::Mat		m_Region;
		cv::Rect	m_RegionRect;
		cv::Point	m_RegionCenter;
	}SRegionInfo;

	typedef struct ARSRegion
	{
		SRegionInfo	m_RegionInfo;		

		int			m_nXEigenValue;		//X����ƽ��ֵ��������ֵ�õ�������ֵ
		int			m_nYEigenValue;		//Y����ƽ��ֵ��������ֵ�õ�������ֵ
		int			m_nGxEigenValue;	//�ݶ�X��������ֵ
		int			m_nGyEigenValue;	//�ݶ�Y��������ֵ
		int			m_nRegionLabel;		//��������ǩ
	}SARSRegion;

	class CARS
	{
	public:
		CARS(int nCellX = 25, int nCellY = 25) :m_nCellX(nCellX), m_nCellY(nCellY), m_nMergeThd(35), m_nLabelNOThd(200){};
		~CARS(){};

		bool ProcessFrame(const cv::Mat InputImage, std::vector<SRegionInfo>& SusFloater);

	private:
		bool CutImage(const cv::Mat InputImage, std::vector<SRegionInfo> &vCellImage, int nX, int nY);
		bool GetEigenValue(const std::vector<SRegionInfo> vCellImage, std::vector<SARSRegion>& vARSRegion);
		bool RegionMerge(std::vector<SARSRegion> vARSRegion, const cv::Mat InputImage, cv::Mat& Label);
		bool RegionSegment(const cv::Mat InputImage, const cv::Mat InputLabel, std::vector<SRegionInfo>& SusFloater);


		bool CalXYGEigenValue(const cv::Mat InputRegion, int& nXEigen, int& nYEigen, int& nGxEigen, int& nGyEigen);
		bool GetGradientImg(const cv::Mat InputRegion, cv::Mat& GradientImg);
		bool CalMedian(std::vector<int> vAverage, int& nMedian);
		bool CalEucDistance4Points(const cv::Point Point1, const cv::Point Point2,float& fDistance);
	private:
		int		m_nCellX;				//X����ͼ���зֿ���
		int		m_nCellY;				//Y����ͼ���зֿ���
		int		m_nMergeThd;			//����ϲ�ʱ��ֵ
		int		m_nLabelNOThd;			//�ϲ���������������
	};
}

#endif // !ADJACENT_REGION_SIMILAR_H_
