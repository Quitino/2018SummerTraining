#include "Tool.h"

bool IPSG::GetCurrentSystemTime(double &timestamp)
{
	FILE_LOG(logDEBUG) << "GetCurrentSystemTime() call";
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);
	timestamp = (double)nowtime.wSecond + (double)(nowtime.wMilliseconds) / 1000.f; //精确到毫秒，具体到秒
	FILE_LOG(logDEBUG) << "GetCurrentSystemTime() return";

	return true;
}

bool IPSG::GetCurrentLocalTime(std::string &strLocalTime, const std::string strFormat)
{
	FILE_LOG(logDEBUG) << "GetCurrentLocalTime() call";
	time_t Time;
	tm* Local;
	char buf[64] = { 0 };
	Time = time(NULL);
	Local = localtime(&Time);
	strftime(buf, sizeof(buf), strFormat.c_str(), Local);
	strLocalTime = buf;
	FILE_LOG(logDEBUG) << "GetCurrentLocalTime() return";

	return true;
}

bool IPSG::GetCurrentSystemTimeForNameFile(std::string &strCurrentTime, std::string strAppend, bool bAppend)
{
	FILE_LOG(logDEBUG) << "GetCurrentSystemTimeForNameFile() call";
	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);
	strCurrentTime = std::to_string(nowtime.wYear).append("_") +
		std::to_string(nowtime.wMonth).append("_") +
		std::to_string(nowtime.wDay).append("_") +
		std::to_string(nowtime.wHour).append("_") +
		std::to_string(nowtime.wMinute).append("_") +
		std::to_string(nowtime.wSecond);
	if (bAppend)
	{
		strCurrentTime.append(strAppend);
	}
	FILE_LOG(logDEBUG) << "GetCurrentSystemTimeForNameFile() return";

	return true;
}

DWORD IPSG::GetProcessIdFromName(const std::string strProcessName)
{
	PROCESSENTRY32 ProcessEntry;
	DWORD ProcessID = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &ProcessEntry))
		return 0;

	char pname[300];
	do
	{
		ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32Next(hSnapshot, &ProcessEntry) == FALSE)
			break;

		sprintf(pname, "%ws", ProcessEntry.szExeFile);
		if (strcmp(pname, strProcessName.c_str()) == 0)
		{
			ProcessID = ProcessEntry.th32ProcessID;
			break;
		}
	} while (1);

	CloseHandle(hSnapshot);
	return ProcessID;
}

bool IPSG::SaveImage(cv::Mat InputImage, std::string strSavePath)
{
	FILE_LOG(logDEBUG) << "CScohe::SaveImage call";

	SYSTEMTIME nowtime;
	GetLocalTime(&nowtime);
	cv::Mat SaveImage = InputImage.clone();
	std::string strCurrentTime = std::to_string(nowtime.wYear).append("_") +
		std::to_string(nowtime.wMonth).append("_") +
		std::to_string(nowtime.wDay).append("_") +
		std::to_string(nowtime.wHour).append("_") +
		std::to_string(nowtime.wMinute).append("_") +
		std::to_string(nowtime.wSecond);
	std::string saveName = strSavePath + strCurrentTime + ".jpg";

	cv::imwrite(saveName, SaveImage);

	SaveImage.release();
	FILE_LOG(logDEBUG) << "CScohe::SaveImage return";
	return true;
}