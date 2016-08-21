#include "main_scene.h"

#include "SDL2/SDL.h"
#include "texture_actor.h"

#include <iostream>
#include <sstream>

using namespace std;

typedef shared_ptr<Actor> actor_ptr;

MainScene::MainScene(Rect canvasRect)
    : _canvasRect(canvasRect)
{
    _window = SDL_CreateWindow("Cosmo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, canvasRect.width, canvasRect.height, SDL_WINDOW_SHOWN);
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

    if ( sinceLastUpdate.count() > (500 + (arc4random() % 3800)) ) {
        _lastFireworkUpdate = chrono::high_resolution_clock::now();

        const float kWidthMargin = 125.0;
        const float kHeightMargin = 240.0;

        int fireworkKind = 1 + arc4random_uniform(3);
        char *fireworkName = (char *)malloc(strlen("fireworkN.png"));
        sprintf(fireworkName, "firework%d.png", fireworkKind);

        actor_ptr firework(new TextureActor(_textureCache[fireworkName]));
        firework->rect.x = -kWidthMargin + (arc4random() % (_canvasRect.width + (int)kWidthMargin / 2));
        firework->rect.y = -kHeightMargin + (arc4random() % (_canvasRect.height + (int)kHeightMargin / 2));

        _fireworks.push_back(firework);
    }
}

void MainScene::_CreateSparkle()
{
    auto sinceLastUpdate = since(_lastSparkleUpdate);

    if ( sinceLastUpdate.count() > (50 + (arc4random() % 400) ) ) {
        _lastSparkleUpdate = chrono::high_resolution_clock::now();

        bool reversed = (arc4random_uniform(2) == 0);
        const float yPosition = -10.0 + (arc4random() % _canvasRect.height);

        int sparkleKind = 1 + arc4random_uniform(4);
        char sparkleName[] = "sparkleN.png";
        sprintf(sparkleName, "sparkle%d.png", sparkleKind);

        actor_ptr sparkle(new TextureActor(_textureCache[sparkleName]));

        if (sparkleKind == 1 || sparkleKind == 2) {
            sparkle->rect.y = _canvasRect.height;
            sparkle->velocity.y = -1 * (float)(arc4random_uniform(100)) / 100;
            sparkle->rect.x = arc4random_uniform(_canvasRect.width);
        } else {
            sparkle->rect.x = reversed ? _canvasRect.width : 0;
            sparkle->rect.y = yPosition;
            sparkle->velocity.x = (reversed ? -1 : 1) * (float)(50 + (int)arc4random_uniform(100)) / 100;
            sparkle->velocity.y = (float)(-50 + (int)arc4random_uniform(150)) / 100;
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
