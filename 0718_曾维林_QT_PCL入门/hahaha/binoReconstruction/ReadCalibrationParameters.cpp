#include "ReadCalibrationParameters.h"
#include <Qtcore/QTextCodec>
#include <QFileDialog>
#include <fstream>

IPSG::CReadCaliParameters::CReadCaliParameters(void)
{

}

IPSG::CReadCaliParameters::~CReadCaliParameters(void)
{

}

bool IPSG::CReadCaliParameters::IsOpenCaliFile(std::string CaliResultPath)
{
	std::fstream file;
	char chFirst;
	file.open(CaliResultPath, std::ios::in);

	//文件是否打开
	if (!file.is_open())
	{
		return false;
	}
	else
	{
		//文件是否为空
		chFirst = file.get();
		if (chFirst == EOF)
		{
			file.close();
			return false;
		}
	}
	return true;
}


bool IPSG::CReadCaliParameters::ReadBinoCaliFile(QString CaliResultPath)
{
	m_sCaliResultPath = CaliResultPath.toStdString();
	bool bIsOpenAndData = IsOpenCaliFile(m_sCaliResultPath);

	if (bIsOpenAndData == 0)
	{
		return false;
	}
	cv::FileStorage fs(m_sCaliResultPath, cv::FileStorage::READ);

	fs["mLeftCameraInParas"]      >> m_mLeftCaliInParas;
	fs["mLeftCameraDistorParas"]  >> m_mLeftDistortParas;
	fs["mRightCameraInParas"]     >> m_mRightCaliInParas;
	fs["mRightCameraDistorParas"] >> m_mRightDistortParas;
	fs["dLeftCameraFocalLen"]     >> m_dLeftCameraFocalLen;
	fs["dRightCameraFocalLen"]    >> m_dRightCameraFocalLen;

	fs["mL2RRotation"]            >> m_mL2RRotation31;
	fs["mL2RRotation33"]          >> m_mL2RRotation33;
	fs["mL2RTranslation"]         >> m_mL2RTransVector;
	fs["mEsential"]               >> m_mEssential;
	fs["mFoundational"]           >> m_mFoundational;
	fs["ImgWidth"]                >> m_ImageSize.width;
	fs["ImgHeight"]               >> m_ImageSize.height;
	fs["dCalibrateError"]         >> m_dCalibrateError;
	fs["dCalibrateError3D"]       >> m_dCalibrateError3D;
	fs["strCalibrateTime"]        >> m_strCalibrateTime;

	fs.release();
	return true;
}