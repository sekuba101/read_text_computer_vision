#include "Numbers_Detector.h"


Numbers_Detector::Numbers_Detector()
{
	;
}
Numbers_Detector::~Numbers_Detector()
{

}

std::vector<std::vector < std::vector<std::vector< Point_XY>>>> Numbers_Detector::get_numbers(std::vector<std::vector<Point_ID>>& input_img, std::vector<int> IDs)
{
	std::vector<std::vector < std::vector<std::vector< Point_XY>>>> col;

	std::vector < std::vector<std::vector< Point_XY>>> row;
	std::vector<std::vector< Point_XY>> number;
	std::vector< Point_XY> tens;
	std::vector< Point_XY> unit;
	Point_XY output_help_3;
	//bool tens_dig


	for (size_t k = 1; k < IDs.size(); k += 2)
	{
		for (size_t i = 0; i < input_img.size() - 1; i++)
		{
			for (size_t j = 0; j < input_img.at(i).size() - 1; j++)
			{
				if (IDs.at(k) == input_img.at(i).at(j).ID)
				{
					output_help_3.x = j;
					output_help_3.y = i;
					output_help_3.ID = input_img.at(i).at(j).ID;
					tens.push_back(output_help_3);
				}

			}
		}

		for (size_t i = 0; i < input_img.size() - 1; i++)
		{
			for (size_t j = 0; j < input_img.at(i).size() - 1; j++)
			{
				if (IDs.at(k + 1) == input_img.at(i).at(j).ID)
				{
					output_help_3.x = j;
					output_help_3.y = i;
					output_help_3.ID = input_img.at(i).at(j).ID;
					unit.push_back(output_help_3);
				}

			}
		}
		number.push_back(tens);
		number.push_back(unit);


		if ((IDs.at(k) % 1000) - 1 == 2 * (int)row.size())
		{
			row.push_back(number);
			if (k + 2 == IDs.size())
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

cv::Mat Numbers_Detector::to_mat(cv::Mat& Mat_start, std::vector<Point_XY>& input)

{
	for (size_t i = 0; i < (Mat_start.size().height); i++)
	{
		for (size_t j = 0; j < (Mat_start.size().width); j++)
		{
			Mat_start.at<uchar>(i, j) = 0;  // row, col
		}

	}
	for (size_t i = 0; i < (input.size()); i++)
	{
		Mat_start.at<uchar>(input.at(i).y, input.at(i).x) = 255;  // row, col
	}
	return Mat_start;
}

cv::Mat Numbers_Detector::to_mat(std::vector<Point_XY>& input)
{
	Feautures help_feautures = get_one_feautuers(input);
	cv::Mat output = cv::Mat::zeros(help_feautures.y_max + 20, help_feautures.x_max + 20, CV_8U);

	for (size_t i = 0; i < (input.size()); i++)
	{
		output.at<uchar>((input.at(i).y - help_feautures.y_min + 10), (input.at(i).x - help_feautures.x_min + 10)) = 255;  // row, col
	}
	return output;
}

Feautures Numbers_Detector::get_one_feautuers(std::vector< Point_XY>& init)
{
	int hx_min, hy_x_min, hx_max, hy_x_max, hy_max, hx_y_max, hy_min, hx_y_min;
	int harea;
	float sum_x_mid_dist, sum_y_mid_dist;
	hx_min = 10000000000;
	hy_x_min = 0;
	hx_max = 0;
	hy_x_max = 0;
	hy_max = 0;
	hx_y_max = 0;
	hy_min = 100000000000;
	hx_y_min = 0;
	harea = 0;
	sum_x_mid_dist = 0;
	sum_y_mid_dist = 0;
	Feautures output;
	for (size_t l = 0; l < init.size(); l++)
	{
		harea++;
		//calculation of edges points
		if (init.at(l).x > hx_max)
		{
			hx_max = init.at(l).x;


		}
		if (init.at(l).x < hx_min)
		{
			hx_min = init.at(l).x;

		}
		if (init.at(l).y > hy_max)
		{

			hy_max = init.at(l).y;

		}
		if (init.at(l).y < hy_min)
		{
			hy_min = init.at(l).y;
		}
	}
	output.x_min = hx_min;
	output.y_min = hy_min;
	output.y_max = hy_max - hy_min;
	output.x_max = hx_max - hx_min;
	return output;
}

std::vector < std::vector < std::vector<int >>> Numbers_Detector::detector(std::vector<std::vector < std::vector<std::vector< Point_XY>>>>& input)
{
	std::vector < std::vector < std::vector<int >>> out = initialise_output(input);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	int factor_max = 0;
	int factor_min = 1000000;
	int factor_avr = 0;
	int help_1 = 0;
	int help_2 = 0;
	int help_3 = 0;
	for (size_t i = 0; i < out.size(); i++)
	{
		for (size_t j = 0; j < out.at(i).size(); j++)
		{
			for (size_t k = 0; k < out.at(i).at(j).size(); k++)
			{

				std::vector<Point_XY> in_put = input.at(i).at(j).at(k);
				cv::Mat im_input = to_mat(in_put);
				cv::resize(im_input, im_input, cv::Size(150, 100), 0, 0, CV_INTER_CUBIC);
				cv::threshold(im_input, im_input, 100, 255, cv::THRESH_BINARY);
				cv::dilate(im_input, im_input, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)), cv::Point(-1, -1), 3);

				cv::findContours(im_input, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

				for (int l = 0; l < contours.size(); l++)
				{

					help_1 += cv::contourArea(contours[l]);


				}
				if (help_1 > factor_max)
					factor_max = help_1;
				if (help_1 < factor_min)
					factor_min = help_1;
				factor_avr += help_1;
				help_1 = 0;
				help_3++;
				//cout << out.at(i).at(j).at(k) << "\t";

			}




		}
	}
	factor_avr = factor_avr / help_3;
	//cout << endl;
//cout << factor_avr << "\t" << factor_max << "\t" << factor_min <<  "\t" << (float)factor_avr /(float)factor_min<<endl;

	for (size_t i = 0; i < out.size(); i++)
	{
		for (size_t j = 0; j < out.at(i).size(); j++)
		{
			for (size_t k = 0; k < out.at(i).at(j).size(); k++)
			{
				std::vector<Point_XY> in_put = input.at(i).at(j).at(k);
				cv::Mat im_input = to_mat(in_put);
				cv::resize(im_input, im_input, cv::Size(100, 150), 0, 0, CV_INTER_CUBIC);
				cv::threshold(im_input, im_input, 100, 255, cv::THRESH_BINARY);
				cv::dilate(im_input, im_input, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)), cv::Point(-1, -1), 3);

				//if (detector8(im_input) == 0)
				//{


				cv::findContours(im_input, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

				for (int l = 0; l < contours.size(); l++)
				{
					help_2 += cv::contourArea(contours[l]);
				}
				if (help_2 > (factor_max * 0.95)) {
					//found 8 or 0
					if ((fill_percent(im_input, 40, 60, 70, 90) > 0.3f))
						out.at(i).at(j).at(k) = 8;
					else
						out.at(i).at(j).at(k) = 0;
				}
				else
				{
					// 1 2 3 4 5 6 7 9 
					if (help_2 > (factor_max * 0.85))
					{
						if ((fill_percent(im_input, 60, 75, 50, 60) > 0.5f))
							out.at(i).at(j).at(k) = 9;
						else
							out.at(i).at(j).at(k) = 6;
					}

					else
					{
						if (help_2 > (factor_max * 0.65))
							//found 5
							out.at(i).at(j).at(k) = 5;
						else
						{
							if (help_2 < (factor_max * 0.4))
								// 1 2 3 4  7  
								//this is possible only when 2 is cuted earlier (shape of cutted 2)
								out.at(i).at(j).at(k) = 2;
							else
							{
								// 1 2 3 4  7  
								if ((help_2 < (factor_max * 0.5)))
								{
									// 1 2 7 
									if (fill_percent(im_input, 50, 60, 50, 65) < 0.4f)
										out.at(i).at(j).at(k) = 2;
									else
									{
										if (fill_percent(im_input, 65, 75, 30, 40) > 0.8f)
											out.at(i).at(j).at(k) = 7;
										else
											out.at(i).at(j).at(k) = 1;
									}
								}
								else
								{
									//1 2 3 4

									if (fill_percent(im_input, 50, 60, 50, 65) < 0.4f)
										// 2 3
										if (fill_percent(im_input, 63, 73, 90, 100) < 0.5f)
											out.at(i).at(j).at(k) = 2;
										else
											out.at(i).at(j).at(k) = 3;
									else
										// 4 1
										if (fill_percent(im_input, 35, 45, 70, 80) < 0.5f)
											out.at(i).at(j).at(k) = 1;
										else
											out.at(i).at(j).at(k) = 4;


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

std::vector<std::vector< std::vector<int>>> Numbers_Detector::initialise_output(std::vector<std::vector < std::vector<std::vector< Point_XY>>>>& input)
{
	std::vector < std::vector < std::vector<int >>> help1;
	std::vector < std::vector<int >> help2;
	std::vector<int > help3;

	for (size_t i = 0; i < input.size(); i++)
	{
		for (size_t j = 0; j < input.at(i).size(); j++)
		{
			for (size_t k = 0; k < input.at(i).at(j).size(); k++)
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

std::vector < std::vector<int> > Numbers_Detector::get_result(std::vector < std::vector < std::vector<int >>> input)
{
	std::vector < std::vector<int> > col;
	std::vector<int> row;
	int number;
	for (size_t i = 0; i < input.size(); i++)
	{
		for (size_t j = 0; j < input.at(i).size(); j++)
		{
			number = 0;
			for (size_t k = 0; k < input.at(i).at(j).size(); k++)
			{
				if (k == 0)
					number += (10 * input.at(i).at(j).at(k));
				else
					number += input.at(i).at(j).at(k);
			}
			row.push_back(number);
			cout << number << "\t";
		}
		col.push_back(row);
		row.clear();
		cout << endl;
	}
	return col;
}

float Numbers_Detector::fill_percent(cv::Mat& input, int x_min, int x_max, int y_min, int y_max)
{
	float output = 0;
	float help_1 = 0;
	for (int i = y_min; i <= y_max; i++)
		for (int j = x_min; j <= x_max; j++) {
			if (input.at<uchar>(i, j) == 255)
			{
				output++;
				//input.at<uchar>(i, j) = 85; this might be used to visualize region we check
			}
			//else
			//input.at<uchar>(i, j) = 175;
			help_1++;
		}
	output = output / help_1;
	//cout << output << "\t";
	return output;
}
