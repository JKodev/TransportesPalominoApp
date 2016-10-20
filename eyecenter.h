#ifndef EYECENTER_H
#define EYECENTER_H

#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <queue>
#include <stdio.h>

#include "constants.h"
#include "helpers.h"

using namespace cv;

class EyeCenter
{
public:
    EyeCenter();

    Mat floodKillEdges(Mat &mat);
    Point unscalePoint(Point p, Rect origSize);
    void scaleToFastSize(const Mat &src, Mat &dst);
    Mat computeMatXGradient(const cv::Mat &mat);

    void testPossibleCentersFormula(int x, int y, const cv::Mat &weight,double gx, double gy, cv::Mat &out);
    Point findEyeCenter(Mat face, cv::Rect eye, std::string debugWindow);

    bool floodShouldPushPoint(const cv::Point &np, const cv::Mat &mat);
};

#endif // EYECENTER_H
