#include <iostream>
#include <vector>
#include "tablecontrol.h"

//辅助开关控制
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

//预设位置控制
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
//定位控制
bool IPSG::CTableControl::PositionControl()
{



	return true;

}
//定位速度设置
bool IPSG::CTableControl::PositioningSpeedControl()
{



	return true;
}
//定位点查询
bool IPSG::CTableControl::PositionPointInquireControl()
{



	return true;
}
//定位速度查询
bool IPSG::CTableControl::PositioningSpeedInquireControl()
{


	return true;

}