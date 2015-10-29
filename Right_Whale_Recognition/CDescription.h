#pragma once

#include "opencv.h"
#include "config.h"

class CDescription
{
public:
	vector<KeyPoint> KeyPoint_loc_;
	vector<int> int_scalar_;
	vector<int> indexCodebook_;
	Mat mat_description_;
public:
	virtual void saveDescription(string fileName){ std::cout << "CDescription" << std::endl; }
	virtual void loadDescription(string fileName){ std::cout << "CDescription" << std::endl; }
	virtual void Get_description(IplImage *img, IplImage *img_colorHist){ std::cout << "CDescription" << std::endl; }
	virtual void Get_description(IplImage *img){ std::cout << "CDescription" << std::endl; }
};