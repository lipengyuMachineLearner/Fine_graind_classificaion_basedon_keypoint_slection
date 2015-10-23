#pragma once

#include "opencv.h"

class CSIFTDescription
{
public:
	vector<KeyPoint> KeyPoint_loc_;
	vector<int> int_scalar_;
	vector<int> indexCodebook_;
	Mat mat_description_;
public:
	void saveSIFTDescription(string fileName);
	void loadSIFTDescription(string fileName);
	void normalizationDescription(string type);
	void GetSIFT_description(IplImage *img, int sign_pyramid);
};