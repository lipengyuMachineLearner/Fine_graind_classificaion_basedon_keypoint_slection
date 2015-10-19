#include "CSIFTDescription.h"
#include "fstream"
using namespace std;
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

	int row = mat_description_.rows;
	int col = mat_description_.cols;
	outfileBin.write((char *)&row, sizeof(row));
	outfileBin.write((char *)&col, sizeof(col));

	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			float data = mat_description_.at<float>(r, c);
			outfileBin.write((char*)&data, sizeof(data));
		}
	}

	outfileBin.close();


	/*ofstream outfileTxt;
	outfileTxt.open("description.txt");
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
			float data = mat_description_.at<float>(r, c);
			outfileTxt << data << ",";
		}
		outfileTxt << endl;
	}

	outfileTxt.close();*/
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

	int row, col;
	infileBin.read((char *)&row, sizeof(row));
	infileBin.read((char *)&col, sizeof(col));
	
	mat_description_ = Mat(row, col, CV_32F);
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			float data = 0;
			infileBin.read((char *)&data, sizeof(data));
			mat_description_.at<float>(r, c) = data;
		}
	}

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
			float data = mat_description_.at<float>(r, c);
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
		int col = mat_description_.cols;
		Mat ones = Mat::ones(col, 1, CV_32F);
		Mat sums = mat_description_*ones;
		for (int r = 0; r < row; r++)
		{

			for (int c = 0; c < col; c++)
				mat_description_.at<float>(r, c) = mat_description_.at<float>(r, c) / sums.at<float>(r, 0);	
		}
	}
}


//void CSIFTDescription::set(vector<KeyPoint> KeyPoint_loc, vector<int> int_scalar, Mat mat_description)
//{
//	KeyPoint_loc_ = KeyPoint_loc;
//	int_scalar_ = int_scalar;
//	mat_description_ = mat_description.clone();
//}