#include "helpfunc.h"

std::string fixedLength(int value, int digits) {
    unsigned int uvalue = value;
    if (value < 0) {
        uvalue = -uvalue;
    }
    std::string result;
    while (digits-- > 0) {
        result += ('0' + uvalue % 10);
        uvalue /= 10;
    }
    if (value < 0) {
        result += '-';
    }
    std::reverse(result.begin(), result.end());
    return result;
}

cv::Mat getImageFrome1DFloatMat(const cv::Mat matrixRow, int height){
    cv::Mat rectangularMat = matrixRow.reshape(1, height);
    cv::Mat dst;
    cv::normalize(rectangularMat, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    return dst;
}
