# yuvToRgb
C/C++ implementation of the yuv422 planar to rgb color space

C/C++ reading a file into buffers, separating the YUV422 color components, calculating the RGB pixels, 
and saving them into separate files in order to be parsed by programs such as Matlab/Octave.

### To compile ###
g++ yuvrgb.cpp -o run

### To run ###
./run

### Octave Output ###
![picture alt](https://github.com/dmacattack/yuvToRgb/blob/master/doc/octaveOutput.png "Result")
