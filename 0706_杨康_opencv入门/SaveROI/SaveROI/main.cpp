//#include "SaveROI.h"
#include <iostream>
#include <opencv2/core/core.hpp>  
//#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/opencv.hpp>
/*
// void main()
{	
	//cv::Mat org, dst, img, tmp;
	process();
	cv::waitKey(0);
}

*/
using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4, g_dstImage5; //int g_nBoxFilterValue = 6;  //方框滤波内核值
//int g_nMeanBlurValue = 10;  //均值滤波内核值
//int g_nGaussianBlurValue = 6;  //高斯滤波内核值
//int g_nMedianBlurValue = 10;  //中值滤波参数值
//int g_nBilateralFilterValue = 10;  //双边滤波参数值
//
//
//static void on_BoxFilter(int, void *);		//方框滤波
//static void on_MeanBlur(int, void *);		//均值块滤波器
//static void on_GaussianBlur(int, void *);			//高斯滤波器
//static void on_MedianBlur(int, void *);			//中值滤波器
//static void on_BilateralFilter(int, void *);			//双边滤波器
//
//static void on_MaxFilter(int, void *);			//最大值滤波器
////
////
////
////
////
////
////
////
////
////
////
////
////
//////-----------------------------【on_BoxFilter( )函数】------------------------------------
//////		描述：方框滤波操作的回调函数
////-----------------------------------------------------------------------------------------------
//static void on_BoxFilter(int, void *)
//{
//	//方框滤波操作
//	boxFilter(g_srcImage, g_dstImage1, -1, Size(g_nBoxFilterValue + 1, g_nBoxFilterValue + 1));
//	//显示窗口
//	imshow("【<1>方框滤波】", g_dstImage1);
//}
//
////-----------------------------【on_MeanBlur( )函数】------------------------------------
////		描述：均值滤波操作的回调函数
////-----------------------------------------------------------------------------------------------
//static void on_MeanBlur(int, void *)
//{
//	blur(g_srcImage, g_dstImage2, Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1), Point(-1, -1));
//	imshow("【<2>均值滤波】", g_dstImage2);
//
//}
//
////-----------------------------【on_GaussianBlur( )函数】------------------------------------
////		描述：高斯滤波操作的回调函数
////-----------------------------------------------------------------------------------------------
//static void on_GaussianBlur(int, void *)
//{
//	GaussianBlur(g_srcImage, g_dstImage3, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);
//	imshow("【<3>高斯滤波】", g_dstImage3);
//}
//
//
////-----------------------------【on_MedianBlur( )函数】------------------------------------
////		描述：中值滤波操作的回调函数
////-----------------------------------------------------------------------------------------------
//static void on_MedianBlur(int, void *)
//{
//	medianBlur(g_srcImage, g_dstImage4, g_nMedianBlurValue * 2 + 1);
//	imshow("【<4>中值滤波】", g_dstImage4);
//}
//
//
////-----------------------------【on_BilateralFilter( )函数】------------------------------------
////		描述：双边滤波操作的回调函数
////-----------------------------------------------------------------------------------------------
//static void on_BilateralFilter(int, void *)
//{
//	bilateralFilter(g_srcImage, g_dstImage5, g_nBilateralFilterValue, g_nBilateralFilterValue * 2, g_nBilateralFilterValue / 2);
//	imshow("【<5>双边滤波】", g_dstImage5);
//}
//
//
//
//
////-----------------------------【on_MaxFilter( )函数】------------------------------------
////		描述：双边滤波操作的回调函数
////-----------------------------------------------------------------------------------------------
//static void on_MaxFilter(Mat & g_dstImage6)
//{
//	int rowNumber = g_dstImage6.rows;
//	int colNumber = g_dstImage6.cols;
//	Mat dst = Mat::zeros(rowNumber, colNumber, CV_8U);
//	//最大值滤波操作
//	for (int i = 1; i < rowNumber - 1; i++)
//	{
//		for (int j = 1; j < colNumber - 1; j++)
//		{
//			int maxt = 0; //如果是最小值滤波则初始化maxt=255;
//			for (int k = i - 1; k <= i + 1; k++)
//				for (int m = j - 1; m <= j + 1; m++)
//					if (maxt < g_dstImage6.at<uchar>(k, m))
//						//如果是最小值滤波，小于号改成大于号就可
//						maxt = g_dstImage6.at<uchar>(k, m);
//			dst.at<uchar>(i, j) = maxt;
//		}
//	}
//	imshow("【<6>最大值滤波】", dst);
//}
//








int main()
{
//	cv::Mat org;
//	org = imread("12.jpg");
//	namedWindow("img");//定义一个img窗口 
//	imshow("src", org);

//=========================================================

	//ShowHelpText();

	// 载入原图
	g_srcImage = imread("C://Users//Administrator//Desktop//12.jpg", 1);
	cvtColor(g_srcImage, g_dstImage1 ,COLOR_BGR2GRAY);
	int rowNumber = g_dstImage1.rows;
	int colNumber = g_dstImage1.cols;
	//if (!g_srcImage.data) { printf("读取srcImage错误~！ \n"); return false; }
	for (int i = 1; i < rowNumber - 1; i++)
	{
		for (int j = 1; j < colNumber - 1; j++)
		{

			if (g_dstImage1.at<char>(i, j) >= 70)    
				g_dstImage1.at<char>(i, j) = 255;
			else  g_dstImage1.at<char>(i, j) = 0;
		}
	}
	imshow("二值化", g_dstImage1);

	//克隆原图到四个Mat类型中
	//g_dstImage1 = g_srcImage.clone();
	//g_dstImage2 = g_srcImage.clone();
	////g_dstImage3 = g_srcImage.clone();
	////g_dstImage4 = g_srcImage.clone();
	////g_dstImage5 = g_srcImage.clone();
	//////===========================================

	////显示原图
	//namedWindow("【<0>原图窗口】", 1);
	//imshow("【<0>原图窗口】", g_srcImage);


	////=================【<1>方框滤波】=========================
	//namedWindow("【<1>方框滤波】", 1);
	//on_MeanBlur(g_nBoxFilterValue, 0);
	//imshow("【<1>方框滤波】", g_dstImage1);
	////=====================================================


	////=================【<2>均值滤波】==========================
	//namedWindow("【<2>均值滤波】", 1);
	//on_MeanBlur(g_nMeanBlurValue, 0);
	//imshow("【<2>均值滤波】", g_dstImage2);
	////======================================================


	////=================【<3>高斯滤波】===========================
	//namedWindow("【<3>高斯滤波】", 1);
	//on_GaussianBlur(g_nGaussianBlurValue, 0);
	//imshow("【<3>高斯滤波】", g_dstImage3);
	////=======================================================


	////=================【<4>中值滤波】===========================
	//namedWindow("【<4>中值滤波】", 1);
	//on_MedianBlur(g_nMedianBlurValue, 0);
	//imshow("【<4>中值滤波】", g_dstImage4);
	////=======================================================


	////=================【<5>双边滤波】===========================
	//namedWindow("【<5>双边滤波】", 1);
	//on_BilateralFilter(g_nBilateralFilterValue, 0);
	//imshow("【<5>双边滤波】", g_dstImage5);
	////=======================================================

	////=================【<6>最大值滤波】===========================
	//namedWindow("【<6>最大值滤波】", 1);
	//on_BilateralFilter(g_nBilateralFilterValue, 0);
	//imshow("【<5>双边滤波】", g_dstImage5);

	cv::waitKey();
	return 0;

}

