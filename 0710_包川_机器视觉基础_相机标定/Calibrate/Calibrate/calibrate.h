#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#include <iostream>  
#include <fstream>  
#include <string.h>

#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
namespace IPSG
{
	class CCalibrate
	{
	public:

		CCalibrate(string txtName, string preserveName = "caliberation_result.txt");
		//第一个参数txtName为输入所用图像文件的路径；
		//第二个参数是保存标定结果的文件，有默认值 "caliberation_result.txt"
		~CCalibrate();				/*析构函数*/
		void program_text();		/*测试函数*/ //该函数用于编译中在控制台输出结果以便观察
		void corner_extraction();	/*角点提取*/
		void camera_calibration();	/*相机标定*/
		void image_correction();	/*图像矫正*/
		void Calibrate();			/*完整执行相机标定*/

	private:
		ifstream fin;				/* 标定所用图像文件的路径 */
		ofstream fout;				/* 保存标定结果的文件 */
		int image_count = 0;		/* 图像数量 */
		Size image_size;			/* 图像的尺寸 */
		Size board_size = Size(6, 9);				/* 标定板上每行、列的角点数 */
		vector<Point2f> image_points_buf;			/* 缓存每幅图像上检测到的角点 */
		vector<vector<Point2f>> image_points_seq;	/* 保存检测到的所有角点 */
		string filename;
		/*相机内外参数*/
		Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 摄像机内参数矩阵 */
		Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));	/* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */

		bool do_corner_extraction = false;		/*是否进行角点检测的标识符*/
		bool do_camera_calibration = false;		/*是否进行相机标定标识符*/
		bool do_image_correction = false;		/*是否进行图像矫正标识符*/
	};

}


#endif // !CALIBRATE_H_
