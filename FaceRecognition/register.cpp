#include "register.h"

// **********注册人脸*************
// 以图片方式载入待注册人脸
// 输入参数：图片，训练方式是否是以更新方式
void Register::registerFaceFromPIC(cv::Mat &image, bool update)  
{
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
	m_faceData = det.findFace(m_data.image);
	//m_faceData = det.fetchFace(m_data.image);     //通过旋转多个角度检测到人脸
	if (!m_faceData.empty())
	{
		m_data = det.biggestFace(m_faceData);
		rectangle(image, Rect(m_data.detectFace.x + detectROI.x, m_data.detectFace.y + detectROI.y, m_data.detectFace.width, m_data.detectFace.height), cv::Scalar(0, 255, 0), 2);     //画出检测到的人脸
		cv::Mat normalizedImg = iNorm.normalize(m_data);
		if (!ifSpoofingFlag || svmClassifier.empty())           //无需判断是否为活体
		{
			printf("no anti-spoofing function.\n");
			int label = 0;
			string name;
			printf("Please input your name:");
			getline(cin, name); 			              //输入注册人姓名
			acquireLabel(const_cast<char*>(name.c_str()), "label_name.txt", label);
			std::vector<cv::Mat> imageData;
			std::vector<int> labels;
			imageData.push_back(normalizedImg);                   //注意：normalizedImg必须为灰度图
			labels.push_back(label);
			if (update)
			{
				lbpModel->update(imageData, labels);
			}
			else
			{
				lbpModel->train(imageData, labels);
			}
			lbpModel->write("model.xml");
		}
		else                      //带活体检测功能
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
				putText(image, "This is spoofing face, you can not register", Point2f(detectROI.x - 130.0, detectROI.y - 50.0), 3, 1.2, cv::Scalar(0, 255, 0), 2); //提示语句
			}
			else
			{
				int label = 0;
				string name;
				printf("Please input your name.\n");
				std::cin >> name; 	                                     //输入注册人姓名
				acquireLabel(const_cast<char*>(name.c_str()), "label_name.txt", label);
				std::vector<cv::Mat> imageData;
				std::vector<int> labels;
				imageData.push_back(normalizedImg);                   //注意：normalizedImg必须为灰度图
				labels.push_back(label);
				if (update)
				{
					lbpModel->update(imageData, labels);
				}
				else
				{
					lbpModel->train(imageData, labels);
				}
				lbpModel->write("model.xml");
			}
		}
	}
	else
	{
		putText(image,"请将人脸置于矩形框内",Point2f(detectROI.x + detectROI.width/2 - 30.0, detectROI.y - 10.0),3,0.8,Scalar(0,0,255),1);    //提示语句
		imshow("Image Windows",image);
		waitKey(1000);
	}
}

// 以摄像头方式载入待注册人脸
void Register::registerFaceFromCAM(bool update)
{
	//【1】从摄像头读入视频
	cv::VideoCapture capture(0);
	//【2】循环显示每一帧
	while(1)  
	{  
		cv::Mat frame;   //定义一个Mat变量，用于存储每一帧的图像
		capture>>frame;  //读取当前帧
		registerFaceFromPIC( frame,update);
		int key=cvWaitKey(50);    //读取键值到key变量中
		if (key==27) break;       //如果按下Esc键，则退出注册
	}   
}

// 以视频方式载入待注册人脸
void Register::registerFaceFromVIDEO(char *videoName, bool update)
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
			registerFaceFromPIC( frame,update);
			int key=cvWaitKey(50);    //读取键值到key变量中
			if (key==27) break;       //如果按下Esc键，则退出注册
		}
	}
}