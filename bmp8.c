#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filename){

    //Ouverture du fichier en mode read binary
    FILE *file = fopen(filename, "rb");
    if (!file){
        printf("Error : Can't open file %s\n",filename);
        return NULL;
    }

    //Allocation de la structure
    t_bmp8 *img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img){
        printf("Error : Memory allocation\n");
        return NULL;
    }

    //Lecture de l'en-tête
    fread(img->header, sizeof(unsigned char), 54, file);

    //Extraction des dimensions et de la profondeur de couleur
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int * )&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];

    if (img->colorDepth !=8){
        printf("Error : unsupported picture (must be on 8 bits)\n");
        free(img);
        fclose(file);
        return NULL;
    }

    //Lecture de la table des couleurs
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    //Calcul de la taille des données et allocation mémoire
    img->dataSize = img->width * img->height;
    img->data = (unsigned char *)malloc(img->dataSize);
    if (!img->data){
        printf("Error : Memory allocation\n");
        free(img);
        fclose(file);
        return NULL;
    }

    //Lecture des données de l'image
    fread(img->data, sizeof(unsigned char),img->dataSize, file);
    fclose(file);

    printf("Succesfully loaded ! \n");
    return img;
};

void bmp8_saveImage(const char *filename, t_bmp8 *img){

    //Ouvrir le fichier en écriture binaire
    FILE * file = fopen(filename,"wb");
    if(!file){
        printf("Error : Can't create the file %s\n",filename);
        return;
    }

    //Ecriture du header (54 octets)
    if(fwrite(img->header, sizeof(unsigned char), 54, file) != 54){
        printf("Error during writting the header !\n");
        fclose(file);
        return;
    }

    //Ecriture de la palette de couleur (1024 octets pour 256 couleurs * 4)
    if(fwrite(img->colorTable, sizeof(unsigned char), 1024, file) != 1024){
        printf("Error during writting the color table !\n");
        fclose(file);
        return;
    }

    //Ecriture des données de pixels (width * height octets)
    if(fwrite(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize){
        printf("Error during writting picture's data !\n");
        fclose(file);
        return;
    }

    //Tout s'est bien passé, on ferme le fichier
    printf("Succesfully saved !\n");
    fclose(file);
    return;
};

void bmp8_free(t_bmp8 * img){
    //On vérifie que le pointeur n'est pas NULL pour éviter les erreurs
    if(img){
        if(img->data){
            //On vient libérer la mémoire des pixels  allouées par malloc dans bmp8_loadImage
            free(img->data);
        }
        //On vient libérer la structure en elle-même. 
        free(img);
    }
};

void bmp8_printInfo(t_bmp8 *img){
    //Si l'image est vide erreur
    if(!img){
        printf("Error : No picture loaded !\n");
        return;
    }
    //Affiche les info de type unsigned int.
    printf("Picture's datas :\n");
    printf("Width -> %u\n",img->width);
    printf("Height -> %u\n",img->height);
    printf("Color Depth -> %u\n",img->colorDepth);
    printf("Data Size -> %u\n",img->dataSize);
};

void bmp8_negative(t_bmp8 *img){
    // On check qu'il y a bien une img et des données pour éviter les erreurs
    if(!img || !img->data){
        printf("Error : No picture loaded or missing datas !\n");
        return;
    }

    unsigned int i;
    // Parcous via une boucle for sur chaque pixel de l'image
    for (i = 0; i< img->dataSize; i++){
        img->data[i] = (unsigned char)(255 - img->data[i]);
    }
};

void bmp8_brightness(t_bmp8 * img, int value){
    // On check qu'il y a bien une img et des données pour éviter les erreurs
    if(!img || !img->data){
        printf("Error : No picture loaded or missing datas !\n");
        return;
    }

    unsigned int i;
    for(i=0; i < img->dataSize; ++i){
        int p = img->data[i] + value; // La nouvelle valeure potentielle
        
        if(p>255){
            p = 255;
        } else if(p<0){
            p = 0;
        }
        // Affectation de la nouvelle valeure de chaque pixel
        img->data[i] = (unsigned char)p;
    }
};

void bmp8_treshold(t_bmp8 * img, int treshold){
    // On check qu'il y a bien une img et des données pour éviter les erreurs
    if(!img || !img->data){
        printf("Error : No picture loaded or missing datas !\n");
        return;
    }

    if(treshold < 0){
        treshold = 0;
    } else if(treshold > 255){
        treshold = 255;
    }

    unsigned int i;
    for(i = 0; i < img->dataSize; ++i){
        img->data[i] = (unsigned char)(img->data[i] >= treshold ? 255 : 0);

    }
};

// Application des filtres via convolution
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    // Vérification des paramètres
    if (!img || !img->data || !kernel) {
        printf("Error: Invalid parameters for filter application!\n");
        return;
    }

    // Calcul du rayon du kernel (n)
    int radius = kernelSize / 2;
    
    // Création d'une copie des données pour éviter de modifier l'image pendant le traitement
    unsigned char *tempData = (unsigned char *)malloc(img->dataSize);
    if (!tempData) {
        printf("Error: Memory allocation for filter application!\n");
        return;
    }
    
    // Copie des données originales
    memcpy(tempData, img->data, img->dataSize);
    
    // Application du filtre
    int x, y, i, j;
    for (y = radius; y < img->height - radius; y++) {
        for (x = radius; x < img->width - radius; x++) {
            float sum = 0.0;
            
            // Application du noyau de convolution
            for (j = -radius; j <= radius; j++) {
                for (i = -radius; i <= radius; i++) {
                    // Position du pixel dans l'image originale
                    int imgX = x + i;
                    int imgY = y + j;
                    
                    // Position dans le noyau (décalé pour avoir des indices positifs)
                    int kernelX = i + radius;
                    int kernelY = j + radius;
                    
                    // Indice linéaire dans le tableau de données
                    int index = imgY * img->width + imgX;
                    
                    // Somme pondérée
                    sum += tempData[index] * kernel[kernelY][kernelX];
                }
            }
            
            // Gestion des valeurs hors limites
            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;
            
            // Mise à jour du pixel dans l'image
            img->data[y * img->width + x] = (unsigned char)sum;
        }
    }
    
    // Libération de la mémoire temporaire
    free(tempData);
}