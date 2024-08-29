#include "opencv2/opencv.hpp"
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;
using namespace cv;

unsigned cvflannSeed = time(nullptr);

// only CV_8UC1/3/4
void saltAndPepperNoise(Mat &mat, double ratio) {
    double mask;
    unsigned channels = mat.channels();
    uchar *dataptr = mat.data;
    uchar iswhite;

    cvflann::seed_random(++cvflannSeed);
    do {
        mask = cvflann::rand_double(1.0, .0);

        if (mask < ratio) {
            iswhite = mask * 2 < ratio;

            switch (channels) {
                case 1:
                    *dataptr = iswhite ? 255 : 0;
                default:
                    break;
                case 3:
                case 4:
                    if (iswhite)
                        *(unsigned *) dataptr |= 0x00ffffff;
                    else
                        *(unsigned *) dataptr &= 0xff000000;
            }
        }

        dataptr += channels;
    } while (dataptr != mat.dataend);
}

// no alpha
void GaussianNoise(Mat &mat, double mean, double std) {
    Mat noise(mat.size(), mat.type());
    RNG().fill(noise, RNG::NORMAL, mean, std);

    mat += noise;
}

void forward(Mat &input, Mat &mat, vector<int32_t> &labelSet) {
    uint8_t *iptr = input.data,
            *ileftptr = iptr - 1,
            *itopptr = iptr - input.cols;
    uint16_t *optr = mat.ptr<uint16_t>(1, 1),
            *oleftptr = optr - 1,
            *otopptr = optr - mat.cols,
            *colend = optr + input.cols - 1;
    int32_t counter = 0;

    labelSet.push_back(0);
    do {
        if (*iptr != 0) {
            switch ((*otopptr != 0 && *itopptr == *iptr) + 2 * (*oleftptr != 0 && *ileftptr == *iptr)) {
                case 0:
                    *optr = ++counter;
                    labelSet.push_back(-counter);
                    break;
                case 1:
                    *optr = *otopptr;
                    break;
                case 2:
                    *optr = *oleftptr;
                    break;
                case 3:
                    if (*oleftptr < *otopptr) {
                        *optr = *oleftptr;
                        labelSet[*otopptr] = *optr;
                    } else if (*otopptr < *oleftptr) {
                        *optr = *otopptr;
                        labelSet[*oleftptr] = *optr;
                    } else {
                        *optr = *otopptr;
                    }
                    break;
            }
        }
        ++iptr;
        ++ileftptr;
        ++itopptr;
        if (optr == colend) {
            optr += 3;
            oleftptr += 3;
            otopptr += 3;
            colend += mat.cols;
        } else {
            ++optr;
            ++oleftptr;
            ++otopptr;
        }
    } while (iptr <= input.dataend);
}

void initlabel(vector<int32_t> &set) {
    for (int y = 1, x; y != set.size(); ++y){
        x = y;
        while(set[x] > 0)
            x = set[x];
        set[y] = set[x];
    }
    for (int & y : set)
        y = -y;
}

void backward(Mat &mat, vector<int32_t> &labelSet) {
    auto *ptr = mat.ptr<uint16_t>(1, 1);
    const uint16_t *ptrend = mat.ptr<uint16_t>(mat.rows - 2, mat.cols - 2);

    initlabel(labelSet);
    do {
        if (*ptr != 0)
            *ptr = labelSet[*ptr];
    } while (++ptr <= ptrend);
}

void twoPass(Mat &input, Mat &label) {
    Mat mat(input.rows + 2, input.cols + 2, CV_16U, Scalar(0));
    vector<int32_t> labelSet;

    forward(input, mat, labelSet);
    backward(mat, labelSet);

    mat(Rect(1, 1, input.cols, input.rows)).copyTo(label);
}

int main() {
    Mat img1 = imread("../kermode.jpg");
    Mat img2, img3, label;

    cvtColor(img1, img2, COLOR_BGR2GRAY);
    GaussianBlur(img2, img2, Size(5, 5), 0);
    threshold(img2, img2, 0, 255, THRESH_BINARY | THRESH_OTSU);
    img3 = img2.clone();
    int n;
    auto start = chrono::high_resolution_clock::now();
    twoPass(img2, label);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    label *= 255;
    imshow("label", label);

    start = chrono::high_resolution_clock::now();
    n = connectedComponents(img3, label, 4, CV_16U);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    label *= 255;
    cout << n << endl;
    imshow("label1", label);

    waitKey();
    return 0;
}