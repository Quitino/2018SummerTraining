#include <iostream>
#include <vector>
#include "tablecontrol.h"

//�������ؿ���
bool IPSG::CTableControl::AuxiliarySwitchControl(int open_or_close)
{
	if (open_or_close)
	{
		CommandEncode();
		write();
	}
	else
	{
		CommandEncode();
		write();
	}
	return true;
}

//Ԥ��λ�ÿ���
bool IPSG::CTableControl::PresetPositionControl(int open_or_close_or_delete0)
{
	if (open_or_close_or_delete0)
	{
		CommandEncode();
		write();
	}
	else if (open_or_close_or_delete0 == 0)
	{
		CommandEncode();
		write();
	}
	else(open_or_close_or_delete0 == -1)
	{
		CommandEncode();
		write();
	}

	return true;

}
//��λ����
bool IPSG::CTableControl::PositionControl()
{



	return true;

}
//��λ�ٶ�����
bool IPSG::CTableControl::PositioningSpeedControl()
{



	return true;
}
//��λ���ѯ
bool IPSG::CTableControl::PositionPointInquireControl()
{



	return true;
}
//��λ�ٶȲ�ѯ
bool IPSG::CTableControl::PositioningSpeedInquireControl()
{


	return true;

}