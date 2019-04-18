#include <opencv2/opencv.hpp>
#include "calibrate.h"

using namespace std;
using namespace cv;

/*************************************************
// Method: CCalibrate
// Description: 构造函数
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter: txtName, preserveName
// History:
*************************************************/
IPSG::CCalibrate::CCalibrate(string txtName, string preserveName)
{
	fin.open(txtName);
	fout.open(preserveName);
}

/*************************************************
// Method: corner_extraction
// Description: 进行角点提取
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::corner_extraction()
{
	cout << "开始提取角点………………" << endl;
	/*判断是否调用函数*/
	if (do_corner_extraction == true)
	{
		cout << "角点结果已保存..." << endl;
		return;
	}
	fin.clear();						/*让文件指针回到开头，当前句为清空指针内容*/
	fin.seekg(0, ios::beg);				/*将指针重置到开头*/
//	int count = -1;						//用于存储角点个数。  
	while (getline(fin, filename))
	{
		image_count++;										// 用于观察检验输出  
		cout << "image_count = " << image_count << endl;
															/* 输出检验*/
//		cout << "-->count = " << count << endl;
		Mat imageInput = imread(filename);
		if (image_count == 1)			//读入第一张图片时获取图像宽高信息  
		{
			image_size.width = imageInput.cols;
			image_size.height = imageInput.rows;
			cout << endl;
			cout << "image_size.width = " << image_size.width << endl;
			cout << "image_size.height = " << image_size.height << endl;
		}
		/* 提取角点 */
		if (0 == findChessboardCorners(imageInput, board_size, image_points_buf))
		{
			cout << "can not find chessboard corners!\n"; //找不到角点  
			exit(1);
		}
		else
		{
			Mat view_gray;
			cvtColor(imageInput, view_gray, CV_RGB2GRAY);
			/* 亚像素精确化 */
			find4QuadCornerSubpix(view_gray, image_points_buf, Size(11, 11));	//对粗提取的角点进行精确化  
			image_points_seq.push_back(image_points_buf);						//保存亚像素角点  
			/* 在图像上显示角点位置 */
			drawChessboardCorners(imageInput, board_size, image_points_buf, true); //用于在图片中标记角点  
			imshow("Camera Calibration", imageInput);	//显示图片  
			waitKey(500);								//暂停0.5S         
		}
	}
	int total = image_points_seq.size();
	cout << "======================================================" << endl;
	cout << endl << "Image_total = " << total << endl;
	int CornerNum = board_size.width*board_size.height; //每张图片上总的角点数  
	cout << "CornerNum = " << board_size.width << " * " << board_size.height<<" = "<<CornerNum << endl;
	cout << "======================================================"<<endl;
	/*
	//此for循环仅用于控制台观察输出结果，对程序功能没有影响
	*/
	for (int ii = 0; ii < total; ii++)
	{
		if (0 == ii%CornerNum)// 24 是每幅图片的角点个数。此判断语句是为了输出 图片号，便于控制台观看   
		{
			int i = -1;
			i = ii / CornerNum;
			int j = i + 1;
			cout << "=> 第 " << j << "图片的数据 => : " << endl;
		}
		if (0 == ii % 3)  // 此判断语句，格式化输出，便于控制台查看  
		{
			cout << endl;
		}
		else
		{
			cout.width(5);
		}
		//输出所有的角点  
		cout << " =x=>" << image_points_seq[ii][0].x;
		cout << " =y=>" << image_points_seq[ii][0].y;
		cout << ii << endl;
	}
	cout << "=====================角点提取完成！===============================" << endl;
	cout << endl;
	do_corner_extraction = true;//标识符取正，标识进行了角点检测
}

/*************************************************
// Method: camera_calibration
// Description: 进行相机标定
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::camera_calibration()
{
	/*对是否对前一步函数进行调用的判断*/
	if (do_corner_extraction == false)
	{
		cout << "未检测到角点提取结果，重新进行角点提取..." << endl;
		cout << "..." << endl;
		CCalibrate::corner_extraction();
	}
	cout << "=====================开始标定===============================" << endl;
	if (do_camera_calibration == true)
	{
		cout << "=====================标定结果已保存==========================" << endl;
		return;
	}
	/*棋盘三维信息*///===================
	Size square_size = Size(30, 30);  /* 实际测量得到的标定板上每个棋盘格的大小,标定板棋盘格间距：30mm */
	vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
	/*内外参数*/

	vector<int> point_counts;  // 每幅图像中角点的数量  

	vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */
	vector<Mat> rvecsMat; /* 每幅图像的平移向量 */
	/* 初始化标定板上角点的三维坐标 */
	int i, j, t;
	for (t = 0; t < image_count; t++)
	{
		vector<Point3f> tempPointSet;
		for (i = 0; i < board_size.height; i++)
		{
			for (j = 0; j < board_size.width; j++)
			{
				Point3f realPoint;
				/* 假设标定板放在世界坐标系中z=0的平面上 */
				realPoint.x = i*square_size.width;
				realPoint.y = j*square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	/* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
	for (i = 0; i < image_count; i++)
	{
		point_counts.push_back(board_size.width*board_size.height);
	}
	/* 开始标定 */
	calibrateCamera(object_points, image_points_seq, image_size, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	cout << "=====================标定完成===============================" << endl;
	//对标定结果进行评价  
	cout << "=====================开始评价标定结果=======================" << endl;
	double total_err = 0.0;					/* 所有图像的平均误差的总和 */
	double err = 0.0;						/* 每幅图像的平均误差 */
	vector<Point2f> image_points2;			/* 保存重新计算得到的投影点 */
	cout << "\t每幅图像的标定误差：\n";
	fout << "每幅图像的标定误差：\n";
	for (i = 0; i < image_count; i++)
	{
		vector<Point3f> tempPointSet = object_points[i];
		/* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
		projectPoints(tempPointSet, rvecsMat[i], tvecsMat[i], cameraMatrix, distCoeffs, image_points2);
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = image_points_seq[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err += err /= point_counts[i];//err除以point_counts[i]然后赋值给err，赋值后的err于total_err相加赋值给total_err
		std::cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
		fout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
	}
	std::cout << "总体平均误差：" << total_err / image_count << "像素" << endl;
	fout << "总体平均误差：" << total_err / image_count << "像素" << endl << endl;
	std::cout << "评价完成！" << endl;
	//保存定标结果  
	cout << "=====================开始保存定标结果=======================" << endl;
	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
	fout << "相机内参数矩阵：" << endl;
	fout << cameraMatrix << endl << endl;
	fout << "畸变系数：\n";
	fout << distCoeffs << endl << endl << endl;
	for (int i = 0; i < image_count; i++)
	{
		fout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
		fout << tvecsMat[i] << endl;
		/* 将旋转向量转换为相对应的旋转矩阵 */
		Rodrigues(tvecsMat[i], rotation_matrix);
		fout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
		fout << rotation_matrix << endl;
		fout << "第" << i + 1 << "幅图像的平移向量：" << endl;
		fout << rvecsMat[i] << endl << endl;
	}
	cout << "=====================完成保存=======================" << endl;
	cout << endl;
	fout << endl;
	do_camera_calibration = true;//标识符取正，标识进行了相机标定
}

/*************************************************
// Method: image_correction
// Description:进行图像矫正
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::image_correction()
{
	/*对是否对函数进行调用的判断*/
	if (do_camera_calibration == false)
	{
		cout << "未检测到相机标定结果，重新进行相机标定..." << endl;
		cout << "..." << endl;
		CCalibrate::camera_calibration();
	}

	cout << "开始展示标定成果......" << endl;
	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);
//	std::cout << "保存矫正图像" << endl;
	string imageFileName;
	std::stringstream StrStm;/*以文本的形式输入到该变量中*/
	int cout_count = 0;
	fin.clear();/*让文件指针回到开头，当前句为清空指针内容*/
	fin.seekg(0, ios::beg);/*将指针重置到开头*/
	while (getline(fin, filename))
	{
		cout_count++;
//		std::cout << "Frame #" << cout_count << "..." << endl;
//		initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);/*计算畸变映射*/
		Mat imageSource = imread(filename);
		Mat newimage = imageSource.clone();
//		remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);/*把求得的映射应用到图像上*/
//		StrStm.clear();/*std::stringstream变量在使用（转化）前都应先调用.cleat()函数*/
//		string filePath;
//		StrStm << cout_count;
//		StrStm >> imageFileName;
//		imageFileName += "_d.jpg";
//		imwrite(imageFileName, newimage);
//		imshow("矫正之后", newimage);
		waitKey(500);
	}
	std::cout << "保存结束" << endl;
	cout << endl;
	do_image_correction = true;//标识符取正，标识进行了图像校正
}

/*************************************************
// Method: Calibrate
// Description:完整执行相机标定
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::Calibrate()
{
	corner_extraction();
	camera_calibration();
	image_correction();
}

/*************************************************
// Method: program_text
// Description:测试函数
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::program_text()
{
	string filename;
	int image_count = 0;
	while (getline(fin, filename))
	{
		image_count++;
		cout << "image_count = " << image_count << endl;
	}
	fout << "写入成功"<<endl;
	cout << "写入成功" << endl;
}

/*************************************************
// Method: ~CCalibrate
// Description:析构函数
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
IPSG::CCalibrate::~CCalibrate()
{
}
