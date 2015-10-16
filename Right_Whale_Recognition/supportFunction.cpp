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