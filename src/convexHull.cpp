#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
//设置全局参数
Mat srcImage, srcGray;
int thresh = 246;
void thresh_callback(int, void*)
{
    Mat srcTemp = srcImage.clone();
    Mat threMat;
    //二值化
    threshold(srcGray, threMat, thresh, 255, CV_THRESH_BINARY_INV);
    namedWindow("threMat",WINDOW_NORMAL);
    imshow("threMat", threMat);
    
    //轮廓检测参数
    vector<vector<Point> > contours;
    vector<Vec4i>hierarchy;
    //轮廓检测
    findContours(threMat, contours, hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    Mat findContoursImage = Mat::zeros(threMat.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(findContoursImage, contours, i, Scalar(0,255,0), 1, 8);
    }
    namedWindow("imageAfterFindContours",WINDOW_NORMAL);
    imshow("imageAfterFindContours",findContoursImage); 
    
    //凸包参数定义
    vector<vector<Point> >   pointHull(contours.size());
    Mat drawing = Mat::zeros(threMat.size(), CV_8UC3);  
    for (size_t i = 0; i < contours.size(); i++)
    {
        //Point 类型凸包检测
	convexHull(Mat(contours[i]),pointHull[i], false);
        drawContours(drawing, contours, i, Scalar(0,255,0), 1, 8);
        drawContours(drawing, pointHull, i,Scalar(0,255,0), 1, 8);
    }
	//绘制凸包
    namedWindow("result",WINDOW_NORMAL);
    imshow("result", drawing);
}
 
int main()
{
	srcImage = imread("../photos/hand.jpg");
	if (!srcImage.data){
		cout << "failed to read" << endl;
		system("pause");
		return -1;
	}
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	blur(srcGray, srcGray, Size(3, 3));
	namedWindow("SRC",WINDOW_NORMAL);
	imshow("SRC", srcImage);
	//鼠标回调
	createTrackbar("Threshold","SRC", &thresh,255,thresh_callback);
	thresh_callback(0, 0);
	waitKey(0);
    return 0;
}

