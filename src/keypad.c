#include <stdlib.h>

#include "keypad.h"

Keypad *keyp_new()
{
    Keypad *keyp = malloc(sizeof(Keypad));
    keyp->keys[0].scancode = SDL_SCANCODE_X;
    keyp->keys[1].scancode = SDL_SCANCODE_1;
    keyp->keys[2].scancode = SDL_SCANCODE_2;
    keyp->keys[3].scancode = SDL_SCANCODE_3;
    keyp->keys[4].scancode = SDL_SCANCODE_Q;
    keyp->keys[5].scancode = SDL_SCANCODE_W;
    keyp->keys[6].scancode = SDL_SCANCODE_E;
    keyp->keys[7].scancode = SDL_SCANCODE_A;
    keyp->keys[8].scancode = SDL_SCANCODE_S;
    keyp->keys[9].scancode = SDL_SCANCODE_D;
    keyp->keys[0xA].scancode = SDL_SCANCODE_Z;
    keyp->keys[0xB].scancode = SDL_SCANCODE_C;
    keyp->keys[0xC].scancode = SDL_SCANCODE_4;
    keyp->keys[0xD].scancode = SDL_SCANCODE_R;
    keyp->keys[0xE].scancode = SDL_SCANCODE_F;
    keyp->keys[0xF].scancode = SDL_SCANCODE_V;
    for (int i = 0; i < 16; i++)
    {
        keyp->keys[i].pressed = false;
    }
    return keyp;
}

void keyp_free(Keypad *keyp)
{
    free(keyp);
}

void keyp_press_key(Keypad *keyp, SDL_Scancode scancode, bool is_pressed)
{
    switch (scancode)
    {
    case SDL_SCANCODE_X:
        keyp->keys[0].pressed = is_pressed;
        break;
    case SDL_SCANCODE_1:
        keyp->keys[1].pressed = is_pressed;
        break;
    case SDL_SCANCODE_2:
        keyp->keys[2].pressed = is_pressed;
        break;
    case SDL_SCANCODE_3:
        keyp->keys[3].pressed = is_pressed;
        break;
    case SDL_SCANCODE_Q:
        keyp->keys[4].pressed = is_pressed;
        break;
    case SDL_SCANCODE_W:
        keyp->keys[5].pressed = is_pressed;
        break;
    case SDL_SCANCODE_E:
        keyp->keys[6].pressed = is_pressed;
        break;
    case SDL_SCANCODE_A:
        keyp->keys[7].pressed = is_pressed;
        break;
    case SDL_SCANCODE_S:
        keyp->keys[8].pressed = is_pressed;
        break;
    case SDL_SCANCODE_D:
        keyp->keys[9].pressed = is_pressed;
        break;
    case SDL_SCANCODE_Z:
        keyp->keys[0xA].pressed = is_pressed;
        break;
    case SDL_SCANCODE_C:
        keyp->keys[0xB].pressed = is_pressed;
        break;
    case SDL_SCANCODE_4:
        keyp->keys[0xC].pressed = is_pressed;
        break;
    case SDL_SCANCODE_R:
        keyp->keys[0xD].pressed = is_pressed;
        break;
    case SDL_SCANCODE_F:
        keyp->keys[0xE].pressed = is_pressed;
        break;
    case SDL_SCANCODE_V:
        keyp->keys[0xF].pressed = is_pressed;
        break;
    default:
        break;
    }
}

uint8_t keyp_get_pressed_key(Keypad *keyp)
{
    for (int i = 0; i < 16; i++)
    {
        if (keyp->keys[i].pressed)
        {
            return (uint8_t)i;
        }
    }
    return 0xFF;
}
