#include "opencv2/opencv.hpp"
#include "myalg.h"
#include "timer.h"

using namespace std;
using namespace cv;


int main() {
    Mat img = imread("../colorful.jpeg");
    Mat Rimg, outimg = img.clone();
    vector<KeyPoint> kps;

    cvtColor(img, img, COLOR_BGR2GRAY);
    normalize(img, img, 0, 255, NORM_MINMAX);
    cornerHarris(img, Rimg, 3, 3, 0.02);

    for (int i = 0; i < Rimg.rows; i++) {
        for (int j = 0; j < Rimg.cols; j++) {
            if (Rimg.at<float>(i, j) > 0.005) {
                kps.emplace_back(j, i, 3);
            }
        }
    }
    drawKeypoints(img, kps, outimg, Scalar::all(-1), DrawMatchesFlags::DRAW_OVER_OUTIMG);


    imshow("img", outimg);
    waitKey();
    return 0;
}
