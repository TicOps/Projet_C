//Ecrire le code test
#include "bmp8.h"
#include <stdio.h>


int main(){
    int running = 1;
    int reponse, filtre, value;
    char file[100];
    t_bmp8 *fichier;
    while(running){
        //Affichage de l'interface en ligne de comande
        printf("-----------------------------------------------\nChoose an option :\n1. Open a picture\n2. Save a picture\n3. Apply filters\n4. See picture's datas\n5. Quit\n");
        scanf("%d",&reponse);
        printf("Your choice : %d\n-----------------------------------------------\n",reponse);

        //Premier cas, on load l'image en mémoire
        if(reponse == 1){
            printf("Please put the file path : ");
            scanf("%s",file);
            fichier = bmp8_loadImage(file);
        }

        //Deuxième cas, sauvegarde du fichier
        if(reponse == 2){
            bmp8_saveImage(file,fichier);
        }

        //Troisième cas, application d'un filtre
        if(reponse == 3){
            //Choix du filtre à appliquer
            printf("Choose a filter to apply :\n1. Negative\n2. Brightness\n3. Treshold\n4. Blur\n5. Gaussian Blur\n6. Sharpen\n7. Outline\n8. emboss\n9. Back\n");
            scanf("%d",&filtre);
            printf("Your choice : %d\n-----------------------------------------------\n",filtre);
                
            if(filtre == 1){
                //Exécution du négatif
                bmp8_negative(fichier);
                printf("Negative applied\n");
            }

            if(filtre == 2){
                printf("Value for brightness : \n");
                scanf("%d",&value);
                // Exécution de brightness
                bmp8_brightness(fichier,value);
                printf("Brightness has been modified of %d\n",value);
            }

            if(filtre == 3){
                printf("Treshold value : \n");
                scanf("%d",&value);
                // Exécution de la binarisation
                bmp8_treshold(fichier,value);
                printf("Treshold applied\n");
            }
        }
        
        //Cinquième cas, on affiche les informations de l'image
        if(reponse == 4){
            bmp8_printInfo(fichier);
        }

        //On quitte l'interface
        if(reponse == 5){
            //On libère la mémoire pour éviter les fuites de données
            bmp8_free(fichier);
            printf("Have a nice day ! ;)\n");
            running = 0;
        }
    }
    return 1;
};

