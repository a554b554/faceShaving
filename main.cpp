#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "detectobject.h"
#include "preprocessor.h"
#include "helpfunc.h"

using namespace cv;
using namespace std;

const string imagebasepath = "../data/image/";
const string classifierbasepath = "../data/Classifier/haarcascades/";
//const char *faceCascadeFilename = "lbpcascade_frontalface.xml";     // LBP face detector.
const string faceCascadeFilename = "haarcascade_frontalface_alt_tree.xml";  // Haar face detector.
//const string eyeCascadeFilename1 = "haarcascade_lefteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const string eyeCascadeFilename2 = "haarcascade_righteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char eyeCascadeFilename1 = "haarcascade_mcs_lefteye.xml";       // Good eye detector for open-or-closed eyes.
//const char eyeCascadeFilename2 = "haarcascade_mcs_righteye.xml";       // Good eye detector for open-or-closed eyes.
const string eyeCascadeFilename1 = "haarcascade_eye.xml";               // Basic eye detector for open eyes only.
const string eyeCascadeFilename2 = "haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.



int main(int argc, char *argv[])
{\
    cout<<fixedLength(23,5)<<endl;

    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;
    faceCascade.load(classifierbasepath + faceCascadeFilename);
    eyeCascade1.load(classifierbasepath + eyeCascadeFilename1);
    eyeCascade2.load(classifierbasepath + eyeCascadeFilename2);
    Rect r;

    Mat avgface;
    int valid = 0;
    for(int i = 100; i < 800; i++){
        Mat img;
        img = imread(imagebasepath+"pic00"+std::to_string(i)+".jpg");
        Rect faceRect;  // Position of detected face.
        Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
        Point leftEye, rightEye;    // Position of the detected eyes.
        Mat preprocessedFace = getPreprocessedFace(img,
                                       320,
                                       faceCascade,
                                       eyeCascade1,
                                       eyeCascade2,
                                       true,
                                       &faceRect,
                                       &leftEye,
                                       &rightEye,
                                       &searchedLeftEye,
                                       &searchedRightEye);

        rectangle(img,faceRect, Scalar(255,0,0));
//        rectangle(img, searchedLeftEye,  Scalar(0,255,0));
//        rectangle(img, searchedRightEye,  Scalar(0,0,255));
//        circle(img, leftEye, 3, Scalar(0,255,0));
//        circle(img, rightEye, 3, Scalar(0,0,255));
        if(avgface.empty()){
            preprocessedFace.copyTo(avgface);
        }


        if(!img.empty()){
            imshow("a",img);
            if(!preprocessedFace.empty()){
                valid++;
                imshow("out", preprocessedFace);
                avgface = ((double)(valid)/(valid+1))*avgface + ((double)(1)/(valid+1))*preprocessedFace;
                imshow("avg",avgface);
            }
            waitKey(0);
        }
    }


    return 0;
}

