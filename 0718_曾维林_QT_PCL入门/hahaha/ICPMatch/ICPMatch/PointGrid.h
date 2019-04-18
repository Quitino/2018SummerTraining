#ifndef POINTGRID_H
#define POINTGRID_H

#include <pcl/point_types.h>  
#include <pcl/io/pcd_io.h>  
#include <pcl/io/ply_io.h>
#include <pcl/kdtree/kdtree_flann.h>  
#include <pcl/features/normal_3d.h>  
#include <pcl/surface/grid_projection.h>  
#include <iostream>  
#include <string.h>  
#include <pcl/visualization/pcl_visualizer.h>  
#include <time.h>  

typedef pcl::PointXYZ PointT; 
typedef pcl::PointCloud<PointT> PointCloud;

namespace IPSG
{
	class CPointGrid
	{
	public:
		CPointGrid();
		~CPointGrid();

		bool Grid_Projection();

	private:
	};
}
#endif