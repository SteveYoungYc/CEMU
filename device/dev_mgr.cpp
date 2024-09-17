#include <SDL2/SDL.h>
#include <simulator.h>
#include <device/dev_mgr.h>

#define TIMER_HZ 60

void DeviceManager::Init()
{
    serial.Init();
    timer.Init();
    keyboard.Init();
}

void DeviceManager::Update()
{
    static uint64_t last = 0;
    uint64_t now = GetTime();
    if (now - last < 1000000 / TIMER_HZ)
    {
        return;
    }
    last = now;

    //   IFDEF(CONFIG_HAS_VGA, vga_update_screen());

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            simulator.simStatus.status = CEMU_Status::QUIT;
            break;

        // If a key was pressed
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            uint8_t k = event.key.keysym.scancode;
            bool is_keydown = (event.key.type == SDL_KEYDOWN);
            send_key(k, is_keydown);
            break;
        }
        default:
            break;
        }
    }
}
