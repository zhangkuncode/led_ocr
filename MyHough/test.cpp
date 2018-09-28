#include "MyHough.h"

/*
class MyHough{
public:
	Mat xg;
	Mat yg;
	Mat gsize;
	Mat gorigin;
	Mat gprocess;
	vector<vector<int>> vx;
	vector<vector<int>> vy;
	MyHough(const Mat &src, const int &piece);
	void process_gradient(const Mat &src, Mat &dst);
	void get_direction(const Mat &gg, const Mat &gsize, 
	                   float &angle, float &angle2, const int &piece);
private:
	void init_mat(const Mat &src, Mat &dst);
	void xy_gradient(const Mat &src, Mat &x_dst, Mat &y_dst);
	void gradient(const Mat &x_src, const Mat &y_src, 
			      Mat &gorigin_dst, Mat &gsize_dst);
};*/

int main(){
	Mat test = imread("/mnt/hgfs/USHARE/test_digit_origin/33.jpg", 0);	 
	test.convertTo(test, CV_32F);
	int piece = 10;
	MyHough hough = MyHough(test, piece);
	hough.process_gradient(hough.gorigin, hough.gprocess);
	float ag1 = 0, ag2 = 0;
	hough.get_direction(hough.gprocess, hough.gsize, ag1, ag2, hough.piece);

	//cout << "OK" << endl;
	//cout << hough.gprocess << endl;
	//cout << hough.gsize << endl;

	return 0;
}

