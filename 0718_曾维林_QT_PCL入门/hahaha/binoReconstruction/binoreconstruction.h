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
	*  @Descrip : ��ȡ�ؽ�ͼƬ
	*  @Para	: [in]		m_nReconIndex��ͼ��������                      
	*  @Para    : [out]	    m_mLeftImage�������ͼƬ��m_mRightImage; �����ͼƬ��m_mLeftGrayImage��������Ҷ�ͼƬ��m_mRightGrayImage��������Ҷ�ͼƬ
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool ReadImage(int m_nReconIndex,cv::Mat& m_mLeftImage, cv::Mat& m_mRightImage, cv::Mat& m_mLeftGrayImage, cv::Mat& m_mRightGrayImage);	

	/***********************************************
	*  @Name    : binoReconstruction::BinoStereoRectify
	*  @Descrip : ˫Ŀ����У��
	*  @Para	: [in]		none
	*  @Para    : [out]	    m_mapxL/m_mapyL�������ͼ���ӳ���,m_mapxR/m_mapyR�������ͼ���ӳ���
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool BinoStereoRectify(cv::Mat& m_mapxL, cv::Mat& m_mapyL, cv::Mat& m_mapxR, cv::Mat& m_mapyR);
	
	/***********************************************
	*  @Name    : binoReconstruction::ThreeDimensionalReconstruction
	*  @Descrip : ��ά�ؽ�
	*  @Para	: [in]		m_mDispLeft/m_mDispRight���Ӳ�ͼ��m_mLeftRGB/m_mRightRGB��ͼ���ӳ����RGBͼ
	*  @Para    : [out]	    m_pPointCloud:����RGB����ָ��
	*  @Return  : bool      true:success false:failed
	*  @Notes   : None
	***********************************************/
	bool ThreeDimensionalReconstruction(cv::Mat& m_mDispLeft, cv::Mat& m_mDispRight, cv::Mat& m_mLeftRGB, cv::Mat& m_mRightRGB, pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud);
	
	/***********************************************
	*  @Name    : binoReconstruction::ThreeDimensionalReconstruction
	*  @Descrip : vtk��ʾ����
	*  @Para	: [in]		none
	*  @Para    : [out]	    m_pPointCloud:����RGB����ָ��
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

	Elas*  m_pElas;                                                           //��Ҷ˹����ƥ��
	IPSG::CReadCaliParameters CalibrateParameters;                            //�궨������ʵ����
	Crgbdrconstruction rgbdrconstruction;

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud;                     //����RGB����ָ��
	boost::shared_ptr<pcl::visualization::PCLVisualizer> m_pViewerPointCloud; //���ƿ��ӻ�����ָ��

	QString m_QstrRunPath;               //��ǰ����·�� 
	QString m_QStrFilePath;              //�ļ�·�� 

	cv::Mat m_mLeftImage;                //�����ͼƬ
	cv::Mat m_mRightImage;               //�����ͼƬ
	cv::Mat m_mLeftGrayImage;            //������Ҷ�ͼƬ
	cv::Mat m_mRightGrayImage;           //������Ҷ�ͼƬ

	cv::Mat m_mapxL, m_mapyL;            //�����ͼ���ӳ���
	cv::Mat m_mapxR, m_mapyR;            //�����ͼ���ӳ���
	cv::Mat m_mLeftRGB, m_mRightRGB;     //ͼ���ӳ����RGBͼ
	cv::Mat m_mDispLeft, m_mDispRight;   //�Ӳ�ͼ

	int m_nReconIndex;                   //ͼ��������

};

#endif // BINORECONSTRUCTION_H
