/* BASICTEXT */
#include <stdio.h>
#include <stdlib.h>
#if defined (__WIN32__)
    #include <conio.h>
#else
    #include <ncurses.h>
#endif

#define maxLineLength 128
#define maxNumberOfLines 256
#define maxFilenameLength 256
#define ESC 27

#define execute 0
#define saveEditSwitch 1

typedef unsigned char byte;
typedef unsigned short word;

FILE* file;
byte* text;
byte positionInLine;
word currentLine;
byte character = 0;
word i = 0;
word j = 0;
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

byte startNewLine() {
    printf("\n");
    writeCharacter('\n');
    positionInLine = 0;
    currentLine++;
    printf("%d:", currentLine+1);
    printCurrentLine();
}

byte putCursorToEndOfFile() {
    for (currentLine = 0; currentLine < maxNumberOfLines; currentLine++) {
        positionInLine = 0;
        if (readCharacter() == 0) {
            /* If we find a null, this is the last line */
            break;
        }
    }
    return 0;
}

byte moveEverythingDown() {
    /* j holds line number to be inserted */
    /* Find EOF */
    putCursorToEndOfFile();

    /* Move all blocks down relative to EOF*/
    for (currentLine = currentLine; currentLine > j; currentLine--) {
        for (i = 0; i < maxLineLength; i++) {
            text[i + currentLine*maxLineLength] = text[i + (currentLine-1)*maxLineLength];
        }
    }
    return 0;
}

byte clearCurrentLine() {
    for (positionInLine = 0; positionInLine < maxLineLength; positionInLine++) {
        if (readCharacter() == 0) {
            break;
        }
        writeCharacter(0);
    }
    positionInLine = 0;
}

byte moveEverythingUp() {
    /* j holds line number to be removed */
    /* Find EOF */
    putCursorToEndOfFile();
    /* currentLine holds last line in file */

    /* Move all blocks down relative to EOF*/
    for (j = j; j < currentLine; j++) {
        for (i = 0; i < maxLineLength; i++) {
            text[i + (j)*maxLineLength] = text[i + (j+1)*maxLineLength];
        }
    }
    putCursorToEndOfFile();
    return 0;
}

byte openFile() {
    clearBuffer();
    file = fopen(buffer, "rb");
    if (file == NULL) {
        return 1;
    }
    printf("FILE: %s\n", buffer);
    positionInLine = 0;
    currentLine = 0;
    for (i = 0; i < maxLineLength*maxNumberOfLines; i++) {
        int nextChar = getc(file);
        if (nextChar == '\r') {
            continue;
        }
        if ((nextChar == EOF) || (nextChar == 0)) {
            break;
        }        
        writeCharacter(nextChar);
        positionInLine++;

        if ((nextChar == '\n') || (nextChar == 0x0A)) {
            printCurrentLineWithLineNumber();
            printf("\n");
            positionInLine = 0;
            currentLine++;
            continue;
        }
    }
    fclose(file);
    printCurrentLineWithLineNumber();
    return 0;
}

int main(int argc, char *argv[]) {
    printf("\n");
	text = (byte*)calloc(maxLineLength * maxNumberOfLines, sizeof(byte));
	buffer = (char*)calloc(maxFilenameLength, sizeof(char));

	if (argc > 1) {
		/* Get the file */
        buffer = argv[1];
        if (openFile()) {
            printf("\nFILE ERROR\n");
            return 1;
        }
    } else {
        printCurrentLineWithLineNumber();
    }

    while(mode & (1 << execute)) {
        #if defined (__WIN32__)
            character = getch();
        #else
            character = getch(stdin);
        #endif

        if (mode & (1 << saveEditSwitch)) {
            /* Save Mode */
            switch (character) {
                case 'n':
                    printf("NEW? (Y/N) ");
                    scanf_s("%s", buffer);
                    if (buffer[0] == 'Y') {
                        clearBuffer();
                        positionInLine = 0;
                        currentLine = 0;
                        mode = mode ^ (1 << saveEditSwitch);
                        printCurrentLineWithLineNumber();
                    } else {
                        printf("\nCMD? ");
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
                /* Insert line */
                case 'i':
                    printf("INSERT? ");
                    scanf_s("%s", buffer);
                    j = atoi(buffer)-1;
                    moveEverythingDown();
                    mode = mode ^ (1 << saveEditSwitch);
                    clearCurrentLine();
                    printCurrentLineWithLineNumber();
                    break;
                /* Insert line */
                case 'd':
                    printf("DELETE? ");
                    scanf_s("%s", buffer);
                    j = atoi(buffer)-1;
                    moveEverythingUp();
                    mode = mode ^ (1 << saveEditSwitch);
                    printCurrentLineWithLineNumber();
                    break;
                /* List */
                case 'l':
                    printf("LIST:\n");
                    positionInLine = 0;
                    currentLine = 0;
                    for (i = 0; i < maxLineLength*maxNumberOfLines; i++) {
                        byte nextChar = readCharacter();
                        if (nextChar == 0) {
                            break;
                        }        
                        positionInLine++;

                        if (nextChar == '\n') {
                            printCurrentLineWithLineNumber();
                            printf("\n");
                            currentLine++;
                            positionInLine = 0;
                            continue;
                        }
                    }
                    printCurrentLineWithLineNumber();
                    mode = mode ^ (1 << saveEditSwitch);
                    break;
                /* Print line */
                case 'r':
                    printf("READLINE? ");
                    scanf_s("%s", buffer);
                    currentLine = atoi(buffer)-1;
                    printCurrentLineWithLineNumber();
                    printf("\nCMD? ");
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
                    printf("\nCMD? ");
                    break;
                /* Open */
                case 'o':
                    printf("OPEN? ");
                    readFilename();
                    openFile();
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
                startNewLine();
                continue;
            }
            if (currentLine >= maxNumberOfLines) {
                printf("\a");
                continue;
            }
            switch (character) {
                case ESC:
                    mode = mode ^ (1 << saveEditSwitch);
                    printf("\nCMD? ");
                    break;
                case '\r':
                case '\n':
                    startNewLine();
                    break;
                case '\b':
                    if (positionInLine > 0) {
                        putchar(character);
                        /* Hack to overwrite the previous character */
                        putchar(' ');
                        putchar(character);
                        positionInLine--;
                        writeCharacter(0);
                    } else {
                        printf("\a");
                    }
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