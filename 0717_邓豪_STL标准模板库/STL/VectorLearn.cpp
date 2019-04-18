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
	}                                          //�쳣����

	vecInt.front() = 12;
	std::cout<<vecInt.front()<<std::endl;
	vecInt.back()  = 13;
	std::cout<<vecInt.back()<<std::endl;       //front��back����

	std::vector<int>::iterator it;
	it = vecInt.begin();
	std::cout<<*it<<std::endl;
	it = (--vecInt.end());
	std::cout<<*it<<std::endl;                 //��������begin��end����
											   //ע�⣺end�ĵ�������vectorĩβԪ�صĺ�һָ�룬�޷�����ȡֵ����

	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;                      //vector����

	//std::vector<int>::reverse_iterator rit;    //�����������rbegin��vectorĩβԪ�صĺ�һָ�룬�޷�����ȡֵ����
	//rit = (++vecInt.rend());                   //������ʹ�����������
	//std::cout<<*it<<std::endl;

	std::cout<<vecInt.size()<<std::endl;         //size ��������Ԫ��ֵ
	if ( !vecInt.empty())						 //empty �ж������Ƿ�Ϊ��
	{
		std::cout<<false<<std::endl;
	}
	vecInt.resize(8,1);							 //resize �ض���������С����һ������Ϊ���ȣ�ԭ������С���ض��峤�ȣ�����չ�Եڶ���������ֵ������ض�
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//std::vector<int>vecData;
	//vecData.push_back(29);
	//vecData.push_back(30);

	vecInt.insert((vecInt.begin()+3),22);							 //insert(pos,elem)  ��ָ��λ�ò���ָ��ֵ
	//vecInt.insert((vecInt.begin()+3),4,22);						 //insert(pos,n,elem)  ��ָ��λ�ò����Ǹ��ƶ�ֵ
	//vecInt.insert((vecInt.begin()+3),vecInt.begin(),vecInt.end()); //insert(pos,beg,end)  ��ָ��λ�ò���beg��end��ֵ
	//vecInt.insert((vecInt.begin()+3),vecData.begin(),vecData.end());
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	//vecInt.erase((vecInt.begin()+2),(vecInt.begin()+4));  //erase(beg,end),ɾ��[beg,end]֮�������
	vecInt.erase(vecInt.begin());                         //erase(pos)��ɾ��pos��������
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		std::cout<<*it<<" ";
	}
	std::cout<<std::endl;

	return true;
}