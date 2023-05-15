#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "args_service.h"
#include "binary_file.h"

int main(int argc, char *argv[])
{
  ArgsService *args_service = new_args_service(argv, argc);
  BinaryFile *bin_file = binfile_new();
  switch (binfile_read(bin_file, args_service->file_path))
  {
  case -1:
    printf("Error: Invalid file. Please input a valid file.\n");
    return -1;
  
  case -2:
    printf("Error: empty file.\n");
    return -2;
  }

  return 0;
}
