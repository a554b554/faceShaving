#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "detectobject.h"
#include "preprocessor.h"

using namespace cv;


const string imagebasepath = "../data/image/";
const string classifierbasepath = "../data/Classifier/haarcascades/";
//const char *faceCascadeFilename = "lbpcascade_frontalface.xml";     // LBP face detector.
const string faceCascadeFilename = "haarcascade_frontalface_alt_tree.xml";  // Haar face detector.
const string eyeCascadeFilename1 = "haarcascade_lefteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
const string eyeCascadeFilename2 = "haarcascade_righteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char eyeCascadeFilename1 = "haarcascade_mcs_lefteye.xml";       // Good eye detector for open-or-closed eyes.
//const char eyeCascadeFilename2 = "haarcascade_mcs_righteye.xml";       // Good eye detector for open-or-closed eyes.
//const char eyeCascadeFilename1 = "haarcascade_eye.xml";               // Basic eye detector for open eyes only.
//const char eyeCascadeFilename2 = "haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.



int main(int argc, char *argv[])
{
    Mat img;
    img = imread(imagebasepath+"pic00211.jpg");
    CascadeClassifier faceDetector;

    faceDetector.load(classifierbasepath+faceCascadeFilename);
    Rect r;
    detectLargestObject(img,faceDetector,r);
    rectangle(img,r,Scalar(0,250,0));
    imshow("r", img);

    waitKey(0);
    return 0;
}

