#include "lesson1.h"

//ÒıÓÃ

bool IPSG::Lesson()
{
	std::fstream file;
	file.open("file.txt",std::ios::out);

	file << "stream" << std::endl;


	SStudent Student("12345","67890",54.0,65.3,100.5);

	std::cout << Student.m_strName << std::endl;
	Student.PrintStudentInfo();


	return true;
}

bool IPSG::SStudent::PrintStudentInfo()
{
	std::cout << m_strName << "    " <<
		m_strStudentNO << "    " <<
		m_fCHNScore << "    " <<
		m_fMathScore << "    " <<
		m_fENGScore << "    " <<
		std::endl;

	return true;
}