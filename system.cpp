#include "system.h"

bool System::analizer(QVector<bool> va)
{
    if (va.size() < 20)
        return true;
    int count_t = 0;
    int count_f = 0;

    for (int i=0; i < va.size(); i++) {
        if (va[i]) {
            count_t++;
        } else {
            count_f++;
        }
    }

    if (count_t > (va.size()/4)*3) {
        this->sms->send_message("El Conductor se esta quedando dormido.", gps->getcurubication());
    }

    return count_t < count_f;
}

System::System()
{
    sms = new Message();
    gps = new GPS();


    face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
    lips_cascade_name = "/home/kodev/Universidad/Proyecto/FaceRec/Mouth2.xml";
    main_window_name = "Captura - Detección de Rostro";
    face_window_name = "Captura - Rostro";
    lips_windows_name = "Corte solo de boca";
    rng = RNG(12345);

    skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
    audio_file = "/home/kodev/Música/nautical022.mp3";

    eye_center = new EyeCenter();
    eye_corner = new EyeCorner();
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(audio_file));
    player->setVolume(50);

}

System::~System()
{
    delete eye_center;
    delete eye_corner;
}

int System::init()
{
    Mat frame;

    // Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){
        printf("--(!)Error loading face cascade, please change face_cascade_name in source code.\n");
        return -1;
    };
    if( !lips_cascade.load( lips_cascade_name ) ){
        printf("--(!)Error cargando ojos\n");
        return -1;
    };

    namedWindow(main_window_name,CV_WINDOW_NORMAL);
    moveWindow(main_window_name, 400, 100);
    namedWindow(face_window_name,CV_WINDOW_NORMAL);
    moveWindow(face_window_name, 10, 100);
    namedWindow("Right Eye",CV_WINDOW_NORMAL);
    moveWindow("Right Eye", 10, 600);
    namedWindow("Left Eye",CV_WINDOW_NORMAL);
    moveWindow("Left Eye", 10, 800);

    eye_corner->createCornerKernels();

    ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2),
            43.0, 0.0, 360.0, Scalar(255, 255, 255), -1);

    VideoCapture capture(-1);
    if( capture.isOpened() ) {
        while( true ) {
            capture.read(frame);
            // mirror it
            flip(frame, frame, 1);
            frame.copyTo(debugImage);

            // Apply the classifier to the frame
            if( !frame.empty() ) {
                this->detectAndDisplay( frame );
            }
            else {
                printf(" --(!) No captured frame -- Break!");
                break;
            }
            //if (timer_lips.size() > 30 || (timer_left.size() > 12 && timer_right.size() > 12)) {
            if (!this->analizer(this->verify_left) || !this->analizer(this->verify_right) || !this->analizer(this->verify_lips)) {
                player->play();
            } else {
                player->stop();
            }

            imshow(main_window_name,debugImage);

            int c = waitKey(10);
            if( (char)c == 'c' ) { break; }
            if( (char)c == 'f' ) {
                imwrite("frame.png",frame);
            }

        }
    }

    eye_corner->releaseCornerKernels();

}

void System::findEyes(Mat frame_gray, Rect face)
{
    Mat faceROI = frame_gray(face);
    Mat debugFace = faceROI;

    EyeCenter *eye_center = new EyeCenter();

    if (kSmoothFaceImage) {
        double sigma = kSmoothFaceFactor * face.width;
        GaussianBlur( faceROI, faceROI, Size( 0, 0 ), sigma);
    }
    //-- Find eye regions and draw them
    int eye_region_width = face.width * (kEyePercentWidth/100.0);
    int eye_region_height = face.width * (kEyePercentHeight/100.0);
    int eye_region_top = face.height * (kEyePercentTop/100.0);
    Rect leftEyeRegion(face.width*(kEyePercentSide/100.0),
                           eye_region_top,eye_region_width,eye_region_height);
    Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide/100.0),
                            eye_region_top,eye_region_width,eye_region_height);

    //-- Find Eye Centers
    Point leftPupil = eye_center->findEyeCenter(faceROI,leftEyeRegion,"Ojo izquierdo");
    if (leftPupil.y < 30) {
        cout << Helpers::currentDateTime() << endl;
        cout << "ERROR: No se ha encontrado la cornea izquierda.\t";
        cout << "X: " << leftPupil.x << " - Y: " << leftPupil.y << endl;
        cout << "Tiempo Izquierdo: " << time(NULL) << endl;

        alerts << QString(QString::fromStdString(Helpers::currentDateTime()) + QString(" - Cornera Izquierda no encontrada."));
        timer_left.append(time(NULL));
    } else {
        if (!alerts.isEmpty())
            alerts.pop_front();
        if (!timer_left.isEmpty()) {
            time_t tp = time(NULL);
            time_t t = timer_left.last();
            cout << "Diferencia Izquierda: " << tp - t << endl;
            if ((tp - t) > 10) {
                timer_left.pop_back();
            }
        }
    }
    verify_left.append(leftPupil.y < 30);
    if (verify_left.size() > 20) {
        verify_left.pop_front();
    }
    //cout << leftPupil.x << " - " << leftPupil.y << endl;
    Point rightPupil = eye_center->findEyeCenter(faceROI,rightEyeRegion,"Ojo Derecho");
    if (rightPupil.y < 30) {
        cout << Helpers::currentDateTime() << endl;
        cout << "ERROR: No se ha encontrado la cornea derecha.\t";
        cout << "X: " << rightPupil.x << " - Y: " << rightPupil.y << endl;
        cout << "Tiempo Derecho: " << time(NULL) << endl;

        alerts << QString(QString::fromStdString(Helpers::currentDateTime()) + " - Cornera Derecha no encontrada.");
        timer_right.append(time(NULL));
    } else {
        if (!alerts.isEmpty())
            alerts.pop_front();
        if (!timer_right.isEmpty()) {
            time_t tp = time(NULL);
            time_t t = timer_right.last();
            cout << "Diferencia Derecha: " << tp - t << endl;
            if ((tp - t) > 10) {
                timer_right.pop_back();
            }
        }
    }
    verify_right.append(rightPupil.y < 30);
    if (verify_right.size() > 20) {
        verify_right.pop_front();
    }
    // get corner regions
    Rect leftRightCornerRegion(leftEyeRegion);
    leftRightCornerRegion.width -= leftPupil.x;
    leftRightCornerRegion.x += leftPupil.x;
    leftRightCornerRegion.height /= 2;
    leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
    Rect leftLeftCornerRegion(leftEyeRegion);
    leftLeftCornerRegion.width = leftPupil.x;
    leftLeftCornerRegion.height /= 2;
    leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
    Rect rightLeftCornerRegion(rightEyeRegion);
    rightLeftCornerRegion.width = rightPupil.x;
    rightLeftCornerRegion.height /= 2;
    rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
    Rect rightRightCornerRegion(rightEyeRegion);
    rightRightCornerRegion.width -= rightPupil.x;
    rightRightCornerRegion.x += rightPupil.x;
    rightRightCornerRegion.height /= 2;
    rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
    rectangle(debugFace,leftRightCornerRegion,200);
    rectangle(debugFace,leftLeftCornerRegion,200);
    rectangle(debugFace,rightLeftCornerRegion,200);
    rectangle(debugFace,rightRightCornerRegion,200);
    // change eye centers to face coordinates
    rightPupil.x += rightEyeRegion.x;
    rightPupil.y += rightEyeRegion.y;
    leftPupil.x += leftEyeRegion.x;
    leftPupil.y += leftEyeRegion.y;
    // draw eye centers
    circle(debugFace, rightPupil, 3, 1234);
    circle(debugFace, leftPupil, 3, 1234);

    //-- Find Eye Corners
    if (kEnableEyeCorner) {
        Point2f leftRightCorner = eye_corner->findEyeCorner(faceROI(leftRightCornerRegion), true, false);
        leftRightCorner.x += leftRightCornerRegion.x;
        leftRightCorner.y += leftRightCornerRegion.y;
        Point2f leftLeftCorner = eye_corner->findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
        leftLeftCorner.x += leftLeftCornerRegion.x;
        leftLeftCorner.y += leftLeftCornerRegion.y;
        Point2f rightLeftCorner = eye_corner->findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
        rightLeftCorner.x += rightLeftCornerRegion.x;
        rightLeftCorner.y += rightLeftCornerRegion.y;
        Point2f rightRightCorner = eye_corner->findEyeCorner(faceROI(rightRightCornerRegion), false, false);
        rightRightCorner.x += rightRightCornerRegion.x;
        rightRightCorner.y += rightRightCornerRegion.y;
        circle(faceROI, leftRightCorner, 3, 200);
        circle(faceROI, leftLeftCorner, 3, 200);
        circle(faceROI, rightLeftCorner, 3, 200);
        circle(faceROI, rightRightCorner, 3, 200);
    }

    int tmp = (faceROI.rows/2) / 3;

    Rect r(0, faceROI.rows/2 + tmp, faceROI.cols, faceROI.rows - faceROI.rows/2 - tmp);

    Mat middle_face_bottom = faceROI(r);
    vector<Rect> lips;

    lips_cascade.detectMultiScale( middle_face_bottom, lips, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));
    if (lips.size() > 0) {
        if (!alerts.isEmpty())
            alerts.pop_front();
        if (!timer_lips.isEmpty()) {
            time_t tp = time(NULL);
            time_t t = timer_lips.last();
            cout << "Diferencia: " << tp - t << endl;
            if ((tp - t) > 10) {
                timer_lips.pop_back();
            }
        }
        //for (size_t j=0; j < 1; j++) {
        int j=0;
        Point start(face.x + lips[j].x, face.y + lips[j].y + middle_face_bottom.rows + tmp*2);
        Point end(face.x + lips[j].x + lips[j].width, face.y + lips[j].y + lips[j].height + middle_face_bottom.rows + tmp*2);
        rectangle(faceROI, start, end, CV_RGB(0,0,255), 1);

        Point start_bw(lips[j].x, lips[j].y);
        Point end_bw(lips[j].x + lips[j].width, lips[j].y + lips[j].height);
        rectangle(middle_face_bottom, start_bw, end_bw, CV_RGB(0,0,255), 1);
        //}
    } else {
        cout << Helpers::currentDateTime() << endl;
        cout << "ERROR: Boca no encontrada." << endl;
        alerts << QString(QString::fromStdString(Helpers::currentDateTime()) + " Boca no encontrada");
        timer_lips.append(time(NULL));
    }
    verify_lips.append(lips.size() == 0);
    if (verify_lips.size() > 20) {
        verify_lips.pop_front();
    }
    imshow(face_window_name, faceROI);
}

void System::detectAndDisplay(Mat frame)
{
    vector<Rect> faces;
    //Mat frame_gray;

    vector<Mat> rgbChannels(3);
    split(frame, rgbChannels);
    Mat frame_gray = rgbChannels[2];

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT, Size(150, 150) );
    //  findSkin(debugImage);

    for( int i = 0; i < faces.size(); i++ )
    {
        rectangle(debugImage, faces[i], 1234);
    }
    //-- Show what you got
    if (faces.size() > 0) {
        findEyes(frame_gray, faces[0]);
        //findLips(frame_gray, faces[0]);
    }

}
