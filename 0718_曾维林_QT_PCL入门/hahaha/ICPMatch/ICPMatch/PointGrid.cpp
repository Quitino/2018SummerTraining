#include "PointGrid.h"

IPSG::CPointGrid::CPointGrid()
{

}

IPSG::CPointGrid::~CPointGrid()
{

}

bool IPSG::CPointGrid::Grid_Projection()
{
	PointCloud::Ptr cloud(new PointCloud);
	pcl::io::loadPCDFile("./data/srcpcd/bunny.pcd", *cloud);
	std::cout << "Loaded " << cloud->width * cloud->height << " data points from " << "pcd files!" << std::endl;

	// Create a timer  
	clock_t start, end;

	// Normal estimation*  
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);

	tree->setInputCloud(cloud);
	ne.setInputCloud(cloud);
	ne.setSearchMethod(tree);
	ne.setKSearch(20);
	ne.compute(*normals);
	//* normals should not contain the point normals + surface curvatures  

	// Concatenate the XYZ and normal fields*  
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
	pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);
	//* cloud_with_normals = cloud + normals  

	std::cout << "normals computed!" << std::endl;

	// Create search tree*  
	pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
	tree2->setInputCloud(cloud_with_normals);

	// Initialize objects  
	pcl::GridProjection<pcl::PointNormal> gp;
	pcl::PolygonMesh mesh;
	std::cout << "Computation began!" << std::endl;
	gp.setInputCloud(cloud_with_normals);
	gp.setSearchMethod(tree2);
	gp.setResolution(0.001);// 输入立方体的边长，这个长度和输入  
	//点云密度有关，其值应与点与点之间的平均距离接近  
	//setNearestNeighborNum(2) k临近点搜索代替上述的Padding，一般2和3就可以了。  
	gp.setPaddingSize(3);//与setPaddingSize()函数二选一使用，不过该函数比较稳定  
	gp.setMaxBinarySearchLevel(11);//二分法搜索投影位置，输入为最大搜索深度  
	start = clock();
	gp.reconstruct(mesh);
	end = clock();

	std::cout << "Computation ended!" << std::endl;
	std::cout << (end - start) / CLOCKS_PER_SEC << "s used!" << std::endl;

	pcl::io::savePLYFile("./data/resultpcd/grid.ply", mesh);
	pcl::visualization::PCLVisualizer viewer("viewer");

	viewer.addPolygonMesh(mesh);
	viewer.spin();

	return true;
}