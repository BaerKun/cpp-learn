#include "myalg.h"

using namespace std;
using namespace cv;

unsigned cvflannSeed = time(nullptr);

static inline int getlabel(vector<int> &set, const int32_t x, const int32_t y) {
    if (x < y) {
        set[y] = x;
        return x;
    }
    if (x > y)
        set[x] = y;
    return y;
}

static void firstPass4(const Mat &input, Mat &mat, vector<int32_t> &labelSet) {
    const uint8_t *iptr = input.data,
            *ileftptr = iptr - 1,
            *itopptr = iptr - input.cols;
    auto *optr = mat.ptr<int32_t>(1, 1);
    const int32_t
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
                default:
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

static void firstPass8(const Mat &input, Mat &mat, vector<int32_t> &labelSet) {
    const uint8_t *iptr = input.data,
            *ileftptr = iptr - 1,
            *itopptr = iptr - input.cols,
            *ilefttopptr = itopptr - 1,
            *irighttopptr = itopptr + 1;
    auto *optr = mat.ptr<int32_t>(1, 1);
    const int32_t
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
                default:
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
    while (set[y] > 0)
        y = set[y];

    if (set[y] == 0)
        set[y] = -++counter;

    const int z = set[y];
    while (x != z) {
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

    const int count = initlabel(labelSet);
    do {
        if (*ptr != 0)
            *ptr = labelSet[*ptr];
    } while (++ptr <= ptrend);

    return count;
}

// only CV_8UC1/3/4
void saltAndPepperNoise(const Mat &mat, const double ratio) {
    double mask;
    const int channels = mat.channels();
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
                        memset(dataptr, 0xff, 3);
                    else
                        memset(dataptr, 0, 3);
            }
        }

        dataptr += channels;
    } while (dataptr != mat.dataend);
}

// no alpha
void GaussianNoise(Mat &mat, const double mean, const double std) {
    Mat noise(mat.size(), mat.type());
    RNG().fill(noise, RNG::NORMAL, mean, std);

    mat += noise;
}

int twoPass(const Mat &image, Mat &label, const int connectivity, const int labelType) {
    Mat mat = Mat::zeros(image.rows + 2, image.cols + 2, CV_32S);
    vector<int32_t> labelSet;

    if (connectivity == 4)
        firstPass4(image, mat, labelSet);
    else
        firstPass8(image, mat, labelSet);
    const int count = secondPass(mat, labelSet);

    mat(Rect(1, 1, image.cols, image.rows)).convertTo(label, labelType);
    return count;
}

void hist2img1D(const Mat &hist, Mat &img, const int width, const int height) {
    img = Mat::zeros(height, width, CV_8U);
    const int npoints = hist.rows;
    float x = 0.f;
    const float deltaX = (float) width / npoints,
            *pVal = hist.ptr<float>();
    Point lefttop(0, 0);
    Point rightbottom(0, 0);
    const Scalar color(255);

    for (int p = 0; p < npoints; ++p) {
        lefttop.x = x;
        rightbottom.x = x += deltaX;
        rightbottom.y = height * *pVal++;
        rectangle(img, lefttop, rightbottom, color, -1);
    }
    flip(img, img, 0);
}

void hist2img2D(const Mat &hist, Mat &img, const int width, const int height, const double brigtnessHance) {
    img = Mat::ones(height, width, CV_8U);
    const int npointsX = hist.cols,
            npointsY = hist.rows;
    float x, y = 0.f;
    const float
            deltaX = (float) width / npointsX,
            deltaY = (float) height / npointsY,
            *pHistVal = hist.ptr<float>(0, 0);
    Point lefttop(0, 0), rightbottom(0, 0);
    Scalar color(255.0);

    int r, c;
    for (r = 0; r < npointsY; ++r) {
        x = 0.f;
        lefttop.y = y;
        rightbottom.y = y += deltaY;
        for (c = 0; c < npointsX; ++c) {
            lefttop.x = x;
            rightbottom.x = x += deltaX;
            *color.val = *pHistVal++ * 255.0 * brigtnessHance;
            rectangle(img, lefttop, rightbottom, color, -1);
        }
    }
    flip(img, img, 0);
}

void histMatch(const Mat &src, const Mat &hist, Mat &dst) {
    Mat srcHist;
    constexpr int channels = 0, histSize = 256;
    constexpr float range[] = {0.f, 256.f};
    const float *ranges[] = {range};

    calcHist(&src, 1, &channels, noArray(), srcHist, 1, &histSize, ranges);
    normalize(srcHist, srcHist, 1.0, 0.0, NORM_L1);

    Mat lut(1, 256, CV_8UC1);
    auto *pLutVal = lut.ptr<uchar>(0);
    const float
            *pSrcVal = srcHist.ptr<float>(0),
            *pTarVal = hist.ptr<float>(0);
    float
            mapVal = 0.f,
            ssum = *pSrcVal,
            dsum = *pTarVal;

    for (int i = 0; i < 256;) {
        if (ssum > dsum) {
            dsum += *++pTarVal;
            mapVal += 1.f;
            if (ssum > dsum + *(pTarVal + 1))
                continue;
        }
        *pLutVal++ = mapVal;
        ssum += *++pSrcVal;
        ++i;
    }

    LUT(src, lut, dst);
}

void getObjectRectAndHist(const Mat &frame, Rect &objectRect, Mat &hist, const int *channels,
                      const int *histSize, const float **ranges) {
    Mat objectGray;
    const Rect objectRectGot = selectROI("meanShift", frame);
    if(objectRectGot.empty())
        return;

    objectRect = objectRectGot;
    cvtColor(frame(objectRectGot), objectGray, COLOR_BGR2GRAY);
    calcHist(&objectGray, 1, channels, noArray(), hist, 1, histSize, ranges);
    normalize(hist, hist, 0, 255, NORM_MINMAX);
}

void meanShiftTrack(VideoCapture &cap) {
    Mat frame, frameGray, objectHist, backProj;
    constexpr int histSize[] = {128};
    constexpr float range[] = {0, 256};
    const float *ranges[] = {range};
    Rect objectRect;

    cap.read(frame);
    getObjectRectAndHist(frame, objectRect, objectHist, nullptr, histSize, ranges);

    while (cap.read(frame)) {
        cvtColor(frame, frameGray, COLOR_BGR2GRAY);
        calcBackProject(&frameGray, 1, nullptr, objectHist, backProj, ranges);

        meanShift(backProj, objectRect,
                  TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

        rectangle(frame, objectRect, Scalar(255, 0, 0));
        imshow("meanShift", frame);

        const int c = waitKey(20);
        if (c == 27)
            return;
        if (c == ' ')
            getObjectRectAndHist(frame, objectRect, objectHist, nullptr, histSize, ranges);
    }

    waitKey();
}
