#ifndef _COMMON_H_
#define _COMMON_H_
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "facedata.h"
#include "detector_dlib.h"
#include "normalizator.h"
#include <io.h>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

void read_csv(const string& filename, vector<cv::Mat>& images, vector<int>& labels, char separator = ' ');
std::vector<string> readClassNames(const char *filename = "D:/dataset/face_video/label_name.txt");    //注册信息所在文件
void acquireLabel(const char *name, const char *filename, int &label);
#endif