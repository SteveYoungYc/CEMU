#ifndef __VGA_H__
#define __VGA_H__

#include <device.h>
#include <SDL2/SDL.h>

#define VGA_FBMEM_OFFSET    0x1000000
#define VGA_CTRL_OFFSET     0x0000100

class VGA : public Device
{
private:
    SDL_Window* window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;

    void InitScreen();

public:
    void Init() override;
    void Callback(uint32_t, int, bool) override;
};

#endif
