#include "VectorLearn.h"

bool VectorLearn(void)
{
	std::cout << std::endl << "**************VectorLearn_display************* " << std::endl << std::endl;
	std::vector<std::vector<int>>vvint;//vectorǶ�ף��൱�ڶ�ά��ֵ
	std::vector<int>vecInt;
	vecInt.clear();				//��֮ǰ����һ��
	vecInt.push_back(1);
	vecInt.push_back(3);
	vecInt.push_back(5);
	vecInt.push_back(7);
	vecInt.push_back(9);
	
	try   //����ִ�иþ䣬�ڴ�Խ������⣬����Ҫ��
	{
		std::cout<<vecInt.at(5)<<std::endl;
	}
	catch (std::out_of_range &exc)             //���Բ����޵��쳣
	{
		std::cerr<<"File:"<<__FILE__<<" Line:"<<__LINE__<<"  "<<exc.what()<<std::endl;
	}                                          //�쳣����

	vecInt.front() = 25;						//front �滻vectorͷԪ��
	std::cout<<"�滻��һ��Ԫ��:"<<vecInt.front()<<std::endl;
	vecInt.back() = 13;							//front �滻vectorβԪ��
	std::cout<<"�滻���һ��Ԫ��:"<<vecInt.back()<<std::endl;       //front��back����


	std::vector<int>::iterator it;				//������=>ָ��
	it = vecInt.begin();
	std::cout<<"vecInt.begin:"<<*it<<std::endl;
	it = (--vecInt.end());
	std::cout<<"vecInt.end:"<<*it<<std::endl;                 //��������begin��end����
											   //ע�⣺end�ĵ�������vectorĩβԪ�صĺ�һָ�룬�޷�����ȡֵ����
	int count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)  //end�����һ��Ԫ�ص���һ����ַ���Ƿ��ĵ�ַ
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" <<  *it << std::endl;
		
	}
	std::cout<<std::endl;						 //vector����

	//std::vector<int>::reverse_iterator rit;    //�����������rbegin��vectorĩβԪ�صĺ�һָ�룬�޷�����ȡֵ����
	//rit = (++vecInt.rend());                   //������ʹ�����������
	//std::cout<<*it<<std::endl;

	std::cout<<"vecInt.size:"<<vecInt.size()<<std::endl;     //size ��������Ԫ��ֵ
	if ( !vecInt.empty())						 //empty �ж������Ƿ�Ϊ��
	{
		std::cout<<"vecInt.empty = "<<false<<std::endl;
	}

	vecInt.resize(8,1);	 //resize �ض���������С����һ������Ϊ���ȣ�ԭ������С���ض��峤�ȣ�����չ�Եڶ���������ֵ������ض�
	
	std::cout << "resize(8,1)����" << std::endl; 
	count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		count++;		
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	//std::vector<int>vecData;
	//vecData.push_back(29);
	//vecData.push_back(30);

	vecInt.insert((vecInt.begin()+3),22);							 //insert(pos,elem)  ��ָ��λ�ò���ָ��ֵ
	//vecInt.insert((vecInt.begin()+3),4,22);						 //insert(pos,n,elem)  ��ָ��λ�ò���n��ָ��ֵ
	//vecInt.insert((vecInt.begin()+3),vecInt.begin(),vecInt.end()); //insert(pos,beg,end)  ��ָ��λ�ò���beg��end��ֵ
	//vecInt.insert((vecInt.begin()+3),vecData.begin(),vecData.end());
	std::cout << "vecInt.insert((vecInt.begin()+3),22)��ֵ��" << std::endl;
	count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	//vecInt.erase((vecInt.begin()+2),(vecInt.begin()+4));  //erase(beg,end),ɾ��[beg,end]֮�������
	vecInt.erase(vecInt.begin());							//erase(pos)��ɾ��pos��������
	std::cout << "vecInt.erase(vecInt.begin())ɾֵ��" << std::endl;
	count = 0;
	for (it = vecInt.begin();it != vecInt.end();it++)
	{
			count++;
			std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	return true;
}