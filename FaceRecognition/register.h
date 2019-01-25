#ifndef _REGISTER_H_
#define _REGISTER_H_

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

class Register
{
public:
	Register() {}
	~Register() {}
	void registerFaceFromPIC(cv::Mat &image, bool update);
	void registerFaceFromCAM( bool update);
	void registerFaceFromVIDEO(char *videoName, bool update);

private:
	FaceData m_data;
	vector<FaceData> m_faceData;
};


#endif