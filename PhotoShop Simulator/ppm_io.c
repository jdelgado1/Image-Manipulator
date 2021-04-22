//Jose Delgado
//jdelgad9
//Sean O'Connor
//soconn19
// __Add your name and JHED above__
// ppm_io.c
// 601.220, Spring 2019
// Starter code for midterm project - feel free to edit/add to this file

#include <assert.h>
#include "ppm_io.h"
#include <stdlib.h>
#include <stdio.h>

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL                                                                                        
  if (fp == NULL) {
    fprintf(stderr, "Specified input file could not be opened\n");
    exit(2);
  }
  assert(fp);
  //allocates space for image. Creates image struct                                                                   
  Image * im = malloc(sizeof(Image));
  if (!im) {
    fprintf(stderr, "Uh oh. Image allocation failed!\n");
    exit(1);
  }
  //scans file for cols rows and color                                                                                
  int colors = 255;
  /*
  //int cols = 0;                                                                                                     
  //int rows = 0;
  char first_letter = '0';
  char second_letter = '0';
  fscanf(fp, "%c%c", &first_letter, &second_letter);
  if ((first_letter != 'P') && (second_letter != '6')) {
    fprintf(stderr, "Invalid image format\n");
    exit(3);
  }
  */

    //TO DO: Add support for an option line of comments, beginning with '#' and ending with '\n'                    
  char c = '0';
  if (fscanf(fp, "P%c\n", &c) != 1 || c != '6') {
    fprintf(stderr, "Invalid image format\n");
    free(im);
    exit(3);
  }

  c = getc(fp);
  while (c == '#') {
    /*                                                                                                            
      while (getc(fp) != '\n') {                                                                                  
      c = getc(fp);                                                                                               
      }                                                                                                           
    */
    do {
        c = getc(fp);
      } while (c != '\n');      /* read to the end of the line */
      c = getc(fp);

  }
  ungetc(c, fp);

  
  fscanf(fp, "%d %d %d", &im->cols, &im->rows, &colors);//check if possible                                            
  //im->cols = cols;                                                                                                  
  //im->rows = rows;                                                                                                  
  if (colors != 255) {
    fprintf(stderr, "colors must always be 255\n");
    free(im);
    exit(3);
  }

  
  //throws away newline char
  char dummy;
  fscanf(fp, "%c", &dummy);
  //allocates space for array of pixels                                                                               
  Pixel * pix = malloc(sizeof(Pixel) * im->cols * im->rows); //check if correct                                         
  if (pix == NULL) {
    fprintf(stderr, "Uh oh. Pixel array allocation failed!\n");
    free(im);
    exit(8);
  }
  //reads ppm formatted image from file                                                                               
  fread((void *)pix, 3 * im->cols, im->rows, fp);
  //data field of image points to created array of pixels

  fclose(fp);
  im->data = pix;
  //free(pix);
  return im;  //TO DO: replace this stub                                                                         
}



/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  if (fp == NULL) {
    fprintf(stderr, "Specified input file could not be opened\n");
    return 7;
  }
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}



