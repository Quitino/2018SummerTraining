/*
使用统计分析技术，从一个点云数据中集中移除测量噪声点（也就是离群点）比如：激光扫描通常会产生密度不均匀的点云数据集，
另外测量中的误差也会产生稀疏的离群点，使效果不好，估计局部点云特征（例如采样点处法向量或曲率变化率）的运算复杂，
这会导致错误的数值，反过来就会导致点云配准等后期的处理失败。

解决办法：每个点的邻域进行一个统计分析，并修剪掉一些不符合一定标准的点，
稀疏离群点移除方法基于在输入数据中对点到临近点的距离分布的计算，对每一个点，
计算它到它的所有临近点的平均距离，假设得到的结果是一个高斯分布，其形状是由均值和标准差决定，
平均距离在标准范围之外的点，可以被定义为离群点并可从数据中去除。
*/

#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/statistical_outlier_removal.h>

namespace IPSG
{
	class CPointFilters
	{
	public:
		CPointFilters();
		~CPointFilters();

		bool FiltersProcess();
	};
}
#endif