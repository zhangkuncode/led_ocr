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
	MyHough(const Mat &src, const int piece);
	void process_gradient(const Mat &src, Mat &dst);
	void get_direction(const Mat &gg, const Mat &gsize, 
	                   float &angle, float &angle2, const int &piece);
private:
	void init_mat(const Mat &src, Mat &dst);
	void xy_gradient(const Mat &src, Mat &x_dst, Mat &y_dst);
	void gradient(const Mat &x_src, const Mat &y_src,
			      Mat &gorigin_dst, Mat &gsize_dst);
};*/

void MyHough::get_direction(const Mat &gg, const Mat &gsize, 
	                   float &angle, float &angle2, const int &piece){
	int ppiece = 180 / piece;
	for(int i = 0; i < gg.rows; ++i){
	 	for(int j = 0; j < gg.cols; ++j){
			float temp = gg.at<float>(i, j);
			float temp2 = gsize.at<float>(i, j);
			//cout << "temp/piece: " << temp << " " << piece;	
			if(temp != 0){
				if(temp == 180){
					vx[piece - 1].push_back(i);
					vy[piece - 1].push_back(j);
				} else { 
					int ttt = (int)(temp / ppiece);	
					//cout << "  ttt: " << ttt << endl;
					vx[ttt].push_back(i);
					vy[ttt].push_back(j);
				}
			}
		}
	}
	cout << "42" << endl;
	float dir[piece];
	float ssize[piece];
	int num[piece];
	for(int i = 0; i < piece; ++i){
		for(int j = 0; j < vx[i].size(); ++j){
			int tx = vx[i][j]; 
			int ty = vy[i][j]; 
			num[i]++;
			dir[i] += gg.at<float>(tx, ty);
			ssize[i] += gsize.at<float>(tx, ty);
		}
	}	
	cout << "num[]: ";
	for(auto val : num){
		cout << val << " ";
	}
	cout << endl;
	cout << "ssize[]: ";
	for(auto val : ssize){
		cout << val << " ";
	}
	cout << endl;
	
	int wmmax = ssize[0], wmax_num = 0;	
	for(int i = 0; i < piece; ++i){
		if(wmmax < ssize[i]){
			wmax_num = i;
			wmmax = ssize[i];
		}
	}
	int wsmax = ssize[1], wsec_num = 1;	
	for(int i = 0; i < piece; ++i){
		if(ssize[i] > wsmax && i != wmax_num){
			wsec_num = i;
			wsmax = ssize[i];
		}
	}
	
	float tt = 0, tt2 = 0;
	float deno = 0, deno2 = 0;
	for(int j = 0; j < vx[wmax_num].size(); ++j){
		float temp = gg.at<float>(wmax_num, j);
		float temp2 = gsize.at<float>(wmax_num, j);
		deno += temp2;
		tt += (temp2 * temp);
	}
	for(int j = 0; j < vx[wsec_num].size(); ++j){
		float temp = gg.at<float>(wsec_num, j);
		float temp2 = gsize.at<float>(wsec_num, j);
		deno2 += temp2;
		tt2 += (temp2 * temp);
	}
	
	tt /= deno;
	tt2 /= deno2;

	cout << "tt: " << tt << endl;
	cout << "tt2: " << tt2 << endl;
	cout << "w max num: " << wmax_num << endl;
	cout << "w sec num: " << wsec_num << endl;
	cout << "find direction OK" << endl;	
}

MyHough::MyHough(const Mat &src, const int piece_num):piece(piece_num){
	vx.resize(piece);
	vy.resize(piece);
	MyHough::init_mat(src, xg);
	MyHough::init_mat(src, yg);
	MyHough::init_mat(src, gsize);
	MyHough::init_mat(src, gorigin);
	MyHough::init_mat(src, gprocess);
	MyHough::xy_gradient(src, xg, yg);
	MyHough::gradient(xg, yg, gorigin, gsize);
}

void MyHough::process_gradient(const Mat &src, Mat &dst){
	for(int i = 0; i < src.rows; ++i){
	 	for(int j = 0; j < src.cols; ++j){
			float temp = src.at<float>(i, j);
			if(temp < 0){
				dst.at<float>(i, j) = 180 + temp;
			} else {
				dst.at<float>(i, j) = temp;
			}
		}
	} 
}

void MyHough::gradient(const Mat &x_src, const Mat &y_src,
		               Mat &gorigin_dst, Mat &gsize_dst){
	for(int i = 0; i < x_src.rows; ++i){
	 	for(int j = 0; j < x_src.cols; ++j){
			float a = x_src.at<float>(i, j);
			float b = y_src.at<float>(i, j);
			gorigin_dst.at<float>(i, j) = atan2(b, a) * 180 / CV_PI;
			gsize_dst.at<float>(i, j) = pow((pow(a, 2) + pow(b, 2)), 0.5);
		}
	}
}

void MyHough::xy_gradient(const Mat &src, Mat &dst, Mat &dst2){
	int m = src.rows;
	int n = src.cols;
	for(int i = 0; i < m; ++i){
		for(int j = 0; j < n; ++j){
			if(j == n-1){
				dst.at<float>(i, j) = 0;
			} else {
				dst.at<float>(i, j) = (src.at<float>(i,j) 
					- src.at<float>(i, j+1))*(src.at<float>(i,j));
			}		
			if(i == m-1){
				dst2.at<float>(i, j) = 0;
			} else {
				dst2.at<float>(i, j) = (src.at<float>(i, j) 
					 - src.at<float>(i+1, j))*(src.at<float>(i,j));
			}
		}
	}
}

void MyHough::init_mat(const Mat &src, Mat &dst){
	dst = src.clone();
	for(int i = 0; i < src.rows; ++i){
	 	for(int j = 0; j < src.cols; ++j){
			dst.at<float>(i, j) = 0;
		}
	}
}
