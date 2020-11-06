#include "SegmentationAndOrder.h"

SegmentationAndOrder::SegmentationAndOrder()
{
	;
}

std::vector<std::vector<uchar>> SegmentationAndOrder::to_array(cv::Mat& input_mat)
{
	std::vector<std::vector<uchar>> output;
	std::vector<uchar> help_vector;
	uchar help_uchar = 0;
	for (size_t row = 0; row < (input_mat.size().height); row++)
	{
		for (size_t col = 0; col < (input_mat.size().width); col++)
		{
			help_uchar = input_mat.at<uchar>(row, col);  
			help_vector.push_back(help_uchar);
		}
		output.push_back(help_vector);
		help_vector.clear();
	}
	return output;
}

cv::Mat SegmentationAndOrder::to_mat(cv::Mat & Mat_start, std::vector<std::vector<PointID>>& input)

{

	for (size_t row = 0; row < (Mat_start.size().height); row++)
	{
		for (size_t col = 0; col < (Mat_start.size().width); col++)
		{
			Mat_start.at<uchar>(row, col) = input.at(row).at(col).value;  // row, col
		}

	}
	return Mat_start;
}

std::vector<std::vector<PointID>> SegmentationAndOrder::init_point(cv::Mat& input)
{
	std::vector<std::vector<PointID>> output;
	std::vector<PointID> help_vector;
	PointID point;
	const uchar white = 255;
	const int zero_id = 0;
	int var_1 = 1;
	std::vector<std::vector<uchar>> input_vec = to_array(input);
	for (size_t col = 0; col < input_vec.size(); col++)
	{
		for (size_t row = 0; row < input_vec.at(col).size(); row++)
		{
			point.value = input_vec.at(col).at(row);
			if (point.value == white)
			{
				point.ID = var_1;
				var_1++;
			}
			else
				point.ID = zero_id;
			help_vector.push_back(point);
		}
		output.push_back(help_vector);
		help_vector.clear();
	}
	return output;
}

std::vector<int> SegmentationAndOrder::read_ID(std::vector<std::vector<PointID>> &input)
{
	std::vector<int> output;
	int help_var = 0;
	output.push_back(help_var);
	bool help_bool = 0;
	for (size_t col = 0; col < input.size() - 1; col++)
	{
		for (size_t row = 0; row < input.at(col).size() - 1; row++)
		{

			if (help_var = input.at(col).at(row).ID != 0)
			{
				help_bool = 0;
				for (size_t k = 0; k < output.size(); k++)
				{
					if (input.at(col).at(row).ID == output.at(k))
						help_bool = 1;
				}
				if (help_bool == 0)
					output.push_back(input.at(col).at(row).ID);
			}
		}
	}
	return output;
}

std::vector<Segment_Params>SegmentationAndOrder::get_params(std::vector<std::vector<PointID>> &input)
{
	std::vector<Segment_Params> output;
	Segment_Params help_output;
	float help_f1, help_f2, hx_y_min;
	int help_int;
	const int big_number = 10000;

	std::vector<int> Idreaded = read_ID(input);
	for (size_t current_id = 1; current_id < Idreaded.size(); current_id++) // because 0  is background
	{
		help_int = 0;
		help_f1 = big_number;
		help_f2 = big_number;
		hx_y_min = 0;
		for (size_t col = 0; col < input.size() - 1; col++)
		{
			for (size_t row = 0; row < input.at(col).size() - 1; row++)
			{
				if (input.at(col).at(row).ID == Idreaded.at(current_id))
				{

					if (help_f1 > col)
						help_f1 = col;
					if (help_f2 > row)
					{
						hx_y_min = col;
						help_f2 = row;
					}
					help_int++;
				}
			}
		}
		help_output.x_min = help_f1;
		help_output.y_min = help_f2;
		
		help_output.x_y_min = hx_y_min;
		help_output.ID = Idreaded.at(current_id);
		help_output.area = help_int;

		output.push_back(help_output);
	}
	return output;
}

void SegmentationAndOrder::small_area_filter(std::vector<std::vector<PointID>>& input_img, std::vector<Segment_Params>& input_parameters, float filter_multiplier)
{
	float avr_area = 0;
	int help_int1 = 0;
	int help_int2;
	const int zero = 0;
	const uchar black = 0;
	std::vector<size_t> help_erase;
	std::vector<int> ID_to_filter;
	for (size_t curren_parameter = 0; curren_parameter < input_parameters.size(); curren_parameter++)
	{

		avr_area += input_parameters.at(curren_parameter).area;
		help_int1++;
	}
	avr_area = avr_area / (float)help_int1;
	float filter = filter_multiplier * avr_area;

	for (size_t curren_parameter = 0; curren_parameter < input_parameters.size(); curren_parameter++)
	{
		if ((float)(input_parameters.at(curren_parameter).area) < filter)
		{
			help_int2 = input_parameters.at(curren_parameter).ID;
			ID_to_filter.push_back(help_int2);
		}
	}
	for (size_t col = 0; col < input_img.size() - 1; col++)
	{
		for (size_t row = 0; row < input_img.at(col).size() - 1; row++)
		{
			for (size_t curren_id = 0; curren_id < ID_to_filter.size(); curren_id++)
				if (input_img.at(col).at(row).ID == ID_to_filter.at(curren_id))
				{
					input_img.at(col).at(row).value = black;
					input_img.at(col).at(row).ID = zero;
				}
		}
	}

}

void SegmentationAndOrder::big_area_erosion(std::vector<std::vector<PointID>>& input_img, std::vector<Segment_Params>& input_parameters)
{
	float avr_area = 0;
	int help_int1 = 0;
	int help_int2;
	const uchar black= 0;
	const int zero_id = 0;
	std::vector<size_t> help_erase;
	std::vector<int> ID_to_filter;
	for (size_t row = 0; row < input_parameters.size(); row++)
	{

		avr_area += input_parameters.at(row).area;
		help_int1++;
	}
	avr_area = avr_area / (float)help_int1;
	float filter = 1.6 * avr_area;
	
	for (size_t row = 0; row < input_parameters.size(); row++)
	{
		if ((float)(input_parameters.at(row).area) > filter)
		{
			help_int2 = input_parameters.at(row).ID;
			ID_to_filter.push_back(help_int2);
		}
	}
	for (size_t col = 0; col < input_img.size() - 1; col++)
	{
		for (size_t row = 0; row < input_img.at(row).size() - 1; row++)
		{
			for (size_t current_id = 0; current_id < ID_to_filter.size(); current_id++)
				if (input_img.at(col).at(row).ID == ID_to_filter.at(current_id))
				{
					input_img.at(col).at(row).value = black;
					input_img.at(col).at(row).ID = zero_id;
				}
		}
	}
}

void SegmentationAndOrder::segment_numeration(std::vector<std::vector<PointID>>& input_img, std::vector<Segment_Params>& input_parameters)
{


	std::vector<Segment_Params>sortedID;
	//std::vector<Segment_Params> input_parameters_clone = input_parameters;
	//first number is that wchich is the closest to the right top edge
	int control = (int)input_parameters.size() - 1;
	int iteration_counter = 0;
	const int already_used = -1;
	const int big_number = 1000000;
	do
	{
		iteration_counter++;
		float help_f1 = big_number;  //value much bigger than expected to test
		float help_f2 = 0;
		int help_int1 = 0;
		int help_int2 = 0;
		
		//here we are looking for first number(points that belong to the number) in the row
		for (size_t params_id = 0; params_id < input_parameters.size(); params_id++)
		{
			if (input_parameters.at(params_id).ID != already_used) //cleaning from further processing used data;
			{
				help_f2 =  input_parameters.at(params_id).x_min+ input_parameters.at(params_id).y_min;
				if (help_f1 > help_f2 && help_f2 != 0)
				{
					help_f1 = help_f2;
					help_int1 = input_parameters.at(params_id).ID;
					help_int2 = params_id;
				}
			}
		}
		sortedID.push_back(input_parameters.at(help_int2));
		//this loop will find other numbers in the row
		for (size_t params_id = 0; params_id < input_parameters.size(); params_id++)
		{
			if (((std::abs(input_parameters.at(help_int2).x_min - input_parameters.at(params_id).x_min)) < 20 ) )
			{
				if(help_int2 != params_id)
				sortedID.push_back(input_parameters.at(params_id));
				input_parameters.at(params_id).ID = already_used; //cleaning from further processing used data;
			}

		}
		std::sort(sortedID.begin(), sortedID.end(), [](const Segment_Params& left, const Segment_Params& right) {return((left.y_min) < (right.y_min)); });
		input_parameters.at(help_int2).x_min = big_number;//filtration from further use
		input_parameters.at(help_int2).y_min = big_number;//filtration from further use

		//in this step we are obtaining position of marks in the picture;
		//therefore we get sorted segments;

		for (size_t col = 0; col < input_img.size() - 1; col++)
		{
			for (size_t row = 0; row < input_img.at(col).size() - 1; row++)
			{
				for (size_t params_id = 0; params_id < sortedID.size(); params_id++)
					if (input_img.at(col).at(row).ID == sortedID.at(params_id).ID)
					{
						input_img.at(col).at(row).ID = iteration_counter * 1000 + params_id +1;  //ID=1002 tells us that pixcel belongs to the mark in the first column(tousents)  in the second row(units)
					}


			}
		}
		

		control -= (int)sortedID.size();
		sortedID.clear();
		


	} while (control > 0);
	
}

std::vector<std::vector<PointID>>SegmentationAndOrder::get_segments(cv::Mat& input)
{
	std::vector<std::vector<PointID>> output = init_point(input);
	int var_num;
	const uchar white = 255;
	//iteration up -> down, left-> right
	for (size_t col = 0; col < output.size() - 1; col++)
	{
		for (size_t row = 0; row < output.at(col).size() - 1; row++)
		{
			if (output.at(col).at(row).value == white)
			{
				if (output.at(col + 1).at(row).value == white && output.at(col).at(row + 1).value == white) {
					var_num = std::min(output.at(col).at(row).ID, std::min(output.at(col + 1).at(row).ID, output.at(col).at(row + 1).ID));
					output.at(col).at(row).ID = var_num;
					output.at(col + 1).at(row).ID = var_num;
					output.at(col).at(row + 1).ID = var_num;

				}
				else if (output.at(col + 1).at(row).value == white)
				{
					var_num = std::min(output.at(col).at(row).ID, output.at(col + 1).at(row).ID);
					output.at(col).at(row).ID = var_num;
					output.at(col + 1).at(row).ID = var_num;
				}
				else if (output.at(col).at(row + 1).value == white)
				{
					var_num = std::min(output.at(col).at(row).ID, output.at(col).at(row + 1).ID);
					output.at(col).at(row).ID = var_num;
					output.at(col).at(row + 1).ID = var_num;
				}
			}
		}
	}
	// iteration down->up, right-> left
	for (size_t col = output.size() - 1; col > 1; col--)
	{
		size_t row = output.at(col).size() - 1;
		while (row > 0)
		{
			if (output.at(col).at(row).value == white)
			{
				if (output.at(col - 1).at(row).value == white && output.at(col).at(row - 1).value == white)
				{
					var_num = std::min(output.at(col).at(row).ID, std::min(output.at(col - 1).at(row).ID, output.at(col).at(row - 1).ID));
					output.at(col).at(row).ID = var_num;
					output.at(col - 1).at(row).ID = var_num;
					output.at(col).at(row - 1).ID = var_num;
				}
				else if (output.at(col - 1).at(row).value == white)
				{
					var_num = std::min(output.at(col).at(row).ID, output.at(col - 1).at(row).ID);
					output.at(col).at(row).ID = var_num;
					output.at(col - 1).at(row).ID = var_num;
				}
				else if (output.at(col).at(row - 1).value == white)
				{
					var_num = std::min(output.at(col).at(row).ID, output.at(col).at(row - 1).ID);
					output.at(col).at(row).ID = var_num;
					output.at(col).at(row - 1).ID = var_num;
				}
			}
			row--;
		}


	}

	//after first segregation comes filtration and proper numeration
	std::vector<Segment_Params> params = get_params(output);
	small_area_filter(output, params, 0.4);
	params.clear();
	params = get_params(output);

	segment_numeration(output, params);
	return output;
}

SegmentationAndOrder::~SegmentationAndOrder()
{
	//std::cout << "element edstructed" << std::endl;
}