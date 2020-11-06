/**********************************************************************************
	Program to recognize numbers, by usage of image processing and computer vision algorithms

	Program uses  "OpenCV – 3.4.12" library
	instructions how to download and instal library:
	windows: https://www.youtube.com/watch?v=M-VHaLHC4XI&t=600s
	linux: https://www.youtube.com/watch?v=6pABIQl1ZP0
	*********************************************************************************/



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
#include "NumbersDetector.h"


using std::cout;
using std::endl;


void morphology(cv::Mat& im_input);
void save_to_file(std::vector < std::vector<int> > input, std::string text);





int main()
{
	/**********************************************************************************
	step_1 - Getting_Files

	in this step we just access file where images are stored and reasult will be saved
	all images are stored as std::vector<cv::Mat> which is standard save of img for opencv library

	*********************************************************************************/
	
	std::string path_open;
	std::string file_name;
	int amount_of_files;
	cout << "Write file location path e.g. C:'\\'Users \n";
	std::cin >> path_open;
	char slash = 92;
	if (path_open.at(path_open.size() - 1) != slash)
		path_open.push_back(slash);
	cout << "file that you want to open should heve constant part(e. g. train_), \n number(from 1 to int_max) and extension .png \n";
	cout << "write constant part" << endl;
	std::cin >> file_name;
	cout << "write amount of files (numerical extension from 1 to number)" << endl;
	std::cin >> amount_of_files;
	std::string extension(".png");
	std::vector<cv::Mat> source_images;

	for (int im_num = 1; im_num < amount_of_files + 1; im_num++)
	{
		
		std::cout << " \n now we processing img" << im_num << "\n";
		std::string final_path_open = path_open + file_name + std::to_string(im_num) + extension;
		cv::Mat im_input = cv::imread(final_path_open, cv::IMREAD_GRAYSCALE);
		if (im_input.empty()) {
			std::cout << "error:" << final_path_open << " not found \n";
		}
		else
		{
			/**********************************************************************************
			step_2 - Initial data processing

			At this stage program start with graysacle image processing.
			firstly program thresholds image to get binnary image. In this case additional filtration was not needed. 
			However it must be notted that histogram stretching before getting threshold might improve results;
			Then image is processed by morphological operation opening to get rid of some primary noises

			*********************************************************************************/

			cv::threshold(im_input, im_input, 158, 255, cv::THRESH_BINARY_INV);
			morphology(im_input);
			/**********************************************************************************
			step_3 - segmentation

			In this step image is segmented.
			As an outpu we get 3dim table of data. 
			1st dim is columns; 2nd is rows; 3rd is distinguish between digits and decimals; data is all points that belongs to one mark.
			At this point we have acces to all data

			NOTE: unfortunately, due to the fact that segmentation sorting is based on
				distance of mark in the specified rowfrom top left corner to the closed point of the mark,
				we get error at distinguishing number 7 in train_3 and we get 70;
				Unfortunately i did not have more time to fix that issue :(
			*********************************************************************************/
			SegmentationAndOrder segment;
			std::vector<std::vector<PointID>> segmented = segment.get_segments(im_input);
			std::vector<int> allIDs = segment.read_ID(segmented);
			NumbersDetector detector;
			std::vector<std::vector < std::vector<std::vector< Point2DID>>>> table_of_numbers = detector.get_numbers(segmented, allIDs);
			/**********************************************************************************
			step_4 - mark detection and save to the file

			mark detection is based on size of "0" or "8" character
			and feutures of numbers based on shape - percent of mark's pixels in the specified image region.

			Finally obtained numbers are stored in the txt file and displayed in the console;

			It must be notted that if picture does not include "0" or "8" character this program will not work properly
			*********************************************************************************/
			std::vector < std::vector < std::vector<int >>> numbers_detected = detector.detect(table_of_numbers);
			std::vector < std::vector<int> > numbers_table = detector.get_result(numbers_detected);
			std::string final_path_save = path_open + "\\" + "numbers_" + std::to_string(im_num) + "_";
			save_to_file(numbers_table, final_path_save);
			cout << ("data from img " + std::to_string(im_num) + " is in your inputfile \n");
		}
	}

	cv::waitKey(0);
	return 0;
}


void morphology(cv::Mat& im_input)
{
	//we want to erode only big pictures because in small we can have significant data loose;
	if (im_input.size().area() > 70000)
	{
		cv::erode(im_input, im_input, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)), cv::Point(-1, -1), 1);
		cv::dilate(im_input, im_input, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)), cv::Point(-1, -1), 2);
	}

}

void save_to_file(std::vector < std::vector<int> > input, std::string text)
{
	std::ofstream inFile;

	inFile.open(text + ".txt");

	for (size_t i = 0; i < input.size(); i++)
	{
		for (size_t j = 0; j < input.at(i).size(); j++)
		{
			inFile << input.at(i).at(j);
			inFile << "\t";
		}
		inFile << "\n";
	}

}





