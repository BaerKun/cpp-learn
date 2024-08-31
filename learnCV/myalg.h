#ifndef LEARNCV_MYALG_H
#define LEARNCV_MYALG_H

#include "opencv2/opencv.hpp"

// only CV_8UC1/3/4
void saltAndPepperNoise(cv::Mat &mat, double ratio);

// no alpha
void GaussianNoise(cv::Mat &mat, double mean, double std);

int twoPass(cv::Mat &image, cv::Mat &label, int connectivity = 8, int labelType = CV_32S);

#endif //LEARNCV_MYALG_H
