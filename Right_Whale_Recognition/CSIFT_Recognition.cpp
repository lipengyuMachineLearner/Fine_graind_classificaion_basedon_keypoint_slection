#include "CSIFT_Recognition.h"

void saveMat(Mat mat_data, string str_outputfile);
void loadMat(Mat &mat_data, string str_inputfile);
string int2str(int val);
CSIFT_Recognition::CSIFT_Recognition() 
	:codebookSIFT_(NUM_WORDS_SIFT, KMEANS_STOP_ITERATION_SIFT, KMEANS_STOP_THRESHOLD_SIFT, SPLIT_STOP_ITERATION_SIFT,
	SPLIT_THRESHOLD_SIFT, MIN_INTIAL_SIFT, MAX_INTIAL_SIFT, MAX_NUM_POINT_SIFT, CODEBOOK_FILE_SIFT),
	CCodebookHoC_(NUM_WORDS_HOC, KMEANS_STOP_ITERATION_HOC, KMEANS_STOP_THRESHOLD_HOC, SPLIT_STOP_ITERATION_HOC,
	SPLIT_THRESHOLD_HOC, MIN_INTIAL_HOC, MAX_INTIAL_HOC, MAX_NUM_POINT_HOC, CODEBOOK_FILE_HOC)
{
	logfile.open("CSIFT_Recognition_logfile.txt");
}
void CSIFT_Recognition::train(vector<string> &vec_inputFile)
{
	/*cout << "SIFT && HoC descriptor getting & saving..." << endl;
	logfile << "SIFT && HoC descriptor getting & saving..." << endl;
	getDescriptor(vec_inputFile);
	cout << "SIFT && HoC descriptor getting complete" << endl;
	logfile << "SIFT && HoC descriptor getting complete" << endl;

	cout << "HoC CodeBook getting..." << endl;
	logfile << "HoC CodeBook getting..." << endl;
	CCodebookHoC_.getCodebook(vec_inputFile, HoC_DESCRIPTION_PATH, 0);
	cout << "HoC CodeBook getting complete" << endl;
	logfile << "HoC CodeBook getting complete" << endl;

	cout << "HoC CodeBook saving..." << endl;
	logfile << "HoC CodeBook saving..." << endl;
	CCodebookHoC_.saveCodebook(CODEBOOK_FILE_HOC);
	cout << "HoC CodeBook saving complete" << endl;
	logfile << "HoC CodeBook saving complete" << endl;*/

	cout << "SIFT CodeBook loading..." << endl;
	logfile << "SIFT CodeBook loading..." << endl;
	int num_keyWords = codebookSIFT_.loadCodebook(CODEBOOK_FILE_SIFT);
	cout << "SIFT CodeBook loading complete, num_keyWords=" << num_keyWords << endl;
	logfile << "SIFT CodeBook loading complete, num_keyWords=" << num_keyWords << endl;

	cout << "SIFT CodeBook getting..." << endl;
	logfile << "SIFT CodeBook getting..." << endl;
	codebookSIFT_.getCodebook(vec_inputFile, SIFT_DESCRIPTION_PATH, 0);
	cout << "SIFT CodeBook getting complete" << endl;
	logfile << "SIFT CodeBook getting complete" << endl;

	cout << "SIFT CodeBook saving..." << endl;
	logfile << "SIFT CodeBook saving..." << endl;
	codebookSIFT_.saveCodebook(CODEBOOK_FILE_SIFT);
	cout << "SIFT CodeBook saving complete" << endl;
	logfile << "SIFT CodeBook saving complete" << endl;
}

void CSIFT_Recognition::test(vector<string> &vec_inputFile, string codebokkFIle, string descriptoPath)
{
	/*cout << "SIFT descriptor getting & saving..." << endl;
	logfile << "SIFT descriptor getting & saving..." << endl;
	getSIFTDescriptor(vec_inputFile);
	cout << "SIFT descriptor getting complete" << endl;
	logfile << "SIFT descriptor getting complete" << endl;*/

	/*cout << "CodeBook loading..." << endl;
	logfile << "CodeBook loading..." << endl;
	CCodebookHoC_.loadCodebook(codebokkFIle);
	cout << "CodeBook loading complete" << endl;
	logfile << "CodeBook loading complete" << endl;

	cout << "CodeBook matching..." << endl;
	logfile << "CodeBook matching..." << endl;
	CCodebookHoC_.matchCodebook(vec_inputFile, descriptoPath);
	cout << "CodeBook matching complete" << endl;
	logfile << "CodeBook matching complete" << endl;*/

	cout << "CodeBook loading..." << endl;
	logfile << "CodeBook loading..." << endl;
	codebookSIFT_.loadCodebook(codebokkFIle);
	cout << "CodeBook loading complete" << endl;
	logfile << "CodeBook loading complete" << endl;

	cout << "CodeBook matching..." << endl;
	logfile << "CodeBook matching..." << endl;
	codebookSIFT_.matchCodebook(vec_inputFile, descriptoPath);
	cout << "CodeBook matching complete" << endl;
	logfile << "CodeBook matching complete" << endl;
}

void CSIFT_Recognition::keyPoint2Image(vector<string> &vec_inputFile, string path, string savePath, bool *uselessHoCPoint, int numHoCPoint)
{

	map<int, int> R, G, B;
	int num_file = vec_inputFile.size();

	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, 0.4, 0.4, 0, 1);
	for (int file_i = 0; file_i < num_file; file_i++)
	{
		IplImage *img_ori = cvLoadImage(vec_inputFile[file_i].c_str(), 1);
		IplImage *img_resize = cvCreateImage(cvSize(img_ori->width, img_ori->height), img_ori->depth, img_ori->nChannels);
		cvResize(img_ori, img_resize);
		IplImage *img_gray = cvCreateImage(cvSize(img_resize->width, img_resize->height), img_ori->depth, 1);
		cvCvtColor(img_resize, img_gray, CV_RGB2GRAY);
		IplImage *img_show = cvCreateImage(cvSize(img_resize->width, img_resize->height), img_ori->depth, 3);
		cvCvtColor(img_gray, img_show, CV_GRAY2RGB);

		int width = img_resize->width;
		int height = img_resize->height;
		int step = img_resize->widthStep;
		/*IplImage* redImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);
		IplImage* greenImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);
		IplImage* blueImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);

		cvSplit(img_resize, blueImage, greenImage, redImage, NULL);
		cvEqualizeHist(redImage, redImage);
		cvEqualizeHist(greenImage, greenImage);
		cvEqualizeHist(blueImage, blueImage);
		IplImage *img_hist = cvCreateImage(cvSize(img_resize->width, img_resize->height), img_ori->depth, img_ori->nChannels);
		cvMerge(blueImage, greenImage, redImage, NULL, img_hist);*/

		if (file_i % 100 == 0)
			cout << "DATA loading..." << file_i << " : " << num_file << endl << path << endl;
		logfile << "DATA loading..." << file_i << " : " << num_file << endl << path << endl;

		CDescription *description = NULL;
		if (path.find("sift") != string::npos)
		{
			description = new CSIFTDescription();
		}
		else if (path.find("hoc") != string::npos)
		{
			description = new CHoCDescription();
		}
		else
		{
			cout << "error in the type of codebook, it should be hoc or sift" << endl;
			logfile << "error in the type of codebook, it should be hoc or sift" << endl;
			exit(0);
		}

		string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = path + vec_inputFile[file_i].substr(pos1, pos2 - pos1) + ".bin";
		description->loadDescription(fileName);
		if (file_i % 100 == 0)
			cout << "DATA loading complete" << file_i << " : " << num_file << endl;
		logfile << "DATA loading complete" << file_i << " : " << num_file << endl;

		int num = description->KeyPoint_loc_.size();
		for (int n = 0; n < num; n++)
		{
			int label = description->indexCodebook_[n];
			if (uselessHoCPoint[label])
				continue;

			if (R.find(label) == R.end())
			{
				R[label] = ((float)rand()) / RAND_MAX * 255;
				G[label] = ((float)rand()) / RAND_MAX * 255;
				B[label] = ((float)rand()) / RAND_MAX * 255;
			}
			
			cvPutText(img_show, int2str(label).c_str(), cvPoint(description->KeyPoint_loc_[n].pt.x, description->KeyPoint_loc_[n].pt.y), &font, CV_RGB(R[label], G[label], B[label]));
			//cvCircle(img_gray, cvPoint(description->KeyPoint_loc_[n].pt.x, description->KeyPoint_loc_[n].pt.y), 3, CV_RGB(R[label], G[label], B[label]), 1, 8, 3);
			
			
		}
		/*Mat outImage;
		drawKeypoints(img_resize, description.KeyPoint_loc_, outImage);
		imshow("outimage",outImage);*/

		

		cvSaveImage((savePath + "\\" + vec_inputFile[file_i].substr(pos1, pos2 - pos1) + ".jpg").c_str(), img_show);

		
		/*cvReleaseImage(&blueImage);
		cvReleaseImage(&greenImage);
		cvReleaseImage(&redImage);
		cvReleaseImage(&img_hist);*/

		cvReleaseImage(&img_resize);
		cvReleaseImage(&img_ori);
		cvReleaseImage(&img_show);
		delete description;
	}
}
/******************************Get SIFT descriptor start******************************************/
void CSIFT_Recognition::getDescriptor(vector<string> &vec_inputFile)
{
	int num_file = vec_inputFile.size();
	for (int file_i = 0; file_i < num_file; file_i++)
	{
		IplImage *img_ori = cvLoadImage(vec_inputFile[file_i].c_str(), 1);
		IplImage *img_resize = cvCreateImage(cvSize(img_ori->width, img_ori->height), img_ori->depth, img_ori->nChannels);
		cvResize(img_ori, img_resize);

		int width = img_resize->width;
		int height = img_resize->height;
		int step = img_resize->widthStep;

		//SIFT
		IplImage *img_gray = cvCreateImage(cvSize(img_resize->width, img_resize->height), img_ori->depth, 1);
		cvCvtColor(img_resize, img_gray, CV_RGB2GRAY);

		if (file_i % 100 == 0)
			cout << "SIFT generating..." << file_i << " : " << num_file << endl;
		logfile << "SIFT generating..." << file_i << " : " << num_file << endl;
		CSIFTDescription description_sift;
		description_sift.Get_description(img_gray);
		description_sift.normalizationDescription(SIFT_DESCRIPTION_TYPE);
		if (file_i % 100 == 0)
			cout << "SIFT generating complete" << file_i << " : " << num_file << endl;
		logfile << "SIFT generating complete" << file_i << " : " << num_file << endl;

		if (file_i % 100 == 0)
			cout << "SIFT saving..." << file_i << " : " << num_file << endl;
		logfile << "SIFT saving..." << file_i << " : " << num_file << endl;

		string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".bin";
		description_sift.saveDescription(SIFT_DESCRIPTION_PATH + "\\" + fileName);
		if (file_i % 100 == 0)
			cout << "SIFT saving complete" << file_i << " : " << num_file << endl;
		logfile << "SIFT saving complete" << file_i << " : " << num_file << endl;

		//HoC
		IplImage* redImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);
		IplImage* greenImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);
		IplImage* blueImage = cvCreateImage(cvGetSize(img_resize), img_resize->depth, 1);

		cvSplit(img_resize, blueImage, greenImage, redImage, NULL);
		cvEqualizeHist(redImage, redImage);
		cvEqualizeHist(greenImage, greenImage);
		cvEqualizeHist(blueImage, blueImage);
		IplImage *img_hist = cvCreateImage(cvSize(img_resize->width, img_resize->height), img_ori->depth, img_ori->nChannels);
		cvMerge(blueImage, greenImage, redImage, NULL, img_hist);

		if (file_i % 100 == 0)
			cout << "HoC generating..." << file_i << " : " << num_file << endl;
		logfile << "HoC generating..." << file_i << " : " << num_file << endl;
		CHoCDescription description_hoc;
		description_hoc.Get_description(img_gray, img_hist);
		if (file_i % 100 == 0)
			cout << "HoC generating complete" << file_i << " : " << num_file << endl;
		logfile << "HoC generating complete" << file_i << " : " << num_file << endl;

		if (file_i % 100 == 0)
			cout << "HoC saving..." << file_i << " : " << num_file << endl;
		logfile << "HoC saving..." << file_i << " : " << num_file << endl;

		pos1 = vec_inputFile[file_i].rfind("\\");
		pos2 = vec_inputFile[file_i].rfind(".");
		fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".bin";
		description_hoc.saveDescription(HoC_DESCRIPTION_PATH + "\\" + fileName);
		if (file_i % 100 == 0)
			cout << "SIFT saving complete" << file_i << " : " << num_file << endl;
		logfile << "SIFT saving complete" << file_i << " : " << num_file << endl;
	/*	string::size_type pos1 = vec_inputFile[file_i].rfind("\\");
		string::size_type pos2 = vec_inputFile[file_i].rfind(".");
		string fileName = vec_inputFile[file_i].substr(pos1 + 1, pos2 - pos1 - 1) + ".jpg";
		cvSaveImage((SIFT_DESCRIPTION_PATH + "\\" + fileName).c_str(), img_hist);*/

		cvReleaseImage(&blueImage);
		cvReleaseImage(&greenImage);
		cvReleaseImage(&redImage);
		cvReleaseImage(&img_hist);

		cvReleaseImage(&img_resize);
		cvReleaseImage(&img_ori);
		
	}
}
/******************************Get SIFT descriptor end******************************************/