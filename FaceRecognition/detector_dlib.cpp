#include "detector_dlib.h"

//初始化模型，加载模型文件
void Detector::initializeModel(const std::string OpencvModel, const std::string DlibDat)
{
	face_cascade.load(OpencvModel);
	dlib::deserialize(DlibDat) >> sp;
	if(face_cascade.empty()||!(sp.num_parts()))
	{ 
		printf("--(!)Error loading\n"); 
	}
}

//additional function allows us to change the arguments manually
void Detector::setArgumentsMan(double detectScale_, int minNeighbor_, cv::Size minFaceSize_, cv::Size maxFaceSize_)
{
	m_detectScale = detectScale_;
	m_minNeighbor = minNeighbor_;
	m_minFaceSize = minFaceSize_;
	m_maxFaceSize = maxFaceSize_;
}

//finds faces and keypoints in the given gray image 
std::vector<FaceData> Detector::findFace(cv::Mat &grayImage)
{
	int eyesPoints[12] = {36,37,38,39,40,41,42,43,44,45,46,47};   //眼睛关键点位置

	std::vector<cv::Rect> faces;
	std::vector<FaceData> detectFaceData;
	CV_Assert(grayImage.type()==CV_8UC1);
	cv::equalizeHist( grayImage, grayImage );
	//-- 人脸检测
	double t = (double)cv::getTickCount();
	face_cascade.detectMultiScale( grayImage, faces, m_detectScale, m_minNeighbor, 0, m_minFaceSize,m_maxFaceSize);
	t = ((double)cv::getTickCount() - t)/cv::getTickFrequency()*1000;
	printf("detecte face time is %.4f ms\n",t);
	
	double t1 = (double)cv::getTickCount();
	size_t nFaces = faces.size();
	for( size_t i = 0; i < nFaces; ++i)
	{
		dlib::rectangle recFace((long)faces[i].x,(long)faces[i].y,(long)(faces[i].x+faces[i].width),(long)(faces[i].y+faces[i].height));  //检测到的人脸矩形框
		//dlib::cv_image<dlib::bgr_pixel> cimg(image);             //以彩色图载入
		dlib::cv_image<unsigned char> cimg(grayImage);                //以灰度图载入
		dlib::full_object_detection shape = sp(cimg, recFace);
		t1 = ((double)cv::getTickCount() - t1)/cv::getTickFrequency()*1000;
		printf("%d faces were detected, %d face detecte keypoints time is %.4f ms\n",faces.size(),i+1,t1);

		cv::Point2f faceCenter;
		cv::Point2f nose;            
		std::pair <cv::Point2f, cv::Point2f> eyes;
		float leftEyex = 0.0, rightEyex = 0.0;
		float leftEyey = 0.0, rightEyey = 0.0;
		for (int j = 0; j < 6; ++j)
		{
			leftEyex += shape.part(eyesPoints[j]).x();
			leftEyey += shape.part(eyesPoints[j]).y();
			rightEyex += shape.part(eyesPoints[6+j]).x();
			rightEyey += shape.part(eyesPoints[6+j]).y();
		}
		eyes.first.x = leftEyex / 6;    //左眼
		eyes.first.y = leftEyey / 6;
		eyes.second.x = rightEyex / 6;   //右眼
		eyes.second.y = rightEyey / 6;

		nose.x = shape.part(30).x();          // 鼻子(鼻尖）
		nose.y = shape.part(30).y();

		//人脸中心点为检测到的人脸中心和鼻子之和的平均值
		faceCenter.x = (faces[i].x + 0.5 * faces[i].width + nose.x) / 2;
		faceCenter.y = (faces[i].y + 0.5 * faces[i].height + nose.y) / 2;
		detectFaceData.push_back(FaceData(grayImage, faces[i], eyes.first, eyes.second, faceCenter));
	}
	return detectFaceData;
}


std::vector<FaceData> Detector::fetchFace(cv::Mat &image)
{
	int angles[] = {0, -35, 35, 70, 70};  //the more angles, the more drastically slow the program
	//appears to be, thus we only chose those angles, not the whole 360 - degrees circle
	std::vector<FaceData> data;
	for(int i = 0; i < sizeof(angles) / sizeof(angles[0]); ++i)
	{
		cv::Mat trans = cv::getRotationMatrix2D(cv::Point2f(image.cols / 2, image.rows / 2), angles[i], 1.0);
		cv::Mat temp;
		cv::warpAffine(image, temp, trans, cv::Size(image.cols, image.rows));
		try
		{
			data = findFace(temp);
		}
		catch(std::exception& e)
		{
			//we do not want to stop looking after a single rotation found no face
			if(!strcmp(e.what(), STR_FACE_NFOUND))
				continue;
			throw std::runtime_error(e.what());
		}
		return data;
	}
	throw std::runtime_error(STR_FACE_NFOUND); 
}


// **********  在不注册和不识别的情况下，只是检测人脸 ************
void Detector::onlyDetectFace(cv::Mat &image, bool ifSpoofingFlag)
{
	cv::Mat imgRoi = image(detectROI);
	cv::rectangle(image, detectROI, Scalar(0, 0, 255));
	//std::vector<cv::Rect> detFaces;
	cv::Mat grayImgRoi;
	switch (imgRoi.type())
	{
	case 0:
		imgRoi.copyTo(grayImgRoi);
		break;
	case 16:
		cv::cvtColor(imgRoi, grayImgRoi, CV_BGR2GRAY);
		break;
	default:
		printf("Image type error, input image must be gray or rgb!");
		break;
	}
	//double t = (double)cv::getTickCount();
	//face_cascade.detectMultiScale( grayImgRoi, detFaces, m_detectScale, m_minNeighbor, 0, m_minFaceSize, m_maxFaceSize);
	//size_t num = detFaces.size();
	//if (num != 0)
	//{
	//	for (size_t i=0; i<num; ++i)
	//	{
	//		cv::rectangle(image, Rect(detFaces[i].x+detectROI.x, detFaces[i].y+detectROI.y, detFaces[i].width, detFaces[i].height), cv::Scalar(0, 255, 0), 2);     //画出检测到的人脸
	//	}
	//}
	m_data.image = grayImgRoi;
	m_faceData = findFace(m_data.image);
	//t = ((double)cv::getTickCount() - t)/cv::getTickFrequency()*1000;
	//printf("detecte face time is %.4f ms\n",t);
	size_t num_fD = m_faceData.size();
	if (num_fD != 0)
	{
		for (size_t i = 0; i<num_fD; ++i)
		{
			if (!ifSpoofingFlag || svmClassifier.empty())    // 不需要判断活体人脸
			{
				printf("no anti-spoofing function.\n");
			}
			else
			{
				cv::Mat normalizedImg = iNorm.normalize(m_faceData[i]);
				Mat lbp_image = my_elbp(normalizedImg, 1, 8);
				// get spatial histogram from this lbp image
				Mat featureVector = spatial_histogram(
					lbp_image, /* lbp_image */
					static_cast<int>(std::pow(2.0, static_cast<double>(8))), /* number of possible patterns */
					8, /* grid size x */
					8, /* grid size y */
					true);    /*normal*/
				int predictResult = svmClassifier->predict(featureVector);
				if (predictResult == 1)
				{
					putText(image, "This is spoofing face, please put real face in the rectangle", Point2f(detectROI.x - 330.0, detectROI.y - 50.0), 3, 1.2, cv::Scalar(0, 255, 0), 2); //提示语句
					continue;
				}
			}
			rectangle(image, Rect(m_faceData[i].detectFace.x + detectROI.x, m_faceData[i].detectFace.y + detectROI.y, m_faceData[i].detectFace.width, m_faceData[i].detectFace.height), cv::Scalar(0, 255, 0), 2);     //画出检测到的人脸
		}
	}
	else
	{
		putText(image, "Please put your face in the rectangle", Point2f(detectROI.x - 120.0, detectROI.y - 50.0), 3, 1.2, cv::Scalar(0, 255, 0), 2);    //提示语句
	}
}


// Returns rectangle with biggest surface area in array of rects
FaceData Detector::biggestFace(std::vector<FaceData> &faces_arr) 
{
	int biggest = 0;
	int arr_num = faces_arr.size();
	for (int i = 1; i < arr_num; ++i) 
	{
		if (faces_arr[i].detectFace.area() > faces_arr[biggest].detectFace.area()) 
		{
			biggest = i;
		}
	}
	return faces_arr[biggest];
}




