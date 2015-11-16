#pragma once

#include "opencv.h"
#include <iostream>
#include <string>
#include <fstream>

#include <stdlib.h>
#include <memory>
#include <direct.h>
#include "windows.h"

using namespace std;
void saveMat(Mat mat_data, string str_outputfile)
{
	ofstream outfileBin;
	outfileBin.open(str_outputfile, ios::binary);


	int col = mat_data.cols;
	int row = mat_data.rows;

	outfileBin.write((char*)&row, sizeof(row));
	outfileBin.write((char*)&col, sizeof(col));
	//outfileBin.write((char*)&mat_data, sizeof(mat_data));

	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			float data = mat_data.at<float>(r, c);
			outfileBin.write((char*)&data, sizeof(data));
		}
		
	}
	outfileBin.close();

	ofstream outfileTxt;
	outfileTxt.open("test2.txt");
	outfileTxt << row << "," << col << endl;
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			outfileTxt << mat_data.at<float>(r, c) << ",";
		}
		outfileTxt << endl;
	}
	outfileTxt.close();

	
	

}
void loadMat(Mat &mat_data, string str_inputfile)
{
	ifstream infileBin;
	infileBin.open(str_inputfile, ios::binary);
	
	int row, col;
	infileBin.read((char *)&row, sizeof(row));
	infileBin.read((char *)&col, sizeof(col));

	mat_data = Mat(row, col, CV_32F);
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			float data = 0;
			infileBin.read((char *)&data, sizeof(data));
			mat_data.at<float>(r, c) = data;
		}
	}

	infileBin.close();

	ofstream  outfileTxt;
	outfileTxt.open("test1.txt");
	outfileTxt << row << "," << col << endl;
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			outfileTxt << mat_data.at<float>(r, c) << ",";
		}
		outfileTxt << endl;
	}

	outfileTxt.close();
}

void cheackPath(string path)
{
	WIN32_FIND_DATA wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile((LPCWSTR)path.c_str(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	else
	{
		_mkdir(path.c_str());
	}
	FindClose(hFind);
}

string int2str(int val)
{
	char buf[64] = "";
	_snprintf(buf, sizeof(buf) - 1, "%d", val);
	return buf;
}


void setUselessHoCPoint(bool *uselessHoCPoint)
{
	memset(uselessHoCPoint, 0, sizeof(bool));
	uselessHoCPoint[7] = true;
	uselessHoCPoint[8] = true;
	uselessHoCPoint[11] = true;
	uselessHoCPoint[17] = true;
	uselessHoCPoint[18] = true;
	uselessHoCPoint[19] = true;
	uselessHoCPoint[22] = true;
	uselessHoCPoint[25] = true;
	uselessHoCPoint[32] = true;
	uselessHoCPoint[35] = true;
	uselessHoCPoint[43] = true;
	uselessHoCPoint[53] = true;
	uselessHoCPoint[55] = true;
	uselessHoCPoint[57] = true;
	uselessHoCPoint[58] = true;
	uselessHoCPoint[60] = true;
	uselessHoCPoint[62] = true;
	uselessHoCPoint[64] = true;
	uselessHoCPoint[66] = true;
	uselessHoCPoint[69] = true;
	uselessHoCPoint[73] = true;
	uselessHoCPoint[74] = true;
	uselessHoCPoint[76] = true;
	uselessHoCPoint[78] = true;
	uselessHoCPoint[80] = true;
	uselessHoCPoint[82] = true;
	uselessHoCPoint[86] = true;
	uselessHoCPoint[88] = true;
	uselessHoCPoint[92] = true;
	uselessHoCPoint[98] = true;
	uselessHoCPoint[99] = true;
	uselessHoCPoint[101] = true;
	uselessHoCPoint[103] = true;
	uselessHoCPoint[105] = true;
	uselessHoCPoint[107] = true;
	uselessHoCPoint[108] = true;
	uselessHoCPoint[113] = true;
	uselessHoCPoint[117] = true;
	uselessHoCPoint[118] = true;
	uselessHoCPoint[123] = true;
	uselessHoCPoint[124] = true;
	uselessHoCPoint[127] = true;
	uselessHoCPoint[129] = true;
	uselessHoCPoint[130] = true;
	uselessHoCPoint[132] = true;
	uselessHoCPoint[133] = true;
	uselessHoCPoint[135] = true;
	uselessHoCPoint[136] = true;
	uselessHoCPoint[138] = true;
	uselessHoCPoint[140] = true;
	uselessHoCPoint[142] = true;
	uselessHoCPoint[145] = true;
	uselessHoCPoint[146] = true;
	uselessHoCPoint[148] = true;
	uselessHoCPoint[152] = true;
	uselessHoCPoint[153] = true;
	uselessHoCPoint[154] = true;
}