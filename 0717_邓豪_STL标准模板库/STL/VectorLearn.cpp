#include "VectorLearn.h"

bool VectorLearn(void)
{
	std::vector<int>vecInt;
	vecInt.push_back(1); 
	vecInt.push_back(3);
	vecInt.push_back(5);
	vecInt.push_back(7);
	vecInt.push_back(9);

	try
	{
		std::cout<<vecInt.at(5)<<std::endl;
	}
	catch (std::out_of_range &exc)
	{
		std::cerr<<"File:"<<__FILE__<<" Line:"<<__LINE__<<"  "<<exc.what()<<std::endl;
	}                                          //异常处理

	vecInt.front() = 12;
	std::cout<<vecInt.front()<<std::endl;
	vecInt.back()  = 13;
	std::cout<<vecInt.back()<<std::endl;       //front及back操作

	std::vector<int>::iterator it;
	it = vecInt.begin();
	std::cout<<*it<<std::endl;
	it = (--vecInt.end());
	std::cout<<*it<<std::endl;                 //迭代器及begin、end操作
											   //注意：end的迭代器是vector末尾元素的后一指针，无法进行取值操作

	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;                      //vector遍历

	//std::vector<int>::reverse_iterator rit;    //逆序迭代器，rbegin是vector末尾元素的后一指针，无法进行取值操作
	//rit = (++vecInt.rend());                   //不建议使用逆序迭代器
	//std::cout<<*it<<std::endl;

	std::cout<<vecInt.size()<<std::endl;         //size 返回容器元素值
	if ( !vecInt.empty())						 //empty 判断容器是否为空
	{
		std::cout<<false<<std::endl;
	}
	vecInt.resize(8,1);							 //resize 重定义容器大小，第一个参数为长度，原来长度小于重定义长度，则扩展以第二个参数的值，否则截断
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//std::vector<int>vecData;
	//vecData.push_back(29);
	//vecData.push_back(30);

	vecInt.insert((vecInt.begin()+3),22);							 //insert(pos,elem)  在指定位置插入指定值
	//vecInt.insert((vecInt.begin()+3),4,22);						 //insert(pos,n,elem)  在指定位置插入那个制定值
	//vecInt.insert((vecInt.begin()+3),vecInt.begin(),vecInt.end()); //insert(pos,beg,end)  在指定位置插入beg到end的值
	//vecInt.insert((vecInt.begin()+3),vecData.begin(),vecData.end());
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//vecInt.erase((vecInt.begin()+2),(vecInt.begin()+4));  //erase(beg,end),删除[beg,end]之间的数据
	vecInt.erase(vecInt.begin());                         //erase(pos)，删除pos处的数据
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	return true;
}