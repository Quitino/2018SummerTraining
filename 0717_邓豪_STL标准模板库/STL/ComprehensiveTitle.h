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

#include <afx.h>   //TRACE 头文件声明，TRACE：调试状态下日志输出

typedef struct SSinger
{
	std::string strName;
	double dLatestScore;
}Singer;

class CSingingCompetition
{
public:
	CSingingCompetition(void);
	bool JoinCompetition(void);								//报名参加比赛
	bool FirstKnockout(void);								//第一轮淘汰赛	
	bool SecondKnockout(void);								//第二轮淘汰赛	
	bool Finals(void);										//决赛
	bool Knockout(void);									//比赛
	bool MakeScore(Singer &Ssinger);						//打分
	bool PrintGroupScore(void);								//打印小组得分
	bool EraseInCurGroup(void);								//从当前小组删除
	bool EraseInRemainingID(std::list<int>::iterator it);	//从剩下ID中删除

private:
	int m_nRound;

private:
	std::map<int,Singer>m_mapSinger;							 //所有的参赛ID与歌手的映射集合
	std::list<int>m_lstRemainingID;								 //剩余歌手的参赛ID
	//当前演唱小组的歌手分数与歌手参赛ID的映射集合,降序，double 分数，int ID
	std::multimap<double,int,std::greater<double>>m_mltmapCurGroup;
	std::vector<int>m_vecIDBeEliminatedInFirstRound;			 //第一轮比赛中被淘汰的歌手的ID集合
	std::multiset<double>m_mltsetScoreBeEliminatedInSecondRound; //第二轮比赛中被淘汰的歌手的分数集合
};

#endif