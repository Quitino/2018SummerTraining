#include "SerialTable.h"

bool IPSG::CSerialTableControl::CalCheckSum(std::vector<BYTE> ucData, BYTE &CheckSum)
{
	if ((ucData.begin() == ucData.end()) || ucData.empty())
	{
		FILE_LOG(logDEBUG) << "Error: empty data";
		return false;
	}

	CheckSum = 0x00;
	std::vector<BYTE>::iterator itBegin = ucData.begin();
	for (std::vector<BYTE>::iterator it = itBegin + 1; it < itBegin + 6;it++)
	{
		CheckSum += *it;
	}
	CheckSum = (CheckSum & 0xFF);

	return true;
}

bool IPSG::CSerialTableControl::Int2UShort(const int nIntNO, BYTE &ucDataH, BYTE &ucDataL)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::Int2UShort() call";

	int nIntNOTemp = nIntNO;
	ucDataH = nIntNOTemp >> 8;			//��λ
	ucDataL = nIntNOTemp;

	FILE_LOG(logDEBUG) << "CSerialTableControl::Int2UShort() return";
	return true;
}

bool IPSG::CSerialTableControl::ECode(BYTE ucCommand2, BYTE ucData1, BYTE ucData2, std::vector<BYTE> &v_ucEcodeData)
{
	v_ucEcodeData.resize(7);

	v_ucEcodeData[0] = 0xFF;
	v_ucEcodeData[1] = 0x01;
	v_ucEcodeData[2] = 0x00;
	v_ucEcodeData[3] = ucCommand2;
	v_ucEcodeData[4] = ucData1;
	v_ucEcodeData[5] = ucData2;

	BYTE ucCheckSum;
	CalCheckSum(v_ucEcodeData, ucCheckSum);
	v_ucEcodeData[6] = ucCheckSum;

	return true;
}

bool IPSG::CSerialTableControl::TableDecode()
{
	std::vector<unsigned char > ReceivedFrame(m_vucReceiveBuff);

	//֡ͷ����ɾ��
	while ((ReceivedFrame.begin() != ReceivedFrame.end()) && (ReceivedFrame.at(0) != 0xFF))
	{
		ReceivedFrame.erase(ReceivedFrame.begin());
	}

	//�п�
	std::vector<unsigned char>::iterator vcitor = ReceivedFrame.begin();
	if (vcitor == ReceivedFrame.end())
	{
		FILE_LOG(logINFO) << "Table's data is empty";
		ReceivedFrame.clear();
		return false;
	}

	//֡��
	if (7 != ReceivedFrame.size())
	{
		FILE_LOG(logINFO) << "The length of table's data error";
		ReceivedFrame.clear();
		return false;
	}

	//֡ͷУ��
	if ((*vcitor != 0XFF))
	{
		FILE_LOG(logINFO) << "The head of table's data error";
		ReceivedFrame.clear();
		return false;
	}

	//��ַ�ж�
	std::vector<unsigned char>::iterator itAddr = vcitor + 1;
	if (*itAddr != 0X01)
	{
		FILE_LOG(logINFO) << "Table's address error";
		ReceivedFrame.clear();
		return false;
	}

	//Command1Ϊ���ж�
	std::vector<unsigned char>::iterator itCommand1 = vcitor + 2;
	if (*itCommand1 != 0X00)
	{
		FILE_LOG(logINFO) << "Table's command1 error";
		ReceivedFrame.clear();
		return false;
	}

	//У���У��
	BYTE TableCheckSum;
	CalCheckSum(ReceivedFrame, TableCheckSum);
	if (TableCheckSum != ReceivedFrame.at(6))
	{
		//m_TableDataErrorNum++;
		FILE_LOG(logINFO) << "Check sum error!";
		ReceivedFrame.clear();
		return false;
	}

	//ȡ����
	std::vector<unsigned char>::iterator itCommand2 = vcitor + 3;
	BYTE ucData1 = ReceivedFrame.at(4);
	BYTE ucData2 = ReceivedFrame.at(5);
	switch (*itCommand2)
	{
	case CMD_BACK_H_ANGLE:
		CalCurrentHoriAngle(ucData1, ucData2);	//ȡ���ݵ�ˮƽ�Ƕȷ�������
		break;
	case CMD_BACK_V_ANGLE:
		CalCurrentVertAngle(ucData1, ucData2);	//ȡ���ݵ������Ƕȷ�������
		break;
	case CMD_BACK_LOC_SPEED:
		CalCurrentLocSpeed(ucData1, ucData2);	//ȡ���ݵ���λ�ٶȷ�������
		break;
	default:
	{
		FILE_LOG(logINFO) << "Table's command2 error";
		ReceivedFrame.clear();
		return false;
	}
	}
	ReceivedFrame.clear();

	return true;
}

bool IPSG::CSerialTableControl::SerialWriteVector(std::vector<BYTE> v_ucSendData)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::SerialWriteVector() call";

	std::vector<BYTE> v_ucSendData_(v_ucSendData);

	if (7 != v_ucSendData_.size())
	{
		FILE_LOG(logINFO) << "Data for sending is error";
		return false;
	}

	BYTE ucSendData[7];
	for (size_t i = 0; i < 7; i++)
	{
		ucSendData[i] = v_ucSendData_[i];
	}
	WriteData(ucSendData, 7);

	FILE_LOG(logDEBUG) << "CSerialTableControl::SerialWriteVector() return";
	return true;
}

bool IPSG::CSerialTableControl::OpenAuxN(int nAuxNO)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::OpenAuxN() call";

	BYTE ucAuxNOH, ucAuxNOL;
	std::vector<BYTE> v_ucEcodeData;
	Int2UShort(nAuxNO, ucAuxNOH, ucAuxNOL);
	if (0x00 != ucAuxNOH)
	{
		FILE_LOG(logINFO) << "Do not have " << nAuxNO << "th Aux";
		return false;
	}
	
	v_ucEcodeData.clear();
	ECode(CMD_OPEN_AUX, ucAuxNOH, ucAuxNOL, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::OpenAuxN() return";
	return true;
}

bool IPSG::CSerialTableControl::CloseAuxN(int nAuxNO)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::CloseAuxN() call";

	BYTE ucAuxNOH, ucAuxNOL;
	std::vector<BYTE> v_ucEcodeData;
	Int2UShort(nAuxNO, ucAuxNOH, ucAuxNOL);
	if (0x00 != ucAuxNOH)
	{
		FILE_LOG(logINFO) << "Do not have " << nAuxNO << "th Aux";
		return false;
	}

	v_ucEcodeData.clear();
	ECode(CMD_CLOSE_AUX, ucAuxNOH, ucAuxNOL, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::CloseAuxN() return";
	return true;
}

bool IPSG::CSerialTableControl::SetPresetN(int nPresetNO)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::SetPresetN() call";

	BYTE ucPresetNOH, ucPresetNOL;
	std::vector<BYTE> v_ucEcodeData;
	Int2UShort(nPresetNO, ucPresetNOH, ucPresetNOL);
	if (0x00 != ucPresetNOH)
	{
		FILE_LOG(logINFO) << "Do not have " << nPresetNO << "th Preset";
		return false;
	}

	v_ucEcodeData.clear();
	ECode(CMD_SET_PRESET, ucPresetNOH, ucPresetNOL, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::SetPresetN() return";
	return true;
}

bool IPSG::CSerialTableControl::ClearPresetN(int nPresetNO)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::ClearPresetN() call";

	BYTE ucPresetNOH, ucPresetNOL;
	std::vector<BYTE> v_ucEcodeData;
	Int2UShort(nPresetNO, ucPresetNOH, ucPresetNOL);
	if (0x00 != ucPresetNOH)
	{
		FILE_LOG(logINFO) << "Do not have " << nPresetNO << "th Preset";
		return false;
	}

	v_ucEcodeData.clear();
	ECode(CMD_CLEAR_PRESET, ucPresetNOH, ucPresetNOL, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::ClearPresetN() return";
	return true;
}

bool IPSG::CSerialTableControl::CallPresetN(int nPresetNO)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::CallPresetN() call";

	BYTE ucPresetNOH, ucPresetNOL;
	std::vector<BYTE> v_ucEcodeData;
	Int2UShort(nPresetNO, ucPresetNOH, ucPresetNOL);
	if (0x00 != ucPresetNOH)
	{
		FILE_LOG(logINFO) << "Do not have " << nPresetNO << "th Preset";
		return false;
	}

	v_ucEcodeData.clear();
	ECode(CMD_CALL_PRESET, ucPresetNOH, ucPresetNOL, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::CallPresetN() return";
	return true;
}

bool IPSG::CSerialTableControl::HoriLocate(float fLocAngle)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::HoriLocate() call";

	//�˴�������Ϲ����Ƕȵķ�Χ�ж�
	int nLocAngle = static_cast<int>(fLocAngle * 100);
	BYTE ucLocAngleH, ucLocAngleL;
	std::vector<BYTE> v_ucEcodeData;
	v_ucEcodeData.clear();

	Int2UShort(nLocAngle, ucLocAngleH, ucLocAngleL);
	ECode(CMD_LOC_H_ANGLE, ucLocAngleH, ucLocAngleL, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::HoriLocate() return";
	return true;
}

bool IPSG::CSerialTableControl::VertLocate(float fLocAngle)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::VertLocate() call";

	//�˴�������Ϲ����Ƕȵķ�Χ�ж�
	int nLocAngle = static_cast<int>(fLocAngle * 100);
	BYTE ucLocAngleH, ucLocAngleL;
	std::vector<BYTE> v_ucEcodeData;
	v_ucEcodeData.clear();

	Int2UShort(nLocAngle, ucLocAngleH, ucLocAngleL);
	ECode(CMD_LOC_V_ANGLE, ucLocAngleH, ucLocAngleL, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::VertLocate() return";
	return true;
}

bool IPSG::CSerialTableControl::HVLocate(float fHLocAngle, float fVLocAngle)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::HVLocate() call";

	if (m_bHorizonAboveVertical)
	{
		HoriLocate(fHLocAngle);
		VertLocate(fVLocAngle);
	}
	else
	{
		VertLocate(fVLocAngle);
		HoriLocate(fHLocAngle);
	}
	FILE_LOG(logDEBUG) << "CSerialTableControl::HVLocate() return";
	return true;
}

bool IPSG::CSerialTableControl::SetLocSpeed(int nHSpeed, int nVSpeed)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::SetLocSpeed() call";

	if (nHSpeed > 63 ||nHSpeed < 0
		|| nVSpeed > 63 || nVSpeed < 0)
	{
		FILE_LOG(logINFO) << "The speed for locate error";
		return false;
	}

	std::vector<BYTE> v_ucEcodeData;
	v_ucEcodeData.clear();

	BYTE ucHSpeed, ucVSpeed;
	ucHSpeed = nHSpeed;
	ucVSpeed = nVSpeed;
	ECode(CMD_SET_LOC_SPEED, ucHSpeed, ucVSpeed, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::SetLocSpeed() return";
	return true;
}

bool IPSG::CSerialTableControl::ResetTable()
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::ResetTable() call";

	std::vector<BYTE> v_ucEcodeData;
	v_ucEcodeData.clear();
	ECode(CMD_RESET_TABLE, 0x00, 0x00, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::ResetTable() return";
	return true;
}

bool IPSG::CSerialTableControl::AskMessage(const BYTE ucCommand2)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::AskMessage() call";

	std::vector<BYTE> v_ucEcodeData;
	v_ucEcodeData.clear();
	ECode(ucCommand2, 0x00, 0x00, v_ucEcodeData);
	SerialWriteVector(v_ucEcodeData);

	FILE_LOG(logDEBUG) << "CSerialTableControl::AskMessage() return";
	return true;
}

bool IPSG::CSerialTableControl::TableStop()
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::TableStop() call";

	AskMessage(CMD_STOP);
	Sleep(30);

	FILE_LOG(logDEBUG) << "CSerialTableControl::TableStop() return";
	return true;
}

bool IPSG::CSerialTableControl::CalCurrentHoriAngle(BYTE ucData1, BYTE ucData2)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::CalCurrentHoriAngle() call";

	int nHoriAngle = (ucData1 << 8) + ucData2;
	m_fCurrentHoriAngle = (static_cast<float>(nHoriAngle)) / 100.0;

	FILE_LOG(logDEBUG) << "CSerialTableControl::CalCurrentHoriAngle() return";
	return true;
}

bool IPSG::CSerialTableControl::CalCurrentVertAngle(BYTE ucData1, BYTE ucData2)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::CalCurrentHoriAngle() call";

	int nVertAngle = (ucData1 << 8) + ucData2;
	m_fCurrentVertAngle = (static_cast<float>(nVertAngle)) / 100.0;

	FILE_LOG(logDEBUG) << "CSerialTableControl::CalCurrentHoriAngle() return";
	return true;
}

bool IPSG::CSerialTableControl::CalCurrentLocSpeed(BYTE ucData1, BYTE ucData2)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::CalCurrentHoriAngle() call";

	m_nCurrentHoriLocSpeed = ucData1;
	m_nCurrentVertLocSpeed = ucData2;

	FILE_LOG(logDEBUG) << "CSerialTableControl::CalCurrentHoriAngle() return";
	return true;
}

bool IPSG::CSerialTableControl::GetHoriAngle(float &fHoriAngle)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::GetHoriAngle() call";
	//FILE_LOG(logINFO) << "��ѯ�Ƕ�ǰ��̨ˮƽ�Ƕ�Ϊ��" << m_fCurrentHoriAngle;

	AskMessage(CMD_ASK_H_ANGLE);
	Sleep(30);

	fHoriAngle = m_fCurrentHoriAngle;

	//FILE_LOG(logINFO) << "��ѯ�ǶȺ���̨ˮƽ�Ƕ�Ϊ��" << fHoriAngle;
	FILE_LOG(logDEBUG) << "CSerialTableControl::GetHoriAngle() return";
	return true;
}

bool IPSG::CSerialTableControl::GetVertAngle(float &fVertAngle)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::GetVertAngle() call";
	//FILE_LOG(logINFO) << "��ѯ�Ƕ�ǰ��̨�����Ƕ�Ϊ��" << m_fCurrentVertAngle;

	AskMessage(CMD_ASK_V_ANGLE);
	Sleep(30);

	fVertAngle = m_fCurrentVertAngle;

	//FILE_LOG(logINFO) << "��ѯ�ǶȺ���̨�����Ƕ�Ϊ��" << fVertAngle;
	FILE_LOG(logDEBUG) << "CSerialTableControl::GetVertAngle() return";
	return true;
}

bool IPSG::CSerialTableControl::GetLocSpeed(int &nHSpeed, int &nVSpeed)
{
	FILE_LOG(logDEBUG) << "CSerialTableControl::GetLocSpeed() call";
	
	AskMessage(CMD_ASK_LOC_SPEED);
	Sleep(20);
	nHSpeed = m_nCurrentHoriLocSpeed;
	nVSpeed = m_nCurrentVertLocSpeed;

	FILE_LOG(logDEBUG) << "CSerialTableControl::GetLocSpeed() return";
	return true;
}

bool IPSG::TableTest()
{
	FILE_LOG(logDEBUG) << "TableTest() call";

	CSerialBase* pTableControl = new CSerialTableControl;

	if (!pTableControl->InitPort(4, 9600))	//���ںźͲ�����
		FILE_LOG(logINFO) << "InitPort fail !";
	else FILE_LOG(logINFO) << "InitPort success !";

	if (!pTableControl->OpenListenThread())
		FILE_LOG(logINFO) << "OpenListenThread fail !";
	else FILE_LOG(logINFO) << "OpenListenThread success !";

	CSerialTableControl* pTableControl2 = dynamic_cast<CSerialTableControl*>(pTableControl);

	//pTableControl2->CallPresetN(101);
	//Sleep(6000);

	pTableControl2->SetLocSpeed(32, 32);   //������̨��Ѳ���ٶȣ����ܳ���63

	//Ѳ�������ݴ洢
	STableCruisePara m_TableCruisePara;
	std::vector<STableLocPoint> m_vTableLocPoint;
	m_vTableLocPoint.clear();
	for (size_t i = 0; i < m_TableCruisePara.m_nTableVertPointNO; i++)	//ˮƽ��ɨ���ٵ���һ��ֱ��
	{
		int nVertLastLocPoint = m_TableCruisePara.m_nTableVertInitLoc - i*m_TableCruisePara.m_nTableLocVertStep;
		for (size_t j = 0; j < m_TableCruisePara.m_nTableHoriPointNO; j++)
		{
			int nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc;
			if (i % 2 == 0)
			{
				nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc + j*m_TableCruisePara.m_nTableLocHoriStep;
			}
			else
			{
				nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc + m_TableCruisePara.m_nTableHoriAngleRange - j*m_TableCruisePara.m_nTableLocHoriStep;
			}
			m_vTableLocPoint.push_back(TableLocPoint(nHoriLastLocPoint, nVertLastLocPoint));
		}
	}
	while (true)
	{
		
		for (size_t i = 0; i < m_vTableLocPoint.size(); i++)
		{
			STableLocPoint TableLocNextPoint(STableLocPoint(m_vTableLocPoint.at(i).m_fHoriLocPoint, m_vTableLocPoint.at(i).m_fVertLocPoint));
			FILE_LOG(logINFO) << "��̨��ʼ��λ��Ŀ�궨λ��Ϊ��(" << m_vTableLocPoint.at(i).m_fHoriLocPoint << "," << m_vTableLocPoint.at(i).m_fVertLocPoint << ")";

			float fNextHoriPoint = m_vTableLocPoint.at(i).m_fHoriLocPoint;
			float fNextVertPoint = m_vTableLocPoint.at(i).m_fVertLocPoint;
		
			float fCurrentHoriPoint = 0.0;   //��̨��ǰ��ˮƽ�Ƕ�
			float fCurrentVertPoint = 0.0;   //��̨��ǰ�ĸ����Ƕ�
			do
			{
				pTableControl2->GetHoriAngle(fCurrentHoriPoint);
				Sleep(30);
				pTableControl2->GetVertAngle(fCurrentVertPoint);
				Sleep(30);
				pTableControl2->HVLocate(fNextHoriPoint, fNextVertPoint);  //��̨��λ��ָ����
			} while (std::abs(fCurrentHoriPoint - fNextHoriPoint) > 1 ||
				std::abs(fCurrentVertPoint - fNextVertPoint) > 1);

			Sleep(300);
		}
		FILE_LOG(logINFO) << "��̨Ѳ�����" << std::endl << std::endl;
	}

	FILE_LOG(logDEBUG) << "TableTest() return";

	return true;
}

bool IPSG::TableTestSimplePoint()
{
	FILE_LOG(logDEBUG) << "TableTest() call";

	CSerialBase* pTableControl = new CSerialTableControl;

	if (!pTableControl->InitPort(7, 9600))	//���ںźͲ�����
		FILE_LOG(logINFO) << "InitPort fail !";
	else FILE_LOG(logINFO) << "InitPort success !";

	if (!pTableControl->OpenListenThread())
		FILE_LOG(logINFO) << "OpenListenThread fail !";
	else FILE_LOG(logINFO) << "OpenListenThread success !";

	CSerialTableControl* pTableControl2 = dynamic_cast<CSerialTableControl*>(pTableControl);

	//pTableControl2->CallPresetN(101);
	//Sleep(6000);

	pTableControl2->SetLocSpeed(32, 32);   //������̨��Ѳ���ٶȣ����ܳ���63

	//STableCruisePara m_TableCruisePara;

	//std::vector<STableLocPoint> m_vTableLocPoint;
	//m_vTableLocPoint.clear();
	//for (size_t i = 0; i < m_TableCruisePara.m_nTableVertPointNO; i++)	//ˮƽ��ɨ���ٵ���һ��ֱ��
	//{
	//	int nVertLastLocPoint = m_TableCruisePara.m_nTableVertInitLoc - i*m_TableCruisePara.m_nTableLocVertStep;
	//	for (size_t j = 0; j < m_TableCruisePara.m_nTableHoriPointNO; j++)
	//	{
	//		int nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc;
	//		if (i % 2 == 0)
	//		{
	//			nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc + j*m_TableCruisePara.m_nTableLocHoriStep;
	//		}
	//		else
	//		{
	//			nHoriLastLocPoint = m_TableCruisePara.m_nTableHoriInitLoc + m_TableCruisePara.m_nTableHoriAngleRange - j*m_TableCruisePara.m_nTableLocHoriStep;
	//		}
	//		m_vTableLocPoint.push_back(TableLocPoint(nHoriLastLocPoint, nVertLastLocPoint));
	//	}
	//}

	STableLocLimit TableLimit;
	float fNextHoriPoint;
	float fNextVertPoint;
	while (std::cin>>fNextHoriPoint)
	{
		std::cin >> fNextVertPoint;
		//for (size_t i = 0; i < m_vTableLocPoint.size(); i++)
		//{
			//STableLocPoint TableLocNextPoint(STableLocPoint(m_vTableLocPoint.at(i).m_fHoriLocPoint, m_vTableLocPoint.at(i).m_fVertLocPoint));
			//FILE_LOG(logINFO) << "��̨��ʼ��λ��Ŀ�궨λ��Ϊ��(" << m_vTableLocPoint.at(i).m_fHoriLocPoint << "," << m_vTableLocPoint.at(i).m_fVertLocPoint << ")";

			//float fNextHoriPoint = m_vTableLocPoint.at(i).m_fHoriLocPoint;
			//float fNextVertPoint = m_vTableLocPoint.at(i).m_fVertLocPoint;
			if (fNextHoriPoint > TableLimit.m_fTableRightThd || fNextHoriPoint <TableLimit.m_fTableLeftThd
				|| fNextVertPoint > TableLimit.m_fTableDownThd4Cruise || fNextVertPoint < TableLimit.m_fTableTopThd)
			{
				FILE_LOG(logINFO) << "��̨Ŀ�궨λ�㳬�����ƣ���̨�ص���ʼ��";
				fNextHoriPoint = 125.0;
				fNextVertPoint = 40.0;
				//continue;
			}

			float fCurrentHoriPoint = 0.0;   //��̨��ǰ��ˮƽ�Ƕ�
			float fCurrentVertPoint = 0.0;   //��̨��ǰ�ĸ����Ƕ�
			do
			{
				pTableControl2->GetHoriAngle(fCurrentHoriPoint);
				Sleep(50);
				pTableControl2->GetVertAngle(fCurrentVertPoint);
				Sleep(50);
				//pTableControl2->GetHoriAngle(fCurrentHoriPoint);
				//Sleep(50);
				//pTableControl2->GetVertAngle(fCurrentVertPoint);
				//Sleep(50);
				pTableControl2->HVLocate(fNextHoriPoint, fNextVertPoint);  //��̨��λ��ָ����
			} while (std::abs(fCurrentHoriPoint - fNextHoriPoint) > 1 ||
				std::abs(fCurrentVertPoint - fNextVertPoint) > 1);

			Sleep(300);
		}
		//FILE_LOG(logINFO) << "��̨Ѳ�����" << std::endl << std::endl;
	//}
	
	FILE_LOG(logDEBUG) << "TableTest() return";

	return true;
}
