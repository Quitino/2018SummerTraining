#include "binoreconstruction.h"

binoReconstruction::binoReconstruction(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.ReadCalibrationParameters, SIGNAL(triggered()), this, SLOT(ReadCaliParameters()));   //读取标定参数
	connect(ui.binoRestruction, SIGNAL(triggered()), this, SLOT(BinoRescontruction()));             //双目重建
	connect(ui.Rgbdreconstruction, SIGNAL(triggered()), this, SLOT(Rgbdrconstruction()));           //rgbd重建
	connect(ui.ReadPointCloud, SIGNAL(triggered()), this, SLOT(ReadPointCloud()));                  //读取点云显示

}

binoReconstruction::~binoReconstruction()
{

}

bool binoReconstruction::ReadCaliParameters()
{
	m_QstrRunPath = QCoreApplication::applicationDirPath();
	m_QStrFilePath = m_QstrRunPath + QString("\\CalibrationResult\\BinoCaliResult.yml");

	bool bReadSuccessflag = CalibrateParameters.ReadBinoCaliFile(m_QStrFilePath);
	if (!bReadSuccessflag)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("读取标定数据失败"));
		return false;
	}
	return true;
}

bool binoReconstruction::ReadPointCloud()
{
	//初始化指针
	m_pPointCloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
	m_pViewerPointCloud = boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer("3D Viewer", false));

	pcl::io::loadPCDFile<pcl::PointXYZRGB>("./pcd/map.pcd", *m_pPointCloud);

	if ((pcl::io::loadPCDFile<pcl::PointXYZRGB>("./pcd/map.pcd", *m_pPointCloud) == -1))
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("点云文件不存在"));
		return false;
	}

	DisplayPointCloud(m_pPointCloud);
	return true;
}

bool binoReconstruction::BinoRescontruction()
{
	//初始化指针
	m_pPointCloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
	m_pViewerPointCloud = boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer("3D Viewer", false));

	//初始化贝叶斯稠密匹配参数
	Elas::parameters param;
	m_pElas = new Elas(param);

	cv::Mat mInput_Left, mInput_Right;
	cv::Mat mProcess_left, mProcess_right;

	BinoStereoRectify(m_mapxL, m_mapyL, m_mapxR, m_mapyR);

	for (int m_nReconIndex = 0; m_nReconIndex < 1; m_nReconIndex++)
	{
		ReadImage(m_nReconIndex,m_mLeftImage, m_mRightImage, m_mLeftGrayImage, m_mRightGrayImage);

		remap(m_mLeftGrayImage, mInput_Left, m_mapxL, m_mapyL, INTER_LINEAR);
		remap(m_mRightGrayImage, mInput_Right, m_mapxR, m_mapyR, INTER_LINEAR);

		remap(m_mLeftImage, m_mLeftRGB, m_mapxL, m_mapyL, INTER_LINEAR);
		remap(m_mRightImage, m_mRightRGB, m_mapxL, m_mapyL, INTER_LINEAR);

		mInput_Left.copyTo(mProcess_left);
		mInput_Right.copyTo(mProcess_right);

		//贝叶斯稠密匹配，输出视差图
		m_pElas->StereoElas(mProcess_left, mProcess_right, m_mDispLeft, m_mDispRight);

		imwrite("mdisp_left.bmp", m_mDispLeft);
		imwrite("mdisp_right.bmp", m_mDispRight);

		ThreeDimensionalReconstruction(m_mDispLeft, m_mDispRight, m_mLeftRGB, m_mRightRGB, m_pPointCloud);
	}

	DisplayPointCloud(m_pPointCloud);
	return true;
}

bool binoReconstruction::Rgbdrconstruction()
{
	vector<cv::Mat> colorImgs, depthImgs;    //彩色图和深度图
	vector<Eigen::Isometry3d> poses;         //相机位姿

	//初始化指针
	m_pPointCloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
	m_pViewerPointCloud = boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer("3D Viewer", false));

	rgbdrconstruction.RgbdRconstruction(colorImgs, depthImgs, poses, m_pPointCloud);
	DisplayPointCloud(m_pPointCloud);

	return true;
}

bool binoReconstruction::BinoStereoRectify(cv::Mat& m_mapxL, cv::Mat& m_mapyL, cv::Mat& m_mapxR, cv::Mat& m_mapyR)
{
	//输出深度视差映射矩阵,输出第一个相机的3x3矫正变换(旋转矩阵),输出第二个相机的3x3矫正变换(旋转矩阵),在第一台相机的新的坐标系统(矫正过的)输出 3x4 的投影矩阵,在第二台相机的新的坐标系统(矫正过的)输出 3x4 的投影矩阵
	cv::Mat Q = cv::Mat(4, 4, CV_64FC1);
	cv::Mat Rl = cv::Mat(3, 3, CV_64FC1);
	cv::Mat Rr = cv::Mat(3, 3, CV_64FC1);
	cv::Mat Pl = cv::Mat(3, 4, CV_64FC1);
	cv::Mat Pr = cv::Mat(3, 4, CV_64FC1);

	//转变变量类型，传入校正函数
	CalibrateParameters.m_mLeftCaliInParas.convertTo(CalibrateParameters.m_mLeftCaliInParas, CV_64FC1);
	CalibrateParameters.m_mLeftDistortParas.convertTo(CalibrateParameters.m_mLeftDistortParas, CV_64FC1);
	CalibrateParameters.m_mRightCaliInParas.convertTo(CalibrateParameters.m_mRightCaliInParas, CV_64FC1);
	CalibrateParameters.m_mRightDistortParas.convertTo(CalibrateParameters.m_mRightDistortParas, CV_64FC1);

	CalibrateParameters.m_mL2RRotation31.convertTo(CalibrateParameters.m_mL2RRotation31, CV_64FC1);
	CalibrateParameters.m_mL2RRotation33.convertTo(CalibrateParameters.m_mL2RRotation33, CV_64FC1);
	CalibrateParameters.m_mL2RTransVector.convertTo(CalibrateParameters.m_mL2RTransVector, CV_64FC1);

	stereoRectify(CalibrateParameters.m_mLeftCaliInParas, CalibrateParameters.m_mLeftDistortParas,
		CalibrateParameters.m_mRightCaliInParas, CalibrateParameters.m_mRightDistortParas,
		CalibrateParameters.m_ImageSize, CalibrateParameters.m_mL2RRotation33, CalibrateParameters.m_mL2RTransVector,
		Rl, Rr, Pl, Pr, Q, CV_CALIB_ZERO_DISPARITY);

	//畸变校正
	initUndistortRectifyMap(CalibrateParameters.m_mLeftCaliInParas, CalibrateParameters.m_mLeftDistortParas,
		Rl, Pl, CalibrateParameters.m_ImageSize, CV_32FC1, m_mapxL, m_mapyL);
	initUndistortRectifyMap(CalibrateParameters.m_mRightCaliInParas, CalibrateParameters.m_mRightDistortParas,
		Rr, Pr, CalibrateParameters.m_ImageSize, CV_32FC1, m_mapxR, m_mapyR);

	return true;
}

bool binoReconstruction::ReadImage(int m_nReconIndex, cv::Mat& m_mLeftImage, cv::Mat& m_mRightImage, cv::Mat& m_mLeftGrayImage, cv::Mat& m_mRightGrayImage)
{
	QString ImgFilePath_L = m_QstrRunPath + QString("\\3D data\\reconstruction_L%1.bmp").arg(m_nReconIndex);
	QString ImgFilePath_R = m_QstrRunPath + QString("\\3D data\\reconstruction_R%1.bmp").arg(m_nReconIndex);

	m_mLeftImage = cv::imread(ImgFilePath_L.toStdString(), CV_LOAD_IMAGE_UNCHANGED);
	m_mRightImage = cv::imread(ImgFilePath_R.toStdString(), CV_LOAD_IMAGE_UNCHANGED);

	QFile fileL(ImgFilePath_L);
	QFile fileR(ImgFilePath_R);

	if (!fileL.exists() || !fileR.exists())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("重建图片不存在"));
		return false;
	}

	if (m_mLeftImage.channels() == 3)
	{
		cvtColor(m_mLeftImage, m_mLeftGrayImage, CV_RGB2GRAY);
		cvtColor(m_mRightImage, m_mRightGrayImage, CV_RGB2GRAY);
	}
	else
	{
		m_mLeftGrayImage = m_mLeftImage.clone();
		m_mRightGrayImage = m_mRightImage.clone();
	}

	return true;
}

bool binoReconstruction::ThreeDimensionalReconstruction(cv::Mat& m_mDispLeft, cv::Mat& m_mDispRight, cv::Mat& m_mLeftRGB, cv::Mat& m_mRightRGB, pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud)
{
	//左右图像中心像素坐标，相机焦距，缩放因子，基线距离
	double dLeftCamUV[2] = { CalibrateParameters.m_mLeftCaliInParas.at<double>(0, 2), CalibrateParameters.m_mLeftCaliInParas.at<double>(1, 2) };
	double dRightCamUV[2] = { CalibrateParameters.m_mRightCaliInParas.at<double>(0, 2), CalibrateParameters.m_mRightCaliInParas.at<double>(1, 2) };

	double dLeftCamF = CalibrateParameters.m_dLeftCameraFocalLen;
	double dRightCamF = CalibrateParameters.m_dRightCameraFocalLen;

	double alpha = CalibrateParameters.m_mLeftCaliInParas.at<double>(0, 0) / CalibrateParameters.m_dLeftCameraFocalLen;
	double belta = CalibrateParameters.m_mLeftCaliInParas.at<double>(1, 1) / CalibrateParameters.m_dLeftCameraFocalLen;

	double B = sqrt(CalibrateParameters.m_mL2RTransVector.at<double>(0, 0)*CalibrateParameters.m_mL2RTransVector.at<double>(0, 0)
		+ CalibrateParameters.m_mL2RTransVector.at<double>(1, 0)*CalibrateParameters.m_mL2RTransVector.at<double>(1, 0)
		+ CalibrateParameters.m_mL2RTransVector.at<double>(2, 0)*CalibrateParameters.m_mL2RTransVector.at<double>(2, 0));

	//计算三维点云
	for (int m = 0; m < m_mDispLeft.rows; m++)
	{
		for (int n = 0; n < m_mDispLeft.cols; n++)
		{
			if (m_mDispLeft.at<uchar>(m, n) != 0.0)
			{
				pcl::PointXYZRGB pointTemp;
				pointTemp.x = B * ((n - dLeftCamUV[0]) / alpha) / m_mDispLeft.at<uchar>(m, n);
				pointTemp.y = B * dLeftCamF * ((m - dLeftCamUV[1]) / belta) / m_mDispLeft.at<uchar>(m, n);
				pointTemp.z = B * dLeftCamF / m_mDispLeft.at<uchar>(m, n);
				
				if (pointTemp.z < 15 || pointTemp.z >60)
				{
					continue;
				}

				pointTemp.b = m_mLeftRGB.at<Vec3b>(m, n)[0];
				pointTemp.g = m_mLeftRGB.at<Vec3b>(m, n)[1];
				pointTemp.r = m_mLeftRGB.at<Vec3b>(m, n)[2];
				m_pPointCloud->points.push_back(pointTemp);
			}
		}
	}

	pcl::io::savePCDFileBinary("./pcd/Binoreconstruction.pcd", *m_pPointCloud);
	return true;
}

bool binoReconstruction::DisplayPointCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_pPointCloud)
{
	m_pPointCloud->width = m_pPointCloud->points.size();        //设置点云宽度
	m_pPointCloud->height = 1;                                  //设置点云为无序点云
	m_pPointCloud->is_dense = false;

	ui.qvtkWidget->SetRenderWindow(m_pViewerPointCloud->getRenderWindow());
	m_pViewerPointCloud->setBackgroundColor(0.153, 0.153, 0.153);
	m_pViewerPointCloud->addCoordinateSystem();
	m_pViewerPointCloud->setCameraPosition(0, 0, 500, -1, -1, 0);
	m_pViewerPointCloud->addPointCloud(m_pPointCloud);

	ui.qvtkWidget->show();
	return true;
}