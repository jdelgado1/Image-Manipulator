//Jose Delgado
//jdelgad9
//Sean O'Connor
//soconn19

#include <assert.h>
#include "ppm_io.h"
#include "imageManip.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


int main(int argc, const char * argv[]) {

  if (argc < 4) {
    printf("Failed to supply input filename or output filename, or both");
    return 1;
  }
  FILE *input = fopen(argv[1], "rb");
  if (input == NULL) {
    printf("Error: could not open file\n");
    return 2;
  }
  FILE *output = fopen(argv[2], "wb");
  if (output == NULL) {
    printf("Error: could not open file\n");
    return 2;
  }
  Image *image1;
  image1 = read_ppm(input);
  //char operation[] = argv[3];
  Image *image_created;


  
  

  if (strcmp(argv[3], "exposure") == 0) {
    if (argc != 5) {
      printf("Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      free(image1->data);
      free(image1);
      return 5;
    }
    float exposure_value = atof(argv[4]);
    if ((exposure_value < -3.0) || (exposure_value > 3.0)) {
      printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
      free(image1->data);
      free(image1);
      return 6;
    }
    image_created = exposure(image1, exposure_value);
  }


  
  else if (strcmp(argv[3], "blend") == 0) {
    printf("1");
    if (argc != 6) {
      printf("Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      free(image1->data);
      free(image1);
      return 5;
    }
    float blend_ratio = atof(argv[5]);
    if ((blend_ratio < 0.0) || (blend_ratio > 1.0)) {
      printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
      free(image1->data);
      free(image1);
      return 6;
    }
    FILE *input2 = fopen(argv[4], "rb");
    if (input2 == NULL) {
      printf("Error: could not open second file\n");
      free(image1->data);
      free(image1);
      return 2;
    }
    Image *image2;
    image2 = read_ppm(input2);
    image_created = blend(image1, image2, blend_ratio);
    
    free(image2->data);
    free(image2);
  }
  

  
  else if (strcmp(argv[3], "zoom_in") == 0) {
    if (argc != 4) {
      printf("Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      free(image1->data);
      free(image1);
      return 5;
    }
    image_created = zoom_in(image1);
  }


  
  else if (strcmp(argv[3], "zoom_out") == 0) {
    if (argc != 4) {
      printf("Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      free(image1->data);
      free(image1);
      return 5;
    }
    image_created = zoom_out(image1);
  }


  
  else if (strcmp(argv[3], "pointilism") == 0) {
    if (argc != 4) {
      printf("Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      free(image1->data);
      free(image1);
      return 5;
    }
    image_created = pointilism(image1);
  }


  
  else if (strcmp(argv[3], "swirl") == 0) {
    if (argc != 7) {
      printf("Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      free(image1->data);
      free(image1);
      return 5;
    }
    const int center_x = atoi(argv[4]);
    const int center_y = atoi(argv[5]);
    const int scale = atoi(argv[6]);
    if ((center_x < 0) || (center_x > image1->cols)) {
      printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
      free(image1->data);
      free(image1);
      return 6;
    }
    if ((center_y < 0) || (center_y > image1->rows)) {
      printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
      free(image1->data);
      free(image1);
      return 6;
    }
    image_created = swirl(image1, center_x, center_y, scale);
  }


  
  else if (strcmp(argv[3], "blur") == 0) {
    if (argc != 5) {
      printf("Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      free(image1->data);
      free(image1);
      return 5;
    }
    float sigma = atof(argv[4]);
    image_created = blur(image1, sigma);
  }
  

  
  else {
    printf("No operation name was specified, or operation name specified was invalid\n");
    free(image1->data);
    free(image1);
    return 4;
  }
  //free(image1);
  int pixels_written = write_ppm(output, image_created);
  //free(image_created);
  fclose(output);
  printf("Image created with %d pixels.\n", pixels_written);

  free(image1->data);
  free(image1);
  free(image_created->data);
  free(image_created);

  return 0;
}
