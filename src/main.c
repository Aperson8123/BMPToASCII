#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
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

//temporary
#define PATH "C:\\Cstuff\\data\\ColorTest.bmp"

int main(void) {
    FILE* BMPfile = fopen(PATH, "rb");

    if (BMPfile == NULL) {
        fprintf(stderr, "File could not be opened");
        exit(EXIT_FAILURE);
    }
    
    BMPHeader* fileHeader = malloc(sizeof(BMPHeader));

    if (fileHeader == NULL) {
        fprintf(stderr, "File header memory allocation failed");
        exit(EXIT_FAILURE);
    }

    size_t fileHeaderElements = fread(fileHeader, sizeof(BMPHeader), 1, BMPfile);

    if (fileHeaderElements != 1) {
        fprintf(stderr, "Error reading the file header");
        exit(EXIT_FAILURE);
    }

    if (fileHeader->type != 0x4D42) {
        printf("Filetype not recognized as BMP\n");
    }
    
    uint32_t imageSize = fileHeader->imageSize;
    unsigned char* imgBuffer = malloc(imageSize);

    if (imgBuffer == NULL) {
        fprintf(stderr, "Image buffer memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    size_t imgBufferElements = fread(imgBuffer, sizeof(unsigned char), imageSize, BMPfile);

    if (imgBufferElements != imageSize) {
        fprintf(stderr, "Error when populating image buffer");
        exit(EXIT_FAILURE);
    }

    size_t pwidth = fileHeader->width;
    size_t pheight = fileHeader->height;
    uint8_t prowPadding = pwidth % 4;
    size_t rowPaddingSize = (prowPadding) * (pheight);

    pixel* pixelBuffer = malloc(imageSize-rowPaddingSize);

    for (size_t i = 0; i < pheight; i++) {
        size_t pixelOffset = i * (pwidth);
        size_t byteOffset = i * pwidth * sizeof(pixel) + i * prowPadding;
        
        memcpy(pixelBuffer + pixelOffset, imgBuffer + byteOffset, pwidth*sizeof(pixel));
    }

    int a = 9;
    printf("%d, %d, %d\n", pixelBuffer[a].b, pixelBuffer[a].g, pixelBuffer[a].r);
    // pixelCount -= rowPaddingSize/sizeof(pixel);
    // printf("pixelCount: %lld\nrowPadding: %lld\n", pixelCount, rowPaddingSize);
    // int a = 2;
    // printf("%d, %d, %d", pbuffer[a].b, pbuffer[a].g, pbuffer[a].r);

    free(pixelBuffer);
    free(imgBuffer);
    free(fileHeader);
    fclose(BMPfile);
    return 0;
}