#include <device/vga.h>

#define SCREEN_W 800
#define SCREEN_H 600

void VGA::InitScreen()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        assert(0);
        return;
    }

    SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &renderer);
    if (!window)
    {
        SDL_Quit();
        assert(0);
        return;
    }
    SDL_SetWindowTitle(window, "CEMU");
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STATIC, SCREEN_W, SCREEN_H);
}

void VGA::Init()
{
    InitScreen();
}

void VGA::Callback(uint32_t, int, bool)
{
}
