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
	bool write();													//����ָ��
	bool AuxiliarySwitchControl(int open_or_close);					//�������ؿ���
	bool PresetPositionControl(int open_or_close_or_delete0);		//Ԥ��λ�ÿ���
	bool PositionControl();											//��λ����
	bool PositioningSpeedControl();									//��λ�ٶ�����
	bool PositionPointInquireControl();								//��λ���ѯ
	bool PositioningSpeedInquireControl();							//��λ�ٶȲ�ѯ
	bool CommandEncode();											//ָ�����
	bool CommandDecode();											//ָ�����

private:
	cmd_1;
	cmd_2;
	data_1;
	data_2;
}

#endif	
