How to open read_text_computer_vision program at linux:

1) Download and install OpenCV library (if you already did it just skip this step).

The packages can be installed using a terminal and the following commands:

sudo apt-get install build-essential

sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

#download opencv from:
#https://opencv.org/releases/

or type
cd ~/<my_working_directory>
git clone https://github.com/opencv/opencv.git


#opencv file location in console

cd opencv

mkdir build

cd build

cmake -D CMAKE_BUILD_TYPE=RELEASE \ -D CMAKE_INSTALL_PREFIX=/usr/local \  -D OPENCV_GENERATE_PKGCONFIG=ON \ ..

make -j7 #This step may take some time

sudo make install

more information about installation you can find at: https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html

2)Download github code repository

https://github.com/sekuba101/read_text_computer_vision/tree/master/linux%20program

3) make myprog

4) run main.o

5) copy your files path (should be ended with ‘/’)

6)write constant part of the fies name e.g. train_

7) write amount of files. Note that files will be readed from number 1 to the number that you will pass.

8) results will be placed In input file as .txt files.



To read all files from the input file you need to firstly pass to the program input file path then pass train_ and then 5.

