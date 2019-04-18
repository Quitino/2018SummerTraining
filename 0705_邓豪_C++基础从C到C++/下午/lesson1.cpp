#include "lesson1.h"

bool IPSG::Lesson()
{
	STableLimit TableLimit;

	std::cout << TableLimit.m_fTopLocLimit << std::endl;

	bool bCruise = true;
	float fHoriLocPoint, fVertLocPoint;
	std::cin >> fHoriLocPoint >> fVertLocPoint;

	std::fstream file;
	file.open("file.txt",std::ios::out);
	if (bCruise)
	{
		if (fHoriLocPoint < TableLimit.m_fLeftLocLimit ||
			fHoriLocPoint > TableLimit.m_fRightLocLimit ||
			fVertLocPoint > TableLimit.m_fBtmLocLimit4Cruise ||
			fVertLocPoint < TableLimit.m_fTopLocLimit)
		{
			file << "Error: 超出限制" << std::endl;
			return false;
		}
	}
	else
	{
		if (fHoriLocPoint < TableLimit.m_fLeftLocLimit ||
			fHoriLocPoint > TableLimit.m_fRightLocLimit ||
			fVertLocPoint > TableLimit.m_fBtmLocLimit4Track ||
			fVertLocPoint < TableLimit.m_fTopLocLimit)
		{
			file << "Error: 超出限制" << std::endl;
			return false;
		}
	}

	std::cout << "Running" << std::endl;

	//std::fstream file;
	//file.open("file.txt",std::ios::out);

	//file << "stream" << std::endl;


	//SStudent Student("12345","67890",54.0,65.3,100.5);

	//std::cout << Student.m_strName << std::endl;
	//Student.PrintStudentInfo();


	return true;
}

bool IPSG::STableLimit::PrintDefaultInfo()
{
	std::cout << "Top: " << m_fTopLocLimit << std::endl;


	return true;
}

//bool IPSG::SStudent::PrintStudentInfo()
//{
//	std::cout << m_strName << "    " <<
//		m_strStudentNO << "    " <<
//		m_fCHNScore << "    " <<
//		m_fMathScore << "    " <<
//		m_fENGScore << "    " <<
//		std::endl;
//
//	return true;
//}


bool IPSG::CRectangle::CalArea(float &fArea)
{
	fArea = m_fLength * m_fWidth;

	return true;
}

bool IPSG::CRectangle::CalRid(float &fRid)
{
	bool bFlag = LenAboveWdh();

	fRid = 2 * (m_fLength + m_fWidth);

	return true;
}


bool IPSG::CRectangle::LenAboveWdh()
{
	return (m_fLength > m_fWidth);
}