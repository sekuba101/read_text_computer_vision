#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include<opencv\cv.h>
#include<opencv\highgui.h>

#include <string>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;

typedef struct PointID
{
	uchar value;
	int ID;
};

typedef struct Segment_Params{
			int ID;
			int x_min;	
			int y_min;
			int x_y_min;
			int area;

};

class SegmentationAndOrder
{
public:
	
	SegmentationAndOrder();
	~SegmentationAndOrder();
	
	cv::Mat to_mat(cv::Mat& Mat_start, std::vector<std::vector<PointID>>& input);
	std::vector<std::vector<PointID>> get_segments(cv::Mat& input_mat);
	std::vector<Segment_Params>get_params(std::vector<std::vector<PointID>>& input);
	std::vector<int> read_ID(std::vector<std::vector<PointID>>& input);

private:
	
	std::vector<std::vector<uchar>>to_array(cv::Mat& input_mat);
	std::vector<std::vector<PointID>> init_point(cv::Mat& input_mat);

	void small_area_filter(std::vector<std::vector<PointID>>& input_img, std::vector<Segment_Params>& input_parameters, float filter_multiplier);
	void big_area_erosion(std::vector<std::vector<PointID>>& input_img, std::vector<Segment_Params>& input_parameters);
	void segment_numeration(std::vector<std::vector<PointID>>& input_img, std::vector<Segment_Params>& input_parameters);
	

		
};

