//#include"SaveROI.h"
// 
////ȫ�ֱ���
//static cv::Mat org, dst, img, tmp;
//
//
////event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ��� 
//void on_mouse(int event, int x, int y, int flags, void *ustc)
//{
//	static Point pre_pt = (-1, -1);//��ʼ����  
//	static Point cur_pt = (-1, -1);//ʵʱ����  
//	char temp[16];
//
//
//	//������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ
//	if (event == CV_EVENT_LBUTTONDOWN)  
//	{
//		org.copyTo(img);//��ԭʼͼƬ���Ƶ�img��  
//		sprintf_s(temp, "(%d,%d)", x, y);
//		pre_pt = Point(x, y);
//		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//�ڴ�������ʾ����  
//		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//��Բ  
//		imshow("img", img);
//	}
//
//
//	//���û�а��µ����������ƶ��Ĵ�����  
//	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))
//	{
//		img.copyTo(tmp);//��img���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
//		sprintf_s(temp, "(%d,%d)", x, y);
//		cur_pt = Point(x, y);
//		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//ֻ��ʵʱ��ʾ����ƶ�������  
//		imshow("img", tmp);
//	}
//
//	//�������ʱ������ƶ�������ͼ���ϻ�����  
//	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
//	{
//		img.copyTo(tmp);
//		sprintf_s(temp, "(%d,%d)", x, y);
//		cur_pt = Point(x, y);
//		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
//		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
//		imshow("img", tmp);
//	}
//
//	//����ɿ�������ͼ���ϻ�����
//	else if (event == CV_EVENT_LBUTTONUP)  
//	{
//		org.copyTo(img);
//		sprintf_s(temp, "(%d,%d)", x, y);
//		cur_pt = Point(x, y);
//		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
//		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
//		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//���ݳ�ʼ��ͽ����㣬�����λ���img��  
//		imshow("img", img);
//		img.copyTo(tmp);
//		//��ȡ���ΰ�Χ��ͼ�񣬲����浽dst��  
//		int width = abs(pre_pt.x - cur_pt.x);
//		int height = abs(pre_pt.y - cur_pt.y);
//		if (width == 0 || height == 0)
//		{
//			printf("width == 0 || height == 0");
//			return;
//		}
//		dst = org(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
//		namedWindow("HandsomeBoy");
//		imshow("HandsomeBoy", dst); 
//		imwrite("./HandsomeBoy.jpg", dst);
//		waitKey(0);
//	}
//}
//void  process()
//{
//	org = imread("IMG.jpg");
//	org.copyTo(img);
//	org.copyTo(tmp);
//	namedWindow("img");//����һ��img����  
//	setMouseCallback("img", on_mouse, 0);//���ûص�����  
//	imshow("img", img);
//}