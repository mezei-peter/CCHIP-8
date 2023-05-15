#include "binary_file.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

BinaryFile *binfile_new()
{
    BinaryFile *binary_file = malloc(sizeof(BinaryFile));
    binary_file->bytes = NULL;
    binary_file->size = 0;
    return binary_file;
}

int binfile_read(BinaryFile *binary_file, char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    if (file_size == 0)
    {
        fclose(file);
        return -2;
    }

    fseek(file, 0, SEEK_SET);
    uint8_t *bytes = malloc(file_size);
    fread(bytes, sizeof(uint8_t), file_size, file);
    binary_file->bytes = bytes;
    binary_file->size = file_size;

    fclose(file);
    return 0;
}
