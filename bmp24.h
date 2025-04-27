#include <stdint.h>

#ifndef BMP24_H
#define BMP24_H

typedef struct {
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
} t_bmp_header;

typedef struct {
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bits;
	uint32_t compression;
	uint32_t imagesize;
	int32_t xresolution;
	int32_t yresolution;
	uint32_t ncolors;
	uint32_t importantcolors;
} t_bmp_info;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} t_pixel;

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;

#endif //BMP24_H
