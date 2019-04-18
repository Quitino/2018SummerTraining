#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
const int kvalue = 15;//˫���˲������С

int main()
{
	VideoCapture capture(0);
		
	while (1)
	{
		Mat src_color;					//����һ��Mat���������ڴ洢ÿһ֡��ͼ��
		capture >> src_color;			//��ȡ��ǰ֡
		//Mat src_color = imread("C://Users//Administrator//Desktop//1.jpg");//��ȡԭ��ɫͼ
		imshow("ԭͼ-��ɫ", src_color);

		//����һ����ͨ��ͼ������ֵȫΪ0������������任������Բ��������
		Mat dst(src_color.size(), src_color.type());
		dst = Scalar::all(0);

		Mat src_gray;					//��ɫͼ��ת���ɻҶ�ͼ
		cvtColor(src_color, src_gray, COLOR_BGR2GRAY);
		//imshow("ԭͼ-�Ҷ�", src_gray);
		imwrite("src_gray.jpg", src_gray);

		Mat bf;//�ԻҶ�ͼ�����˫���˲�
		cvtColor(src_color, bf, CV_BGR2GRAY);//ת����Ե�����ͼΪ�Ҷ�ͼ
		GaussianBlur(bf, bf, Size(9, 9), 2, 2);
		//bilateralFilter(src_gray, bf, kvalue, kvalue * 2, kvalue / 2);
		//imshow("�Ҷ�˫���˲�����", bf);
		imwrite("src_bf.jpg", bf);

		vector<Vec3f> circles;//����һ�����������������Բ��Բ������Ͱ뾶
		HoughCircles(bf, circles,CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);//����任���Բ

		cout << "Count=\tx=\ty=\tr=" << endl;
		for (size_t i = 0; i < circles.size(); i++)//�ѻ���任������Բ������
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);

			circle(dst, center, 0, Scalar(0, 255, 0), -1, 8, 0);
			circle(dst, center, radius, Scalar(0, 0, 255), 1, 8, 0);

			cout << "count:"<<i<<"\n"<<cvRound(circles[i][0]) << "\t" << cvRound(circles[i][1]) << "\t"
				<< cvRound(circles[i][2]) << endl;//�ڿ���̨���Բ������Ͱ뾶	

		}

		imshow("������ȡ", dst);
		//imwrite("dst.jpg", dst);

		waitKey(300);  //��ʱ30ms

	}

	waitKey();
}