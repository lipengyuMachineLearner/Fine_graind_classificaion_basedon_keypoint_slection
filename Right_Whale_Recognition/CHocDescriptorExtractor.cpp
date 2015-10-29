#include "CHocDescriptorExtractor.h"

void CHocDescriptorExtractor::extractor(IplImage *img, vector<KeyPoint> keypoints, Mat &descriptor)
{
	int channel = img->nChannels;
	width_ = img->width;
	height_ = img->height;
	int num = keypoints.size();
	
	descriptor = Mat::zeros(num, channel*bin_, CV_32FC1);

	IplImage* redImage = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage* greenImage = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage* blueImage = cvCreateImage(cvGetSize(img), img->depth, 1);
	cvSplit(img, blueImage, greenImage, redImage, NULL);

	for (int n = 0; n < num; n++)
	{
		blockHoC(redImage, greenImage, blueImage, keypoints[n].pt.x, keypoints[n].pt.y, n, descriptor);
	}
}
void CHocDescriptorExtractor::blockHoC(IplImage* redImage, IplImage* greenImage, IplImage* blueImage, int x, int y, int index, Mat &descriptor)
{
	int sta_x = x - block_ / 2 > 0 ? x - block_ / 2 : 0;
	int end_x = x + block_ / 2 < width_ ? x + block_ / 2 : width_;
	int sta_y = y - block_ / 2 > 0 ? y - block_ / 2 : 0;
	int end_y = y + block_ / 2 < height_ ? y + block_ / 2 : height_;

	int sumPoint = (end_x - sta_x) * (end_y - sta_y);
	unsigned char *data_red = (unsigned char*)redImage->imageData;
	unsigned char *data_green = (unsigned char*)greenImage->imageData;
	unsigned char *data_blue = (unsigned char*)blueImage->imageData;
	int step = redImage->widthStep;

	for (int i = sta_y; i < end_y; i++)
	{
		for (int j = sta_x; j < end_x; j++)
		{
			int color = data_red[j + i*step];
			descriptor.at<float>(index, color / colorStep_) += 1.0f/sumPoint;

			color = data_green[j + i*step];
			descriptor.at<float>(index, color / colorStep_ + bin_) += 1.0f / sumPoint;

			color = data_blue[j + i*step];
			descriptor.at<float>(index, color / colorStep_ + 2 * bin_) += 1.0f / sumPoint;
		}
	}
}