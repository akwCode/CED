# CED

## About 
This repo contains an example implementation of the Canny Edge Detector. It uses and requires the C++ [OpenCV](https://opencv.org/) computer vision library.

## Build
Before building this project, you must have the C++ OpenCV library installed. Instructions to download the library can be found [here](https://docs.opencv.org/master/d0/d3d/tutorial_general_install.html), on the OpenCV website. Make sure to make the appropriate changes in the CMakeLists.txt file to reflect the location of your installation.

The only line you should have to modify
```
set(OpenCV_DIR "YOUR_PATH")
```

Once you have OpenCV properly install and included in your CMakeLists.txt, you can proceed with building the project.

First, cd into the directory where you have cloned this repository.
```
cd ../CED
```

Then create an empty build directory 
```
mkdir build
```

Then generate the makefiles
```
cmake -S . -B build/
```

And finally cd into the build directory, and build the project
```
cd build
make
```

## Run
To run the program simply cd into the build folder (after building the project), and run the executable. The program allows you to run the Canny Edge Detector on any of the images located in the testImages directory. Simply specify the name of the image as the first argument. Example:
```
cd build
./CED "bug.png"
```

If no image is specified, the program will default to `"eskimo.png"`
