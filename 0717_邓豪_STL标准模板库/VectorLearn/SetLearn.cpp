#include "SetLearn.h"

bool SetLearn(void)
{
	std::cout << std::endl << "**************SetLearn_display************* " << std::endl << std::endl;
	//一堆沙子，set集合中不允许有相同元素！！！
	//std::set<int,std::greater<int>>setInt;    //greater降序排列
	//std::set<int,std::less<int>>setInt;       //less升序排列
	std::set<int>setInt;						//默认升序排列
	setInt.insert(5);
	setInt.insert(3);
	setInt.insert(4);
	setInt.insert(2);
	setInt.insert(1);

	if ( !setInt.empty())
	{
		std::cout<<"setInt.size(): "<<setInt.size()<<std::endl;
	}

	std::set<int>::iterator it;   //迭代器
	int count = 0;
	for(it = setInt.begin();it != setInt.end();it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;

	//setInt.clear();								//clear 删除所有元素
	//setInt.erase(setInt.begin()++);				//erase(pos) 删除pos处元素
	//setInt.erase(setInt.begin(),setInt.end()--);  //erase(beg,end) 删除[beg,end]之间的元素
	setInt.erase(3);								//erase(elem) 删除值为elem的元素
	std::cout << "setInt.erase(3)后： " << std::endl;
	count = 0;
	for(it = setInt.begin();it != setInt.end();it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;

	try
	{
		it = setInt.find(4);                
		//find(elem) 查找值为elem的元素，返回迭代器，如果没有就返回伪迭代器，伪迭代器无法访问
		std::cout<<"setInt.find(4): " <<*it<<std::endl;
	}
	catch(std::out_of_range &exc)
	{
		std::cerr<<"File:"<<__FILE__<<" Line"<<__LINE__<<"  "<<exc.what()<<std::endl;
	}

	std::cout<<"setInt.count(3): "<<setInt.count(3)<<std::endl;  //count(elem) 统计集合中elem的个数
	
	it = setInt.lower_bound(1);             //lower_bound(elem) 返回第一个小于等于elem元素的迭代器 
	std::cout<<"setInt.lower_bound(1): "<<*it<<std::endl;
	it = setInt.upper_bound(1);				//lower_bound(elem) 返回第一个大于elem元素的迭代器
	std::cout << "setInt.upper_bound(1): " << *it << std::endl;
	
	std::pair<std::set<int>::iterator,std::set<int>::iterator>pairIt;   //迭代器对
	pairIt = setInt.equal_range(2);         //equal_range(elem) 返回迭代器对，表示指定elem的lower_bound和upper_bound
	std::cout<<"迭代器对*pairIt.first: "<<*pairIt.first<<std::endl;
	std::cout << "迭代器对*pairIt.second: " << *pairIt.second << std::endl;	//带*的写法不常用

	return true;
}

bool MultisetLearn(void)  //支持相同元素
{
	std::multiset<int>mulsetInt;
	//与set类似，区别在于，set支持唯一键值，每个元素值只能出现一次；而multiset中同一值可以出现多次
	return true;
}