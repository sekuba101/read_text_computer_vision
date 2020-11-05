#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include<opencv\cv.h>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include<opencv2/ml/ml.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>

#include "segmentation.h"

typedef struct Point_XY
{
	int x;
	int y;
	int ID;
	//bool tens_dig = 0;


};

typedef struct Feautures
{
	int x_min;
	int y_x_min;
	int x_max;
	int y_x_max;
	int y_max;
	int x_y_max;
	int y_min;
	int x_y_min;
	int area;
	int x_mid;
	int y_mid;
	float avr_dist_from_mid_x;
	float avr_dist_from_mid_y;
	double scale;
	float difference;
	double mid_dist_avr;
	//int obwod;
};


class Numbers_Detector
{
public:
	Numbers_Detector();
	~Numbers_Detector();
	
	std::vector<std::vector < std::vector<std::vector< Point_XY>>>> get_numbers(std::vector<std::vector<Point_ID>>& input_img, std::vector<int> IDs);
	std::vector < std::vector < std::vector<int >>> detector(std::vector<std::vector < std::vector<std::vector< Point_XY>>>>& input);
	std::vector < std::vector<int> > get_result(std::vector < std::vector < std::vector<int >>> input);
private:
	cv::Mat to_mat(cv::Mat& Mat_start, std::vector<Point_XY>& input);
	std::vector < std::vector < std::vector<int >>> initialise_output(std::vector<std::vector < std::vector<std::vector< Point_XY>>>>& input);
	cv::Mat to_mat(std::vector<Point_XY>& input);
	Feautures get_one_feautuers(std::vector< Point_XY>& init);
	float fill_percent(cv::Mat& input, int x_min, int x_max, int y_min, int y_max);
	

};

