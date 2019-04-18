#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cv.h>
//#include "opencv2/highgui/highgui.hpp"
//include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Point Center_cal2(vector<vector<Point> > contours, int i);
bool QRCodeTest3(Mat &src,Mat &src_gray);

//Scalar colorful = CV_RGB(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));

//获取轮廓的中心点
