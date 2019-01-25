#ifndef _IDENTIFIER_H_
#define _IDENTIFIER_H_
#define I_THRESHOLD 80

#include "detector_dlib.h"
#include "normalizator.h"
#include "facedata.h"
#include "common.h"
#include "elbp.h"

extern Rect detectROI;
extern Detector det;
extern cv::Ptr<cv::FaceRecognizer> lbpModel;
extern Normalizator iNorm;
extern cv::Ptr<cv::ml::SVM> svmClassifier;
extern bool ifSpoofingFlag;

class Identifier
{
public:
	Identifier()  {}
	~Identifier() {}
	void identificationFaceFromPIC(cv::Mat &image);
	void identificationFaceFromCAM();
	void identificationFaceFromVIDEO(char *videoName);

private:
	FaceData m_data;
	vector<FaceData> m_faceData;
};

#endif