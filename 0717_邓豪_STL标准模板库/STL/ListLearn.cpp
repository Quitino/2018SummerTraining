#include "ListLearn.h"

bool ListLearn(void)
{
	std::list<int>lstInt;
	lstInt.push_front(2);    //push_front �ӱ�ͷ���Ԫ�أ�pop_front����
	lstInt.push_front(1);
	lstInt.push_back(4);     //push_back  �ӱ�β���Ԫ�أ�pop_back����
	lstInt.push_back(5);

	//std::cout<<lstInt.front()<<std::endl;
	//std::cout<<lstInt.back()<<std::endl;

	//if ( !lstInt.empty())    //size empty resize ͬvector
	//{
	//	std::cout<<lstInt.size()<<std::endl;
	//}

	std::cout<<lstInt.front()<<std::endl;
	std::cout<<lstInt.back()<<std::endl;

	std::list<int>::iterator it;    //����������
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//std::vector<int>vecData;
	//vecData.assign(5,1);                           //vector  assign ��ֵ����deque��list����

	//lstInt.assign(4,1);                            //list assign(n,elem),��n��elem��ֵ��list  
	//lstInt.assign(vecData.begin(),vecData.end());  //list assign(beg,end),��[beg,end]֮�����ݸ�ֵ��list
	//for (it = lstInt.begin();it != lstInt.end();it++)
	//{
	//	std::cout<<*it<<" ";
	//}
	//std::cout<<std::endl;

	//std::list<int>lstData;
	//lstData.assign(4,1);

	//lstInt.swap(lstData);    //list.swap(lst) ��lst��list�����Ԫ�ػ���
	//for (it = lstInt.begin();it != lstInt.end();it++)
	//{
	//	std::cout<<*it<<" ";
	//}
	//std::cout<<std::endl;
	//lstInt.swap(lstData);    //�ָ�

	//lstInt.insert((lstInt.begin()++),3);    //���롢ɾ��ͬvector
	//for (it = lstInt.begin();it != lstInt.end();it++)
	//{
	//	std::cout<<*it<<" ";
	//}
	//std::cout<<std::endl;


	return true;
}