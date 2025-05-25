//Ecrire le code test
#include "bmp8.h"
#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
    int running = 1;
    int reponse, filtre, value,type;
    char file[100];
    t_bmp8 * fichier8 = NULL;
    t_bmp24 * fichier24 = NULL;
    
    //Affichage de l'interface en ligne de comande
    printf("-----------------------------------------------\n _   _  _____  _       _       ___    _  \n"
        "| | | || ____|| |     | |     / _ \\  | |\n"
        "| |_| ||  _|  | |     | |    | | | | |_|\n"
        "|  _  || |___ | |___  | |___ | |_| |  _\n"
        "|_| |_||_____||_____| |_____| \\___/  |_|\n\nWhat type of image do you want to process :\n1. bmp8\n2. bmp24\n3. Quit\n");
    scanf("%d",&type);
    while(running){

        //------------------------------------------ Traitemet en BMP8 -------------------------------------------------

        if(type == 1){
            printf("-----------------------------------------------\nChoose an option :\n1. Open a picture\n2. Save a picture\n3. Apply filters\n4. See picture's datas\n5. Quit\n");
            scanf("%d",&reponse);
            printf("Your choice : %d\n-----------------------------------------------\n\n",reponse);

            //Premier cas, on load l'image en mémoire
            if(reponse == 1){
                printf("Please put the file path : ");
                scanf("%s",file);
                fichier8 = bmp8_loadImage(file);
            }

            //Deuxième cas, sauvegarde du fichier
            if(reponse == 2){
                bmp8_saveImage(file,fichier8);
            }

            //Troisième cas, application d'un filtre
            if(reponse == 3){
                //Choix du filtre à appliquer
                printf("Choose a filter to apply :\n1. Negative\n2. Brightness\n3. Treshold\n4. Box blur\n5. Gaussian Blur\n6. Sharpen\n7. Outline\n8. emboss\n9. Back\n");
                scanf("%d",&filtre);
                printf("Your choice : %d\n-----------------------------------------------\n\n",filtre);
                    
                if(filtre == 1){
                    //Exécution du négatif
                    bmp8_negative(fichier8);
                    printf("Negative applied\n");
                }

                if(filtre == 2){
                    printf("Value for brightness : \n");
                    scanf("%d",&value);
                    // Exécution de brightness
                    bmp8_brightness(fichier8,value);
                    printf("Brightness has been modified of %d\n",value);
                }

                if(filtre == 3){
                    printf("Treshold value : \n");
                    scanf("%d",&value);
                    // Exécution de la binarisation
                    bmp8_treshold(fichier8,value);
                    printf("Treshold applied\n");
                }

                if (filtre == 4) {
                    // Box blur
                    float **boxBlur = (float **)malloc(3 * sizeof(float *));
                    for (int i = 0; i < 3; i++) {
                        boxBlur[i] = (float *)malloc(3 * sizeof(float));
                        for (int j = 0; j < 3; j++) {
                            boxBlur[i][j] = 1.0/9.0;
                        }
                    }
                    bmp8_applyFilter(fichier8, boxBlur, 3);
                    printf("Box blur applied\n");
                    // Libération de la mémoire
                    for (int i = 0; i < 3; i++) {
                        free(boxBlur[i]);
                    }
                    free(boxBlur);
                }

                if (filtre == 5) {
                    // Gaussian blur
                    float **gaussianBlur = (float **)malloc(3 * sizeof(float *));
                    for (int i = 0; i < 3; i++) {
                        gaussianBlur[i] = (float *)malloc(3 * sizeof(float));
                    }
                
                    // Remplissage du noyau
                    gaussianBlur[0][0] = 1.0/16.0; gaussianBlur[0][1] = 2.0/16.0; gaussianBlur[0][2] = 1.0/16.0;
                    gaussianBlur[1][0] = 2.0/16.0; gaussianBlur[1][1] = 4.0/16.0; gaussianBlur[1][2] = 2.0/16.0;
                    gaussianBlur[2][0] = 1.0/16.0; gaussianBlur[2][1] = 2.0/16.0; gaussianBlur[2][2] = 1.0/16.0;
                
                    bmp8_applyFilter(fichier8, gaussianBlur, 3);
                    printf("Gaussian blur applied\n");
                
                    // Libération de la mémoire
                    for (int i = 0; i < 3; i++) {
                        free(gaussianBlur[i]);
                    }
                    free(gaussianBlur);
                } 

                if (filtre == 6) {
                    // Sharpen
                    float **sharpen = (float **)malloc(3 * sizeof(float *));
                    for (int i = 0; i < 3; i++) {
                        sharpen[i] = (float *)malloc(3 * sizeof(float));
                    }
                
                    // Remplissage du noyau
                    sharpen[0][0] = 0.0; sharpen[0][1] = -1.0; sharpen[0][2] = 0.0;
                    sharpen[1][0] = -1.0; sharpen[1][1] = 5.0; sharpen[1][2] = -1.0;
                    sharpen[2][0] = 0.0; sharpen[2][1] = -1.0; sharpen[2][2] = 0.0;
                
                    bmp8_applyFilter(fichier8, sharpen, 3);
                    printf("Sharpen applied\n");
                
                    // Libération de la mémoire
                    for (int i = 0; i < 3; i++) {
                        free(sharpen[i]);
                    }
                    free(sharpen);
                }

                if (filtre == 7) {
                    // Outline
                    float **outline = (float **)malloc(3 * sizeof(float *));
                    for (int i = 0; i < 3; i++) {
                        outline[i] = (float *)malloc(3 * sizeof(float));
                    }
                
                    // Remplissage du noyau
                    outline[0][0] = -1.0; outline[0][1] = -1.0; outline[0][2] = -1.0;
                    outline[1][0] = -1.0; outline[1][1] = 8.0; outline[1][2] = -1.0;
                    outline[2][0] = -1.0; outline[2][1] = -1.0; outline[2][2] = -1.0;
                
                    bmp8_applyFilter(fichier8, outline, 3);
                    printf("Outline applied\n");
                
                    // Libération de la mémoire
                    for (int i = 0; i < 3; i++) {
                        free(outline[i]);
                    }
                    free(outline);
                }

                if (filtre == 8) {
                    // Emboss
                    float **emboss = (float **)malloc(3 * sizeof(float *));
                    for (int i = 0; i < 3; i++) {
                        emboss[i] = (float *)malloc(3 * sizeof(float));
                    }
                
                    // Remplissage du noyau
                    emboss[0][0] = -2.0; emboss[0][1] = -1.0; emboss[0][2] = 0.0;
                    emboss[1][0] = -1.0; emboss[1][1] = 1.0; emboss[1][2] = 1.0;
                    emboss[2][0] = 0.0; emboss[2][1] = 1.0; emboss[2][2] = 2.0;
                
                    bmp8_applyFilter(fichier8, emboss, 3);
                    printf("Emboss applied\n");
                
                    // Libération de la mémoire
                    for (int i = 0; i < 3; i++) {
                        free(emboss[i]);
                    }
                    free(emboss);
                }
            }
            
            //Quatrième cas, on affiche les informations de l'image
            if(reponse == 4){
                bmp8_printInfo(fichier8);
            }

            //On quitte l'interface
            if(reponse == 5){
                //On libère la mémoire pour éviter les fuites de données
                bmp8_free(fichier8);
                printf("--------------------------------------------------\n\n    +------------+\n    |    ^  ^    |\n    |     --     |\n    |    \\__/    |\n    +------------+\n\nHave a nice day ! ;)\n");
                running = 0;
            }
        }

        //---------------------------------------------------- Traitement en BMP24 --------------------------------------
        else if(type == 2){
            printf("-----------------------------------------------\nChoose an option :\n1. Open a picture\n2. Save a picture\n3. Apply filters\n4. Quit\n");
            scanf("%d",&reponse);
            printf("Your choice : %d\n-----------------------------------------------\n\n",reponse);
            //Créer tout le menu pour la partie bmp24

             //Premier cas, on load l'image en mémoire
            if(reponse == 1){
                printf("Please put the file path : ");
                scanf("%s",file);
                fichier24 = bmp24_loadImage(file);
            }

            //Deuxième cas, on sauvegarde l'image
            if(reponse == 2){
                bmp24_saveImage(fichier24,file);
            }

            //Troisième cas, application d'un filtre
            if(reponse == 3){
                //Choix du filtre à appliquer
                printf("Choose a filter to apply :\n1. Negative\n2. Brightness\n3. grayscale\n4. Box blur\n5. Gaussian Blur\n6. Sharpen\n7. Outline\n8. emboss\n9. Back\n");
                scanf("%d",&filtre);
                printf("Your choice : %d\n-----------------------------------------------\n\n",filtre);

                //Modifier pour chaque filtre -----
                if(filtre == 1){
                    //Exécution du négatif
                    bmp24_negative(fichier24);
                    printf("Negative applied\n");
                }

                if(filtre == 2){
                    printf("Value for brightness : \n");
                    scanf("%d",&value);
                    // Exécution de brightness
                    bmp24_brightness(fichier24,value);
                    printf("Brightness has been modified of %d\n",value);
                }

                if(filtre == 3){
                    // Exécution de la conversion en niveaux de gris
                    bmp24_grayscale(fichier24);
                    printf("Grayscale applied\n");
                }

                if (filtre == 4) {
                    // Box blur - utilisation de la nouvelle fonction
                    bmp24_boxBlur(fichier24);
                    printf("Box blur applied\n");
                }

                if (filtre == 5) {
                    // Gaussian blur - à implémenter
                    // bmp24_gaussianBlur(fichier24);
                    printf("Gaussian blur - Function not implemented yet\n");
                } 

                if (filtre == 6) {
                    // Sharpen - à implémenter
                    // bmp24_sharpen(fichier24);
                    printf("Sharpen - Function not implemented yet\n");
                }

                if (filtre == 7) {
                    // Outline - à implémenter
                    // bmp24_outline(fichier24);
                    printf("Outline - Function not implemented yet\n");
                }

                if (filtre == 8) {
                    // Emboss - à implémenter
                    // bmp24_emboss(fichier24);
                    printf("Emboss - Function not implemented yet\n");
                }
            break;
            }

            //On quitte l'interface
            if(reponse == 4){
                //On libère la mémoire pour éviter les fuites de données
                bmp24_free(fichier24);
                printf("--------------------------------------------------\n\n    +------------+\n    |    ^  ^    |\n    |     --     |\n    |    \\__/    |\n    +------------+\n\nHave a nice day ! ;)\n");
                running = 0;
            }

        //Quitter le programme    
        else if(type == 3){
            printf("--------------------------------------------------\n\n    +------------+\n    |    ^  ^    |\n    |     --     |\n    |    \\__/    |\n    +------------+\n\nHave a nice day ! ;)\n");
            running = 0;
        }
        }
    }
    return 0;
}
