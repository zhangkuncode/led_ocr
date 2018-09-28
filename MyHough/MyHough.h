#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class MyHough{
public:
	Mat gorigin;
	Mat gprocess;
	Mat xg;
	Mat yg;
	Mat gsize;
	int piece;
	vector<vector<int>> vx;
	vector<vector<int>> vy;
	MyHough(const Mat &src, const int piece);
	void process_gradient(const Mat &src, Mat &dst);
	void get_direction(const Mat &gg, const Mat &gsize, 
	                   float &angle, float &angle2, const int &piece);
private:
	void init_mat(const Mat &src, Mat &dst);
	void xy_gradient(const Mat &src, Mat &x_dst, Mat &y_dst);
	void gradient(const Mat &x_src, const Mat &y_src, 
			      Mat &gorigin_dst, Mat &gsize_dst);
};
