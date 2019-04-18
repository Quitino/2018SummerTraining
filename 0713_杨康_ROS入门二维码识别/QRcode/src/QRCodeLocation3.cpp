//�ο����ӣ�http://blog.csdn.net/nick123chao/article/details/77573675
//Ч������

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


using namespace cv;
using namespace std;



Mat src; Mat src_gray;


RNG rng(12345);
//Scalar colorful = CV_RGB(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));

//��ȡ���������ĵ�
Point Center_cal2(vector<vector<Point> > contours, int i)
{
	int centerx = 0, centery = 0, n = contours[i].size();
	//����ȡ��С�����εı߽���ÿ���ܳ���������ȡһ��������꣬
	//������ȡ�ĸ����ƽ�����꣨��ΪС�����εĴ������ģ�
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}


bool QRCodeTest3()
{
	src = imread("/home/fb/QRcode_/src/test.png", 1);
	Mat src_all = src.clone();


	//��ɫͼת�Ҷ�ͼ
	cvtColor(src, src_gray, CV_BGR2GRAY);
	//��ͼ�����ƽ������
	blur(src_gray, src_gray, Size(3, 3));
	//ʹ�Ҷ�ͼ��ֱ��ͼ���⻯
	equalizeHist(src_gray, src_gray);
	namedWindow("src_gray");
	imshow("src_gray", src_gray);

	Scalar color = Scalar(1, 1, 255);
	Mat threshold_output;
	vector<vector<Point> > contours, contours2;
	vector<Vec4i> hierarchy;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	Mat drawing2 = Mat::zeros(src.size(), CV_8UC3);
	Mat drawingAllContours = Mat::zeros(src.size(), CV_8UC3);

	//ָ��112��ֵ���ж�ֵ��
	threshold(src_gray, threshold_output, 15, 255, THRESH_BINARY);

	namedWindow("Threshold_output");
	imshow("Threshold_output", threshold_output);


	/*��������
	*  ����˵��
	����ͼ��image����Ϊһ��2ֵ��ͨ��ͼ��
	contours����Ϊ�����������飬ÿһ��������һ��point���͵�vector��ʾ
	hiararchy����������������ͬ��ÿ������contours[ i ]��Ӧ4��hierarchyԪ��hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]��
	�ֱ��ʾ��һ��������ǰһ������������������Ƕ������������ţ����û�ж�Ӧ���ֵ����Ϊ������
	mode��ʾ�����ļ���ģʽ
	CV_RETR_EXTERNAL ��ʾֻ���������
	CV_RETR_LIST ���������������ȼ���ϵ
	CV_RETR_CCOMP ���������ȼ��������������һ��Ϊ��߽磬�����һ��Ϊ�ڿ׵ı߽���Ϣ������ڿ��ڻ���һ����ͨ���壬�������ı߽�Ҳ�ڶ��㡣
	CV_RETR_TREE ����һ���ȼ����ṹ������������ο�contours.c���demo
	methodΪ�����Ľ��ư취
	CV_CHAIN_APPROX_NONE �洢���е������㣬���ڵ������������λ�ò����1����max��abs��x1-x2����abs��y2-y1����==1
	CV_CHAIN_APPROX_SIMPLE ѹ��ˮƽ���򣬴�ֱ���򣬶Խ��߷����Ԫ�أ�ֻ�����÷�����յ����꣬����һ����������ֻ��4����������������Ϣ
	CV_CHAIN_APPROX_TC89_L1��CV_CHAIN_APPROX_TC89_KCOS ʹ��teh-Chinl chain �����㷨
	offset��ʾ�����������ƫ��������������Ϊ����ֵ����ROIͼ�����ҳ�����������Ҫ������ͼ���н��з���ʱ������������Ǻ����õġ�
	*/
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	int c = 0, ic = 0, k = 0, area = 0;

	//ͨ����ɫ��λ����Ϊ�����������������������ص㣬ɸѡ��������λ��
	int parentIdx = -1;
	for (int i = 0; i< contours.size(); i++)
	{
		//������������ͼ
		drawContours(drawingAllContours, contours, parentIdx, CV_RGB(255, 255, 255), 1, 8);
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}

		//������������
		if (ic >= 2)
		{
			//�����ҵ���������ɫ��λ��
			contours2.push_back(contours[parentIdx]);
			//����������ɫ��λ�ǵ�����
			drawContours(drawing, contours, parentIdx, CV_RGB(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8);
			ic = 0;
			parentIdx = -1;
		}
	}

	//���ķ�ʽ����������ɫ��λ�ǵ�����
	for (int i = 0; i<contours2.size(); i++)
		drawContours(drawing2, contours2, i, CV_RGB(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255)), -1, 4, hierarchy[k][2], 0, Point());

	//��ȡ������λ�ǵ���������
	Point point[3];
	for (int i = 0; i<contours2.size(); i++)
	{
		point[i] = Center_cal2(contours2, i);
	}

	//������������������㶨λ�ǵ�������Ӷ�������߳�
	area = contourArea(contours2[1]);
	int area_side = cvRound(sqrt(double(area)));
	for (int i = 0; i<contours2.size(); i++)
	{
		//����������λ�ǵ���������
		line(drawing2, point[i%contours2.size()], point[(i + 1) % contours2.size()], color, area_side / 2, 8);
	}

	namedWindow("DrawingAllContours");
	imshow("DrawingAllContours", drawingAllContours);

	namedWindow("Drawing2");
	imshow("Drawing2", drawing2);

	namedWindow("Drawing");
	imshow("Drawing", drawing);


	//������Ҫ�����������ά��
	Mat gray_all, threshold_output_all;
	vector<vector<Point> > contours_all;
	vector<Vec4i> hierarchy_all;
	cvtColor(drawing2, gray_all, COLOR_BGR2GRAY);



	threshold(gray_all, threshold_output_all, 45, 255, THRESH_BINARY);
	findContours(threshold_output_all, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));//RETR_EXTERNAL��ʾֻѰ�����������


	Point2f fourPoint2f[4];
	//����С��Χ����
	RotatedRect rectPoint = minAreaRect(contours_all[0]);

	//��rectPoint�����д洢������ֵ�ŵ� fourPoint��������
	rectPoint.points(fourPoint2f);

	float maxdisparty_y=0.0,maxdisparty_x=0.0;
	for (int i=0;i < 3;i++)
	{
		float disparty_y= abs(fourPoint2f[i+1].y-fourPoint2f[i].y);
		float disparty_x= abs(fourPoint2f[i+1].x-fourPoint2f[i].x);

		if(disparty_y>maxdisparty_y)
		{
			maxdisparty_y=disparty_y;
		}

		if(disparty_x>maxdisparty_x)
		{
			maxdisparty_x=disparty_x;
		}
	}

	//���ǲ���
	float f_distance=0.0;
	float f_CameraPix=0.00375,f_focus=5.0,f_Height=118.0;

	f_distance=f_Height*f_focus/(maxdisparty_y*f_CameraPix);
	cout<<"really distance="<<f_distance<<".mm"<<endl;

	for (int i = 0; i < 4; i++ )
	{
		line(src_all, fourPoint2f[i % 4], fourPoint2f[(i + 1) % 4]
			, Scalar(20, 21, 237), 3);
	}

	namedWindow("Src_all");
	imshow("Src_all", src_all);

	//�����ά��󣬾Ϳ�����ȡ����ά�룬Ȼ��ʹ�ý����zxing����������Ϣ��
	//�����о���ά����Ų������Լ�д���벿��

	waitKey(0);
	return true;
}

int main()
{
	QRCodeTest3();

	return 0;
}
