#include "StackLearn.h"

bool StackLearn(void)
{
	std::stack<int>stkInt;

	stkInt.push(5);      //push ��ջͷѹ��һ��Ԫ��
	stkInt.push(4);
	stkInt.push(3);
	stkInt.push(2);
	stkInt.push(1);
	stkInt.pop();        //pop ��ջͷ����һ��Ԫ��

	if ( !stkInt.empty())
	{
		std::cout<<stkInt.size()<<std::endl;
	}

	std::cout<<stkInt.top()<<std::endl;
	//stack��FILO�ṹ����֧�ֱ���������top����ջ��ֵ

	return true;
}