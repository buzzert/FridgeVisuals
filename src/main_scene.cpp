#include "main_scene.h"

#include "SDL2/SDL.h"
#include "texture_actor.h"

#include <iostream>
#include <sstream>
#include <random>

using namespace std;

typedef shared_ptr<Actor> actor_ptr;

static int uniform_random(int low, int high)
{
    static random_device rd;
    static mt19937 gen(rd());

    uniform_int_distribution<int> dis(low, high);
    return dis(gen);
}

MainScene::MainScene(Rect canvasRect, bool windowed)
    : _canvasRect(canvasRect)
{
    int flags = windowed ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN_DESKTOP;

    _window = SDL_CreateWindow("Cosmo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, canvasRect.width, canvasRect.height, flags);
    if (_window) {
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }

    // Initialize texture cache
    vector<string> textures = {
        "firework1.png",
        "firework2.png",
        "firework3.png",

        "sparkle1.png",
        "sparkle2.png",
        "sparkle3.png",
        "sparkle4.png"
    };

    for (string texture : textures) {
        _textureCache[texture] = TextureForRes(texture, _renderer);
    }

    _lastSparkleUpdate = chrono::high_resolution_clock::now();
    _lastFireworkUpdate = chrono::high_resolution_clock::now();
}

MainScene::~MainScene()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

bool MainScene::_ActorIsOffscreen(Actor actor)
{
    return (
        ((actor.rect.x + actor.rect.width) < 0) ||     // offscreen left
        ((actor.rect.y + actor.rect.height) < 0) ||    // offscreen top
        (actor.rect.x > _canvasRect.width) ||          // offscreen right
        (actor.rect.y > _canvasRect.height)            // offscreen bottom
    );
}

static inline chrono::duration<int, std::milli> since(update_time timePoint)
{
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(now - timePoint);
}

void MainScene::_CreateFirework()
{
    auto sinceLastUpdate = since(_lastFireworkUpdate);

    if ( sinceLastUpdate.count() > uniform_random(500, 3800) ) {
        _lastFireworkUpdate = chrono::high_resolution_clock::now();

        const float kWidthMargin = 125.0;
        const float kHeightMargin = 240.0;

        int fireworkKind = uniform_random(1, 3);
        char fireworkName[] = "fireworkN.png";
        sprintf(fireworkName, "firework%d.png", fireworkKind);

        actor_ptr firework(new TextureActor(_textureCache[fireworkName]));
        firework->rect.x = -kWidthMargin + uniform_random(0, _canvasRect.width + (int)kWidthMargin / 2);
        firework->rect.y = -kHeightMargin + uniform_random(0, _canvasRect.height + (int)kHeightMargin / 2);

        _fireworks.push_back(firework);
    }
}

void MainScene::_CreateSparkle()
{
    auto sinceLastUpdate = since(_lastSparkleUpdate);

    if ( sinceLastUpdate.count() > uniform_random(50, 400) ) {
        _lastSparkleUpdate = chrono::high_resolution_clock::now();

        bool reversed = (uniform_random(0, 1) == 0);
        const float yPosition = uniform_random(-10, _canvasRect.height);

        int sparkleKind = uniform_random(1, 4);
        char sparkleName[] = "sparkleN.png";
        sprintf(sparkleName, "sparkle%d.png", sparkleKind);

        actor_ptr sparkle(new TextureActor(_textureCache[sparkleName]));

        if (sparkleKind == 1 || sparkleKind == 2) {
            sparkle->rect.y = _canvasRect.height;
            sparkle->velocity.y = -1 * (float)(uniform_random(10, 100)) / 100;
            sparkle->rect.x = uniform_random(0, _canvasRect.width);
        } else {
            sparkle->rect.x = reversed ? _canvasRect.width : 0;
            sparkle->rect.y = yPosition;
            sparkle->velocity.x = (reversed ? -1 : 1) * (float)uniform_random(50, 100) / 100;
            sparkle->velocity.y = (float)uniform_random(-50, 150) / 100;
        }

        _sparkles.push_back(sparkle);
    }
}

void MainScene::Update()
{
    _CreateFirework();
    _CreateSparkle();

    for (auto it = _fireworks.begin(); it != _fireworks.end();) {
        actor_ptr firework = *it;

        firework->alpha -= 0.005;

        // Cleanup
        if (firework->alpha <= 0.0) {
            it = _fireworks.erase(it);
        } else {
            it++;
        }
    }

    for (auto it = _sparkles.begin(); it != _sparkles.end();) {
        actor_ptr sparkle = *it;

        sparkle->rect.x += sparkle->velocity.x;
        sparkle->rect.y += sparkle->velocity.y;

        // Cleanup
        if (_ActorIsOffscreen(*sparkle)) {
            it = _sparkles.erase(it);
        } else {
            it++;
        }
    }
}


void MainScene::Render()
{
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(_renderer);

    for (actor_ptr &firework : _fireworks) {
        firework->Render(_renderer);
    }

    for (actor_ptr &sparkle : _sparkles) {
        sparkle->Render(_renderer);
    }

    SDL_RenderPresent(_renderer);
}
