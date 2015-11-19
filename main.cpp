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
const string testingbasepath = "../data/test/testdata1/";


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

vector<Point> pset;

static bool clicked=false;
static void onMouse(int event, int x, int y, int, void*){
    if(event == EVENT_LBUTTONDOWN){
        clicked = true;
    }
    else if(event == EVENT_LBUTTONUP){
        clicked = false;
    }
    if(clicked==false){
        return;
    }

    pset.push_back(Point(x,y));
    cout<<Point(x,y)<<endl;
}

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
           // waitKey(0);pic
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

    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;
    faceCascade.load(classifierbasepath + faceCascadeFilename);
    eyeCascade1.load(classifierbasepath + eyeCascadeFilename1);
    eyeCascade2.load(classifierbasepath + eyeCascadeFilename2);


    namedWindow("origin");
    setMouseCallback("origin", onMouse);

    //reconstruction
    Mat eigenvectors = model->get<Mat>("eigenvectors");
    Mat averageFaceRow = model->get<Mat>("mean");

    for(int i = 100; i < 200; i++){
        Mat img;
        img = imread(testingbasepath+"313000"+fixedLength(i,4)+".jpg");
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
        if(preprocessedFace.empty()){
            continue;
        }
        cout<<preprocessedFace.type()<<" "<<eigenvectors.type()<<" "<<averageFaceRow.type()<<endl;
        Mat maskedFace = preprocessedFace.clone();
        Mat mask;
        mask.create(maskedFace.size(), CV_8UC1);
        mask.setTo(0);

        while(1){
            int key = waitKey(1);
            if((char)key == 'r'){
                pset.clear();
            }
            else if((char)key == 'q'){
                break;
            }
            imshow("origin", maskedFace);
            drawPoints(maskedFace, pset);
            drawPoints(mask, pset);
            imshow("mask", mask);
        }
        pset.clear();
        mask = mask.reshape(1,mask.rows*mask.cols);


        //mask eigen
        cout<<"mask row: "<<mask.rows<<endl;
        int maskregionsize = getmaskedsize(mask);
        cout<<"mask size: "<<maskregionsize<<endl;
        Mat maskedeigenvectors(maskregionsize, eigenvectors.cols, eigenvectors.type());


        for(int j = 0; j < eigenvectors.cols; j++){
            vector<double> data;
            for(int i = 0; i < mask.rows; i++){
                if(mask.at<uchar>(i) == 0){
                    data.push_back(eigenvectors.col(j).at<double>(i));
                }
            }
            Mat maskedeigenvector(data, true);
            maskedeigenvector = maskedeigenvector.reshape(1, data.size());
            maskedeigenvector.copyTo(maskedeigenvectors.col(j));
        }

        vector<uchar> imgdata;
        vector<double> avgdata;
        for(int i = 0; i < mask.rows; i++){
            if(mask.at<uchar>(i) == 0){
                avgdata.push_back(averageFaceRow.at<double>(i));
                imgdata.push_back(preprocessedFace.at<uchar>(i));
            }
        }
        Mat maskedimg(imgdata,true);
        Mat maskedavg(avgdata,true);
        maskedimg = maskedimg.reshape(1,1);
        maskedavg = maskedavg.reshape(1,1);
        cout<<maskedeigenvectors.type()<<" "<<maskedimg.type()<<" "<<maskedavg.type()<<endl;
        Mat projection = subspaceProject(maskedeigenvectors, maskedavg, maskedimg);


       // Mat projection = subspaceProject(eigenvectors, averageFaceRow,
                                   //preprocessedFace.reshape(1,1));
        Mat reconstructionRow = subspaceReconstruct(eigenvectors,
                                                    averageFaceRow, projection);
        reconstructionRow = getImageFrome1DFloatMat(reconstructionRow,faceHeight);
        imshow("reconstruction", reconstructionRow);

        waitKey(0);
    }

    waitKey(0);

    return 0;
}
