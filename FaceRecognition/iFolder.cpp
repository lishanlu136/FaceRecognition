#include "iFolder.h"

void iFolder::reFileName(string folderPath, string exd, char *name, int pos){
	
	std::vector<string> files;
	getFiles(folderPath, exd, files);
	std::vector<string>::iterator it;
	char posC[100];
	int ind = 0;
	for (it = files.begin(); it < files.end(); it++){

		switch (pos)
		{
		case 2:
			sprintf(posC, "%s%02d", name,++ind);
			break;
		case 3:
			sprintf(posC, "%s%03d", name, ++ind);
			break;
		case 4:
			sprintf(posC, "%s%04d", name, ++ind);
			break;
		case 5:
			sprintf(posC, "%s%05d", name, ++ind);
			break;
		case 6:
			sprintf(posC, "%s%06d", name, ++ind);
			break;
		case 7:
			sprintf(posC, "%s%07d", name, ++ind);
			break;
		case 8:
			sprintf(posC, "%s%08d", name, ++ind);
			break;
		default:
			sprintf(posC, "%s%d", name, ++ind);
			break;
		}
		string newName = folderPath + "/";
		newName += posC;
		newName += ".";
		newName += exd;
		if (rename((*it).c_str(), newName.c_str()) < 0) return;
	}
}

/************************************************************************/
/*  获取文件夹下所有文件名
输入：
path	:	文件夹路径
exd		:   所要获取的文件名后缀，如jpg、png等；如果希望获取所有
文件名, exd = ""
输出：
files	:	获取的文件名列表

shao, 20140707
*/
/************************************************************************/
void iFolder::getFiles(string path, string exd, std::vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("/").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(pathName.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

//检查文件夹是否存在，不存在则创建
//文件夹存在返回 0  
//文件夹创建失败返回-1  
//文件夹创建成功返回1  
int iFolder::checkDir(char* Dir)
{
	FILE *fp = NULL;
	char TempDir[200];
	memset(TempDir, '\0', sizeof(TempDir));
	sprintf(TempDir, Dir);
	strcat(TempDir, "\\");
	strcat(TempDir, ".temp.fortest");
	fp = fopen(TempDir, "w");
	if (!fp)
	{
		if (_mkdir(Dir) == 0)
		{
			return 1;//文件夹创建成功  
		}
		else
		{
			return -1;//can not make a dir;  
		}
	}
	else
	{
		fclose(fp);
	}
	return 0;
}
int iFolder::checkDir(const char* Dir)
{
	FILE *fp = NULL;
	char TempDir[200];
	memset(TempDir, '\0', sizeof(TempDir));
	sprintf(TempDir, Dir);
	strcat(TempDir, "\\");
	strcat(TempDir, ".temp.fortest");
	fp = fopen(TempDir, "w");
	if (!fp)
	{
		if (_mkdir(Dir) == 0)
		{
			return 1;//文件夹创建成功  
		}
		else
		{
			return -1;//can not make a dir;  
		}
	}
	else
	{
		fclose(fp);
	}
	return 0;
}


bool iFolder::checkFolder(char* str){

	wchar_t dir[1000];
	mbstowcs(dir, str, 999);
	return 	PathIsDirectory(dir);
}

void iFolder::get_filelist(string path, std::vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		_findnext(hFile, &fileinfo);
		_findnext(hFile, &fileinfo);
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			/*if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{*/
				//files.push_back(p.assign(path).append("/").append(fileinfo.name));
				files.push_back(fileinfo.name);
			//}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void iFolder::get_filelist(string path, string exd, std::vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之
			//如果不是,加入列表
			/*if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("/").append(fileinfo.name), exd, files);
			}
			else
			{*/
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(fileinfo.name);
			//}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}