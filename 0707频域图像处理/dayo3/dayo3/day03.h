#ifndef DAY03_H_
#define DAY03_H_

#include <iostream>
#include<opencv2/opencv.hpp>
#include<string>

namespace IPSG
{
	class CRectangle
	{
	public:
		CRectangle(float flength = 0.0,float fwidth = 0.0,
			bool bPublicTestFlag = false, bool bProtectTestFlag = false, 
			bool bPrivateTestFlag = false) :m_fwidth(fwidth), m_flength(flength)
			,m_bPublicTestFlag(bPublicTestFlag),m_bProtectTestFlag(bProtectTestFlag),
			m_bPrivateTestFlag(bPrivateTestFlag){};
		~CRectangle(){};


		bool CalArea(float &fArea);
		bool CalRadi(float &fRadi);
		


	protected:

		bool IsSquare();
		bool m_bPublicTestFlag;
		bool m_bProtectTestFlag;
		bool friend printLength(CRectangle Rectangle);

	private:

		float m_fwidth;
		float m_flength;
		bool m_bPrivateTestFlag;
	};
	

	
}



#endif // !DAY03_H_
