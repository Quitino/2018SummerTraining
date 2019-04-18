#include "ComprehensiveTitle.h"

CSingingCompetition::CSingingCompetition(void)
{
	m_nRound = 0;
	srand( (unsigned)time(0) ); //�����������
};

bool CSingingCompetition::JoinCompetition(void)
{
	std::string strNameBaseSource("ABCDEFGHIJKLMNOPQRSTUVWXYZ");      //�������Ԫ�ص���Դ
	random_shuffle(strNameBaseSource.begin(),strNameBaseSource.end());//����������Ԫ�ؽ����������  

	for(int i = 0;i < 24;i++)
	{
		std::string strExt(1,strNameBaseSource[i]);                   //�������֣�ȡһλ�ַ�

		Singer Ssinger;
		Ssinger.dLatestScore = 0.0;
		Ssinger.strName = "ѡ��";
		Ssinger.strName += strExt;

		m_mapSinger.insert(std::pair<int,Singer>(i+100,Ssinger));
		m_lstRemainingID.push_back(i+100);
	}

	for(std::map<int,Singer>::iterator it = m_mapSinger.begin();it != m_mapSinger.end();it++)
	{
		TRACE("����:%s    ����ID:%d\n",it->second.strName.c_str(),it->first);  //TRACE���洰�������ʾ��
		//std::cout<<"���֣�"<<it->second.strName.c_str()<<"\t����ID��"<<it->first<<std::endl;
	}

	return true;
}

bool CSingingCompetition::FirstKnockout(void)
{
	if (0 == m_nRound)
	{
		m_nRound = 1;
		Knockout(); //������̭

		std::cout<<"��"<<m_nRound<<"����̭���б���̭�ĸ��ֵ�ID�����ּ��÷֣�"<<std::endl;
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
		Knockout();    //������̭��

		std::cout<<"��"<<m_nRound<<"����̭���б���̭�ĸ��ֵķ�����"<<std::endl;

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
			//���ɸ��ֵķ���
			MakeScore(m_mapSinger[*it]);

			//��¼��ǰС����ֵĵ÷��������������������
			m_mltmapCurGroup.insert(std::pair<double,int>(m_mapSinger[*it].dLatestScore,*it));
		}

		std::cout<<"С��������"<<std::endl;
		for (std::multimap<double,int,std::greater<double>>::iterator itMltMapScoreToID = m_mltmapCurGroup.begin();itMltMapScoreToID != m_mltmapCurGroup.end();itMltMapScoreToID++)
		{
			std::cout<<m_mapSinger[itMltMapScoreToID->second].strName.c_str()<<"�ĵ÷֣�"<<std::setiosflags(std::ios::fixed)<<std::setprecision(2)<<itMltMapScoreToID->first<<std::endl;
		}
		std::cout<<std::endl;

		EraseInCurGroup();

		//std::string strCompetitionRes[3] = {"Champion","Second","Third"};
		std::vector<std::string>vecStrCompetitionRes;
		vecStrCompetitionRes.push_back("�ھ�");
		vecStrCompetitionRes.push_back("�Ǿ�");
		vecStrCompetitionRes.push_back("����");
		int nLabel = 0;

		std::cout<<"�������"<<std::endl;
		std::cout<<"����\t ���\t ����\t �÷�"<<std::endl;
		for (std::multimap<double,int,std::greater<double>>::iterator it = m_mltmapCurGroup.begin();it != m_mltmapCurGroup.end();it++)
		{
			std::cout<<vecStrCompetitionRes[nLabel++].c_str()<<"\t "
				<<it->second<<"\t "<<m_mapSinger[it->second].strName.c_str()<<"\t "
				<<std::setiosflags(std::ios::fixed)<<std::setprecision(2)<<it->first<<std::endl;
		}
		
		//�����������
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
	std::cout<<"��"<<m_nRound<<"����̭��"<<std::endl;

	int nSingerIndex = 0;

	for (std::list<int>::iterator it = m_lstRemainingID.begin();it != m_lstRemainingID.end();)
	{
		nSingerIndex++;

		MakeScore(m_mapSinger[*it]);

		//��¼��ǰ�ݳ�С����ֵĵ÷��������������������
		m_mltmapCurGroup.insert(std::pair<double,int>(m_mapSinger[*it].dLatestScore,*it));

		if (0==nSingerIndex%6)
		{
			//С���ݳ���ϣ���ӡ�÷����
			PrintGroupScore();

			//�ڵ�ǰС����ɾ������
			EraseInCurGroup();
			
			//��ʣ�������ɾ������
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
	sort(deqScore.begin(),deqScore.end());//����
	
	deqScore.pop_front();   //ȥ����������
	deqScore.pop_back();

	double dScoreSum = accumulate(deqScore.begin(),deqScore.end(),0);  //���
	double dScoreAverage = (double)(dScoreSum/deqScore.size());        //���ƽ��ֵ

	Ssinger.dLatestScore = dScoreAverage;

	return true;
}

bool CSingingCompetition::PrintGroupScore(void)
{
	std::cout<<"С��÷������"<<std::endl;

	for (std::multimap<double,int,std::greater<double>>::iterator it = m_mltmapCurGroup.begin();it != m_mltmapCurGroup.end();it++)
	{
		std::cout<<m_mapSinger[it->second].strName.c_str()<<"�ĵ÷֣�"<<std::setiosflags(std::ios::fixed)<<std::setprecision(2)<<it->first<<std::endl;
	}
	std::cout<<std::endl;

	return true;
}

bool CSingingCompetition::EraseInCurGroup(void)
{
	int nSingerLastIndexInGroup = 0;   //���ڸ��ֵĵ�������

	while (nSingerLastIndexInGroup < 3)
	{
		std::multimap<double,int,std::greater<double>>::iterator it = m_mltmapCurGroup.end();
		it--;

		nSingerLastIndexInGroup++;

		if (m_nRound == 1)
		{
			//��¼��һ����̭���б���̭�ĸ��ֵĲ�����
			m_vecIDBeEliminatedInFirstRound.push_back(it->second);
		}
		else if (m_nRound == 2)
		{
			//��¼�ڶ�����̭���б���̭�ĸ��ֵķ���
			m_mltsetScoreBeEliminatedInSecondRound.insert(m_mapSinger[it->second].dLatestScore);
		}

		//�ӵ�ǰ�ݳ�С��ļ���������ɾ�����һ��Ԫ��
		m_mltmapCurGroup.erase(it);
	}

	return true;
}

bool CSingingCompetition::EraseInRemainingID(std::list<int>::iterator it)
{
	int nSingerReverseIndexInGroup = 0;   //�����������

	while (nSingerReverseIndexInGroup < 6)
	{
		//�������������������ָ�Ĳ���ID����Ӧ���ֵ�{����������ID}�Ƿ��ڵ�ǰ�ݳ�С����
		std::multimap<double,int,std::greater<double>>::iterator itMltMapScoreToID;
		itMltMapScoreToID = find(m_mltmapCurGroup.begin(),m_mltmapCurGroup.end(),
			std::multimap<double,int,std::greater<double>>::value_type(m_mapSinger[*it].dLatestScore,*it));
	
		if (itMltMapScoreToID == m_mltmapCurGroup.end())
		{
			//û�ҵ�����ʣ����ּ�����ɾ���ø���
			it = m_lstRemainingID.erase(it);
		}

		//��������
		nSingerReverseIndexInGroup++;

		//����������,��ֹ��begin���������д���
		if (it != m_lstRemainingID.begin())
		{
			it--;
		}
	}

	//�������ı�����¼�洢���Ա���һ�������¼�Ĵ洢
	m_mltmapCurGroup.clear();

	return true;
}

