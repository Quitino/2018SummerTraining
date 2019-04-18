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
		//��һ������txtNameΪ��������ͼ���ļ���·����
		//�ڶ��������Ǳ���궨������ļ�����Ĭ��ֵ "caliberation_result.txt"
		~CCalibrate();				/*��������*/
		void program_text();		/*���Ժ���*/ //�ú������ڱ������ڿ���̨�������Ա�۲�
		void corner_extraction();	/*�ǵ���ȡ*/
		void camera_calibration();	/*����궨*/
		void image_correction();	/*ͼ�����*/
		void Calibrate();			/*����ִ������궨*/

	private:
		ifstream fin;				/* �궨����ͼ���ļ���·�� */
		ofstream fout;				/* ����궨������ļ� */
		int image_count = 0;		/* ͼ������ */
		Size image_size;			/* ͼ��ĳߴ� */
		Size board_size = Size(6, 9);				/* �궨����ÿ�С��еĽǵ��� */
		vector<Point2f> image_points_buf;			/* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
		vector<vector<Point2f>> image_points_seq;	/* �����⵽�����нǵ� */
		string filename;
		/*����������*/
		Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* ������ڲ������� */
		Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));	/* �������5������ϵ����k1,k2,p1,p2,k3 */

		bool do_corner_extraction = false;		/*�Ƿ���нǵ���ı�ʶ��*/
		bool do_camera_calibration = false;		/*�Ƿ��������궨��ʶ��*/
		bool do_image_correction = false;		/*�Ƿ����ͼ�������ʶ��*/
	};

}


#endif // !CALIBRATE_H_
