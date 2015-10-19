#include "CCodebook.h"
#include <time.h>
#include "config.h"
#include <fstream>
using namespace std;

void CCodebook::getCodebook(vector<string> &vec_inputFile, int num_words, int iter_split)
{
	int num_file = vec_inputFile.size();

	float cov = 0;
	std::cout << iter_split << ":--------------------------------------------------------------------------------------" << std::endl;
	for (int iter = 0; iter < KMEANS_STOP_ITERATION; iter++)
	{
		for (int file_i = 0; file_i < num_file; file_i++)
		{
			string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
			string::size_type pos2 = vec_inputFile[file_i].rfind(".");
			string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".bin";

			CSIFTDescription description;
			description.loadSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);

			if (dimision_ == 0)
			{
				dimision_ = description.mat_description_.cols;
				codebookIntial(num_words, dimision_, 0.5/128, 1.5 / 128);
			}

			if (dimision_ != description.mat_description_.cols)
			{
				std::cout << "(void CSIFT_Recognition::getCoodbook(vector<string> &vec_inputFile)) error in the dimision" << std::endl;
				exit(0);
			}
			inputData(description);
			description.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);
		}

		std::cout << iter << ": ################################" << std::endl;
		std::cout << "num_sum_ : ";
		for (int w = 0; w < num_words_; w++)
			std::cout << num_sum_.at<float>(w, 0) << ",";
		std::cout << std::endl;
	

		cov = updata(vec_inputFile);
		std::cout << "cov = " << cov << std::endl;
		if (cov > KMEANS_STOP_THRESHOLD)
		{
			sum_ = Mat(num_words_, dimision_, CV_32FC1) * 0;
			num_sum_ = Mat(num_words_, 1, CV_32FC1) * 0;
		}
		else
		{
			bool sign = splitCenter(vec_inputFile);
			if (sign)
			{
				sum_ = Mat(num_words_, dimision_, CV_32FC1) * 0;
				num_sum_ = Mat(num_words_, 1, CV_32FC1) * 0;
				iter = 0;
			}
		}
	}

	bool sign = splitCenter(vec_inputFile);
	if (sign && iter_split < SPLIT_STOP_ITERATION)
	{
		sum_ = Mat(num_words_, dimision_, CV_32FC1) * 0;
		num_sum_ = Mat(num_words_, 1, CV_32FC1) * 0;
		getCodebook(vec_inputFile, num_words_, iter_split + 1);
	}
}

void CCodebook::codebookIntial(int num_words, int dimision, float min, float max)
{
	if (sign_intial_ == false)
		return;
	

	num_words_ = num_words;
	dimision_ = dimision;
	srand((int)time(0));
	for (int i = 0; i < num_words_; i++)
	{
		Mat words = Mat(1, dimision_, CV_32FC1);
		
		for (int i = 0; i < dimision_; i++)
		{
			float data = ((float)rand()) / RAND_MAX *(max - min) + min;
			words.at<float>(0, i) = data;
		}

		codebook_.push_back(words);
	}

	sum_ = Mat(num_words_, dimision_, CV_32FC1) * 0;
	num_sum_ = Mat(num_words_, 1, CV_32FC1) * 0;

	sign_intial_ = false;
}

void CCodebook::inputData(CSIFTDescription &data)
{
	Mat description = data.mat_description_;

	int num_data = description.rows;

	for (int r = 0; r < num_data; r++)
	{
		float min_distance = 100000;
		int min_index = -1;
		Mat mat_row = description.row(r);
		for (int w = 0; w < num_words_; w++)
		{
			float distance = (mat_row - codebook_[w]).dot((mat_row - codebook_[w]));
			if (distance < min_distance)
			{
				min_distance = distance;
				min_index = w;
			}
		}

		data.indexCodebook_[r] = min_index;
		sum_.row(min_index) = sum_.row(min_index) + mat_row;
		num_sum_.at<float>(min_index, 0) += 1;
	}
}

float CCodebook::updata(vector<string> &vec_inputFile)
{
	for (int i = 0; i < num_words_; i++)
	{
		if (abs(num_sum_.at<float>(i, 0) != 0) < 0.01)
			codebook_[i] = sum_.row(i) * 0;
		else
			codebook_[i] = sum_.row(i) * (1.0 / num_sum_.at<float>(i, 0));

	/*	for (int j = 0; j < dimision_; j++)
			std::cout << codebook_[i].at<float>(0, j) << " , ";
		std::cout << std::endl << "###################################" << std::endl;*/
	}

	
	int num_file = vec_inputFile.size();
	float sum = 0;
	int num = 0;
	cov_ = Mat(num_words_, 1, CV_32FC1) * 0;

	for (int file_i = 0; file_i < num_file; file_i++)
	{
		string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".bin";

		CSIFTDescription description;
		description.loadSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);

		int rows = description.mat_description_.rows;
		for (int r = 0; r < rows; r++)
		{
			Mat mat_row = description.mat_description_.row(r);

			int index = description.indexCodebook_[r];

		
			float distance = (mat_row - codebook_[index]).dot((mat_row - codebook_[index]));

			cov_.at<float>(index, 0) += distance;
			sum += distance;
			num += 1;
		}
	}

	cov_ = cov_ / num_sum_;
	
	return sum / num;
}
bool CCodebook::splitCenter(vector<string> &vec_inputFile)
{
	bool result = false;
	for (int n = 0; n < num_words_; n++)
	{
		if (cov_.at<float>(n, 0) > SPLIT_THRESHOLD)
		{
			Mat words_new = Mat(1, dimision_, CV_32FC1);
			srand((int)time(0));
			for (int i = 0; i < dimision_; i++)
			{
				float data = ((float)rand()) / RAND_MAX * sqrt(cov_.at<float>(n, 0)) * 1.0 / dimision_;
				codebook_[n].at<float>(0, i) = codebook_[n].at<float>(0, i) + data;
				words_new.at<float>(0, i) = codebook_[n].at<float>(0, i) - data;
			}

			codebook_.push_back(words_new);
			result = true;
		}
	}

	num_words_ = codebook_.size();
	return result;
}

void CCodebook::saveCodebook(string fileName)
{
	ofstream outfileBin;
	outfileBin.open(fileName, ios::binary);

	outfileBin.write((char *)&num_words_, sizeof(num_words_));
	outfileBin.write((char *)&dimision_, sizeof(dimision_));

	for (int n = 0; n < num_words_; n++)
	{
		for (int d = 0; d < dimision_; d++)
		{
			float data = codebook_[n].at<float>(0, d);
			outfileBin.write((char *)&data, sizeof(data));
		}
	}

	outfileBin.close();


	/*ofstream outfileTxt;
	outfileTxt.open("codebook1.txt");

	outfileTxt << num_words_ << endl;
	outfileTxt << dimision_ << endl;

	for (int n = 0; n < num_words_; n++)
	{
		for (int d = 0; d < dimision_; d++)
		{
			float data = codebook_[n].at<float>(0, d);
			outfileTxt << data << ",";
		}
		outfileTxt << endl;
	}

	outfileTxt.close();*/
}

void CCodebook::loadCodebook(string fileName)
{
	ifstream infileBin;
	infileBin.open(fileName, ios::binary);

	infileBin.read((char *)&num_words_, sizeof(num_words_));
	infileBin.read((char *)&dimision_, sizeof(dimision_));


	for (int n = 0; n < num_words_; n++)
	{
		Mat words = Mat(1, dimision_, CV_32FC1);
		for (int i = 0; i < dimision_; i++)
		{
			float data;
			infileBin.read((char *)&data, sizeof(data));
			words.at<float>(0, i) = data;
		}

		codebook_.push_back(words);
	}

	infileBin.close();

	/*ofstream outfileTxt;
	outfileTxt.open("codebook2.txt");

	outfileTxt << num_words_ << endl;
	outfileTxt << dimision_ << endl;

	for (int n = 0; n < num_words_; n++)
	{
		for (int d = 0; d < dimision_; d++)
		{
			float data = codebook_[n].at<float>(0, d);
			outfileTxt << data << ",";
		}
		outfileTxt << endl;
	}

	outfileTxt.close();*/
}