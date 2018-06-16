#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat src;
    src = imread("../data/test.jpg");
    imshow("test",src);
    waitKey(0);
    return 0;
}
