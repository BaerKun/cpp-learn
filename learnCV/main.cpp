#include "opencv2/opencv.hpp"
#include <ctime>

using namespace std;
using namespace cv;

unsigned cvflannSeed = time(nullptr);

// only CV_8UC1/3/4
void saltAndPepperNoise(Mat &mat, double ratio){
    double mask;
    unsigned channels = mat.channels();
    uchar *dataptr = mat.data;
    uchar iswhite;

    cvflann::seed_random(++cvflannSeed);
    do {
        mask = cvflann::rand_double(1.0, .0);

        if(mask < ratio) {
            iswhite = mask * 2 < ratio;

            switch(channels){
                case 1:
                    *dataptr = iswhite ? 255 : 0;
                default:
                    break;
                case 3:
                case 4:
                    if(iswhite)
                        *(unsigned *)dataptr |= 0x00ffffff;
                    else
                        *(unsigned *)dataptr &= 0xff000000;
            }
        }

        dataptr += channels;
    } while(dataptr != mat.dataend);
}

// no alpha
void GaussianNoise(Mat &mat, double mean, double std){
    Mat noise(mat.size(), mat.type());
    RNG().fill(noise, RNG::NORMAL, mean, std);

    mat += noise;
}

int main(){
    Mat img1 = imread("../kermode.jpg");
    Mat img2;

    GaussianBlur(img1, img1, Size(-3, 3), 5, -10);
    imshow("img", img1);

    waitKey();

    return 0;
}