#include "Scohe.h"

int main(int _Argc, char *_Argv[])
{
	FILE * file;
	std::string strLogFileName = "../log.txt";	//�ļ�·��
	file = fopen(strLogFileName.c_str(), "a+");

	FILELog::ReportingLevel() = logINFO;		//������־�ȼ�
	Output2FILE::Stream() = file;				//������־·��
	
	IPSG::CScohe Schoe;
	Schoe.ModelInital();
	Schoe.Process();


	//IPSG::TableTest();

	system("pause");
	return EXIT_SUCCESS;
}