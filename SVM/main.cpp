#include "preprocess.h"

#include <iostream>

using namespace std;
using namespace cv::ml;

Mat train_data;
Mat train_classes;
Mat test_data; // background must be white

void set_test_data(Mat &src);
void get_train_data(); 

int main(){		
	Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
 	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	get_train_data();
	
	train_data.convertTo(train_data, CV_32F);
	train_classes.convertTo(train_classes, CV_32S);
	
	svm->train(train_data, ROW_SAMPLE, train_classes);	
	
	//Mat src = imread("./test_data/43.jpg", 0);
	//Mat src = imread("./test_data/24.jpg", 0);
	//Mat src = imread("./test_data/05.jpg", 0);
	//Mat src = imread("./test_data/71.jpg", 0);
	//Mat src = imread("./test_data/34.jpg", 0);
	//Mat src = imread("./test_data/15.jpg", 0);
	//Mat src = imread("./test_data/23.jpg", 0);
	//Mat src = imread("./test_data/33.jpg", 0);
	//Mat src = imread("./test_data/42.jpg", 0);
	//Mat src = imread("./test_data/51.jpg", 0);
	//Mat src = imread("./test_data/61.jpg", 0);
	//Mat src = imread("./test_data/76.jpg", 0);
	//Mat src = imread("./test_data/81.jpg", 0);
	//Mat src = imread("./test_data/14.png", 0);
	//Mat src = imread("./test_data/13.png", 0);
	//Mat src = imread("./test_data/22.png", 0);
	//Mat src = imread("./test_data/04.png", 0);
	//Mat src = imread("./test_data/11.png", 0);
	//Mat src = imread("./test_data/32.png", 0);
	//Mat src = imread("./test_data/03.png", 0);
	//Mat src = imread("./test_data/91.png", 0);
	//Mat src = imread("./test_data/41.jpg", 0);
	//Mat src = imread("./test_data/31.jpg", 0);
	//Mat src = imread("./test_data/21.jpg", 0);
	//Mat src = imread("./test_data/01.jpg", 0);
	//Mat src = imread("./test_data/02.jpg", 0);
	//Mat src = imread("./test_data/12.png", 0);
	//Mat src = imread("./test_data/53.png", 0);
	//Mat src = imread("./test_data/52.jpg", 0);
	//Mat src = imread("./test_data/72.jpg", 0);
	//Mat src = imread("./test_data/73.jpg", 0);
	Mat src = imread("./test_data/74.jpg", 0);
	//Mat src = imread("./test_data/75.png", 0);
	
	if(!src.data){
		perror("read image failed\n");
	
	src = src < 100;
	set_test_data(src);

	test_data = src.reshape(0, 1);
	test_data.convertTo(test_data, CV_32F);
	
	auto r = svm->predict(test_data);
	cout << "result: "<< r << endl;
	
	waitKey(0);
	return 0;
}

void set_test_data(Mat &src){
	imshow("origin", src);
	preprocess3(src);
	preprocess(src);
	imshow("done", src);
	preprocess2(src);
}

void get_train_data(){
	char path[255] = "../train_data";
	for(int i = 0; i <= 9; ++i){
		for(int j = 1; j <= 9; ++j){
			char file[255];
			sprintf(file, "%s/%d%d.png", path, i, j);
			Mat temp = imread(file, 0);
			temp = temp < 100;
			preprocess(temp);
			preprocess2(temp);
			temp = temp.reshape(0, 1);
			temp.convertTo(temp, CV_32F);
			train_data.push_back(temp);
			train_classes.push_back(i);
		}
	}
	cout << "get data OK" << endl;
}
