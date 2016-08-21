#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <libgen.h>

#include "main_scene.h"

void print_usage(char *progname)
{
    printf("%s by buzzert 2016\n", progname);
    printf("usage: %s width height [-w]\n", progname);
    printf("\t-w : windowed mode\n");
}

int main(int argc, char **argv)
{
    int canvasWidth = 64;
    int canvasHeight = 160;
    bool windowed = false;

    if (argc < 3) {
        print_usage(basename(argv[0]));
        return 1;
    }

    for (unsigned int i = 1; i < argc; i++) {
        switch (i) {
        case 1:
            canvasWidth = atoi(argv[i]);
            break;
        case 2:
            canvasHeight = atoi(argv[i]);
            break;
        case 3:
            if (strcmp("-w", argv[i]) == 0) {
                windowed = true;
            }
            break;
        }
    }

    Rect canvasRect(0, 0, canvasWidth, canvasHeight);

    MainScene mainScene(canvasRect, windowed);

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
