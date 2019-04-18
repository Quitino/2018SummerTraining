#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>      //贪婪投影三角化算法
#include <pcl/visualization/pcl_visualizer.h>

#include <iostream>
#include <boost/thread/thread.hpp>

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;   //定义点云的格式

namespace IPSG
{
	class CTriangulation
	{
	public:
		CTriangulation();
		~CTriangulation();

		bool PointTriangulation();

	private:
	};
}
#endif