#include "opencv2/ximgproc.hpp"
#include "myalg.h"
#include <chrono>

using namespace std;
using namespace cv;


int main() {
    Mat img = imread("../kermode.jpg");
    Mat img1, img2;

    cvtColor(img, img, COLOR_BGR2GRAY);
    GaussianBlur(img, img, Size(5, 5), 0);
    threshold(img, img, 0, 255, THRESH_BINARY | THRESH_OTSU);

    imshow("img", img);

    Mat mask = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    cout << (mask.type() == CV_8UC1) << endl;
    

    return 0;
}