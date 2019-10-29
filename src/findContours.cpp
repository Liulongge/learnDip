#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    Mat imageSource = imread("test.jpg", 0);
    imshow("原始图像", imageSource);
    
    Mat image;
    GaussianBlur(imageSource, image, Size(3, 3), 0);
    Canny(imageSource, image, 100, 250);

    vector<vector<Point> > contours;//contours包涵Point类型，向量内每个元素保存了一组由连续的Point点构成的点的集合的向量，
                                    //每一组Point点集就是一个轮廓
    vector<Vec4i> hierarchy;//4维int类型变量,向量内每一个元素的4个int型变量——hierarchy[i][0] ~hierarchy[i][3]，分别表示第
                            //i个轮廓的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号
    findContours(image, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point());
    Mat imageContours = Mat::zeros(image.size(), CV_8UC1);

    for (int i = 0; i < contours.size(); i++)
    {
        //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
        cout << "向量hierarchy的第" << i << "个元素内容为：" << hierarchy[i] << endl << endl;
        //绘制轮廓                                                                                                      
        drawContours(imageContours, contours, i, Scalar(255), 1, 8);
    }
    imshow("Contours Image CV_TRET_TREE", imageContours); //轮廓
    waitKey(0);
    
    return 0;
}
