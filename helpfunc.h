#ifndef HELPFUNC_H
#define HELPFUNC_H
#include <string>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <vector>

std::string fixedLength(int value, int digits = 3);
cv::Mat getImageFrome1DFloatMat(const cv::Mat matrixRow, int height);

void drawPoints(cv::Mat& src, const std::vector<cv::Point>& pset);
int getmaskedsize(const cv::Mat& mask);
#endif // HELPFUNC_H
