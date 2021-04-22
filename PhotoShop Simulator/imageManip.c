//Sean O'Connor
//soconn19
//Jose Delgado
//jdelgad9
#include "imageManip.h"
#include "ppm_io.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define PI 3.14159265358979323846

// helper functions for a lot of functions

void assign(Pixel * old, const Pixel * new) {
  old->r = new->r;
  old->g = new->g;
  old->b = new->b;  
}

int oobe(const int x, const int y, const int cols, const int rows) {
  return (x < 0 || y < 0 || x >= cols || y >= rows);
}


// helper function for exposure

Pixel * product(const Pixel * px, const float multiplier) {

  Pixel * output = malloc(sizeof(Pixel));
  if (!output) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  int value;
  
  if ((value = px->r * multiplier) < 256) output->r = value;
  else output->r = 255;
  if ((value = px->g * multiplier) < 256) output->g = value;
  else output->g = 255;
  if ((value = px->b * multiplier) < 256) output->b = value;
  else output->b = 255;

  return output;
}

Image * exposure(const Image * im, const float ev) {

  Image * oi = malloc(sizeof(Image));
  if (!oi) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  oi->rows = im->rows;
  oi->cols = im->cols;
  int size = oi->rows * oi->cols;
  float scale = pow(2, ev);
  Pixel * pixels = malloc(sizeof(Pixel) * size);
  if (!pixels) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  for(int i = 0; i < size; i++) {
    Pixel * temp = (product((im->data + i), scale));
    assign(pixels + i, temp);
    free(temp);
  }

  oi->data = pixels;
  return oi;
}



Image * blend(Image * im, Image * fade_in, const double alpha) {
  
  Image * oi = malloc(sizeof(Image));
  if (!oi){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  int max_rows = (abs(im->rows - fade_in->rows) + im->rows + fade_in->rows)/2;
  int max_cols = (abs(im->cols - fade_in->cols) + im->cols + fade_in->cols)/2;

  oi->rows = max_rows;
  oi->cols = max_cols;
  int size = oi->rows * oi->cols;
  Pixel * pixels = malloc(sizeof(Pixel) * size);
  if (!pixels) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
 
  Pixel * zero = malloc(sizeof(Pixel));
  if (!zero){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  zero->r = 0;
  zero->g = 0;
  zero->b = 0; 

  for (int x = 0; x < oi->cols; x++) {
    for (int y = 0; y < oi->rows; y++) {
      Pixel * current = pixels + x + y * oi->cols;

      if (oobe(x, y, im->cols, im->rows)) {
	if (oobe(x, y, fade_in->cols, fade_in->rows)) assign(current, zero);
	else assign(current, fade_in->data + x + y * fade_in->cols);
      }
      
      else if (oobe(x, y, fade_in->cols, fade_in->rows)) assign(current, im->data + x + y * im->cols);
      
      else {
	current->r = alpha * ((im->data + x + y * im->cols)->r) + (1 - alpha) * ((fade_in->data + x + y * fade_in->cols)->r);
        current->g = alpha * ((im->data + x + y * im->cols)->g) + (1 - alpha) * ((fade_in->data + x + y * fade_in->cols)->g);
        current->b = alpha * ((im->data + x + y * im->cols)->b) + (1 - alpha) * ((fade_in->data + x + y * fade_in->cols)->b);
      }
    }
  }

  free(zero);

  oi->data = pixels;
  return oi;
}



Image * zoom_in(const Image * im) {

  Image * oi = malloc(sizeof(Image));
  if (!oi){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  oi->rows = im->rows * 2;
  oi->cols = im->cols * 2;
  int size = oi->rows * oi->cols;
  Pixel * pixels = malloc(sizeof(Pixel) * size);
  if (!pixels) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {

      Pixel * temp = (im->data + im->cols * i + j);
      
      assign(pixels + oi->cols * (2 * i) + (2 * j), temp);
      assign(pixels + oi->cols * (2 * i) + (2 * j + 1), temp);
      assign(pixels + oi->cols * (2 * i + 1) + (2 * j), temp);
      assign(pixels + oi->cols * (2 * i + 1) + (2 * j + 1), temp);
    }
  }

  oi->data = pixels;
  return oi;
}



//helper function for zoom_out

Pixel * average(const Pixel * first, const Pixel * second, const Pixel * third, Pixel * fourth) {

  Pixel * output = malloc(sizeof(Pixel));
  if (!output){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  
  output->r = (first->r + second->r + third->r + fourth->r)/4.0f;
  output->g = (first->g + second->g + third->g + fourth->g)/4.0f;
  output->b = (first->b + second->b + third->b + fourth->b)/4.0f;

  return output;
}

Image * zoom_out(const Image * im) {

  Image * oi = malloc(sizeof(Image));
  if (!oi){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  oi->rows = im->rows / 2;
  oi->cols = im->cols / 2;
  int size = oi->rows * oi->cols;
  Pixel * pixels = malloc(sizeof(Pixel) * size);
  if (!pixels) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  for (int i = 0; i < oi->rows; i++) {
    for (int j = 0; j < oi->cols; j++) {
      Pixel * temp = average(im->data + im->cols * (2 * i) + (2 * j),
                             im->data + im->cols * (2 * i) + (2 * j + 1),
                             im->data + im->cols * (2 * i + 1) + (2 * j),
                             im->data + im->cols * (2 * i + 1) + (2 * j + 1));

      assign(pixels + oi->cols * i + j, temp);
      free(temp);
    }
  }

  oi->data = pixels;
  return oi;
}



//helper functions for swirl

double alpha(const int x, const int y, const int center_x, const int center_y, const int scale) {
  return sqrt(pow(x - center_x, 2) + pow(y - center_y, 2))/scale;
}

int transformed_x(const int x, const int y, const int center_x, const int center_y, const double alpha) {
   return ((x - center_x) * cos(alpha) - (y - center_y) * sin(alpha) + center_x);
}

int transformed_y(const int x, const int y, const int center_x, const int center_y, const double alpha) {
   return ((x - center_x) * sin(alpha) + (y - center_y) * cos(alpha) + center_y);
}

Image * swirl(const Image * im, const int center_x, const int center_y, const int scale) {

  Image * oi = malloc(sizeof(Image));
  if (!oi){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  oi->rows = im->rows;
  oi->cols = im->cols;
  int size = oi->rows * oi->cols;
  Pixel * pixels = malloc(sizeof(Pixel) * size);
  if (!pixels) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  Pixel * zero = malloc(sizeof(Pixel));
  if (!zero){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  zero->r = 0;
  zero->g = 0;
  zero->b = 0;

  for (int x = 0; x < oi->cols; x++) {
    for (int y = 0; y < oi->rows; y++) {
      double a = alpha(x, y, center_x, center_y, scale);
      int original_x = transformed_x(x, y, center_x, center_y, a);
      int original_y = transformed_y(x, y, center_x, center_y, a);

      if (oobe(original_x, original_y, im->cols, im->rows)) assign(pixels + x + y * oi->cols, zero);
      else assign(pixels + x + y * oi->cols, im->data + original_x + original_y * im->cols);

    }
  }

  free(zero);

  oi->data = pixels;
  return oi;
}



//helper functions for pointilism

int dist(const int x_1, const int y_1, const int x_2, const int y_2) {
  return ((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
}

void make_point(Pixel * current, const int center_x, const int center_y, const int radius, const int cols, const int rows) {

  for (int x = center_x - radius; x <= center_x + radius; x++) {
    for (int y = center_y - radius; y <= center_y + radius; y++) {
      if (oobe(x, y, cols, rows)) continue;

      if (dist(x, y, center_x, center_y) <= radius * radius) {
      	assign(current + x + y * cols, current + center_x + center_y * cols);
      }

    }
  }
}

Image * pointilism(const Image * im) {

  Image * oi = malloc(sizeof(Image));
  if (!oi){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  oi->rows = im->rows;
  oi->cols = im->cols;
  int size = oi->rows * oi->cols;
  Pixel * pixels = malloc(sizeof(Pixel) * size);
  if (!pixels) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  for (int i = 0; i < oi->rows; i++) {
    for (int j = 0; j < oi->cols; j++) {
      assign(pixels + oi->cols * i + j, im->data + im->cols * i + j);
    }
  }

  int num_changes = size * 0.03;
  srand(1);

  for (int i = 0; i < num_changes; i++) {
    int rand_x = rand() % oi->cols;
    int rand_y = rand() % oi->rows;
    int rand_r = rand() % 5 + 1;

    make_point(pixels, rand_x, rand_y, rand_r, oi->cols, oi->rows);
  }

  oi->data = pixels;
  return oi;
}



//helper functions for blur

void convolve(Pixel * original, Pixel * new, double * filter, const int center_x, const int center_y, const int cols, const int rows, const int side) {

  double r = 0;
  double g = 0;
  double b = 0;
  double normalizer = 0;

  for (int x = center_x - side/2; x <= center_x + side/2; x++) {
    for (int y = center_y - side/2; y <= center_y + side/2; y++) {
      if (oobe(x, y, cols, rows)) continue;
      Pixel * current_pixel = original + x + y * cols;
      double current_double = *(filter + (x - center_x + side/2) + (y - center_y + side/2) * side);

      r += current_double * current_pixel->r;
      g += current_double * current_pixel->g;
      b += current_double * current_pixel->b;
      normalizer += current_double;
    }
  }

  r /= normalizer;
  g /= normalizer;
  b /= normalizer;

  Pixel * center = new + center_x + center_y * cols;
  center->r = r;
  center->g = g;
  center->b = b;
}

Image * blur(Image * im, const double sigma) {

  Image * oi = malloc(sizeof(Image));
  if (!oi){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }

  oi->rows = im->rows;
  oi->cols = im->cols;
  int size = oi->rows * oi->cols;
  Pixel * pixels = malloc(sizeof(Pixel) * size);
  if (!pixels) {
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  int side = sigma * 10 + !((int)(sigma * 10) % 2);
  double * filter = malloc(sizeof(double) * side * side);
  if (!filter){
    fprintf(stderr, "Uh oh. Allocation failed!\n");
    exit(8);
  }
  
  for (int i = -side/2; i <= side/2; i++) {
    for (int j = -side/2; j <= side/2; j++) {
      *(filter + (side/2 + i) + (side/2 + j) * side) = (1.0 / (2.0 * PI * sigma * sigma)) * exp( -(i * i + j * j) / (2 * sigma * sigma));
    }
  }

  for (int x = 0; x < oi->cols; x++) {
    for (int y = 0; y < oi->rows; y++) {
      convolve(im->data, pixels, filter, x, y, oi->cols, oi->rows, side);
    }
  }

  free(filter);

  oi->data = pixels;
  return oi;
}
