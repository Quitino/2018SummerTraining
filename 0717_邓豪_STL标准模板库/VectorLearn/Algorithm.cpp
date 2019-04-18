#include "Algorithm.h"

bool AlgorithmLearn(void)
{
	std::vector<int>vecInt;
	vecInt.push_back(1);
	vecInt.push_back(3);
	vecInt.push_back(5);
	vecInt.push_back(7);
	vecInt.push_back(9);
	
	/* adjacent_find():��iterator�Ա�ʶԪ�ط�Χ�ڣ�����һ�������ظ�Ԫ�أ�
	�ҵ��򷵻�ָ�����Ԫ�صĵ�һ��Ԫ�صĵ����������򷵻�past-the-end*/
	std::vector<int>::iterator it;
	it = adjacent_find(vecInt.begin(),vecInt.end());
	if (it != vecInt.end())
	{
		std::cout<<(it-vecInt.begin())+1<<" "<<*it<<std::endl;
	}
	else std::cout<<"False"<<std::endl;

	/*binary_search:   �����������в���value,�ҵ��򷵻�true��
	ע�⣺�����������У�����ʹ��*/
	std::set<int>setInt;						//Ĭ����������
	setInt.insert(5);
	setInt.insert(3);
	setInt.insert(4);
	setInt.insert(2);
	setInt.insert(1);
	bool bFind = binary_search(setInt.begin(),setInt.end(),3);    //������û�����Ԫ��
	std::cout<<bFind<<std::endl;

	/*	count:     ���õ��ڲ��������ѱ�־��Χ�ڵ�Ԫ��������ֵ�Ƚϣ�������ȵĸ�����
		count_if:  ��������ĺ������Ա�־��Χ�ڵ�Ԫ�ؽ��бȽϲ��������ؽ��Ϊtrue�ĸ���*/
	int nCount = count_if(vecInt.begin(),vecInt.end(),GreaterThree);
	std::cout<<nCount<<std::endl;
	int nCountEqual = count(vecInt.begin(),vecInt.end(),2);
	std::cout<<nCountEqual<<std::endl;

	/*	find:  ���õײ�Ԫ�صĵ��ڲ���������ָ����Χ�ڵ�Ԫ��������ֵ���бȽϡ���ƥ��ʱ���������������ظ�Ԫ�صĵ�����
		find_if:   ʹ������ĺ���������ڲ�����ִ��find�����ر��ҵ���Ԫ�صĵ�����
	*/
	std::vector<int>::iterator itvecInt;
	itvecInt = find(vecInt.begin(),vecInt.end(),2);
	if (itvecInt != vecInt.end())     //������δ�ҵ�ƥ��ֵʱ������β����������ֹ���
	{
		std::cout<<*itvecInt<<std::endl;
	}
	else std::cout<<"Find fail"<<std::endl;
	itvecInt = find_if(vecInt.begin(),vecInt.end(),GreaterThree);
	if (itvecInt != vecInt.end())
	{
		std::cout<<*itvecInt<<std::endl;            //��һ������3��Ԫ��
	}
	else std::cout<<"Find Greater Fail"<<std::endl;

	/*
		merge:    �ϲ������������У���ŵ���һ������,vectorʹ��merge�����ڲ�Ԫ������
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
	sort(vecInt.begin(),vecInt.end(),std::greater<int>());    //��������
	//sort(vecInt.begin(),vecInt.end(),std::less<int>());       //��������
	//sort(vecInt.begin(),vecInt.end());                        //Ĭ����������
	for (std::vector<int>::iterator itvec = vecInt.begin();itvec != vecInt.end();itvec++ )
	{
		std::cout<<*itvec<<" ";
	}
	std::cout<<std::endl;
	vecInt.erase(vecInt.begin());          //ɾ��12

	/*	random_shuffle:	��ָ����Χ�ڵ�Ԫ�������������
		reverse:		��ָ����Χ��Ԫ�����·�������*/

	vecInt2.clear();
	vecInt2.resize(5);
	copy(vecInt.begin(),vecInt.end(),vecInt2.begin());    //��������
	for (std::vector<int>::iterator itvec = vecInt2.begin();itvec != vecInt2.end();itvec++)
	{
		std::cout<<*itvec<<" ";
	}
	std::cout<<std::endl;

	/*replace(beg,end,oldValue,newValue):��ָ����Χ�ڵ����е���oldValue��Ԫ���滻��newValue��*/
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

	/*	accumulate:  ��ָ����Χ�ڵ�Ԫ����ͣ�Ȼ�����ټ���һ����valָ���ĳ�ʼֵ��
		fill:		 ������ֵ������־��Χ�ڵ�����Ԫ�ء�*/

	int nvecIntSum = 5;
	nvecIntSum = accumulate(vecInt.begin(),vecInt.end(),nvecIntSum);     //���
	std::cout<<nvecIntSum<<std::endl;

	/*
	set_union:			����һ���������У����������������еĲ�����
	set_intersection:	����һ���������У����������������еĽ�����
	set_difference:		����һ���������У������б�����һ�����������д��ڶ��ڶ������������в����ڵ�Ԫ�ء�
	*/

	/*
	for_each:	��ָ���������ζ�ָ����Χ������Ԫ�ؽ��е������ʡ��ú��������޸������е�Ԫ�ء�
	transform:  ��for_each���ƣ���������Ԫ�أ����ɶ�������Ԫ�ؽ����޸�
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
