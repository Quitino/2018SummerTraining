#include "ComprehensiveTitle.h"

CSingingCompetition::CSingingCompetition(void)
{
	m_nRound = 0;
	srand( (unsigned)time(0) ); //设置随机种子
};

bool CSingingCompetition::JoinCompetition(void)
{
	std::string strNameBaseSource("ABCDEFGHIJKLMNOPQRSTUVWXYZ");      //名字组成元素的来源
	random_shuffle(strNameBaseSource.begin(),strNameBaseSource.end());//对名字容器元素进行随机排列  

	for(int i = 0;i < 24;i++)
	{
		std::string strExt(1,strNameBaseSource[i]);                   //生成名字，取一位字符

		Singer Ssinger;
		Ssinger.dLatestScore = 0.0;
		Ssinger.strName = "选手";
		Ssinger.strName += strExt;

		m_mapSinger.insert(std::pair<int,Singer>(i+100,Ssinger));
		m_lstRemainingID.push_back(i+100);
	}

	for(std::map<int,Singer>::iterator it = m_mapSinger.begin();it != m_mapSinger.end();it++)
	{
		TRACE("歌手:%s    参赛ID:%d\n",it->second.strName.c_str(),it->first);  //TRACE下面窗口输出显示。
		//std::cout<<"歌手："<<it->second.strName.c_str()<<"\t参赛ID："<<it->first<<std::endl;
	}

	return true;
}

bool CSingingCompetition::FirstKnockout(void)
{
	if (0 == m_nRound)
	{
		m_nRound = 1;
		Knockout(); //进行淘汰

		std::cout<<"第"<<m_nRound<<"轮淘汰赛中被淘汰的歌手的ID、名字及得分："<<std::endl;
		for (std::vector<int>::iterator it = m_vecIDBeEliminatedInFirstRound.begin();it != m_vecIDBeEliminatedInFirstRound.end();++it)
		{
			std::cout<<*it<<" "<<m_mapSinger[*it].strName.c_str()<<" "<<m_mapSinger[*it].dLatestScore<<std::endl;;
		}
		std::cout<<std::endl;
		std::cout<<std::endl;
	}
	return true;
}

bool CSingingCompetition::SecondKnockout(void)
{
	if (1 == m_nRound)
	{
		m_nRound = 2;
		Knockout();    //进行淘汰赛

		std::cout<<"第"<<m_nRound<<"轮淘汰赛中被淘汰的歌手的分数："<<std::endl;

		for (std::multiset<double>::iterator it = m_mltsetScoreBeEliminatedInSecondRound.begin();it != m_mltsetScoreBeEliminatedInSecondRound.end();++it)
		{
			std::cout<<std::setiosflags(std::ios::fixed)<<std::setprecision(2)<<*it<<" \t";
		}
		std::cout<<std::endl<<std::endl;;
	}

	return true;
}

bool CSingingCompetition::Finals(void)
{
	if (2 == m_nRound)
	{
		m_nRound = 3;

		for (std::list<int>::iterator it = m_lstRemainingID.begin();it != m_lstRemainingID.end();++it)
		{
			//生成歌手的分数
			MakeScore(m_mapSinger[*it]);

			//记录当前小组歌手的得分情况，按分数降序排列
			m_mltmapCurGroup.insert(std::pair<double,int>(m_mapSinger[*it].dLatestScore,*it));
		}

		std::cout<<"小组决赛情况"<<std::endl;
		for (std::multimap<double,int,std::greater<double>>::iterator itMltMapScoreToID = m_mltmapCurGroup.begin();itMltMapScoreToID != m_mltmapCurGroup.end();itMltMapScoreToID++)
		{
			std::cout<<m_mapSinger[itMltMapScoreToID->second].strName.c_str()<<"的得分："<<std::setiosflags(std::ios::fixed)<<std::setprecision(2)<<itMltMapScoreToID->first<<std::endl;
		}
		std::cout<<std::endl;

		EraseInCurGroup();

		//std::string strCompetitionRes[3] = {"Champion","Second","Third"};
		std::vector<std::string>vecStrCompetitionRes;
		vecStrCompetitionRes.push_back("冠军");
		vecStrCompetitionRes.push_back("亚军");
		vecStrCompetitionRes.push_back("季军");
		int nLabel = 0;

		std::cout<<"决赛结果"<<std::endl;
		std::cout<<"名次\t 编号\t 姓名\t 得分"<<std::endl;
		for (std::multimap<double,int,std::greater<double>>::iterator it = m_mltmapCurGroup.begin();it != m_mltmapCurGroup.end();it++)
		{
			std::cout<<vecStrCompetitionRes[nLabel++].c_str()<<"\t "
				<<it->second<<"\t "<<m_mapSinger[it->second].strName.c_str()<<"\t "
				<<std::setiosflags(std::ios::fixed)<<std::setprecision(2)<<it->first<<std::endl;
		}
		
		//清除所有数据
		m_mapSinger.clear();
		m_lstRemainingID.clear();
		m_vecIDBeEliminatedInFirstRound.clear();
		m_mltsetScoreBeEliminatedInSecondRound.clear();
		m_mltmapCurGroup.clear();
		m_nRound = 0;
	}

	return true;
}

bool CSingingCompetition::Knockout(void)
{
	std::cout<<"第"<<m_nRound<<"轮淘汰赛"<<std::endl;

	int nSingerIndex = 0;

	for (std::list<int>::iterator it = m_lstRemainingID.begin();it != m_lstRemainingID.end();)
	{
		nSingerIndex++;

		MakeScore(m_mapSinger[*it]);

		//记录当前演唱小组歌手的得分情况，按分数降序排列
		m_mltmapCurGroup.insert(std::pair<double,int>(m_mapSinger[*it].dLatestScore,*it));

		if (0==nSingerIndex%6)
		{
			//小组演唱完毕，打印得分情况
			PrintGroupScore();

			//在当前小组中删除歌手
			EraseInCurGroup();
			
			//在剩余歌手中删除歌手
			EraseInRemainingID(it++);
		}
		else
		{
			it++;
		}
	}
	return true;
}

bool CSingingCompetition::MakeScore(Singer &Ssinger)
{
	std::deque<double>deqScore;

	for (int i = 0;i<10;i++)
	{
		double dScore =  60.0 + rand()%40;
		deqScore.push_back(dScore);
	}
	sort(deqScore.begin(),deqScore.end());//排序
	
	deqScore.pop_front();   //去掉最高与最低
	deqScore.pop_back();

	double dScoreSum = accumulate(deqScore.begin(),deqScore.end(),0);  //求和
	double dScoreAverage = (double)(dScoreSum/deqScore.size());        //获得平均值

	Ssinger.dLatestScore = dScoreAverage;

	return true;
}

bool CSingingCompetition::PrintGroupScore(void)
{
	std::cout<<"小组得分情况："<<std::endl;

	for (std::multimap<double,int,std::greater<double>>::iterator it = m_mltmapCurGroup.begin();it != m_mltmapCurGroup.end();it++)
	{
		std::cout<<m_mapSinger[it->second].strName.c_str()<<"的得分："<<std::setiosflags(std::ios::fixed)<<std::setprecision(2)<<it->first<<std::endl;
	}
	std::cout<<std::endl;

	return true;
}

bool CSingingCompetition::EraseInCurGroup(void)
{
	int nSingerLastIndexInGroup = 0;   //组内歌手的倒数索引

	while (nSingerLastIndexInGroup < 3)
	{
		std::multimap<double,int,std::greater<double>>::iterator it = m_mltmapCurGroup.end();
		it--;

		nSingerLastIndexInGroup++;

		if (m_nRound == 1)
		{
			//记录第一轮淘汰赛中被淘汰的歌手的参赛号
			m_vecIDBeEliminatedInFirstRound.push_back(it->second);
		}
		else if (m_nRound == 2)
		{
			//记录第二轮淘汰赛中被淘汰的歌手的分数
			m_mltsetScoreBeEliminatedInSecondRound.insert(m_mapSinger[it->second].dLatestScore);
		}

		//从当前演唱小组的集合容器中删除最后一个元素
		m_mltmapCurGroup.erase(it);
	}

	return true;
}

bool CSingingCompetition::EraseInRemainingID(std::list<int>::iterator it)
{
	int nSingerReverseIndexInGroup = 0;   //逆向遍历索引

	while (nSingerReverseIndexInGroup < 6)
	{
		//查找逆向遍历迭代器所指的参赛ID所对应歌手的{分数，参赛ID}是否在当前演唱小组中
		std::multimap<double,int,std::greater<double>>::iterator itMltMapScoreToID;
		itMltMapScoreToID = find(m_mltmapCurGroup.begin(),m_mltmapCurGroup.end(),
			std::multimap<double,int,std::greater<double>>::value_type(m_mapSinger[*it].dLatestScore,*it));
	
		if (itMltMapScoreToID == m_mltmapCurGroup.end())
		{
			//没找到，从剩余歌手集合中删除该歌手
			it = m_lstRemainingID.erase(it);
		}

		//索引自增
		nSingerReverseIndexInGroup++;

		//迭代器处理,防止对begin迭代器进行处理
		if (it != m_lstRemainingID.begin())
		{
			it--;
		}
	}

	//清除该组的比赛记录存储，以便下一组比赛记录的存储
	m_mltmapCurGroup.clear();

	return true;
}

