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

#include "SegmentationAndOrder.h"

typedef struct Point2DID
{
	int x;
	int y;
	int ID;
	//bool tens_dig = 0;


};

typedef struct Features
{
	int x_min;
	int x_max;
	int y_max;
	int y_min;
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
 

class NumbersDetector
{
public:
	NumbersDetector();
	~NumbersDetector();
	
	std::vector<std::vector < std::vector<std::vector< Point2DID>>>> get_numbers(std::vector<std::vector<PointID>>& input_img, std::vector<int>& IDs);
	std::vector < std::vector < std::vector<int >>> detect(std::vector<std::vector < std::vector<std::vector< Point2DID>>>>& input);
	std::vector < std::vector<int> > get_result(std::vector < std::vector < std::vector<int >>>& input);
private:
	cv::Mat to_mat(cv::Mat& Mat_start, std::vector<Point2DID>& input);
	std::vector < std::vector < std::vector<int >>> initialise_output(std::vector<std::vector < std::vector<std::vector< Point2DID>>>>& input);
	cv::Mat to_mat(std::vector<Point2DID>& input);
	Features get_one_featuers(std::vector< Point2DID>& init);
	float fill_percent(cv::Mat& input, int x_min, int x_max, int y_min, int y_max);
	

};

