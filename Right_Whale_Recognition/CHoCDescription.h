#pragma once

#include "opencv.h"
#include "config.h"
#include "CHoCDescriptorExtractor.h"
#include "CDescription.h"
class CHoCDescription :public CDescription
{
//public:
//	vector<KeyPoint> KeyPoint_loc_;
//	vector<int> int_scalar_;
//	vector<int> indexCodebook_;
//	Mat mat_description_;
public:
	void saveDescription(string fileName);
	void loadDescription(string fileName);
	void Get_description(IplImage *img, IplImage *img_colorHist);
};