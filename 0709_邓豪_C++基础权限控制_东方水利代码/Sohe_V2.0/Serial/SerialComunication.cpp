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
	v_ucData[2] = 0x50;							//地址
	v_ucData[3] = nAngelTemp >> 8;				//高位
	v_ucData[4] = nAngelTemp;
	v_ucData[5] = nDistanceTemp >> 8;			//高位
	v_ucData[6] = nDistanceTemp;

	unsigned short CRC = 0x0000;
	CountCRC(v_ucData, 7, CRC);
	v_ucData[7] = (CRC & 0xFF00) >> 8;			//高位
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
	v_ucData[2] = 0x51;						//地址
	v_ucData[3] = SwitchTemp;				//高位
	v_ucData[4] = 0x00;
	v_ucData[5] = 0x00;						//高位
	v_ucData[6] = 0x00;

	unsigned short CRC = 0x0000;
	CountCRC(v_ucData, 7, CRC);
	v_ucData[7] = (CRC & 0xFF00) >> 8;		//高位
	v_ucData[8] = (CRC & 0x00FF);
	v_ucData[9] = 0xDD;
	return true;
}

bool IPSG::CSerialCommunication::CommunicationDeCode()
{
	m_bSendFlag = false;
	std::vector<unsigned char > ReceivedFrame(m_vucReceiveBuff);

	////帧头错码删除
	while ((ReceivedFrame.begin() != ReceivedFrame.end()) && (ReceivedFrame.at(0) != 0xAA))
	{
		ReceivedFrame.erase(ReceivedFrame.begin());
	}

	//判空
	std::vector<unsigned char>::iterator vcitor = ReceivedFrame.begin();
	if (vcitor == ReceivedFrame.end())
	{
		FILE_LOG(logINFO) << "Data is empty";
		ReceivedFrame.clear();
		return false;
	}

	//帧长
	if (7 != ReceivedFrame.size())
	{
		FILE_LOG(logINFO) << "The length of data error";
		ReceivedFrame.clear();
		return false;
	}

	//帧头帧尾校验
	std::vector<unsigned char>::iterator vcItorEnd = ReceivedFrame.end() - 1;
	if ((*vcitor != 0XAA) || (*(vcitor + 1) != 0X55) || (*vcItorEnd != 0XDD))
	{
		FILE_LOG(logINFO) << "Data error";
		ReceivedFrame.clear();
		return false;
	}

	//地址判断
	std::vector<unsigned char>::iterator itAddr = vcitor + 2;
	if (*itAddr != 0X50)
	{
	//	FILE_LOG(logINFO) << "Address error!";
		ReceivedFrame.clear();
		return false;
	}

	////校验和校验
	//unsigned short ReceivedDataCRC;
	//CountCRC(ReceivedFrame, 4, ReceivedDataCRC);
	//unsigned char CRCHigh, CRCLow;
	//CRCHigh = (ReceivedDataCRC & 0xFF00) >> 8;	//高位
	//CRCLow = (ReceivedDataCRC & 0x00FF);
	//if (CRCHigh != ReceivedFrame.at(4) || CRCLow != ReceivedFrame.at(5))
	//{
	//	FILE_LOG(logINFO) << "CRC error!";
	//	ReceivedFrame.clear();
	//	return false;
	//}

	//功能码判断
	unsigned char FunctionCode = ReceivedFrame.at(3);
	if (0x00 != FunctionCode)
	{
		FILE_LOG(logINFO) << "Function code error!";
		ReceivedFrame.clear();
		return false;
	}

	m_bSendFlag = true;    //表示接收到发送数据请求
	ReceivedFrame.clear();

	return true;
}

bool IPSG::SerialCommunicationTest()
{
	CSerialBase* pCommunication = new CSerialCommunication;

	if (!pCommunication->InitPort(10,9600))	//串口号和波特率
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