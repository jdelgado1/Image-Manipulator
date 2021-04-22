//Sean O'Connor Jose Delgado
//soconn19 jdelgad9
#include "ppm_io.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define PI 3.14159265358979323846


void assign(Pixel * old, const Pixel * new);
int oobe(const int x, const int y, const int cols, const int rows);
Pixel * product(const Pixel * px, const float multiplier);
Image * exposure(const Image * im, const float ev);
Image * blend(Image * im, Image * fade_in, const double alpha);
Image * zoom_in(const Image * im);
Pixel * average(const Pixel * first, const Pixel * second, const Pixel * third, Pixel * fourth);
Image * zoom_out(const Image * im);
int oobe(const int x, const int y, const int cols, const int rows);
double alpha(const int x, const int y, const int center_x, const int center_y, const int scale);
int transformed_x(const int x, const int y, const int center_x, const int center_y, const double alpha);
int transformed_y(const int x, const int y, const int center_x, const int center_y, const double alpha);
Image * swirl(const Image * im, const int center_x, const int center_y, const int scale);
void make_point(Pixel * current, const int center_x, const int center_y, const int radius, const int cols, const int rows);
Image * pointilism(const Image * im);
void convolve(Pixel * original, Pixel * new, double * filter, const int center_x, const int center_y, const int cols, const int rows, const int side);
Image * blur(Image * im, const double sigma);
