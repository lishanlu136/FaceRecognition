#pragma once

#include <string>
#include <io.h>
#include <vector>
#include <iostream>
#include <direct.h>  

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
using namespace std;

class iFolder
{
public:
	void getFiles(string path, string exd, std::vector<string>& files);
	int checkDir(char* Dir);     // 检查文件夹是否存在，不存在则创建
	int checkDir(const char* Dir);   // 检查文件夹是否存在，不存在则创建
	bool checkFolder(char* str); // 只检查文件夹是否存在
	void get_filelist(string path, std::vector<string>& files);//读取所有文件夹


	void get_filelist(string path, string exd, std::vector<string>& files);
	void reFileName(string folderPath, string exd, char * name, int pos);
};
