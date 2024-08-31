#include "myalg.h"

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

static inline int getlabel(vector<int> &set, int32_t x, int32_t y) {
    if (x < y) {
        set[y] = x;
        return x;
    }
    if (x > y)
        set[x] = y;
    return y;
}

static void firstPass4(Mat &input, Mat &mat, vector<int32_t> &labelSet) {
    uint8_t *iptr = input.data,
            *ileftptr = iptr - 1,
            *itopptr = iptr - input.cols;
    int32_t *optr = mat.ptr<int32_t>(1, 1),
            *oleftptr = optr - 1,
            *otopptr = optr - mat.cols,
            *colend = optr + input.cols - 1;
    int32_t counter = 0;

    labelSet.push_back(0);
    do {
        if (*iptr != 0) {
            switch ((*oleftptr != 0 && *ileftptr == *iptr) + 2 * (*otopptr != 0 && *itopptr == *iptr)) {
                case 0:
                    *optr = ++counter;
                    labelSet.push_back(0);
                    break;
                case 3:
                    *optr = getlabel(labelSet, *oleftptr, *otopptr);
                    break;
                case 1:
                    *optr = *oleftptr;
                    break;
                case 2:
                    *optr = *otopptr;
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

static void firstPass8(Mat &input, Mat &mat, vector<int32_t> &labelSet) {
    uint8_t *iptr = input.data,
            *ileftptr = iptr - 1,
            *itopptr = iptr - input.cols,
            *ilefttopptr = itopptr - 1,
            *irighttopptr = itopptr + 1;
    int32_t *optr = mat.ptr<int32_t>(1, 1),
            *oleftptr = optr - 1,
            *otopptr = optr - mat.cols,
            *olefttopptr = otopptr - 1,
            *orighttopptr = otopptr + 1,
            *colend = optr + input.cols - 1;
    int32_t counter = 0;

    labelSet.push_back(0);
    do {
        if (*iptr != 0) {
            switch ((*oleftptr != 0 && *ileftptr == *iptr) | 2 * (*olefttopptr != 0 && *ilefttopptr == *iptr)
                    | 4 * (*otopptr != 0 && *itopptr == *iptr) | 8 * (*orighttopptr != 0 && *irighttopptr == *iptr)) {
                case 0:
                    *optr = ++counter;
                    labelSet.push_back(0);
                    break;
                case 15:
                case 9:
                case 11:
                case 13:
                    *optr = getlabel(labelSet, *oleftptr, *orighttopptr);
                    break;
                case 7:
                case 5:
                    *optr = getlabel(labelSet, *oleftptr, *otopptr);
                    break;
                case 1:
                case 3:
                    *optr = *oleftptr;
                    break;
                case 4:
                case 6:
                    *optr = *otopptr;
                    break;
                case 8:
                case 12:
                case 14:
                    *optr = *orighttopptr;
                    break;
                case 2:
                    *optr = *olefttopptr;
                    break;
                case 10:
                    *optr = getlabel(labelSet, *olefttopptr, *orighttopptr);
                    break;
            }
        }
        ++iptr;
        ++ileftptr;
        ++itopptr;
        ++ilefttopptr;
        ++irighttopptr;
        if (optr == colend) {
            optr += 3;
            oleftptr += 3;
            otopptr += 3;
            olefttopptr += 3;
            orighttopptr += 3;
            colend += mat.cols;
        } else {
            ++optr;
            ++oleftptr;
            ++otopptr;
            ++olefttopptr;
            ++orighttopptr;
        }
    } while (iptr <= input.dataend);
}

static void find(vector<int32_t> &set, int32_t x, int32_t &counter) {
    int y = x;
    while(set[y] > 0)
        y = set[y];

    if(set[y] == 0)
        set[y] = -++counter;

    int z = set[y];
    while(x != z){
        y = set[x];
        set[x] = z;
        x = y;
    }

}

static int32_t initlabel(vector<int32_t> &set) {
    int32_t counter = 0, size = (int32_t) set.size();
    for (int y = 1; y != size; ++y)
        find(set, y, counter);

    for (int &y: set)
        y = -y;
    return counter;
}

static int32_t secondPass(Mat &mat, vector<int32_t> &labelSet) {
    auto *ptr = mat.ptr<uint16_t>(1, 1);
    const uint16_t *ptrend = mat.ptr<uint16_t>(mat.rows - 2, mat.cols - 2);

    int count = initlabel(labelSet);
    do {
        if (*ptr != 0)
            *ptr = labelSet[*ptr];
    } while (++ptr <= ptrend);

    return count;
}

int twoPass(Mat &image, Mat &label, int connectivity, int labelType) {
    Mat mat = Mat::zeros(image.rows + 2, image.cols + 2, CV_32S);
    vector<int32_t> labelSet;

    if (connectivity == 4)
        firstPass4(image, mat, labelSet);
    else
        firstPass8(image, mat, labelSet);
    int count = secondPass(mat, labelSet);

    mat(Rect(1, 1, image.cols, image.rows)).convertTo(label, labelType);
    return count;
}