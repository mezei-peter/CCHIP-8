#include <stdlib.h>

#include "screen.h"

Screen *scrn_new()
{
    Screen *scrn = malloc(sizeof(Screen));
    return scrn;
}

void scrn_free(Screen *scrn)
{
    free(scrn);
}
