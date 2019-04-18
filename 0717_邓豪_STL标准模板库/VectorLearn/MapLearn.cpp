#include "MapLearn.h"

bool MapLearn(void)  //key值不能一样
{
	std::cout << std::endl << "**************MapLearn_display************* " << std::endl << std::endl;
	//std::map<int,std::string>mapStudent;                            //默认升序
	//std::map<int,std::string,std::less<int>>mapStudent;             //按key升序进行存放
	std::map<int,std::string,std::greater<int>>mapStudent;            //按key降序进行存放

	mapStudent.insert(std::pair<int,std::string>(1,"dialouch"));           //pair形式插入 返回pair<iterator,bool>
	mapStudent.insert(std::map<int,std::string>::value_type(2,"panda"));   //value_type形式插入 返回pair<iterator,bool>
	





	std::pair<std::map<int,std::string>::iterator,bool >pairIt;             //pair成对！！！pair是C++里面的一种数据类型！！
	pairIt = mapStudent.insert(std::map<int,std::string>::value_type(3,"Little_Girl"));
	std::cout<<"(pairIt.first)->first: "<<(pairIt.first)->first<<
		std::endl << "(pairIt.first)->second: " << (pairIt.first)->second << std::endl << std::endl;

	std::cout << "pairIt.second:" << pairIt.second << std::endl << std::endl; //输出为1，即是那个bool型   
	//insert返回为pair<iterator,bool>，迭代器（指针）指向第一个成员为key，第二个成员为data

	if ( !mapStudent.empty())
	{
		std::cout << "mapStudent.size():" << mapStudent.size() << std::endl << std::endl;
	}

	std::map<int,std::string,std::greater<int>>::iterator it;  //按key降序进行存放
	int count = 0;
	for (it = mapStudent.begin();it != mapStudent.end();it++)   //迭代器作map遍历
	{
		count++;
		//为啥不用*取值？？？
		std::cout << "第 " << count << " 个元素:" << " first: " << it->first 
			<< ";  second: " << it->second << "." <<  std::endl;
	}
	std::cout<<std::endl;

	try
	{
		it = mapStudent.find(2);//find(key);   查找键key是否存在，若存在，返回该键的元素的迭代器；若不存在，返回map.end();
		std::cout << "it = mapStudent.find(2): " << it->first << " " << it->second << std::endl << std::endl;
	}
	catch(std::out_of_range &exc)
	{
		std::cerr<<"File:"<<__FILE__<<" Line:"<<__LINE__<<" "<<exc.what()<<std::endl;
	}



	//count lower_bound(keyElem) upper_bound(keyElem)与set类似

	std::pair<std::map<int,std::string,std::greater<int>>::iterator,std::map<int,std::string,std::greater<int>>::iterator>mapEqualPairIt;
	mapEqualPairIt = mapStudent.equal_range(2);  // 2为key值！！//迭代器对,这个什么意思没看懂？？？？？==>两个迭代器
	std::cout<<"mapEqualPairIt.first->first: "<<mapEqualPairIt.first->first
		<< "   mapEqualPairIt.first->second: " << mapEqualPairIt.first->second << std::endl ;
	std::cout << "mapEqualPairIt.second->first: " << mapEqualPairIt.second->first 
		<< "   mapEqualPairIt.second->second: " << mapEqualPairIt.second->second << std::endl ;

	return true;
}

bool MultimapLearn(void)   //复合图，多重图，key值可以一样
{
	std::multimap<int,std::string,std::greater<int>>mulmapStudent;
	//与map类似
	return true;
}