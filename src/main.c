#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "args_service.h"

int main(int argc, char *argv[])
{
  ArgsService *args_service = new_args_service(argv, argc);
  printf("%s\n", args_service->file_path);
  return 0;
}
