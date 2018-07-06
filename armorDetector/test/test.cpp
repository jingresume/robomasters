#include<opencv2/opencv.hpp>
#include<iostream>
#include <assert.h>

using namespace std;
using namespace cv;

#define USE_SIMPLE
#ifdef USE_SIMPLE
    #include "ArmorSimple.hpp"
#else
    #include "ArmorDetector.hpp"
#endif

int main( int argc, char** argv )
{
#ifdef USE_SIMPLE
    ArmorSimple detector;
#else
    ArmorDetector detector;
#endif
    Mat temp,temp_s;
    temp = imread("../data/template.bmp");
    temp_s = imread("../data/small_template.bmp");
    if(temp.empty()||temp_s.empty())
    {
        cout<<"template img read faild" << endl;
        return -1;
    }
#ifndef USE_SIMPLE
    detector.initTemplate(temp, temp_s);
#endif
    VideoCapture cap("../data/webcam_1.avi");

    assert(detector.setColor(BLUE));

    Mat src;
    cap>>src;
    RotatedRect rect;
    while(!src.empty())
    {
        vector<cv::RotatedRect> rects;
        detector.PreProcession(src);
        detector.findlamp(rects);
        rect=detector.boudingLamp(rects);

//        rect = detector.getTargetAera(src);
//        Point2f vertices[4];
//        rect.points(vertices);
//        for (int i = 0; i < 4; i++) {
//          line(src, vertices[i], vertices[(i + 1) % 4], CV_RGB(0, 255, 0), 3);
//        }
        Point2f vertices[4];
        rect.points(vertices);
        for (int i = 0; i < 4; i++) {
          line(src, vertices[i], vertices[(i + 1) % 4], CV_RGB(0, 255, 0), 3);
        }

        imshow("src",src);
        if(waitKey(10)=='q')  break;
        cap>>src;
    }
}
