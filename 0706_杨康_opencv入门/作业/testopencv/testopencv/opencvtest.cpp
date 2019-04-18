#include <opencv2/opencv.hpp>  //头文件
using namespace cv;  //包含cv命名空间

void main()
{
	// 【1】读入一张图片，载入图像
	Mat srcImage = imread("C://Users//Administrator//Desktop//Fourier.jpg");
	// 【2】显示载入的图片
	imshow("【原始图】", srcImage);
	// 【3】等待任意按键按下
	waitKey(0);
}  
