#include "Scohe.h"

int main(int _Argc, char *_Argv[])
{
	FILE * file;
	std::string strLogFileName = "../log.txt";	//文件路径
	file = fopen(strLogFileName.c_str(), "a+");

	FILELog::ReportingLevel() = logINFO;		//设置日志等级
	Output2FILE::Stream() = file;				//设置日志路径
	
	IPSG::CScohe Schoe;
	Schoe.ModelInital();
	Schoe.Process();


	//IPSG::TableTest();

	system("pause");
	return EXIT_SUCCESS;
}