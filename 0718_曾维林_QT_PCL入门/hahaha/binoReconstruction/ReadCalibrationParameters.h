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
		*  @Descrip : ��ȡ˫Ŀ�궨�ļ�/�жϱ궨�ļ��Է��
		*  @Para	: [in]		CaliResultPath���궨�ļ�·��
		*  @Para    : [in/out]	none
		*  @Return  : bool true:success false:failed
		*  @Notes   : None
		***********************************************/
		bool ReadBinoCaliFile(QString CaliResultPath);
		bool IsOpenCaliFile(std::string CaliResultPath);
	public:
		cv::Mat       m_mLeftCaliInParas;              //������궨�ڲ�����
		cv::Mat       m_mRightCaliInParas;             //������궨�ڲ�����
		cv::Mat       m_mLeftDistortParas;             //������궨�������
		cv::Mat       m_mRightDistortParas;            //������궨�������

		cv::Mat       m_mLeftCaliRotation;             //�����R����
		cv::Mat       m_mRinhtCaliRotation;            //�����R����
		cv::Mat       m_mLeftCaliTrans;                //�����T����
		cv::Mat       m_mRightCaliTrans;               //�����T����

		double        m_dCameraPixSize;                //�����Ԫ�ߴ�
		cv::Size      m_ImageSize;                     //ͼ��ֱ���
		double        m_dLeftCameraFocalLen;           //��������㽹��
		double        m_dRightCameraFocalLen;          //��������㽹��

		cv::Mat       m_mL2RRotation31;                //��ת����3*1
		cv::Mat       m_mL2RTransVector;               //ƽ������
		cv::Mat       m_mL2RRotation33;                //��ת����3*3

		cv::Mat       m_mEssential;                    //���ʾ���
		cv::Mat       m_mFoundational;                 //��������
		double        m_dCalibrateError;               //�궨���
		double        m_dErr;                          //��ͶӰ�в�
		double        m_dCalibrateError3D;             //�ؽ��궨����ά�����ʵ��ά���������ı궨����dCalirateError�����д�����
		bool          m_bIsBinocularCameraCalibration; //˫Ŀ����궨��־λ
		bool          m_IsOpenAndData;                 //���ļ��Ҳ�Ϊ�յı�־

		std::string   m_strCalibrateTime;              //�궨ʱ��
		std::string   m_sCaliResultPath;               //�궨�ļ�·��
		cv::Mat		  m_mHomographyFirst;		       //��һ��ͼ��ĵ�Ӧ����ͶӰ����	
	};
}
#endif // !READCALIBRATE_H