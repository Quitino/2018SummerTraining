#ifndef FLOATER_DETECTING_CLASSIFIER_H_
#define FLOATER_DETECTING_CLASSIFIER_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <opencv2/opencv.hpp>

#include "log.h"
//#include "HalconCpp.h"
//#include "HDevThread.h"
//
//using namespace HalconCpp;
//
//namespace IPSG
//{
//	class CClassifierMLP
//	{
//	public:
//		CClassifierMLP(){};
//		~CClassifierMLP(){};
//
//		bool RecongitionClassifier(cv::Mat InputImage);
//		bool MatToHImage(cv::Mat& InputImage, HObject& HSrcImage);
//		//bool RecongitionClassifier(cv::Mat InputImage);
//
//	private:
//		void disp_continue_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);
//		void disp_end_of_program_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);
//		void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem, HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
//		void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, HTuple hv_Slant);
//		void gen_features(HObject ho_Image, HTuple *hv_FeatureVector);
//		void gen_sobel_features(HObject ho_Image, HTuple hv_Features, HTuple *hv_FeaturesExtended);
//	};
//}

#endif // !FLOATER_DETECTING_CLASSIFIER_H_
