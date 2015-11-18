#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <vector>
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

const string facealgorithm = "FaceRecognizer.Eigenfaces";

const int faceHeight = 320;


int main0(int argc, char *argv[])
{
    if(!initModule_contrib()){
        cerr<<"ERROR: module 'contrib' not loaded!"<<endl;
        exit(0);
    }
//    vector<string> algorithms;
//    Algorithm::getList(algorithms);
//    cout<<"size: "<<algorithms.size()<<endl;
//    for(int i = 0; i < algorithms.size(); i++){
//        cout<<algorithms[i]<<endl;
//    }

//    return 0;






    //preprocessing
    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;
    faceCascade.load(classifierbasepath + faceCascadeFilename);
    eyeCascade1.load(classifierbasepath + eyeCascadeFilename1);
    eyeCascade2.load(classifierbasepath + eyeCascadeFilename2);
    Rect r;

    Mat avgface;
    vector<Mat> traningset;
    vector<int> label;
    int valid = 0;
    for(int i = 1; i < 6025; i++){
        Mat img;
        img = imread(imagebasepath+"pic"+fixedLength(i,5)+".jpg");
        Rect faceRect;  // Position of detected face.
        Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
        Point leftEye, rightEye;    // Position of the detected eyes.
        Mat preprocessedFace = getPreprocessedFace(img,
                                       faceHeight,
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
//        circle(img, leftEye, 3, Scalar(28000,255,0));
//        circle(img, rightEye, 3, Scalar(0,0,255));
        if(avgface.empty()){
            preprocessedFace.copyTo(avgface);
        }


        if(!img.empty()){
           // imshow("a",img);
            if(!preprocessedFace.empty()){
                traningset.push_back(preprocessedFace);
                label.push_back(valid);
                valid++;
               // imshow("out", preprocessedFace);
                avgface = ((double)(valid)/(valid+1))*avgface + ((double)(1)/(valid+1))*preprocessedFace;
                //imshow("avg",avgface);
                cout<<valid<<endl;
            }
           // waitKey(0);
        }

    }
   // imshow("avg",avgface);
    waitKey(0);



    //traning
    Ptr<FaceRecognizer> model;
    model = Algorithm::create<FaceRecognizer>(facealgorithm);
    if(model.empty()){
        cerr<<"ERROR: model create failed!"<<endl;
        exit(0);
    }
    int t = getTickCount();
    model->train(traningset,label);
    cout<<"traning time: "<<(getTickCount()-t)/getTickFrequency()<<endl;
    Mat avg2 = model->get<Mat>("mean");
    avg2 = getImageFrome1DFloatMat(avg2,faceHeight);
    imshow("avg2", avg2);

    Mat eigenvectors = model->get<Mat>("eigenvectors");

    for(int i = 0; i < min(20,eigenvectors.cols); i++){
        Mat eigenvector = eigenvectors.col(i).clone();
        Mat eigenface = getImageFrome1DFloatMat(eigenvector,
                                                  faceHeight);
        imshow(format("Eigenface%d",i), eigenface);
    }

    model->save("../data/model/model.yml");

    waitKey(0);






    return 0;
}

int main(int argc, char *argv[]){ //load model
    if(!initModule_contrib()){
        cerr<<"ERROR: module 'contrib' not loaded!"<<endl;
        exit(0);
    }
    Ptr<FaceRecognizer> model;
    model = Algorithm::create<FaceRecognizer>(facealgorithm);
    Mat labels;
    try{
        model->load("../data/model/model.yml");
        labels = model->get<Mat>("labels");
    }
    catch(cv::Exception &e){

    }
    if(labels.rows<=0){
        cerr<<"Load model failed!"<<endl;
        exit(1);
    }

    Mat eigenvectors = model->get<Mat>("eigenvectors");

    cout<<"size: "<<eigenvectors.cols<<endl;
    for(int i = 0; i < min(20,eigenvectors.cols); i++){
        Mat eigenvector = eigenvectors.col(i).clone();
        Mat eigenface = getImageFrome1DFloatMat(eigenvector,
                                                  faceHeight);
        imshow(format("Eigenface%d",i), eigenface);
    }


    waitKey(0);

    return 0;
}
