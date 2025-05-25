#include "bmp8.h"

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_computeCDF(unsigned int * hist);
void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);

#endif //HISTOGRAM_H
