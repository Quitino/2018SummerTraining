#include "QueueLearn.h"

bool QueueLearn(void)
{
	std::queue<int>queInt;
	queInt.push(1);        //push 从队尾添加一个元素
	queInt.push(2);
	queInt.push(3);
	queInt.push(4);
	queInt.push(5);
	queInt.pop();          //pop 从队头移除第一个元素

	//std::queue<int>::iterator it;   //queue 不支持迭代器
	//try                             //queue 不支持at 不支持[]
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

	std::cout<<queInt.front()<<std::endl;   //front 返回队头元素
	std::cout<<queInt.back()<<std::endl;    //back  返回队尾元素
	//queue 是一种FIFO结构，除队头队尾外无法访问其他元素，故无法遍历，需要遍历可先将数据存入其他结构
	return true;
}