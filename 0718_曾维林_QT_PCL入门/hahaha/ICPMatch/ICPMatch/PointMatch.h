#ifndef POINTMATCH_H
#define POINTMATCH_H

#include <iostream>                 //标准输入输出头文件
#include <string>

#include <pcl/io/ply_io.h>               //PLY相关头文件
#include <pcl/io/pcd_io.h>               //PCD文件打开存储类头文件
#include <pcl/point_types.h>             //点类型定义头文件
#include <pcl/registration/icp.h>        //ICP配准类相关头文件
#include <pcl/filters/approximate_voxel_grid.h>   //滤波类头文件  （使用体素网格过滤器处理的效果比较好）
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/time.h>  

static bool  NextIteration;

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;   //定义点云的格式

namespace IPSG
{
	class CPointMatch
	{
	public:
		CPointMatch();
		~CPointMatch();

		bool ICP_Process();
		void print4x4Matrix(const Eigen::Matrix4d & matrix);
		static void keyboardEventOccurred(const pcl::visualization::KeyboardEvent& event, void* nothing);

	private:
		PointCloudT::Ptr               m_pTargetCloud;
		PointCloudT::Ptr               m_pInputCloud;

		int                            m_nIterations;   // 默认的ICP迭代次数
	};
}
#endif