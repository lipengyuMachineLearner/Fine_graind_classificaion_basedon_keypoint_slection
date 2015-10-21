#include "CSIFT_Recognition.h"

void saveMat(Mat mat_data, string str_outputfile);
void loadMat(Mat &mat_data, string str_inputfile);

CSIFT_Recognition::CSIFT_Recognition()
{
	logfile.open("CSIFT_Recognition_logfile.txt");
}
void CSIFT_Recognition::train(vector<string> &vec_inputFile)
{
	cout << "SIFT descriptor getting & saving..." << endl;
	logfile << "SIFT descriptor getting & saving..." << endl;
	getSIFTDescriptor(vec_inputFile);
	cout << "SIFT descriptor getting complete" << endl;
	logfile << "SIFT descriptor getting complete" << endl;

	cout << "CodeBook getting..." << endl;
	logfile << "CodeBook getting..." << endl;
	codebook_.getCodebook(vec_inputFile, NUM_WORDS, 0);
	cout << "CodeBook getting complete" << endl;
	logfile << "CodeBook getting complete" << endl;

	cout << "CodeBook saving..." << endl;
	logfile << "CodeBook saving..." << endl;
	codebook_.saveCodebook(CODEBOOK_FILE);
	cout << "CodeBook saving complete" << endl;
	logfile << "CodeBook saving complete" << endl;
}


/******************************Get SIFT descriptor start******************************************/
void CSIFT_Recognition::getSIFTDescriptor(vector<string> &vec_inputFile)
{
	int num_file = vec_inputFile.size();
	for (int file_i = 0; file_i < num_file; file_i++)
	{
		IplImage *img_ori = cvLoadImage(vec_inputFile[file_i].c_str(), 0);
		IplImage *img_hist = cvCloneImage(img_ori);
		cvEqualizeHist(img_ori, img_hist);
		
		int width = img_ori->width;
		int height = img_ori->height;
		int step = img_ori->widthStep;
	
		if (file_i % 100 == 0)
			cout << "SIFT generating..." << file_i << " : " << num_file << endl;
		logfile << "SIFT generating..." << file_i << " : " << num_file << endl;
		CSIFTDescription description = SIFT_description(img_hist, 1);
		description.normalizationDescription(SIFT_DESCRIPTION_TYPE);
		if (file_i % 100 == 0)
			cout << "SIFT generating complete" << file_i << " : " << num_file << endl;
		logfile << "SIFT generating complete" << file_i << " : " << num_file << endl;

		if (file_i % 100 == 0)
			cout << "SIFT saving..." << file_i << " : " << num_file << endl;
		logfile << "SIFT saving..." << file_i << " : " << num_file << endl;

		string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".bin";
		description.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);
		if (file_i % 100 == 0)
			cout << "SIFT saving complete" << file_i << " : " << num_file << endl;
		logfile << "SIFT saving complete" << file_i << " : " << num_file << endl;

		cvReleaseImage(&img_ori);
		cvReleaseImage(&img_hist);
	}
}


CSIFTDescription CSIFT_Recognition::SIFT_description(IplImage *img, int sign_pyramid)
{
	CSIFTDescription description;

	//cout << "SIFT Point detecting..." << endl;

	SiftFeatureDetector detector;
	detector.detect(img, description.KeyPoint_loc_);
	//cout << "SIFT Point detecting complete : " << description.KeyPoint_loc_.size() << endl;

	//cout << "SIFT description generating..." << endl;
	SiftDescriptorExtractor extractor;
	extractor.compute(img, description.KeyPoint_loc_, description.mat_description_);
	//cout << "SIFT description generating complete" << endl;

	return description;
}
/******************************Get SIFT descriptor end******************************************/