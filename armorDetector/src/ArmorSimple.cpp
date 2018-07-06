#include "ArmorSimple.hpp"

#define SHOW_DEBUG

void ArmorSimple::PreProcession(cv::Mat img)
{
   cv::Mat BGR[3];
   _src = img.clone();
   cv::split(img,BGR);
   _g = BGR[1];
   if(_color == RED)
   {
    _ec = BGR[2].clone();
    cv::threshold(_ec,_max_color,254,255,cv::THRESH_BINARY);
   }
   else
   {
    _ec = BGR[0].clone();
    cv::threshold(_ec,_max_color,254,255,cv::THRESH_BINARY);
   }
   cv::medianBlur(_max_color,_max_color,5);
#ifdef SHOW_DEBUG
   imshow("max_color", _max_color);
//   imshow("b",BGR[0]);
//   imshow("g",BGR[1]);
//   imshow("r",BGR[2]);
#endif
}

cv::RotatedRect ArmorSimple::adjustRRect(const cv::RotatedRect & rect){
    const cv::Size2f & s = rect.size;
    if (s.width < s.height)
        return rect;
    return cv::RotatedRect(rect.center, cv::Size2f(s.height, s.width), rect.angle + 90.0);
}


void ArmorSimple::findlamp(std::vector<cv::RotatedRect> & rects)
{
    std::vector<std::vector<cv::Point2i> > contours_br;
    std::vector<cv::Vec4i> hierarchy;
    findContours(_max_color, contours_br, hierarchy, CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point2i> >::const_iterator it = contours_br.begin();
    int count = 0;
    while( it != contours_br.end() )
    {

       // std::cout << count++ << std::endl;
        cv::RotatedRect rrect = cv::minAreaRect(*it);
        cv::Rect rect = cv::boundingRect(*it);
        ++it;

        if ((rect.height < 10) ||
            (rect.height > 15 && rect.width > 0.6 * rect.height + 0.5) ||
            (rect.height <= 15 && rect.width > 0.9 * rect.height)){
           // ++it;
            continue;
        }
        rrect = adjustRRect(rrect);
        double angle = rrect.angle;
        angle = 90 - angle;
        angle = angle < 0 ? angle + 180 : angle;
        if( angle <80 || angle >100 )  continue;
        if( rect.height/rect.width < 2 ) continue;
#ifdef SHOW_DEBUG
        cv::Scalar color(0,0,255);
        if(_src.empty()) {
            std::cout << "_src is empty" << std::endl;
            continue;
        }
        cv::rectangle(_src, rect, color, 2);
        count++;
#endif
 //   ++it;
        rects.push_back(rrect);

    }
#ifdef SHOW_DEBUG
    imshow("show",_src);
    std::cout << count << std::endl;
#endif
}

#define POINT_DIST(p1,p2) std::sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y))

cv::RotatedRect ArmorSimple::boundingRRect(const cv::RotatedRect & left, const cv::RotatedRect & right){
    const cv::Point & pl = left.center, & pr = right.center;
    cv::Point2f center = (pl + pr) / 2.0;
    cv::Size2f wh_l = left.size;
    cv::Size2f wh_r = right.size;
    float width = POINT_DIST(pl, pr) - (wh_l.width + wh_r.width) / 2.0;
    float height = std::max(wh_l.height, wh_r.height);
    //float height = (wh_l.height + wh_r.height) / 2.0;
    float angle = std::atan2(right.center.y - left.center.y, right.center.x - left.center.x);
    return cv::RotatedRect(center, cv::Size2f(width, height), angle * 180 / CV_PI);
}

cv::RotatedRect ArmorSimple::boudingLamp(std::vector<cv::RotatedRect> & rrects)
{
    if(rrects.size()<2) return cv::RotatedRect();
    int count = 0;
    std::vector<cv::RotatedRect> lrects(rrects);
    std::vector<cv::RotatedRect> candidates;
    for(auto it=rrects.begin(), ite=rrects.end(); it!=ite; it++)
    {
        cv::RotatedRect right = *it;
        for(auto itl=lrects.begin(), itle=lrects.end(); itl!=itle; itl++)
        {
            cv::RotatedRect left = *itl;
            cv::RotatedRect candidate;
            if (right.center==left.center) continue;
            if (abs(right.angle-left.angle)>20) continue;
            if (abs(right.center.y - left.center.y) > 10) continue;
            if ((double)right.size.height/left.size.height > 1.5 ) continue;
            candidate = boundingRRect(left,right);
            candidates.push_back(candidate);
            count++;
        }
    }
    std::cout <<"boudingLamp"<< count << std::endl;
    cv::RotatedRect result;
    int max = 0;
    for (auto it = candidates.begin(), ite = candidates.end(); it!=ite; it++)
    {
        cv::RotatedRect tmp = *it;
       if((double)tmp.size.width/tmp.size.height<1)  continue;
       if(tmp.size.area()>max){ max = tmp.size.area(); result = tmp;}
    }
    if (max !=0 ) return result;
    return cv::RotatedRect();
}


cv::RotatedRect ArmorSimple::getTargetArea(cv::Mat img)
{
    std::vector<cv::RotatedRect> rects;
    PreProcession(img);
    findlamp(rects);
    return boudingLamp(rects);
}
