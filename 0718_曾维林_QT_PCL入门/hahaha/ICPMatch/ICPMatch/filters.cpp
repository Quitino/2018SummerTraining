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

	// 定义读取对象
	pcl::PCDReader reader;
	// 读取点云文件
	reader.read<pcl::PointXYZ>("./data/srcpcd/table_400.pcd", *cloud);

	std::cout << "Loaded " << cloud->width * cloud->height << " data points from " << "pcd files!" << std::endl;

	std::cerr << "Cloud before filtering: " << std::endl;
	std::cerr << *cloud << std::endl;

	start = clock();
	// 创建滤波器，对每个点分析的临近点的个数设置为50 ，并将标准差的倍数设置为1  这意味着如果一
	//个点的距离超出了平均距离一个标准差以上，则该点被标记为离群点，并将它移除，存储起来
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;   //创建滤波器对象
	sor.setInputCloud(cloud);                           //设置待滤波的点云
	sor.setMeanK(50);                                   //设置在进行统计时考虑查询点临近点数
	sor.setStddevMulThresh(1.0);                        //设置判断是否为离群点的阀值
	sor.filter(*cloud_filtered);                        //存储

	std::cout << "Loaded " << cloud_filtered->width * cloud_filtered->height << " data points from " << "pcd files!" << std::endl;
	end = clock();

	std::cout << "Computation ended!" << std::endl;
	std::cout << (end - start) / CLOCKS_PER_SEC << "s used!" << std::endl;

	// pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;  //创建滤波器

	// outrem.setInputCloud(cloud);     //设置输入点云
	// outrem.setRadiusSearch(20);     //设置半径为0.8的范围内找临近点
	// outrem.setMinNeighborsInRadius (20); //设置查询点的邻域点集数小于2的删除
	// // apply filter
	// outrem.filter (*cloud_filtered);     //执行条件滤波   在半径为0.8 在此半径内必须要有两个邻居点，此点才会保存

	std::cerr << "Cloud after filtering: " << std::endl;
	std::cerr << *cloud_filtered << std::endl;

	// 初始化点云可视化界面
	boost::shared_ptr<pcl::visualization::PCLVisualizer>viewer_final(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer_final->setBackgroundColor(0, 0, 0);
	//对目标点云着色（红色）并可视化
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>color(cloud_filtered, 255, 0, 0);
	viewer_final->addPointCloud<pcl::PointXYZ>(cloud_filtered, color, "cloud_filtered");
	viewer_final->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud_filtered");

	// 启动可视化
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