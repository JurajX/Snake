//
//  Game.cpp
//  Snake
//
//  Created by X on 16.05.18.
//

#include "Game.hpp"
#include <algorithm>

#include <iostream>



Game::Game(SDL_Point playground_top_left,
           SDL_Point playground_dimensions,
           SDL_Point score_top_left,
           SDL_Point score_dimensions,
           int tile_size)
:mWindow(nullptr)
,mRenderer(nullptr)
,mFont(nullptr)
,mIsRunning(true)
,mDelay(100)         // delay in ms between frames
,mTicksCount(0)
,mPlayground(Playground(playground_top_left, playground_dimensions))
,mScore(Score(score_top_left, score_dimensions))
,mSnake(Snake(mPlayground.GetSurfaceDims(), tile_size))
,mPellet(Pellet(mPlayground.GetSurfaceDims(), tile_size)) {}


bool Game::Initialise() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
        return false;}
    
    int WIDTH = mScore.GetSurfaceDims().x;       // score_width          // ________________
    int HEIGHT =  mPlayground.GetTopLeft().y     // score_height+border  // |    score     |
                 +mPlayground.GetSurfaceDims().y // playground_height    // ================
                 +mPlayground.GetTopLeft().x;    // border               // || playground ||
                                                                         // ================
    
    mWindow = SDL_CreateWindow("Snake Game", 330, 50, WIDTH, HEIGHT, 0);
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;}
    
    mRenderer = SDL_CreateRenderer(mWindow, -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;}
    
    if (TTF_Init()==-1) {
        SDL_Log("Failed to initialise SDL_ttf: %s", TTF_GetError());
        return false;
    }

    mFont = TTF_OpenFont("Fonts/actionj.ttf", mScore.GetSurfaceDims().y);
    if (mFont == nullptr) {
        SDL_Log("Failed to open font: %s", TTF_GetError());
        return false;
    }
        
    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
// Bug to fix (wlog snake moves up): if player quickly presses right/left and then down the snale roles over (bites) itself -> gameover!
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            mIsRunning = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym ) {
                case SDLK_ESCAPE:
                    mIsRunning = false;
                    break;
                case SDLK_UP:
                    mSnake.SetNewDirection(Direction::UP);
                    while(SDL_PollEvent(&event)); // a terrible way to fix the above bug
                    break;
                case SDLK_DOWN:
                    mSnake.SetNewDirection(Direction::DOWN);
                    while(SDL_PollEvent(&event)); // a terrible way to fix the above bug
                    break;
                case SDLK_RIGHT:
                    mSnake.SetNewDirection(Direction::RIGHT);
                    while(SDL_PollEvent(&event)); // a terrible way to fix the above bug
                    break;
                case SDLK_LEFT:
                    mSnake.SetNewDirection(Direction::LEFT);
                    while(SDL_PollEvent(&event)); // a terrible way to fix the above bug
                    break;
            }
        }
    }
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + mDelay));
    mTicksCount = SDL_GetTicks();
    
    int scoreGain = mSnake.Update(mPellet.GetTopLeft());
    if (mSnake.SelfColision()) {mIsRunning = false;}
    while (mSnake.ColidesWithSnake(mPellet.GetTopLeft())) { mPellet.MoveRandomly();}

    mScore.Update(scoreGain, mFont);
    mPlayground.Clear();
    mPellet.BlitOnPlayground(mPlayground.GetSurface());
    mSnake.BlitOnPlayground(mPlayground.GetSurface());
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);
    
    this->RenderPlayground(mRenderer);
    this->RenderScore(mRenderer);
    
    SDL_RenderPresent(mRenderer);
}

void Game::RenderPlayground(SDL_Renderer *renderer) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, mPlayground.GetSurface());
    SDL_Rect dstRect = {
        mPlayground.GetTopLeft().x,
        mPlayground.GetTopLeft().y,
        mPlayground.GetSurfaceDims().x,
        mPlayground.GetSurfaceDims().y};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Game::RenderScore(SDL_Renderer *renderer) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, mScore.GetSurface());
    SDL_Rect dstRect = {
        mScore.GetTopLeft().x,
        mScore.GetTopLeft().y,
        mScore.GetSurfaceDims().x,
        mScore.GetSurfaceDims().y};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Game::Shoutdown() {
    TTF_CloseFont(mFont);
    // TODO destroy TTF
    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
    SDL_Quit();
}
