#include "bmp24.h"
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

