#include "MapLearn.h"

bool MapLearn(void)
{
	//std::map<int,std::string>mapStudent;                            //默认升序
	//std::map<int,std::string,std::less<int>>mapStudent;             //按key升序进行存放
	std::map<int,std::string,std::greater<int>>mapStudent;          //按key降序进行存放

	mapStudent.insert(std::pair<int,std::string>(1,"dialouch"));           //pair形式插入 返回pair<iterator,bool>
	mapStudent.insert(std::map<int,std::string>::value_type(2,"panda"));   //value_type形式插入 返回pair<iterator,bool>
	
	std::pair<std::map<int,std::string>::iterator,bool >pairIt;
	pairIt = mapStudent.insert(std::map<int,std::string>::value_type(3,"Little Girl"));

	std::cout<<(pairIt.first)->first<<" "<<(pairIt.first)->second<<std::endl;
	std::cout<<pairIt.second<<std::endl;     //insert返回为pair<iterator,bool>，迭代器（指针）指向第一个成员为key，第二个成员为data

	if ( !mapStudent.empty())
	{
		std::cout<<mapStudent.size()<<std::endl;
	}

	std::map<int,std::string,std::greater<int>>::iterator it;
	for (it = mapStudent.begin();it != mapStudent.end();it++)   //迭代器作map遍历
	{
		std::cout<<it->first<<" "<<it->second<<std::endl;
	}

	try
	{
		it = mapStudent.find(2);//find(key);   查找键key是否存在，若存在，返回该键的元素的迭代器；若不存在，返回map.end();
		std::cout<<it->first<<" "<<it->second<<std::endl;
	}
	catch(std::out_of_range &exc)
	{
		std::cerr<<"File:"<<__FILE__<<" Line:"<<__LINE__<<" "<<exc.what()<<std::endl;
	}

	//count lower_bound(keyElem) upper_bound(keyElem)与set类似

	std::pair<std::map<int,std::string,std::greater<int>>::iterator,std::map<int,std::string,std::greater<int>>::iterator>mapEqualPairIt;
	mapEqualPairIt = mapStudent.equal_range(2);           //迭代器对
	std::cout<<mapEqualPairIt.first->first<<" "<<mapEqualPairIt.first->second<<std::endl;
	std::cout<<mapEqualPairIt.second->first<<" "<<mapEqualPairIt.second->second<<std::endl;

	return true;
}

bool MultimapLearn(void)
{
	std::multimap<int,std::string,std::greater<int>>mulmapStudent;
	//与map类似
	return true;
}