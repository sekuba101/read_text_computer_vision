#include "NumbersDetector.h"


NumbersDetector::NumbersDetector()
{
	;
}

NumbersDetector::~NumbersDetector()
{

}

std::vector<std::vector < std::vector<std::vector< Point2DID>>>> NumbersDetector::get_numbers(std::vector<std::vector<PointID>>& input_img, std::vector<int>& IDs)
{
	std::vector<std::vector < std::vector<std::vector< Point2DID>>>> col;
	std::vector < std::vector<std::vector< Point2DID>>> row;
	std::vector<std::vector< Point2DID>> number;
	std::vector< Point2DID> tens;
	std::vector< Point2DID> unit;
	Point2DID output_help_3;

	for (size_t mark = 1; mark < IDs.size(); mark += 2)
	{
		for (size_t col = 0; col < input_img.size() - 1; col++)
		{
			for (size_t row = 0; row < input_img.at(col).size() - 1; row++)
			{
				if (IDs.at(mark) == input_img.at(col).at(row).ID)
				{
					output_help_3.x = row;
					output_help_3.y = col;
					output_help_3.ID = input_img.at(col).at(row).ID;
					tens.push_back(output_help_3);
				}

			}
		}

		for (size_t col = 0; col < input_img.size() - 1; col++)
		{
			for (size_t row = 0; row < input_img.at(col).size() - 1; row++)
			{
				if (IDs.at(mark + 1) == input_img.at(col).at(row).ID)
				{
					output_help_3.x = row;
					output_help_3.y = col;
					output_help_3.ID = input_img.at(col).at(row).ID;
					unit.push_back(output_help_3);
				}

			}
		}
		number.push_back(tens);
		number.push_back(unit);


		if ((IDs.at(mark) % 1000) - 1 == 2 * (int)row.size())
		{
			row.push_back(number);
			if (mark + 2 == IDs.size())
			{
				col.push_back(row);
				row.clear();
			}
		}

		else
		{
			col.push_back(row);
			row.clear();
			row.push_back(number);
		}

		//(row ended)
		tens.clear();
		unit.clear();
		number.clear();

	}


	return col;
}

cv::Mat NumbersDetector::to_mat(cv::Mat& Mat_start, std::vector<Point2DID>& input)
{
	const uchar black = 0;
	const uchar white = 255;
	for (size_t col = 0; col < (Mat_start.size().height); col++)
	{
		for (size_t row = 0; row < (Mat_start.size().width); row++)
		{
			Mat_start.at<uchar>(col, row) = black;  
		}

	}
	for (size_t p2D = 0; p2D < (input.size()); p2D++)
	{
		Mat_start.at<uchar>(input.at(p2D).y, input.at(p2D).x) = white;  
	}
	return Mat_start;
}

cv::Mat NumbersDetector::to_mat(std::vector<Point2DID>& input)
{
	Features help_feautures = get_one_featuers(input);
	cv::Mat output = cv::Mat::zeros(help_feautures.y_max + 20, help_feautures.x_max + 20, CV_8U);
	const uchar white = 255;

	for (size_t p2D = 0; p2D < (input.size()); p2D++)
	{
		output.at<uchar>((input.at(p2D).y - help_feautures.y_min + 10), (input.at(p2D).x - help_feautures.x_min + 10)) = white;  // row, col
	}
	return output;
}

Features NumbersDetector::get_one_featuers(std::vector< Point2DID>& init)
{
	int hx_min, hx_max, hy_max , hy_min ;
	int harea;
	float sum_x_mid_dist, sum_y_mid_dist;
	const int big_number = 10000000000; 
	hx_min = big_number;
	hx_max = 0;
	hy_max = 0;
	hy_min = big_number;
	
	harea = 0;
	sum_x_mid_dist = 0;
	sum_y_mid_dist = 0;
	Features output;
	for (size_t p2D = 0; p2D < init.size(); p2D++)
	{
		harea++;
		//calculation of edges points
		if (init.at(p2D).x > hx_max)
		{
			hx_max = init.at(p2D).x;


		}
		if (init.at(p2D).x < hx_min)
		{
			hx_min = init.at(p2D).x;

		}
		if (init.at(p2D).y > hy_max)
		{

			hy_max = init.at(p2D).y;

		}
		if (init.at(p2D).y < hy_min)
		{
			hy_min = init.at(p2D).y;
		}
	}
	output.x_min = hx_min;
	output.y_min = hy_min;
	output.y_max = hy_max - hy_min;
	output.x_max = hx_max - hx_min;
	return output;
}

std::vector < std::vector < std::vector<int >>> NumbersDetector::detect(std::vector<std::vector < std::vector<std::vector< Point2DID>>>>& input)
{
	std::vector < std::vector < std::vector<int >>> out = initialise_output(input);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	const int big_number = 10000000000;
	int factor_max = 0;
	int factor_min = big_number;
	int factor_avr = 0;
	int help_1 = 0;
	int help_2 = 0;
	int help_3 = 0;

	for (size_t col = 0; col < out.size(); col++)
	{
		for (size_t row = 0; row < out.at(col).size(); row++)
		{
			for (size_t digit = 0; digit < out.at(col).at(row).size(); digit++)
			{
				
				cv::Mat im_input = to_mat(input.at(col).at(row).at(digit));
				cv::resize(im_input, im_input, cv::Size(150, 100), 0, 0, CV_INTER_CUBIC);
				cv::threshold(im_input, im_input, 100, 255, cv::THRESH_BINARY);
				cv::dilate(im_input, im_input, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)), cv::Point(-1, -1), 3);
				cv::findContours(im_input, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

				for (int num = 0; num < contours.size(); num++)
				{
					help_1 += cv::contourArea(contours[num]);
				}
				if (help_1 > factor_max)
					factor_max = help_1;
				if (help_1 < factor_min)
					factor_min = help_1;
				factor_avr += help_1;
				help_1 = 0;
				help_3++;
			}
		}
	}

	factor_avr = factor_avr / help_3;

	for (size_t col = 0; col < out.size(); col++)
	{
		for (size_t row = 0; row < out.at(col).size(); row++)
		{
			for (size_t digit = 0; digit < out.at(col).at(row).size(); digit++)
			{
				std::vector<Point2DID> in_put = input.at(col).at(row).at(digit);
				cv::Mat im_input = to_mat(in_put);
				cv::resize(im_input, im_input, cv::Size(100, 150), 0, 0, CV_INTER_CUBIC);
				cv::threshold(im_input, im_input, 100, 255, cv::THRESH_BINARY);
				cv::dilate(im_input, im_input, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)), cv::Point(-1, -1), 3);
				cv::findContours(im_input, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

				for (int num = 0; num < contours.size(); num++)
				{
					help_2 += cv::contourArea(contours[num]);
				}

				if (help_2 > (factor_max * 0.95)) {
					//found 8 or 0
					if ((fill_percent(im_input, 40, 60, 70, 90) > 0.3f))
						out.at(col).at(row).at(digit) = 8;
					else
						out.at(col).at(row).at(digit) = 0;
				}
				else
				{
					// left 1 2 3 4 5 6 7 9 
					if (help_2 > (factor_max * 0.85))
					{
						// found 9 or 6
						if ((fill_percent(im_input, 60, 75, 50, 60) > 0.5f))
							out.at(col).at(row).at(digit) = 9;
						else
							out.at(col).at(row).at(digit) = 6;
					}
					else
					{
						// left 1 2 3 4 5 7 
						if (help_2 > (factor_max * 0.65))
							//found 5
							out.at(col).at(row).at(digit) = 5;
						else
						{
							if (help_2 < (factor_max * 0.4))
								// left 1 2 3 4 7  
								//this is possible only when 2 is cuted earlier (shape of cutted 2)
								out.at(col).at(row).at(digit) = 2;
							else
							{
								// 1 2 3 4 7  
								if ((help_2 < (factor_max * 0.5)))
								{
									// found 1 2 7 
									if (fill_percent(im_input, 50, 60, 50, 65) < 0.4f)
										out.at(col).at(row).at(digit) = 2;
									else
									{
										if (fill_percent(im_input, 65, 75, 30, 40) > 0.8f)
											out.at(col).at(row).at(digit) = 7;
										else
											out.at(col).at(row).at(digit) = 1;
									}
								}
								else
								{
									//found 1 2 3 4

									if (fill_percent(im_input, 50, 60, 50, 65) < 0.4f)
										// 2 3
										if (fill_percent(im_input, 63, 73, 90, 100) < 0.5f)
											out.at(col).at(row).at(digit) = 2;
										else
											out.at(col).at(row).at(digit) = 3;
									else
										// 4 1
										if (fill_percent(im_input, 35, 45, 70, 80) < 0.5f)
											out.at(col).at(row).at(digit) = 1;
										else
											out.at(col).at(row).at(digit) = 4;
								}
							}
						}
					}
				}
				help_2 = 0;
			}
		}
	}
	return out;
}

std::vector<std::vector< std::vector<int>>> NumbersDetector::initialise_output(std::vector<std::vector < std::vector<std::vector< Point2DID>>>>& input)
{
	std::vector < std::vector < std::vector<int >>> help1;
	std::vector < std::vector<int >> help2;
	std::vector<int > help3;

	for (size_t col = 0; col < input.size(); col++)
	{
		for (size_t row = 0; row < input.at(col).size(); row++)
		{
			for (size_t digit = 0; digit < input.at(col).at(row).size(); digit++)
			{
				help3.push_back(0);
			}
			help2.push_back(help3);
			help3.clear();
		}
		help1.push_back(help2);
		help2.clear();
	}
	return help1;
}

std::vector < std::vector<int> > NumbersDetector::get_result(std::vector < std::vector < std::vector<int >>>& input)
{
	std::vector < std::vector<int> > output;
	std::vector<int> row_var;
	int number;
	for (size_t col = 0; col < input.size(); col++)
	{
		for (size_t row = 0; row < input.at(col).size(); row++)
		{
			number = 0;
			for (size_t digit = 0; digit < input.at(col).at(row).size(); digit++)
			{
				if (digit == 0)
					number += (10 * input.at(col).at(row).at(digit));
				else
					number += input.at(col).at(row).at(digit);
			}
			row_var.push_back(number);
			cout << number << "\t";
		}
		output.push_back(row_var);
		row_var.clear();
		cout << endl;
	}
	return output;
}

float NumbersDetector::fill_percent(cv::Mat& input, int x_min, int x_max, int y_min, int y_max)
{
	float output = 0;
	float help_1 = 0;
	const uchar white = 255;
	for (int col = y_min; col <= y_max; col++){
		for (int row = x_min; row <= x_max; row++) {
			if (input.at<uchar>(col, row) == white)
			{
				output++;
				//input.at<uchar>(i, j) = 85; uncoment all to visualize region we check for the better settelment of the region
			}
			//else
			//input.at<uchar>(i, j) = 175; 
			help_1++;
		}
			
	}
	output = output / help_1;
	return output;
}
