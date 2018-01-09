#ifndef __USE_OCV_CONTOUR_ANALYSIS_H__
#define __USE_OCV_CONTOUR_ANALYSIS_H__

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
// OpenCV
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
// #include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
// #include "opencv2/objdetect.hpp"
// #include "opencv2/videoio.hpp"

typedef std::vector<cv::Point2f> cvContour;
typedef double real;

/**
 * Calculates Euclidean distance.
 */
real euclid_dist(cv::Point point) {
    return sqrt(real(point.x * point.x) + real(point.y * point.y));
}

real euclid_dist(cv::Point2f point) {
    return sqrt(point.x * point.x + point.y * point.y);
}

/**
 * Dot product.
 */
double dot_product(cv::Point2f vec1, cv::Point2f vec2) {
    return double(vec1.x * vec2.x + vec1.y * vec2.y);
}

/**
 *
 */
real distance_between_contours(cvContour const& cntA, cvContour const& cntB) {
    real distance = 0.0;


    return distance;
}

/**
 * Center of rectangle by diagonal intersection.
 */
cv::Point2f center_by_diagonals_intersection(cvContour const& cnt) {
    real x1 = cnt[0].x;
    real y1 = cnt[0].y;
    real x3 = cnt[1].x;
    real y3 = cnt[1].y;
    real x2 = cnt[2].x;
    real y2 = cnt[2].y;
    real x4 = cnt[3].x;
    real y4 = cnt[3].y;
    real Cx = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
              ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    real Cy = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
              ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    return cv::Point2f(Cx, Cy);
}


/*cv::Point2f center_by_average(cvContour const& cnt) {
    return np.average(cnt[0, :]), np.average(cnt[1, :]);
}*/

/**
 * Center of mass by moments.
 */
cv::Point2f center_by_moments(cvContour const& cnt) {
    cv::Moments M = cv::moments(cnt);
    real Cx = int(M.m10 / M.m00);
    real Cy = int(M.m01 / M.m00);
    return cv::Point2f(Cx, Cy);
}

/**
 * Removes too small objects.
 */
void filter_by_area(std::vector<cvContour> & filtered_contours,
                    std::vector<cvContour> const& contours,
                    double area_thresh = 10000) {
    for(unsigned int i = 0; i < contours.size(); ++i) {
        if (cv::contourArea(contours[i]) > area_thresh) {
            filtered_contours.push_back(contours[i]);
        }
    }
}

/**
 *
 * Takes vector of contours and returns vector of rectangular contours.
 *
 * \param[out] rect_contours - vector of rectangular contours;
 * \param[in] contours - vector of contours;
 * \param[in] sum_of_cos - threshold for sum of cosines of angles of contour with 4 corners.
 *                         It is maximum value of sum of corners' angles cosines
 *                         to consider contour as rectangular.
 *
 * \returns ids of rectangular contours.
 *
 */
std::vector<unsigned int> find_rectangles(std::vector<cvContour> & rect_contours, std::vector<cvContour> const& contours, float sum_of_cos = 0.4) {
    std::vector<unsigned int> rect_ids;

    // Approximation (redundancy removal).
    for(unsigned int i = 0; i < contours.size(); ++i) {
        double epsilon = 0.1 * cv::arcLength(contours[i], true);
        cvContour cnt_apprx;
        cv::approxPolyDP(contours[i], cnt_apprx, epsilon, true);
        // Check if contours has 4 corners.
        if (cnt_apprx.size() == 4) {
            // Check for corners' orthogonality. Find sum of cosines.
            double sum_of_cos = 0;
            for (unsigned int j = 0; j < 4; ++j) {
                cv::Point2f vec1 = cnt_apprx[j - 1] - cnt_apprx[j];
                cv::Point2f vec2 = cnt_apprx[j] - cnt_apprx[(j + 1) % 4];
                double norm1 = euclid_dist(vec1);
                double norm2 = euclid_dist(vec2);
                double dot_cos = dot_product(vec1, vec2) / (norm1 * norm2);
                sum_of_cos += fabs(dot_cos);
            }
            if (sum_of_cos < sum_of_cos) {
                rect_contours.push_back(cnt_apprx);
                rect_ids.push_back(i);
            }
        }

    }

    return rect_ids;
}

#endif
