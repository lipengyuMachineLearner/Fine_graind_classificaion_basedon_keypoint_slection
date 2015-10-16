#pragma once

#include <iostream>
#include <vector>
#include "opencv.h"
#include "CSIFTDescription.h"

class CCodebook
{
public:
	CCodebook()
	{ 
		sign_intial_ = true; 
		num_words_ = 0;
	}
	void getCodebook(vector<string> &vec_inputFile, int num_words, int iteration);

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

};