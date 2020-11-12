read_text_computer_vision program instructions.
Program is written in C++  in Visual Studio 2019 with the open source machine vision library OpenCV 3.4.12. Therefore to see performance output from this program user need to download free version of visual studio 2019
and OpenCV library.
1)	Download OpenCV 3.4.12. from https://opencv.org/releases/ website.
2) 	Go to System properies
2)	Install OpenCV at: “C:\” location. If you do not want to install it there you will need to change properties of application of the library. To do it you need to:
	a)	go to: solution explorer>>read_text_computer_vision>>properties>>C/C++>>General>> Additional include directories>>edit and paste into the window: “your_installation_path/opencv/build/include”, click apply.
	b)	go to: solution explorer>> read_text_computer_vision>>properties>>linker>>General>> Additional include directories>>edit   paste into the window: “your_installation_path/opencv/ x64/vc14/lib”, click apply. 
	c)	Go to go to: solution explorer>>read_text_computer_vision>>properties>>linker>> Input>> Additional Dependencies>>edit paste into the window: “opencv_world3412d.lib” 
	d)	Change if you are using debug mode x64
3)	Running the program
	a)	When you run program you will need to prepare ”.png” files. All files should consist of constant part  e.g.  “Train_” and integer  number from 1(for the first file) to 2147483647( max amount of files).
		Program will read all files that exists between 1 and number of files.
	b)	When you run the program you will be asked for the location of files
4)	Results you will obtain results printed in the consol and  in your images location file  as coma separated data “numbers_” with extension ”.txt” .

