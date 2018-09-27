#include "preprocess.h"

#include <iostream>
using namespace std;

/** input data background must be black */
void preprocess(Mat &src){
	int x_min = 0, y_min = 0, x_max = 0, y_max = 0;
	Scalar max = Scalar(src.cols * 255);
	Mat data;
	// find x
	int xminFound = 0;
	for(int i = 0; i < src.cols; ++i){
		data = src.colRange(i, i + 1);
		Scalar sss = sum(data);
		if(sss.val[0] > 0){
			x_max = i;
			if(!xminFound){
				x_min = i;
				xminFound = 1;
			}
		}
	}
	// find y
	int yminFound = 0;
	for(int i = 0; i < src.rows; ++i){
		data = src.rowRange(i, i + 1);
		Scalar sss = sum(data);
		if(sss.val[0] > 0){
			y_max = i;
			if(!yminFound){
				y_min = i;
				yminFound = 1;
			}
		}
	}
	int height = y_max - y_min;
	int weight = x_max - x_min;
	if((weight < (height / 2)) && ((height / 2) < (src.cols - x_min))){
		src = src(Rect(x_min, y_min, height / 2, height));
		src = src.clone();
		resize(src, src, Size(src.cols / 2, src.rows));
	} else if((weight < (height / 2)) && ((height / 2) >= (src.cols - x_min))){
		src = src(Rect(x_min, y_min, src.cols - x_min, height));
		src = src.clone();
		resize(src, src, Size(src.cols / 2, src.rows));
	} else {
		src = src(Rect(x_min, y_min, weight, height));
		src = src.clone();
		resize(src, src, Size(src.cols / 2, src.rows));
	}
}

/** Dimensionality reduction for input-features */
void preprocess2(Mat &src){
	Scalar ss;
	for(int i = 0; i < (src.rows / 3); ++i){
		ss += sum(src.rowRange(i, i+1));
	}
	
	Scalar ss1;
	for(int i = (src.rows / 3); i < (src.rows / 3) * 2; ++i){
		ss1 += sum(src.rowRange(i, i+1));
	}

	Scalar ss2;
	for(int i = (src.rows / 3) * 2; i < src.rows; ++i){
		ss2 += sum(src.rowRange(i, i+1));
	}
	
	Mat o1 = src(Rect(0, 0, src.cols / 2, src.rows / 2));
	Scalar ss3;
	for(int i = 0; i < o1.rows; ++i){
		ss3 += sum(o1.rowRange(i, i+1));
	}
	
	Mat o2 = src(Rect(src.cols / 2, 0, src.cols / 2, src.rows / 2));
	Scalar ss4;
	for(int i = 0; i < o2.rows; ++i){
		ss4 += sum(o2.rowRange(i, i+1));
	}
	
	Mat o3 = src(Rect(0, src.rows / 2, src.cols / 2, src.rows / 2));
	Scalar ss5;
	for(int i = 0; i < o3.rows; ++i){
		ss5 += sum(o3.rowRange(i, i+1));
	}

	Mat o4 = src(Rect(src.cols / 2, src.rows / 2, 
				      src.cols / 2, src.rows / 2));
	Scalar ss6;
	for(int i = 0; i < o4.rows; ++i){
		ss6 += sum(o4.rowRange(i, i+1));
	}
	
	double dd[7] = {ss.val[0],  ss1.val[0], ss2.val[0], 
		            ss3.val[0], ss4.val[0], ss5.val[0], ss6.val[0]};
	double max = dd[0];
	for(int i = 1; i < 7; ++i){
		if(dd[i] > dd[i - 1]){
			max = dd[i];
		}
	}
	float arr[7][1] ={static_cast<float>(ss.val[0]  / max),  
		              static_cast<float>(ss1.val[0] / max), 
					  static_cast<float>(ss2.val[0] / max), 
					  static_cast<float>(ss3.val[0] / max), 
		              static_cast<float>(ss4.val[0] / max), 
					  static_cast<float>(ss5.val[0] / max), 
					  static_cast<float>(ss6.val[0] / max)} ;
	Mat out = Mat(7, 1, CV_32FC1, arr); 
	src = out.clone();
}

void preprocess3(Mat &src){
	Mat canny, rect;
	vector<Vec2f> lines;
	Canny(src, canny, 3, 9);
	HoughLines(canny, lines, 1, CV_PI/180, 25);
	float angle = 0, angle2 = 0;
	int num = 0, num2 = 0;
	for(size_t i = 0; i < lines.size(); i++){
		float theta = lines[i][1];// radian
		float temp = theta / CV_PI * 180;// angle
		if((temp < 50) && (temp > 0)){
			angle += temp;
			num++;
		} else if((temp > 90) && (temp < 140)){
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
	float x_cdt = 40, y_cdt = 40, rate = 20;
	Point2f pdst[3] = {Point2f(x_cdt, y_cdt), Point2f(x_cdt, y_cdt-rate), 
					   Point2f(x_cdt+rate, y_cdt)};
	if(num2 != 0){
		Point2f psrc[3] = {Point2f(x_cdt, y_cdt), 
					       Point2f(x_cdt+rate*sin(angle),y_cdt-rate*cos(angle)),
					       Point2f(x_cdt+rate*sin(CV_PI-angle2),
								   y_cdt+rate*cos(CV_PI-angle2))};
		rect = getAffineTransform(psrc, pdst);
	} else {
		Point2f psrc[3] = {Point2f(x_cdt, y_cdt), 
					       Point2f(x_cdt+rate*sin(angle),y_cdt-rate*cos(angle)),
					       Point2f(x_cdt+rate,y_cdt)};	
		rect = getAffineTransform(psrc, pdst);
	}
	warpAffine(src, src, rect, src.size());	
}
