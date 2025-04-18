//Ecrire le code test
#include "bmp8.h"
#include <stdio.h>


int main(){
    t_bmp8 *fichier = bmp8_loadImage("./barbara_gray.bmp");
    bmp8_printInfo(fichier);
    return 0;
};

