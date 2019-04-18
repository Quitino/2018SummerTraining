#include "ListLearn.h"

bool ListLearn(void)
{
	std::cout << std::endl << "**************ListLearn_display************* " << std::endl << std::endl;
	std::list<int>lstInt;
	lstInt.push_front(2);    //push_front �ӱ�ͷ���Ԫ�أ�pop_front����
	lstInt.push_front(1);
	lstInt.push_back(4);     //push_back  �ӱ�β���Ԫ�أ�pop_back����
	lstInt.push_back(5);

	//std::cout<<lstInt.front()<<std::endl;
	//std::cout<<lstInt.back()<<std::endl;
	
	if ( !lstInt.empty())    //size empty resize ͬvector
	{
		std::cout << "lstInt.size() : " << lstInt.size() << std::endl << std::endl;
	}

	std::list<int>::iterator it;    //����������
	int count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;



	std::cout << "lstInt.front()�� "<<lstInt.front() << std::endl;
	std::cout << "lstInt.back(): " << lstInt.back() << std::endl << std::endl;

	std::vector<int>vecData;
	vecData.assign(5,2);                           //vector  assign �鸳ֵ����ֵ5��1����deque��list����
	std::cout << "vecData.assign(5,2)��: " << std::endl;
	std::vector<int>::iterator it_v;               //����������
	count = 0;
	for (it_v = vecData.begin(); it_v != vecData.end(); it_v++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it_v << std::endl;
	}
	std::cout << std::endl;
//########################################################################################

	lstInt.assign(6,1);                            //list assign(n,elem),��n��elem��ֵ��list,�鸴�ƣ�5��1
 	std::cout << "lstInt.assign(6,1)��: " << std::endl;
	//std::list<int>::iterator it_l;               //����������
	count = 0;
	for (it = lstInt.begin(); it != lstInt.end(); it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout << std::endl;
//####################################################################################

	lstInt.assign(vecData.begin(),vecData.end());  //list assign(beg,end),��[beg,end]֮�����ݸ�ֵ��list,��Ҫ�ضϳ��Ĳ��֣��鸳ֵ����
	std::cout << "lstInt.assign(vecData.begin(),vecData.end())�� " << std::endl; 
	count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	std::list<int>lstData;
	//###############################################################################
	lstData.assign(4,1);
	std::cout << "lstData.assign(4,1)�� " << std::endl;
	count = 0;
	for (it = lstData.begin(); it != lstData.end(); it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout << std::endl;
	//##############################################################################

	lstInt.swap(lstData);    //list.swap(lst) ��lst��list�����Ԫ�ػ���
	std::cout << "lstInt.swap(lstData)�� " << std::endl << std::endl;
	std::cout << "lstIntԪ�أ�" << std::endl;
	count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	std::cout << "lstDataԪ�أ�" << std::endl;
	count = 0;
	for (it = lstData.begin(); it != lstData.end(); it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout << std::endl;


	//lstInt.swap(lstData);    //�ָ�����

	lstInt.insert((lstInt.begin()++),3);    //���롢ɾ��ͬvector
	std::cout << "lstInt.insert((lstInt.begin()++),3)�� " << std::endl;
	count = 0;
	for (it = lstInt.begin();it != lstInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;


	return true;
}