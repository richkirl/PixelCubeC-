//g++ -std=c++23 -O2 main.cpp -lSDL2 -lSDL2main -lSDL2_mixer
#include <cstdlib>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <chrono>

/*
 * 
 */
void pixel(std::vector<SDL_FPoint> *points, float x, float y) {
    points->emplace_back(x, y);
}

void clear(std::vector<SDL_FPoint> *points) {
    points->clear();
}

struct vec3 {
    float x, y, z;
};

void rotate(float dt, vec3& p, float x = 1, float y = 1, float z = 1) {
    float rad = 0;
    rad = x;
    p.y = std::cos(rad) * p.y - std::sin(rad) * p.z;
    p.z = std::sin(rad) * p.y + std::cos(rad) * p.z;

    rad = y;
    p.x = std::cos(rad) * p.x + std::sin(rad) * p.z;
  
    p.z = -std::sin(rad) * p.x + std::cos(rad) * p.z;


    rad = z;
    p.x = std::cos(rad) * p.x - std::sin(rad) * p.y;

    p.y = std::sin(rad) * p.x + std::cos(rad) * p.y;


}

void show(std::vector<SDL_FPoint> points,SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto& point : points) {
        SDL_RenderDrawPointF(renderer, point.x, point.y);
    }
    SDL_RenderPresent(renderer);
}

void input(SDL_Event e) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {

            Mix_CloseAudio();

            Mix_Quit();


            SDL_Quit();
        
        }
    }
}

int main() {


    SDL_Window* win = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event e;
    std::vector<SDL_FPoint> points;
    Mix_Music *music = NULL;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "ERROR INIT!\n";
        return -1;
    }

    SDL_CreateWindowAndRenderer(800 * 2, 600 * 2, 0, &win, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    Mix_Init(MIX_INIT_MP3);
    music = Mix_LoadMUS("/home/jimmy/tests/Sound/te/out1.mp3");

    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Mix_SetMusicCMD(SDL_getenv("MUSIC_CMD"));
    Mix_FadeInMusic(music, 0, 2000);

    std::vector<vec3>point{
        {300, 300, 300},
        {800, 300, 300},
        {800, 800, 300},
        {300, 800, 300},

        {300, 300, -200},
        {800, 300, -200},
        {800, 800, -200},
        {300, 800, -200},};
        srand(time(NULL));
    for (int var = 0; var < 32212; ++var) {
        vec3 x;
        x.x = (rand() % 500) + 300;
        x.y = (rand() % 500) + 300;
        x.z = (rand() % 500) -200;
        point.push_back(x);
    }
    vec3 c{-1, -1, -100};
    for (auto& p : point) {
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }
    c.x /= point.size();
    c.y /= point.size();
    c.z /= point.size();




    auto tp1 = std::chrono::system_clock::now();


    while ((Mix_PlayingMusic() || Mix_PausedMusic() || Mix_HaltMusic())) {
        {
            auto tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tp2 - tp1;
            tp1 = tp2;
            float fElapsedTime = elapsedTime.count();
            for (auto& p : point) {
                //p.z++;
                p.x -= c.x;
                p.y -= c.y;
                p.z -= c.z;
                rotate(fElapsedTime, p, 0.004, 0.004, 0.0001);
                p.x += c.x;
                p.y += c.y;
                p.z += c.z;
            }

            for (auto& p : point) {
                
                pixel(&points, p.x, p.y);
            }
            show(points,renderer);

            clear(&points);
            input(e);
            SDL_Delay(30);
        }
    }
    Mix_FreeMusic(music);
    return 0;
}
