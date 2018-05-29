//
//  Surfaces.cpp
//  Snake
//
//  Created by X on 19.05.18.
//

#include "Surfaces.hpp"
#include <string>

//-------------------- Playground class

Playground::Playground(SDL_Point top_left, SDL_Point dimensions, Uint32 pixel_format)
:mTopLeft(top_left)
,mSurfaceDims(dimensions)
,mSurface(nullptr) {
    mSurface = SDL_CreateRGBSurfaceWithFormat(0, mSurfaceDims.x, mSurfaceDims.y, 32, pixel_format);
    if (mSurface == nullptr) {SDL_Log("Failed to create playground surface: %s", SDL_GetError());}
    SDL_FillRect(mSurface, nullptr, SDL_MapRGBA(mSurface->format, 0, 0, 180, 255));
}


void Playground::FreeSurfaces() {SDL_FreeSurface(mSurface);}


void Playground::Clear() {
    SDL_FillRect(mSurface, nullptr, SDL_MapRGBA(mSurface->format, 0, 0, 180, 255));
}


// Getters
SDL_Surface *Playground::GetSurface() const {return mSurface;}
SDL_Point Playground::GetTopLeft() const {return mTopLeft;}
SDL_Point Playground::GetSurfaceDims() const {return mSurfaceDims;}



//-------------------- Score class

Score::Score(SDL_Point top_left, SDL_Point dimensions, Uint32 pixel_format)
:mTopLeft(top_left)
,mSurfaceDims(dimensions)
,mSurface(nullptr)
,mScore(0) {
    mSurface = SDL_CreateRGBSurfaceWithFormat(0, mSurfaceDims.x, mSurfaceDims.y, 32, pixel_format);
    if (mSurface == nullptr) {SDL_Log("Failed to create score surface: %s", SDL_GetError());}
    SDL_FillRect(mSurface, nullptr, SDL_MapRGBA(mSurface->format, 250, 250, 250, 255));
}


void Score::FreeSurfaces() {SDL_FreeSurface(mSurface);}


void Score::Update(int gained_score, TTF_Font *font) {
    mScore += gained_score;
    SDL_FillRect(mSurface, nullptr, SDL_MapRGBA(mSurface->format, 79, 91, 102, 255));
    
    std::string textToRender = "Score: "+std::to_string(mScore);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, textToRender.c_str(), textColor);
    SDL_Rect dstRect = {mSurfaceDims.x/3, 0, mSurfaceDims.y, (1/3)*mSurfaceDims.x};
    SDL_BlitSurface(textSurface, nullptr, mSurface, &dstRect);
}


// Getters
SDL_Surface *Score::GetSurface() const {return mSurface;}
SDL_Point Score::GetTopLeft() const {return mTopLeft;}
SDL_Point Score::GetSurfaceDims() const {return mSurfaceDims;}
