#include "header.h"

int main()
{
	IPSG::CPointProcess PointProcess;

	//ICP点云配准使用——包含体素滤波
	//PointProcess.pointmatch.ICP_Process();

	//贪婪三角法网格化使用
	PointProcess.Triangulation.PointTriangulation();

	//滤波使用——基于统计方法的滤波——对比体素滤波
	//PointProcess.PointFilters.FiltersProcess();

	//投影网格化使用——对比三角网格化
	//PointProcess.PointGrid.Grid_Projection();

	return 0;
}