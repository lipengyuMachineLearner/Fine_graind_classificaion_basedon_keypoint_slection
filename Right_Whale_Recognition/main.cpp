#include "CSIFT_Recognition.h"


void saveMat(Mat mat_data, string str_outputfile);
void loadMat(Mat &mat_data, string str_inputfile);
void cheackPath(string path);
int getFiles(string rootPath, string format, vector<string> &files);
void checkConfig()
{
	cheackPath(SIFT_DESCRIPTION_PATH);
	cheackPath(HoC_DESCRIPTION_PATH);
	cheackPath(SIFT_IMAGE_PATH);
	cheackPath(HOC_IMAGE_PATH);
}

const int numHoCPoint = 155;
bool uselessHoCPoint[numHoCPoint];
void setUselessHoCPoint(bool *uselessHoCPoint);

void main()
{
	setUselessHoCPoint(uselessHoCPoint);
	

	CSIFT_Recognition recognition;
	vector<string> vec_inputFile;

	//vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\w_0_0.jpg");
	//vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\w_0.jpg");
	string rootPath = "D:\\kaggle\\Right_Whale_Recognition\\data\\imgs_ROI";
	string format = "*.jpg";
	int num = getFiles(rootPath, format, vec_inputFile);
	cout << "there are " << num << "files in the rootPath" << endl;
	checkConfig();
	//recognition.train(vec_inputFile);
	//recognition.test(vec_inputFile, CODEBOOK_FILE_SIFT, SIFT_DESCRIPTION_PATH);
	recognition.keyPoint2Image(vec_inputFile, HoC_DESCRIPTION_PATH, HOC_IMAGE_PATH, uselessHoCPoint, numHoCPoint);
}