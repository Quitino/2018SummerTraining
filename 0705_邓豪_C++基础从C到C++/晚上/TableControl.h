#ifndef TABLE_CONTROL_H_
#define TABLE_CONTROL_H_

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include <opencv2/opencv.hpp>

#include "TableCommand.h"

namespace IPSG
{
	class CTableControl
	{
	public:
		CTableControl();
		~CTableControl();

		bool OpenAux(int nAuxNO);
		bool CloseAux(int nAuxNO);

		bool SetPreset(int nPresetNO);
		bool ClrPreset(int nPresetNO);
		bool CallPreset(int nPresetNO);

		bool HoriLoc(float fHoriLocPoint);
		bool VertLoc(float fVertLocPoint);
		bool HVLoc(float fHoriLocPoint, float fVertLocPoint);

		bool SetLocSpeed(float fHoriSpeed, float fVertSpeed);

		bool GetHoriLocPoint();
		bool GetVertLocPoint();
		bool GetLocSpeed();

	private:
		bool EnCode(BYTE ucCommand2, BYTE ucDataH, BYTE ucDataL, std::vector<BYTE> &vucOutData);
		bool DeCode();

		bool Int2UShort(int nNO, BYTE ucData2);
		bool CalCheckSum(std::vector<BYTE> vucData,BYTE &ucCheckSum);

		bool WriteFrame(char *cSendBuff, int nSendLength);
		bool WriteFrame(std::string strSendBuff);
	
	private:
		std::vector<BYTE>	m_vucReceivedFrame;

		float				m_fHoriLocPoint;
		float				m_fVertLocPoint;
		float				m_fLocSpeed;
	};
}

#endif // !TABLE_CONTROL_H_
