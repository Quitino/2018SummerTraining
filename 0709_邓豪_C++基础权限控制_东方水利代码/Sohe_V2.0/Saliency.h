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
		float		m_fAnglePerPixel;		//每一个像素代表的角度偏移，用以计算目标区域相对于相机光轴的水平及俯仰角度偏移
		float		m_fHoriAngle;			//漂浮物水平方向相对云台的夹角，对应云台的水平方向
		float		m_fVertAngle;			//漂浮物垂直方向相对云台的夹角，对应云台的俯仰方向
	}SSaliencyRegion;

	enum SaliencyAlgorithm { ASLIC = 0, AARS };	//ASLIC表示超像素分割SLIC方法，AARS表示相邻相似方法
	class CSaliency
	{
	public:
		CSaliency(SaliencyAlgorithm SA = ASLIC) :m_SaliencyAlgorithm(SA), m_fCameraFramHoriAngle(73.1){};
		~CSaliency(){};

		bool ImageSaliency(const cv::Mat InputImage, std::vector<SSaliencyRegion> &vSaliencyRegion);

	private:
		SaliencyAlgorithm	m_SaliencyAlgorithm;	//采用的算法
		float				m_fCameraFramHoriAngle;	//相机的水平视场角
	};

	//单元测试
	bool SaliencyTest();
}

#endif // !IMAGE_PROCESS_SALIENT_TARGET_EXTRACT_H_
