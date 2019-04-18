#include "ListLearn.h"

bool ListLearn(void)
{
	std::cout << std::endl << "**************ListLearn_display************* " << std::endl << std::endl;
	std::list<int>lstInt;
	lstInt.push_front(2);    //push_front 从表头添加元素，pop_front类似
	lstInt.push_front(1);
	lstInt.push_back(4);     //push_back  从表尾添加元素，pop_back类似
	lstInt.push_back(5);

	//std::cout<<lstInt.front()<<std::endl;
	//std::cout<<lstInt.back()<<std::endl;
	
	if ( !lstInt.empty())    //size empty resize 同vector
	{
		std::cout << "lstInt.size() : " << lstInt.size() << std::endl << std::endl;
	}

	std::list<int>::iterator it;    //迭代器遍历
	int count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;



	std::cout << "lstInt.front()： "<<lstInt.front() << std::endl;
	std::cout << "lstInt.back(): " << lstInt.back() << std::endl << std::endl;

	std::vector<int>vecData;
	vecData.assign(5,2);                           //vector  assign 块赋值，赋值5个1，与deque、list类似
	std::cout << "vecData.assign(5,2)后: " << std::endl;
	std::vector<int>::iterator it_v;               //迭代器遍历
	count = 0;
	for (it_v = vecData.begin(); it_v != vecData.end(); it_v++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it_v << std::endl;
	}
	std::cout << std::endl;
//########################################################################################

	lstInt.assign(6,1);                            //list assign(n,elem),将n个elem赋值给list,块复制：5个1
 	std::cout << "lstInt.assign(6,1)后: " << std::endl;
	//std::list<int>::iterator it_l;               //迭代器遍历
	count = 0;
	for (it = lstInt.begin(); it != lstInt.end(); it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout << std::endl;
//####################################################################################

	lstInt.assign(vecData.begin(),vecData.end());  //list assign(beg,end),将[beg,end]之间数据赋值给list,且要截断长的部分，块赋值！！
	std::cout << "lstInt.assign(vecData.begin(),vecData.end())后： " << std::endl; 
	count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;

	std::list<int>lstData;
	//###############################################################################
	lstData.assign(4,1);
	std::cout << "lstData.assign(4,1)后： " << std::endl;
	count = 0;
	for (it = lstData.begin(); it != lstData.end(); it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout << std::endl;
	//##############################################################################

	lstInt.swap(lstData);    //list.swap(lst) 将lst与list本身的元素互换
	std::cout << "lstInt.swap(lstData)后： " << std::endl << std::endl;
	std::cout << "lstInt元素：" << std::endl;
	count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;

	std::cout << "lstData元素：" << std::endl;
	count = 0;
	for (it = lstData.begin(); it != lstData.end(); it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout << std::endl;


	//lstInt.swap(lstData);    //恢复交换

	lstInt.insert((lstInt.begin()++),3);    //插入、删除同vector
	std::cout << "lstInt.insert((lstInt.begin()++),3)后： " << std::endl;
	count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;


	return true;
}