#include "common.h"

// 读取用于训练分类器的图片及其标签
void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator) 
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file) 
	{
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) 
	{
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if(!path.empty() && !classlabel.empty()) 
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

// 读取标签和对应人姓名的文件
std::vector<string> readClassNames(const char *filename)
{
	std::vector<string> classNames;

	std::ifstream fp(filename);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found: " << filename << std::endl;
		exit(-1);
	}

	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
			classNames.push_back( name.substr(name.find(' ')+1) );
	}

	fp.close();
	return classNames;
}

//获取注册名字的label
void acquireLabel(const char *name, const char *filename, int &label)
{
	std::vector<string> classNames = readClassNames(filename);
	int numRegisted = classNames.size();
	for (int i=0; i<numRegisted; i++)
	{
		if (classNames.at(i) == name)     //如果该名字已注册过，直接返回其label
		{
			label = i;
			return;
		}
	}
	label = numRegisted;
	std::ofstream writerFp(filename,std::ios::app);  //如果该名字没有被注册过，则在文件末尾写入label和对应姓名
	writerFp << std::endl;                          //写入换行符
	writerFp << label << " "<< name << std::endl;
    writerFp.close();
}