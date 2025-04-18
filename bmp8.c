#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filename){

    //Ouverture du fichier en mode read binary
    FILE *file = fopen(filename, "rb");
    if (!file){
        printf("Erreur : impossible d'ouvrir le fichier %s\n",filename);
        return NULL;
    }

    //Allocation de la structure
    t_bmp8 *img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img){
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    //Lecture de l'en-tête
    fread(img->header, sizeof(unsigned char), 54, file);

    //Extraction des dimensions et de la profondeur de couleur
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int * )&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];

    if (img->colorDepth !=8){
        printf("Erreur : image non supportée (doit être en 8 bit)\n");
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
        printf("Erreur d'allocation mémoire pour les pixels\n");
        free(img);
        fclose(file);
        return NULL;
    }

    //Lecture des données de l'image
    fread(img->data, sizeof(unsigned char),img->dataSize, file);
    fclose(file);

    return img;
};

void bmp8_saveImage(const char *filename, t_bmp8 *img){

    //Ouvrir le fichier en écriture binaire
    FILE * file = fopen(filename,"wb");
    if(!file){
        printf("Erreur : Impossible de créer le fichier %s\n",filename);
        return;
    }

    //Ecriture du header (54 octets)
    if(fwrite(img->header, sizeof(unsigned char), 54, file) != 54){
        printf("Erreur de lors de l'écriture de l'en-tête !\n");
        fclose(file);
        return;
    }

    //Ecriture de la palette de couleur (1024 octets pour 256 couleurs * 4)
    if(fwrite(img->colorTable, sizeof(unsigned char), 1024, file) != 1024){
        printf("Erreur lors de l'écriture de la palette de couleurs !\n");
        fclose(file);
        return;
    }

    //Ecriture des données de pixels (width * height octets)
    if(fwrite(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize){
        printf("Erreur lors de l'écriture des données de l'image !\n");
        fclose(file);
        return;
    }

    //Tout s'est bien passé, on ferme le fichier
    printf("Sauvegarde réussie !\n");
    fclose(file);
    return;
}

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
}

void bmp8_printInfo(t_bmp8 *img){
    //Si l'image est vide erreur
    if(!img){
        printf("Erreur : image non chargée !\n");
        return;
    }
    //Affiche les info de type unsigned int.
    printf("Image Info :\n");
    printf("Width : %u\n",img->width);
    printf("Height : %u\n",img->height);
    printf("Color Depth : %u\n",img->colorDepth);
    printf("Data Size : %u\n",img->dataSize);
}