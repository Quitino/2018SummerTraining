#include "Triangulation.h"

IPSG::CTriangulation::CTriangulation()
{

}

IPSG::CTriangulation::~CTriangulation()
{

}


bool IPSG::CTriangulation::PointTriangulation()
{
	// ��һ��XYZ�����͵�PCD�ļ��򿪲��洢��������
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PCLPointCloud2 cloud_blob;
	pcl::io::loadPCDFile("./data/srcpcd/bunny.pcd", cloud_blob);
	pcl::fromPCLPointCloud2(cloud_blob, *cloud);
	//* the data should be available in cloud

	// Normal estimation*
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;      //���߹��ƶ���
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);   //�洢���Ƶķ���
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);  //����kd��ָ��
	tree->setInputCloud(cloud);   ///��cloud����tree����
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	n.setKSearch(20);
	n.compute(*normals);       ////���Ʒ��ߴ洢������
	//* normals should not contain the point normals + surface curvatures

	// Concatenate the XYZ and normal fields*
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
	pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);    //�����ֶ�
	//* cloud_with_normals = cloud + normals

	//��������������
	pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
	tree2->setInputCloud(cloud_with_normals);   //���ƹ���������

	// Initialize objects
	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;   //�������ǻ�����
	pcl::PolygonMesh triangles;                //�洢�������ǻ�������ģ��

	//��һ������Ӱ��ܴ�  
	gp3.setSearchRadius(0.025);  //�������ӵ�֮��������룬���������������߳���

	// ���ø�����ֵ
	gp3.setMu(2.5);  //���ñ���������������ڵ����Զ����Ϊ2.5��Ϊ��ʹ�õ����ܶȵı仯
	gp3.setMaximumNearestNeighbors(100);    //������������������������
	gp3.setMaximumSurfaceAngle(M_PI / 4); // ����ĳ�㷨�߷���ƫ�������㷨�ߵ����Ƕ�45
	gp3.setMinimumAngle(M_PI / 18); // �������ǻ���õ����������ڽǵ���С�ĽǶ�Ϊ10
	gp3.setMaximumAngle(2 * M_PI / 3); // �������ǻ���õ����������ڽǵ����Ƕ�Ϊ120
	gp3.setNormalConsistency(false);  //���øò�����֤���߳���һ��

	// Get result
	gp3.setInputCloud(cloud_with_normals);     //�����������Ϊ�������
	gp3.setSearchMethod(tree2);   //����������ʽ
	gp3.reconstruct(triangles);  //�ؽ���ȡ���ǻ�

	pcl::io::savePLYFile("./data/resultpcd/triangles.ply", triangles);

	// ���Ӷ�����Ϣ
	std::vector<int> parts = gp3.getPartIDs();
	std::vector<int> states = gp3.getPointStates();

	// ��ʾ���ͼ
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	//viewer->setRepresentationToSurfaceForAllActors(); //����ģ������Ƭ��ʽ��ʾ    
	viewer->setRepresentationToPointsForAllActors(); //����ģ���Ե���ʽ��ʾ    
	//viewer->setRepresentationToWireframeForAllActors();  //����ģ�����߿�ͼģʽ��ʾ
	viewer->setBackgroundColor(0, 0, 0); //���ñ���
	viewer->addPolygonMesh(triangles, "my"); //������ʾ������
	//viewer->addCoordinateSystem (1.0); //��������ϵ
	viewer->initCameraParameters();

	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
	// Finish
	return 0;
}