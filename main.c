#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   

#define maxLineLength 128
#define maxNumberOfLines 256
#define maxFilenameLength 256
#define ESC 27

#define execute 0
#define saveEditSwitch 1

typedef unsigned char byte;
typedef unsigned short word;

byte* text;
byte positionInLine;
word currentLine;
byte character = 0;
word i = 0;
byte mode = 0 | (1 << execute);
char* buffer;

int readFilename() {
    scanf_s("%s", buffer);
    /*fgets(filename, maxFilenameLength-1, stdin);*/
    printf("FILENAME: %s\n", buffer);
    return 0;
}

byte writeCharacter(byte character) {
    text[positionInLine + currentLine*maxLineLength] = character;
    return 0;
}

byte readCharacter() {
    return text[positionInLine + currentLine*maxLineLength];
}

byte printCurrentLine() {
    for (positionInLine = 0; positionInLine < maxLineLength; positionInLine++) {
        byte character = readCharacter();
        if ((character == 0) || (character == '\n')) {
            break;
        }
        putchar(character);
    }
}

byte printCurrentLineWithLineNumber() {
    printf("%d:", currentLine+1);
    printCurrentLine();
}

byte clearBuffer() {
    for (i = 0; i < maxLineLength * maxNumberOfLines; i++) {
        text[i] = 0;
    }
    return 0;
}

int main() {
    FILE* file;
	text = (byte*)calloc(maxLineLength * maxNumberOfLines, sizeof(byte));
	buffer = (char*)calloc(maxFilenameLength, sizeof(char));

    printf("%d:", currentLine+1);
    while(mode & (1 << execute)) {
        character = getch();
        if (mode & (1 << saveEditSwitch)) {
            /* Save Mode */
            switch (character) {
                case 'n':
                    printf("NEW? (Y/N) ");
                    scanf_s("%s", buffer);
                    if ("Y") {
                        clearBuffer();
                    }
                    break;
                /* Goto line */
                case 'g':
                    printf("GOTO? ");
                    scanf_s("%s", buffer);
                    currentLine = atoi(buffer)-1;
                    mode = mode ^ (1 << saveEditSwitch);
                    printCurrentLineWithLineNumber();
                    break;
                /* Edit */
                case 'e':
                    mode = mode ^ (1 << saveEditSwitch);
                    printf("EDIT\n");
                    printCurrentLineWithLineNumber();
                    break;
                /* Save */
                case 's':
                    printf("SAVE? ");
                    readFilename();
                    file = fopen(buffer, "wb");
                    for (i = 0; i < maxLineLength*maxNumberOfLines; i++) {
                        if (text[i] == 0) {
                            continue;
                        }
                        fputc(text[i],file);
                    }
                    fclose(file);
                    break;
                /* Open */
                case 'o':
                    printf("OPEN? ");
                    readFilename();
                    clearBuffer();
                    file = fopen(buffer, "rb");
                    positionInLine = 0;
                    currentLine = 0;
                    printf("%d:", currentLine+1);
                    for (i = 0; i < maxLineLength*maxNumberOfLines; i++) {
                        int nextChar = getc(file);
                        if ((nextChar == EOF) || (nextChar == 0)) {
                            break;
                        }
                        
                        writeCharacter(nextChar);
                        positionInLine++;

                        if (nextChar == '\n') {
                            currentLine++;
                            printf("\n%d:", currentLine+1);
                            continue;
                        }
                        putchar(nextChar);
                    }
                    fclose(file);
                    mode = mode ^ (1 << saveEditSwitch);
                    break;
                /* Exit */
                case 'x':
                    printf("BYE!\n");
                    return 0;
                default:
                    break;
            }
        } else {
            /* Edit Mode */
            if (positionInLine >= maxLineLength) {
                positionInLine = maxLineLength;
                printf("\a");
            }
            if (currentLine >= maxNumberOfLines) {
                currentLine = maxNumberOfLines;
                printf("\a");
            }
            switch (character) {
                case ESC:
                    mode = mode ^ (1 << saveEditSwitch);
                    printf("\nCMD? ");
                    break;
                case '\r':
                case '\n':
                    printf("\n");
                    writeCharacter('\n');
                    positionInLine = 0;
                    currentLine++;
                    printf("%d:", currentLine+1);
                    printCurrentLine();
                    break;
                case '\b':
                    putchar(character);
                    positionInLine--;
                    writeCharacter(0);
                    break;
                default:
                    putchar(character);
                    writeCharacter(character);
                    positionInLine++;
                    break;
            }
        }
    }
    printf("\nERROR\n");
    return 1;
}