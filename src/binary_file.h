#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct BinaryFile
{
    uint8_t *bytes;
    size_t size;

} BinaryFile;

BinaryFile *binfile_new();
int binfile_read(BinaryFile *binary_file, char *path);
