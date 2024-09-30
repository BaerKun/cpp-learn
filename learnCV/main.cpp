#include "opencv2/opencv.hpp"
#include "myalg.h"
#include "timer.h"

using namespace std;
using namespace cv;

int main() {
    setLogLevel(utils::logging::LOG_LEVEL_SILENT);

    VideoCapture cap(R"(C:\Users\B3220\Desktop\jjj.mp4)");

    meanShiftTrack(cap);

    return 0;
}
