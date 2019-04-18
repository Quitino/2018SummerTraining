#include "PointMatch.h"

IPSG::CPointMatch::CPointMatch()
{
	m_pTargetCloud = PointCloudT::Ptr(new PointCloudT);
	m_pInputCloud = PointCloudT::Ptr(new PointCloudT);

	m_nIterations = 1;
}


IPSG::CPointMatch::~CPointMatch()
{
	
}

void IPSG::CPointMatch::keyboardEventOccurred(const pcl::visualization::KeyboardEvent& event, void* nothing)
{  
	//ʹ�ÿո�������ӵ�����������������ʾ
	if (event.getKeySym() == "space" && event.keyDown())
	{
		NextIteration = true;
	}	
}

void IPSG::CPointMatch::print4x4Matrix(const Eigen::Matrix4d & matrix)
{
	printf("Rotation matrix :\n");
	printf("    | %6.3f %6.3f %6.3f | \n", matrix(0, 0), matrix(0, 1), matrix(0, 2));
	printf("R = | %6.3f %6.3f %6.3f | \n", matrix(1, 0), matrix(1, 1), matrix(1, 2));
	printf("    | %6.3f %6.3f %6.3f | \n", matrix(2, 0), matrix(2, 1), matrix(2, 2));
	printf("Translation vector :\n");
	printf("t = < %6.3f, %6.3f, %6.3f >\n\n", matrix(0, 3), matrix(1, 3), matrix(2, 3));
}

bool IPSG::CPointMatch::ICP_Process()
{

	NextIteration = false;
	std::vector<int> indices;
	std::vector<int> indices2;

	PointCloudT::Ptr filtered_cloud(new PointCloudT);   // �˲�Ŀ�����
	PointCloudT::Ptr cloud_tr(new PointCloudT);         // ת����ĵ���
	PointCloudT::Ptr cloud_icp(new PointCloudT);        // ICP�������

	pcl::console::TicToc time;                          //����ʱ���¼
	pcl::io::loadPCDFile("./data/srcpcd/bunny.pcd", *m_pTargetCloud);

	//�ӵ������Ƴ�NAN��Ҳ������Ч��
	pcl::removeNaNFromPointCloud(*m_pTargetCloud, *m_pTargetCloud, indices);

	//�����˲�
	pcl::ApproximateVoxelGrid<pcl::PointXYZ> approximate_voxel_filter;
	approximate_voxel_filter.setLeafSize(0.01, 0.01, 0.01);
	approximate_voxel_filter.setInputCloud(m_pTargetCloud);
	approximate_voxel_filter.filter(*filtered_cloud);
	std::cout << "Filtered cloud contains " << filtered_cloud->size() << " data points from bunny.pcd" << std::endl;

	//������ת�����ƽ������Matrix4d��Ϊ4*4�ľ���
	Eigen::Matrix4d transformation_matrix = Eigen::Matrix4d::Identity();  //��ʼ��

	//��ת����Ķ�����Բο� ( https://en.wikipedia.org/wiki/Rotation_matrix)
	double theta = M_PI / 8;                 
	transformation_matrix(0, 0) = cos(theta);
	transformation_matrix(0, 1) = -sin(theta);
	transformation_matrix(1, 0) = sin(theta);
	transformation_matrix(1, 1) = cos(theta);
	transformation_matrix(2, 3) = 0.4;

	pcl::io::loadPCDFile("./data/srcpcd/bunny_cut.pcd", *m_pInputCloud);
	pcl::removeNaNFromPointCloud(*m_pInputCloud, *m_pInputCloud, indices2);

	//��ӡת������ִ�е���ת��
	print4x4Matrix(transformation_matrix);
	pcl::transformPointCloud(*m_pInputCloud, *cloud_icp, transformation_matrix);

	*cloud_tr = *cloud_icp;                     //����cloud_icp��ֵ��cloud_trΪ����ʹ��

	//����������㷨
	time.tic();                                 //ʱ��
	pcl::IterativeClosestPoint<PointT, PointT> icp;
	icp.setMaximumIterations(m_nIterations);    //��������������iterations=true
	icp.setInputSource(cloud_icp);              //��������ĵ���
	icp.setInputTarget(filtered_cloud);         //Ŀ�����
	icp.align(*cloud_icp);                      //ƥ���Դ����
	icp.setMaximumIterations(1);                //����Ϊ1�Ա��´ε���
	std::cout << "Applied " << m_nIterations << " ICP iteration(s) in " << time.toc() << " ms" << std::endl;

	if (icp.hasConverged())                     //icp.hasConverged ()=1��true������任������ʺ�������
	{
		std::cout << "\nICP has converged, score is " << icp.getFitnessScore() << std::endl;
		std::cout << "\nICP transformation " << m_nIterations << " : cloud_icp -> m_pTargetCloud" << std::endl;
		transformation_matrix = icp.getFinalTransformation().cast<double>();
		print4x4Matrix(transformation_matrix);
	}
	else
	{
		PCL_ERROR("\nICP has not converged.\n");
		return (-1);
	}


	// ���ӻ�ICP�Ĺ�������
	pcl::visualization::PCLVisualizer viewer("ICP demo");

	// ���������۲��ӵ�
	int v1(0);
	int v2(1);
	viewer.createViewPort(0.0, 0.0, 0.5, 1.0, v1);
	viewer.createViewPort(0.5, 0.0, 1.0, 1.0, v2);

	// ������ʾ����ɫ��Ϣ
	float bckgr_gray_level = 0.0;
	float txt_gray_lvl = 1.0 - bckgr_gray_level;

	// ԭʼ�ĵ�������Ϊ��ɫ��
	pcl::visualization::PointCloudColorHandlerCustom<PointT> cloud_in_color_h(filtered_cloud, (int)255 * txt_gray_lvl, (int)255 * txt_gray_lvl,(int)255 * txt_gray_lvl);
	viewer.addPointCloud(filtered_cloud, cloud_in_color_h, "cloud_in_v1", v1);//����ԭʼ�ĵ��ƶ�����ʾΪ��ɫ
	viewer.addPointCloud(filtered_cloud, cloud_in_color_h, "cloud_in_v2", v2);

	// ת����ĵ�����ʾΪ��ɫ
	pcl::visualization::PointCloudColorHandlerCustom<PointT> cloud_tr_color_h(cloud_tr, 20, 180, 20);
	viewer.addPointCloud(cloud_tr, cloud_tr_color_h, "cloud_tr_v1", v1);

	// ICP��׼��ĵ���Ϊ��ɫ
	pcl::visualization::PointCloudColorHandlerCustom<PointT> cloud_icp_color_h(cloud_icp, 180, 20, 20);
	viewer.addPointCloud(cloud_icp, cloud_icp_color_h, "cloud_icp_v2", v2);

	// �����ı��������ڸ��Ե��ӿڽ���
	//��ָ���ӿ�viewport=v1����ַ�����white ������������"icp_info_1"������ַ�����ID��־����10��15��Ϊ����16Ϊ�ַ���С ����ֱ���RGBֵ
	viewer.addText("White: Original point cloud\nGreen: Matrix transformed point cloud", 10, 15, 16, txt_gray_lvl, txt_gray_lvl, txt_gray_lvl, "icp_info_1", v1);
	viewer.addText("White: Original point cloud\nRed: ICP aligned point cloud", 10, 15, 16, txt_gray_lvl, txt_gray_lvl, txt_gray_lvl, "icp_info_2", v2);

	std::stringstream ss;
	ss << m_nIterations;            //����ĵ����Ĵ���
	std::string iterations_cnt = "ICP iterations = " + ss.str();
	viewer.addText(iterations_cnt, 10, 60, 16, txt_gray_lvl, txt_gray_lvl, txt_gray_lvl, "iterations_cnt", v2);

	// ���ñ�����ɫ
	viewer.setBackgroundColor(bckgr_gray_level, bckgr_gray_level, bckgr_gray_level, v1);
	viewer.setBackgroundColor(bckgr_gray_level, bckgr_gray_level, bckgr_gray_level, v2);

	// �������������ͷ���
	viewer.setCameraPosition(-3.68332, 2.94092, 5.71266, 0.289847, 0.921947, -0.256907, 0);
	viewer.setSize(1280, 512);  // ���ӻ����ڵĴ�С

	// ע�ᰴ���ص�����
	IPSG::CPointMatch  PointMatch;
	viewer.registerKeyboardCallback(&(PointMatch.keyboardEventOccurred), (void*)NULL);

	// ��ʾ
	while (!viewer.wasStopped())
	{
		viewer.spinOnce();                 
		//���¿ո���ĺ���
		if (NextIteration)
		{
			time.tic();
			icp.align(*cloud_icp);
			std::cout << "Applied 1 ICP iteration in " << time.toc() << " ms" << std::endl;

			if (icp.hasConverged())
			{
				std::cout << "\nICP transformation " << ++m_nIterations << " : cloud_icp -> cloud_in" << std::endl;
				transformation_matrix *= icp.getFinalTransformation().cast<double>();
				print4x4Matrix(transformation_matrix);

				ss.str("");
				ss << m_nIterations;
				std::string iterations_cnt = "ICP iterations = " + ss.str();
				viewer.updateText(iterations_cnt, 10, 60, 16, txt_gray_lvl, txt_gray_lvl, txt_gray_lvl, "iterations_cnt");
				viewer.updatePointCloud(cloud_icp, cloud_icp_color_h, "cloud_icp_v2");
			}
			else
			{
				PCL_ERROR("\nICP has not converged.\n");
				return (-1);
			}

			if (m_nIterations>40)
			{
				break;
			}
			else
			{
				continue;
			}
		}
		NextIteration = false;
	}

	return true;
}