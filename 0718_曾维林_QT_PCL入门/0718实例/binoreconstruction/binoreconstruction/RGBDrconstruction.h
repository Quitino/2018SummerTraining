#ifndef RGBDRCONSTRUCTION_H
#define RGBDRCONSTRUCTION_H

#include <fstream>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include <Eigen/Geometry> 
#include <boost/format.hpp>  
#include <pcl/point_types.h> 
#include <pcl/io/pcd_io.h> 
#include <pcl/visualization/pcl_visualizer.h>

class Crgbdrconstruction
{
public:
	Crgbdrconstruction() :m_fx(518.0), m_fy(519.0), m_cx(325.5), m_cy(253.5),m_depthScale(1000.0){}
	~Crgbdrconstruction(void);

	/***********************************************
	*  @Name    : Crgbdrconstruction::ReadImagePose
	*  @Descrip : ��ȡͼ������λ��
	*  @Para	: [in]		none
	*  @Para    : [out]	    m_vmcolorImgs/m_vmdepthImgs����ɫͼ�����ͼ��m_vIposes�����λ��
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool ReadImagePose(std::vector<cv::Mat>& m_vmcolorImgs, std::vector<cv::Mat>& m_vmdepthImgs, std::vector<Eigen::Isometry3d>& m_vIposes);
	
	/***********************************************
	*  @Name    : Crgbdrconstruction::RgbdRconstruction
	*  @Descrip : RGBD��ά�ؽ�
	*  @Para	: [in]		m_vmcolorImgs/m_vmdepthImgs����ɫͼ�����ͼ��m_vIposes�����λ��
	*  @Para    : [out]	    m_pPointCloud��RGB����ָ��
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool RgbdRconstruction(std::vector<cv::Mat>& m_vmcolorImgs, std::vector<cv::Mat>& m_vmdepthImgs, std::vector<Eigen::Isometry3d>& m_vIposes, pcl::PointCloud<pcl::PointXYZRGB>::Ptr& m_pPointCloud);
private:

	//����ڲ�����
	double m_fx;
	double m_fy;
	double m_cx;
	double m_cy;
	double m_depthScale;

	std::vector<cv::Mat> m_vmcolorImgs, m_vmdepthImgs;                // ��ɫͼ�����ͼ
	std::vector<Eigen::Isometry3d> m_vIposes;                         // ���λ��

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud;             //����RGB����ָ��
};
#endif // !RGBDRCONSTRUCTION