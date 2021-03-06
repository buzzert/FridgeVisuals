#ifndef TEXTURE_ACTOR_H
#define TEXTURE_ACTOR_H

#include "actor.h"

#include <string>
#include <memory>

std::shared_ptr<SDL_Texture> TextureForRes(std::string const &resourceName, SDL_Renderer *renderer);

class TextureActor : public Actor
{
public:
    TextureActor(std::shared_ptr<SDL_Texture> texture);

    virtual void Render(SDL_Renderer *renderer);

    std::shared_ptr<SDL_Texture> texture;
};

#endif
