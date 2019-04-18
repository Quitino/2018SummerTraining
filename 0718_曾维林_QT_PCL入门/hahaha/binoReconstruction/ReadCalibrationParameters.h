#ifndef READCALIBRATIONPARAMETERS_H
#define READCALIBRATIONPARAMETERS_H

#include <string>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include "opencv2/flann/flann.hpp"

namespace IPSG
{
	class CReadCaliParameters
	{
	public:
		CReadCaliParameters(void);
		~CReadCaliParameters(void);

		/***********************************************
		*  @Name    : CReadCaliParameters::ReadBinoCaliFile/IsOpenCaliFile
		*  @Descrip : 读取双目标定文件/判断标定文件对否打开
		*  @Para	: [in]		CaliResultPath：标定文件路径
		*  @Para    : [in/out]	none
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool ReadBinoCaliFile(QString CaliResultPath);
		bool IsOpenCaliFile(std::string CaliResultPath);
	public:
		cv::Mat       m_mLeftCaliInParas;              //左相机标定内部参数
		cv::Mat       m_mRightCaliInParas;             //右相机标定内部参数
		cv::Mat       m_mLeftDistortParas;             //左相机标定畸变参数
		cv::Mat       m_mRightDistortParas;            //右相机标定畸变参数

		cv::Mat       m_mLeftCaliRotation;             //左相机R矩阵
		cv::Mat       m_mRinhtCaliRotation;            //右相机R矩阵
		cv::Mat       m_mLeftCaliTrans;                //左相机T矩阵
		cv::Mat       m_mRightCaliTrans;               //右相机T矩阵

		double        m_dCameraPixSize;                //相机像元尺寸
		cv::Size      m_ImageSize;                     //图像分辨率
		double        m_dLeftCameraFocalLen;           //左相机计算焦距
		double        m_dRightCameraFocalLen;          //右相机计算焦距

		cv::Mat       m_mL2RRotation31;                //旋转矩阵3*1
		cv::Mat       m_mL2RTransVector;               //平移向量
		cv::Mat       m_mL2RRotation33;                //旋转矩阵3*3

		cv::Mat       m_mEssential;                    //本质矩阵
		cv::Mat       m_mFoundational;                 //基本矩阵
		double        m_dCalibrateError;               //标定误差
		double        m_dErr;                          //反投影残差
		double        m_dCalibrateError3D;             //重建标定板三维点和真实三维点坐标求解的标定误差，比dCalirateError更具有代表性
		bool          m_bIsBinocularCameraCalibration; //双目相机标定标志位
		bool          m_IsOpenAndData;                 //打开文件且不为空的标志

		std::string   m_strCalibrateTime;              //标定时间
		std::string   m_sCaliResultPath;               //标定文件路径
		cv::Mat		  m_mHomographyFirst;		       //第一幅图像的单应矩阵（投影矩阵）	
	};
}
#endif // !READCALIBRATE_H