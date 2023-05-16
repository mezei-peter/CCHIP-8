#include <stdlib.h>

#include "keypad.h"

Keypad *keyp_new()
{
    Keypad *keyp = malloc(sizeof(Keypad));
    return keyp;
}

void keyp_free(Keypad *keyp)
{
    free(keyp);
}
