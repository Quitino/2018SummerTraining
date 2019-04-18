#include "MapLearn.h"

bool MapLearn(void)  //keyֵ����һ��
{
	std::cout << std::endl << "**************MapLearn_display************* " << std::endl << std::endl;
	//std::map<int,std::string>mapStudent;                            //Ĭ������
	//std::map<int,std::string,std::less<int>>mapStudent;             //��key������д��
	std::map<int,std::string,std::greater<int>>mapStudent;            //��key������д��

	mapStudent.insert(std::pair<int,std::string>(1,"dialouch"));           //pair��ʽ���� ����pair<iterator,bool>
	mapStudent.insert(std::map<int,std::string>::value_type(2,"panda"));   //value_type��ʽ���� ����pair<iterator,bool>
	





	std::pair<std::map<int,std::string>::iterator,bool >pairIt;             //pair�ɶԣ�����pair��C++�����һ���������ͣ���
	pairIt = mapStudent.insert(std::map<int,std::string>::value_type(3,"Little_Girl"));
	std::cout<<"(pairIt.first)->first: "<<(pairIt.first)->first<<
		std::endl << "(pairIt.first)->second: " << (pairIt.first)->second << std::endl << std::endl;

	std::cout << "pairIt.second:" << pairIt.second << std::endl << std::endl; //���Ϊ1�������Ǹ�bool��   
	//insert����Ϊpair<iterator,bool>����������ָ�룩ָ���һ����ԱΪkey���ڶ�����ԱΪdata

	if ( !mapStudent.empty())
	{
		std::cout << "mapStudent.size():" << mapStudent.size() << std::endl << std::endl;
	}

	std::map<int,std::string,std::greater<int>>::iterator it;  //��key������д��
	int count = 0;
	for (it = mapStudent.begin();it != mapStudent.end();it++)   //��������map����
	{
		count++;
		//Ϊɶ����*ȡֵ������
		std::cout << "�� " << count << " ��Ԫ��:" << " first: " << it->first 
			<< ";  second: " << it->second << "." <<  std::endl;
	}
	std::cout<<std::endl;

	try
	{
		it = mapStudent.find(2);//find(key);   ���Ҽ�key�Ƿ���ڣ������ڣ����ظü���Ԫ�صĵ��������������ڣ�����map.end();
		std::cout << "it = mapStudent.find(2): " << it->first << " " << it->second << std::endl << std::endl;
	}
	catch(std::out_of_range &exc)
	{
		std::cerr<<"File:"<<__FILE__<<" Line:"<<__LINE__<<" "<<exc.what()<<std::endl;
	}



	//count lower_bound(keyElem) upper_bound(keyElem)��set����

	std::pair<std::map<int,std::string,std::greater<int>>::iterator,std::map<int,std::string,std::greater<int>>::iterator>mapEqualPairIt;
	mapEqualPairIt = mapStudent.equal_range(2);  // 2Ϊkeyֵ����//��������,���ʲô��˼û��������������==>����������
	std::cout<<"mapEqualPairIt.first->first: "<<mapEqualPairIt.first->first
		<< "   mapEqualPairIt.first->second: " << mapEqualPairIt.first->second << std::endl ;
	std::cout << "mapEqualPairIt.second->first: " << mapEqualPairIt.second->first 
		<< "   mapEqualPairIt.second->second: " << mapEqualPairIt.second->second << std::endl ;

	return true;
}

bool MultimapLearn(void)   //����ͼ������ͼ��keyֵ����һ��
{
	std::multimap<int,std::string,std::greater<int>>mulmapStudent;
	//��map����
	return true;
}