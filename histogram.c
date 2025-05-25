#include "histogram.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    // Vérification des paramètres
    if (!img || !img->data) {
        printf("Error: Invalid image or missing data!\n");
        return NULL;
    }

    // Allocation du tableau pour l'histogramme (256 niveaux de gris)
    unsigned int *histogram = (unsigned int *)calloc(256, sizeof(unsigned int));
    if (!histogram) {
        printf("Error: Memory allocation failed for histogram!\n");
        return NULL;
    }

    // Parcours de tous les pixels de l'image
    for (unsigned int i = 0; i < img->dataSize; i++) {
        unsigned char pixelValue = img->data[i];
        histogram[pixelValue]++;
    }

    return histogram;
}

unsigned int * bmp8_computeCDF(unsigned int * hist) {
    if (!hist) {
        printf("Error: Invalid histogram!\n");
        return NULL;
    }

    // Allocation du tableau pour l'histogramme égalisé
    unsigned int *hist_eq = (unsigned int *)malloc(256 * sizeof(unsigned int));
    if (!hist_eq) {
        printf("Error: Memory allocation failed for equalized histogram!\n");
        return NULL;
    }

    // Calcul de la CDF (Cumulative Distribution Function)
    unsigned int cdf[256];
    cdf[0] = hist[0];

    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Trouver la valeur minimale non-nulle de la CDF
    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (hist[i] != 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    // Nombre total de pixels (dernière valeur de la CDF)
    unsigned int N = cdf[255];

    // Normalisation de la CDF pour obtenir l'histogramme égalisé
    for (int i = 0; i < 256; i++) {
        if (hist[i] == 0) {
            hist_eq[i] = 0;
        } else {
            // Application de la formule de normalisation
            double normalized = ((double)(cdf[i] - cdf_min) / (double)(N - cdf_min)) * 255.0;
            hist_eq[i] = (unsigned int)round(normalized);

            // Assurer que la valeur reste dans [0, 255]
            if (hist_eq[i] > 255) {
                hist_eq[i] = 255;
            }
        }
    }

    return hist_eq;
}

void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq) {
    if (!img || !img->data) {
        printf("Error: Invalid image or missing data!\n");
        return;
    }

    unsigned int *hist = NULL;
    unsigned int *hist_eq_local = NULL;


    hist = bmp8_computeHistogram(img);
    hist_eq = bmp8_computeCDF(hist);


    for (unsigned int i = 0; i < img->dataSize; i++) {
        unsigned char originalValue = img->data[i];
        img->data[i] = (unsigned char)hist_eq[originalValue];
    }

    if (hist_eq) {
        free(hist_eq);
    }

    printf("Histogram equalization applied successfully!\n");
}