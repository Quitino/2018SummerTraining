#include "filters.h"

IPSG::CPointFilters::CPointFilters()
{

}

IPSG::CPointFilters::~CPointFilters()
{

}

bool IPSG::CPointFilters::FiltersProcess()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	clock_t start, end;

	// �����ȡ����
	pcl::PCDReader reader;
	// ��ȡ�����ļ�
	reader.read<pcl::PointXYZ>("./data/srcpcd/table_400.pcd", *cloud);

	std::cout << "Loaded " << cloud->width * cloud->height << " data points from " << "pcd files!" << std::endl;

	std::cerr << "Cloud before filtering: " << std::endl;
	std::cerr << *cloud << std::endl;

	start = clock();
	// �����˲�������ÿ����������ٽ���ĸ�������Ϊ50 ��������׼��ı�������Ϊ1  ����ζ�����һ
	//����ľ��볬����ƽ������һ����׼�����ϣ���õ㱻���Ϊ��Ⱥ�㣬�������Ƴ����洢����
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;   //�����˲�������
	sor.setInputCloud(cloud);                           //���ô��˲��ĵ���
	sor.setMeanK(50);                                   //�����ڽ���ͳ��ʱ���ǲ�ѯ���ٽ�����
	sor.setStddevMulThresh(1.0);                        //�����ж��Ƿ�Ϊ��Ⱥ��ķ�ֵ
	sor.filter(*cloud_filtered);                        //�洢

	std::cout << "Loaded " << cloud_filtered->width * cloud_filtered->height << " data points from " << "pcd files!" << std::endl;
	end = clock();

	std::cout << "Computation ended!" << std::endl;
	std::cout << (end - start) / CLOCKS_PER_SEC << "s used!" << std::endl;

	// pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;  //�����˲���

	// outrem.setInputCloud(cloud);     //�����������
	// outrem.setRadiusSearch(20);     //���ð뾶Ϊ0.8�ķ�Χ�����ٽ���
	// outrem.setMinNeighborsInRadius (20); //���ò�ѯ�������㼯��С��2��ɾ��
	// // apply filter
	// outrem.filter (*cloud_filtered);     //ִ�������˲�   �ڰ뾶Ϊ0.8 �ڴ˰뾶�ڱ���Ҫ�������ھӵ㣬�˵�Żᱣ��

	std::cerr << "Cloud after filtering: " << std::endl;
	std::cerr << *cloud_filtered << std::endl;

	// ��ʼ�����ƿ��ӻ�����
	boost::shared_ptr<pcl::visualization::PCLVisualizer>viewer_final(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer_final->setBackgroundColor(0, 0, 0);
	//��Ŀ�������ɫ����ɫ�������ӻ�
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>color(cloud_filtered, 255, 0, 0);
	viewer_final->addPointCloud<pcl::PointXYZ>(cloud_filtered, color, "cloud_filtered");
	viewer_final->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud_filtered");

	// �������ӻ�
	viewer_final->addCoordinateSystem(1.0);
	viewer_final->initCameraParameters();

	pcl::PCDWriter writer;
	writer.write<pcl::PointXYZ>("./data/resultpcd/table_scene_lms400_inliers.pcd", *cloud_filtered, false);

	sor.setNegative(true);
	sor.filter(*cloud_filtered);
	writer.write<pcl::PointXYZ>("./data/resultpcd/table_scene_lms400_outliers.pcd", *cloud_filtered, false);

	while (!viewer_final->wasStopped())
	{
		viewer_final->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	return true;
}