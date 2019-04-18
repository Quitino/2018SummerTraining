#include "ListLearn.h"

bool ListLearn(void)
{
	std::list<int>lstInt;
	lstInt.push_front(2);    //push_front 从表头添加元素，pop_front类似
	lstInt.push_front(1);
	lstInt.push_back(4);     //push_back  从表尾添加元素，pop_back类似
	lstInt.push_back(5);

	//std::cout<<lstInt.front()<<std::endl;
	//std::cout<<lstInt.back()<<std::endl;

	//if ( !lstInt.empty())    //size empty resize 同vector
	//{
	//	std::cout<<lstInt.size()<<std::endl;
	//}

	std::cout<<lstInt.front()<<std::endl;
	std::cout<<lstInt.back()<<std::endl;

	std::list<int>::iterator it;    //迭代器遍历
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//std::vector<int>vecData;
	//vecData.assign(5,1);                           //vector  assign 赋值，与deque、list类似

	//lstInt.assign(4,1);                            //list assign(n,elem),将n个elem赋值给list  
	//lstInt.assign(vecData.begin(),vecData.end());  //list assign(beg,end),将[beg,end]之间数据赋值给list
	//for (it = lstInt.begin();it != lstInt.end();it++)
	//{
	//	std::cout<<*it<<" ";
	//}
	//std::cout<<std::endl;

	//std::list<int>lstData;
	//lstData.assign(4,1);

	//lstInt.swap(lstData);    //list.swap(lst) 将lst与list本身的元素互换
	//for (it = lstInt.begin();it != lstInt.end();it++)
	//{
	//	std::cout<<*it<<" ";
	//}
	//std::cout<<std::endl;
	//lstInt.swap(lstData);    //恢复

	//lstInt.insert((lstInt.begin()++),3);    //插入、删除同vector
	//for (it = lstInt.begin();it != lstInt.end();it++)
	//{
	//	std::cout<<*it<<" ";
	//}
	//std::cout<<std::endl;


	return true;
}