#include "opencv2/ximgproc.hpp"
#include "myalg.h"
#include <chrono>

using namespace std;
using namespace cv;


int main() {
    Mat img = imread("../kermode.jpg");
    Mat img1, img2;

    imshow("img", img);
    waitKey();
    return 0;
}