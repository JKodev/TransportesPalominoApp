#include "eyecorner.h"

EyeCorner::EyeCorner()
{

}

void EyeCorner::createCornerKernels() {
    rightCornerKernel = new Mat(4,6,CV_32F,kEyeCornerKernel);
    leftCornerKernel = new Mat(4,6,CV_32F);

    flip(*rightCornerKernel, *leftCornerKernel, 1);
}

void EyeCorner::releaseCornerKernels() {
    delete leftCornerKernel;
    delete rightCornerKernel;
}

Mat EyeCorner::eyeCornerMap(const Mat &region, bool left, bool left2) {
    Mat cornerMap;

    Size sizeRegion = region.size();
    Range colRange(sizeRegion.width / 4, sizeRegion.width * 3 / 4);
    Range rowRange(sizeRegion.height / 4, sizeRegion.height * 3 / 4);

    Mat miRegion(region, rowRange, colRange);

    filter2D(miRegion, cornerMap, CV_32F,
                 (left && !left2) || (!left && !left2) ? *leftCornerKernel : *rightCornerKernel);

    return cornerMap;
}

Point2f EyeCorner::findEyeCorner(Mat region, bool left, bool left2) {
    Mat cornerMap = this->eyeCornerMap(region, left, left2);

    Point maxP;
    minMaxLoc(cornerMap,NULL,NULL,NULL,&maxP);

    Point2f maxP2;
    maxP2 = this->findSubpixelEyeCorner(cornerMap, maxP);

    return maxP2;
}

Point2f EyeCorner::findSubpixelEyeCorner(Mat region, Point maxP) {

    Size sizeRegion = region.size();


    Mat cornerMap(sizeRegion.height * 10, sizeRegion.width * 10, CV_32F);

    resize(region, cornerMap, cornerMap.size(), 0, 0, INTER_CUBIC);

    Point maxP2;
    minMaxLoc(cornerMap, NULL,NULL,NULL,&maxP2);

    return Point2f(sizeRegion.width / 2 + maxP2.x / 10,
                       sizeRegion.height / 2 + maxP2.y / 10);
}
