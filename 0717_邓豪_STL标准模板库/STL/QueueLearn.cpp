#include "QueueLearn.h"

bool QueueLearn(void)
{
	std::queue<int>queInt;
	queInt.push(1);        //push �Ӷ�β���һ��Ԫ��
	queInt.push(2);
	queInt.push(3);
	queInt.push(4);
	queInt.push(5);
	queInt.pop();          //pop �Ӷ�ͷ�Ƴ���һ��Ԫ��

	//std::queue<int>::iterator it;   //queue ��֧�ֵ�����
	//try                             //queue ��֧��at ��֧��[]
	//{
	//	for (int i = 0;i<queInt.size();i++)
	//	{
	//		std::cout<<queInt[i];
	//	}
	//}
	//catch(std::out_of_range &exc)
	//{
	//	;
	//}

	std::cout<<queInt.size()<<std::endl;
	if ( !queInt.empty())
	{
		std::cout<<queInt.size()<<std::endl;
	}

	std::cout<<queInt.front()<<std::endl;   //front ���ض�ͷԪ��
	std::cout<<queInt.back()<<std::endl;    //back  ���ض�βԪ��
	//queue ��һ��FIFO�ṹ������ͷ��β���޷���������Ԫ�أ����޷���������Ҫ�������Ƚ����ݴ��������ṹ
	return true;
}