#pragma once

#include <iostream>
#include <vector>
#include "opencv.h"
#include "CSIFTDescription.h"
#include "CHoCDescription.h"
#include <fstream>
using namespace std;

class CCodebook
{
private:
	int num_words_;
	int kmeans_stop_iteration_;
	float kmeans_stop_threshold_; 

	int split_stop_iteration_;
	float split_threshold_;
	float min_intial_;
	float max_intial_;

	bool sign_intial_;

	vector<Mat> codebook_;
	int dimision_;

	Mat sum_;
	Mat cov_;
	Mat num_sum_;
public:
	CCodebook(int num_words, int kmeans_stop_iteration, float kmeans_stop_threshold,
		int split_stop_iteration, float split_threshold, float min_intial, float max_intial);
	void getCodebook(vector<string> &vec_inputFile, string path, int iter_split);
	void saveCodebook(string fileName);
	void loadCodebook(string fileName);
	void matchCodebook(vector<string> &vec_inputFile);
	

private:
	void codebookIntial(int num_words, int dimision, float min, float max, CDescription &data);
	void inputData(CDescription &data, string path);
	float updata(vector<string> &vec_inputFile, string path);
	bool splitCenter(vector<string> &vec_inputFile);
	bool deleteCenter();

private:
	ofstream logfile;

};