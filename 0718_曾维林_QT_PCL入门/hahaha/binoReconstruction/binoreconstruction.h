#ifndef BINORECONSTRUCTION_H
#define BINORECONSTRUCTION_H

#include <QtWidgets/QMainWindow>
#include "ui_binoreconstruction.h"

#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QTime>

//pcl
#include "boost/shared_ptr.hpp"
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

//vtk
#include <vtkRenderWindow.h>  
#include <vtkRenderer.h>  
#include <vtkImageViewer2.h>  
#include <QVTKWidget.h>  
#include <vtkJPEGReader.h>  
#include <vtkImageActor.h>  
#include <vtkSmartPointer.h>  
#include <vtkTextActor.h>
#include <vtkAutoInit.h> 

#include <BinoReconstruction/elas.h>
#include "ReadCalibrationParameters.h"
#include "RGBDrconstruction.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

class binoReconstruction : public QMainWindow
{
	Q_OBJECT

public:
	binoReconstruction(QWidget *parent = 0);
	~binoReconstruction();

	/***********************************************
	*  @Name    : binoReconstruction::ReadImage
	*  @Descrip : 读取重建图片
	*  @Para	: [in]		m_nReconIndex：图像索引号                      
	*  @Para    : [out]	    m_mLeftImage：左相机图片，m_mRightImage; 右相机图片，m_mLeftGrayImage：左相机灰度图片，m_mRightGrayImage：右相机灰度图片
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool ReadImage(int m_nReconIndex,cv::Mat& m_mLeftImage, cv::Mat& m_mRightImage, cv::Mat& m_mLeftGrayImage, cv::Mat& m_mRightGrayImage);	

	/***********************************************
	*  @Name    : binoReconstruction::BinoStereoRectify
	*  @Descrip : 双目立体校正
	*  @Para	: [in]		none
	*  @Para    : [out]	    m_mapxL/m_mapyL：左相机图像的映射表,m_mapxR/m_mapyR：右相机图像的映射表
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool BinoStereoRectify(cv::Mat& m_mapxL, cv::Mat& m_mapyL, cv::Mat& m_mapxR, cv::Mat& m_mapyR);
	
	/***********************************************
	*  @Name    : binoReconstruction::ThreeDimensionalReconstruction
	*  @Descrip : 三维重建
	*  @Para	: [in]		m_mDispLeft/m_mDispRight：视差图，m_mLeftRGB/m_mRightRGB：图像的映射后的RGB图
	*  @Para    : [out]	    m_pPointCloud:保存RGB点云指针
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool ThreeDimensionalReconstruction(cv::Mat& m_mDispLeft, cv::Mat& m_mDispRight, cv::Mat& m_mLeftRGB, cv::Mat& m_mRightRGB, pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud);
	
	/***********************************************
	*  @Name    : binoReconstruction::ThreeDimensionalReconstruction
	*  @Descrip : vtk显示点云
	*  @Para	: [in]		none
	*  @Para    : [out]	    m_pPointCloud:保存RGB点云指针
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool DisplayPointCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud);

private slots:

	bool ReadCaliParameters();
	bool BinoRescontruction();
	bool Rgbdrconstruction();
	bool ReadPointCloud();

private:
	Ui::binoReconstructionClass ui;

	Elas*  m_pElas;                                                           //贝叶斯稠密匹配
	IPSG::CReadCaliParameters CalibrateParameters;                            //标定参数类实例化
	Crgbdrconstruction rgbdrconstruction;

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud;                     //保存RGB点云指针
	boost::shared_ptr<pcl::visualization::PCLVisualizer> m_pViewerPointCloud; //点云可视化界面指针

	QString m_QstrRunPath;               //当前运行路径 
	QString m_QStrFilePath;              //文件路径 

	cv::Mat m_mLeftImage;                //左相机图片
	cv::Mat m_mRightImage;               //右相机图片
	cv::Mat m_mLeftGrayImage;            //左相机灰度图片
	cv::Mat m_mRightGrayImage;           //右相机灰度图片

	cv::Mat m_mapxL, m_mapyL;            //左相机图像的映射表
	cv::Mat m_mapxR, m_mapyR;            //右相机图像的映射表
	cv::Mat m_mLeftRGB, m_mRightRGB;     //图像的映射后的RGB图
	cv::Mat m_mDispLeft, m_mDispRight;   //视差图

	int m_nReconIndex;                   //图像索引号

};

#endif // BINORECONSTRUCTION_H
