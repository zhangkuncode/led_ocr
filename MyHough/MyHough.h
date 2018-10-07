#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class MyHough{
public:
	Mat gprocess;
	MyHough(const Mat &src, const int piece);
	void get_direction(float &angle, float &angle2);
private:

	Mat gorigin;
	Mat xg;
	Mat yg;
	Mat gsize;
	const int piece;
	vector<vector<int>> vx;
	vector<vector<int>> vy;
	
	//计算每个点x，y方向的梯度值，并存储在(Mat)xg, yg中
	void xy_gradient(const Mat &src);
	
	//计算每个点x，y方向的梯度值的和,类似欧式距离公式
	void gradient();
};







