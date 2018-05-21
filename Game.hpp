//
//  Game.hpp
//  Snake
//
//  Created by X on 16.05.18.
//

#ifndef Game_hpp
#define Game_hpp

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "Sprites.hpp"
#include "Surfaces.hpp"

class Game {
public:
    Game(SDL_Point playground_top_left,
         SDL_Point playground_dimensions,
         SDL_Point score_top_left,
         SDL_Point score_dimensions,
         int tile_size);
    bool Initialise();
    void RunLoop();
    void Shoutdown();
    
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void RenderPlayground(SDL_Renderer *renderer);
    void RenderScore(SDL_Renderer *renderer);
    
    bool mIsRunning;
    Uint32 mDelay;
    Uint32 mTicksCount;

    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
    TTF_Font *mFont;
    
    Playground mPlayground;
    Score mScore;
    Snake mSnake;
    Pellet mPellet;
};


#endif /* Game_hpp */
