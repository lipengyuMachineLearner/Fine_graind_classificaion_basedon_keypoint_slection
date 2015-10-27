#include "CSIFTDescription.h"
#include "fstream"
using namespace std;


void CSIFTDescription::GetSIFT_description(IplImage *img, int sign_pyramid, int DenseSampleStride, int num_scalar, const int const scalar[])
{
	if (DenseSampleStride == 0 && num_scalar == 1)
	{
		//cout << "SIFT Point detecting..." << endl;
		SiftFeatureDetector detector;
		detector.detect(img, KeyPoint_loc_);
		//cout << "SIFT Point detecting complete : " << description.KeyPoint_loc_.size() << endl;

		//cout << "SIFT description generating..." << endl;
		SiftDescriptorExtractor extractor;
		extractor.compute(img, KeyPoint_loc_, mat_SIFT_description_);
		//cout << "SIFT description generating complete" << endl;
	}
	else if (DenseSampleStride > 0 && num_scalar > 1)
	{
		vector<IplImage *> vec_img;
		for (int s = 0; s < num_scalar; s++)
		{
			IplImage *img_tmp = cvCreateImage(cvSize(img->width / scalar[s], img->height / scalar[s]), img->depth, img->nChannels);
			cvResize(img, img_tmp);
			IplImage *img_tmp2 = cvCreateImage(cvSize(img->width / scalar[s], img->height / scalar[s]), img->depth, 1);
			cvCvtColor(img_tmp, img_tmp2, CV_RGB2GRAY);

			int width = img_tmp->width;
			int height = img_tmp->height;
			int step = img_tmp->widthStep;
			int channel = img_tmp->nChannels;

			HOGDescriptor *hog = new HOGDescriptor(cvSize(width, height), cvSize(16, 16), cvSize(DenseSampleStride, DenseSampleStride), cvSize(16, 16), 36);
				//, 1, 3, HOGDescriptor::L2Hys, 0.2, true, 1);
		
			vector<float> hog_des;
			hog->compute(img_tmp2, hog_des);
			for (int h = DenseSampleStride; h < height; h += DenseSampleStride)
			{
				for (int w = DenseSampleStride; w < width; w += DenseSampleStride)
				{
					KeyPoint tmp;
					tmp.pt.x = w;
					tmp.pt.y = h;
				}
			}


			cvResize(img, img_tmp);
			vec_img.push_back(img_tmp);
		}
	}
	else
	{
		cout << "DenseSampleStride and num_scalar error" << endl;
		exit(0);
	}
}
void CSIFTDescription::saveSIFTDescription(string fileName)
{
	
	ofstream outfileBin;
	outfileBin.open(fileName, ios::binary);

	int num = KeyPoint_loc_.size();
	outfileBin.write((char *)&num, sizeof(num));

	while (indexCodebook_.size() < num)
	{
		indexCodebook_.push_back(-1);
	}

	for (int i = 0; i < num; i++)
	{
		float x = KeyPoint_loc_[i].pt.x;
		float y = KeyPoint_loc_[i].pt.y;
		int index = indexCodebook_[i];

		outfileBin.write((char *)&x, sizeof(x));
		outfileBin.write((char *)&y, sizeof(y));
		outfileBin.write((char *)&index, sizeof(index));
	}

	int row_SIFT = mat_SIFT_description_.rows;
	int col_SIFT = mat_SIFT_description_.cols;
	outfileBin.write((char *)&row_SIFT, sizeof(row_SIFT));
	outfileBin.write((char *)&col_SIFT, sizeof(col_SIFT));

	for (int r = 0; r < row_SIFT; r++)
	{
		for (int c = 0; c < col_SIFT; c++)
		{
			float data = mat_SIFT_description_.at<float>(r, c);
			outfileBin.write((char*)&data, sizeof(data));
		}
	}

	/*int row_HoC = mat_HoC_description_.rows;
	int col_HoC = mat_HoC_description_.cols;
	outfileBin.write((char *)&row_HoC, sizeof(row_HoC));
	outfileBin.write((char *)&col_HoC, sizeof(col_HoC));

	for (int r = 0; r < row_HoC; r++)
	{
		for (int c = 0; c < col_HoC; c++)
		{
			float data = mat_HoC_description_.at<float>(r, c);
			outfileBin.write((char*)&data, sizeof(data));
		}
	}*/

	outfileBin.close();


	/*ofstream outfiletxt;
	outfiletxt.open("description.txt");
	outfiletxt << num << endl;

	for (int i = 0; i < num; i++)
	{
		float x = KeyPoint_loc_[i].pt.x;
		float y = KeyPoint_loc_[i].pt.y;

		outfiletxt << x << "," << y << endl;
	}

	
	outfiletxt << row << "," << col << endl;
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			float data = mat_SIFT_description_.at<float>(r, c);
			outfiletxt << data << ",";
		}
		outfiletxt << endl;
	}

	outfiletxt.close();*/
}

void CSIFTDescription::loadSIFTDescription(string fileName)
{

	ifstream infileBin;
	infileBin.open(fileName, ios::binary);

	int num;
	infileBin.read((char *)&num, sizeof(num));

	for (int i = 0; i < num; i++)
	{
		float x;
		float y;
		int index;
		infileBin.read((char *)&x, sizeof(x));
		infileBin.read((char *)&y, sizeof(y));
		infileBin.read((char *)&index, sizeof(index));

		KeyPoint tmp;
		tmp.pt.x = x;
		tmp.pt.y = y;
		KeyPoint_loc_.push_back(tmp);
		indexCodebook_.push_back(index);
	}

	int row_SIFT, col_SIFT;
	infileBin.read((char *)&row_SIFT, sizeof(row_SIFT));
	infileBin.read((char *)&col_SIFT, sizeof(col_SIFT));
	mat_SIFT_description_ = Mat(row_SIFT, col_SIFT, CV_32F);
	for (int r = 0; r < row_SIFT; r++)
	{
		for (int c = 0; c < col_SIFT; c++)
		{
			float data = 0;
			infileBin.read((char *)&data, sizeof(data));
			mat_SIFT_description_.at<float>(r, c) = data;
		}
	}

	/*int row_HoC, col_HoC;
	infileBin.read((char *)&row_HoC, sizeof(row_HoC));
	infileBin.read((char *)&col_HoC, sizeof(col_HoC));
	mat_HoC_description_ = Mat(row_HoC, col_HoC, CV_32F);
	for (int r = 0; r < row_HoC; r++)
	{
		for (int c = 0; c < col_HoC; c++)
		{
			float data = 0;
			infileBin.read((char *)&data, sizeof(data));
			mat_HoC_description_.at<float>(r, c) = data;
		}
	}*/

	infileBin.close();

	/*ofstream outfileTxt;
	outfileTxt.open("description2.txt");
	outfileTxt << num << endl;

	for (int i = 0; i < num; i++)
	{
		float x = KeyPoint_loc_[i].pt.x;
		float y = KeyPoint_loc_[i].pt.y;

		outfileTxt << x << "," << y << endl;
	}


	outfileTxt << row << "," << col << endl;
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			float data = mat_SIFT_description_.at<float>(r, c);
			outfileTxt << data << ",";
		}
		outfileTxt << endl;
	}

	outfileTxt.close();*/
}

void CSIFTDescription::normalizationDescription(string type)
{
	if (type == "L1")
	{
		int row = mat_SIFT_description_.rows;
		int col_SIFT = mat_SIFT_description_.cols;
		Mat ones_SIFT = Mat::ones(col_SIFT, 1, CV_32FC1);
		Mat sums_SIFT = mat_SIFT_description_*ones_SIFT;
		/*int col_HoC = mat_HoC_description_.cols;
		Mat ones_HoC = Mat::ones(col_HoC, 1, CV_32FC1);
		Mat sums_HoC = mat_HoC_description_*ones_HoC;*/
		for (int r = 0; r < row; r++)
		{

			for (int c = 0; c < col_SIFT; c++)
				mat_SIFT_description_.at<float>(r, c) = mat_SIFT_description_.at<float>(r, c) / sums_SIFT.at<float>(r, 0);

			/*for (int c = 0; c < col_HoC; c++)
				mat_HoC_description_.at<float>(r, c) = mat_HoC_description_.at<float>(r, c) / sums_HoC.at<float>(r, 0);*/
		}
	}
}


//void CSIFTDescription::set(vector<KeyPoint> KeyPoint_loc, vector<int> int_scalar, Mat mat_description)
//{
//	KeyPoint_loc_ = KeyPoint_loc;
//	int_scalar_ = int_scalar;
//	mat_SIFT_description_ = mat_description.clone();
//}