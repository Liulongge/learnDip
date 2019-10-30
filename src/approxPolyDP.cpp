#include <iostream>
#include "opencv2/opencv.hpp"
 
using namespace std;
using namespace cv;
 
int thresholdVal = 86;
void PolyBound_demo(int pos, void* userdata);
int ploymode = 0;
Mat src, blursrc;
int main()
{
    Mat graysrc;
    src = imread("../photos/hand.jpg");
    if (src.empty())
    {
  	cout << "Can't load the image " << endl;
    }

    imshow("src", src);
    cvtColor(src, graysrc, COLOR_BGR2GRAY);
    GaussianBlur(graysrc, blursrc, Size(5, 5),3,3);
    
    namedWindow("PolyBound", WINDOW_AUTOSIZE);
    createTrackbar("threshold", "PolyBound", &thresholdVal , 255, PolyBound_demo);
    createTrackbar("ploymode", "PolyBound", &ploymode, 1, PolyBound_demo);
	
    PolyBound_demo(thresholdVal, 0);
    waitKey(0);
 }
 
void PolyBound_demo(int pos, void* userdata)
{
	//阈值化
    Mat bin;
    threshold(~blursrc, bin, thresholdVal, 255, CV_THRESH_BINARY);
    imshow("bin", bin);
 
    //轮廓检测
    vector<vector<Point> >  contours;
    vector<Vec4i> hiearchy;
    findContours(bin, contours, hiearchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
    Mat findContoursImage = Mat::zeros(bin.size(), CV_8UC1);
    for (int i = 0; i < contours.size(); i++)
    {
        drawContours(findContoursImage, contours, i, Scalar(255), 1, 8);
    }
    imshow("findContoursImage", findContoursImage); //轮廓 
 
    //对图像轮廓点进行多边形拟合
    Mat approxPolyDPImage = Mat::zeros(bin.size(), CV_8UC1);
    vector<vector<Point> > contours_ploy(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
 	approxPolyDP(contours[i], contours_ploy[i], 3, true);
        drawContours(approxPolyDPImage, contours_ploy, i, Scalar(255), 1, 8);
    }
    imshow("approxPolyDPImage", approxPolyDPImage); //轮廓 
   
    Mat dst;//必须在钩子函数里重新创建dst，如果使用src将会造成多次图像重影！！！
    src.copyTo(dst);
    if (ploymode == 0)//矩形+圆形
    {
        for (int j = 0; j < contours_ploy.size(); j++)
        {
            //对于以上生成的点集，寻找最外面的矩形边界
	    Rect poly_rect = boundingRect(contours_ploy[j]);
	    rectangle(dst, poly_rect,Scalar(0,0,255), 2, LINE_AA);
	    //对给定的2D点集，去寻找面积最小的可包围它们的圆形
	    Point2f center;
	    float radius;
	    minEnclosingCircle(contours_ploy[j], center, radius);
	    circle(dst, center, radius,Scalar(0,255,0), 2, LINE_AA);
        }	
    }
    else//旋转矩形+椭圆
    {
 	for (int j = 0; j < contours_ploy.size(); j++)
 	{
            //对于给定的2D点集，寻找可旋转的最小面积的包围矩形
	    RotatedRect poly_rotateRect = minAreaRect(contours_ploy[j]);
	    Point2f pts[4];
	    poly_rotateRect.points(pts);   //返回矩形四个顶点  void points(Point2f pts[]) const;
	    for (int k = 0; k < 4; k++)
	    {
		line(dst, pts[k], pts[(k + 1) % 4],Scalar(0,0,255), 2, LINE_AA);
	    }
	    //用椭圆拟合二维点集 至少需要有5个点
	    if (contours_ploy[j].size() >= 5)
	    {
		RotatedRect poly_ellipse = fitEllipse(contours_ploy[j]);
		ellipse(dst, poly_ellipse,Scalar(0,255,0), 2, LINE_AA);
	    }
	}
		
    }
    imshow("PolyBound", dst);
}


