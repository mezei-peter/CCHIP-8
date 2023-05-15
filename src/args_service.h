#pragma once

typedef struct ArgsService
{
  char *file_path;
} ArgsService;

ArgsService *new_args_service(char *args[], int length);
