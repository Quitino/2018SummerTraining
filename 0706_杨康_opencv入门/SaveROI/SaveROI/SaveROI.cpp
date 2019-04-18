//#include"SaveROI.h"
// 
////全局变量
//static cv::Mat org, dst, img, tmp;
//
//
////event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号 
//void on_mouse(int event, int x, int y, int flags, void *ustc)
//{
//	static Point pre_pt = (-1, -1);//初始坐标  
//	static Point cur_pt = (-1, -1);//实时坐标  
//	char temp[16];
//
//
//	//左键按下，读取初始坐标，并在图像上该点处划圆
//	if (event == CV_EVENT_LBUTTONDOWN)  
//	{
//		org.copyTo(img);//将原始图片复制到img中  
//		sprintf_s(temp, "(%d,%d)", x, y);
//		pre_pt = Point(x, y);
//		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
//		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
//		imshow("img", img);
//	}
//
//
//	//左键没有按下的情况下鼠标移动的处理函数  
//	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))
//	{
//		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
//		sprintf_s(temp, "(%d,%d)", x, y);
//		cur_pt = Point(x, y);
//		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
//		imshow("img", tmp);
//	}
//
//	//左键按下时，鼠标移动，则在图像上划矩形  
//	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
//	{
//		img.copyTo(tmp);
//		sprintf_s(temp, "(%d,%d)", x, y);
//		cur_pt = Point(x, y);
//		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
//		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
//		imshow("img", tmp);
//	}
//
//	//左键松开，将在图像上划矩形
//	else if (event == CV_EVENT_LBUTTONUP)  
//	{
//		org.copyTo(img);
//		sprintf_s(temp, "(%d,%d)", x, y);
//		cur_pt = Point(x, y);
//		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
//		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
//		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
//		imshow("img", img);
//		img.copyTo(tmp);
//		//截取矩形包围的图像，并保存到dst中  
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
//	namedWindow("img");//定义一个img窗口  
//	setMouseCallback("img", on_mouse, 0);//调用回调函数  
//	imshow("img", img);
//}