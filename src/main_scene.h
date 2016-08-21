#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "actor.h"

#include <vector>
#include <map>
#include <chrono>

typedef std::chrono::high_resolution_clock::time_point update_time;

class MainScene
{
public:
    MainScene(Rect canvasRect);
    ~MainScene();

    void Update();
    void Render();

private:
    void _CreateSparkle();
    void _CreateFirework();

    bool _ActorIsOffscreen(Actor actor);

    SDL_Window   *_window;
    SDL_Renderer *_renderer;

    Rect _canvasRect;

    update_time _lastSparkleUpdate;
    std::vector<std::shared_ptr<Actor>> _sparkles;

    update_time _lastFireworkUpdate;
    std::vector<std::shared_ptr<Actor>> _fireworks;

    std::map<std::string, std::shared_ptr<SDL_Texture>> _textureCache;
};

#endif
