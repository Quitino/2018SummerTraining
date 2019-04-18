#include <iostream>
#include "CircleDetect.h"

 

using IPSG::CCircleDetect;

int main(int _Argc, char *_Argv[])
{
	FILELog::ReportingLevel() = logINFO;

	std::string strOutputData;
	CCircleDetect CircleDetect;
	CircleDetect.ProcessFrame();




	system("pause");
	return 0;
}