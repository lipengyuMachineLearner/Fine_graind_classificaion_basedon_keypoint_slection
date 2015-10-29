#include "CHoCDescription.h"
#include "fstream"
using namespace std;


void CHoCDescription::Get_description(IplImage *img, IplImage *img_colorHist)
{
	//cout << " Point detecting..." << endl;
	SiftFeatureDetector detector;
	SurfFeatureDetector detector_surf;
	vector<KeyPoint> Point, surfPoint;
	detector.detect(img, Point);
	detector_surf.detect(img, surfPoint);
	KeyPoint_loc_.insert(KeyPoint_loc_.begin(), Point.begin(), Point.end());
	KeyPoint_loc_.insert(KeyPoint_loc_.begin(), surfPoint.begin(), surfPoint.end());
	//for (int i = 0; i < Point.size(); i++)
	//	indexCodebook_.push_back(1);
	//for (int i = 0; i < surfPoint.size(); i++)
	//	indexCodebook_.push_back(2);
	//cout << " Point detecting complete : " << KeyPoint_loc_.size() << endl;

	//cout << " description generating..." << endl;
	CHocDescriptorExtractor extractorHoc(128, 16);
	extractorHoc.extractor(img_colorHist, KeyPoint_loc_, mat_description_);
	//cout << " description generating complete" << endl;

}
void CHoCDescription::saveDescription(string fileName)
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

	int row_ = mat_description_.rows;
	int col_ = mat_description_.cols;
	outfileBin.write((char *)&row_, sizeof(row_));
	outfileBin.write((char *)&col_, sizeof(col_));

	for (int r = 0; r < row_; r++)
	{
		for (int c = 0; c < col_; c++)
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
	float data = mat__description_.at<float>(r, c);
	outfiletxt << data << ",";
	}
	outfiletxt << endl;
	}

	outfiletxt.close();*/
}

void CHoCDescription::loadDescription(string fileName)
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

	int row_, col_;
	infileBin.read((char *)&row_, sizeof(row_));
	infileBin.read((char *)&col_, sizeof(col_));
	mat_description_ = Mat(row_, col_, CV_32F);
	for (int r = 0; r < row_; r++)
	{
		for (int c = 0; c < col_; c++)
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
	float data = mat__description_.at<float>(r, c);
	outfileTxt << data << ",";
	}
	outfileTxt << endl;
	}

	outfileTxt.close();*/
}

