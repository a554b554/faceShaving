#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    Mat g;
    g = imread("pic00001.jpg");
    imshow("g",g);
    waitKey(0);
    return 0;
}

