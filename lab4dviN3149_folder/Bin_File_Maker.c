#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для хранения индекса и смещения
typedef struct {
    int index;
    int offset;
} IndexOffset;

// Структура для хранения длины строки и самой строки
typedef struct {
    int length;
    char* string;
} LengthString;

void Write_To_File(const char* filename, int n, IndexOffset* ioArray, LengthString* lsArray) 
{
    FILE* file = fopen(filename, "wb");
    if (!file) 
    {
        perror("\nОшибка:\n\t\tНе удалось открыть файл");
        exit(EXIT_FAILURE);
    }

    // Запись n
    fwrite(&n, sizeof(int), 1, file);

    // Запись области i, o
    for (int i = 0; i < n; i++) 
    {
        fwrite(&ioArray[i].index, sizeof(int), 1, file);
        fwrite(&ioArray[i].offset, sizeof(int), 1, file);
    }

    // Запись мусора длинной в 100 байт
    char emptySpace[100] = {0};
    fwrite(emptySpace, sizeof(char), 100, file);

    // Запись области k, s
    for (int i = 0; i < n; i++) 
    {
        fwrite(&lsArray[i].length, sizeof(int), 1, file);
        fwrite(lsArray[i].string, sizeof(char), lsArray[i].length, file);
    }

    fclose(file);
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) {
        fprintf(stderr, "Используйте: %s <n_value> <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    const char* filename = argv[2];

    if (n <= 0) 
    {
        perror("\nОшибка:\n\t\tКоличество строк должно быть натуральным числом");
        exit(EXIT_FAILURE);
    }

    IndexOffset* ioArray = (IndexOffset*)malloc(n * sizeof(IndexOffset));
    LengthString* lsArray = (LengthString*)malloc(n * sizeof(LengthString));

    if (!ioArray || !lsArray) {
        perror("\nОшибка:\n\t\tНе удалось выделить память");
        exit(EXIT_FAILURE);
    }

    int currentOffset = 0;

    for (int i = 0; i < n; i++) 
    {
        char buffer[256];
        printf("Enter string %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n') 
        {
            buffer[len - 1] = '\0';
            len--;
        }

        lsArray[i].length = (int)len;
        lsArray[i].string = (char*)malloc((len + 1) * sizeof(char));
        strcpy(lsArray[i].string, buffer);

        ioArray[i].index = i;
        ioArray[i].offset = currentOffset;
        currentOffset += sizeof(int) + len;
    }

    Write_To_File(filename, n, ioArray, lsArray);

    for (int i = 0; i < n; i++) 
    {
        free(lsArray[i].string);
    }

    free(ioArray);
    free(lsArray);

    return 0;
}
