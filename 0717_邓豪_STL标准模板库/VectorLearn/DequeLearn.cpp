#include "DequeLearn.h"    //֧�ֵ�����

bool DequeLearn(void)
{
	std::cout << std::endl << "**************DequeLearn_display************* " << std::endl << std::endl;
	std::deque<int>deqInt;
	
	deqInt.push_front(2);        //push_front �Ӷ���ͷ����������  pop_front �Ӷ���ͷ����������
	deqInt.push_front(1);
	deqInt.push_back(4);         //push_back  �Ӷ���β����������  pop_back  �Ӷ���β����������
	deqInt.push_back(5);

	std::deque<int>::iterator it;
	int count = 0;
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
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

	std::cout<<"deqInt.front(): "<<deqInt.front()<<std::endl;   //front ���ض��е�һ��Ԫ��
	std::cout<<"deqInt.back(): "<<deqInt.back()<<std::endl;    //back  ���ض������һ��Ԫ��
	std::cout<<"deqInt.size(): "<<deqInt.size()<<std::endl;    //size  ���ض��д�С
	if ( !deqInt.empty())					//empty �ж϶����Ƿ�Ϊ��
	{
		std::cout << "eqInt.empty(): " << false << std::endl;
	}

	deqInt.resize(6,0);                     //resize ͬvector resize
	std::cout << std::endl << "deqInt.resize(6,0)��" << std::endl << std::endl;
	count = 0;
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	std::deque<int>deqData;  //�¶����һ��˫�����
	//std::vector<int>deqData;
	deqData.push_back(11);
	deqData.push_back(12);
	//deqInt.insert((deqInt.begin()+2),3);   //ͬvector insert
	//deqInt.insert((deqInt.begin()+2),4,3);
	//deqInt.insert((deqInt.begin()+2),deqInt.begin(),deqInt.end());  //�˾�ᱨ��
	//���½����е�����Ԫ�ز���֮ǰ�Ķ���
	deqInt.insert((deqInt.begin()+2),deqData.begin(),deqData.end());
	std::cout << "deqInt.insert((deqInt.begin()+2),deqData.begin(),deqData.end())�� " << std::endl << std::endl;
	count = 0;
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	//deqInt.erase((deqInt.begin()),(deqInt.begin()+2));  //erase(beg,end),ɾ��[beg,end]֮�������
	deqInt.erase(deqInt.begin());                         //erase(pos)��ɾ��pos��������
	std::cout << "deqInt.erase(deqInt.begin())ɾ���� " << std::endl << std::endl;
	count = 0;
	for (it = deqInt.begin();it != deqInt.end();it++)
	{
		count++;
		std::cout << "�� " << count << " ��Ԫ��:" << *it << std::endl;
	}
	std::cout<<std::endl;

	return true;
}