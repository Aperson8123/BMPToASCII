#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

const char ASCII[] = {'#','0','X','/','~'};

char brightnessToASCII(uint8_t brightness) {
    if (brightness > 255) {
        fprintf(stderr, "Brightness value must not exceed 255");
        exit(EXIT_FAILURE);
    }
    
    uint8_t max = sizeof(ASCII);
    uint8_t resultIndex = max-1;
    
    for (uint8_t i = 0; i < max; i++) {
        if (brightness > 51*(i+1)) {
            resultIndex--;
        }
    }
    return ASCII[resultIndex];
}

// int main(void) {
//     printf("%c\n",brightnessToASCII(255));
//     return 0;
// }