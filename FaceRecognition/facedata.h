#ifndef _FACEDATA_H_
#define _FACEDATA_H_

#include "opencv2/core/core.hpp"

//struct is being used to send data between components, notably detector > main > normalizator
struct FaceData
{
	cv::Mat image; 
	cv::Rect detectFace;
	cv::Point2f leye, reye, faceCenter;     //positions of the centers of eyes and face
	FaceData() {};
	FaceData(cv::Mat image_, cv::Rect detectFace_, cv::Point2f leye_, cv::Point2f reye_, cv::Point2f faceCenter_):
		image(image_),
		detectFace(detectFace_),
		leye(leye_),
		reye(reye_),
		faceCenter(faceCenter_)
	{
		if(leye.x > reye.x)
		{
			cv::Point swapper = leye;
			leye = reye;
			reye = swapper;
		}
	}
};

#endif