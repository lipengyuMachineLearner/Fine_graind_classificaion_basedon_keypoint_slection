#include <stdio.h>
#include<io.h>
#include<vector>
#include<string>
using namespace std;

int getFiles(string rootPath, string format, vector<string> &files)
{
	struct _finddata_t fa;
	long fHandle;

	if ((fHandle = _findfirst((rootPath+"\\"+format).c_str(), &fa)) == -1L)//这里可以改成需要的目录 
	{
		printf("there is no relative file in the rootPath\n");
		return 0;
	}
	else
		do
		{
			files.push_back(rootPath + "\\" + fa.name);
		} while (_findnext(fHandle, &fa) == 0);
		_findclose(fHandle);
		return files.size();
}