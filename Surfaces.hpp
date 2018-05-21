//
//  Surfaces.hpp
//  Snake
//
//  Created by X on 19.05.18.
//

#ifndef Surfaces_hpp
#define Surfaces_hpp

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"


//-------------------- Playground class

class Playground {
public:
    Playground(SDL_Point top_left,
               SDL_Point dimensions,
               Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    void FreeSurfaces();
    void Clear();
    SDL_Surface *GetSurface();
    SDL_Point GetTopLeft();
    SDL_Point GetSurfaceDims();
private:
    SDL_Point mTopLeft;
    SDL_Point mSurfaceDims;
    SDL_Surface *mSurface;
};



//-------------------- Score class

class Score {
public:
    Score(SDL_Point top_left,
          SDL_Point dimensions,
          Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    void FreeSurfaces();
    void Update(int gained_score, TTF_Font *font);
    SDL_Surface *GetSurface();
    SDL_Point GetTopLeft();
    SDL_Point GetSurfaceDims();
private:
    SDL_Point mTopLeft;
    SDL_Point mSurfaceDims;
    SDL_Surface *mSurface;
    int mScore;
};

#endif /* Surfaces_hpp */
