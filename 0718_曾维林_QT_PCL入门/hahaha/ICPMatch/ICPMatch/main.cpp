#include "header.h"

int main()
{
	IPSG::CPointProcess PointProcess;

	//ICP������׼ʹ�á������������˲�
	//PointProcess.pointmatch.ICP_Process();

	//̰�����Ƿ�����ʹ��
	PointProcess.Triangulation.PointTriangulation();

	//�˲�ʹ�á�������ͳ�Ʒ������˲������Ա������˲�
	//PointProcess.PointFilters.FiltersProcess();

	//ͶӰ����ʹ�á����Ա���������
	//PointProcess.PointGrid.Grid_Projection();

	return 0;
}