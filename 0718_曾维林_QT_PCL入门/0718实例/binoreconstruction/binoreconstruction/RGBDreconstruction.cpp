#include "RGBDrconstruction.h"

Crgbdrconstruction::~Crgbdrconstruction(void)
{

}

bool Crgbdrconstruction::RgbdRconstruction(std::vector<cv::Mat>& m_vmcolorImgs, std::vector<cv::Mat>& m_vmdepthImgs, std::vector<Eigen::Isometry3d>& m_vIposes, pcl::PointCloud<pcl::PointXYZRGB>::Ptr& m_pPointCloud)
{
	ReadImagePose(m_vmcolorImgs, m_vmdepthImgs, m_vIposes);

	for (int i = 0; i < 5; i++)
	{
		cout << "转换图像中: " << i + 1 << endl;
		cv::Mat color = m_vmcolorImgs[i];
		cv::Mat depth = m_vmdepthImgs[i];
		Eigen::Isometry3d T = m_vIposes[i];
		for (int v = 0; v < color.rows; v++)
		{
			for (int u = 0; u < color.cols; u++)
			{
				unsigned int d = depth.ptr<unsigned short>(v)[u]; // 深度值

				if (d == 0)
				{
					continue;                                      
				}			

				Eigen::Vector3d point;
				pcl::PointXYZRGB p;

				point[2] = double(d) / m_depthScale;
				point[0] = (u - m_cx)*point[2] / m_fx;
				point[1] = (v - m_cy)*point[2] / m_fy;

				//转换到世界坐标系下坐标点
				Eigen::Vector3d pointWorld = T*point;

				p.x = pointWorld[0];
				p.y = pointWorld[1];
				p.z = pointWorld[2];

				p.b = color.data[v*color.step + u*color.channels()];
				p.g = color.data[v*color.step + u*color.channels() + 1];
				p.r = color.data[v*color.step + u*color.channels() + 2];

				m_pPointCloud->points.push_back(p);
			}
		}
	}

	m_pPointCloud->is_dense = false;
	pcl::io::savePCDFileBinary("./pcd/RGBDreconstruction.pcd", *m_pPointCloud);

	return true;
}

bool Crgbdrconstruction::ReadImagePose(std::vector<cv::Mat>& m_vmcolorImgs, std::vector<cv::Mat>& m_vmdepthImgs, std::vector<Eigen::Isometry3d>& m_vIposes)
{
	ifstream fin("./data/pose.txt");
	if (!fin)
	{
		cerr << "请在有pose.txt的目录下运行此程序" << endl;
		return 1;
	}

	for (int i = 0; i < 5; i++)
	{
		boost::format fmt("./data/%s/%d.%s"); //图像文件格式
		m_vmcolorImgs.push_back(cv::imread((fmt%"color" % (i + 1) % "png").str()));
		m_vmdepthImgs.push_back(cv::imread((fmt%"depth" % (i + 1) % "pgm").str(), -1)); // 使用-1读取原始图像

		double data[7] = { 0 };
		for (auto& d : data)
		{
			fin >> d;
		}
			
		Eigen::Quaterniond q(data[6], data[3], data[4], data[5]);
		Eigen::Isometry3d T(q);
		T.pretranslate(Eigen::Vector3d(data[0], data[1], data[2]));
		m_vIposes.push_back(T);
	}

	return true;
}