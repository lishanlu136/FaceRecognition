#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/core/persistence.hpp"

using namespace cv;
Mat histc_(const Mat& src, int minVal = 0, int maxVal = 255, bool normed = false);
Mat histc(Mat &src, int minVal, int maxVal, bool normed);
Mat spatial_histogram(Mat &src, int numPatterns, int grid_x, int grid_y, bool /*normed*/);
Mat my_elbp(Mat &src, int radius, int neighbors);
