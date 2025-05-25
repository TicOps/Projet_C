#include "bmp24.h"
#include <stdio.h>      
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>     
#include <math.h>   


void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

/*---------------------------------------------------------------------------------------------*/


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
    free(data);
    bmp24_free(bmp24);
    return NULL;
  }
  bmp24->width = width;
  bmp24->height = height;
  bmp24->colorDepth = colorDepth;
  if (colorDepth != 24) {
    printf("Error : invalid color depth (%d)\n", colorDepth);
    free(data);
    bmp24_free(bmp24);
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
  if (!(image->height%4 == 0) || !(image->width%4 == 0)) {
    printf("Error : invalid image size (%d,%d)\n", image->height, image->width);
    return;
  }
  int pos;
  x = image->width - x * image->height;
  pos = x + y;
  uint32_t header_offset = HEADER_SIZE + INFO_SIZE + BITMAP_OFFSET;
  t_pixel *pixel = (t_pixel *) malloc(sizeof(t_pixel));
  if (file == NULL) {
    printf("Error : could not open file");
    fclose(file);
    return;
  }
  file_rawRead(header_offset, &pixel, sizeof(t_pixel), 1, file);
  uint8_t temp = pixel->red;
  printf("pixel_value:%d,%d,%d\n", pixel->red, pixel->green, pixel->blue);
  pixel->red = pixel->blue;
  pixel->blue = temp;
  printf("pixel_value:%d,%d,%d\n", pixel->red, pixel->green, pixel->blue);
  free(pixel);
  fclose(file);

}

t_bmp24 * bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error : could not open file %s\n", filename);
        return NULL;
    }

    t_bmp_header header;
    // Lire l'en-tête BMP depuis le début du fichier
    file_rawRead(BITMAP_MAGIC, &header, sizeof(t_bmp_header), 1, file);

    // Vérifier le magic number BMP
    if (header.type != BMP_TYPE) {
        printf("Error: Not a valid BMP file (magic: 0x%X)\n", header.type);
        fclose(file);
        return NULL;
    }
    printf("%d\n",sizeof(t_bmp_header));
    t_bmp_info header_info;
    // Lire les informations de l'image après l'en-tête principal
    file_rawRead(HEADER_SIZE, &header_info, sizeof(t_bmp_info), 1, file);

    int width = header_info.width;
    int height = header_info.height;
    int colorDepth = header_info.bits;

    printf("Image dimensions: %dx%d, Color depth: %d\n", width, height, colorDepth);

    // Vérifier que c'est bien du 24-bit
    if (colorDepth != 24) {
        printf("Error: Only 24-bit BMP files are supported (found %d-bit)\n", colorDepth);
        fclose(file);
        return NULL;
    }

    t_bmp24 *img = bmp24_allocate(width, height, colorDepth);
    if (!img) {
        printf("Error: Failed to allocate image structure\n");
        fclose(file);
        return NULL;
    }

    img->header = header;
    img->header_info = header_info;

    // Allouer les données pixel
    img->data = bmp24_allocateDataPixels(img->width, img->height);
    if (img->data == NULL) {
        printf("Error : memory allocation failed for pixel data\n");
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
  file_rawWrite(BITMAP_MAGIC, &img->header, sizeof(t_bmp_header), 1, file);
  file_rawWrite(HEADER_SIZE, &img->header_info, sizeof(t_bmp_info), 1, file);

  fclose(file);
}

void bmp24_negative(t_bmp24 * img){
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

void bmp24_grayscale(t_bmp24 * img) {
    // Vérification des paramètres
    if (!img || !img->data) {
        printf("Error: No picture loaded or missing data!\n");
        return;
    }

    // Parcours de tous les pixels de l'image
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Récupération des valeurs RGB du pixel actuel
            uint8_t red = img->data[y][x].red;
            uint8_t green = img->data[y][x].green;
            uint8_t blue = img->data[y][x].blue;
            //On fait la moyenne des 3
            uint8_t gray = (red + green + blue) / 3;
            
            // Affectation de la valeur de gris à toutes les composantes
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}

void bmp24_brightness(t_bmp24 * img, int value) {
    // Vérification des paramètres
    if (!img || !img->data) {
        printf("Error: No picture loaded or missing data!\n");
        return;
    }
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            //rouge
            int newRed = img->data[y][x].red + value;
            if (newRed > 255) {
                newRed = 255;
            } else if (newRed < 0) {
                newRed = 0;
            }
            img->data[y][x].red = (uint8_t)newRed;
            
            //vert
            int newGreen = img->data[y][x].green + value;
            if (newGreen > 255) {
                newGreen = 255;
            } else if (newGreen < 0) {
                newGreen = 0;
            }
            img->data[y][x].green = (uint8_t)newGreen;
            
            //bleu
            int newBlue = img->data[y][x].blue + value;
            if (newBlue > 255) {
                newBlue = 255;
            } else if (newBlue < 0) {
                newBlue = 0;
            }
            img->data[y][x].blue = (uint8_t)newBlue;
        }
    }
}

t_pixel bmp24_convolution(t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {
    // Vérification des paramètres
    if (!img || !img->data || !kernel) {
        printf("Error: Invalid parameters for convolution!\n");
        t_pixel errorPixel = {0, 0, 0};
        return errorPixel;
    }
    
    // Calcul du rayon du kernel (n = kernelSize / 2)
    int radius = kernelSize / 2;
    
    // Variables pour stocker les sommes pondérées de chaque composante
    float sumRed = 0.0;
    float sumGreen = 0.0;
    float sumBlue = 0.0;
    
    // Application du noyau de convolution
    // Parcours du kernel de -radius à +radius
    for (int j = -radius; j <= radius; j++) {
        for (int i = -radius; i <= radius; i++) {
            // Position du pixel dans l'image originale
            int imgX = x + i;
            int imgY = y + j;
            
            // Vérification des limites de l'image
            if (imgX >= 0 && imgX < img->width && imgY >= 0 && imgY < img->height) {
                // Position dans le noyau
                int kernelX = i + radius;
                int kernelY = j + radius;
                
                // Récupération du pixel de l'image
                t_pixel currentPixel = img->data[imgY][imgX];
                sumRed += currentPixel.red * kernel[kernelY][kernelX];
                sumGreen += currentPixel.green * kernel[kernelY][kernelX];
                sumBlue += currentPixel.blue * kernel[kernelY][kernelX];
            }
        }
    }
    // Gestion des valeurs hors limites [0, 255]
    if (sumRed > 255) sumRed = 255;
    if (sumRed < 0) sumRed = 0;
    
    if (sumGreen > 255) sumGreen = 255;
    if (sumGreen < 0) sumGreen = 0;
    
    if (sumBlue > 255) sumBlue = 255;
    if (sumBlue < 0) sumBlue = 0;
    
    // Création du pixel résultat
    t_pixel resultPixel;
    resultPixel.red = (uint8_t)sumRed;
    resultPixel.green = (uint8_t)sumGreen;
    resultPixel.blue = (uint8_t)sumBlue;
    
    return resultPixel;
}

void bmp24_boxBlur(t_bmp24 * img) {
    // Vérification des paramètres
    if (!img || !img->data) {
        printf("Error: No picture loaded or missing data!\n");
        return;
    }

    // Création du noyau box blur 3x3
    float **boxBlurKernel = (float **)malloc(3 * sizeof(float *));
    if (!boxBlurKernel) {
        printf("Error: Memory allocation for kernel!\n");
        return;
    }
    
    for (int i = 0; i < 3; i++) {
        boxBlurKernel[i] = (float *)malloc(3 * sizeof(float));
        if (!boxBlurKernel[i]) {
            printf("Error: Memory allocation for kernel row!\n");
            // Libération de ce qui a déjà été alloué
            for (int j = 0; j < i; j++) {
                free(boxBlurKernel[j]);
            }
            free(boxBlurKernel);
            return;
        }
        
        // Remplissage du noyau : chaque élément = 1/9
        for (int j = 0; j < 3; j++) {
            boxBlurKernel[i][j] = 1.0f / 9.0f;
        }
    }
    
    // Création d'une image temporaire pour stocker les résultats
    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (!tempImg) {
        printf("Error: Memory allocation for temporary image!\n");
        // Libération du kernel
        for (int i = 0; i < 3; i++) {
            free(boxBlurKernel[i]);
        }
        free(boxBlurKernel);
        return;
    }
    
    // Copie des en-têtes
    tempImg->header = img->header;
    tempImg->header_info = img->header_info;
    
    // Copie de l'image originale dans l'image temporaire
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = img->data[y][x];
        }
    }
    
    // Application du filtre box blur (en évitant les bords)
    int radius = 1; // Pour un noyau 3x3, le rayon est 1
    for (int y = radius; y < img->height - radius; y++) {
        for (int x = radius; x < img->width - radius; x++) {
            // Utilisation de bmp24_convolution pour calculer le nouveau pixel
            t_pixel newPixel = bmp24_convolution(tempImg, x, y, boxBlurKernel, 3);
            
            // Mise à jour du pixel dans l'image originale
            img->data[y][x] = newPixel;
        }
    }
    
    // Libération de la mémoire
    bmp24_free(tempImg);
    for (int i = 0; i < 3; i++) {
        free(boxBlurKernel[i]);
    }
    free(boxBlurKernel);
    
    printf("Box blur applied successfully!\n");
}

int main(void){
  t_bmp24 *img = bmp24_allocate(512,512,24);
  char *filename = "flowers_color.bmp";
  FILE *file = fopen(filename, "rb");
  bmp24_readPixelValue(img,0,0,file);
  fclose(file);
  return 1;
}


