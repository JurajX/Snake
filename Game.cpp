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
:mIsRunning(true)
,mDelay(130)         // delay in ms between frames
,mTicksCount(0)
,mWindow(nullptr)
,mRenderer(nullptr)
,mFont(nullptr)
,mPlayground(playground_top_left, playground_dimensions)
,mScore(score_top_left, score_dimensions)
,mSnake(mPlayground.GetSurfaceDims(), tile_size)
,mPellet(mPlayground.GetSurfaceDims(), tile_size) {}


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
    
    Uint32 rndrFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    mRenderer = SDL_CreateRenderer(mWindow, -1, rndrFlags);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;}
    
    int imgFlags = IMG_INIT_PNG; //| IMG_INIT_JPG | IMG_INIT_TIF;
    int imgInit = IMG_Init(imgFlags);
    if ((imgInit&imgFlags) != imgFlags) {
        SDL_Log("Failed to initialise SDL_image: %s", IMG_GetError());
        return false;
    }
    
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
// TODO: (wlog snake moves up): quickly presses right or left and then down the snake changes direction from up to down and bites itself -> gameover! The event queue needs to be cleared after changing snake's direction.
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
                    while(SDL_PollEvent(&event)); // a terrible way to clear event queue
                    break;
                case SDLK_DOWN:
                    mSnake.SetNewDirection(Direction::DOWN);
                    while(SDL_PollEvent(&event)); // a terrible way to clear event queue
                    break;
                case SDLK_RIGHT:
                    mSnake.SetNewDirection(Direction::RIGHT);
                    while(SDL_PollEvent(&event)); // a terrible way to clear event queue
                    break;
                case SDLK_LEFT:
                    mSnake.SetNewDirection(Direction::LEFT);
                    while(SDL_PollEvent(&event)); // a terrible way to clear event queue
                    break;
            }
        }
    }
}


void Game::UpdateGame() {
    // Wait untill the previous call to UpdateGame and this one is >= mDelay.
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + mDelay));
    mTicksCount = SDL_GetTicks();
    
    int scoreGain = mSnake.Update(mPellet.GetTopLeft());
    if (mSnake.SelfColision()) {mIsRunning = false;}
    while (mSnake.ColidesWithPoint(mPellet.GetTopLeft())) { mPellet.MoveRandomly();}

    mScore.Update(scoreGain, mFont);
    if (scoreGain != 0) {mDelay -= 2;}
    mPlayground.Clear();
    mPellet.BlitOnPlayground(mPlayground.GetSurface());
    mSnake.BlitOnPlayground(mPlayground.GetSurface());
}


void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);
    
    this->RenderPlayground();
    this->RenderScore();
    
    SDL_RenderPresent(mRenderer);
}


void Game::RenderPlayground() {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, mPlayground.GetSurface());
    SDL_Rect dstRect = {
        mPlayground.GetTopLeft().x,
        mPlayground.GetTopLeft().y,
        mPlayground.GetSurfaceDims().x,
        mPlayground.GetSurfaceDims().y};
    SDL_RenderCopy(mRenderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
    texture = nullptr;
}


void Game::RenderScore() {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, mScore.GetSurface());
    SDL_Rect dstRect = {
        mScore.GetTopLeft().x,
        mScore.GetTopLeft().y,
        mScore.GetSurfaceDims().x,
        mScore.GetSurfaceDims().y};
    SDL_RenderCopy(mRenderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
    texture = nullptr;
}


void Game::Shoutdown() {
    TTF_CloseFont(mFont);
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
    SDL_Quit();
}
