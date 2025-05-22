#include "bmp24.h"
#include <stdio.h>      
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>     
#include <math.h>   


t_pixel ** bmp24_allocateDataPixels(int width, int height){
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        printf("Erreur : échec allocation mémoire pour les pointeurs de lignes\n");
        return NULL;
    }
    // Allocation de chaque ligne de pixels
    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            printf("Erreur : échec allocation mémoire pour la ligne %d\n", i);
            // Libérer les lignes déjà allouées en cas d'échec
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }
    printf("Matrice de pixels %dx%d allouée avec succès\n", width, height);
    return pixels;
}
