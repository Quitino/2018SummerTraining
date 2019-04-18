#include "DequeLearn.h"

bool DequeLearn(void)
{
	std::deque<int>deqInt;
	
	deqInt.push_front(2);        //push_front 从队列头部插入数据  pop_front 从队列头部弹出数据
	deqInt.push_front(1);
	deqInt.push_back(4);         //push_back  从队列尾部插入数据  pop_back  从队列尾部弹出数据
	deqInt.push_back(5);

	std::deque<int>::iterator it;
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	try
	{
		std::cout<<deqInt.at(5)<<std::endl;
	}
	catch(std::out_of_range &exc)
	{
		std::cerr<<"File:"<<__FILE__<<" Line:"<<__LINE__<<" "<<exc.what()<<std::endl;
	}

	std::cout<<deqInt.front()<<std::endl;   //front 返回队列第一个元素
	std::cout<<deqInt.back()<<std::endl;    //back  返回队列最后一个元素
	std::cout<<deqInt.size()<<std::endl;    //size  返回队列大小
	if ( !deqInt.empty())					//empty 判断队列是否为空
	{
		std::cout<<false<<std::endl;
	}

	deqInt.resize(6,0);                     //resize 同vector resize
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	std::deque<int>deqData;
	//std::vector<int>deqData;
	deqData.push_back(11);
	deqData.push_back(12);
	//deqInt.insert((deqInt.begin()+2),3);   //同vector insert
	//deqInt.insert((deqInt.begin()+2),4,3);
	//deqInt.insert((deqInt.begin()+2),deqInt.begin(),deqInt.end());  //此句会报错
	deqInt.insert((deqInt.begin()+2),deqData.begin(),deqData.end());
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//deqInt.erase((deqInt.begin()),(deqInt.begin()+2));  //erase(beg,end),删除[beg,end]之间的数据
	deqInt.erase(deqInt.begin());                         //erase(pos)，删除pos处的数据
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	return true;
}