#include "VectorLearn.h"

bool VectorLearn(void)
{
	std::cout << std::endl << "**************VectorLearn_display************* " << std::endl << std::endl;
	std::vector<std::vector<int>>vvint;//vector嵌套，相当于二维数值
	std::vector<int>vecInt;
	vecInt.clear();				//用之前清理一下
	vecInt.push_back(1);
	vecInt.push_back(3);
	vecInt.push_back(5);
	vecInt.push_back(7);
	vecInt.push_back(9);
	
	try   //尝试执行该句，内存越界的问题，很重要！
	{
		std::cout<<vecInt.at(5)<<std::endl;
	}
	catch (std::out_of_range &exc)             //尝试捕获超限的异常
	{
		std::cerr<<"File:"<<__FILE__<<" Line:"<<__LINE__<<"  "<<exc.what()<<std::endl;
	}                                          //异常处理

	vecInt.front() = 25;						//front 替换vector头元素
	std::cout<<"替换第一个元素:"<<vecInt.front()<<std::endl;
	vecInt.back() = 13;							//front 替换vector尾元素
	std::cout<<"替换最后一个元素:"<<vecInt.back()<<std::endl;       //front及back操作


	std::vector<int>::iterator it;				//迭代器=>指针
	it = vecInt.begin();
	std::cout<<"vecInt.begin:"<<*it<<std::endl;
	it = (--vecInt.end());
	std::cout<<"vecInt.end:"<<*it<<std::endl;                 //迭代器及begin、end操作
											   //注意：end的迭代器是vector末尾元素的后一指针，无法进行取值操作
	int count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)  //end是最后一个元素的下一个地址，非法的地址
	{
		count++;
		std::cout << "第 " << count << " 个元素:" <<  *it << std::endl;
		
	}
	std::cout<<std::endl;						 //vector遍历

	//std::vector<int>::reverse_iterator rit;    //逆序迭代器，rbegin是vector末尾元素的后一指针，无法进行取值操作
	//rit = (++vecInt.rend());                   //不建议使用逆序迭代器
	//std::cout<<*it<<std::endl;

	std::cout<<"vecInt.size:"<<vecInt.size()<<std::endl;     //size 返回容器元素值
	if ( !vecInt.empty())						 //empty 判断容器是否为空
	{
		std::cout<<"vecInt.empty = "<<false<<std::endl;
	}

	vecInt.resize(8,1);	 //resize 重定义容器大小，第一个参数为长度，原来长度小于重定义长度，则扩展以第二个参数的值，否则截断
	
	std::cout << "resize(8,1)过后：" << std::endl; 
	count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		count++;		
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;

	//std::vector<int>vecData;
	//vecData.push_back(29);
	//vecData.push_back(30);

	vecInt.insert((vecInt.begin()+3),22);							 //insert(pos,elem)  在指定位置插入指定值
	//vecInt.insert((vecInt.begin()+3),4,22);						 //insert(pos,n,elem)  在指定位置插入n个指定值
	//vecInt.insert((vecInt.begin()+3),vecInt.begin(),vecInt.end()); //insert(pos,beg,end)  在指定位置插入beg到end的值
	//vecInt.insert((vecInt.begin()+3),vecData.begin(),vecData.end());
	std::cout << "vecInt.insert((vecInt.begin()+3),22)插值后：" << std::endl;
	count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		count++;
		std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;

	//vecInt.erase((vecInt.begin()+2),(vecInt.begin()+4));  //erase(beg,end),删除[beg,end]之间的数据
	vecInt.erase(vecInt.begin());							//erase(pos)，删除pos处的数据
	std::cout << "vecInt.erase(vecInt.begin())删值后：" << std::endl;
	count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
			count++;
			std::cout << "第 " << count << " 个元素:" << *it << std::endl;
	}
	std::cout<<std::endl;

	return true;
}