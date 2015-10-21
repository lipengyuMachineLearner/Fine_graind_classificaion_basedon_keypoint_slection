#include "CSIFT_Recognition.h"


void saveMat(Mat mat_data, string str_outputfile);
void loadMat(Mat &mat_data, string str_inputfile);
void cheackPath(string path);
int getFiles(string rootPath, string format, vector<string> &files);
void checkConfig()
{
	cheackPath(SIFT_DESCRIPTION_PATH);
}

void main()
{
	CSIFT_Recognition recognition;
	vector<string> vec_inputFile;
	//vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\test.PNG");
	//vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\test2.PNG");
	string rootPath = "D:\\kaggle\\Right_Whale_Recognition\\data\\imgs_subset";
	string format = "*.jpg";
	int num = getFiles(rootPath, format, vec_inputFile);
	cout << "there are " << num << "files in the rootPath" << endl;
	checkConfig();
	recognition.train(vec_inputFile);

	/*CSIFTDescription des1, des2, des3;
	Mat mat1 = Mat(100, 2, CV_32FC1);
	Mat mat2 = Mat(100, 2, CV_32FC1);
	Mat mat3 = Mat(100, 2, CV_32FC1);
	vector<KeyPoint> loc1, loc2, loc3;
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
	float x3 = 0;
	float y3 = 0;

	for (int i = 0; i < 100; i++)
	{
		mat1.at<float>(i, 0) = 0.2 + (((float)rand()) / RAND_MAX - 0.5)*0.2;
		mat1.at<float>(i, 1) = 0.2 + (((float)rand()) / RAND_MAX - 0.5)*0.2;

		mat2.at<float>(i, 0) = 0.5 + (((float)rand()) / RAND_MAX - 0.5)*0.2;
		mat2.at<float>(i, 1) = 0.5 + (((float)rand()) / RAND_MAX - 0.5)*0.2;

		mat3.at<float>(i, 0) = 0.7 + (((float)rand()) / RAND_MAX - 0.5)*0.2;
		mat3.at<float>(i, 1) = 0.7 + (((float)rand()) / RAND_MAX - 0.5)*0.2;

		x1 += mat1.at<float>(i, 0);
		y1 += mat1.at<float>(i, 1);

		x2 += mat2.at<float>(i, 0);
		y2 += mat2.at<float>(i, 1);

		x3 += mat3.at<float>(i, 0);
		y3 += mat3.at<float>(i, 1);


		KeyPoint tmp;
		loc1.push_back(tmp);
		loc2.push_back(tmp);
		loc3.push_back(tmp);
	}

	mat1.copyTo(des1.mat_description_);
	des1.KeyPoint_loc_ = loc1;
	mat2.copyTo(des2.mat_description_);
	des2.KeyPoint_loc_ = loc2;
	mat3.copyTo(des3.mat_description_);
	des3.KeyPoint_loc_ = loc3;

	des1.saveSIFTDescription(SIFT_DESCRIPTION_PATH+"\\"+"des1.bin");
	des2.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + "des2.bin");
	des3.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + "des3.bin");

	vector<string> vec_inputFile;
	vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\des1.PNG");
	vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\des2.PNG");
	vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\des3.PNG");

	CCodebook code;
	code.getCodebook(vec_inputFile, NUM_WORDS, 0);

	cout << endl << "-------------------------------" << endl;
	cout << "result : " << endl;
	cout << x1 / 100 << " , " << y1 / 100 << endl;
	cout << x2 / 100 << " , " << y2 / 100 << endl;
	cout << x3 / 100 << " , " << y3 / 100 << endl;*/
}