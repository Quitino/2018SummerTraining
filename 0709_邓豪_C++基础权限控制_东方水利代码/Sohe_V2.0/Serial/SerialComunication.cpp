#include "SerialComunication.h"

bool IPSG::CSerialCommunication::CountCRC(std::vector<unsigned char>v_ucData, int nLength, unsigned short &usCRC)
{
	usCRC = 0xFFFF;
	int i;

	for (size_t j = 0; j < nLength; j++)
	{
		usCRC ^= v_ucData[j];
		for (i = 0; i < 8; i++)
		{
			if (usCRC & 1)
			{
				usCRC >>= 1;
				usCRC ^= 0xA001;
			}
			else usCRC >>= 1;
		}
	}
	return true;
}

bool IPSG::CSerialCommunication::ECode(int nAngel, int nDistance, std::vector<unsigned char> &v_ucData)
{
	v_ucData.resize(10);

	int nAngelTemp = nAngel;
	int nDistanceTemp = nDistance;

	v_ucData[0] = 0xAA;
	v_ucData[1] = 0x55;
	v_ucData[2] = 0x50;							//��ַ
	v_ucData[3] = nAngelTemp >> 8;				//��λ
	v_ucData[4] = nAngelTemp;
	v_ucData[5] = nDistanceTemp >> 8;			//��λ
	v_ucData[6] = nDistanceTemp;

	unsigned short CRC = 0x0000;
	CountCRC(v_ucData, 7, CRC);
	v_ucData[7] = (CRC & 0xFF00) >> 8;			//��λ
	v_ucData[8] = (CRC & 0x00FF);
	v_ucData[9] = 0xDD;
	return true;
}

bool IPSG::CSerialCommunication::ECode4Brush(const int nSwitch, std::vector<unsigned char> &v_ucData)
{
	v_ucData.resize(10);
	int SwitchTemp = nSwitch;

	v_ucData[0] = 0xAA;
	v_ucData[1] = 0x55;
	v_ucData[2] = 0x51;						//��ַ
	v_ucData[3] = SwitchTemp;				//��λ
	v_ucData[4] = 0x00;
	v_ucData[5] = 0x00;						//��λ
	v_ucData[6] = 0x00;

	unsigned short CRC = 0x0000;
	CountCRC(v_ucData, 7, CRC);
	v_ucData[7] = (CRC & 0xFF00) >> 8;		//��λ
	v_ucData[8] = (CRC & 0x00FF);
	v_ucData[9] = 0xDD;
	return true;
}

bool IPSG::CSerialCommunication::CommunicationDeCode()
{
	m_bSendFlag = false;
	std::vector<unsigned char > ReceivedFrame(m_vucReceiveBuff);

	////֡ͷ����ɾ��
	while ((ReceivedFrame.begin() != ReceivedFrame.end()) && (ReceivedFrame.at(0) != 0xAA))
	{
		ReceivedFrame.erase(ReceivedFrame.begin());
	}

	//�п�
	std::vector<unsigned char>::iterator vcitor = ReceivedFrame.begin();
	if (vcitor == ReceivedFrame.end())
	{
		FILE_LOG(logINFO) << "Data is empty";
		ReceivedFrame.clear();
		return false;
	}

	//֡��
	if (7 != ReceivedFrame.size())
	{
		FILE_LOG(logINFO) << "The length of data error";
		ReceivedFrame.clear();
		return false;
	}

	//֡ͷ֡βУ��
	std::vector<unsigned char>::iterator vcItorEnd = ReceivedFrame.end() - 1;
	if ((*vcitor != 0XAA) || (*(vcitor + 1) != 0X55) || (*vcItorEnd != 0XDD))
	{
		FILE_LOG(logINFO) << "Data error";
		ReceivedFrame.clear();
		return false;
	}

	//��ַ�ж�
	std::vector<unsigned char>::iterator itAddr = vcitor + 2;
	if (*itAddr != 0X50)
	{
	//	FILE_LOG(logINFO) << "Address error!";
		ReceivedFrame.clear();
		return false;
	}

	////У���У��
	//unsigned short ReceivedDataCRC;
	//CountCRC(ReceivedFrame, 4, ReceivedDataCRC);
	//unsigned char CRCHigh, CRCLow;
	//CRCHigh = (ReceivedDataCRC & 0xFF00) >> 8;	//��λ
	//CRCLow = (ReceivedDataCRC & 0x00FF);
	//if (CRCHigh != ReceivedFrame.at(4) || CRCLow != ReceivedFrame.at(5))
	//{
	//	FILE_LOG(logINFO) << "CRC error!";
	//	ReceivedFrame.clear();
	//	return false;
	//}

	//�������ж�
	unsigned char FunctionCode = ReceivedFrame.at(3);
	if (0x00 != FunctionCode)
	{
		FILE_LOG(logINFO) << "Function code error!";
		ReceivedFrame.clear();
		return false;
	}

	m_bSendFlag = true;    //��ʾ���յ�������������
	ReceivedFrame.clear();

	return true;
}

bool IPSG::SerialCommunicationTest()
{
	CSerialBase* pCommunication = new CSerialCommunication;

	if (!pCommunication->InitPort(10,9600))	//���ںźͲ�����
		FILE_LOG(logINFO) << "InitPort fail !";
	else FILE_LOG(logINFO) << "InitPort success !";

	if (!pCommunication->OpenListenThread())
		FILE_LOG(logINFO) << "OpenListenThread fail !";
	else FILE_LOG(logINFO) << "OpenListenThread success !";

	std::string strTemp = "helloworld";
	while (1)
	{
		std::cout << std::endl;
		std::cout << strTemp;
		std::cout << std::endl;
		Sleep(1000);
	}

	return true;
}