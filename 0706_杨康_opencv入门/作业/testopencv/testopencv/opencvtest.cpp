#include <opencv2/opencv.hpp>  //ͷ�ļ�
using namespace cv;  //����cv�����ռ�

void main()
{
	// ��1������һ��ͼƬ������ͼ��
	Mat srcImage = imread("C://Users//Administrator//Desktop//Fourier.jpg");
	// ��2����ʾ�����ͼƬ
	imshow("��ԭʼͼ��", srcImage);
	// ��3���ȴ����ⰴ������
	waitKey(0);
}  
