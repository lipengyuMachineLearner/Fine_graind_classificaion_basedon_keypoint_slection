#include "CSIFT_Recognition.h"


void saveMat(Mat mat_data, string str_outputfile);
void loadMat(Mat &mat_data, string str_inputfile);
void cheackPath(string path);

void checkConfig()
{
	cheackPath(SIFT_DESCRIPTION_PATH);
}

void main()
{
	CSIFT_Recognition recognition;
	vector<string> vec_inputFile;
	vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\test.PNG");
	vec_inputFile.push_back("D:\\kaggle\\Right_Whale_Recognition\\data\\test2.PNG");
	checkConfig();
	recognition.train(vec_inputFile);

	//Mat tmp;
	//loadMat(tmp, "test.bin");
	//CSIFTDescription cc;
	//cc.loadSIFTDescription("description.bin");
}