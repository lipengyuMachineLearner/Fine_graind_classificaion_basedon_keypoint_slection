#include "CSIFT_Recognition.h"

void saveMat(Mat mat_data, string str_outputfile);
void loadMat(Mat &mat_data, string str_inputfile);

void CSIFT_Recognition::train(vector<string> &vec_inputFile)
{
	getSIFTDescriptor(vec_inputFile);
	codebook_.getCodebook(vec_inputFile, NUM_WORDS, 0);
	codebook_.saveCodebook(CODEBOOK_FILE);
	CCodebook tmp;
	tmp.loadCodebook(CODEBOOK_FILE);
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
	
		cout << "SIFT generating..." << endl;
		CSIFTDescription description = SIFT_description(img_hist, 1);
		description.normalizationDescription(SIFT_DESCRIPTION_TYPE);
		cout << "SIFT generating complete" << endl;

		cout << "SIFT saving..." << endl;
		string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".bin";
		description.saveSIFTDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);
		cout << "SIFT saving complete" << endl;

		cvReleaseImage(&img_ori);
		cvReleaseImage(&img_hist);
	}
}


CSIFTDescription CSIFT_Recognition::SIFT_description(IplImage *img, int sign_pyramid)
{
	CSIFTDescription description;

	cout << "SIFT Point detecting..." << endl;

	SiftFeatureDetector detector;
	detector.detect(img, description.KeyPoint_loc_);
	cout << "SIFT Point detecting complete : " << description.KeyPoint_loc_.size() << endl;

	//Mat res1;
	//int drawmode = DrawMatchesFlags::DRAW_RICH_KEYPOINTS;
	//drawKeypoints(img, vec_keypoints, res1, Scalar::all(-1), drawmode);//在内存中画出特征点
	//imshow("matches", res1);
	//cvWaitKey();

	cout << "SIFT description generating..." << endl;
	SiftDescriptorExtractor extractor;
	extractor.compute(img, description.KeyPoint_loc_, description.mat_description_);
	cout << "SIFT description generating complete" << endl;

	//saveMat(mat_descriptor, "");

	return description;
}
/******************************Get SIFT descriptor end******************************************/