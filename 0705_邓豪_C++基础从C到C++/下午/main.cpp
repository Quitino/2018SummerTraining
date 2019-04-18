#include <iostream>

#include "lesson1.h"

using namespace std;

int main(int _Argc, char *_Argv[])
{
	//if (!IPSG::Lesson())
	//{
	//	std::cout << "Error" << std::endl;
	//	system("pause");
	//	return EXIT_FAILURE;
	//}

	IPSG::CRectangle Rectangle(3.5, 4);
	float fArea;
	Rectangle.CalArea(fArea);
	std::cout << fArea << std::endl;

	system("pause");
	return EXIT_SUCCESS;
}

//main.exe -v -print
