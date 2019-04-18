#ifndef IMAGE_PROCESS_SALIENT_TARGET_EXTRACT_H_
#define IMAGE_PROCESS_SALIENT_TARGET_EXTRACT_H_

#include "SLIC.h"
#include "ARS.h"

namespace IPSG
{
	typedef struct SaliencyRegion
	{
		cv::Mat		m_SaliencyRegion;
		cv::Rect	m_SaliencyRect;
		float		m_fAnglePerPixel;		//ÿһ�����ش���ĽǶ�ƫ�ƣ����Լ���Ŀ�������������������ˮƽ�������Ƕ�ƫ��
		float		m_fHoriAngle;			//Ư����ˮƽ���������̨�ļнǣ���Ӧ��̨��ˮƽ����
		float		m_fVertAngle;			//Ư���ﴹֱ���������̨�ļнǣ���Ӧ��̨�ĸ�������
	}SSaliencyRegion;

	enum SaliencyAlgorithm { ASLIC = 0, AARS };	//ASLIC��ʾ�����طָ�SLIC������AARS��ʾ�������Ʒ���
	class CSaliency
	{
	public:
		CSaliency(SaliencyAlgorithm SA = ASLIC) :m_SaliencyAlgorithm(SA), m_fCameraFramHoriAngle(73.1){};
		~CSaliency(){};

		bool ImageSaliency(const cv::Mat InputImage, std::vector<SSaliencyRegion> &vSaliencyRegion);

	private:
		SaliencyAlgorithm	m_SaliencyAlgorithm;	//���õ��㷨
		float				m_fCameraFramHoriAngle;	//�����ˮƽ�ӳ���
	};

	//��Ԫ����
	bool SaliencyTest();
}

#endif // !IMAGE_PROCESS_SALIENT_TARGET_EXTRACT_H_
