#include "SetLearn.h"

bool SetLearn(void)
{
	//std::set<int,std::greater<int>>setInt;    //��������
	//std::set<int,std::less<int>>setInt;       //��������
	std::set<int>setInt;						//Ĭ����������
	setInt.insert(5);
	setInt.insert(3);
	setInt.insert(4);
	setInt.insert(2);
	setInt.insert(1);

	if ( !setInt.empty())
	{
		std::cout<<setInt.size()<<std::endl;
	}

	std::set<int>::iterator it;
	for(it = setInt.begin();it != setInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//setInt.clear();								//clear ɾ������Ԫ��
	//setInt.erase(setInt.begin()++);				//erase(pos) ɾ��pos��Ԫ��
	//setInt.erase(setInt.begin(),setInt.end()--);  //erase(beg,end) ɾ��[beg,end]֮���Ԫ��
	setInt.erase(3);								//erase(elem) ɾ��ֵΪelem��Ԫ��
	for(it = setInt.begin();it != setInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	try
	{
		it = setInt.find(4);                //find(elem) ����ֵΪelem��Ԫ��
		std::cout<<*it<<std::endl;
	}
	catch(std::out_of_range &exc)
	{
		std::cerr<<"File:"<<__FILE__<<" Line"<<__LINE__<<"  "<<exc.what()<<std::endl;
	}

	std::cout<<setInt.count(3)<<std::endl;  //count(elem) ͳ�Ƽ�����elem�ĸ���
	
	it = setInt.lower_bound(1);             //lower_bound(elem) ���ص�һ��С�ڵ���elemԪ�صĵ����� 
	std::cout<<*it<<std::endl;
	it = setInt.upper_bound(1);				//lower_bound(elem) ���ص�һ������elemԪ�صĵ�����
	std::cout<<*it<<std::endl;
	
	std::pair<std::set<int>::iterator,std::set<int>::iterator>pairIt;   //��������
	pairIt = setInt.equal_range(2);         //equal_range(elem) ���ص������ԣ���ʾָ��elem��lower_bound��upper_bound
	std::cout<<*pairIt.first<<std::endl;
	std::cout<<*pairIt.second<<std::endl;

	return true;
}

bool MultisetLearn(void)
{
	std::multiset<int>mulsetInt;
	//��set���ƣ��������ڣ�set֧��Ψһ��ֵ��ÿ��Ԫ��ֵֻ�ܳ���һ�Σ���multiset��ͬһֵ���Գ��ֶ��
	return true;
}