#include "opencv.h"

class CHocDescriptorExtractor
{
public:
	CHocDescriptorExtractor(int bin, int block)
	{
		bin_ = bin;
		block_ = block;

		colorStep_ = (255.0f / bin + 0.5);
	}
	void extractor(IplImage *img, vector<KeyPoint> keypoints, Mat &descriptor);
private:
	int bin_;
	int block_;
	int colorStep_;
	int width_;
	int height_;

	void blockHoC(IplImage* redImage, IplImage* greenImage, IplImage* blueImage, int x, int y, int index, Mat &descriptor);
};