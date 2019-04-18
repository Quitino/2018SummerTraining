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
	*  @Descrip : 读取图像和相机位姿
	*  @Para	: [in]		none
	*  @Para    : [out]	    m_vmcolorImgs/m_vmdepthImgs：彩色图和深度图，m_vIposes：相机位姿
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool ReadImagePose(std::vector<cv::Mat>& m_vmcolorImgs, std::vector<cv::Mat>& m_vmdepthImgs, std::vector<Eigen::Isometry3d>& m_vIposes);
	
	/***********************************************
	*  @Name    : Crgbdrconstruction::RgbdRconstruction
	*  @Descrip : RGBD三维重建
	*  @Para	: [in]		m_vmcolorImgs/m_vmdepthImgs：彩色图和深度图，m_vIposes：相机位姿
	*  @Para    : [out]	    m_pPointCloud：RGB点云指针
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool RgbdRconstruction(std::vector<cv::Mat>& m_vmcolorImgs, std::vector<cv::Mat>& m_vmdepthImgs, std::vector<Eigen::Isometry3d>& m_vIposes, pcl::PointCloud<pcl::PointXYZRGB>::Ptr& m_pPointCloud);
private:

	//相机内部参数
	double m_fx;
	double m_fy;
	double m_cx;
	double m_cy;
	double m_depthScale;

	std::vector<cv::Mat> m_vmcolorImgs, m_vmdepthImgs;                // 彩色图和深度图
	std::vector<Eigen::Isometry3d> m_vIposes;                         // 相机位姿

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud;             //保存RGB点云指针
};
#endif // !RGBDRCONSTRUCTION