#include "DequeLearn.h"

bool DequeLearn(void)
{
	std::deque<int>deqInt;
	
	deqInt.push_front(2);        //push_front �Ӷ���ͷ����������  pop_front �Ӷ���ͷ����������
	deqInt.push_front(1);
	deqInt.push_back(4);         //push_back  �Ӷ���β����������  pop_back  �Ӷ���β����������
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

	std::cout<<deqInt.front()<<std::endl;   //front ���ض��е�һ��Ԫ��
	std::cout<<deqInt.back()<<std::endl;    //back  ���ض������һ��Ԫ��
	std::cout<<deqInt.size()<<std::endl;    //size  ���ض��д�С
	if ( !deqInt.empty())					//empty �ж϶����Ƿ�Ϊ��
	{
		std::cout<<false<<std::endl;
	}

	deqInt.resize(6,0);                     //resize ͬvector resize
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	std::deque<int>deqData;
	//std::vector<int>deqData;
	deqData.push_back(11);
	deqData.push_back(12);
	//deqInt.insert((deqInt.begin()+2),3);   //ͬvector insert
	//deqInt.insert((deqInt.begin()+2),4,3);
	//deqInt.insert((deqInt.begin()+2),deqInt.begin(),deqInt.end());  //�˾�ᱨ��
	deqInt.insert((deqInt.begin()+2),deqData.begin(),deqData.end());
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//deqInt.erase((deqInt.begin()),(deqInt.begin()+2));  //erase(beg,end),ɾ��[beg,end]֮�������
	deqInt.erase(deqInt.begin());                         //erase(pos)��ɾ��pos��������
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	return true;
}