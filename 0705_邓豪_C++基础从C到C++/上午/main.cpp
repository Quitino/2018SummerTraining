#include <iostream>

#include "lesson1.h"

using namespace std;

int main(int _Argc, char *_Argv[])
{
	if (!IPSG::Lesson())
	{
		std::cout << "Error" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	system("pause");
	return EXIT_SUCCESS;
}

//main.exe -v -print
