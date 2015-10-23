#pragma once


#include <iostream>
#include <string>
#include <vector>
#include "opencv.h"
#include "config.h"
#include "CSIFTDescription.h"
#include "CCodebook.h"
#include "fstream"
using namespace std;

class CSIFT_Recognition
{
public:
	CSIFT_Recognition();
	void train(vector<string> &vec_inputFile) ; 
	void keyPoint2Image(vector<string> &vec_inputFile);
	void test(vector<string> &vec_inputFile);
	
private:
	CCodebook codebook_;
	/*void getCodebook(vector<string> &vec_inputFile, int num_words);*/
private:
	void getSIFTDescriptor(vector<string> &vec_inputFile);

private:
	ofstream logfile;
	//Mat forwardBackwardSlection();
};

