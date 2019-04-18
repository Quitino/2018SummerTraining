
#include <iostream>
#include <opencv.hpp>
#include <opencv2/core/core.hpp>


using namespace cv;

int main()
{
	//����Haar��LBP��������������
	CascadeClassifier faceDetector;
	std::string faceCascadeFilename = "haarcascade_frontalface_default.xml";

	//�Ѻô�����Ϣ��ʾ
	try{
		faceDetector.load(faceCascadeFilename);
	}
	catch (cv::Exception e){}
	if (faceDetector.empty())
	{
		std::cerr << "������������ܼ��� (";
		std::cerr << faceCascadeFilename << ")!" << std::endl;
		exit(1);
	}

	//������ͷ
	VideoCapture camera(0);
	while (true)
	{
		Mat camerFrame;
		camera >> camerFrame;
		if (camerFrame.empty())
		{
			std::cerr << "�޷���ȡ����ͷͼ��" << std::endl;
			getchar();
			exit(1);
		}
		Mat displayedFrame(camerFrame.size(), CV_8UC3);


		//�������ֻ�����ڻҶ�ͼ��
		Mat gray;
		cvtColor(camerFrame, gray, CV_BGRA2GRAY);
		//ֱ��ͼ���Ȼ�(����ͼ��ĶԱȶȺ�����)
		Mat equalizedImg;
		equalizeHist(gray, equalizedImg);

		//���������Cascade Classifier::detectMultiScale�������������
		//int flags = CASCADE_FIND_BIGGEST_OBJECT | CASCADE_DO_ROUGH_SEARCH;	//ֻ�����������
		int flags = CASCADE_SCALE_IMAGE;	//�������
		Size minFeatureSize(30, 30);
		float searchScaleFactor = 1.1f;
		int minNeighbors = 4;
		std::vector<Rect> faces;
		faceDetector.detectMultiScale(equalizedImg, faces, searchScaleFactor, minNeighbors, flags, minFeatureSize);

		//�����ο�
		cv::Mat face;
		cv::Point text_lb;
		for (size_t i = 0; i < faces.size(); i++)
		{
			if (faces[i].height > 0 && faces[i].width > 0)
			{
				face = gray(faces[i]);
				text_lb = cv::Point(faces[i].x, faces[i].y);
				cv::rectangle(equalizedImg, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);
				cv::rectangle(gray, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);
				cv::rectangle(camerFrame, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);
			}
		}


		imshow("ֱ��ͼ���Ȼ�", equalizedImg);
		imshow("�ҶȻ�", gray);
		imshow("ԭͼ", camerFrame);

		waitKey(20);
	}

	waitKey();
	return 0;
}