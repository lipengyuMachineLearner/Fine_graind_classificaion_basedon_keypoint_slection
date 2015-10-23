#include "CCodebook.h"
#include <time.h>
#include "config.h"




CCodebook::CCodebook()
{
	sign_intial_ = true;
	num_words_ = 0;
	dimision_ = 0;

	logfile.open("CCodebook_logfile.txt");
}
void CCodebook::getCodebook(vector<string> &vec_inputFile, int num_words, int iter_split)
{
	int num_file = vec_inputFile.size();

	float cov = 0;
	std::cout << iter_split << "...:\n--------------------------------------------------------------------------------------" << std::endl;
	logfile << iter_split << "...:\n--------------------------------------------------------------------------------------" << std::endl;

	bool sign_num_sum = false;
	bool sign_split = false;
	float cov_pre = -1000000;

	for (int iter = 0; iter < KMEANS_STOP_ITERATION; iter++)
	{
		sum_ = Mat::zeros(num_words_, dimision_, CV_32FC1);
		num_sum_ = Mat::zeros(num_words_, 1, CV_32FC1);
		cov_ = Mat::zeros(num_words_, 1, CV_32FC1);

		sign_num_sum = false;
		sign_split = false;
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
				codebookIntial(num_words, dimision_, MIN_INTIAL, MAX_INTIAL, description);
			}

			if (dimision_ != description.mat_description_.cols)
			{
				std::cout << "(void CSIFT_Recognition::getCoodbook(vector<string> &vec_inputFile)) error in the dimision" << std::endl;
				logfile << "(void CSIFT_Recognition::getCoodbook(vector<string> &vec_inputFile)) error in the dimision" << std::endl;
				exit(0);
			}
			inputData(description);
			description.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);
		}

		cov_pre = cov;
		cov = updata(vec_inputFile);

		int sum_point = 0;
		std::cout << "iter = " << iter << "; cov = " << cov << "; num_words_ = " << num_words_ << " : ";
		logfile << "iter = " << iter << "; cov = " << cov << "; num_words_ = " << num_words_ << " : ";
		for (int w = 0; w < num_words_; w++)
		{
			logfile << num_sum_.at<float>(w, 0) << ",";
			sum_point += num_sum_.at<float>(w, 0);
			if (abs(num_sum_.at<float>(w, 0)) < 0.0000001)
				sign_num_sum = true;
		}
		std::cout << std::endl;
		logfile << std::endl;
		cout << "sum_point = " << sum_point << endl;
		logfile << "sum_point = " << sum_point << endl;

		if (cov < KMEANS_STOP_THRESHOLD)
		{
			cout << "cov < KMEANS_STOP_THRESHOLD : " << cov << "<" << KMEANS_STOP_THRESHOLD << " break iter" << endl;
			logfile << "cov < KMEANS_STOP_THRESHOLD : " << cov << "<" << KMEANS_STOP_THRESHOLD << " break iter" << endl;

			std::cout << "iter = " << iter << "; cov = " << cov << "; num_words_ = " << num_words_ << " : ";
			logfile << "iter = " << iter << "; cov = " << cov << "; num_words_ = " << num_words_ << " : ";
			int sum_point = 0;
			for (int w = 0; w < num_words_; w++)
			{
				logfile << num_sum_.at<float>(w, 0) << ",";
				sum_point += num_sum_.at<float>(w, 0);
				if (abs(num_sum_.at<float>(w, 0)) < 0.0000001)
					sign_num_sum = true;
			}
			std::cout << std::endl;
			logfile << std::endl;
			cout << "sum_point = " << sum_point << endl;
			logfile << "sum_point = " << sum_point << endl;

			break;
		}

		if (num_words_ <= 1 && sign_split == false)
		{ 
			std::cout << "num_words_ <= 1 " << num_words_ << std::endl;
			logfile << "num_words_ <= 1 " << num_words_ << std::endl;
			bool signSplit = splitCenter(vec_inputFile);
			if (signSplit == true)
			{
				/*sum_ = Mat::zeros(num_words_, dimision_, CV_32FC1);
				num_sum_ = Mat::zeros(num_words_, 1, CV_32FC1);
				cov_ = Mat::zeros(num_words_, 1, CV_32FC1);*/

				std::cout << "splitCenter == true" << std::endl;
				logfile << "splitCenter == true" << std::endl;
				sign_split = true;
			}
			else
			{
				std::cout << "splitCenter == false" << std::endl;
				logfile << "splitCenter == false" << std::endl;
				break;
			}
		}

		if (sign_num_sum && sign_split == false)
		{
			bool signDel = deleteCenter();
			//bool signSplit = splitCenter(vec_inputFile);
			if (signDel)
			{
				/*sum_ = Mat::zeros(num_words_, dimision_, CV_32FC1);
				num_sum_ = Mat::zeros(num_words_, 1, CV_32FC1);
				cov_ = Mat::zeros(num_words_, 1, CV_32FC1);*/
				logfile << "deleteCenter == true" << endl;
				sign_split = true;
				continue;
			}
		}

		if (abs(cov - cov_pre) < KMEANS_STOP_THRESHOLD / 1000 && sign_num_sum == false && sign_split == false)
		{
			std::cout << "abs(cov - cov_pre) < KMEANS_STOP_THRESHOLD / 1000 && sign_num_sum == false" << abs(cov - cov_pre) << " < " << KMEANS_STOP_THRESHOLD / 1000 << ", sign_num_sum = " << sign_num_sum <<", num_words_ : " << num_words_ << " , break " << std::endl;
			logfile << "abs(cov - cov_pre) < KMEANS_STOP_THRESHOLD / 1000 && sign_num_sum == false" << abs(cov - cov_pre) << " < " << KMEANS_STOP_THRESHOLD / 1000 << ", sign_num_sum = " << sign_num_sum << ", num_words_ : " << num_words_ << " , break " << std::endl;
			break;
		}
	}

	/*std::cout << "codebook : ";
	for (int w = 0; w < num_words_; w++)
	{
		for (int d = 0; d < dimision_; d++)
			std::cout << w << " : " << codebook_[w].at<float>(0, d) << ",";
		std::cout << std::endl;
	}*/

	std::cout <<iter_split << "complete\n--------------------------------------------------------------------------------------" << std::endl;
	std::cout << "cov = " << cov << std::endl;
	std::cout << "num_words_ = " << num_words_ << endl;

	logfile << iter_split << "complete\n--------------------------------------------------------------------------------------" << std::endl;
	logfile << "cov = " << cov << std::endl;
	logfile << "num_words_ = " << num_words_ << endl;

	bool signSplit = splitCenter(vec_inputFile);
	if (signSplit)
	{
		sum_ = Mat::zeros(num_words_, dimision_, CV_32FC1);
		num_sum_ = Mat::zeros(num_words_, 1, CV_32FC1);
		cov_ = Mat::zeros(num_words_, 1, CV_32FC1);
		cout << "splitCenter=" << signSplit << iter_split << endl;
		logfile << "splitCenter=" << signSplit << iter_split << endl;

		if (iter_split < SPLIT_STOP_ITERATION)
			getCodebook(vec_inputFile, num_words_, iter_split + 1);
	}
	else
	{
		cout << "do not need to split center" << endl;
		logfile << "do not need to split center" << endl;
	}
}




void CCodebook::codebookIntial(int num_words, int dimision, float min, float max, CSIFTDescription &data)
{
	if (sign_intial_ == false)
		return;
	

	num_words_ = num_words;
	dimision_ = dimision;
	srand((int)time(0));

	vector<int> index;
	int row = data.mat_description_.rows;
	for (int i = 0; i < row; i++)
		index.push_back(i);
	random_shuffle(index.begin(), index.end());

	num_words_ = num_words_ < row ? num_words_ : row;

	for (int i = 0; i < num_words_; i++)
	{
		Mat words = Mat(1, dimision_, CV_32FC1);

		data.mat_description_.row(index[i]).copyTo(words);
		
		codebook_.push_back(words);
	}

	/*for (int i = 0; i < num_words_; i++)
	{
		Mat words = Mat(1, dimision_, CV_32FC1);
		
		for (int j = 0; j < dimision_; j++)
		{
			float data = ((float)rand()) / RAND_MAX *(max - min) + min;
		
			words.at<float>(0, j) = data;
		}

		codebook_.push_back(words);
	}*/

	sum_ = Mat::zeros(num_words_, dimision_, CV_32FC1);
	num_sum_ = Mat::zeros(num_words_, 1, CV_32FC1);
	cov_ = Mat::zeros(num_words_, 1, CV_32FC1);

	sign_intial_ = false;

	/*cout << "-----------------------------------------------" << endl;
	for (int r = 0; r < num_words_; r++)
		for (int c = 0; c < dimision_; c++)
			cout << sum_.at<float>(0, c) << ",";
	cout << endl;*/
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

		/*for (int j = 0; j < dimision_; j++)
			std::cout << sum_.at<float>(r, j) <<  " , ";
		std::cout << std::endl << "###################################" << std::endl;*/
	}

	/*cout << "-----------------------------------------------" << endl;
	for (int r = 0; r < num_words_; r++)
		for (int c = 0; c < dimision_; c++)
			cout << sum_.at<float>(r, c) << ":" << description.at<float>(r, c) << " , ";
	cout << endl;*/
}

float CCodebook::updata(vector<string> &vec_inputFile)
{
	for (int i = 0; i < num_words_; i++)
	{
		if (abs(num_sum_.at<float>(i, 0)) < 1)
			codebook_[i] = sum_.row(i) *0;
		else
			codebook_[i] = sum_.row(i) *(1.0 / num_sum_.at<float>(i, 0));

		/*for (int j = 0; j < dimision_; j++)
			std::cout << codebook_[i].at<float>(0, j) << " , ";
		std::cout << std::endl << "###################################" << std::endl;*/
	}

	
	int num_file = vec_inputFile.size();
	float sum = 0;
	int num = 0;
	cov_ = Mat::zeros(num_words_, 1, CV_32FC1);

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
	vector<int> index_del;

	for (int n = 0; n < num_words_; n++)
	{
		if (cov_.at<float>(n, 0) > SPLIT_THRESHOLD && abs(num_sum_.at<float>(n, 0)) > 0.00001)
		{
			Mat words_new = Mat(1, dimision_, CV_32FC1);
			srand((int)time(0));
			for (int i = 0; i < dimision_; i++)
			{
				float data = ((float)rand()) / RAND_MAX * sqrt(cov_.at<float>(n, 0)) * 1.0 / dimision_ * 0.1;
				codebook_[n].at<float>(0, i) = codebook_[n].at<float>(0, i) + data;
				words_new.at<float>(0, i) = codebook_[n].at<float>(0, i) - data;
			}

			codebook_.push_back(words_new);
			result = true;
		}

		if (abs(num_sum_.at<float>(n, 0)) < 0.00001)
		{
			index_del.push_back(n);
			result = true;
		}
	}

	for (int i = index_del.size() - 1; i >= 0; i--)
	{
		result = true;
		codebook_.erase(codebook_.begin() + i);
	}

	num_words_ = codebook_.size();

	return result;
}

bool CCodebook::deleteCenter()
{
	bool result = false;
	vector<int> index_del;
	for (int n = 0; n < num_words_; n++)
	{
		if (abs(num_sum_.at<float>(n, 0)) < 0.00001)
			index_del.push_back(n);
	}

	for (int i = index_del.size() - 1; i >= 0; i--)
	{
		result = true;
		codebook_.erase(codebook_.begin() + i);
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

void CCodebook::matchCodebook(vector<string> &vec_inputFile)
{
	int num_file = vec_inputFile.size();
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
		}

		if (dimision_ != description.mat_description_.cols)
		{
			std::cout << "(void CSIFT_Recognition::getCoodbook(vector<string> &vec_inputFile)) error in the dimision" << std::endl;
			logfile << "(void CSIFT_Recognition::getCoodbook(vector<string> &vec_inputFile)) error in the dimision" << std::endl;
			exit(0);
		}
		
		if (file_i % 100 == 0)
			cout << "matchCodebook..." << file_i << " : " << num_file << endl;
		logfile << "matchCodebook..." << file_i << " : " << num_file << endl;
		Mat mat_description = description.mat_description_;

		int num_data = mat_description.rows;

		for (int r = 0; r < num_data; r++)
		{
			float min_distance = 100000;
			int min_index = -1;
			Mat mat_row = mat_description.row(r);
			for (int w = 0; w < num_words_; w++)
			{
				float distance = (mat_row - codebook_[w]).dot((mat_row - codebook_[w]));
				if (distance < min_distance)
				{
					min_distance = distance;
					min_index = w;
				}

			}
			description.indexCodebook_[r] = min_index;
		}

		description.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);

		if (file_i % 100 == 0)
			cout << "matchCodebook complete" << file_i << " : " << num_file << endl;
		logfile << "matchCodebook complete" << file_i << " : " << num_file << endl;
	}

}