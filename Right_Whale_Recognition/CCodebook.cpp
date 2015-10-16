#include "CCodebook.h"
#include <time.h>
#include "config.h"

void CCodebook::getCodebook(vector<string> &vec_inputFile, int num_words, int iteration)
{
	int num_file = vec_inputFile.size();
	int dimision = 0;

	for (int file_i = 0; file_i < num_file; file_i++)
	{
		string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".bin";

		CSIFTDescription description;
		description.loadSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);

		if (dimision == 0)
		{
			dimision = description.mat_description_.cols;
			codebookIntial(num_words, dimision, 0, 1);
		}
		else
		{
			if (dimision != description.mat_description_.cols)
			{
				std::cout << "(void CSIFT_Recognition::getCoodbook(vector<string> &vec_inputFile)) error in the dimision" << std::endl;
				exit(0);
			}
			inputData(description);
			description.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);
		}
	}
	float cov = updata(vec_inputFile);
	if (cov > KMEANS_STOP_THRESHOLD && iteration < KMEANS_STOP_ITERATION)
		getCodebook(vec_inputFile, num_words_, iteration + 1);
	else
	{
		bool sign = splitCenter(vec_inputFile);
		if (sign)
			getCodebook(vec_inputFile, num_words_, 0);
	}
}

void CCodebook::codebookIntial(int num_words, int dimision, float min, float max)
{
	if (sign_intial_ == false)
		return;
	

	num_words_ = num_words;
	dimision_ = dimision;
	for (int i = 0; i < num_words_; i++)
	{
		Mat words = Mat(1, dimision_, CV_32FC1);
		srand((int)time(0));
		for (int i = 0; i < dimision_; i++)
		{
			float data = ((float)rand()) / RAND_MAX *(max - min) + min;
			words.at<float>(0, i) = data;

			std::cout << words.at<float>(0, i) << std::endl;

			codebook_.push_back(words);
		}
	}

	Mat sum_ = Mat(num_words_, dimision_, CV_32FC1) * 0;
	Mat num_sum_ = Mat(num_words_, 1, CV_32FC1) * 0;
	cov_ = Mat(num_words_, 1, CV_32FC1) * 0;

	sign_intial_ = false;
}

void CCodebook::inputData(CSIFTDescription &data)
{
	Mat description = data.mat_description_;

	int num_data = description.rows;
	
	for (int r = 0; r < num_data; r++)
	{
		int min_distance = 100000;
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
		codebook_[i] = sum_.row(i) * (1.0 / num_sum_.at<float>(i, 0));
	}

	
	int num_file = vec_inputFile.size();
	float sum = 0;
	int num = 0;

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
			cov_.at<float>(index, 0) = (rows - codebook_[index]).dot((rows - codebook_[index]));

			sum = (rows - codebook_[index]).dot((rows - codebook_[index]));
			num += dimision_;
		}
	}

	return sum / num;
}
bool CCodebook::splitCenter(vector<string> &vec_inputFile)
{
	bool result = false;



	return result;
}