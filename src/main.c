#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset; 
} BMPHeader;

//temporary
#define PATH "C:\\Cstuff\\data\\ColorTest.bmp"

int main(void) {
    FILE* BMPfile = fopen(PATH, "rb");

    if (BMPfile == NULL) {
        fprintf(stderr, "File could not be opened");
        exit(EXIT_FAILURE);
    }

    BMPHeader fileHeader;
    size_t fileHeaderElements;

    fileHeaderElements = fread(&fileHeader, sizeof(BMPHeader), 1, BMPfile);

    if (fileHeader.type == 0x4D42) {
        printf("BMP\n");
    }

    fclose(BMPfile);
    return 0;
}