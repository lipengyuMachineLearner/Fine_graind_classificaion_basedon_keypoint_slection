#include "CSIFTDescription.h"
#include "fstream"
using namespace std;


void CSIFTDescription::Get_description(IplImage *img)
{
	//cout << "SIFT Point detecting..." << endl;
	SiftFeatureDetector detector;
	SurfFeatureDetector detector_surf;
	vector<KeyPoint> siftPoint, surfPoint;
	detector.detect(img, siftPoint);
	detector_surf.detect(img, surfPoint);
	KeyPoint_loc_.insert(KeyPoint_loc_.begin(), siftPoint.begin(), siftPoint.end());
	KeyPoint_loc_.insert(KeyPoint_loc_.begin(), surfPoint.begin(), surfPoint.end());
	//for (int i = 0; i < siftPoint.size(); i++)
	//	indexCodebook_.push_back(1);
	//for (int i = 0; i < surfPoint.size(); i++)
	//	indexCodebook_.push_back(2);
	//cout << "SIFT Point detecting complete : " << KeyPoint_loc_.size() << endl;

	//cout << "SIFT description generating..." << endl;
	SiftDescriptorExtractor extractor;
	extractor.compute(img, KeyPoint_loc_, mat_description_);
	//cout << "SIFT description generating complete" << endl;
	
}
void CSIFTDescription::saveDescription(string fileName)
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

	int row_SIFT = mat_description_.rows;
	int col_SIFT = mat_description_.cols;
	outfileBin.write((char *)&row_SIFT, sizeof(row_SIFT));
	outfileBin.write((char *)&col_SIFT, sizeof(col_SIFT));

	for (int r = 0; r < row_SIFT; r++)
	{
		for (int c = 0; c < col_SIFT; c++)
		{
			float data = mat_description_.at<float>(r, c);
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

void CSIFTDescription::loadDescription(string fileName)
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
	mat_description_ = Mat(row_SIFT, col_SIFT, CV_32F);
	for (int r = 0; r < row_SIFT; r++)
	{
		for (int c = 0; c < col_SIFT; c++)
		{
			float data = 0;
			infileBin.read((char *)&data, sizeof(data));
			mat_description_.at<float>(r, c) = data;
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
		int row = mat_description_.rows;
		int col_SIFT = mat_description_.cols;
		Mat ones_SIFT = Mat::ones(col_SIFT, 1, CV_32FC1);
		Mat sums_SIFT = mat_description_*ones_SIFT;

		for (int r = 0; r < row; r++)
		{

			for (int c = 0; c < col_SIFT; c++)
				mat_description_.at<float>(r, c) = mat_description_.at<float>(r, c) / sums_SIFT.at<float>(r, 0);
		}
	}
}
