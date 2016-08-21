#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "main_scene.h"

int main(int argc, char **argv)
{
    Rect canvasRect(0, 0, 64, 160);

    MainScene mainScene(canvasRect);

    bool running = true;
    while (running) {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) != 0 ) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        mainScene.Update();
        mainScene.Render();
    }

    return 0;
}
