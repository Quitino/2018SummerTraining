#ifndef LESSON_H_  
#define LESSON_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>



#include <opencv2/opencv.hpp>
#include <boost/thread/thread.hpp>

namespace IPSG
{
	typedef struct Student
	{
		std::string	m_strName;			//����
		std::string	m_strStudentNO;		//ѧ��

		float		m_fCHNScore;		//���ĳɼ�
		float		m_fMathScore;		//��ѧ�ɼ�
		float		m_fENGScore;		//Ӣ�ĳɼ�
	
		Student(std::string strName = "123",
			std::string strStudentNO = "456",
			float fCHNScore = 12.0,
			float fMathScore = 23.5,
			float fENGScore = 56.33)
			:m_strName(strName),
			m_strStudentNO(strStudentNO),
			m_fCHNScore(fCHNScore),
			m_fMathScore(fMathScore),
			m_fENGScore(fENGScore){};

		bool PrintStudentInfo();
	
	}SStudent;

	bool Lesson();
}

#endif // !LESSON_H_


