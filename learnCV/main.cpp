#include "opencv2/opencv.hpp"
#include "myalg.h"
#include <chrono>

using namespace std;
using namespace cv;


int main() {
    Mat img = imread("../bears.jpeg");
    Mat mark = Mat::zeros(img.size(), CV_32SC1);


    watershed(img, mark);
    for(int i = 0; i < mark.rows; i++)
        for(int j = 0; j < mark.cols; j++)
            if(mark.at<int>(i, j) == -1)
                img.at<Vec3b>(i, j) = Vec3b(255, 255, 255);

    imshow("img", img);
    waitKey();

    return 0;
}