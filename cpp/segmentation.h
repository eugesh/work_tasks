#ifndef __SEGMENTATION_H__
#define __SEGMENTATION_H__

// OpenCV
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
// #include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
// #include "opencv2/objdetect.hpp"
// #include "opencv2/videoio.hpp"

typedef double real;

std::string type2str(int type) {
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

/**
 * Binarization using Otsu's threshold.
 */
real binarization_Otsu(cv::Mat img_bw, cv::Mat const& img) {
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_RGBA2GRAY);

    // Smooth edges.
    cv::Mat blur;
    cv::GaussianBlur(img_gray, blur, cv::Size(25, 25), 0);

    // Binarize image with Otsu's threshold.
    return cv::threshold(blur, img_bw, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
}

/**
 * Binarization by Adaptive Threshold.
 */
void binarization_adaptive(cv::Mat img_bw, cv::Mat img) {
    cv::Mat img_gray = img.clone();
    printf("img.type() = %s\n", type2str(img.type()).c_str());
    printf("img size = %d %d %d\n", img.rows, img.cols, img.depth());
    cv::imshow ("img", img);
    if(cv::waitKey (3000) >= 0)
        {;}
    cv::cvtColor(img, img_gray, CV_BGR2GRAY);

    printf("cvtColor - ok\n");
    printf("img_gray size = %d %d %d\n", img_gray.rows, img_gray.cols, img_gray.depth());
    printf("img_gray.type() = %s\n", type2str(img_gray.type()).c_str());
    // Smooth edges.
    cv::Mat blur = img_gray;
    cv::imshow ("img_gray", img_gray);
    if(cv::waitKey (3000) >= 0)
        {;}
    cv::GaussianBlur(img_gray, blur, cv::Size(25, 25), 0);

    printf("GaussianBlur - ok\n");

    cv::adaptiveThreshold(blur, img_bw, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                          cv::THRESH_BINARY, 113, 1);

    printf("adaptiveThreshold - ok\n");
}

#endif
