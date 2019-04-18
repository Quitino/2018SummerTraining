#include "StackLearn.h"  //FILO结构！！

bool StackLearn(void)
{
	std::cout << std::endl << "**************StackLearn_display************* " << std::endl << std::endl;
	std::stack<int>stkInt;
						//不能插入、不能删除、不能改值

	stkInt.push(5);      //push 往栈头压入一个元素
	stkInt.push(4);
	stkInt.push(3);
	stkInt.push(2);
	stkInt.push(1);
	stkInt.pop();        //pop 从栈头弹出一个元素

	if ( !stkInt.empty())
	{
		std::cout<<"stkInt.size(): "<<stkInt.size()<<std::endl;
	}

		std::cout << "stkInt.top(): "<<stkInt.top()<<std::endl;
	//stack是FILO结构，不支持遍历，仅含top返回栈顶值
	return true;
}