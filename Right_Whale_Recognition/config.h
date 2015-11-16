#pragma once
//const int STRID_KEYPOINT = 400;
const int NUM_WORDS_SIFT = 2048;
const int KMEANS_STOP_ITERATION_SIFT = 500;
const float KMEANS_STOP_THRESHOLD_SIFT = 0.01;
const int MAX_NUM_POINT_SIFT = 1000000;
const int SPLIT_STOP_ITERATION_SIFT = 25;
const float SPLIT_THRESHOLD_SIFT = 0.04;
const float MIN_INTIAL_SIFT = 1 / 128 * 0;
const float MAX_INTIAL_SIFT = 1 / 128 * 1;
const string CODEBOOK_FILE_SIFT = "codebook_sift.bin";

const int NUM_WORDS_HOC = 64;
const int KMEANS_STOP_ITERATION_HOC = 200;
const float KMEANS_STOP_THRESHOLD_HOC = 0.03;
const int MAX_NUM_POINT_HOC = 500000;
const int SPLIT_STOP_ITERATION_HOC = 10;
const float SPLIT_THRESHOLD_HOC = 0.09;
const float MIN_INTIAL_HOC = 1 / 128 * 0;
const float MAX_INTIAL_HOC = 1 / 128 * 1;
const string CODEBOOK_FILE_HOC = "codebook_hoc.bin";

const string SIFT_DESCRIPTION_PATH = "D:\\kaggle\\Right_Whale_Recognition\\data\\sift_description";
const string HoC_DESCRIPTION_PATH = "D:\\kaggle\\Right_Whale_Recognition\\data\\hoc_description";
const string SIFT_DESCRIPTION_TYPE = "L1";
const string SIFT_IMAGE_PATH = "D:\\kaggle\\Right_Whale_Recognition\\data\\sift_imgs";
const string HOC_IMAGE_PATH = "D:\\kaggle\\Right_Whale_Recognition\\data\\hoc_imgs";