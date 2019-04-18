#include "TableControl.h"

bool IPSG::CTableControl::EnCode(BYTE ucCommand2, BYTE ucDataH, BYTE ucDataL, std::vector<BYTE> &vucOutData)
{
	vucOutData[0] = 0XFF;
	vucOutData[1] = 0X01;
	vucOutData[2] = 0X00;
	vucOutData[3] = ucCommand2;
	vucOutData[4] = ucDataH;
	vucOutData[5] = ucDataL;

	CalCheckSum(vucOutData, vucOutData[6]);

	return true;
}

bool IPSG::CTableControl::DeCode()
{
	std::vector<BYTE> vucRecvData(m_vucReceivedFrame);

	if (vucRecvData.at(0) != 0XFF)
		std::cout << "Frame Head error" << std::endl;

	if (vucRecvData.at(1) != 0X01)
		std::cout << "Frame Address error" << std::endl;

	if (vucRecvData.at(2) != 0X00)
		std::cout << " error" << std::endl;

	BYTE ucCheckSum;
	CalCheckSum(vucRecvData, ucCheckSum);
	if (vucRecvData.at(6) != ucCheckSum)
		std::cout << " error" << std::endl;

	if (vucRecvData.size() != 7)
		std::cout << " error" << std::endl;

	return true;
}