# Image-Manipulator
This is an image manipulation program like Photoshop. We read ppm image files from disk, perform one of a variety of image manipulation tasks, and then wrote the result back to disk as a new image file.
* Version 1.0

* Writing tests
There were 5 test files made to test out functions. They all included what was in ppm_io.h, imageManip.h, as well as the function in ppm_io.c. The functions of imageManip.c were tested out in these files. Copies of trees.ppm and building.ppm were put into the same directory as the test files in order to run the test using ./a.out.

* Code
Here's a brief overview of the functions used in this project and how we coded them. Most of them rely on some sort of helper function, which was used to improve readibility and reusability.
