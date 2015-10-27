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

void CSIFT_Recognition::test(vector<string> &vec_inputFile)
{
	/*cout << "SIFT descriptor getting & saving..." << endl;
	logfile << "SIFT descriptor getting & saving..." << endl;
	getSIFTDescriptor(vec_inputFile);
	cout << "SIFT descriptor getting complete" << endl;
	logfile << "SIFT descriptor getting complete" << endl;*/

	cout << "CodeBook loading..." << endl;
	logfile << "CodeBook loading..." << endl;
	codebook_.loadCodebook(CODEBOOK_FILE);
	cout << "CodeBook loading complete" << endl;
	logfile << "CodeBook loading complete" << endl;

	cout << "CodeBook matching..." << endl;
	logfile << "CodeBook matching..." << endl;
	codebook_.matchCodebook(vec_inputFile);
	cout << "CodeBook matching complete" << endl;
	logfile << "CodeBook matching complete" << endl;
}

void CSIFT_Recognition::keyPoint2Image(vector<string> &vec_inputFile)
{
	map<int, int> R, G, B;
	int num_file = vec_inputFile.size();
	for (int file_i = 0; file_i < num_file; file_i++)
	{
		IplImage *img_ori = cvLoadImage(vec_inputFile[file_i].c_str(), 0);
		IplImage *img_resize = cvCreateImage(cvSize(img_ori->width / 4, img_ori->height / 4), img_ori->depth, img_ori->nChannels);
		cvResize(img_ori, img_resize);
		IplImage *img_hist = cvCreateImage(cvSize(img_ori->width / 4, img_ori->height / 4), img_ori->depth, img_ori->nChannels);
		cvEqualizeHist(img_resize, img_hist);
		
		int width = img_resize->width;
		int height = img_resize->height;
		int step = img_resize->widthStep;

		if (file_i % 100 == 0)
			cout << "SIFT loading..." << file_i << " : " << num_file << endl;
		logfile << "SIFT loading..." << file_i << " : " << num_file << endl;
		CSIFTDescription description;
		string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string SIFTFileName = SIFT_DESCRIPTION_PATH + vec_inputFile[file_i].substr(pos1, pos2 - pos1) + ".bin";
		description.loadSIFTDescription(SIFTFileName);
		if (file_i % 100 == 0)
			cout << "SIFT loading complete" << file_i << " : " << num_file << endl;
		logfile << "SIFT loading complete" << file_i << " : " << num_file << endl;

		int num = description.KeyPoint_loc_.size();

		IplImage *img_show = cvCreateImage(cvSize(width, height), img_hist->depth, 3);
		cvCvtColor(img_resize, img_show, CV_GRAY2BGR);
		for (int n = 0; n < num; n++)
		{
			int label = description.indexCodebook_[n];
			if (R.find(label) == R.end())
			{
				R[label] = ((float)rand()) / RAND_MAX * 255;
				G[label] = ((float)rand()) / RAND_MAX * 255;
				B[label] = ((float)rand()) / RAND_MAX * 255;
			}

			cvCircle(img_show, cvPoint(description.KeyPoint_loc_[n].pt.x, description.KeyPoint_loc_[n].pt.y), 3, CV_RGB(R[label], G[label], B[label]), 1, 8, 3);
		}
		/*Mat outImage;
		drawKeypoints(img_resize, description.KeyPoint_loc_, outImage);
		imshow("outimage",outImage);*/

		
		//cvShowImage("df", img_show);
		//cvWaitKey(0);

		cvSaveImage((SIFT_IMAGE_PATH + "\\" + vec_inputFile[file_i].substr(pos1, pos2 - pos1) + ".jpg").c_str(), img_show);
		cvReleaseImage(&img_resize);
		cvReleaseImage(&img_ori);
		cvReleaseImage(&img_hist); 
		cvReleaseImage(&img_show);
	}
}
/******************************Get SIFT descriptor start******************************************/
void CSIFT_Recognition::getSIFTDescriptor(vector<string> &vec_inputFile)
{
	int num_file = vec_inputFile.size();
	for (int file_i = 0; file_i < num_file; file_i++)
	{
		IplImage *img_ori = cvLoadImage(vec_inputFile[file_i].c_str(), 0);
		IplImage *img_resize = cvCreateImage(cvSize(img_ori->width / 4, img_ori->height / 4), img_ori->depth, img_ori->nChannels);
		cvResize(img_ori, img_resize);
		/*IplImage* redImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);
		IplImage* greenImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);
		IplImage* blueImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);

		cvSplit(img_resize, blueImage, greenImage, redImage, NULL);
		cvEqualizeHist(redImage, redImage);
		cvEqualizeHist(greenImage, greenImage);
		cvEqualizeHist(blueImage, blueImage);
		IplImage *img_hist = cvCreateImage(cvSize(img_resize->width, img_resize->height), img_ori->depth, img_ori->nChannels);
		cvMerge(blueImage, greenImage, redImage, NULL, img_hist);*/

		int width = img_resize->width;
		int height = img_resize->height;
		int step = img_resize->widthStep;
	
		if (file_i % 100 == 0)
			cout << "SIFT generating..." << file_i << " : " << num_file << endl;
		logfile << "SIFT generating..." << file_i << " : " << num_file << endl;
		CSIFTDescription description;
		description.GetSIFT_description(img_resize, 1, DENSESAMPLE_STRIDE, NUM_SCALAR, SCALAR);////////////important
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



	/*	string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".jpg";
		cvSaveImage((SIFT_DESCRIPTION_PATH + "\\" + fileName).c_str(), img_hist);*/

		/*cvReleaseImage(&blueImage);
		cvReleaseImage(&greenImage);
		cvReleaseImage(&redImage);
		cvReleaseImage(&img_hist);*/

		cvReleaseImage(&img_resize);
		cvReleaseImage(&img_ori);
		
	}
}
/******************************Get SIFT descriptor end******************************************/