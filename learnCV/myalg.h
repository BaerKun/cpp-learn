#ifndef LEARNCV_MYALG_H
#define LEARNCV_MYALG_H

#include "opencv2/opencv.hpp"

// only CV_8UC1/3/4
void saltAndPepperNoise(const cv::Mat &mat, double ratio);

// no alpha
void GaussianNoise(cv::Mat &mat, double mean, double std);

int twoPass(const cv::Mat &image, cv::Mat &label, int connectivity = 8, int labelType = CV_32S);

void hist2img1D(const cv::Mat &hist, cv::Mat &img, int width, int height);

void hist2img2D(const cv::Mat &hist, cv::Mat &img, int width, int height, double brigtnessHance=10.);

void histMatch(const cv::Mat &src, const cv::Mat &hist, cv::Mat &dst);

void meanShiftTrack(cv::VideoCapture &cap);

#endif //LEARNCV_MYALG_H
