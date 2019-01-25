#include "identifier.h"

// ********识别人脸*****************
// 输入单张图片进行人脸识别
void Identifier::identificationFaceFromPIC(cv::Mat &image)
{
	//在识别之前先判定lbpModel是否为空，为空则需要重新开始训练Model（注册）
	if (lbpModel.empty()) 
	{
		std::cerr << "Error in identification! LBP model is empty, you must register firstly." << std::endl;
	}
	Mat imgRoi = image(detectROI);
	rectangle(image,detectROI,Scalar(0,0,255));
	Mat grayImgRoi;
	switch (imgRoi.type())
	{
	case 0:
		imgRoi.copyTo(grayImgRoi);  
		break;
	case 16:
		cv::cvtColor(imgRoi,grayImgRoi,CV_BGR2GRAY);
		break;
	default:
		printf("Image type error, input image must be gray or rgb!");
		break;
	}
	m_data.image = grayImgRoi;
	//m_faceData = det.fetchFace(m_data.image);  //多角度捕捉人脸
	m_faceData = det.findFace(m_data.image);
	size_t num_fD = m_faceData.size();
	if (num_fD != 0)
	{
		for (size_t i=0; i<num_fD; ++i)
		{
			cv::Mat normalizedImg = iNorm.normalize(m_faceData[i]);

			// 判别是否是活体人脸
			if (!ifSpoofingFlag || svmClassifier.empty())
			{
				printf("no anti-spoofing function.\n");
			}
			else
			{
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
			int predictedLabel;
			double dist;
			lbpModel->predict(normalizedImg, predictedLabel, dist);       //对normalizedImg提取LBP特征,必须为灰度图输入
			//! [Print results]
			if (dist < I_THRESHOLD)
			{
				std::vector<string> classNames = readClassNames();
				char preName[100];
				sprintf(preName, "%s", classNames.at(predictedLabel).c_str());
				putText(image, preName, Point2f(detectROI.x + m_faceData[i].detectFace.x + m_faceData[i].detectFace.width / 2 - 40.0, detectROI.y + m_faceData[i].detectFace.y - 10.0), 3, 0.8, Scalar(255, 0, 0), 1);         //写出识别姓名            
			}
			else
			{
				putText(image, "No registration information, no identifiacation", Point2f(detectROI.x - 220.0, detectROI.y - 50.0), 3, 1.2, cv::Scalar(0, 255, 0), 2);   //提示语句
			}
		}
	}
	else
	{
		putText(image,"请将人脸置于矩形框内",Point2f(detectROI.x + detectROI.width/2 - 30.0, detectROI.y - 10.0),3,0.8,Scalar(0,0,255),1);    //提示语句
	}
}

// 以摄像头方式输入图像进行识别
void Identifier::identificationFaceFromCAM()
{
	cv::VideoCapture capture(0);
	while(1)  
	{  
		cv::Mat frame;   //定义一个Mat变量，用于存储每一帧的图像
		capture>>frame;  //读取当前帧
		identificationFaceFromPIC( frame);
		int key=cvWaitKey(50);    //读取键值到key变量中
		if (key==27) break;       //如果按下Esc键，则退出识别
	}   
}

// 以视频方式输入图像进行识别
void Identifier::identificationFaceFromVIDEO(char *videoName)
{
	cv::VideoCapture capture;
	capture.open(videoName);
	cv::Mat frame;
	if (capture.isOpened())
	{
		for (;;)
		{
			capture >> frame;
			if (frame.empty())                 //当读到视频最后一帧时，判断是否为空，为空则跳出
			{
				break;                        
			}
			identificationFaceFromPIC( frame);
			int key=cvWaitKey(50);    //读取键值到key变量中
			if (key==27) break;       //如果按下Esc键，则退出识别
		}
	}
}

