#pragma once

#include "opencv.h"
#include "config.h"
#include "CDescription.h"
class CSIFTDescription :public CDescription
{
//public:
//	vector<KeyPoint> KeyPoint_loc_;
//	vector<int> int_scalar_;
//	vector<int> indexCodebook_;
//	Mat mat_description_;
public:
	void saveDescription(string fileName);
	void loadDescription(string fileName);
	void normalizationDescription(string type);
	void Get_description(IplImage *img);
};