#include "opencv2/ximgproc.hpp"
#include "myalg.h"
#include <chrono>

using namespace std;
using namespace cv;


int main() {
    Mat img = imread("../bears.jpeg");
    Mat img1, img2, c;
    vector<Mat> contours;
    cvtColor(img, img1, COLOR_BGR2GRAY);
    GaussianBlur(img1, img2, Size(3, 3), 0);
    threshold(img2, img2, 0, 255, THRESH_BINARY | THRESH_OTSU);

    findContours(img2, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    approxPolyDP(contours[0], c, 5, true);

    cout << c.at<Point>(0) << endl << c.at<Point>(c.rows - 1) << endl;
    return 0;
}