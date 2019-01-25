#ifndef _DETECTOR_DLIB_H_
#define _DETECTOR_DLIB_H_

#include "facedata.h"
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "dlib/image_processing/frontal_face_detector.h"
#include "dlib/opencv.h"
#include "dlib/gui_widgets.h"
#include "dlib/image_processing.h"  
#include "dlib/image_processing/render_face_detections.h"
#include "elbp.h"
#include "normalizator.h"


#define STR_FACE_NFOUND "No face found in the picture\n"

#define MIN_FACE_SIZE cv::Size(100, 100)  
#define MAX_FACE_SIZE cv::Size(0, 0)

#define SCALE_FACE_PIC 1.2
#define MIN_NEIGHBOR_PIC 2

using namespace cv;
using namespace std;

extern cv::Rect detectROI;
extern Normalizator iNorm;
extern cv::Ptr<cv::ml::SVM> svmClassifier;
extern bool ifSpoofingFlag;

class Detector
{
public:
	Detector() :
		m_detectScale(SCALE_FACE_PIC),
		m_minNeighbor(MIN_NEIGHBOR_PIC),
		m_minFaceSize(MIN_FACE_SIZE),
		m_maxFaceSize(MAX_FACE_SIZE){}
	~Detector() {}

	void initializeModel(const std::string OpencvModel, const std::string DlibDat);
	void setArgumentsMan(double detectScale_, int minNeighbor_, cv::Size minFaceSize_, cv::Size maxFaceSize_);
	
	std::vector<FaceData> findFace(cv::Mat &image);
	std::vector<FaceData> fetchFace(cv::Mat &image);
	void onlyDetectFace(cv::Mat &image, bool ifSpoofingFlag);
	FaceData biggestFace(std::vector<FaceData> &faces_arr);	  // Returns rectangle with biggest surface area in array of rects


private:
	cv::CascadeClassifier face_cascade;
	dlib::shape_predictor sp;
	double m_detectScale;   //¼ì²â³ß¶È
	int m_minNeighbor;
	cv::Size m_minFaceSize;
	cv::Size m_maxFaceSize;
	FaceData m_data;
	vector<FaceData> m_faceData;
};

#endif