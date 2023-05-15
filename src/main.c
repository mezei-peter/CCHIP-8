#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "args_service.h"

int main(int argc, char *argv[])
{
  ArgsService *args_service = new_args_service(argv, argc);
  FILE *rom_file = fopen(args_service->file_path, "rb");
  if (rom_file == NULL)
  {
    printf("Invalid file. Please pass a valid file path as an argument.\n");
    return 1;
  }

  return 0;
}
