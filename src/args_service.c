#include "args_service.h"
#include <stdio.h>
#include <stdlib.h>

ArgsService *new_args_service(char *args[], int length)
{
    ArgsService *args_service = malloc(sizeof(ArgsService));
    if (length > 1)
    {
        args_service->file_path = args[1];
    }
    else
    {
        args_service->file_path="";
    }
    return args_service;
}
