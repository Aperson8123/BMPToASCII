#ifndef BMP_C
#define BMP_C

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t colorPanes;
    uint16_t colorDepth;
    uint32_t compressionMethod;
    uint32_t imageSize;
    uint32_t pwidth;;
    uint32_t pheight; 
    uint32_t colorPaletteCount;
    uint32_t importantColors;
} BMPHeader;

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} pixel;
#pragma pack(pop)

BMPHeader* newHeader(FILE* file);
pixel getPixel(pixel* pbuffer, uint16_t width, uint16_t y, uint16_t x);
uint8_t getBrightness(pixel p);
void fillPixelBuffer(pixel** buffer, BMPHeader* header, FILE* file);

#endif