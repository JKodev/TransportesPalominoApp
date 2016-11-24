#include "eyecenter.h"

EyeCenter::EyeCenter()
{

}


Point EyeCenter::unscalePoint(Point p, Rect origSize) {
    float ratio = (((float)kFastEyeWidth)/origSize.width);
    int x = round(p.x / ratio);
    int y = round(p.y / ratio);
    return Point(x,y);
}

void EyeCenter::scaleToFastSize(const Mat &src, Mat &dst) {
    resize(src, dst, Size(kFastEyeWidth,(((float)kFastEyeWidth)/src.cols) * src.rows));
}

Mat EyeCenter::computeMatXGradient(const Mat &mat) {
    Mat out(mat.rows,mat.cols,CV_64F);

    for (int y = 0; y < mat.rows; ++y) {
        const uchar *Mr = mat.ptr<uchar>(y);
        double *Or = out.ptr<double>(y);

        Or[0] = Mr[1] - Mr[0];
        for (int x = 1; x < mat.cols - 1; ++x) {
            Or[x] = (Mr[x+1] - Mr[x-1])/2.0;
        }
        Or[mat.cols-1] = Mr[mat.cols-1] - Mr[mat.cols-2];
    }

    return out;
}

void EyeCenter::testPossibleCentersFormula(int x, int y, const Mat &weight, double gx, double gy, Mat &out) {
    // for all possible centers
    for (int cy = 0; cy < out.rows; ++cy) {
        double *Or = out.ptr<double>(cy);
        const unsigned char *Wr = weight.ptr<unsigned char>(cy);

        for (int cx = 0; cx < out.cols; ++cx) {
            if (x == cx && y == cy) {
                continue;
            }
            // create a vector from the possible center to the gradient origin
            double dx = x - cx;
            double dy = y - cy;
            // normalize d
            double magnitude = sqrt((dx * dx) + (dy * dy));
            dx = dx / magnitude;
            dy = dy / magnitude;
            double dotProduct = dx*gx + dy*gy;
            dotProduct = std::max(0.0,dotProduct);
            // square and multiply by the weight
            if (kEnableWeight) {
                Or[cx] += dotProduct * dotProduct * (Wr[cx]/kWeightDivisor);
            } else {
                Or[cx] += dotProduct * dotProduct;
            }
        }
    }
}

Point EyeCenter::findEyeCenter(Mat face, Rect eye, std::string debugWindow) {

    Mat eyeROIUnscaled = face(eye);
    Mat eyeROI;
    this->scaleToFastSize(eyeROIUnscaled, eyeROI);
    // dibujar region del ojo
    rectangle(face,eye,1234);
    //-- encontrar gradadiente
    Mat gradientX = computeMatXGradient(eyeROI);
    Mat gradientY = computeMatXGradient(eyeROI.t()).t();
    //-- Normalizar el gradadiente
    // calcular todas las magnitudes
    Mat mags = Helpers::matrixMagnitude(gradientX, gradientY);
    //calcular el limite
    double gradientThresh = Helpers::computeDynamicThreshold(mags, kGradientThreshold);
    //normalizar
    for (int y = 0; y < eyeROI.rows; ++y) {
        double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
        const double *Mr = mags.ptr<double>(y);
        for (int x = 0; x < eyeROI.cols; ++x) {
            double gX = Xr[x], gY = Yr[x];
            double magnitude = Mr[x];
            if (magnitude > gradientThresh) {
                Xr[x] = gX/magnitude;
                Yr[x] = gY/magnitude;
            } else {
                Xr[x] = 0.0;
                Yr[x] = 0.0;
            }
        }
    }
    imshow(debugWindow,gradientX);
    //-- Crear una imagen borrosa e invertida para cargar
    Mat weight;
    GaussianBlur( eyeROI, weight, Size( kWeightBlurSize, kWeightBlurSize ), 0, 0 );
    for (int y = 0; y < weight.rows; ++y) {
        unsigned char *row = weight.ptr<unsigned char>(y);
        for (int x = 0; x < weight.cols; ++x) {
            row[x] = (255 - row[x]);
        }
    }

    //-- Correr el algoritmo
    Mat outSum = Mat::zeros(eyeROI.rows,eyeROI.cols,CV_64F);
    // por cada posible ubicacion de gradadiente
    for (int y = 0; y < weight.rows; ++y) {
        const double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
        for (int x = 0; x < weight.cols; ++x) {
            double gX = Xr[x], gY = Yr[x];
            if (gX == 0.0 && gY == 0.0) {
                continue;
            }
            this->testPossibleCentersFormula(x, y, weight, gX, gY, outSum);
        }
    }

    double numGradients = (weight.rows*weight.cols);
    Mat out;
    outSum.convertTo(out, CV_32F,1.0/numGradients);

    Point maxP;
    double maxVal;
    minMaxLoc(out, NULL,&maxVal,NULL,&maxP);

    if(kEnablePostProcess) {
        Mat floodClone;

        double floodThresh = maxVal * kPostProcessThreshold;
        threshold(out, floodClone, floodThresh, 0.0f, THRESH_TOZERO);

        if(kPlotVectorField) {
            imwrite("eyeFrame.png",eyeROIUnscaled);
        }

        Mat mask = this->floodKillEdges(floodClone);
        minMaxLoc(out, NULL,&maxVal,NULL,&maxP,mask);
    }
    return this->unscalePoint(maxP,eye);
}


bool EyeCenter::floodShouldPushPoint(const Point &np, const Mat &mat)
{
    return Helpers::inMat(np, mat.rows, mat.cols);
}


Mat EyeCenter::floodKillEdges(Mat &mat)
{
    rectangle(mat,Rect(0,0,mat.cols,mat.rows),255);

    Mat mask(mat.rows, mat.cols, CV_8U, 255);
    std::queue<Point> toDo;
    toDo.push(Point(0,0));
    while (!toDo.empty()) {
        Point p = toDo.front();
        toDo.pop();
        if (mat.at<float>(p) == 0.0f) {
            continue;
        }

        Point np(p.x + 1, p.y); // derecha
        if (this->floodShouldPushPoint(np, mat)) toDo.push(np);
        np.x = p.x - 1; np.y = p.y; // izquierda
        if (this->floodShouldPushPoint(np, mat)) toDo.push(np);
        np.x = p.x; np.y = p.y + 1; // abajo
        if (this->floodShouldPushPoint(np, mat)) toDo.push(np);
        np.x = p.x; np.y = p.y - 1; // arriba
        if (this->floodShouldPushPoint(np, mat)) toDo.push(np);
        // destruir
        mat.at<float>(p) = 0.0f;
        mask.at<uchar>(p) = 0;
    }
    return mask;
}
