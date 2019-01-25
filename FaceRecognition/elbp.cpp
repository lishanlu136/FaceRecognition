#include "elbp.h"

Mat histc_(const Mat& src, int minVal, int maxVal, bool normed)
{
	Mat result;
	// Establish the number of bins.
	int histSize = maxVal - minVal + 1;
	// Set the ranges.
	float range[] = { static_cast<float>(minVal), static_cast<float>(maxVal + 1) };
	const float* histRange = { range };
	// calc histogram
	calcHist(&src, 1, 0, Mat(), result, 1, &histSize, &histRange, true, false);
	// normalize
	if (normed) {
		result /= (int)src.total();
	}
	return result.reshape(1, 1);
}

Mat histc(Mat &src, int minVal, int maxVal, bool normed)
{
	switch (src.type()) {
	case CV_8SC1:
		return histc_(Mat_<float>(src), minVal, maxVal, normed);
		break;
	case CV_8UC1:
		return histc_(src, minVal, maxVal, normed);
		break;
	case CV_16SC1:
		return histc_(Mat_<float>(src), minVal, maxVal, normed);
		break;
	case CV_16UC1:
		return histc_(src, minVal, maxVal, normed);
		break;
	case CV_32SC1:
		return histc_(Mat_<float>(src), minVal, maxVal, normed);
		break;
	case CV_32FC1:
		return histc_(src, minVal, maxVal, normed);
		break;
	default:
		CV_Error(Error::StsUnmatchedFormats, "This type is not implemented yet."); break;
	}
	return Mat();
}


Mat spatial_histogram(Mat &src, int numPatterns,
	int grid_x, int grid_y, bool /*normed*/)
{
	// calculate LBP patch size
	int width = src.cols / grid_x;
	int height = src.rows / grid_y;
	// allocate memory for the spatial histogram
	Mat result = Mat::zeros(grid_x * grid_y, numPatterns, CV_32FC1);
	// return matrix with zeros if no data was given
	if (src.empty())
		return result.reshape(1, 1);
	// initial result_row
	int resultRowIdx = 0;
	// iterate through grid
	for (int i = 0; i < grid_y; i++) {
		for (int j = 0; j < grid_x; j++) {
			Mat src_cell = Mat(src, Range(i*height, (i + 1)*height), Range(j*width, (j + 1)*width));
			Mat cell_hist = histc(src_cell, 0, (numPatterns - 1), true);
			// copy to the result matrix
			Mat result_row = result.row(resultRowIdx);
			cell_hist.reshape(1, 1).convertTo(result_row, CV_32FC1);
			// increase row count in result matrix
			resultRowIdx++;
		}
	}
	// return result as reshaped feature vector
	return result.reshape(1, 1);
}


Mat my_elbp(Mat &src, int radius, int neighbors) 
{
	Mat dst;
	// allocate memory for result
	dst.create(src.rows - 2 * radius, src.cols - 2 * radius, CV_32SC1);
	// zero
	dst.setTo(0);
	for (int n = 0; n<neighbors; n++) 
	{
		// sample points
		float x = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));
		float y = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
		// relative indices
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));
		// fractional part
		float ty = y - fy;
		float tx = x - fx;
		// set interpolation weights
		float w1 = (1 - tx) * (1 - ty);
		float w2 = tx  * (1 - ty);
		float w3 = (1 - tx) *      ty;
		float w4 = tx  *      ty;
		// iterate through your data
		for (int i = radius; i < src.rows - radius; i++) 
		{
			for (int j = radius; j < src.cols - radius; j++) 
			{
				// calculate interpolated value
				float t = static_cast<float>(w1*src.at<unsigned char>(i + fy, j + fx) + w2*src.at<unsigned char>(i + fy, j + cx) + w3*src.at<unsigned char>(i + cy, j + fx) + w4*src.at<unsigned char>(i + cy, j + cx));
				// floating point precision, so check some machine-dependent epsilon
				dst.at<int>(i - radius, j - radius) += ((t > src.at<unsigned char>(i, j)) || (std::abs(t - src.at<unsigned char>(i, j)) < std::numeric_limits<float>::epsilon())) << n;
			}
		}
	}
	return dst;
}