#ifndef _NORMALIZATOR_H_
#define _NORMALIZATOR_H_

#include "facedata.h"
#include <algorithm>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define STR_NORMALIZATION_SUCCESS "Face found!"
#define NORMALIZED_WIDTH 96
#define NORMALIZED_HEIGHT 96

class Normalizator
{
public:
	cv::Mat normalize(FaceData &data);
};

#endif