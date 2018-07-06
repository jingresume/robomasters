#include <opencv2/opencv.hpp>
#include <iostream>

enum EnemyColor {
    RED = 0 ,
    BLUE = 1
};

class ArmorSimple
{
public:
    ArmorSimple( EnemyColor color=BLUE ) : _color(color) {}
    cv::RotatedRect getTargetArea(cv::Mat img);
    void PreProcession(cv::Mat img);
    cv::RotatedRect adjustRRect(const cv::RotatedRect & rect);
    void findlamp(std::vector<cv::RotatedRect> & rects);
    cv::RotatedRect boundingRRect(const cv::RotatedRect & left, const cv::RotatedRect & right);
    cv::RotatedRect boudingLamp(std::vector<cv::RotatedRect> & rrects);
    bool setColor( EnemyColor color = BLUE )
    {
        _color = color;
        return true;
    }

private:

    bool _is_lost;
    int _lost_cnt;
    bool _is_small_armor;           // true if armor is the small one, otherwise false
    cv::RotatedRect _res_last;      // last detect result
    cv::Rect _dect_rect;            // detect roi of original image
    cv::Mat _binary_template;       // armor template binary image
    cv::Mat _binary_template_small; // small armor template binay image
    cv::Mat _src;                   // source image
    cv::Mat _g;                     // green component of source image
    cv::Mat _ec;                    // enemy color
    cv::Mat _max_color;             // binary image of sub between blue and red component
    cv::Size _size;
    EnemyColor _color;

};
