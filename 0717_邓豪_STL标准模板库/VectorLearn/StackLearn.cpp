#include "StackLearn.h"  //FILO�ṹ����

bool StackLearn(void)
{
	std::cout << std::endl << "**************StackLearn_display************* " << std::endl << std::endl;
	std::stack<int>stkInt;
						//���ܲ��롢����ɾ�������ܸ�ֵ

	stkInt.push(5);      //push ��ջͷѹ��һ��Ԫ��
	stkInt.push(4);
	stkInt.push(3);
	stkInt.push(2);
	stkInt.push(1);
	stkInt.pop();        //pop ��ջͷ����һ��Ԫ��

	if ( !stkInt.empty())
	{
		std::cout<<"stkInt.size(): "<<stkInt.size()<<std::endl;
	}

		std::cout << "stkInt.top(): "<<stkInt.top()<<std::endl;
	//stack��FILO�ṹ����֧�ֱ���������top����ջ��ֵ
	return true;
}