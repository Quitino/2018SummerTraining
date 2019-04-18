#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
const int kvalue = 15;//双边滤波邻域大小

int main()
{
	VideoCapture capture(0);
		
	while (1)
	{
		Mat src_color;					//定义一个Mat变量，用于存储每一帧的图像
		capture >> src_color;			//读取当前帧
		//Mat src_color = imread("C://Users//Administrator//Desktop//1.jpg");//读取原彩色图
		imshow("原图-彩色", src_color);

		//声明一个三通道图像，像素值全为0，用来将霍夫变换检测出的圆画在上面
		Mat dst(src_color.size(), src_color.type());
		dst = Scalar::all(0);

		Mat src_gray;					//彩色图像转化成灰度图
		cvtColor(src_color, src_gray, COLOR_BGR2GRAY);
		//imshow("原图-灰度", src_gray);
		imwrite("src_gray.jpg", src_gray);

		Mat bf;//对灰度图像进行双边滤波
		cvtColor(src_color, bf, CV_BGR2GRAY);//转化边缘检测后的图为灰度图
		GaussianBlur(bf, bf, Size(9, 9), 2, 2);
		//bilateralFilter(src_gray, bf, kvalue, kvalue * 2, kvalue / 2);
		//imshow("灰度双边滤波处理", bf);
		imwrite("src_bf.jpg", bf);

		vector<Vec3f> circles;//声明一个向量，保存检测出的圆的圆心坐标和半径
		HoughCircles(bf, circles,CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);//霍夫变换检测圆

		cout << "Count=\tx=\ty=\tr=" << endl;
		for (size_t i = 0; i < circles.size(); i++)//把霍夫变换检测出的圆画出来
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);

			circle(dst, center, 0, Scalar(0, 255, 0), -1, 8, 0);
			circle(dst, center, radius, Scalar(0, 0, 255), 1, 8, 0);

			cout << "count:"<<i<<"\n"<<cvRound(circles[i][0]) << "\t" << cvRound(circles[i][1]) << "\t"
				<< cvRound(circles[i][2]) << endl;//在控制台输出圆心坐标和半径	

		}

		imshow("特征提取", dst);
		//imwrite("dst.jpg", dst);

		waitKey(300);  //延时30ms

	}

	waitKey();
}