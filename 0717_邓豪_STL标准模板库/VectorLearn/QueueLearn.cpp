#include "QueueLearn.h"   //FIFO�ṹ������֧�ֵ�����,�ܵ���ʽ

bool QueueLearn(void)
{
	std::cout << std::endl << "**************QueueLearn_display************* " << std::endl << std::endl;
	std::queue<int>queInt;
	queInt.push(1);        //push �Ӷ�β���һ��Ԫ��
	queInt.push(2);
	queInt.push(3);
	queInt.push(4);
	queInt.push(5);
	queInt.push(6);
	queInt.push(7);
	//queInt.pop();          //pop �Ӷ�ͷ�Ƴ���һ��Ԫ��,��ֱ��ɾ������

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
	std::cout << "queInt.size(): "<<queInt.size() << std::endl;
	if ( !queInt.empty())
	{
		std::cout << "!queInt.empty(): "<<queInt.size() << std::endl;
	}

	std::cout<<"queInt.front(): "<<queInt.front()<<std::endl;   //front ���ض�ͷԪ��
	std::cout<<"queInt.back()�� "<<queInt.back()<<std::endl;    //back  ���ض�βԪ��
	//queue ��һ��FIFO�ṹ������ͷ��β���޷���������Ԫ�أ����޷���������Ҫ�������Ƚ����ݴ��������ṹ
	return true;
}


