/*
ʹ��ͳ�Ʒ�����������һ�����������м����Ƴ����������㣨Ҳ������Ⱥ�㣩���磺����ɨ��ͨ��������ܶȲ����ȵĵ������ݼ���
��������е����Ҳ�����ϡ�����Ⱥ�㣬ʹЧ�����ã����ƾֲ�������������������㴦�����������ʱ仯�ʣ������㸴�ӣ�
��ᵼ�´������ֵ���������ͻᵼ�µ�����׼�Ⱥ��ڵĴ���ʧ�ܡ�

����취��ÿ������������һ��ͳ�Ʒ��������޼���һЩ������һ����׼�ĵ㣬
ϡ����Ⱥ���Ƴ��������������������жԵ㵽�ٽ���ľ���ֲ��ļ��㣬��ÿһ���㣬
�����������������ٽ����ƽ�����룬����õ��Ľ����һ����˹�ֲ�������״���ɾ�ֵ�ͱ�׼�������
ƽ�������ڱ�׼��Χ֮��ĵ㣬���Ա�����Ϊ��Ⱥ�㲢�ɴ�������ȥ����
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