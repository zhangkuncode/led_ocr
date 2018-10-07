#include "MyHough.h"
#include <cstring>
/*
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
	int piece;
	vector<vector<int>> vx;
	vector<vector<int>> vy;
	void xy_gradient(const Mat &src);
	void gradient();
};
*/
MyHough::MyHough(const Mat &src, const int piece_num):piece(piece_num){
	xg = Mat(src.rows, src.cols, CV_32F);	
	yg = Mat(src.rows, src.cols, CV_32F);	
	gsize = Mat(src.rows, src.cols, CV_32F);	
	gprocess = Mat(src.rows, src.cols, CV_32F);	
	gorigin = Mat(src.rows, src.cols, CV_32F);	
	//cout << xg << endl;
	//cout << yg << endl;
	//cout << gsize << endl;
	//cout << gprocess << endl;
	//cout << gorigin << endl;
	vx.resize(piece);
	vy.resize(piece);
	//cout <<  << endl;
	MyHough::xy_gradient(src);
	MyHough::gradient();
}

void MyHough::xy_gradient(const Mat &src){
	int m = src.rows;
	int n = src.cols;
	for(int i = 0; i < m; ++i){
		for(int j = 0; j < n; ++j){
			// the boundary's gradient is 0
			if(j == n-1){
				xg.at<float>(i, j) = 0;
			} else {
				xg.at<float>(i, j) = (src.at<float>(i,j+1) 
					- src.at<float>(i, j))*(src.at<float>(i,j));
			}		
			if(i == m-1){
				yg.at<float>(i, j) = 0;
			} else {
				yg.at<float>(i, j) = (src.at<float>(i+1, j) 
					 - src.at<float>(i, j))*(src.at<float>(i,j));
			}
		}
	}
}

void MyHough::gradient(){
	for(int i = 0; i < xg.rows; ++i){
	 	for(int j = 0; j < xg.cols; ++j){
			float a = xg.at<float>(i, j);
			float b = yg.at<float>(i, j);
			float temp = atan2(b, a) * 180 / CV_PI;
			gorigin.at<float>(i, j) = temp;
			if(temp >= 0 && temp != 180){
				gprocess.at<float>(i, j) = temp;
			} else if(temp == 180){
				gprocess.at<float>(i, j) = 0;
			} else {
				// 将角度为负的 + 180
				gprocess.at<float>(i, j) = 180 + temp;
			}
			gsize.at<float>(i, j) = pow((pow(a, 2) + pow(b, 2)), 0.5);
			//if(gsize.at<float>(i, j) < 0){
			//	cout << "i,j" << i << " : " << j << endl;
			//}
		}
	}
}

void MyHough::get_direction(float &angle, float &angle2){
	float ppiece = (180 / (float)piece);
	for(int i = 0; i < gsize.rows; ++i){
	 	for(int j = 0; j < gsize.cols; ++j){
			float gg = gprocess.at<float>(i, j);
			float ggsize = gsize.at<float>(i, j);
			
			//cout << "temp/piece: " << temp << " " << piece;	
			
			if(ggsize != 0){// 梯度值不为0的都统计
				int index = (int)(gg / ppiece);	
				
				//cout << "  index: " << index << endl;
				
				vx[index].push_back(i);
				vy[index].push_back(j);
			}
		}
	}
	float dir[piece];// 每一个划分区间的角度大小之和
	memset(dir, 0, sizeof(dir));
	float ssize[piece];// 每一个划分区间的gradient大小之和
	memset(ssize, 0, sizeof(ssize));
	int num[piece];// 每一个划分区间有多少个点落在上面
	memset(dir, 0, sizeof(num));
	for(int i = 0; i < piece; ++i){
		num[i] = vx[i].size();
		for(int j = 0; j < vx[i].size(); ++j){
			int tx = vx[i][j]; 
			int ty = vy[i][j]; 
			dir[i] += gprocess.at<float>(tx, ty);
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
	
	// 根据ssize的大小，找到第一个大的区间
	int wmax = 0, wmax_num = 0;	
	for(int i = 0; i < piece; ++i){
		if(wmax < ssize[i]){
			wmax_num = i;
			wmax = ssize[i];
		}
	}
	// 根据ssize的大小，找到第2个大的区间
	int wsec = 0, wsec_num = 0;	
	for(int i = 0; i < piece; ++i){
		if(ssize[i] > wsec && i != wmax_num){
			wsec_num = i;
			wsec = ssize[i];
		}
	}
	
	cout << "w max num: " << wmax_num << endl;
	cout << "w sec num: " << wsec_num << endl;
	
	float tt = 0, tt2 = 0;
	// 遍历已确定范围了的点
	// 根据该点的梯度的值作为权重确定该范围内角度的加权平均值
	for(int j = 0; j < vx[wmax_num].size(); ++j){
		// gg: 每个点的梯度角度
		float gg = gprocess.at<float>(vx[wmax_num][j], vy[wmax_num][j]);
		float weight = gsize.at<float>(vx[wmax_num][j], vy[wmax_num][j]);
		tt += (weight * gg);
	}
	for(int j = 0; j < vx[wsec_num].size(); ++j){
		float gg = gprocess.at<float>(vx[wsec_num][j], vy[wsec_num][j]);
		float weight = gsize.at<float>(vx[wsec_num][j], vy[wsec_num][j]);
		tt2 += (weight * gg);
	}
	//ssize[]: 每一个划分区间的gradient大小之和
	cout << "ones: " << ssize[wmax_num] << endl;
	cout << "twos: " << ssize[wsec_num] << endl;	
	
	tt = tt / ssize[wmax_num];
	tt2 = tt2 / ssize[wsec_num];
	
	cout << "tt: " << tt << endl;
	cout << "tt2: " << tt2 << endl;
	cout << "find direction OK" << endl;	
}


