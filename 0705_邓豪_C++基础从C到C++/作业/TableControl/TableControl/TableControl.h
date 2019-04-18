#ifndef TABLECONTROL_H_
#define TABLECONTROL_H_
#include<iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

const int open = 1;
const int close = 0;
const int delete0 = -1;

namespace IPSG
{
	byte CodeTable[]=
	{
	};
	
class CTableControl
{
public:
	CTableControl();
	bool write();													//发送指令
	bool AuxiliarySwitchControl(int open_or_close);					//辅助开关控制
	bool PresetPositionControl(int open_or_close_or_delete0);		//预设位置控制
	bool PositionControl();											//定位控制
	bool PositioningSpeedControl();									//定位速度设置
	bool PositionPointInquireControl();								//定位点查询
	bool PositioningSpeedInquireControl();							//定位速度查询
	bool CommandEncode();											//指令编码
	bool CommandDecode();											//指令解码

private:
	cmd_1;
	cmd_2;
	data_1;
	data_2;
}

#endif	
