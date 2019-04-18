#include "StackLearn.h"

bool StackLearn(void)
{
	std::stack<int>stkInt;

	stkInt.push(5);      //push 往栈头压入一个元素
	stkInt.push(4);
	stkInt.push(3);
	stkInt.push(2);
	stkInt.push(1);
	stkInt.pop();        //pop 从栈头弹出一个元素

	if ( !stkInt.empty())
	{
		std::cout<<stkInt.size()<<std::endl;
	}

	std::cout<<stkInt.top()<<std::endl;
	//stack是FILO结构，不支持遍历，仅含top返回栈顶值

	return true;
}