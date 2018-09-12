#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

void x_gradient(const Mat &src, Mat &dst);
void y_gradient(const Mat &src, Mat &dst);
void gradient(const Mat &x_dst, const Mat &y_dst, Mat &dst);
void my_init(Mat &src, Mat &src2, Mat &src3);
void find_direction(const Mat &src, float &dir1, float &dir2);

void hof(const Mat &src);

int main(){
	Mat test = imread("./test_data/33.jpg", 0);
	//Mat test = imread("./test_data/01.jpg", 0);
	
	imshow("origin", test);
	test.convertTo(test, CV_32F);
	
	Mat xg = test.clone();
	Mat yg = test.clone();
	Mat gg = test.clone();
	
	my_init(xg, yg, gg);

	x_gradient(test, xg);
	y_gradient(test, yg);
	gradient(xg, yg, gg);	
	//cout << gg << endl;	

	float obtuse = 0;
	float acute = 0;
	find_direction(gg, acute, obtuse);

/*	Point2f p1, p2;
	find_max(gg, p1, p2);
	circle(gg, p1, 10, Scalar(255, 255, 255));
	circle(gg, p2, 5, Scalar(255, 255, 255));	
	imshow("done", gg);
*/
	waitKey(0);
	return 0;
}

void hof(const  Mat &src){
	Mat canny, out, rect;
	vector<Vec2f> lines;
	Canny(src, canny, 3, 9);
	HoughLines(canny, lines, 1, CV_PI/180, 25);
	float angle = 0, angle2 = 0;
	int num = 0, num2 = 0;
	for(size_t i = 0; i < lines.size(); i++){
		float theta = lines[i][1];
		float temp = theta / CV_PI * 180;
		cout << lines[i][0] << "   " << temp << endl;
		if((temp < 50) && (temp > 0)){
			angle += temp;
			num++;
		} else if(temp > 90 && temp < 135){
			angle2 += temp;
			num2++;
		}
	}
	if(num != 0){
		angle = angle / num;
		angle = angle * CV_PI / 180;
	}

	if(num2 != 0){
		angle2 = angle2 / num2;
		angle2 = angle2 * CV_PI / 180;
	}
	cout << angle << endl;	
	cout << angle2 << endl;
	cout << "hof OK!" << endl;
}

void my_init(Mat &src, Mat &src2, Mat &src3){
	for(int i = 0; i < src.rows; ++i){
	 	for(int j = 0; j < src.cols; ++j){
			src.at<float>(i, j) = 0;
			src2.at<float>(i, j) = 0;
			src3.at<float>(i, j) = 0;
		}
	}
	cout << "my_init OK" << endl;
}

void find_direction(const Mat &src, float &dir1, float &dir2){
	int num1 = 0, num2 = 0;
	for(int i = 0; i < src.rows; ++i){
	 	for(int j = 0; j < src.cols; ++j){
			float temp = src.at<float>(i, j);
			if((temp > 0) && (temp < 45)){
				num1++;
				dir1 += temp;
			} else if((temp > 90) && (temp < 135)){
				num2++;
				dir2 += temp;
			} else {
				continue;
			}
		}
	}
	dir1 = dir1 / num1;
	dir2 = dir2 / num2;
	cout << "dir1: " << dir1 << endl;
	cout << "dir2: " << dir2 << endl;
	cout << "find direction OK" << endl;
}

void gradient(const Mat &x_dst, const Mat &y_dst, Mat &dst){
	for(int i = 0; i < x_dst.rows; ++i){
	 	for(int j = 0; j < x_dst.cols; ++j){
			float a = x_dst.at<float>(i, j);
			float b = y_dst.at<float>(i, j);
			dst.at<float>(i, j) = atan2(b, a) * 180 / CV_PI;
		}
	}
	cout << "gradient OK" << endl;
}

void x_gradient(const Mat &src, Mat &dst){
	int m = src.rows;
	int n = src.cols;
	for(int i = 0; i < m; ++i){
		for(int j = 0; j < n; ++j){
			if(j == n-1){
				dst.at<float>(i, j) = 0;
			} else {
				dst.at<float>(i, j) = (src.at<float>(i,j) - 
							           src.at<float>(i, j+1)) * 
						              src.at<float>(i, j);
			}
		}
	}
	cout << "x_gradient OK" << endl;
}

void y_gradient(const Mat &src, Mat &dst){
	int m = src.rows;
	int n = src.cols;
	for(int i = 0; i < m; ++i){
		for(int j = 0; j < n; ++j){
			if(i == m-1){
				dst.at<float>(i, j) = 0;
			} else {
				dst.at<float>(i, j) = (src.at<float>(i, j) - 
			                           src.at<float>(i+1, j)) * 
				                      src.at<float>(i, j);
			}
		}
	}
	cout << "y_gradient OK" << endl;
}

 
