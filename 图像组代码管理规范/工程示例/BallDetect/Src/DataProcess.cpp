#include "DataProcess.h"

bool IPSG::PointsVector2String(const std::vector<cv::Point2f> vecPoints, std::string & strData)
{
	strData.clear();
	std::string strPrefix = "S"; //数据前缀，一般用于帧头前缀,不要加后缀，一般来说会多次对发送数据进行处理，在发送前加后缀
	strData += strPrefix;
	for (size_t i = 0; i < vecPoints.size(); i++)
	{
		int nXTemp = static_cast<int>(vecPoints.at(i).x);
		int nYTemp = static_cast<int>(vecPoints.at(i).y);

		if ( !IsThreeDigital(nXTemp))
		{
			if ( !IsTwoDigital(nXTemp))
			{
				strData += "00";
				strData += std::to_string(nXTemp);
			}
			else
			{
				strData += "0";
				strData += std::to_string(nXTemp);
			}
		}
		else strData += std::to_string(nXTemp);

		if (!IsThreeDigital(nYTemp))
		{
			if (!IsTwoDigital(nYTemp))
			{
				strData += "00";
				strData += std::to_string(nYTemp);
			}
			else
			{
				strData += "0";
				strData += std::to_string(nYTemp);
			}
		}
		else strData += std::to_string(nYTemp);
	}
	return true;
}

bool IPSG::Point2String(const cv::Point2f Points, std::string &strData)
{
	strData.clear();
	std::string strPrefix = "C";
	strData += strPrefix;

	int nXTemp = static_cast<int>(Points.x);
	int nYTemp = static_cast<int>(Points.y);
	if (!IsThreeDigital(nXTemp))
	{
		if (!IsTwoDigital(nXTemp))
		{
			strData += "00";
			strData += std::to_string(nXTemp);
		}
		else
		{
			strData += "0";
			strData += std::to_string(nXTemp);
		}
	}
	else strData += std::to_string(nXTemp);

	if (!IsThreeDigital(nYTemp))
	{
		if (!IsTwoDigital(nYTemp))
		{
			strData += "00";
			strData += std::to_string(nYTemp);
		}
		else
		{
			strData += "0";
			strData += std::to_string(nYTemp);
		}
	}
	else strData += std::to_string(nYTemp);
	return true;
}

bool IPSG::IsThreeDigital(const int nNumber)
{
	if (nNumber / 100) return true;
	else return false;
}

bool IPSG::IsTwoDigital(const int nNumber)
{
	if (nNumber / 10) return true;
	else return false;
}

bool IPSG::AddZero(const int nAddNO, std::string &strData)
{
	strData.clear();
	for (size_t i = 0; i < nAddNO; i++)
	{
		strData.append("0");
	}
	return true;
}