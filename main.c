#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   

#define maxLineLength 128
#define maxNumberOfLines 256
#define maxFilenameLength 256
#define ESC 27

#define execute 1 << 0
#define saveEditSwitch 1 << 1

typedef unsigned char byte;
typedef unsigned short word;

byte* text;
word position;
byte character = 0;
word i = 0;
byte mode = 0 | execute;
byte* filename;

int readFilename() {
    i = 0;
    while (character != '\n') {
        filename[i] = character;
        putchar(character);
        i++;
    }
    return 0;
}

int main() {
    FILE* file;
	text = (byte*)malloc(maxLineLength * maxNumberOfLines * sizeof(byte));
	filename = (byte*)malloc(maxFilenameLength * sizeof(byte));

    while(mode & execute) {
        character = getch();
        if (mode & saveEditSwitch) {
            /* Save Mode */
            switch (character) {
                /* Edit */
                case 'e':
                    mode = mode & ~saveEditSwitch;
                    printf("EDIT\n");
                    break;
                /* Save */
                case 's':
                    readFilename();
                    file = fopen(filename, "wb");
                    for (i = 0; i < maxLineLength*maxNumberOfLines; i++) {
                        fputc(text[i],file);
                    }
                    fclose(file);
                    break;
                /* Open */
                case 'o':
                    readFilename();
                    file = fopen(filename, "rb");
                    for (i = 0; i < maxLineLength*maxNumberOfLines; i++) {
                        byte nextChar = getc(file);
                        if (nextChar == EOF) {
                            break;
                        }
                        text[i] = nextChar;
                    }
                    fclose(file);
                    break;
                /* Exit */
                case 'x':
                    printf("BYE!\n");
                    return 0;
            }
            printf("\n");
        } else {
            /* Edit Mode */
            if (character == ESC) {
                mode = mode | saveEditSwitch;
                printf("CMD? ");
            }
            putchar(character);
        }
    }
    return 1;
}