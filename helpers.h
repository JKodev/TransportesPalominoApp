#ifndef HELPERS_H
#define HELPERS_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <queue>
#include <stdio.h>

using namespace cv;
using namespace std;

class Helpers
{
public:
    static bool rectInImage(Rect rect, Mat image);
    static bool inMat(Point p, int rows, int cols);
    static Mat matrixMagnitude(const Mat &matX, const Mat &matY);
    static double computeDynamicThreshold(const Mat &mat, double stdDevFactor);
    static string currentDateTime();
};

#endif // HELPERS_H
