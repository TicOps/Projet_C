#include "bmp24.h"
#include "utils.h"
#include <stdio.h>      
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>     
#include <math.h>   


t_pixel ** bmp24_allocateDataPixels(int width, int height){
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        printf("Error : memory allocation failed\n");
        return NULL;
    }
    // Allocation de chaque ligne de pixels
    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            printf("Error : memory allocation failed %d\n", i);
            // Libérer les lignes déjà allouées en cas d'échec
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }
    printf("Pixel matrix %dx%d successfully allocated\n", width, height);
    return pixels;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    // Vérification des paramètres
    if (!pixels) {
        printf("Warning: Attempt to free a NULL pointer\n");
        return;
    }
    
    if (height <= 0) {
        printf("Error : invalid height (%d)\n", height);
        return;
    }
    
    // Libérer chaque ligne de pixels
    for (int i = 0; i < height; i++) {
        if (pixels[i]) {
            free(pixels[i]);
            pixels[i] = NULL;
        }
    }
    free(pixels);
    printf("Pixel matrix successfully released\n");
}

t_bmp24 * bmp24_allocate(int width, int height, int colorDepth){
  t_pixel **data = bmp24_allocateDataPixels(width, height);
  if (data == NULL) {
    printf("Error : memory allocation failed for t_pixel matrix\n");
    free(data);
    return NULL;
  }
  t_bmp24 *bmp24 = (t_bmp24 *)malloc(sizeof(t_bmp24));
  if (!bmp24) {
    printf("Error : t_bmp24 can't be allocated\n");
    bmp24_free(data);
    return NULL;
  }
  bmp24->width = width;
  bmp24->height = height;
  bmp24->colorDepth = colorDepth;
  if (colorDepth != 24) {
    printf("Error : invalid color depth (%d)\n", colorDepth);
    bmp24_free(data);
    return NULL;
  }
  return bmp24;
}

void bmp24_free(t_bmp24 *img){
  if (!img) {
    printf("Error : attempt to free a NULL pointer\n");
    return;
  }
  if (img->data) {
    bmp24_freeDataPixels(img->data, img->height);
  }
  free(img);
  printf("Image successfully released\n");
  return;
}


void bmp24_readPixelValue(t_bmp24 * image,int x,int y,FILE * file){

}

t_bmp24 * bmp24_loadImage(const char *filename){
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error : could not open file %s\n", filename);
    fclose(file);
    return NULL;
  }

  t_bmp_header header;
  file_rawRead(BITMAP_MAGIG, &header, sizeof(t_bmp_header), 1, file);
  t_bmp_info header_info;
  file_rawRead(HEADER_SIZE, header_info, sizeof(t_bmp_info), 1, file);

  int width = header_info.width;
  int height = header_info.height;
  int colorDepth = header_info.bits;

  t_bmp24 *img = bmp24_allocate(width, height, colorDepth);

  img->header = header;
  img->header_info = header_info;

  img->data = bmp24_allocateDataPixels(img->width, img->height);
  if (img->data == NULL) {
    printf("Error : memory allocation failed for t_bmp24\n");
    bmp24_free(img);
    fclose(file);
    return NULL;
  }
  fclose(file);
  return img;
}

void bmp24_saveImage(t_bmp24 *img, const char *filename){
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    printf("Error : could not open file %s\n", filename);
    fclose(file);
    return;
  }
  file_rawWrite(BITMAP_MAGIG, &header, sizeof(t_bmp_header), 1, file);
  file_rawWrite(HEADER_SIZE, &img->header_info, sizeof(t_bmp_info), 1, file);

  fclose(file);
}


void bmp24_negative(t_bmp * img){
  // Vérification des paramètres
    if (!img || !img->data) {
        printf("Error: No picture loaded or missing data!\n");
        return;
    }
    
    // Parcours de tous les pixels de l'image
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Inversion de chaque composante RGB
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}