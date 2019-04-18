#include "day03.h"

bool IPSG::CRectangle::CalArea(float &fArea)
{
	fArea = m_fwidth*m_flength;
	return  fArea;
}


bool IPSG::CRectangle::CalRadi(float &fRadi)
{
	fRadi = 2 * (m_flength + m_fwidth);
	return fRadi;

}


bool IPSG::CRectangle::IsSquare()
{
	if (m_fwidth == m_flength)
		return true;
	else
		return false;
}


bool IPSG::printLength(CRectangle Rectangle)  //实现部分不需要friend关键字
{
	std::cout<< Rectangle.m_flength<<std::endl;
	return true;

}
