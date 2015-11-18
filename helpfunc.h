#ifndef HELPFUNC_H
#define HELPFUNC_H
#include <string>
#include <algorithm>
#include <opencv2/core/core.hpp>

std::string fixedLength(int value, int digits = 3);
cv::Mat getImageFrome1DFloatMat(const cv::Mat matrixRow, int height);



#endif // HELPFUNC_H
