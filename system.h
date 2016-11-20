#ifndef SYSTEM_H
#define SYSTEM_H
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <time.h>
#include <queue>
#include <stdio.h>
#include <math.h>

#include <QString>
#include <QStringList>
#include <QMediaPlayer>
#include <QVector>

#include "constants.h"
#include "eyecenter.h"
#include "eyecorner.h"
#include "message.h"
#include "gps.h"

using namespace std;
using namespace cv;

class System
{
private:
    String face_cascade_name;
    String lips_cascade_name;
    CascadeClassifier face_cascade;
    CascadeClassifier lips_cascade;
    string main_window_name;
    string face_window_name;
    string lips_windows_name;
    RNG rng;
    Mat debugImage;
    Mat skinCrCbHist;
    EyeCenter *eye_center;
    EyeCorner *eye_corner;
    QString audio_file;
    QStringList alerts;
    QMediaPlayer *player;
    QVector<time_t> timer_left;
    QVector<time_t> timer_right;
    QVector<time_t> timer_lips;

    QVector<bool> verify_left;
    QVector<bool> verify_right;
    QVector<bool> verify_lips;

    bool analizer(QVector<bool> va);

    Message *sms;
    GPS *gps;

    QString getMediaPath();

    double FPS;
public:
    System();
    ~System();
    int init();
    void findEyes(Mat frame_gray, Rect face);
    void detectAndDisplay(Mat frame);
};

#endif // SYSTEM_H
