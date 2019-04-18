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
		std::string	m_strName;			//姓名
		std::string	m_strStudentNO;		//学号

		float		m_fCHNScore;		//中文成绩
		float		m_fMathScore;		//数学成绩
		float		m_fENGScore;		//英文成绩
	
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

		///*bool PrintStudentInfo();*/
	
	}SStudent;

	typedef struct TableLimit
	{
		float	m_fTopLocLimit;
		float	m_fBtmLocLimit4Cruise;
		float	m_fBtmLocLimit4Track;

		float	m_fLeftLocLimit;
		float	m_fRightLocLimit;

		TableLimit(float fTopLocLimit = 12.0,
			float fBtmLocLimit4Cruise = 46.0,
			float fBtmLocLimit4Track = 48.0,
			float fLeftLocLimit = 125,
			float fRightLocLimit = 215) :
			m_fTopLocLimit(fTopLocLimit),
			m_fBtmLocLimit4Cruise(fBtmLocLimit4Cruise),
			m_fBtmLocLimit4Track(fBtmLocLimit4Track),
			m_fLeftLocLimit(fLeftLocLimit),
			m_fRightLocLimit(fRightLocLimit){};

		bool PrintDefaultInfo();
	}STableLimit;

	class CStudent
	{
	public:
		CStudent() :m_bUpdate(true), m_fNature(85.65),
			m_strAccount("admin"), m_strPassword("admin"){};
		~CStudent();

		bool Login();

	private:
		bool Index();
		

	public:
		bool		m_bUpdate;
	private:
		SStudent	m_Student;
		float		m_fNature;

		std::string m_strAccount;
		std::string m_strPassword;
	};


	class CRectangle
	{
	public:
		CRectangle() :m_fLength(0.0), m_fWidth(0.0){};
		CRectangle(float fLength = 0.0, float fWidth = 0.0) :
			m_fLength(fLength), m_fWidth(fWidth){};

		~CRectangle(){};

		bool CalArea(float &fArea);
		bool CalRid(float &fRid);

	private:
		bool LenAboveWdh();

	public:
		bool	m_bBusBusy;
	private:
		float	m_fLength;
		float	m_fWidth;
	};




	bool Lesson();
}

#endif // !LESSON_H_


