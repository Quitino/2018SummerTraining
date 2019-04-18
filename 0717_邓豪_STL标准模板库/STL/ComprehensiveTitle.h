#ifndef COMPREHENSIVE_TITLE_H
#define COMPREHENSIVE_TITLE_H

#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <iomanip>

#include <vector>
#include <map>
#include <set>
#include <deque>
#include <list>

#include <algorithm>
#include <numeric>
#include <xfunctional>

#include <afx.h>   //TRACE ͷ�ļ�������TRACE������״̬����־���

typedef struct SSinger
{
	std::string strName;
	double dLatestScore;
}Singer;

class CSingingCompetition
{
public:
	CSingingCompetition(void);
	bool JoinCompetition(void);								//�����μӱ���
	bool FirstKnockout(void);								//��һ����̭��	
	bool SecondKnockout(void);								//�ڶ�����̭��	
	bool Finals(void);										//����
	bool Knockout(void);									//����
	bool MakeScore(Singer &Ssinger);						//���
	bool PrintGroupScore(void);								//��ӡС��÷�
	bool EraseInCurGroup(void);								//�ӵ�ǰС��ɾ��
	bool EraseInRemainingID(std::list<int>::iterator it);	//��ʣ��ID��ɾ��

private:
	int m_nRound;

private:
	std::map<int,Singer>m_mapSinger;							 //���еĲ���ID����ֵ�ӳ�伯��
	std::list<int>m_lstRemainingID;								 //ʣ����ֵĲ���ID
	//��ǰ�ݳ�С��ĸ��ַ�������ֲ���ID��ӳ�伯��,����double ������int ID
	std::multimap<double,int,std::greater<double>>m_mltmapCurGroup;
	std::vector<int>m_vecIDBeEliminatedInFirstRound;			 //��һ�ֱ����б���̭�ĸ��ֵ�ID����
	std::multiset<double>m_mltsetScoreBeEliminatedInSecondRound; //�ڶ��ֱ����б���̭�ĸ��ֵķ�������
};

#endif