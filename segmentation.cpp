#include "segmentation.h"

segmentation::segmentation()
{
	;
}

std::vector<std::vector<uchar>> segmentation::to_array(cv::Mat& input_mat)
{
	std::vector<std::vector<uchar>> output;
	std::vector<uchar> help_vector;
	uchar help_uchar = 0;
	for (size_t i = 0; i < (input_mat.size().height); i++)
	{
		for (size_t j = 0; j < (input_mat.size().width); j++)
		{
			help_uchar = input_mat.at<uchar>(i, j);  // row, col
			help_vector.push_back(help_uchar);
		}
		output.push_back(help_vector);
		help_vector.clear();
	}
	return output;
}

cv::Mat segmentation::to_mat(cv::Mat & Mat_start, std::vector<std::vector<Point_ID>>& input)

{

	for (size_t i = 0; i < (Mat_start.size().height); i++)
	{
		for (size_t j = 0; j < (Mat_start.size().width); j++)
		{
			Mat_start.at<uchar>(i, j) = input.at(i).at(j).value;  // row, col
		}

	}
	return Mat_start;
}
//initialization of vector Point_ID 
std::vector<std::vector<Point_ID>> segmentation::init_Potin(cv::Mat& input)
{
	std::vector<std::vector<Point_ID>> output;
	std::vector<Point_ID> help_vector;
	Point_ID point;

	int var_1 = 1;
	std::vector<std::vector<uchar>> input_vec = to_array(input);
	for (size_t i = 0; i < input_vec.size(); i++)
	{
		for (size_t j = 0; j < input_vec.at(i).size(); j++)
		{
			point.value = input_vec.at(i).at(j);
			if (point.value == 255)
			{
				point.ID = var_1;
				var_1++;
			}
			else
				point.ID = 0;
			help_vector.push_back(point);
		}
		output.push_back(help_vector);
		help_vector.clear();
	}
	return output;
}
// read all id's that exists in picture after segmentation 
std::vector<int> segmentation::read_ID(std::vector<std::vector<Point_ID>> &input)
{
	std::vector<int> output;
	int help_var = 0;
	output.push_back(help_var);
	bool help_bool = 0;
	for (size_t i = 0; i < input.size() - 1; i++)
	{
		for (size_t j = 0; j < input.at(i).size() - 1; j++)
		{

			if (help_var = input.at(i).at(j).ID != 0)
			{
				help_bool = 0;
				for (size_t k = 0; k < output.size(); k++)
				{
					if (input.at(i).at(j).ID == output.at(k))
						help_bool = 1;
				}
				if (help_bool == 0)
					output.push_back(input.at(i).at(j).ID);
			}
		}
	}
	return output;
}
// nie uwzglednia pixceli z wart 0
std::vector<Segment_Params>segmentation::get_params(std::vector<std::vector<Point_ID>> &input)
{
	std::vector<Segment_Params> output;
	Segment_Params help_output;
	float help_f1, help_f2, hx_y_min;
	int help_int;

	std::vector<int> Idreaded = read_ID(input);
	for (size_t k = 1; k < Idreaded.size(); k++)
	{
		help_int = 0;
		help_f1 = 10000;
		help_f2 = 10000;
		hx_y_min = 0;
		for (size_t i = 0; i < input.size() - 1; i++)
		{
			for (size_t j = 0; j < input.at(i).size() - 1; j++)
			{
				if (input.at(i).at(j).ID == Idreaded.at(k))
				{

					if (help_f1 > i)
						help_f1 = i;
					if (help_f2 > j)
					{
						hx_y_min = i;
						help_f2 = j;
					}
					help_int++;
				}
			}
		}
		help_output.x_min = help_f1;
		help_output.y_min = help_f2;
		
		help_output.x_y_min = hx_y_min;
		help_output.ID = Idreaded.at(k);
		help_output.area = help_int;

		output.push_back(help_output);
	}
	return output;
}

void segmentation::small_area_filter(std::vector<std::vector<Point_ID>>& input_img, std::vector<Segment_Params>& input_parameters, float filter_multiplier)
{
	float avr_area = 0;
	int help_int1 = 0;
	int help_int2;
	std::vector<size_t> help_erase;
	std::vector<int> ID_to_filter;
	for (size_t i = 0; i < input_parameters.size(); i++)
	{

		avr_area += input_parameters.at(i).area;
		help_int1++;
	}
	avr_area = avr_area / (float)help_int1;
	float filter = filter_multiplier * avr_area;

	for (size_t i = 0; i < input_parameters.size(); i++)
	{
		if (float(input_parameters.at(i).area) < filter)
		{
			help_int2 = input_parameters.at(i).ID;
			ID_to_filter.push_back(help_int2);
		}
	}
	for (size_t i = 0; i < input_img.size() - 1; i++)
	{
		for (size_t j = 0; j < input_img.at(i).size() - 1; j++)
		{
			for (size_t k = 0; k < ID_to_filter.size(); k++)
				if (input_img.at(i).at(j).ID == ID_to_filter.at(k))
				{
					input_img.at(i).at(j).value = 0;
					input_img.at(i).at(j).ID = 0;
				}
		}
	}

}

void segmentation::big_area_erosion(std::vector<std::vector<Point_ID>>& input_img, std::vector<Segment_Params>& input_parameters)
{
	float avr_area = 0;
	int help_int1 = 0;
	int help_int2;
	std::vector<size_t> help_erase;
	std::vector<int> ID_to_filter;
	for (size_t i = 0; i < input_parameters.size(); i++)
	{

		avr_area += input_parameters.at(i).area;
		help_int1++;
	}
	avr_area = avr_area / (float)help_int1;
	float filter = 1.6 * avr_area;
	//std::cout << std::endl << filter << std::endl << std::endl;
	for (size_t i = 0; i < input_parameters.size(); i++)
	{
		if (float(input_parameters.at(i).area) > filter)
		{
			help_int2 = input_parameters.at(i).ID;
			ID_to_filter.push_back(help_int2);
		}
	}
	for (size_t i = 0; i < input_img.size() - 1; i++)
	{
		for (size_t j = 0; j < input_img.at(i).size() - 1; j++)
		{
			for (size_t k = 0; k < ID_to_filter.size(); k++)
				if (input_img.at(i).at(j).ID == ID_to_filter.at(k))
				{
					input_img.at(i).at(j).value = 0;
					input_img.at(i).at(j).ID = 0;
				}
		}
	}
}

// numeration is assigned based on: 
//1) distance from segment to top right corner in x axis 
//2) distance from segment to top right corner by y axis
//it is to get proper matrix of data

void segmentation::segment_numeration(std::vector<std::vector<Point_ID>>& input_img, std::vector<Segment_Params>& input_parameters)
{


	std::vector<Segment_Params>sortedID;
	//std::vector<Segment_Params> input_parameters_clone = input_parameters;
	//first number is that wchich is the closest to the right top edge
	int control = (int)input_parameters.size() - 1;
	int iteration_counter = 0;
	do
	{
		iteration_counter++;
		float help_f1 = 1000000;
		float help_f2 = 0;
		int help_int1 = 0;
		int help_int2 = 0;
		//std::sort(input_parameters.begin(), input_parameters.end(), [](const Segment_Params& left, const Segment_Params& right) {return(left.y_min < right.y_min); });
		//here we are looking for first number(points that belong to the number) in the row
		for (size_t i = 0; i < input_parameters.size(); i++)
		{
			if (input_parameters.at(i).ID != -1) //cleaning from further processing used data;
			{
				help_f2 =  input_parameters.at(i).x_min+ input_parameters.at(i).y_min;
				if (help_f1 > help_f2 && help_f2 != 0)
				{
					help_f1 = help_f2;
					help_int1 = input_parameters.at(i).ID;
					help_int2 = i;
				}
			}
		}
		sortedID.push_back(input_parameters.at(help_int2));
		//this loop will find other numbers in the row
		for (size_t i = 0; i < input_parameters.size(); i++)
		{
			if (((std::abs(input_parameters.at(help_int2).x_min - input_parameters.at(i).x_min)) < 20 ) )
			{
				if(help_int2 != i)
				sortedID.push_back(input_parameters.at(i));
				input_parameters.at(i).ID = -1; //cleaning from further processing used data;
			}

		}
		std::sort(sortedID.begin(), sortedID.end(), [](const Segment_Params& left, const Segment_Params& right) {return((left.y_min) < (right.y_min)); });
		input_parameters.at(help_int2).x_min = 10000;
		input_parameters.at(help_int2).y_min = 10000;
		//cout <<help_int1<<"\t"<< help_int2;
		//input_parameters_clone.erase(input_parameters_clone.begin() + help_int2);


		//seting ID of the number 

		for (size_t i = 0; i < input_img.size() - 1; i++)
		{
			for (size_t j = 0; j < input_img.at(i).size() - 1; j++)
			{
				for (size_t k = 0; k < sortedID.size(); k++)
					if (input_img.at(i).at(j).ID == sortedID.at(k).ID)
					{
						input_img.at(i).at(j).ID = iteration_counter * 1000 + k+1;
						input_img.at(i).at(j).value = (k+1)*10;
					}


			}
		}
		

		control -= (int)sortedID.size();
		sortedID.clear();
		


	} while (control > 0);
	
}

std::vector<std::vector<Point_ID>>segmentation::get_segments(cv::Mat& input)
{
	std::vector<std::vector<Point_ID>> output = init_Potin(input);
	int var_num;

	//iteration up -> down, left-> right
	for (size_t i = 0; i < output.size() - 1; i++)
	{
		for (size_t j = 0; j < output.at(i).size() - 1; j++)
		{
			if (output.at(i).at(j).value == 255)
			{
				if (output.at(i + 1).at(j).value == 255 && output.at(i).at(j + 1).value == 255) {
					var_num = std::min(output.at(i).at(j).ID, std::min(output.at(i + 1).at(j).ID, output.at(i).at(j + 1).ID));
					output.at(i).at(j).ID = var_num;
					output.at(i + 1).at(j).ID = var_num;
					output.at(i).at(j + 1).ID = var_num;

				}
				else if (output.at(i + 1).at(j).value == 255)
				{
					var_num = std::min(output.at(i).at(j).ID, output.at(i + 1).at(j).ID);
					output.at(i).at(j).ID = var_num;
					output.at(i + 1).at(j).ID = var_num;
				}
				else if (output.at(i).at(j + 1).value == 255)
				{
					var_num = std::min(output.at(i).at(j).ID, output.at(i).at(j + 1).ID);
					output.at(i).at(j).ID = var_num;
					output.at(i).at(j + 1).ID = var_num;
				}
			}
		}
	}
	// iteration down->up, right-> left
	for (size_t i = output.size() - 1; i > 1; i--)
	{
		size_t j = output.at(i).size() - 1;
		while (j > 0)
		{
			if (output.at(i).at(j).value == 255)
			{
				if (output.at(i - 1).at(j).value == 255 && output.at(i).at(j - 1).value == 255)
				{
					var_num = std::min(output.at(i).at(j).ID, std::min(output.at(i - 1).at(j).ID, output.at(i).at(j - 1).ID));
					output.at(i).at(j).ID = var_num;
					output.at(i - 1).at(j).ID = var_num;
					output.at(i).at(j - 1).ID = var_num;
				}
				else if (output.at(i - 1).at(j).value == 255)
				{
					var_num = std::min(output.at(i).at(j).ID, output.at(i - 1).at(j).ID);
					output.at(i).at(j).ID = var_num;
					output.at(i - 1).at(j).ID = var_num;
				}
				else if (output.at(i).at(j - 1).value == 255)
				{
					var_num = std::min(output.at(i).at(j).ID, output.at(i).at(j - 1).ID);
					output.at(i).at(j).ID = var_num;
					output.at(i).at(j - 1).ID = var_num;
				}
			}
			j--;
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

segmentation::~segmentation()
{
	//std::cout << "element edstructed" << std::endl;
}