#ifndef EYECORNER_H
#define EYECORNER_H

#include "opencv2/imgproc/imgproc.hpp"

#define kEyeLeft true
#define kEyeRight false

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <queue>
#include <stdio.h>

#include "constants.h"
#include "helpers.h"

#include "eyecenter.h"




class EyeCorner
{
private:
    float kEyeCornerKernel[4][6] = {
        {-1,-1,-1, 1, 1, 1},
        {-1,-1,-1,-1, 1, 1},
        {-1,-1,-1,-1, 0, 3},
        { 1, 1, 1, 1, 1, 1},
    };
    Mat *leftCornerKernel;
    Mat *rightCornerKernel;

public:
    EyeCorner();

    void createCornerKernels();
    void releaseCornerKernels();
    Mat eyeCornerMap(const cv::Mat &region, bool left, bool left2);
    Point2f findEyeCorner(cv::Mat region, bool left, bool left2);
    Point2f findSubpixelEyeCorner(cv::Mat region, cv::Point maxP);
};

#endif // EYECORNER_H
