#include <cstdio>
#include <iostream>
#include "segmentation.h"
#include "ocv_contour_analysis.h"

std::string img_path = "./../data/opencv_test3.bmp";

void plate_measurer() {
    // Read image.
	cv::Mat img_orig = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);

	printf("imread - ok\n");

	// Binarization.
	cv::Mat img_bw;
	binarization_adaptive(img_bw, img_orig);

	printf("binarization_adaptive - ok\n");

	// Find contours.
	std::vector<cvContour> contours;
	std::vector <cv::Vec4i> hierarchy;
	cv::findContours(img_bw, contours, hierarchy, cv::RETR_TREE,
	                 cv::CHAIN_APPROX_NONE, cv::Point(0,0));

	printf("findContours - ok\n");

	// Remove too small objects.
	std::vector<cvContour> filtered_contours;
	filter_by_area(filtered_contours, contours);

	printf("filter_by_area - ok\n");

	// Find rectangles.
	std::vector<unsigned int> ids;
	std::vector<cvContour> rect_contours;
	ids = find_rectangles(rect_contours, filtered_contours);

	printf("find_rectangles - ok\n");

	// Highlite rectangles.
	// cv::drawContours(img_orig, rect_contours, -1, (0, 0, 255), 3);
	cv::drawContours(img_orig, rect_contours, -1, 255, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
}

int main(int argc, char **argv)
{

	plate_measurer();

    return 0;
}
