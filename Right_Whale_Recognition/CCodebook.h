#pragma once

#include <iostream>
#include <vector>
#include "opencv.h"
#include "CSIFTDescription.h"
#include <fstream>
using namespace std;

class CCodebook
{
public:
	CCodebook();
	void getCodebook(vector<string> &vec_inputFile, int num_words, int iter_split);
	void saveCodebook(string fileName);
	void loadCodebook(string fileName);
private:
	bool sign_intial_;

	vector<Mat> codebook_;
	int num_words_;
	int dimision_;

	Mat sum_;
	Mat cov_;
	Mat num_sum_;

private:
	void codebookIntial(int num_words, int dimision, float min, float max);
	void inputData(CSIFTDescription &data);
	float updata(vector<string> &vec_inputFile);
	bool splitCenter(vector<string> &vec_inputFile);
	bool deleteCenter();

private:
	ofstream logfile;

};