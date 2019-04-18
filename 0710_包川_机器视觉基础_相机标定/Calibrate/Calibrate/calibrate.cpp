#include <opencv2/opencv.hpp>
#include "calibrate.h"

using namespace std;
using namespace cv;

/*************************************************
// Method: CCalibrate
// Description: ���캯��
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
// Description: ���нǵ���ȡ
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::corner_extraction()
{
	cout << "��ʼ��ȡ�ǵ㡭����������" << endl;
	/*�ж��Ƿ���ú���*/
	if (do_corner_extraction == true)
	{
		cout << "�ǵ����ѱ���..." << endl;
		return;
	}
	fin.clear();						/*���ļ�ָ��ص���ͷ����ǰ��Ϊ���ָ������*/
	fin.seekg(0, ios::beg);				/*��ָ�����õ���ͷ*/
//	int count = -1;						//���ڴ洢�ǵ������  
	while (getline(fin, filename))
	{
		image_count++;										// ���ڹ۲�������  
		cout << "image_count = " << image_count << endl;
															/* �������*/
//		cout << "-->count = " << count << endl;
		Mat imageInput = imread(filename);
		if (image_count == 1)			//�����һ��ͼƬʱ��ȡͼ������Ϣ  
		{
			image_size.width = imageInput.cols;
			image_size.height = imageInput.rows;
			cout << endl;
			cout << "image_size.width = " << image_size.width << endl;
			cout << "image_size.height = " << image_size.height << endl;
		}
		/* ��ȡ�ǵ� */
		if (0 == findChessboardCorners(imageInput, board_size, image_points_buf))
		{
			cout << "can not find chessboard corners!\n"; //�Ҳ����ǵ�  
			exit(1);
		}
		else
		{
			Mat view_gray;
			cvtColor(imageInput, view_gray, CV_RGB2GRAY);
			/* �����ؾ�ȷ�� */
			find4QuadCornerSubpix(view_gray, image_points_buf, Size(11, 11));	//�Դ���ȡ�Ľǵ���о�ȷ��  
			image_points_seq.push_back(image_points_buf);						//���������ؽǵ�  
			/* ��ͼ������ʾ�ǵ�λ�� */
			drawChessboardCorners(imageInput, board_size, image_points_buf, true); //������ͼƬ�б�ǽǵ�  
			imshow("Camera Calibration", imageInput);	//��ʾͼƬ  
			waitKey(500);								//��ͣ0.5S         
		}
	}
	int total = image_points_seq.size();
	cout << "======================================================" << endl;
	cout << endl << "Image_total = " << total << endl;
	int CornerNum = board_size.width*board_size.height; //ÿ��ͼƬ���ܵĽǵ���  
	cout << "CornerNum = " << board_size.width << " * " << board_size.height<<" = "<<CornerNum << endl;
	cout << "======================================================"<<endl;
	/*
	//��forѭ�������ڿ���̨�۲����������Գ�����û��Ӱ��
	*/
	for (int ii = 0; ii < total; ii++)
	{
		if (0 == ii%CornerNum)// 24 ��ÿ��ͼƬ�Ľǵ���������ж������Ϊ����� ͼƬ�ţ����ڿ���̨�ۿ�   
		{
			int i = -1;
			i = ii / CornerNum;
			int j = i + 1;
			cout << "=> �� " << j << "ͼƬ������ => : " << endl;
		}
		if (0 == ii % 3)  // ���ж���䣬��ʽ����������ڿ���̨�鿴  
		{
			cout << endl;
		}
		else
		{
			cout.width(5);
		}
		//������еĽǵ�  
		cout << " =x=>" << image_points_seq[ii][0].x;
		cout << " =y=>" << image_points_seq[ii][0].y;
		cout << ii << endl;
	}
	cout << "=====================�ǵ���ȡ��ɣ�===============================" << endl;
	cout << endl;
	do_corner_extraction = true;//��ʶ��ȡ������ʶ�����˽ǵ���
}

/*************************************************
// Method: camera_calibration
// Description: ��������궨
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::camera_calibration()
{
	/*���Ƿ��ǰһ���������е��õ��ж�*/
	if (do_corner_extraction == false)
	{
		cout << "δ��⵽�ǵ���ȡ��������½��нǵ���ȡ..." << endl;
		cout << "..." << endl;
		CCalibrate::corner_extraction();
	}
	cout << "=====================��ʼ�궨===============================" << endl;
	if (do_camera_calibration == true)
	{
		cout << "=====================�궨����ѱ���==========================" << endl;
		return;
	}
	/*������ά��Ϣ*///===================
	Size square_size = Size(30, 30);  /* ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С,�궨�����̸��ࣺ30mm */
	vector<vector<Point3f>> object_points; /* ����궨���Ͻǵ����ά���� */
	/*�������*/

	vector<int> point_counts;  // ÿ��ͼ���нǵ������  

	vector<Mat> tvecsMat;  /* ÿ��ͼ�����ת���� */
	vector<Mat> rvecsMat; /* ÿ��ͼ���ƽ������ */
	/* ��ʼ���궨���Ͻǵ����ά���� */
	int i, j, t;
	for (t = 0; t < image_count; t++)
	{
		vector<Point3f> tempPointSet;
		for (i = 0; i < board_size.height; i++)
		{
			for (j = 0; j < board_size.width; j++)
			{
				Point3f realPoint;
				/* ����궨�������������ϵ��z=0��ƽ���� */
				realPoint.x = i*square_size.width;
				realPoint.y = j*square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	/* ��ʼ��ÿ��ͼ���еĽǵ��������ٶ�ÿ��ͼ���ж����Կ��������ı궨�� */
	for (i = 0; i < image_count; i++)
	{
		point_counts.push_back(board_size.width*board_size.height);
	}
	/* ��ʼ�궨 */
	calibrateCamera(object_points, image_points_seq, image_size, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	cout << "=====================�궨���===============================" << endl;
	//�Ա궨�����������  
	cout << "=====================��ʼ���۱궨���=======================" << endl;
	double total_err = 0.0;					/* ����ͼ���ƽ�������ܺ� */
	double err = 0.0;						/* ÿ��ͼ���ƽ����� */
	vector<Point2f> image_points2;			/* �������¼���õ���ͶӰ�� */
	cout << "\tÿ��ͼ��ı궨��\n";
	fout << "ÿ��ͼ��ı궨��\n";
	for (i = 0; i < image_count; i++)
	{
		vector<Point3f> tempPointSet = object_points[i];
		/* ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ�� */
		projectPoints(tempPointSet, rvecsMat[i], tvecsMat[i], cameraMatrix, distCoeffs, image_points2);
		/* �����µ�ͶӰ��;ɵ�ͶӰ��֮������*/
		vector<Point2f> tempImagePoint = image_points_seq[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err += err /= point_counts[i];//err����point_counts[i]Ȼ��ֵ��err����ֵ���err��total_err��Ӹ�ֵ��total_err
		std::cout << "��" << i + 1 << "��ͼ���ƽ����" << err << "����" << endl;
		fout << "��" << i + 1 << "��ͼ���ƽ����" << err << "����" << endl;
	}
	std::cout << "����ƽ����" << total_err / image_count << "����" << endl;
	fout << "����ƽ����" << total_err / image_count << "����" << endl << endl;
	std::cout << "������ɣ�" << endl;
	//���涨����  
	cout << "=====================��ʼ���涨����=======================" << endl;
	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* ����ÿ��ͼ�����ת���� */
	fout << "����ڲ�������" << endl;
	fout << cameraMatrix << endl << endl;
	fout << "����ϵ����\n";
	fout << distCoeffs << endl << endl << endl;
	for (int i = 0; i < image_count; i++)
	{
		fout << "��" << i + 1 << "��ͼ�����ת������" << endl;
		fout << tvecsMat[i] << endl;
		/* ����ת����ת��Ϊ���Ӧ����ת���� */
		Rodrigues(tvecsMat[i], rotation_matrix);
		fout << "��" << i + 1 << "��ͼ�����ת����" << endl;
		fout << rotation_matrix << endl;
		fout << "��" << i + 1 << "��ͼ���ƽ��������" << endl;
		fout << rvecsMat[i] << endl << endl;
	}
	cout << "=====================��ɱ���=======================" << endl;
	cout << endl;
	fout << endl;
	do_camera_calibration = true;//��ʶ��ȡ������ʶ����������궨
}

/*************************************************
// Method: image_correction
// Description:����ͼ�����
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
void IPSG::CCalibrate::image_correction()
{
	/*���Ƿ�Ժ������е��õ��ж�*/
	if (do_camera_calibration == false)
	{
		cout << "δ��⵽����궨��������½�������궨..." << endl;
		cout << "..." << endl;
		CCalibrate::camera_calibration();
	}

	cout << "��ʼչʾ�궨�ɹ�......" << endl;
	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);
//	std::cout << "�������ͼ��" << endl;
	string imageFileName;
	std::stringstream StrStm;/*���ı�����ʽ���뵽�ñ�����*/
	int cout_count = 0;
	fin.clear();/*���ļ�ָ��ص���ͷ����ǰ��Ϊ���ָ������*/
	fin.seekg(0, ios::beg);/*��ָ�����õ���ͷ*/
	while (getline(fin, filename))
	{
		cout_count++;
//		std::cout << "Frame #" << cout_count << "..." << endl;
//		initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);/*�������ӳ��*/
		Mat imageSource = imread(filename);
		Mat newimage = imageSource.clone();
//		remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);/*����õ�ӳ��Ӧ�õ�ͼ����*/
//		StrStm.clear();/*std::stringstream������ʹ�ã�ת����ǰ��Ӧ�ȵ���.cleat()����*/
//		string filePath;
//		StrStm << cout_count;
//		StrStm >> imageFileName;
//		imageFileName += "_d.jpg";
//		imwrite(imageFileName, newimage);
//		imshow("����֮��", newimage);
		waitKey(500);
	}
	std::cout << "�������" << endl;
	cout << endl;
	do_image_correction = true;//��ʶ��ȡ������ʶ������ͼ��У��
}

/*************************************************
// Method: Calibrate
// Description:����ִ������궨
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
// Description:���Ժ���
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
	fout << "д��ɹ�"<<endl;
	cout << "д��ɹ�" << endl;
}

/*************************************************
// Method: ~CCalibrate
// Description:��������
// Author: Boonvon
// Date: 2018/07/10
// Returns: void
// Parameter:
// History:
*************************************************/
IPSG::CCalibrate::~CCalibrate()
{
}
