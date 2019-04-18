#include<iostream>			
#include"day03.h"



int main()
{

	IPSG::CRectangle Rectangle(4.0,3.0);
	float fArea;
	Rectangle.CalArea(fArea);
	std::cout << fArea << std::endl;

	IPSG::printLength(Rectangle);
	system("pause");
	return 0;




}