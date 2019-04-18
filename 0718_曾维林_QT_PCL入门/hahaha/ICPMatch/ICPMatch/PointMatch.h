#ifndef POINTMATCH_H
#define POINTMATCH_H

#include <iostream>                 //��׼�������ͷ�ļ�
#include <string>

#include <pcl/io/ply_io.h>               //PLY���ͷ�ļ�
#include <pcl/io/pcd_io.h>               //PCD�ļ��򿪴洢��ͷ�ļ�
#include <pcl/point_types.h>             //�����Ͷ���ͷ�ļ�
#include <pcl/registration/icp.h>        //ICP��׼�����ͷ�ļ�
#include <pcl/filters/approximate_voxel_grid.h>   //�˲���ͷ�ļ�  ��ʹ��������������������Ч���ȽϺã�
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/time.h>  

static bool  NextIteration;

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;   //������Ƶĸ�ʽ

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

		int                            m_nIterations;   // Ĭ�ϵ�ICP��������
	};
}
#endif