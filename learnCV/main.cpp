#include "opencv2/opencv.hpp"
#include "myalg.h"
#include "timer.h"

using namespace std;
using namespace cv;

int main() {
    setLogLevel(utils::logging::LOG_LEVEL_SILENT);

    Mat img = imread("../colorful.jpeg");
    Mat Rimg, outimg;
    vector<KeyPoint> kps;
    Mat desc;
;
    cvtColor(img, img, COLOR_BGR2GRAY);
    auto orb =  ORB::create(20);
    orb->detectAndCompute(img, noArray(), kps, desc);

    cout << desc.size() << endl;

    for(auto &kp : kps) {
        cout << kp.pt << endl;
        cout << kp.angle << endl;
        cout << kp.octave << endl;
        cout << kp.size << endl << endl;
    }

    drawKeypoints(img, kps, outimg);
    imshow("img", outimg.clone());

    waitKey();
    return 0;
}
