#pragma once


#include <iostream>
#include <string>
#include <vector>
#include "opencv.h"
#include "config.h"
#include "CSIFTDescription.h"
#include "CCodebook.h"

using namespace std;

class CSIFT_Recognition
{
public:

	void train(vector<string> &vec_inputFile) ; 
	void test();
	
private:
	CCodebook codebook_;

private:
	void getSIFTDescriptor(vector<string> &vec_inputFile);
	CSIFTDescription SIFT_description(IplImage *img, int int_pyramidLayerNumber);

private:
	void getCodebook(vector<string> &vec_inputFile, int num_words);
	
	Mat SIFT_codebookGeneration(string str_SIFTDescriptorPath, int int_ClusterCenterNumber);
	void loadSIFTDescription(string str_inputPath);
	Mat forwardBackwardSlection();
};

