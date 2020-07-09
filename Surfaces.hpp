//
//  Surfaces.hpp
//  Snake
//
//  Created by X on 19.05.18.
//

#ifndef Surfaces_hpp
#define Surfaces_hpp

#include "SDL.h"
#include "SDL_ttf.h"


//-------------------- Playground class

class Playground {
public:
    Playground(SDL_Point top_left,
               SDL_Point dimensions,
               Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    Playground(const Playground&) = delete;
    Playground& operator=(const Playground&) = delete;
    Playground(Playground&&) = delete;
    Playground& operator=(Playground&&) = delete;
    ~Playground();

    void Clear();
    SDL_Surface *GetSurface() const;
    SDL_Point GetTopLeft() const;
    SDL_Point GetSurfaceDims() const;
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
    Score(const Score&) = delete;
    Score& operator=(const Score&) = delete;
    Score(Score&&) = delete;
    Score& operator=(Score&&) = delete;
    ~Score();

    void Update(int gained_score, TTF_Font *font);
    SDL_Surface *GetSurface() const;
    SDL_Point GetTopLeft() const;
    SDL_Point GetSurfaceDims() const;
private:
    SDL_Point mTopLeft;
    SDL_Point mSurfaceDims;
    SDL_Surface *mSurface;
    int mScore;
};

#endif /* Surfaces_hpp */
