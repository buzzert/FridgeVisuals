#ifndef ACTOR_H
#define ACTOR_H

#include "SDL2/SDL.h"

struct Vector
{
    float x;
    float y;

    Vector(int x, int y) : x(x), y(y) {};
};

struct Rect
{
    float x;
    float y;

    int width;
    int height;

    Rect() : x(0), y(0), width(0), height(0) {};
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {};
    SDL_Rect ToSDLRect() {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;

        return rect;
    };
};

class Actor
{
public:
    Actor(Rect rect);

    virtual void Render(SDL_Renderer *renderer) {};

    Rect rect;
    Vector velocity;
    float alpha;
};

#endif
