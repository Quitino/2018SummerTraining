#include "Algorithm.h"

bool AlgorithmLearn(void)
{
	std::vector<int>vecInt;
	vecInt.push_back(1);
	vecInt.push_back(3);
	vecInt.push_back(5);
	vecInt.push_back(7);
	vecInt.push_back(9);
	
	/* adjacent_find():在iterator对标识元素范围内，查找一对相邻重复元素，
	找到则返回指向这对元素的第一个元素的迭代器。否则返回past-the-end*/
	std::vector<int>::iterator it;
	it = adjacent_find(vecInt.begin(),vecInt.end());
	if (it != vecInt.end())
	{
		std::cout<<(it-vecInt.begin())+1<<" "<<*it<<std::endl;
	}
	else std::cout<<"False"<<std::endl;

	/*binary_search:   在有序序列中查找value,找到则返回true。
	注意：在无序序列中，不可使用*/
	std::set<int>setInt;						//默认升序排列
	setInt.insert(5);
	setInt.insert(3);
	setInt.insert(4);
	setInt.insert(2);
	setInt.insert(1);
	bool bFind = binary_search(setInt.begin(),setInt.end(),3);    //查找有没有这个元素
	std::cout<<bFind<<std::endl;

	/*	count:     利用等于操作符，把标志范围内的元素与输入值比较，返回相等的个数。
		count_if:  利用输入的函数，对标志范围内的元素进行比较操作，返回结果为true的个数*/
	int nCount = count_if(vecInt.begin(),vecInt.end(),GreaterThree);
	std::cout<<nCount<<std::endl;
	int nCountEqual = count(vecInt.begin(),vecInt.end(),2);
	std::cout<<nCountEqual<<std::endl;

	/*	find:  利用底层元素的等于操作符，对指定范围内的元素与输入值进行比较。当匹配时，结束搜索，返回该元素的迭代器
		find_if:   使用输入的函数代替等于操作符执行find。返回被找到的元素的迭代器
	*/
	std::vector<int>::iterator itvecInt;
	itvecInt = find(vecInt.begin(),vecInt.end(),2);
	if (itvecInt != vecInt.end())     //容器中未找到匹配值时，返回尾迭代器，防止溢出
	{
		std::cout<<*itvecInt<<std::endl;
	}
	else std::cout<<"Find fail"<<std::endl;
	itvecInt = find_if(vecInt.begin(),vecInt.end(),GreaterThree);
	if (itvecInt != vecInt.end())
	{
		std::cout<<*itvecInt<<std::endl;            //第一个大于3的元素
	}
	else std::cout<<"Find Greater Fail"<<std::endl;

	/*
		merge:    合并两个有序序列，存放到另一个序列,vector使用merge必须内部元素有序
	*/
	std::vector<int>vecInt2;
	std::vector<int>vecIntRes;
	vecInt2.assign(4,6);
	vecIntRes.resize(9);
	merge(vecInt.begin(),vecInt.end(),vecInt2.begin(),vecInt2.end(),vecIntRes.begin());
	for (itvecInt = vecIntRes.begin();itvecInt != vecIntRes.end();itvecInt++)
	{
		std::cout<<*itvecInt<<std::endl;
	}
	std::cout<<std::endl;

	vecInt.insert(vecInt.begin()+3,12);
	sort(vecInt.begin(),vecInt.end(),std::greater<int>());    //降序排序
	//sort(vecInt.begin(),vecInt.end(),std::less<int>());       //升序排序
	//sort(vecInt.begin(),vecInt.end());                        //默认升序排序
	for (std::vector<int>::iterator itvec = vecInt.begin();itvec != vecInt.end();itvec++ )
	{
		std::cout<<*itvec<<" ";
	}
	std::cout<<std::endl;
	vecInt.erase(vecInt.begin());          //删除12

	/*	random_shuffle:	对指定范围内的元素随机调整次序。
		reverse:		对指定范围内元素重新反序排序。*/

	vecInt2.clear();
	vecInt2.resize(5);
	copy(vecInt.begin(),vecInt.end(),vecInt2.begin());    //拷贝函数
	for (std::vector<int>::iterator itvec = vecInt2.begin();itvec != vecInt2.end();itvec++)
	{
		std::cout<<*itvec<<" ";
	}
	std::cout<<std::endl;

	/*replace(beg,end,oldValue,newValue):将指定范围内的所有等于oldValue的元素替换成newValue。*/
	replace(vecInt2.begin(),vecInt2.end(),9,12);
	for (std::vector<int>::iterator itvec = vecInt2.begin();itvec != vecInt2.end();itvec++)
	{
		std::cout<<*itvec<<" ";
	}
	std::cout<<std::endl;

	replace_if(vecInt2.begin(),vecInt2.end(),GreaterThree,0);
	for (std::vector<int>::iterator itvec = vecInt2.begin();itvec != vecInt2.end();itvec++)
	{
		std::cout<<*itvec<<" ";
	}
	std::cout<<std::endl;

	/*	accumulate:  对指定范围内的元素求和，然后结果再加上一个由val指定的初始值。
		fill:		 将输入值赋给标志范围内的所有元素。*/

	int nvecIntSum = 5;
	nvecIntSum = accumulate(vecInt.begin(),vecInt.end(),nvecIntSum);     //求和
	std::cout<<nvecIntSum<<std::endl;

	/*
	set_union:			构造一个有序序列，包含两个有序序列的并集。
	set_intersection:	构造一个有序序列，包含两个有序序列的交集。
	set_difference:		构造一个有序序列，该序列保留第一个有序序列中存在而第二个有序序列中不存在的元素。
	*/

	/*
	for_each:	用指定函数依次对指定范围内所有元素进行迭代访问。该函数不得修改序列中的元素。
	transform:  与for_each类似，遍历所有元素，但可对容器的元素进行修改
	*/

	return true;
}

bool GreaterThree(int nNumber)
{
	if (nNumber > 3)
	{
		return true;
	}
	return false;
}
