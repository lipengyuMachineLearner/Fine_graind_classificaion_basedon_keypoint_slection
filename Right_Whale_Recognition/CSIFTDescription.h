#pragma once

#include "opencv.h"
#include "config.h"
class CSIFTDescription
{
public:
	CSIFTDescription()
	{
		mat_Weight_ = Mat(2, 1, CV_32FC1);
		mat_Weight_.at<float>(0, 0) = DESTRIPTION_WEIGHT[0];
		mat_Weight_.at<float>(1, 0) = DESTRIPTION_WEIGHT[1];
	}

public:
	vector<KeyPoint> KeyPoint_loc_;
	vector<int> int_scalar_;
	vector<int> indexCodebook_;
	Mat mat_SIFT_description_;
	//Mat mat_HoC_description_;
	Mat mat_Weight_;
public:
	void saveSIFTDescription(string fileName);
	void loadSIFTDescription(string fileName);
	void normalizationDescription(string type);
	void GetSIFT_description(IplImage *img, int sign_pyramid, int DenseSampleStride, int num_scalar, const int const scalar[]);
};